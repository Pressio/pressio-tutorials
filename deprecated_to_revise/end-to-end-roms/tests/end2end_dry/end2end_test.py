import os
import sys

try:
    tutorials_dir = f'{os.sep}'.join(os.path.abspath(__file__).split(os.sep)[:-4])
    sys.path.append(tutorials_dir)
except Exception as e:
    print(f'Can not add project path to system path! Exiting!\nERROR: {e}')
    raise SystemExit(1)

import subprocess
import unittest
import yaml


def build_tutorials() -> None:
    """ Build Pressio-tutorials before the tests. """
    # Finding compiler
    print(f'==================================================\n===> Finding compiler...')
    cxx = subprocess.run(['which', 'g++'],
                         stdout=subprocess.PIPE,
                         stderr=subprocess.PIPE,
                         encoding='utf-8').stdout.replace('\n', '')
    if cxx:
        print(f'===> Found cpp compiler: {cxx}')
        os.environ['CXX'] = cxx
    else:
        raise FileNotFoundError('=> Can not find a cpp compiler. Exiting.')

    # Setting tutorials build directory
    print(f'--------------------------------------------------\n===> Setting tutorials build directory...')
    up_tutorials_dir = f'{os.sep}'.join(os.path.abspath(__file__).split(os.sep)[:-5])
    tutorial_build_dir = os.path.join(up_tutorials_dir, 'pressio-tutorials-build')
    if not os.path.exists(tutorial_build_dir):
        os.makedirs(tutorial_build_dir)
        print(f'===> Created tutorials build directory: {tutorial_build_dir}')
    else:
        raise FileExistsError(f'=> Directory already exists: {tutorial_build_dir}')
    os.environ['BUILDDIR'] = tutorial_build_dir

    # Setting CMake build
    print(f'--------------------------------------------------\n===> Setting CMake build...')
    cmake_args = ['cmake', '-DCMAKE_BUILD_TYPE=Release', '-S', tutorials_dir, '-B', tutorial_build_dir]
    print(f'===> Running command: {" ".join(cmake_args)}')
    cmake_build = subprocess.run(cmake_args, stdout=subprocess.PIPE, stderr=subprocess.PIPE, encoding='utf-8')
    print(cmake_build.stdout)

    # Building tutorials
    print(f'--------------------------------------------------\n===> Building tutorials')
    num_of_proc = os.cpu_count()
    print(f'===> Found: {num_of_proc} CPU\'s')
    make_args = ['make', f'-j{num_of_proc}']
    print(f'===> Running command: {" ".join(make_args)}')
    make_build = subprocess.run(make_args,
                                stdout=subprocess.PIPE,
                                stderr=subprocess.PIPE,
                                encoding='utf-8',
                                cwd=tutorial_build_dir)
    print(make_build.stdout)
    print(f'===> Build finished.\n==================================================')


class TestConfig(unittest.TestCase):
    def setUp(self):
        up_tutorials_dir = f'{os.sep}'.join(os.path.abspath(__file__).split(os.sep)[:-5])
        tutorial_build_dir = os.path.join(up_tutorials_dir, 'pressio-tutorials-build')
        self.tutorial_build_dir = tutorial_build_dir
        self.wf_foms = os.path.join(tutorials_dir, 'end-to-end-roms', 'wf_foms.py')
        self.wf_offline_rom = os.path.join(tutorials_dir, 'end-to-end-roms', 'wf_offline_rom.py')

    def test_2d_swe_galerkin_default(self):
        tutorials_build_dir_name = '2d_swe_galerkin_default'
        cwd = os.path.join(self.tutorial_build_dir, 'end-to-end-roms', tutorials_build_dir_name)
        # Running wf_foms
        wf_foms_command = ['python3', self.wf_foms, '--wf', 'wf.yaml', '--dryrun', 'True']
        print(f'===> Running...\n==> Command: {" ".join(wf_foms_command)}\n==> Working dir: {cwd}')
        wf_foms_build = subprocess.run(wf_foms_command, stdout=subprocess.PIPE, stderr=subprocess.PIPE,
                                       encoding='utf-8', cwd=cwd)
        print(wf_foms_build.stdout)
        # Running wf_offline_rom
        wf_offline_rom_command = ['python3', self.wf_offline_rom, '--wf', 'wf.yaml', '--dryrun', 'True']
        print(f'===> Running...\n==> Command: {" ".join(wf_offline_rom_command)}\n==> Working dir: {cwd}')
        wf_offline_rom_build = subprocess.run(wf_offline_rom_command, stdout=subprocess.PIPE, stderr=subprocess.PIPE,
                                              encoding='utf-8', cwd=cwd)
        print(wf_offline_rom_build.stdout)
        # Running wf_galerkin
        wf_galerkin = os.path.join(tutorials_dir, 'end-to-end-roms', 'wf_galerkin.py')
        wf_galerkin_command = ['python3', wf_galerkin, '--wf', 'wf.yaml', '--dryrun', 'True']
        print(f'===> Running...\n==> Command: {" ".join(wf_galerkin_command)}\n==> Working dir: {cwd}')
        wf_offline_rom_build = subprocess.run(wf_galerkin_command, stdout=subprocess.PIPE, stderr=subprocess.PIPE,
                                              encoding='utf-8', cwd=cwd)
        print(wf_offline_rom_build.stdout)
        # Assertions
        # fom_test_runid_0
        fom_test_runid_0 = {'inviscidFluxReconstruction': 'Weno3', 'odeScheme': 'RK4', 'timeStepSize': 0.005,
                            'problem': '2d_swe', 'runId': 0, 'meshDir': f'{cwd}/fom_mesh', 'numDofsPerCell': 3,
                            'coriolis': -0.5, 'pulsemag': 0.125, 'gravity': 9.8, 'finalTime': 5.0,
                            'stateSamplingFreq': 10, 'rhsSamplingFreq': 100}
        fom_test_runid_0_input = os.path.join(cwd, 'fom_test_runid_0', 'input.yaml')
        with open(fom_test_runid_0_input, 'rt') as input_yaml:
            input_yaml_dict = yaml.safe_load(input_yaml)
            print(f'===> Asserting...\n==> File: {fom_test_runid_0_input}')
            self.assertEqual(fom_test_runid_0, input_yaml_dict)
            print(f'==> Assertion passed.')
        # fom_train_runid_0
        fom_train_runid_0 = {'inviscidFluxReconstruction': 'Weno3', 'odeScheme': 'RK4', 'timeStepSize': 0.005,
                             'problem': '2d_swe', 'runId': 0, 'meshDir': f'{cwd}/fom_mesh', 'numDofsPerCell': 3,
                             'coriolis': -1.0, 'pulsemag': 0.125, 'gravity': 9.8, 'finalTime': 5.0,
                             'stateSamplingFreq': 10, 'rhsSamplingFreq': 5}
        fom_train_runid_0_input = os.path.join(cwd, 'fom_train_runid_0', 'input.yaml')
        with open(fom_train_runid_0_input, 'rt') as input_yaml:
            input_yaml_dict = yaml.safe_load(input_yaml)
            print(f'===> Asserting...\n==> File: {fom_train_runid_0_input}')
            self.assertEqual(fom_train_runid_0, input_yaml_dict)
            print(f'==> Assertion passed.')
        # fom_train_runid_1
        fom_train_runid_1 = {'inviscidFluxReconstruction': 'Weno3', 'odeScheme': 'RK4', 'timeStepSize': 0.005,
                             'problem': '2d_swe', 'runId': 1, 'meshDir': f'{cwd}/fom_mesh', 'numDofsPerCell': 3,
                             'coriolis': 0.0, 'pulsemag': 0.125, 'gravity': 9.8, 'finalTime': 5.0,
                             'stateSamplingFreq': 10, 'rhsSamplingFreq': 5}
        fom_train_runid_1_input = os.path.join(cwd, 'fom_train_runid_1', 'input.yaml')
        with open(fom_train_runid_1_input, 'rt') as input_yaml:
            input_yaml_dict = yaml.safe_load(input_yaml)
            print(f'===> Asserting...\n==> File: {fom_train_runid_1_input}')
            self.assertEqual(fom_train_runid_1, input_yaml_dict)
            print(f'==> Assertion passed.')
        # offline_rom
        offline_rom = {'stateData': {'useTrainingRuns': 'all'}, 'rhsData': {'useTrainingRuns': 'all'}}
        offline_rom_input = os.path.join(cwd, 'offline_rom', 'pod_input.yaml')
        with open(offline_rom_input, 'rt') as input_yaml:
            input_yaml_dict = yaml.safe_load(input_yaml)
            print(f'===> Asserting...\n==> File: {offline_rom_input}')
            self.assertEqual(offline_rom, input_yaml_dict)
            print(f'==> Assertion passed.')
        # default_galerkin_99_999
        default_galerkin_99_999 = {
            'inviscidFluxReconstruction': 'Weno3', 'odeScheme': 'RK4', 'timeStepSize': 0.005, 'problem': '2d_swe',
            'runId': 0, 'meshDir': f'{cwd}/fom_mesh', 'numDofsPerCell': 3, 'coriolis': -0.5, 'pulsemag': 0.125,
            'gravity': 9.8, 'finalTime': 5.0, 'stateSamplingFreq': 10, 'rhsSamplingFreq': 100,
            'rom': {'fullMeshPodFile': f'{cwd}/offline_rom/state_left_singular_vectors.bin',
                    'affineShiftFile': f'{cwd}/default_galerkin_truncation_energybased_99.999_runid_0/optional_affine_shift.txt',
                    'isAffine': False, 'algorithm': 'defaultGalerkin', 'numModes': 22,
                    'romInitialStateFile': f'{cwd}/default_galerkin_truncation_energybased_99.999_runid_0/rom_initial_state.txt'}}
        default_galerkin_99_999_input = os.path.join(cwd,
                                                     'default_galerkin_truncation_energybased_99.999_runid_0',
                                                     'input.yaml')
        with open(default_galerkin_99_999_input, 'rt') as input_yaml:
            input_yaml_dict = yaml.safe_load(input_yaml)
            print(f'===> Asserting...\n==> File: {default_galerkin_99_999_input}')
            self.assertEqual(default_galerkin_99_999, input_yaml_dict)
            print(f'==> Assertion passed.')
        # default_galerkin_99_99999
        default_galerkin_99_99999 = {
            'inviscidFluxReconstruction': 'Weno3', 'odeScheme': 'RK4', 'timeStepSize': 0.005, 'problem': '2d_swe',
            'runId': 0, 'meshDir': f'{cwd}/fom_mesh', 'numDofsPerCell': 3, 'coriolis': -0.5, 'pulsemag': 0.125,
            'gravity': 9.8, 'finalTime': 5.0, 'stateSamplingFreq': 10, 'rhsSamplingFreq': 100,
            'rom': {'fullMeshPodFile': f'{cwd}/offline_rom/state_left_singular_vectors.bin',
                    'affineShiftFile': f'{cwd}/default_galerkin_truncation_energybased_99.99999_runid_0/optional_affine_shift.txt',
                    'isAffine': False, 'algorithm': 'defaultGalerkin', 'numModes': 22,
                    'romInitialStateFile': f'{cwd}/default_galerkin_truncation_energybased_99.99999_runid_0/rom_initial_state.txt'}}
        default_galerkin_99_99999_input = os.path.join(cwd,
                                                       'default_galerkin_truncation_energybased_99.99999_runid_0',
                                                       'input.yaml')
        with open(default_galerkin_99_99999_input, 'rt') as input_yaml:
            input_yaml_dict = yaml.safe_load(input_yaml)
            print(f'===> Asserting...\n==> File: {default_galerkin_99_99999_input}')
            self.assertEqual(default_galerkin_99_99999, input_yaml_dict)
            print(f'==> Assertion passed.')

    def test_2d_swe_galerkin_hypred_1(self):
        tutorials_build_dir_name = '2d_swe_galerkin_hypred_1'
        cwd = os.path.join(self.tutorial_build_dir, 'end-to-end-roms', tutorials_build_dir_name)
        # Running wf_foms
        wf_foms_command = ['python3', self.wf_foms, '--wf', 'wf.yaml', '--dryrun', 'True']
        print(f'===> Running...\n==> Command: {" ".join(wf_foms_command)}\n==> Working dir: {cwd}')
        wf_foms_build = subprocess.run(wf_foms_command, stdout=subprocess.PIPE, stderr=subprocess.PIPE,
                                       encoding='utf-8', cwd=cwd)
        print(wf_foms_build.stdout)
        # Running wf_offline_rom
        wf_offline_rom_command = ['python3', self.wf_offline_rom, '--wf', 'wf.yaml', '--dryrun', 'True']
        print(f'===> Running...\n==> Command: {" ".join(wf_offline_rom_command)}\n==> Working dir: {cwd}')
        wf_offline_rom_build = subprocess.run(wf_offline_rom_command, stdout=subprocess.PIPE, stderr=subprocess.PIPE,
                                              encoding='utf-8', cwd=cwd)
        print(wf_offline_rom_build.stdout)
        # Running wf_galerkin
        wf_galerkin = os.path.join(tutorials_dir, 'end-to-end-roms', 'wf_galerkin.py')
        wf_galerkin_command = ['python3', wf_galerkin, '--wf', 'wf.yaml', '--dryrun', 'True']
        print(f'===> Running...\n==> Command: {" ".join(wf_galerkin_command)}\n==> Working dir: {cwd}')
        wf_offline_rom_build = subprocess.run(wf_galerkin_command, stdout=subprocess.PIPE, stderr=subprocess.PIPE,
                                              encoding='utf-8', cwd=cwd)
        print(wf_offline_rom_build.stdout)
        # Assertions
        # fom_test_runid_0
        fom_test_runid_0 = {'inviscidFluxReconstruction': 'Weno3', 'odeScheme': 'RK4', 'timeStepSize': 0.005,
                            'problem': '2d_swe', 'runId': 0, 'meshDir': f'{cwd}/fom_mesh', 'numDofsPerCell': 3,
                            'coriolis': -0.5, 'pulsemag': 0.125, 'gravity': 9.8, 'finalTime': 5.0,
                            'stateSamplingFreq': 10, 'rhsSamplingFreq': 100}
        fom_test_runid_0_input = os.path.join(cwd, 'fom_test_runid_0', 'input.yaml')
        with open(fom_test_runid_0_input, 'rt') as input_yaml:
            input_yaml_dict = yaml.safe_load(input_yaml)
            print(f'===> Asserting...\n==> File: {fom_test_runid_0_input}')
            self.assertEqual(fom_test_runid_0, input_yaml_dict)
            print(f'==> Assertion passed.')
        # fom_train_runid_0
        fom_train_runid_0 = {'inviscidFluxReconstruction': 'Weno3', 'odeScheme': 'RK4', 'timeStepSize': 0.005,
                             'problem': '2d_swe', 'runId': 0, 'meshDir': f'{cwd}/fom_mesh', 'numDofsPerCell': 3,
                             'coriolis': -1.0, 'pulsemag': 0.125, 'gravity': 9.8, 'finalTime': 5.0,
                             'stateSamplingFreq': 10, 'rhsSamplingFreq': 10}
        fom_train_runid_0_input = os.path.join(cwd, 'fom_train_runid_0', 'input.yaml')
        with open(fom_train_runid_0_input, 'rt') as input_yaml:
            input_yaml_dict = yaml.safe_load(input_yaml)
            print(f'===> Asserting...\n==> File: {fom_train_runid_0_input}')
            self.assertEqual(fom_train_runid_0, input_yaml_dict)
            print(f'==> Assertion passed.')
        # fom_train_runid_1
        fom_train_runid_1 = {'inviscidFluxReconstruction': 'Weno3', 'odeScheme': 'RK4', 'timeStepSize': 0.005,
                             'problem': '2d_swe', 'runId': 1, 'meshDir': f'{cwd}/fom_mesh', 'numDofsPerCell': 3,
                             'coriolis': 0.0, 'pulsemag': 0.125, 'gravity': 9.8, 'finalTime': 5.0,
                             'stateSamplingFreq': 10, 'rhsSamplingFreq': 10}
        fom_train_runid_1_input = os.path.join(cwd, 'fom_train_runid_1', 'input.yaml')
        with open(fom_train_runid_1_input, 'rt') as input_yaml:
            input_yaml_dict = yaml.safe_load(input_yaml)
            print(f'===> Asserting...\n==> File: {fom_train_runid_1_input}')
            self.assertEqual(fom_train_runid_1, input_yaml_dict)
            print(f'==> Assertion passed.')
        # offline_rom
        offline_rom = {'stateData': {'useTrainingRuns': 'all'}, 'rhsData': {'useTrainingRuns': 'all'}}
        offline_rom_input = os.path.join(cwd, 'offline_rom', 'pod_input.yaml')
        with open(offline_rom_input, 'rt') as input_yaml:
            input_yaml_dict = yaml.safe_load(input_yaml)
            print(f'===> Asserting...\n==> File: {offline_rom_input}')
            self.assertEqual(offline_rom, input_yaml_dict)
            print(f'==> Assertion passed.')
        # hyperreduced_galerkin_99_999
        hyperreduced_galerkin_99_999 = {
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
        hyperreduced_galerkin_99_999_input = os.path.join(cwd,
                                                     'hyperreduced_galerkin_truncation_energybased_99.999_sample_mesh_random_0.100_runid_0',
                                                     'input.yaml')
        with open(hyperreduced_galerkin_99_999_input, 'rt') as input_yaml:
            input_yaml_dict = yaml.safe_load(input_yaml)
            print(f'===> Asserting...\n==> File: {hyperreduced_galerkin_99_999_input}')
            self.assertEqual(hyperreduced_galerkin_99_999, input_yaml_dict)
            print(f'==> Assertion passed.')
        # hyperreduced_galerkin_99_99999
        hyperreduced_galerkin_99_99999 = {
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
        hyperreduced_galerkin_99_99999_input = os.path.join(cwd,
                                                       'hyperreduced_galerkin_truncation_energybased_99.99999_sample_mesh_random_0.100_runid_0',
                                                       'input.yaml')
        with open(hyperreduced_galerkin_99_99999_input, 'rt') as input_yaml:
            input_yaml_dict = yaml.safe_load(input_yaml)
            print(f'===> Asserting...\n==> File: {hyperreduced_galerkin_99_99999_input}')
            self.assertEqual(hyperreduced_galerkin_99_99999, input_yaml_dict)
            print(f'==> Assertion passed.')

    def test_2d_swe_lspg_default(self):
        tutorials_build_dir_name = '2d_swe_lspg_default'
        cwd = os.path.join(self.tutorial_build_dir, 'end-to-end-roms', tutorials_build_dir_name)
        # Running wf_foms
        wf_foms_command = ['python3', self.wf_foms, '--wf', 'wf.yaml', '--dryrun', 'True']
        print(f'===> Running...\n==> Command: {" ".join(wf_foms_command)}\n==> Working dir: {cwd}')
        wf_foms_build = subprocess.run(wf_foms_command, stdout=subprocess.PIPE, stderr=subprocess.PIPE,
                                       encoding='utf-8', cwd=cwd)
        print(wf_foms_build.stdout)
        # Running wf_offline_rom
        wf_offline_rom_command = ['python3', self.wf_offline_rom, '--wf', 'wf.yaml', '--dryrun', 'True']
        print(f'===> Running...\n==> Command: {" ".join(wf_offline_rom_command)}\n==> Working dir: {cwd}')
        wf_offline_rom_build = subprocess.run(wf_offline_rom_command, stdout=subprocess.PIPE, stderr=subprocess.PIPE,
                                              encoding='utf-8', cwd=cwd)
        print(wf_offline_rom_build.stdout)
        # Running wf_lspg
        wf_lspg = os.path.join(tutorials_dir, 'end-to-end-roms', 'wf_lspg.py')
        wf_lspg_command = ['python3', wf_lspg, '--wf', 'wf.yaml', '--dryrun', 'True']
        print(f'===> Running...\n==> Command: {" ".join(wf_lspg_command)}\n==> Working dir: {cwd}')
        wf_lspg_build = subprocess.run(wf_lspg_command, stdout=subprocess.PIPE, stderr=subprocess.PIPE,
                                       encoding='utf-8', cwd=cwd)
        print(wf_lspg_build.stdout)
        # Assertions
        # fom_test_runid_0
        fom_test_runid_0 = {'inviscidFluxReconstruction': 'Weno3', 'odeScheme': 'BDF1', 'timeStepSize': 0.01,
                            'nonlinearSolverName': 'NewtonRaphson', 'nonlinearSolverTolerance': '1e-6',
                            'problem': '2d_swe', 'runId': 0, 'meshDir': f'{cwd}/fom_mesh', 'numDofsPerCell': 3,
                            'coriolis': -0.5, 'pulsemag': 0.125, 'gravity': 9.8, 'finalTime': 5.0,
                            'stateSamplingFreq': 10}
        fom_test_runid_0_input = os.path.join(cwd, 'fom_test_runid_0', 'input.yaml')
        with open(fom_test_runid_0_input, 'rt') as input_yaml:
            input_yaml_dict = yaml.safe_load(input_yaml)
            print(f'===> Asserting...\n==> File: {fom_test_runid_0_input}')
            self.assertEqual(fom_test_runid_0, input_yaml_dict)
            print(f'==> Assertion passed.')
        # fom_train_runid_0
        fom_train_runid_0 = {'inviscidFluxReconstruction': 'Weno3', 'odeScheme': 'BDF1', 'timeStepSize': 0.01,
                             'nonlinearSolverName': 'NewtonRaphson', 'nonlinearSolverTolerance': '1e-6',
                             'problem': '2d_swe', 'runId': 0, 'meshDir': f'{cwd}/fom_mesh', 'numDofsPerCell': 3,
                             'coriolis': -1.0, 'pulsemag': 0.125, 'gravity': 9.8, 'finalTime': 5.0,
                             'stateSamplingFreq': 10}
        fom_train_runid_0_input = os.path.join(cwd, 'fom_train_runid_0', 'input.yaml')
        with open(fom_train_runid_0_input, 'rt') as input_yaml:
            input_yaml_dict = yaml.safe_load(input_yaml)
            print(f'===> Asserting...\n==> File: {fom_train_runid_0_input}')
            self.assertEqual(fom_train_runid_0, input_yaml_dict)
            print(f'==> Assertion passed.')
        # fom_train_runid_1
        fom_train_runid_1 = {'inviscidFluxReconstruction': 'Weno3', 'odeScheme': 'BDF1', 'timeStepSize': 0.01,
                             'nonlinearSolverName': 'NewtonRaphson', 'nonlinearSolverTolerance': '1e-6',
                             'problem': '2d_swe', 'runId': 1, 'meshDir': f'{cwd}/fom_mesh', 'numDofsPerCell': 3,
                             'coriolis': 0.0, 'pulsemag': 0.125, 'gravity': 9.8, 'finalTime': 5.0,
                             'stateSamplingFreq': 10}
        fom_train_runid_1_input = os.path.join(cwd, 'fom_train_runid_1', 'input.yaml')
        with open(fom_train_runid_1_input, 'rt') as input_yaml:
            input_yaml_dict = yaml.safe_load(input_yaml)
            print(f'===> Asserting...\n==> File: {fom_train_runid_1_input}')
            self.assertEqual(fom_train_runid_1, input_yaml_dict)
            print(f'==> Assertion passed.')
        # offline_rom
        offline_rom = {'stateData': {'useTrainingRuns': 'all'}}
        offline_rom_input = os.path.join(cwd, 'offline_rom', 'pod_input.yaml')
        with open(offline_rom_input, 'rt') as input_yaml:
            input_yaml_dict = yaml.safe_load(input_yaml)
            print(f'===> Asserting...\n==> File: {offline_rom_input}')
            self.assertEqual(offline_rom, input_yaml_dict)
            print(f'==> Assertion passed.')
        # default_lspg_99_999
        default_lspg_99_999 = {
            'inviscidFluxReconstruction': 'Weno3', 'odeScheme': 'BDF1', 'timeStepSize': 0.01,
            'nonlinearSolverName': 'GaussNewton', 'nonlinearSolverTolerance': '1e-6', 'problem': '2d_swe', 'runId': 0,
            'meshDir': f'{cwd}/fom_mesh', 'numDofsPerCell': 3, 'coriolis': -0.5, 'pulsemag': 0.125, 'gravity': 9.8,
            'finalTime': 5.0, 'stateSamplingFreq': 10,
            'rom': {'fullMeshPodFile': f'{cwd}/offline_rom/state_left_singular_vectors.bin',
                    'affineShiftFile': f'{cwd}/default_lspg_truncation_energybased_99.999_runid_0/optional_affine_shift.txt',
                    'isAffine': False, 'algorithm': 'defaultLspg', 'numModes': 22,
                    'romInitialStateFile': f'{cwd}/default_lspg_truncation_energybased_99.999_runid_0/rom_initial_state.txt'}}
        default_lspg_99_999_input = os.path.join(cwd,
                                                 'default_lspg_truncation_energybased_99.999_runid_0',
                                                 'input.yaml')
        with open(default_lspg_99_999_input, 'rt') as input_yaml:
            input_yaml_dict = yaml.safe_load(input_yaml)
            print(f'===> Asserting...\n==> File: {default_lspg_99_999_input}')
            self.assertEqual(default_lspg_99_999, input_yaml_dict)
            print(f'==> Assertion passed.')
        # default_lspg_99_99999
        default_lspg_99_99999 = {
            'inviscidFluxReconstruction': 'Weno3', 'odeScheme': 'BDF1', 'timeStepSize': 0.01,
            'nonlinearSolverName': 'GaussNewton', 'nonlinearSolverTolerance': '1e-6', 'problem': '2d_swe', 'runId': 0,
            'meshDir': f'{cwd}/fom_mesh', 'numDofsPerCell': 3, 'coriolis': -0.5, 'pulsemag': 0.125, 'gravity': 9.8,
            'finalTime': 5.0, 'stateSamplingFreq': 10,
            'rom': {'fullMeshPodFile': f'{cwd}/offline_rom/state_left_singular_vectors.bin',
                    'affineShiftFile': f'{cwd}/default_lspg_truncation_energybased_99.99999_runid_0/optional_affine_shift.txt',
                    'isAffine': False, 'algorithm': 'defaultLspg', 'numModes': 22,
                    'romInitialStateFile': f'{cwd}/default_lspg_truncation_energybased_99.99999_runid_0/rom_initial_state.txt'}}
        default_lspg_99_99999_input = os.path.join(cwd,
                                                   'default_lspg_truncation_energybased_99.99999_runid_0',
                                                   'input.yaml')
        with open(default_lspg_99_99999_input, 'rt') as input_yaml:
            input_yaml_dict = yaml.safe_load(input_yaml)
            print(f'===> Asserting...\n==> File: {default_lspg_99_99999_input}')
            self.assertEqual(default_lspg_99_99999, input_yaml_dict)
            print(f'==> Assertion passed.')

    def test_2d_swe_lspg_hypred_1(self):
        tutorials_build_dir_name = '2d_swe_lspg_hypred_1'
        cwd = os.path.join(self.tutorial_build_dir, 'end-to-end-roms', tutorials_build_dir_name)
        # Running wf_foms
        wf_foms_command = ['python3', self.wf_foms, '--wf', 'wf.yaml', '--dryrun', 'True']
        print(f'===> Running...\n==> Command: {" ".join(wf_foms_command)}\n==> Working dir: {cwd}')
        wf_foms_build = subprocess.run(wf_foms_command, stdout=subprocess.PIPE, stderr=subprocess.PIPE,
                                       encoding='utf-8', cwd=cwd)
        print(wf_foms_build.stdout)
        # Running wf_offline_rom
        wf_offline_rom_command = ['python3', self.wf_offline_rom, '--wf', 'wf.yaml', '--dryrun', 'True']
        print(f'===> Running...\n==> Command: {" ".join(wf_offline_rom_command)}\n==> Working dir: {cwd}')
        wf_offline_rom_build = subprocess.run(wf_offline_rom_command, stdout=subprocess.PIPE, stderr=subprocess.PIPE,
                                              encoding='utf-8', cwd=cwd)
        print(wf_offline_rom_build.stdout)
        # Running wf_lspg
        wf_lspg = os.path.join(tutorials_dir, 'end-to-end-roms', 'wf_lspg.py')
        wf_lspg_command = ['python3', wf_lspg, '--wf', 'wf.yaml', '--dryrun', 'True']
        print(f'===> Running...\n==> Command: {" ".join(wf_lspg_command)}\n==> Working dir: {cwd}')
        wf_lspg_build = subprocess.run(wf_lspg_command, stdout=subprocess.PIPE, stderr=subprocess.PIPE,
                                       encoding='utf-8', cwd=cwd)
        print(wf_lspg_build.stdout)
        # Assertions
        # fom_test_runid_0
        fom_test_runid_0 = {'inviscidFluxReconstruction': 'Weno3', 'odeScheme': 'BDF1', 'timeStepSize': 0.01,
                            'nonlinearSolverName': 'NewtonRaphson', 'nonlinearSolverTolerance': '1e-6',
                            'problem': '2d_swe', 'runId': 0, 'meshDir': f'{cwd}/fom_mesh',
                            'numDofsPerCell': 3, 'coriolis': -0.5, 'pulsemag': 0.125, 'gravity': 9.8, 'finalTime': 5.0,
                            'stateSamplingFreq': 10}
        fom_test_runid_0_input = os.path.join(cwd, 'fom_test_runid_0', 'input.yaml')
        with open(fom_test_runid_0_input, 'rt') as input_yaml:
            input_yaml_dict = yaml.safe_load(input_yaml)
            print(f'===> Asserting...\n==> File: {fom_test_runid_0_input}')
            self.assertEqual(fom_test_runid_0, input_yaml_dict)
            print(f'==> Assertion passed.')
        # fom_train_runid_0
        fom_train_runid_0 = {'inviscidFluxReconstruction': 'Weno3', 'odeScheme': 'BDF1', 'timeStepSize': 0.01,
                             'nonlinearSolverName': 'NewtonRaphson', 'nonlinearSolverTolerance': '1e-6',
                             'problem': '2d_swe', 'runId': 0, 'meshDir': f'{cwd}/fom_mesh', 'numDofsPerCell': 3,
                             'coriolis': -1.0, 'pulsemag': 0.125, 'gravity': 9.8, 'finalTime': 5.0,
                             'stateSamplingFreq': 10}
        fom_train_runid_0_input = os.path.join(cwd, 'fom_train_runid_0', 'input.yaml')
        with open(fom_train_runid_0_input, 'rt') as input_yaml:
            input_yaml_dict = yaml.safe_load(input_yaml)
            print(f'===> Asserting...\n==> File: {fom_train_runid_0_input}')
            self.assertEqual(fom_train_runid_0, input_yaml_dict)
            print(f'==> Assertion passed.')
        # fom_train_runid_1
        fom_train_runid_1 = {'inviscidFluxReconstruction': 'Weno3', 'odeScheme': 'BDF1', 'timeStepSize': 0.01,
                             'nonlinearSolverName': 'NewtonRaphson', 'nonlinearSolverTolerance': '1e-6',
                             'problem': '2d_swe', 'runId': 1, 'meshDir': f'{cwd}/fom_mesh', 'numDofsPerCell': 3,
                             'coriolis': 0.0, 'pulsemag': 0.125, 'gravity': 9.8, 'finalTime': 5.0,
                             'stateSamplingFreq': 10}
        fom_train_runid_1_input = os.path.join(cwd, 'fom_train_runid_1', 'input.yaml')
        with open(fom_train_runid_1_input, 'rt') as input_yaml:
            input_yaml_dict = yaml.safe_load(input_yaml)
            print(f'===> Asserting...\n==> File: {fom_train_runid_1_input}')
            self.assertEqual(fom_train_runid_1, input_yaml_dict)
            print(f'==> Assertion passed.')
        # offline_rom
        offline_rom = {'stateData': {'useTrainingRuns': 'all'}}
        offline_rom_input = os.path.join(cwd, 'offline_rom', 'pod_input.yaml')
        with open(offline_rom_input, 'rt') as input_yaml:
            input_yaml_dict = yaml.safe_load(input_yaml)
            print(f'===> Asserting...\n==> File: {offline_rom_input}')
            self.assertEqual(offline_rom, input_yaml_dict)
            print(f'==> Assertion passed.')
        # default_lspg_99_999
        default_lspg_99_999 = {
            'inviscidFluxReconstruction': 'Weno3', 'odeScheme': 'BDF1', 'timeStepSize': 0.01,
            'nonlinearSolverName': 'GaussNewton', 'nonlinearSolverTolerance': '1e-6', 'problem': '2d_swe', 'runId': 0,
            'meshDir': f'{cwd}/offline_rom/sample_mesh_random_0.100', 'numDofsPerCell': 3, 'coriolis': -0.5,
            'pulsemag': 0.125, 'gravity': 9.8, 'finalTime': 5.0, 'stateSamplingFreq': 10,
            'rom': {'fullMeshPodFile': f'{cwd}/offline_rom/state_left_singular_vectors.bin',
                    'affineShiftFile': f'{cwd}/hyperreduced_lspg_truncation_energybased_99.999_sample_mesh_random_0.100_runid_0/optional_affine_shift.txt',
                    'isAffine': False, 'algorithm': 'hyperreducedLspg', 'numModes': 22,
                    'romInitialStateFile': f'{cwd}/hyperreduced_lspg_truncation_energybased_99.999_sample_mesh_random_0.100_runid_0/rom_initial_state.txt',
                    'stencilMeshGidsFile': f'{cwd}/offline_rom/sample_mesh_random_0.100/stencil_mesh_gids.dat',
                    'sampleMeshGidsFile': f'{cwd}/offline_rom/sample_mesh_random_0.100/sample_mesh_gids.txt'}}
        default_lspg_99_999_input = os.path.join(cwd,
                                                 'hyperreduced_lspg_truncation_energybased_99.999_sample_mesh_random_0.100_runid_0',
                                                 'input.yaml')
        with open(default_lspg_99_999_input, 'rt') as input_yaml:
            input_yaml_dict = yaml.safe_load(input_yaml)
            print(f'===> Asserting...\n==> File: {default_lspg_99_999_input}')
            self.assertEqual(default_lspg_99_999, input_yaml_dict)
            print(f'==> Assertion passed.')
        # default_lspg_99_99999
        default_lspg_99_99999 = {
            'inviscidFluxReconstruction': 'Weno3', 'odeScheme': 'BDF1', 'timeStepSize': 0.01,
            'nonlinearSolverName': 'GaussNewton', 'nonlinearSolverTolerance': '1e-6', 'problem': '2d_swe', 'runId': 0,
            'meshDir': f'{cwd}/offline_rom/sample_mesh_random_0.100', 'numDofsPerCell': 3, 'coriolis': -0.5,
            'pulsemag': 0.125, 'gravity': 9.8, 'finalTime': 5.0, 'stateSamplingFreq': 10,
            'rom': {'fullMeshPodFile': f'{cwd}/offline_rom/state_left_singular_vectors.bin',
                    'affineShiftFile': f'{cwd}/hyperreduced_lspg_truncation_energybased_99.99999_sample_mesh_random_0.100_runid_0/optional_affine_shift.txt',
                    'isAffine': False, 'algorithm': 'hyperreducedLspg', 'numModes': 22,
                    'romInitialStateFile': f'{cwd}/hyperreduced_lspg_truncation_energybased_99.99999_sample_mesh_random_0.100_runid_0/rom_initial_state.txt',
                    'stencilMeshGidsFile': f'{cwd}/offline_rom/sample_mesh_random_0.100/stencil_mesh_gids.dat',
                    'sampleMeshGidsFile': f'{cwd}/offline_rom/sample_mesh_random_0.100/sample_mesh_gids.txt'}}
        default_lspg_99_99999_input = os.path.join(cwd,
                                                   'hyperreduced_lspg_truncation_energybased_99.99999_sample_mesh_random_0.100_runid_0',
                                                   'input.yaml')
        with open(default_lspg_99_99999_input, 'rt') as input_yaml:
            input_yaml_dict = yaml.safe_load(input_yaml)
            print(f'===> Asserting...\n==> File: {default_lspg_99_99999_input}')
            self.assertEqual(default_lspg_99_99999, input_yaml_dict)
            print(f'==> Assertion passed.')


if __name__ == "__main__":
    build_tutorials()
    unittest.main()
