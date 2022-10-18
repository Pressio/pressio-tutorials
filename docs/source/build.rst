
Build instructions
##################

These tutorials should be easy and quick to build, follow below.

What you need
-------------

- **CMake > 3.18.0**

- **C++14 compiler**: `our CI <https://github.com/Pressio/pressio-tutorials/blob/develop/.github/workflows/test-all.yml>`__ is currently testing both GCC and Clang

- Python 3.x with specific packages listed `here <../../py_requirements.txt>`__

Steps
-----

.. code-block:: bash

   git clone --recursive git@github.com:Pressio/pressio-tutorials.git

   export CXX=<fullpath-to-your-CXX-compiler>
   export BUILDDIR=$HOME/tutorialBuild
   mkdir $BUILDDIR
   cmake -DCMAKE_BUILD_TYPE=Release -S <path-to-your-cloned-repo> -B $BUILDDIR
   cd $BUILDDIR && make -j4

   # if needed, the Python packages can be installed as
   pip3 install <path-to-your-cloned-repo>/py_requirements.txt


Then what?
----------

Individual executables and the end-to-end demos can be found inside the build directory.

.. tip::

   If you want to verify the build, you can enable tests via ``-DPRESSIOTUTORIALS_ENABLE_TESTS=ON``,
   and then do:

   .. code-block:: cpp

      cd $BUIlDDIR
      ctest


More details on dependencies
----------------------------

The tutorials have the following dependencies:

- pressio C++ library: contains the C++ code that this tutorials repo convers
- pressio-demoapps: provides a suite of physics problems that are used in the tutorials to demonstrate the functionalities of pressio
- Eigen: is the library used by pressio-demoapps
- Python: is used in pressio-demoapps to implement all the ROM workflow drivers
