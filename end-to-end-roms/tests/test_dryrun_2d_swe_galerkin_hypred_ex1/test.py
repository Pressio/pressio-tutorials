import os
import sys

try:
    tutorials_dir = f'{os.sep}'.join(os.path.abspath(__file__).split(os.sep)[:-4])
    etoetestdir = os.path.join(tutorials_dir, 'end-to-end-roms', 'tests')
    sys.path.append(tutorials_dir)
    sys.path.append(etoetestdir)
except Exception as e:
    print(f'Can not add project path to system path! Exiting!\nERROR: {e}')
    raise SystemExit(1)

import subprocess
import unittest
import yaml
from dryrun_check_utils import yaml_dic_check

class Tester(unittest.TestCase):
    WORK_DIR = ""

    def setUp(self):
        self.wf_foms = os.path.join(tutorials_dir, 'end-to-end-roms', 'wf_foms.py')
        self.wf_offline_rom = os.path.join(tutorials_dir, 'end-to-end-roms', 'wf_offline_rom.py')
        self.my_work_dir = os.path.abspath(Tester.WORK_DIR)

    def test_foms(self):
        # we want to run this from inside the test dire
        cwd = os.path.join(self.my_work_dir)

        #
        # Running
        #
        wf_foms_command = ['python3', self.wf_foms, '--wf', 'wf.yaml', '--dryrun', 'True']
        print(f'===> Running...\n===> Command: {" ".join(wf_foms_command)}\n===> Working dir: {cwd}')
        wf_foms_build = subprocess.run(wf_foms_command, stdout=subprocess.PIPE, stderr=subprocess.PIPE,
                                       encoding='utf-8', cwd=cwd)
        print(wf_foms_build.stdout)

        #
        # checking
        #
        # fom_test_runid_0
        gold = {'inviscidFluxReconstruction': 'Weno3', \
                'odeScheme': 'RK4', 'timeStepSize': 0.005,
                'problem': '2d_swe', 'runId': 0, \
                'meshDir': f'{cwd}/fom_mesh', 'numDofsPerCell': 3,
                'coriolis': -0.5, 'pulsemag': 0.125, 'gravity': 9.8,\
                'finalTime': 5.0,
                'stateSamplingFreq': 10, 'rhsSamplingFreq': 100}
        trial = os.path.join(cwd, 'fom_test_runid_0', 'input.yaml')
        yaml_dic_check(gold, trial)


        # fom_train_runid_0
        gold = {'inviscidFluxReconstruction': 'Weno3', \
                'odeScheme': 'RK4', 'timeStepSize': 0.005,
                'problem': '2d_swe', 'runId': 0, \
                'meshDir': f'{cwd}/fom_mesh', 'numDofsPerCell': 3,
                'coriolis': -1.0, 'pulsemag': 0.125, 'gravity': 9.8,\
                'finalTime': 5.0,
                'stateSamplingFreq': 10, 'rhsSamplingFreq': 10}
        trial = os.path.join(cwd, 'fom_train_runid_0', 'input.yaml')
        yaml_dic_check(gold, trial)

        # fom_train_runid_1
        gold = {'inviscidFluxReconstruction': 'Weno3', \
                'odeScheme': 'RK4', 'timeStepSize': 0.005,
                'problem': '2d_swe', 'runId': 1, \
                'meshDir': f'{cwd}/fom_mesh', 'numDofsPerCell': 3,
                'coriolis': 0.0, 'pulsemag': 0.125, 'gravity': 9.8, \
                'finalTime': 5.0,
                'stateSamplingFreq': 10, 'rhsSamplingFreq': 10}
        trial = os.path.join(cwd, 'fom_train_runid_1', 'input.yaml')
        yaml_dic_check(gold, trial)


    def test_offline(self):
        # we want to run this from inside the test dire
        cwd = os.path.join(self.my_work_dir)

        #
        # Running
        #
        wf_offline_rom_command = ['python3', self.wf_offline_rom, '--wf', 'wf.yaml', '--dryrun', 'True']
        print(f'===> Running...\n===> Command: {" ".join(wf_offline_rom_command)}\n===> Working dir: {cwd}')
        wf_offline_rom_build = subprocess.run(wf_offline_rom_command, stdout=subprocess.PIPE, stderr=subprocess.PIPE,
                                              encoding='utf-8', cwd=cwd)
        print(wf_offline_rom_build.stdout)

        #
        # checking
        #
        gold = {'stateData': {'useTrainingRuns': 'all'}, \
                'rhsData': {'useTrainingRuns': 'all'}}
        trial = os.path.join(cwd, 'offline_rom', 'pod_input.yaml')
        yaml_dic_check(gold, trial)
        # check that sample mesh exists
        assert os.path.exists(cwd)


    def test_online(self):
        # we want to run this from inside the test dire
        cwd = os.path.join(self.my_work_dir)

        #
        # Running
        #
        wf_galerkin = os.path.join(tutorials_dir, 'end-to-end-roms', 'wf_galerkin.py')
        wf_galerkin_command = ['python3', wf_galerkin, '--wf', 'wf.yaml', '--dryrun', 'True']
        print(f'===> Running...\n===> Command: {" ".join(wf_galerkin_command)}\n===> Working dir: {cwd}')
        wf_offline_rom_build = subprocess.run(wf_galerkin_command, stdout=subprocess.PIPE, stderr=subprocess.PIPE,
                                              encoding='utf-8', cwd=cwd)
        print(wf_offline_rom_build.stdout)

        # hyperreduced_galerkin_99_999
        gold = {
            'inviscidFluxReconstruction': 'Weno3', 'odeScheme': 'RK4', 'timeStepSize': 0.005, 'problem': '2d_swe',
            'runId': 0, 'meshDir': f'{cwd}/offline_rom/sample_mesh_random_0.100', 'numDofsPerCell': 3, 'coriolis': -0.5,
            'pulsemag': 0.125, 'gravity': 9.8, 'finalTime': 5.0, 'stateSamplingFreq': 10, 'rhsSamplingFreq': 100,
            'rom': {'fullMeshPodFile': f'{cwd}/offline_rom/state_left_singular_vectors.bin',
                    'affineShiftFile': f'{cwd}/hyperreduced_galerkin_truncation_energybased_99.999_sample_mesh_random_0.100_runid_0/optional_affine_shift.txt',
                    'isAffine': False, 'algorithm': 'hyperreducedGalerkin', 'numModes': 22,
                    'romInitialStateFile': f'{cwd}/hyperreduced_galerkin_truncation_energybased_99.999_sample_mesh_random_0.100_runid_0/rom_initial_state.txt',
                    'galerkinHypRedOperatorFile': f'{cwd}/hyperreducer_99.999_sample_mesh_random_0.100/hyperReductionOperator.bin',
                    'stencilMeshGidsFile': f'{cwd}/offline_rom/sample_mesh_random_0.100/stencil_mesh_gids.dat',
                    'sampleMeshGidsFile': f'{cwd}/offline_rom/sample_mesh_random_0.100/sample_mesh_gids.txt'}}
        trial = os.path.join(cwd,
                             'hyperreduced_galerkin_truncation_energybased_99.999_sample_mesh_random_0.100_runid_0',
                             'input.yaml')
        yaml_dic_check(gold, trial)

        # hyperreduced_galerkin_99_99999
        gold = {
            'inviscidFluxReconstruction': 'Weno3', 'odeScheme': 'RK4', 'timeStepSize': 0.005, 'problem': '2d_swe',
            'runId': 0, 'meshDir': f'{cwd}/offline_rom/sample_mesh_random_0.100', 'numDofsPerCell': 3, 'coriolis': -0.5,
            'pulsemag': 0.125, 'gravity': 9.8, 'finalTime': 5.0, 'stateSamplingFreq': 10, 'rhsSamplingFreq': 100,
            'rom': {'fullMeshPodFile': f'{cwd}/offline_rom/state_left_singular_vectors.bin',
                    'affineShiftFile': f'{cwd}/hyperreduced_galerkin_truncation_energybased_99.99999_sample_mesh_random_0.100_runid_0/optional_affine_shift.txt',
                    'isAffine': False, 'algorithm': 'hyperreducedGalerkin', 'numModes': 22,
                    'romInitialStateFile': f'{cwd}/hyperreduced_galerkin_truncation_energybased_99.99999_sample_mesh_random_0.100_runid_0/rom_initial_state.txt',
                    'galerkinHypRedOperatorFile': f'{cwd}/hyperreducer_99.99999_sample_mesh_random_0.100/hyperReductionOperator.bin',
                    'stencilMeshGidsFile': f'{cwd}/offline_rom/sample_mesh_random_0.100/stencil_mesh_gids.dat',
                    'sampleMeshGidsFile': f'{cwd}/offline_rom/sample_mesh_random_0.100/sample_mesh_gids.txt'}}
        trial = os.path.join(cwd,
                             'hyperreduced_galerkin_truncation_energybased_99.99999_sample_mesh_random_0.100_runid_0',
                             'input.yaml')
        yaml_dic_check(gold, trial)


if __name__ == "__main__":
    if len(sys.argv) > 1:
        Tester.WORK_DIR = sys.argv.pop()
    unittest.main()
