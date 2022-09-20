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
def _validate_wf_lspg_section(wfDic, customModule):
  assert 'lspgRom' in wfDic, "lspgRom section not found"
  subDic = wfDic['lspgRom']

def _run_single_rom(romDir, offlineRomDir, numModes, \
                    romDic, inputsDic, \
                    fomRunDir, algoString, \
                    sampleMeshDir = None):

  if os.path.exists(romDir):
    print('{} already exists'.format(romDir))
  else:
    print("Running LSPG rom in {}".format(os.path.basename(romDir)))
    os.system('mkdir -p ' + romDir)

    # write some info to run directory
    f = open(romDir+"/rom_dofs_count.txt", "w")
    f.write(str(numModes))
    f.close()

    romSpecifics = {}

    # full mesh pod
    fullMeshPodFile = offlineRomDir +"/state_left_singular_vectors.bin"
    romSpecifics['fullMeshPodFile'] = fullMeshPodFile

    # compute rom initial state using the basis
    myPhi = load_pod_basis(fullMeshPodFile)[:, 0:numModes]
    fomIc = load_fom_initial_condition(fomRunDir+"/initial_state.bin")
    romIc = np.dot(myPhi.transpose(), fomIc)
    romInitStateFile = romDir+"/rom_initial_state.txt"
    np.savetxt(romInitStateFile, romIc)

    # deal with affine shift
    np.savetxt(romDir+"/optional_affine_shift.txt", np.zeros(len(fomIc)))
    romSpecifics['affineShiftFile'] = romDir +"/optional_affine_shift.txt"
    romSpecifics['isAffine'] = False

    romSpecifics['algorithm'] = algoString
    romSpecifics['numModes'] = numModes
    romSpecifics['romInitialStateFile'] = romInitStateFile

    if sampleMeshDir!= None:
      inputsDic['meshDir'] = sampleMeshDir
      romSpecifics["stencilMeshGidsFile"] = sampleMeshDir +"/stencil_mesh_gids.dat"
      romSpecifics["sampleMeshGidsFile"]  = sampleMeshDir +"/sample_mesh_gids.txt"

    inputsDic['rom'] = romSpecifics

    # since we are doing lspg, we need to overwrite the solver
    # to use what is specified in the workflow file
    for s in ['nonlinearSolverName', 'nonlinearSolverTolerance']:
      inputsDic[s] = romDic[s]

    # now we can write to file
    write_dic_to_yaml_file(romDir+"/input.yaml", inputsDic)

    if dryRun:
      print("dryrun: fom: {}".format(romDir))
    else:
      exeDirFullPath = str(Path(romDir).parent)
      link_if_needed_and_run_exe(romDir,  exeDirFullPath, cppExecutableName)


def _main_default_impl(workDir, romDic):
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
        singValues = np.loadtxt(offlineRomDir+'/state_singular_values.txt')
        numModes = compute_cumulative_energy(singValues, energy)
        print("numModes = ", numModes)

        #
        # for a given num of modes, find all FOM test runs
        # inside workDir and loop over each and run a corresponding ROM
        #
        for fomTestDir in find_all_fom_test_dirs(workDir):
          # read the yaml file used for that FOM run
          fomInputs = read_yaml_file(fomTestDir + "/input.yaml")

          # extract from fom dir the identifier so that
          # we know which test point it refers to
          runId = get_run_id(fomTestDir)

          # make name of the output rom directory
          romDir = workDir + "/default_lspg_truncation_energybased"
          romDir += "_"+str(energy)
          romDir += "_runid_"+str(runId)
          _run_single_rom(romDir, offlineRomDir, numModes, \
                          romDic, fomInputs, fomTestDir, "defaultLspg")

# -------------------------------------------------------------------
def _main_hyperreduced_impl(workDir, romDic, numDofsPerCell):
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
          singValues = np.loadtxt(offlineRomDir+'/state_singular_values.txt')
          numModes = compute_cumulative_energy(singValues, energy)
          print(numModes)

          for fomTestDir in find_all_fom_test_dirs(workDir):
            # read the yaml file used for that FOM run
            fomInputs = read_yaml_file(fomTestDir + "/input.yaml")

            # extract from fom dir the identifier so that
            # we know which test point it refers to
            runId = get_run_id(fomTestDir)

            # make name of the output rom directory
            romDir = workDir + "/hyperreduced_lspg_truncation_energybased"
            romDir += "_"+str(energy)
            romDir += "_" + strIdSm
            romDir += "_runid_"+str(runId)
            print(romDir)
            _run_single_rom(romDir, offlineRomDir, numModes, \
                            romDic, fomInputs, fomTestDir, "hyperreducedLspg",
                            sampleMeshDir = sampleMeshDir)

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
  _validate_wf_lspg_section(wfDic, customModule)
  romDic = wfDic['lspgRom']

  if romDic['algorithm'].lower() == "defaultlspg":
    _main_default_impl(workDirFullPath, romDic)

  elif romDic['algorithm'].lower() == "hyperreducedlspg":
    _main_hyperreduced_impl(workDirFullPath, romDic, customModule.numDofsPerCell)
