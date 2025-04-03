
.. include:: mydefs.rst

Pressio Tutorials
=====================

Tutorials suite for the `pressio-rom C++ library <https://pressio.github.io/pressio-rom>`_.


Scope and Motivation
--------------------

Pressio was started with a focus on projection-based
reduced-order models (ROMs), which is a strongly multidisciplinary topic.
Working on a production-level ROM capability inevitably means touching
multiple fields, ranging from, e.g., linear algebra, nonlinear solvers
and optimization, to time integration, generic programming, and distributed computing/HPC.
This is clearly reflected in the `structure of pressio-rom <https://pressio.github.io/pressio-rom>`__:
each component handles a specific capability and depends
on the ones below it. This allows users to use a component directly, but, as a whole,
the full stack constitutes the foundation of the top-level ``pressio-rom/rom``.

Describing how to use Pressio for ROMs cannot be simply reduced to presenting the API.
Several steps are involved, and the best way to understand them is via examples.
Therefore, these tutorials are designed to show how to *integrate* various
capabilities of Pressio to do something meaningful, as well as unveil
some details that might not be obvious, and are organized into these sections:

1. End-to-end ROMs using pressio-demoapps
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Complete workflow-based end-to-end demos to demonstrate *in practice* what
pressio offers for ROMs, and to allow you to easily experiment with it

2. Self-contained tutorials using Eigen data types
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Collection of short, independent tutorials on the foundational capabilities
that are easily explained in a self-contained fashion, for example ODE integration
and nonlinear solvers

3. Advance topics
^^^^^^^^^^^^^^^^^

TBD

|

Miscellanea
-----------

This is work-in-progress.
The BSD-3 license is available `here <https://pressio.github.io/license.html>`_.

We are working on publishing this: you can find our arXiv preprint at: https://arxiv.org/abs/2003.07798

Questions?
----------

Find us on `Slack <https://pressioteam.slack.com>`_.

.. toctree::
   :maxdepth: 1
   :hidden:

   build
   Join our Slack <https://pressioteam.slack.com>
   GitHub Repo <https://github.com/Pressio/pressio-tutorials>
   Open an issue/feature req. <https://github.com/Pressio/pressio-tutorials/issues>
   license


.. toctree::
   :maxdepth: 0
   :hidden:
   :caption: 1. End-to-end ROMs using pressio-demoapps

   ./endtoend/readthisfirst
   ./endtoend/swe_galerkin_default
   ./endtoend/swe_galerkin_hypred_1
   ./endtoend/swe_lspg_default
   ./endtoend/swe_lspg_hypred_1

.. toctree::
   :maxdepth: 0
   :hidden:
   :caption: 2. self-contained tutorials using Eigen data types

   ./using_eigen/nonlinsolvers1
   ./using_eigen/nonlinsolvers2
   ./using_eigen/ode1
   ./using_eigen/ode2
   ./using_eigen/ode3

.. toctree::
   :maxdepth: 0
   :hidden:
   :caption: 3. Advanced Topics

   ./advanced/comingsoon
