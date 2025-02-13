
.. include:: mydefs.rst

Pressio Tutorials
=================

Tutorials suite for the `pressio C++ library <https://pressio.github.io/pressio>`_.

..
   Pressio was started with a focus on projection-based
   reduced-order models (ROMs), which is inherently multidisciplinary.
   Working on a production-level ROM capability inevitably means spanning
   multiple domains from, e.g., linear algebra, nonlinear solvers
   and optimization, to time integration, generic programming, and distributed computing/HPC.
   This is clearly reflected in the `structure of pressio <https://pressio.github.io/pressio>`__:
   each component focuses on a specific capability and depends
   on the ones below it. This allows users to use a component directly, but, as a whole,
   the full stack constitutes the foundation of the top-level ``pressio/rom``.
   This tutorial doc shows the core capabilities of pressio, for example ODE integration, nonlinear solvers, etc.

Building
--------

You need:

- **CMake > 3.18.0**

- **C++17 compiler**: GCC or Clang are tested in our CI

- Python 3.x

and the steps are:

.. code-block:: bash

   git clone git@github.com:Pressio/pressio-tutorials.git

   export CXX=<fullpath-to-your-CXX-compiler>
   export BUILDDIR=$HOME/tutorialBuild
   mkdir $BUILDDIR
   cmake -DCMAKE_BUILD_TYPE=Release -S <path-to-your-cloned-repo> -B $BUILDDIR
   cd $BUILDDIR && make -j4

   # ensure you have all required Python packages installed
   pip3 install <path-to-your-cloned-repo>/py_requirements.txt

Miscellanea
-----------

This is work-in-progress.
The BSD-3 license is available `here <https://pressio.github.io/license.html>`_.

We are working on publishing this: you can find our arXiv preprint at: https://arxiv.org/abs/2003.07798

Questions?
----------

Find us on `Slack <https://pressioteam.slack.com>`_.

.. toctree::
   :maxdepth: 0
   :hidden:
   :caption: tutorials

   ./using_eigen/nonlinsolvers1
   ./using_eigen/nonlinsolvers2
   ./using_eigen/ode1
   ./using_eigen/ode2
   ./using_eigen/ode3

.. toctree::
   :maxdepth: 1
   :hidden:
   :caption: Various

   Join our Slack <https://pressioteam.slack.com>
   GitHub Repo <https://github.com/Pressio/pressio-tutorials>
   Open an issue/feature req. <https://github.com/Pressio/pressio-tutorials/issues>
   license
