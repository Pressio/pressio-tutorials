
.. include:: mydefs.rst

Pressio Tutorials
=====================

Tutorials suite for the `pressio C++ library <https://pressio.github.io/pressio/html/index.html>`_.


Scope and Motivation
--------------------

These tutorials are intended to show users how to *integrate* together various
capabilities of pressio to do something meaningful, as well as unveil some details
that might not be obvious.

We believe such an approach on writing tutorials is useful in general,
but even more so for pressio. Pressio was started with a focus on projection-based
reduced-order models (ROMs), which is a strongly multidisciplinary topic.
Working on ROMs means inevitably touching multiple fields, ranging from, e.g.,
linear algebra, nonlinear solvers and optimization, to distributed computing, HPC and time integration.
This is clearly reflected in the structure of pressio, where several components
(all usable independently) offer different capabilities, but as a whole
constitute the pyramidal foundation of the ``pressio/rom`` component.
Describing how to use pressio for ROMs cannot be simply boiled down to presenting the API.
Several steps are involved, and the best way to understand them is via
examples showing the logical progression.
Therefore, we have organized this website in two main parts: one the one hand
we have short, independent tutorials on the foundational capabilities that
can are easily explained in a self-contained fashion, for example nonlinear
solvers and ODE integration; on the other hand, we have more
`complete workflow-based end-to-end demos <endtoend/readthisfirst.html>`__
to demonstrate in practice what pressio offers for ROMs.


.. admonition:: This is work-in-progress
   :class: caution

   We are actively working on adding more.
   If you don't find something, you can temporarily look at the `C++ tests
   subdirectory <https://github.com/Pressio/pressio/tree/main/tests>`_,
   which contains tests that are readable, but are obviously not formatted in a tutorial fashion.

..
   This website has these main sections :red:`TBD`

   1. **C++ tutorials for data types that pressio already supports internally**

      This section covers data types that pressio already knows how to operate (efficiently) on.
      Currently, these data types include Eigen, Kokkos, and Trilinos. We will add more over time.

      - **End-to-end ROM demos using pressio-demoapps**: Note that in here you can also find
	end-to-end ROM demos/workflows using the miniapps inside pressio-demoapps.

   2. **C++ tutorials for data types that pressio does not yet support**

      This section covers arbitrary data types that pressio does not know how to manipulate or perform operations on, so you as a user are responsible to provide all the necessary operations and info to do so. This is actually an advantage from a certain viewpoint, since it gives you *full control* to make all the operations needed as efficient as possible.


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
   GitHub Repo <https://github.com/Pressio/pressio-tutorials>
   Open an issue/feature req. <https://github.com/Pressio/pressio-tutorials/issues>
   license

.. toctree::
   :maxdepth: 0
   :hidden:
   :caption: End-to-end ROMs using pressio-demoapps

   ./endtoend/readthisfirst
   ./endtoend/templateyaml
   ./endtoend/swe_galerkin_default
   ./endtoend/swe_galerkin_gappy

.. toctree::
   :maxdepth: 0
   :hidden:
   :caption: Self-contained tutorials using Eigen data types

   ./comingsoon

.. toctree::
   :maxdepth: 0
   :hidden:
   :caption: Advanced Topics

   ./comingsoon
