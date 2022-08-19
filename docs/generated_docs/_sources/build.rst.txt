
Dependencies and build instructions
###################################

If in a hurry and just want to build...
=======================================

.. code-block:: bash

   export CXX=<fullpath-to-your-C++14-compiler>
   git clone --recursive git@github.com:Pressio/pressio-tutorials.git
   cd pressio-tutorials && mkdir build && cd build
   cmake -DCMAKE_BUILD_TYPE=Release ..
   make -j4


If you want more details
========================

Dependencies
------------

The pressio-tutorials have the following dependencies:



More specifically, we have the following scope

- pressio: contains the C++ code that this tutorials repo convers
- pressio-demoapps: provides a suite of physics problems that are used in the tutorials to demonstrate the functionalities of pressio
- Eigen: is the data structure library that pressio-demoapps rely on
- Python: is used in pressio-demoapps to implement all the ROM workflow drivers

Building
--------

The pressio-tutorials are designed to be easy and quick
to build. Thefore, to make your life easy, all dependencies
are already shipped with the repo, and to build the code
you ONLY need CMake > 3.18.0 and a C++14 compliant compiler.

.. code-block:: bash

   export CXX=<fullpath-to-your-CXX-compiler>
   git clone --recursive git@github.com:Pressio/pressio-tutorials.git
   cd pressio-tutorials && mkdir build && cd build
   cmake -DCMAKE_BUILD_TYPE=Release ..
   make -j4

Individual executables for all tutorials are built inside the build subdirectory.
