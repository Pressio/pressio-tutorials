Quick Start
===========

These instructions assume the following env variables
are set:

.. code-block:: bash

   export SRC=<path-to-your-cloned-repo>
   export BUILD=<path-to-your-build-directory>

The main driver for the tutorial is defined in
``$SRC/full-tutorial/burgers.cpp``.
It is fully commented to help you understand each
step of the implementation.


Additionally, a full discussion of the tutorial can be found
in the :doc:`understand` page.

Build
-----


The full tutorial will build with the rest of the repository.
See :doc:`../build` for instructions. Be sure to install the Python dependencies
listed in ``py_requirements.txt`` in order to use the plotting
utility at the end of the tutorial.

Once you have built the repository, the tutorial will be in
``$BUILD/full-tutorial/burgers``.

.. note::

   This tutorial requires C++20 support in your compiler.

Run
---

Run the tutorial with:

.. code-block:: bash

    cd $BUILD/full-tutorial
   ./burgers

This will run the tutorial and generate output files
in the ``output/`` subdirectory.

You can generate plots with the output by running the
python script in the same directory. It is hard-coded
to find the output files in the ``output/`` subdirectory.

.. code-block:: bash

   cd $BUILD/full-tutorial
   python plot.py

For a detailed breakdown of what you just ran, see
the :doc:`understand` page.