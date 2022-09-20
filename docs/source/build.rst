
Build instructions
##################

The pressio-tutorials are designed to be easy and quick
to build. Thefore, to make your life easy, all dependencies
**are already snapshotted inside the repo**, and to build the code
you ONLY need CMake > 3.18.0 and a C++14 compliant compiler.

.. code-block:: bash

   export CXX=<fullpath-to-your-CXX-compiler>
   git clone --recursive git@github.com:Pressio/pressio-tutorials.git

   export BUILDDIR=$HOME/tutorialBuild
   mkdir $BUILDDIR
   cmake -DCMAKE_BUILD_TYPE=Release -S <path-to-your-tutorials-repo> -B $BUILDDIR
   cd $BUILDDIR && make -j4

Individual executables for all tutorials are built inside the build subdirectory.


Dependencies
------------

If you want to know, these are the dependencies:

- pressio: contains the C++ code that this tutorials repo convers
- pressio-demoapps: provides a suite of physics problems that are used in the tutorials to demonstrate the functionalities of pressio
- Eigen: is the data structure library that pressio-demoapps rely on
- Python: is used in pressio-demoapps to implement all the ROM workflow drivers
