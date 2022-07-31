
Building Instructions
=====================

C++ tutorials
-------------

You only need CMake > 3.18.0 and a C++14 compliant compiler.

.. code-block:: bash

   export CXX=<fullpath-to-your-CXX-compiler>
   git clone --recursive git@github.com:Pressio/pressio-tutorials.git
   cd pressio-tutorials && mkdir build && cd build
   cmake -DCMAKE_BUILD_TYPE=Release ..
   make -j4

Individual executables for all tutorials are built inside the build subdirectory.


pressio4py tutorials
--------------------

bla bla

