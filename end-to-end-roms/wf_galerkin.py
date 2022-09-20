#!/usr/bin/env python

#==============================================================
# imports
#==============================================================

import sys, os, importlib, copy, git, time
from argparse import ArgumentParser
from pathlib import Path
from scipy import linalg as scipyla

# local
from all import *

#==============================================================
# functions
#==============================================================
def _validate_wf_galerkin_section(wfDic, customModule):
  assert 'galerkinRom' in wfDic, "galerkinRom section not found"
  subDic = wfDic['galerkinRom']


def compute_hypreducer_matrix_operator(outDir, offlineRomDir, \
                                       numStateModes, sampleMeshDir, \
                                       numDofsPerCell, dryRun):

  myFullPhiFile = offlineRomDir + "/state_left_singular_vectors.bin"
  myFile2 = sampleMeshDir + "/sample_mesh_gids.txt"
  fullRhsPodFile = offlineRomDir + "/rhs_left_singular_vectors.bin"
  K = numStateModes*3 + 1

  if not dryRun:
    # load my full phi
    myFullPhi     = load_pod_basis(myFullPhiFile)[:,0:numStateModes]

    # indexing info
    mySmMeshGids = np.loadtxt(myFile2, dtype=int)
    mySmCount    = len(mySmMeshGids)

    if mySmCount*numDofsPerCell < K:
      print("Cannot have K > mySmCount*numDofsPerCell, using K")
      K = mySmCount*numDofsPerCell - 1

    # K should be larger than numStateModes
    if K < numStateModes:
      print("Cannot have K < myNumStatePodModes, using K")
      K = numStateModes + 1

    theta = load_pod_basis(fullRhsPodFile)[:,0:K]
    slicedTheta = np.zeros((mySmCount*numDofsPerCell, theta.shape[1]), order='F')
    for j in range(numDofsPerCell):
      slicedTheta[j::numDofsPerCell, :] = theta[numDofsPerCell*mySmMeshGids + j, :]

    A = myFullPhi.transpose() @ theta
    hypRedOp = A @ scipyla.pinv(slicedTheta)
    print(" hyperReductionOperatorShape = {}".format(hypRedOp.shape))

    numRows = np.int64(hypRedOp.shape[1])
    numCols = np.int64(hypRedOp.shape[0])
    print("numRows = ", numRows)
    print("numCols = ", numCols)
    fileo = open(outDir+'/hyperReductionOperator.bin', "wb")
    np.array([numRows]).tofile(fileo)
    np.array([numCols]).tofile(fileo)
    hypRedOp.tofile(fileo)
    fileo.close()
    #np.savetxt(outDir+'/hyperReductionOperator.txt', hypRedOp.T)


def _run_single_rom(romDir, offlineRomDir, numModes, inputsDic, \
                    fomRunDir, algoString, dryRun,
                    sampleMeshDir = None, hypRedOpDir = None):

  if os.path.exists(romDir):
    print('{} already exists'.format(romDir))
  else:
    print("Running galerkin rom in {}".format(os.path.basename(romDir)))
    os.system('mkdir -p ' + romDir)

    # write some info to run directory
    f = open(romDir+"/rom_dofs_count.txt", "w")
    f.write(str(numModes))
    f.close()

    romSpecifics = {}

    # full mesh pod
    fullMeshPodFile = offlineRomDir +"/state_left_singular_vectors.bin"
    romSpecifics['fullMeshPodFile'] = fullMeshPodFile

    romInitStateFile = romDir + "/rom_initial_state.txt"
    if not dryRun:
      # compute rom initial state using the basis
      myPhi = load_pod_basis(fullMeshPodFile)[:, 0:numModes]
      fomIc = load_fom_initial_condition(fomRunDir+"/initial_state.bin")
      romIc = np.dot(myPhi.transpose(), fomIc)
      np.savetxt(romInitStateFile, romIc)

      # deal with affine shift
      np.savetxt(romDir+"/optional_affine_shift.txt", np.zeros(len(fomIc)))
    romSpecifics['affineShiftFile'] = romDir +"/optional_affine_shift.txt"
    romSpecifics['isAffine'] = False

    romSpecifics['algorithm'] = algoString
    romSpecifics['numModes'] = numModes
    romSpecifics['romInitialStateFile'] = romInitStateFile

    if sampleMeshDir!= None and hypRedOpDir != None:
      inputsDic['meshDir'] = sampleMeshDir
      romSpecifics["galerkinHypRedOperatorFile"] = hypRedOpDir+"/hyperReductionOperator.bin"
      romSpecifics["stencilMeshGidsFile"] = sampleMeshDir +"/stencil_mesh_gids.dat"
      romSpecifics["sampleMeshGidsFile"]  = sampleMeshDir +"/sample_mesh_gids.txt"

    inputsDic['rom'] = romSpecifics
    # now we can write to file
    write_dic_to_yaml_file(romDir+"/input.yaml", inputsDic)

    if dryRun:
      print("dryrun: fom: {}".format(romDir))
    else:
      exeDirFullPath = str(Path(romDir).parent)
      link_if_needed_and_run_exe(romDir,  exeDirFullPath, cppExecutableName)


def _main_default_impl(workDir, romDic, dryRun=False):
  # location of the offline stuff
  offlineRomDir = workDir + "/offline_rom"

  #
  # outer loop over all truncations
  #
  truncationDic = romDic['podTruncation']
  for truncPolicy, truncValues in truncationDic.items():
    print (truncPolicy, truncValues)

    if (truncPolicy.lower() == "energybased"):
      for energy in truncValues:
        if dryRun:
          numModes = 22
        else:
          singValues = np.loadtxt(offlineRomDir+'/state_singular_values.txt')
          numModes = compute_cumulative_energy(singValues, energy)
          print(numModes)

        #
        # for a given num of modes, find all FOM test runs
        # inside workDir and loop over each and run a corresponding ROM
        #
        for fomTestDir in find_all_fom_test_dirs(workDir):
          # read the yaml file used for that FOM run
          fomInputs = read_yaml_file(fomTestDir + "/input.yaml")
          print(fomInputs)

          # extract from fom dir the identifier so that
          # we know which test point it refers to
          runId = get_run_id(fomTestDir)

          # make name of the output rom directory
          romDir = workDir + "/default_galerkin_truncation_energybased"
          romDir += "_"+str(energy)
          romDir += "_runid_"+str(runId)
          _run_single_rom(romDir, offlineRomDir, numModes, \
                          fomInputs, fomTestDir, "defaultGalerkin", dryRun)

# -------------------------------------------------------------------
def string_identifier_from_sample_mesh_dir(sampleMeshDir):
  if "sample_mesh_random" in sampleMeshDir:
    return "sample_mesh_random_"+sampleMeshDir[-5:]
  elif "sample_mesh_psampling" in sampleMeshDir:
    return "sample_mesh_psampling_"+sampleMeshDir[-5:]

# -------------------------------------------------------------------
def _main_hyperreduced_impl(workDir, romDic, numDofsPerCell, dryRun=False):
  # location of the offline stuff
  offlineRomDir = workDir + "/offline_rom"

  # find all sample meshes
  sampleMeshes = [offlineRomDir +'/'+d for d in os.listdir(offlineRomDir) \
                  if "sample_mesh_" in d[0:15]]
  print(sampleMeshes)
  assert len(sampleMeshes) >= 1, "sampleMeshes cannot be empty"
  for sampleMeshDir in sampleMeshes:
    strIdSm = string_identifier_from_sample_mesh_dir(sampleMeshDir)

    truncationDic = romDic['podTruncation']
    for truncPolicy, truncValues in truncationDic.items():
      print (truncPolicy, truncValues)
      if (truncPolicy.lower() == "energybased"):
        for energy in truncValues:
          if dryRun:
            numModes = 22
          else:
            singValues = np.loadtxt(offlineRomDir+'/state_singular_values.txt')
            numModes = compute_cumulative_energy(singValues, energy)
            print(numModes)

          s1 = workDir + "/hyperreducer"
          if energy != None:
            s2 = str(energy)
          sep = "_"
          hypRedPath = s1 + sep + s2 + sep + strIdSm
          print(hypRedPath)

          if os.path.exists(hypRedPath):
            logging.info('{} already exists'.format(os.path.basename(hypRedPath)))
          else:
            logging.info('Generating {}'.format(os.path.basename(hypRedPath)))
            os.system('mkdir -p ' + hypRedPath)

          compute_hypreducer_matrix_operator(hypRedPath, offlineRomDir, \
                                             numModes, sampleMeshDir, \
                                             numDofsPerCell, dryRun)

          for fomTestDir in find_all_fom_test_dirs(workDir):
            # read the yaml file used for that FOM run
            fomInputs = read_yaml_file(fomTestDir + "/input.yaml")

            # extract from fom dir the identifier so that
            # we know which test point it refers to
            runId = get_run_id(fomTestDir)

            # make name of the output rom directory
            romDir = workDir + "/hyperreduced_galerkin_truncation_energybased"
            romDir += "_"+str(energy)
            romDir += "_" + strIdSm
            romDir += "_runid_"+str(runId)
            print(romDir)
            _run_single_rom(romDir, offlineRomDir, numModes, fomInputs,\
                            fomTestDir, "hyperreducedGalerkin", dryRun,
                            sampleMeshDir = sampleMeshDir,\
                            hypRedOpDir = hypRedPath)


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
  moduleName = 'customizations_'+problemName
  sys.path.append(os.path.realpath(ptutRepoEndToEndFullPath+'/'+moduleName))
  print("Loading customizations from {}".format(moduleName))
  customModule = importlib.import_module(moduleName)

  # before moving on, ensure that workDir contains a unique FULL mesh.
  # If one wants to run for a different mesh, then they have to
  # run this script again with a different working directory
  fomMeshPath = find_full_mesh_and_ensure_unique(workDirFullPath)

  # ======================================================================

  # for this driver to be valid, we need to ensure this:
  _validate_wf_galerkin_section(wfDic, customModule)
  romDic = wfDic['galerkinRom']

  # figure out which galerkin we want and run
  if romDic['algorithm'].lower() == "defaultgalerkin":
    _main_default_impl(workDirFullPath, romDic, dryRun)
  elif romDic['algorithm'].lower() == "hyperreducedgalerkin":
    _main_hyperreduced_impl(workDirFullPath, romDic, customModule.numDofsPerCell, dryRun)
