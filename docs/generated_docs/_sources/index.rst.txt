
Pressio Tutorials
=====================

Tutorials suite covering the `pressio C++ library <https://pressio.github.io/pressio/html/index.html>`_ and `pressio4py <https://pressio.github.io/pressio4py/html/index.html>`_.

.. admonition:: This is work-in-progress
   :class: caution
   
   We are actively working on adding more.
   If you don't find something, you can temporarily look at the `C++ tests 
   subdirectory <https://github.com/Pressio/pressio/tree/main/tests>`_, 
   which contains tests that are readable, but are obviously not formatted in a tutorial fashion.


Organization 
------------

This website has three main sections:

1. **C++ tutorials for data types that pressio already supports internally**

   This section covers usecase of data types that pressio already knows how to operate (efficiently) on. Currently, these include data types from these libraries: Eigen, Kokkos, and Trilinos. We will add more over time.
   For example, you have an application built on top of Trilinos linear algebra data structure.


2. **C++ tutorials for data types that pressio does not yet support**

   This section covers arbitrary data types that pressio does not know how to manipulate or perform operations on, so you as a user are responsible to provide all the necessary operations and info to do so. This is actually an advantage from a certain viewpoint, since it gives you *full control* to make all the operations needed as efficient as possible.


3. **Python tutorials for using pressio4py**

   This section covers the Python tutorials which rely on numpy.


License and Citation
--------------------

The BSD-3 license is available `here <https://pressio.github.io/various/license/>`_.

We are working on publishing this: you can find our arXiv preprint at: https://arxiv.org/abs/2003.07798


Questions?
----------

Find us on `Slack <https://pressioteam.slack.com>`_.

.. toctree::
   :maxdepth: 1
   :hidden:

   build

.. toctree::
   :maxdepth: 0
   :hidden:
   :caption: C++ for Supported Data Types

   ./known_types/nonlinearsolvers
   ./known_types/ode

.. toctree::
   :maxdepth: 0
   :hidden:
   :caption: C++ for Arbitrary Data Types

   ./arbitrary_types/nonlinearsolvers
   ./arbitrary_types/ode


.. toctree::
   :maxdepth: 0
   :hidden:
   :caption: pressio4py

   ./pressio4py/dummy


.. toctree::
   :caption: Miscellanea
   :maxdepth: 1
   :hidden:

   GitHub Repo <https://github.com/Pressio/pressio-tutorials>
   Open an issue/feature req. <https://github.com/Pressio/pressio-tutorials/issues>
   license
