#!/usr/bin/env python

#==============================================================
# imports
#==============================================================

import sys, os, importlib, copy, git, time, random
from argparse import ArgumentParser
from pathlib import Path
from scipy import linalg as scipyla

# local
from all import *

#==============================================================
# functions
#==============================================================

def _validate_wf_offlinerom_section(wfDic, customModule):
  assert 'offlineRom' in wfDic, "offlineRom section not found"
  subDic = wfDic['offlineRom']

  assert 'pod' in subDic, "cannot find subfield: " +field
  podDic = subDic['pod']

  assert 'stateData' in podDic, "stateData MUST be a subfield"
  statePodDic = podDic['stateData']
  assert 'useTrainingRuns' in statePodDic, "cannot find subfield: useTrainingRuns"
  #assert 'policy' in statePodDic, "cannot find subfield: policy"


# -------------------------------------------------------------------
def _default_snapshots_and_pod(outDir, customModule, dataDirs, \
                               fomMeshPath, stateDataOrRhsData):
  # collect some info that we need
  fomTotCells = find_total_cells_from_info_file(fomMeshPath)
  totFomDofs  = fomTotCells*customModule.numDofsPerCell

  if stateDataOrRhsData == "stateData":
    print("\n")
    print("-------- computing POD on state data --------")
    M = load_fom_state_snapshots_from_multiple_runs(dataDirs, totFomDofs,
                                                    customModule.numDofsPerCell,
                                                    False) # don't subtract IC
    write_matrix_to_bin_omit_shape(outDir+"/state_snapshots.bin", M)

    lsvFile = outDir + '/state_left_singular_vectors.bin'
    svaFile = outDir + '/state_singular_values.txt'
    # need to do SVD on M.T because M.T has the correct shape,
    # i.e. each column is an instance of state or rhs
    # so that the columns of M represent the snapshots taken
    do_svd(M.T, lsvFile, svaFile)

  elif stateDataOrRhsData == "rhsData":
    print("\n")
    print("-------- computing POD on RHS data --------")
    M = load_fom_rhs_snapshots_from_multiple_runs(dataDirs, totFomDofs,
                                                  customModule.numDofsPerCell)
    write_matrix_to_bin_omit_shape(outDir+"/rhs_snapshots.bin", M)
    lsvFile = outDir + '/rhs_left_singular_vectors.bin'
    svaFile = outDir + '/rhs_singular_values.txt'
    # need to do SVD on M.T because M.T has the correct shape,
    # i.e. each column is an instance of state or rhs
    # so that the columns of M represent the snapshots taken
    do_svd(M.T, lsvFile, svaFile)

  else:
    print("Invalid case = ", stateDataOrRhsData)

# -------------------------------------------------------------------
def make_sample_mesh_random(valueOrList, workDir, module, pdaDir, fomMeshPath):
  if type(valueOrList) != list:
    valueOrList = [valueOrList]

  for fractionOfCellsNeeded in valueOrList:
    outDir = workDir + "/sample_mesh_random_{:3.3f}".format(fractionOfCellsNeeded)
    if os.path.exists(outDir):
      print('{} already exists'.format(outDir))
    else:
      print('Generating random sample mesh in {}'.format(outDir))
      os.system('mkdir -p ' + outDir)

      fomNumCells = find_total_cells_from_info_file(fomMeshPath)
      sampleMeshCount = int(fomNumCells * fractionOfCellsNeeded)
      sample_mesh_gids = random.sample(range(0, fomNumCells), sampleMeshCount)
      sample_mesh_gids = np.sort(sample_mesh_gids)
      print(" numCellsFullDomain = {}".format(fomNumCells))
      print(" sampleMeshSize     = {}".format(sampleMeshCount))
      np.savetxt(outDir+'/sample_mesh_gids.txt', sample_mesh_gids, fmt='%8i')

      call_pda_to_make_sample_mesh(pdaDir, fomMeshPath, outDir)

# -------------------------------------------------------------------
def call_pda_to_make_sample_mesh(pdaDir, fomMeshPath, outDir):
   owd = os.getcwd()
   meshScriptsDir = pdaDir + "/meshing_scripts"
   args = ("python3", meshScriptsDir+'/create_sample_mesh.py',
           "--fullMeshDir", fomMeshPath,
           "--sampleMeshIndices", outDir+'/sample_mesh_gids.txt',
           "--outDir", outDir)
   popen  = subprocess.Popen(args, stdout=subprocess.PIPE);
   popen.wait()
   output = popen.stdout.read();
   print(output)

# -------------------------------------------------------------------
def make_sample_mesh_psampling(valueOrList, workDir, module, pdaDir, fomMeshPath):
  # ensure there is RHS pod modes available or psampling cannot be done
  rhsPodFile = workDir + '/rhs_left_singular_vectors.bin'
  assert os.path.exists(rhsPodFile), "cannot find rhs modes, so aborting psampling sample mesh"

  if type(valueOrList) != list:
    valueOrList = [valueOrList]

  for fractionOfCellsNeeded in valueOrList:
    outDir = workDir + "/sample_mesh_psampling_{:3.3f}".format(fractionOfCellsNeeded)
    if os.path.exists(outDir):
      print('{} already exists'.format(outDir))
    else:
      print('Generating psampling sample mesh in {}'.format(outDir))
      os.system('mkdir -p ' + outDir)

      fomNumCells = find_total_cells_from_info_file(fomMeshPath)
      sampleMeshCount = int(fomNumCells * fractionOfCellsNeeded)
      sample_mesh_gids = random.sample(range(0, fomNumCells), sampleMeshCount)
      sample_mesh_gids = np.sort(sample_mesh_gids)
      print(" numCellsFullDomain = {}".format(fomNumCells))
      print(" sampleMeshSize     = {}".format(sampleMeshCount))
      np.savetxt(outDir+'/sample_mesh_gids.txt', sample_mesh_gids, fmt='%8i')

      whichDofToUseForFindingCells = 0
      myRhsPod = load_pod_basis(rhsPodFile)[whichDofToUseForFindingCells::module.numDofsPerCell]

      if myRhsPod.shape[1] < sampleMeshCount:
        print("Warning: psampling sample mesh: not enough rhs modes, I am reducing sample mesh count")
        sampleMeshCount = myRhsPod.shape[1]-1

      Q,R,P = scipyla.qr(myRhsPod[:,0:sampleMeshCount].transpose(), pivoting=True)
      mySampleMeshGidsWrtFullMesh = np.sort(P[0:sampleMeshCount])
      np.savetxt(outDir+'/sample_mesh_gids_p_0.txt',\
                 mySampleMeshGidsWrtFullMesh, fmt='%8i')

      call_pda_to_make_sample_mesh(pdaDir, fomMeshPath, outDir)

#==============================================================
# main
#==============================================================
if __name__== "__main__":
  # parsing
  parser = ArgumentParser(add_help=False)
  add_dryrun_cmd_line_option(parser)
  add_wffile_cmd_line_option(parser)
  args = parser.parse_args()
  dryRun = args.dryRun
  wfFile = args.workFlowFile

  # set working directory to where we call this file from
  workDirFullPath = os.path.realpath(".")
  print("workingDir =", workDirFullPath)

  # ensure this is NEVER called directly from the tutorials repo
  # because we want to this to be called from within the build
  # directory of the tutorials or any other place
  # except the tutorials source.
  if is_git_repo(workDirFullPath):
    print("you cannot run this script from the tutorials repo!")
    sys.exit(23)

  # find full path to the top-level tutorials repo
  ptutRepoFullPath = str(os.path.split(os.path.dirname(__file__))[0])
  print("PTUT repo  =", ptutRepoFullPath)
  ptutRepoEndToEndFullPath = ptutRepoFullPath + '/end-to-end-roms'

  # read the full workflow dic
  wfDic = read_workflow_yaml_file(workDirFullPath+'/'+wfFile)
  assert_valid_and_supported_problem(wfDic, ptutRepoEndToEndFullPath)
  problemName = find_problem_name_from_wf_dic(wfDic)

  # import customizations from target problem
  # we can do this safely because we already asserted customization exists
  moduleName = problemName+'_customizations'
  sys.path.append(os.path.realpath(ptutRepoEndToEndFullPath+'/'+moduleName))
  print("Loading customizations from {}".format(moduleName))
  customModule = importlib.import_module(moduleName)

  # before moving on, ensure that workDir contains a unique FULL mesh.
  # If one wants to run for a different mesh, then they have to
  # run this script again with a different working directory
  fomMeshPath = find_full_mesh_and_ensure_unique(workDirFullPath)

  # for this driver to be valid, we need to ensure this:
  _validate_wf_offlinerom_section(wfDic, customModule)

  # figure out if offlineRom dir exists
  offlineRomDir = workDirFullPath + "/offline_rom"
  if os.path.exists(offlineRomDir):
    print('{} already exists'.format(offlineRomDir))

  else:
    os.system('mkdir -p ' + offlineRomDir)

    # -----------
    # do POD
    # -----------
    podDic = wfDic['offlineRom']['pod']
    write_dic_to_yaml_file(offlineRomDir +"/pod_input.yaml", podDic)
    for s in ['stateData', 'rhsData']:
      if s in podDic:
        # figure out which train runs need to read snapshots from
        trainRunsEntry = podDic[s]['useTrainingRuns']
        trainDirsToUse = None
        if trainRunsEntry == "all":
          # use all train runs in the workdir
          trainDirsToUse = [workDirFullPath+'/'+d for d in os.listdir(workDirFullPath) \
                            if "fom_train" in d]
        else:
          # if useTrainingRuns is found in the yaml file we use it
          trainRunIndices = podDic[s]['useTrainingRuns']
          trainDirsToUse = find_fom_train_dirs_for_target_set_of_indices(workDirFullPath, \
                                                                         trainRunIndices)
        if not dryRun:
          # figure out the policy
          # if missing, that means use default
          if 'policy' not in podDic[s]:
            _default_snapshots_and_pod(offlineRomDir, customModule, trainDirsToUse, fomMeshPath, s)
          else:
            if policy == "default":
              _default_snapshots_and_pod(offlineRomDir, customModule, trainDirsToUse, fomMeshPath, s)
            else:
              print("invalid or unsupported pod policy {}".format(policy))

    # -----------
    # do sample mesh
    # -----------
    pdaRepoFullPath = ptutRepoFullPath + '/tpls/pressio-demoapps'
    if "sampleMesh" in wfDic['offlineRom']:
      print("\n")
      print("-------- computing sample meshes --------")
      smDic = wfDic['offlineRom']['sampleMesh']
      for key, valueOrList in smDic.items():
        if key == "random":
          make_sample_mesh_random(valueOrList, offlineRomDir, \
                                  customModule, pdaRepoFullPath, \
                                  fomMeshPath)
        elif key == "psampling":
          make_sample_mesh_psampling(valueOrList, offlineRomDir, \
                                     customModule, pdaRepoFullPath, \
                                     fomMeshPath)
