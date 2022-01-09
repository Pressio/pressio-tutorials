
Pressio C++ Tutorials
=====================

Tutorials suite covering the `pressio C++ library <https://pressio.github.io/pressio/html/index.html>`_.

.. Warning:: 

   This is still work-in-progress, some coverage might be missing.
   We are actively working on adding more.
   If you don't find something, you can temporarily look at the `C++ tests 
   subdirectory <https://github.com/Pressio/pressio/tree/main/tests>`_. 
   These tests are readable, 
   but are obviously not formatted in a tutorial fashion.


Try yourself!
-------------

You only need CMake > 3.18.0 and a C++14 compliant compiler.

.. code-block::

   git clone --recursive git@github.com:Pressio/pressio-tutorials.git
   export CXX=<path-to-your-CXX-compiler>
   cd pressio-tutorials && mkdir build && cd build
   cmake -DCMAKE_BUILD_TYPE=Release ..
   make -j4

Individual executables for all tutorials are built inside the build subdirectory.


Organization and structure
--------------------------

These tutorials are organized to closely reflect the modularity/structure of the code. 
For each "component" of pressio, we provide here code and discussions to demonstrate the corresponding functionalities. More specifically, we also separate the usage when using known data types (i.e., data types that pressio already knows how to operate on) from the one based on arbitrary data types (i.e. data types that pressio does not know how to manipulate, so the user is responsible to provide all the info needed). 
This way, we hope that users can easily pick up what they need.

.. toctree::
    :maxdepth: 3

    nonlinearsolvers
    ode
    proms


|

License and Citation
--------------------

The BSD-3 license is available `here <https://pressio.github.io/various/license/>`_.

We are working on publishing this. 
A previous arXiv `preprint <https://arxiv.org/abs/2003.07798>`_.


Questions?
----------

Find us on `Slack <https://pressioteam.slack.com>`_.

.. toctree::
    :maxdepth: 0

    GitHub Repo <https://github.com/Pressio/pressio-tutorials>
    Open an issue/feature req. <https://github.com/Pressio/pressio-tutorials/issues>
