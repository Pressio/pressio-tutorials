.. include:: ../mydefs.rst

2D SWE: default Galerkin
========================

  - ROM technique: default Galerkin

  - problem: `2D shallow water equations (SWE) <https://pressio.github.io/pressio-demoapps/swe_2d.html>`_


Prerequisites
-------------

- A valid build of the tutorials, see `here <../build.html>`__, and the following env variables set:

  .. code-block:: bash

     export REPOSRC=<full-path-to-the-pressio-tutorials-source-repo>/end-to-end-roms
     export BUILDDIR=<full-path-to-where-you-built-the-tutorials>

- To run all scripts below, you MUST be in the correct end-to-end directory:

  .. code-block:: bash

     cd $BUILDDIR/end-to-end-roms/2d_swe_galerkin_default


Workflow File
-------------

The `workflow file <../../../end-to-end-roms/2d_swe_galerkin_default/wf.yaml>`_
for this demo is shown below for exposition purposes, but it is automatically
copied to the build directory, so you don't need to do anything:

.. literalinclude:: ../../../end-to-end-roms/2d_swe_galerkin_default/wf.yaml
   :language: yaml
   :lines: 1-35


Step 1: execute FOMs
--------------------

.. code-block:: bash

   # from within $BUILDDIR/end-to-end-roms/2d_swe_galerkin_default
   python3 $REPOSRC/wf_foms.py --wf wf.yaml

This driver script automates this first stage by creating input files,
generating run directories and running the C++ executable
to generate all the FOM train and test data at the points specified
in the yaml file. For exposition purposes, we show below the actual
C++ code executed during this stage to run a single FOM:

.. literalinclude:: ../../../end-to-end-roms/cpp/run_fom_explicit.hpp
   :language: cpp
   :lines: 56-76

At the end, doing ``tree -L 1 .`` should produce:

.. code-block:: bash

   .
   ├── CMakeFiles
   ├── Makefile
   ├── cmake_install.cmake
   ├── fom_mesh
   ├── fom_test_runid_0
   ├── fom_train_runid_0
   ├── fom_train_runid_1
   ├── plot.py
   └── wf.yaml

Where we see the run train and test directories and their corresponding IDs.

Step 2: offline rom
-------------------

.. code-block:: bash

   # from within $BUILDDIR/end-to-end-roms/2d_swe_galerkin_default
   python3 $REPOSRC/wf_offline_rom.py --wf wf.yaml

The offline rom takes care of using the FOM training data to compute
the POD modes, and creates all data into an "offline_rom" subdirectory.
Specifically, in this case the demo specifies that *all* the training
runs should be used to compute POD modes.

.. code-block:: bash

   ./offline_rom/
   ├── pod_input.yaml
   ├── rhs_left_singular_vectors.bin
   ├── rhs_singular_values.txt
   ├── rhs_snapshots.bin
   ├── state_left_singular_vectors.bin
   ├── state_singular_values.txt
   └── state_snapshots.bin

Step 3: galerkin rom
--------------------

.. code-block:: bash

   # from within $BUILDDIR/end-to-end-roms/2d_swe_galerkin_default
   python3 $REPOSRC/wf_galerkin.py --wf wf.yaml

This driver script automates all the Galerkin runs.
Specifically, it creates all the run directories, writes all input files,
prepares initial conditions and runs the actual Galerkin run at every test point.
For exposition purposes, we show below the actual
C++ code executed during this stage to run a single Galerkin run:

.. literalinclude:: ../../../end-to-end-roms/cpp/run_default_galerkin.hpp
   :language: cpp
   :lines: 57-79, 82-87, 92

More specifically, what happens here is the following:

- the total number of modes used varies and is computed by truncating
  the basis using the energy values specified in the workflow file
  at the top: the higher the target energy, the more modes it uses
  (the code doing this computes the energy of the singular
  values of the POD modes and truncate based on the target energy,
  and again this is done automatically by our driver script)

- the reduced initial condition is read from file but here simply
  corresponds to the projection of the FOM initial condition onto the basis

- note that we also monitor the evolution of the reduced state and
  store its history into file (this will be used for the postprocessing)

At the end, you should have the following directory structure:

.. code-block:: bash

   .
   ├── CMakeFiles
   ├── Makefile
   ├── cmake_install.cmake
   ├── default_galerkin_truncation_energybased_99.99999_runid_0
   ├── default_galerkin_truncation_energybased_99.999_runid_0
   ├── fom_mesh
   ├── fom_test_runid_0
   ├── fom_train_runid_0
   ├── fom_train_runid_1
   ├── offline_rom
   ├── plot.py
   └── wf.yaml


Step 4: process results
-----------------------

.. code-block:: bash

   # from within $BUILDDIR/end-to-end-roms/2d_swe_galerkin_default
   python3 $REPOSRC/wf_reconstruct_on_full_mesh.py
   python3 plot.py

.. image:: ../../../end-to-end-roms/2d_swe_galerkin_default/FOM.png
  :width: 32 %
  :alt: FOM
.. image:: ../../../end-to-end-roms/2d_swe_galerkin_default/ROM_22.png
  :width: 32 %
  :alt: ROM, 22 modes
.. image:: ../../../end-to-end-roms/2d_swe_galerkin_default/ROM_55.png
  :width: 32 %
  :alt: ROM, 55 modes


Step 5: things you can try
--------------------------

This section suggests a few things you can experiment with.
Note: before you run a new experiment in the same directory, to avoid conflicts
you need to cleanup all the existing content, which can you easily do as follows:

.. code-block:: bash

   # from within $BUILDDIR/end-to-end-roms/2d_swe_galerkin_default
   python3 clean.py

- a different simulation time for the train and test runs
  so that you can assess how the ROM performs in a time extrapolation regime.
  This can easily be done by just replacing:

  .. code-block:: yaml

     fom:
       # ...
       train:
	 finalTime: 5.0
       # ...
       test:
	 finalTime: 6.0

- add new test points to the ``parameterSpace`` section of the workflow
  file to as follows:

  .. code-block:: yaml

     parameterSpace:
       # ...
       testPoints:
	 0: [-0.5, 0.125, 9.8]
	 1: [-0.9, 0.125, 9.8]
	 2: [ 0.1, 0.125, 9.8] # this test point is outside of training range
