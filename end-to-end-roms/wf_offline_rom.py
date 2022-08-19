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

def _validate_wf_offlinerom_section(wfDic, customModule):
  assert 'offlineRom' in wfDic, "offlineRom section not found"
  subDic = wfDic['offlineRom']

  assert 'pod' in subDic, "cannot find subfield: " +field
  podDic = subDic['pod']

  assert 'stateData' in podDic, "stateData MUST be a subfield"
  statePodDic = podDic['stateData']
  assert 'useTrainingRuns' in statePodDic, "cannot find subfield: useTrainingRuns"
  assert 'policy' in statePodDic, "cannot find subfield: policy"

# -------------------------------------------------------------------
def _do_svd(mymatrix, lsvFile, svaFile):
  timing = np.zeros(1)
  start = time.time()
  U,S,_ = scipyla.svd(mymatrix, full_matrices=False, lapack_driver='gesdd')
  end = time.time()
  elapsed = end - start
  timing[0] = elapsed
  #print("elapsed ", elapsed)

  #singular values
  #print("Writing sing values to file: {}".format(svaFile))
  np.savetxt(svaFile, S)

  assert(U.flags['F_CONTIGUOUS'])

  # left singular vectors
  fileo = open(lsvFile, "wb")
  # write to beginning of file the extents of the matrix
  numRows=np.int64(U.shape[0])
  np.array([numRows]).tofile(fileo)
  numCols=np.int64(U.shape[1])
  np.array([numCols]).tofile(fileo)
  '''
  NOTE: tofile writes an array rowwise, REGARDLESS of the layout of the matrix.
  So here we need to pass U.T to tofile so that tofile writes U in the proper
  way required format for how we read these later
  '''
  UT = np.transpose(U)
  UT.tofile(fileo)
  fileo.close()
  #outDir = os.path.dirname(lsvFile)
  #np.savetxt(lsvFile+'.txt', U[:,:3])
  # np.savetxt(outDir+'/timings.txt', timing)

# -------------------------------------------------------------------
def _default_snapshots_and_pod(outDir, customModule, dataDirs, \
                               fomMeshPath, stateDataOrRhsData):
  # collect some info that we need
  fomTotCells = find_total_cells_from_info_file(fomMeshPath)
  totFomDofs  = fomTotCells*customModule.numDofsPerCell
  print(totFomDofs)

  if stateDataOrRhsData == "stateData":
    M = load_fom_state_snapshots_from_multiple_runs(dataDirs, totFomDofs,
                                                    customModule.numDofsPerCell,
                                                    False) # don't subtract IC
    write_matrix_to_bin_omit_shape(outDir+"/state_snapshots.bin", M)

    lsvFile = outDir + '/state_left_singular_vectors.bin'
    svaFile = outDir + '/state_singular_values.txt'
    # need to do SVD on M.T because M.T has the correct shape,
    # i.e. each column is an instance of state or rhs
    # so that the columns of M represent the snapshots taken
    _do_svd(M.T, lsvFile, svaFile)

  else:
    M = load_fom_rhs_snapshots_from_multiple_runs(dataDirs, totFomDofs,
                                                  customModule.numDofsPerCell)
    write_matrix_to_bin_omit_shape(outDir+"/rhs_snapshots.bin", M)
    lsvFile = outDir + '/rhs_left_singular_vectors.bin'
    svaFile = outDir + '/rhs_singular_values.txt'
    # need to do SVD on M.T because M.T has the correct shape,
    # i.e. each column is an instance of state or rhs
    # so that the columns of M represent the snapshots taken
    _do_svd(M.T, lsvFile, svaFile)

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

  # for this driver to be valid, we need to ensure this:
  _validate_wf_offlinerom_section(wfDic, customModule)
  podDic = wfDic['offlineRom']['pod']

  # figure out if offlineRom dir exists
  offlineRomDir = workDirFullPath + "/offline_rom"
  if os.path.exists(offlineRomDir):
    print('{} already exists'.format(offlineRomDir))

  else:
    os.system('mkdir -p ' + offlineRomDir)

    # write to file
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
        print(trainDirsToUse)

        # figure out the policy
        policy = podDic[s]['policy']
        if policy == "default":
          _default_snapshots_and_pod(offlineRomDir, customModule, trainDirsToUse, fomMeshPath, s)
        else:
          print("invalid or unsupported pod policy {}".format(policy))
