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

def extract_from_rom_input_file(runDir, key):
  with open(runDir+'/input.yaml') as file:
    dic = yaml.load(file, Loader=yaml.FullLoader)
  if key in dic:
    return dic[key]
  elif key in dic['rom']:
    return dic['rom'][key]
  else:
    print("cannot find {} in dic".format(key))

def compute_and_save_errors(outDir, fomStates, approximation):
  assert(fomStates.shape == approximation.shape)

  # compute error at each time step
  # each column in fomStates or approximation contains one step
  numSteps = fomStates.shape[1]
  errMat   = np.zeros((numSteps, 3))
  error = fomStates-approximation
  errMat[:, 0] = np.linalg.norm(error, axis=0)/np.linalg.norm(fomStates, axis=0)
  errMat[:, 1] = np.linalg.norm(error, ord=1, axis=0)/np.linalg.norm(fomStates, ord=1, axis=0)
  errMat[:, 2] = np.linalg.norm(error, ord=np.inf, axis=0)/np.linalg.norm(fomStates, ord=np.inf, axis=0)
  np.savetxt(outDir+"/errors_in_time.txt", errMat)

  # compute space-time errorw
  stErrs = []
  fomSflat = fomStates.flatten('F')
  approxSflat = approximation.flatten('F')
  error    = approxSflat-fomSflat
  stErrs.append( np.linalg.norm(error)/np.linalg.norm(fomSflat) )
  stErrs.append( np.linalg.norm(error, ord=1)/np.linalg.norm(fomSflat, ord=1) )
  stErrs.append( np.linalg.norm(error, ord=np.inf)/np.linalg.norm(fomSflat, ord=np.inf) )
  np.savetxt(outDir+"/errors_space_time.txt", np.array(stErrs))

#==============================================================
# main
#==============================================================
if __name__== "__main__":
  # parsing
  parser = ArgumentParser(add_help=False)
  add_dryrun_cmd_line_option(parser)
  args = parser.parse_args()
  dryRun = args.dryRun

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

  # before moving on, ensure that workDir contains a unique FULL mesh.
  # If one wants to run for a different mesh, then they have to
  # run this script again with a different working directory
  fomMeshPath = find_full_mesh_and_ensure_unique(workDirFullPath)

  # ======================================================================

  # loop over all rom runs
  romDirs = [workDirFullPath+'/'+d for d in os.listdir(workDirFullPath) \
             if "galerkin" in d]
  for runDirIt in romDirs:
    # load pod modes
    fullMeshPodFile = extract_from_rom_input_file(runDirIt, "fullMeshPodFile")
    phi = load_pod_basis(fullMeshPodFile)

    # load rom snapshots
    numDofsPerCell = extract_from_rom_input_file(runDirIt, "numDofsPerCell")
    numModes = extract_from_rom_input_file(runDirIt, "numModes")
    romSnaps = load_single_snapshots_with_time_steps_along_rows(runDirIt, \
                                                                "state_snapshots.bin",
                                                                numModes)
    tmpy = np.dot(phi[:, 0:numModes], romSnaps.T)
    np.savetxt(runDirIt + "/reconstructed.txt", tmpy)

    # load fom snaps
    fomTotCells = find_total_cells_from_info_file(fomMeshPath)
    totFomDofs  = fomTotCells*numDofsPerCell
    fomS = load_single_snapshots_with_time_steps_along_rows(workDirFullPath+"/fom_test_runid_0",\
                                                            "state_snapshots.bin",\
                                                            totFomDofs)
    np.savetxt(runDirIt + "/fomS.txt", fomS.T)
    compute_and_save_errors(runDirIt, fomS.T, tmpy)
