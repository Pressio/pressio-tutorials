#!/usr/bin/env python

#==============================================================
# imports
#==============================================================

import sys, os, importlib, copy, git
from argparse import ArgumentParser
from pathlib import Path

# local
from all import *

#==============================================================
# functions
#==============================================================

def _validate_wf_parameter_space_section(wfDic, customModule):
  assert 'parameterSpace' in wfDic, "parameterSpace section not found"
  subDic = wfDic['parameterSpace']

  # the parameterSpace dic must have these subfields
  for field in ['names', 'trainPoints', 'testPoints']:
    assert field in subDic, "subfield: " +field +" must be present"

  # names must be valid for this problem
  for paramName in subDic['names']:
    assert paramName in customModule.validParameterNames,\
      "parameter '{}' is not a valid for problem '{}' ".format(paramName, wfDic['problem'])

  # ensure there is at least one train run and one test run
  assert subDic['trainPoints'], "there must be at least one train run"
  assert subDic['testPoints'] , "there must be at least one test run"

  for k,v in subDic['trainPoints'].items():
    assert len(v) == len(subDic['names']), \
      "num of coordinates for train run {} is different than num of params provided".format(k)

  for k,v in subDic['testPoints'].items():
    assert len(v) == len(subDic['names']), \
      "num of coordinates for test run {} is different than num of params provided".format(k)

# -----------------------------------------------------------
def _validate_wf_fom_section(wfDic, customModule):
  assert 'fom' in wfDic, "fom section not found"
  fomDic = wfDic['fom']

  # detect if there is a train/test subsection
  trainSubsec = 'train' in fomDic
  testSubsec  = 'test' in fomDic

  # verify that meshSize exists ONLY in global scope
  assert 'meshSize' in fomDic, "meshSize is not present, MUST be unique"
  if trainSubsec:
    assert 'meshSize' not in fomDic['train'], "meshSize cannot be inside train subsection"
  if testSubsec:
    assert 'meshSize' not in fomDic['test'], "meshSize cannot be inside test subsection"
  # matches dims of the problem
  assert len(fomDic['meshSize']) == customModule.dimensionality, \
    "meshSize must match the dimensionality of the problem"

  # odeScheme is found in global scope or train/test
  globalOdeScheme = 'odeScheme' in fomDic
  if globalOdeScheme: is_valid_odescheme(fomDic['odeScheme'])

# -----------------------------------------------------------
def _do_runs(dryRun, workDirFullPath, customModule, dics, kind):
  assert kind=="train" or kind=="test"

  for dicIterator in dics:
    runNumber = dicIterator['runId']

    runDirFullPath = create_fom_run_directory_name(workDirFullPath, kind, runNumber)

    if not os.path.exists(runDirFullPath):
      os.mkdir(runDirFullPath)

      # need to write to file the input yaml file
      # before we can do that, we need to simplify things a bit
      # since we know that some settings are only valid for
      # training or testing
      inputFileToWrite = runDirFullPath + "/input.yaml"
      specificFields = dicIterator[kind]
      # make these global fields
      for k,v in specificFields.items():
        dicIterator[k] = v

      # delete things we don't need anymore
      del dicIterator['test']
      del dicIterator['train']
      # mesh size not needed because we store the full mesh path
      del dicIterator['meshSize']

      # now we can write to file
      write_dic_to_yaml_file(inputFileToWrite, dicIterator)

      if dryRun:
        print("dryrun: fom: {}".format(runDirFullPath))
      else:
        exeDirFullPath = str(Path(runDirFullPath).parent)
        link_if_needed_and_run_exe(runDirFullPath,  exeDirFullPath, cppExecutableName)

    else:
      print("Skip existing: {}".format(runDirFullPath))

# -----------------------------------------------------------
def _do_train_runs(dryRun, workDirFullPath, customModule, dics):
  _do_runs(dryRun, workDirFullPath, customModule, dics, "train")

# -----------------------------------------------------------
def _do_test_runs(dryRun, workDirFullPath, customModule, dics):
  _do_runs(dryRun, workDirFullPath, customModule, dics, "test")

# -----------------------------------------------------------
def _create_input_dics(problemName, customModule, meshFullPath, \
                       fomDic, parametersNames, values):
  result = []
  for runGlobalId, v in values.items():
    # make a copy of the base dic
    thisRunDic = copy.deepcopy(fomDic)
    thisRunDic['problem'] = problemName
    thisRunDic['runId'] = runGlobalId
    thisRunDic['meshDir'] = meshFullPath
    thisRunDic['numDofsPerCell'] = customModule.numDofsPerCell

    # insert info about parameters
    for i, paramName in enumerate(parametersNames):
      thisRunDic[paramName] = v[i]

    result.append(thisRunDic)
  return result

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

  # for this driver to be valid, we need to ensure this:
  _validate_wf_parameter_space_section(wfDic, customModule)
  _validate_wf_fom_section(wfDic, customModule)
  # if here, we know these are ok so get them
  fomDic = wfDic['fom']
  paramsDic = wfDic['parameterSpace']

  # make FOM mesh if needed
  pdaRepoFullPath = ptutRepoFullPath + '/tpls/pressio-demoapps'
  fomMeshFullPath = workDirFullPath + "/fom_mesh"
  if os.path.exists(fomMeshFullPath):
    print('Mesh {} already exists'.format(fomMeshFullPath))
  else:
    customModule.generate_full_mesh(pdaRepoFullPath, fomMeshFullPath, fomDic)

  print("===================")
  print("Train runs")
  print("===================")
  trainDics = _create_input_dics(problemName, customModule,
                                 fomMeshFullPath, fomDic,
                                 paramsDic['names'],
                                 paramsDic['trainPoints'])
  _do_train_runs(dryRun, workDirFullPath, customModule, trainDics)

  print("\n===================")
  print("Test runs")
  print("===================")
  testDics = _create_input_dics(problemName, customModule,
                                fomMeshFullPath, fomDic,
                                paramsDic['names'],
                                paramsDic['testPoints'])
  _do_test_runs(dryRun, workDirFullPath, customModule, testDics)
