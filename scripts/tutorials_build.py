import os
import sys

try:
    tutorials_dir = f'{os.sep}'.join(os.path.abspath(__file__).split(os.sep)[:-4])
    sys.path.append(tutorials_dir)
except Exception as e:
    print(f'Can not add project path to system path! Exiting!\nERROR: {e}')
    raise SystemExit(1)

import subprocess


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


if __name__ == "__main__":
    build_tutorials()
