
Build instructions
##################

The pressio tutorials are designed to be easy and quick to build.
Thefore, all C++ dependencies **are already snapshotted inside the repo**,
and to build the code you ONLY need **CMake > 3.18.0** and a **C++14 compliant compiler**.

.. code-block:: bash

   git clone --recursive git@github.com:Pressio/pressio-tutorials.git

   export CXX=<fullpath-to-your-CXX-compiler>
   export BUILDDIR=$HOME/tutorialBuild
   mkdir $BUILDDIR
   cmake -DCMAKE_BUILD_TYPE=Release -S <path-to-your-cloned-repo> -B $BUILDDIR
   cd $BUILDDIR && make -j4

Executables for all tutorials and the end-to-end demos
can be found inside the build directory.

.. tip::

   If you want to verify your build, you can enable the tests via
   the Cmake variable ``-DPRESSIOTUTORIALS_ENABLE_TESTS=ON``,
   and then run the tests as:

   .. code-block:: cpp

      cd $BUIlDDIR
      ctest

The Python requirements can be found `here <../../end-to-end-roms/requirements.txt>`__
and can be installed via ``pip3 install requirements.txt``


Dependencies
------------

If you are interested, these tutorials have the following dependencies:

- pressio: contains the C++ code that this tutorials repo convers
- pressio-demoapps: provides a suite of physics problems that are used in the tutorials to demonstrate the functionalities of pressio
- Eigen: is the data structure library that pressio-demoapps rely on
- Python: is used in pressio-demoapps to implement all the ROM workflow drivers
