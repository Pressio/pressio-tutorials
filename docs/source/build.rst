
Build instructions
##################

These tutorials should be easy and quick to build, follow below.

What you need
-------------

- **CMake > 3.18.0**

- **C++17 compiler**: GCC or Clang are tested in our CI

- Python 3.x

Steps
-----

.. code-block:: bash

   git clone git@github.com:Pressio/pressio-tutorials.git

   export CXX=<fullpath-to-your-CXX-compiler>
   export BUILDDIR=$HOME/tutorialBuild
   mkdir $BUILDDIR
   cmake -DPRESSIOTUTORIALS_ENABLE_TESTS=ON \
         -DCMAKE_BUILD_TYPE=Release -S <path-to-your-cloned-repo> -B $BUILDDIR
   cd $BUILDDIR && make -j4

   # ensure you have all required Python packages installed
   pip3 install <path-to-your-cloned-repo>/py_requirements.txt


Verify the build
----------------

.. code-block:: cpp

   cd $BUIlDDIR
   ctest

Then what?
----------

Individual executables and the end-to-end demos can be found inside the build directory.


More details on dependencies
----------------------------

The tutorials have the following dependencies:

- pressio-rom C++ library: contains the C++ code that this tutorials repo covers
- pressio-demoapps: provides a suite of physics problems that are used in the tutorials to demonstrate the functionalities of pressio
- Eigen: is the library used by pressio-demoapps
- Python: is used in pressio-demoapps to implement all the ROM workflow drivers

During configuration, all Pressio repositories will be automatically cloned inside of the tutorials directory.
