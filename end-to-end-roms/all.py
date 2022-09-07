
import numpy as np
import sys, os, re, yaml, logging, git, subprocess

cppExecutableName = "pdaWfExe"

# -----------------------------------------------------------
def is_valid_odescheme(scheme):
  if scheme in ["RungeKutta4", "RK4", "rungekutta4", "rk4", \
                "SSPRungeKutta3", "SSPRK3", "ssprungekutta3", "ssprk3",\
                "EulerForward", "eulerforward"]:
    return True
  else:
    print("invalid ode scheme selected: {}".format(scheme))
    sys.exit(1)

# -------------------------------------------------------------------
def find_problem_name_from_wf_dic(wfDic):
  assert 'problem' in wfDic, "cannot find 'problem' field"
  return wfDic['problem']

# -------------------------------------------------------------------
def assert_valid_and_supported_problem(wfDic, ptutRepoEndToEndFullPath):
  assert('problem' in wfDic)

  problemName = find_problem_name_from_wf_dic(wfDic)
  moduleName  = 'customizations_'+problemName
  modulePath  = os.path.realpath(ptutRepoEndToEndFullPath+'/'+moduleName)
  if not os.path.exists(modulePath):
    print("\n")
    print("Your problem = {} seems invalid!".format(problemName))
    print("because I cannot find a valid module path: \n {}".format(modulePath))
    print("Maybe you mispelled the problem name?".format(problemName))
    sys.exit(322)

# -------------------------------------------------------------------
def link_exe(whereToRun, exeDir, exeName):
  # check if exeDir exists and contains the executables
  if not os.path.exists(exeDir):
    print("exeDir {} does not exist".format(exeDir))
    sys.exit(1)

  # check if exe exist
  sourceExe  = exeDir+"/"+exeName
  if not os.path.exists(sourceExe):
    print("exe {} does not exist".format(sourceExe))
    sys.exit(1)

  # delete if already in target dir, and relink
  destExe  = whereToRun+"/"+exeName
  if os.path.exists(destExe):
    os.system("rm -f {}".format(destExe))

  os.system("ln -s {} {}".format(sourceExe, destExe))

# -------------------------------------------------------------------
def link_if_needed_and_run_exe(whereToRun, exeDir, exeName, numThreads=1):
  link_exe(whereToRun, exeDir, exeName)

  my_env = os.environ.copy()
  if (numThreads ==1):
    my_env["OMP_NUM_THREADS"] = str(1)
    my_env["OMP_PLACES"]="{0}"
    my_env["OMP_PROC_BIND"]="true"

  else:
    my_env["OMP_NUM_THREADS"] = str(numThreads)
    my_env["OMP_PLACES"]      ="threads"
    my_env["OMP_PROC_BIND"]   ="spread"

  args = ("./"+exeName, whereToRun+"/input.yaml")

  # launch subprocess
  print("Running: {}".format(args))
  print(" inside: {}".format(whereToRun))
  logfile = open(whereToRun+'/out.log', 'w')
  p = subprocess.Popen(args, stdout=logfile,
                       stderr=logfile, env=my_env,
                       cwd=whereToRun)
  p.wait()
  logfile.close()
  assert(p.returncode==0)

# -------------------------------------------------------------------
def is_git_repo(fullPath):
  try:
    _ = git.Repo(fullPath, search_parent_directories=True)
    return True
  except git.exc.InvalidGitRepositoryError:
    return False

# -------------------------------------------------------------------
def str2bool(v):
  if isinstance(v, bool):
    return v
  if v.lower() in ('yes', 'true', 't', 'y', '1'):
    return True
  elif v.lower() in ('no', 'false', 'f', 'n', '0'):
    return False
  else:
    raise argparse.ArgumentTypeError('Boolean value expected.')

# -------------------------------------------------------------------
def add_dryrun_cmd_line_option(parser):
  parser.add_argument("--dryrun", "--dr",
                      dest="dryRun", type=str2bool,
                      default=False,
                      help="True: creates directories but does not run. Default=True.")

# -------------------------------------------------------------------
def add_wffile_cmd_line_option(parser):
  parser.add_argument("--wffile", "--wffile", "--wf",
                      dest="workFlowFile",
                      required=True,
                      help="Path to workflow file")

# -------------------------------------------------------------------
def read_workflow_yaml_file(filePath):
  with open(filePath) as file:
    yamlDic = yaml.load(file, Loader=yaml.FullLoader)
  return yamlDic

# -------------------------------------------------------------------
def read_yaml_file(filePath):
  with open(filePath) as file:
    yamlDic = yaml.load(file, Loader=yaml.FullLoader)
  return yamlDic

# -------------------------------------------------------------------
def write_dic_to_yaml_file(filePath, dicToWrite):
  with open(filePath, 'w') as yaml_file:
    yaml.dump(dicToWrite, yaml_file, \
              default_flow_style=False, \
              sort_keys=False)

# -------------------------------------------------------------------
def create_fom_run_directory_name(workDir, kind, runId):
  return workDir+"/fom" + "_"+kind+"_runid_"+str(runId)

# -------------------------------------------------------------------
def get_run_id(runDir):
  return int(runDir.split('_')[-1])

# ----------------------------------------------------------------
def find_full_mesh_and_ensure_unique(workDir):
  fomFullMeshes = [workDir+'/'+d for d in os.listdir(workDir) \
                   # we need to find only dirs that BEGIN with
                   # this string otherwise we pick up other things
                   if "fom_mesh" == os.path.basename(d)[0:9]]
  if len(fomFullMeshes) != 1:
    em = "Error: I found multiple full meshes:\n"
    for it in fomFullMeshes:
      em += it + "\n"
    em += "inside the workDir = {} \n".format(workDir)
    em += "You can only have a single FULL mesh the working directory."
    print(em)
    sys.exit(1)
  return fomFullMeshes[0]

# -------------------------------------------------------------------
def find_fom_train_dirs_for_target_set_of_indices(workDir, trainIndices):
  trainDirs = [workDir+'/'+d for d in os.listdir(workDir) \
               if "fom_train" in d and get_run_id(d) in trainIndices]
  assert(len(trainDirs) == len(trainIndices))
  return trainDirs

# -------------------------------------------------------------------
def find_all_fom_test_dirs(workDir):
  return [workDir+'/'+d for d in os.listdir(workDir) if "fom_test" in d]

# #==========================================================
# def default_create_rom_run_directory_name(workDir, modeCount, romType, setId):
#   dd = "rom_" + romType + "_set_"+str(setId)

#   # use this format to print mode count so that directories look nicer
#   #modeCountString = "{:04d}".format( int(runDic['rom']['numModes']) )
#   dd += "_nPod_"+str(modeCount)

#   return workDir+"/"+dd

# -------------------------------------------------------------------
def find_sample_mesh_count_from_info_file(workDir):
  reg = re.compile(r'sampleMeshSize.+')
  file1 = open(workDir+'/info.dat', 'r')
  strings = re.search(reg, file1.read())
  file1.close()
  assert(strings)
  return int(strings.group().split()[1])

# -------------------------------------------------------------------
def find_stencil_mesh_count_from_info_file(workDir):
  reg = re.compile(r'stencilMeshSize.+')
  file1 = open(workDir+'/info.dat', 'r')
  strings = re.search(reg, file1.read())
  file1.close()
  assert(strings)
  return int(strings.group().split()[1])

# -------------------------------------------------------------------
def find_dimensionality_from_info_file(workDir):
  reg = re.compile(r'dim.+')
  file1 = open(workDir+'/info.dat', 'r')
  strings = re.search(reg, file1.read())
  file1.close()
  assert(strings)
  return int(strings.group().split()[1])

# -------------------------------------------------------------------
def find_num_cells_from_info_file(workDir, ns):
  reg = re.compile(r''+ns+'.+')
  file1 = open(workDir+'/info.dat', 'r')
  strings = re.search(reg, file1.read())
  file1.close()
  assert(strings)
  return int(strings.group().split()[1])

# -------------------------------------------------------------------
def find_total_cells_from_info_file(workDir):
  dims = find_dimensionality_from_info_file(workDir)
  if dims == 1:
    return find_num_cells_from_info_file(workDir, "nx")
  elif dims==2:
    nx = find_num_cells_from_info_file(workDir, "nx")
    ny = find_num_cells_from_info_file(workDir, "ny")
    return nx*ny
  else:
    print("Invalid dims = {}".format(dims))
    sys.exit(1)


# -------------------------------------------------------------------
def load_single_snapshots_with_time_steps_along_rows(dataDir, fileName, totDofs):
  print("reading data from {}".format(os.path.basename(dataDir)))
  data = np.fromfile(dataDir+"/"+fileName)
  numTimeSteps = int(np.size(data)/totDofs)
  print(numTimeSteps, totDofs)
  D  = np.reshape(data, (numTimeSteps, totDofs))
  print(" (numRows, numCols) = {}".format(D.shape))
  return D

# -------------------------------------------------------------------
def load_fom_state_snapshots_from_multiple_runs(dataDirs, fomTotDofs, \
                                                numDofsPerCell, subtractInitialCondition):

  M = np.zeros((0, fomTotDofs))
  for targetDirec in dataDirs:
    D = load_single_snapshots_with_time_steps_along_rows(targetDirec, \
                                                          "state_snapshots.bin", \
                                                          fomTotDofs)
    if subtractInitialCondition:
      print("subtracting initial state")
      IC = np.fromfile(targetDirec+"/initial_state.bin")
      for i in range(D.shape[0]):
        D[i,:] = D[i,:] - IC

    M = np.append(M, D, axis=0)

  print("state snapshots: shape  : {}".format(M.shape))
  print("state snapshots: min/max: {} {}".format(np.min(M), np.max(M)))
  return M

# -------------------------------------------------------------------
def load_fom_rhs_snapshots_from_multiple_runs(dataDirs, fomTotDofs, numDofsPerCell):
  M = np.zeros((0, fomTotDofs))
  for targetDirec in dataDirs:
    D = load_single_snapshots_with_time_steps_along_rows(targetDirec, \
                                                         "rhs_snapshots.bin", \
                                                         fomTotDofs)
    M = np.append(M, D, axis=0)

  print("rhs snapshots: shape  : {}".format(M.shape))
  print("rhs snapshots: min/max: {} {}".format(np.min(M), np.max(M)))
  return M

# -------------------------------------------------------------------
def write_matrix_to_bin_omit_shape(fileName, M, \
                                   transposeBeforeWriting = False):
  fileo = open(fileName, "wb")
  if transposeBeforeWriting:
    MT = np.transpose(M)
    MT.tofile(fileo)
  else:
    M.tofile(fileo)
  fileo.close()

# -------------------------------------------------------------------
def compute_cumulative_energy(svalues, targetPercentage):
  if targetPercentage == 100.:
    return len(svalues)
  else:
    # convert percentage to decimal
    target = float(targetPercentage)/100.
    sSq = np.square(svalues)
    den = np.sum(sSq)
    rsum = 0.
    for i in range(0, len(svalues)):
      rsum += sSq[i]
      ratio = (rsum/den)
      if ratio >= target:
        return i
    return len(svalues)

# -------------------------------------------------------------------
def load_pod_basis(lsvFile):
  # count=2 because the basis file always contains
  # the shape at the top
  nr, nc  = np.fromfile(lsvFile, dtype=np.int64, count=2)
  M = np.fromfile(lsvFile, offset=np.dtype(np.int64).itemsize*2)
  M = np.reshape(M, (nr, nc), order='F')
  return M

# -------------------------------------------------------------------
def load_fom_initial_condition(fromFile):
  return np.fromfile(fromFile)

# # -------------------------------------------------------------------
# def load_rom_state_snapshot_matrix(runDir, totalNumModes):
#   logger = logging.getLogger(__name__)
#   logger.info("reading data from {}".format(os.path.basename(runDir)))

#   data = np.fromfile(runDir+"/rom_snaps_state")
#   numTimeSteps = int(np.size(data)/totalNumModes)
#   M = np.reshape(data, (numTimeSteps, totalNumModes))
#   #print("A= ", M.flags['C_CONTIGUOUS'])
#   logger.debug("rom state snapshots: shape  : {}".format(M.T.shape))
#   logger.debug("rom state snapshots: min/max: {} {}".format(np.min(M), np.max(M)))
#   return M.T
