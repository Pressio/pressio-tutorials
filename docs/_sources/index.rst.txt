
Pressio C++ Tutorials
=====================

Tutorials suite covering the `pressio C++ library <https://pressio.github.io/pressio/html/index.html>`_.

.. admonition:: This is still work-in-progress, some coverage might be missing.
   :class: caution
   
   We are actively working on adding more.
   If you don't find something, you can temporarily look at the `C++ tests 
   subdirectory <https://github.com/Pressio/pressio/tree/main/tests>`_, 
   which contains tests that are readable, but are obviously not formatted in a tutorial fashion.

Organization and structure
--------------------------

| The key thing to notice is that there are two main toplevel sections:

.. toctree::
    :maxdepth: 1

    known_types
    arbitrary_types

| The reason behind this is that it reflects two categories of users:

.. admonition:: users relying on data types/structures that pressio already supports internally
   :class: important

   You are in this category if you use data types/structures that pressio already knows how to operate on efficiently. Currently, these include data structures from these libraries: Eigen, Kokkos, and Trilinos. As we progress, we will add more.
   For example, you have an application built on top of Trilinos linear algebra data structure.

   If you are in this category, :ref:`start exploring here <knowntypesanchor>`.


.. admonition:: users relying on data types/structures *not yet known* to pressio

   This refers to truly arbitrary data types that pressio does not know how to manipulate or perform operations on, so the user is responsible to provide all the necessary operations and info to do so. This is actually an advantage from a certain viewpoint, since it gives you *full control* to make all the operations needed as efficient as possible.

   If you are in this category, :ref:`start exploring here <arbtypesanchor>`.




Build the tutorials!
--------------------

You only need CMake > 3.18.0 and a C++14 compliant compiler.

.. code-block:: bash

   export CXX=<fullpath-to-your-CXX-compiler>
   git clone --recursive git@github.com:Pressio/pressio-tutorials.git
   cd pressio-tutorials && mkdir build && cd build
   cmake -DCMAKE_BUILD_TYPE=Release ..
   make -j4

Individual executables for all tutorials are built inside the build subdirectory.



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
