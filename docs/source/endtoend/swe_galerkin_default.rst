Default Galerkin for 2D SWE
===========================

This tutorials presents an end-to-end demostration of applying
the pressio Galerkin ROM to the 2D shallow water equations (SWE)
leveraging the 2D SWE in pressio-demoapps.

Since this is an end-to-end, we show how to run the following steps:
generating the FOM training data, constructing the POD modes,
setting up and running the Galerkin problem, and processing the results.

Note that while the SWE are used here, one can easily setup and
run the same workflow for any other problem in pressio-demoapps.

To simplify things and actually make this tutorials useful,
we are going to use the workflow codes inside pressio-demoapps
which enable one to easily setup ROMs for any of those problems.
Using those codes, this complete tutorial can be encoded via the
following yaml file, which will show below how to run.

.. code-block:: yaml

   problem: 2d_swe

   parameterSpace:
     names: ['coriolis', 'pulsemag', 'gravity']
     trainPoints:
       0: [-1.0, 0.125, 9.8]
       1: [ 0.0, 0.125, 9.8]
     testPoints:
       0: [-1.0, 0.125, 9.8]

   fom:
     meshSize: [65, 65]
     inviscidFluxReconstruction: "Weno3"
     odeScheme: "RK4"
     timeStepSize: 0.005
     train:
       finalTime: 5.0
       stateSamplingFreq: 10
       rhsSamplingFreq: 100
     test:
       finalTime: 5.0
       stateSamplingFreq: 10
       rhsSamplingFreq: 100

   offlineRom:
     pod:
       stateData:
	 useTrainingRuns: all
	 policy: default
       rhsData:
	 useTrainingRuns: all
	 policy: default

   galerkinRom:
     algorithm: defaultGalerkin
     podTruncation:
       energyBased: [99.99, 99.99999]



Running the worflow
-------------------

.. code-block:: py

   python3 $REPOSRC/wf_foms.py --wf wf.yaml --dr false
   python3 $REPOSRC/wf_offline_rom.py --wf wf.yaml --dr false
   python3 $REPOSRC/wf_default_galerkin.py --wf wf.yaml --dr false
   python3 $REPOSRC/wf_reconstruct_on_full_mesh.py


This should generate following directory structure:

.. code-block:: bash

   .
   ├── CMakeFiles
   ├── Makefile
   ├── cmake_install.cmake
   ├── default_galerkin_truncation_energybased_99.99999_runid_0
   ├── default_galerkin_truncation_energybased_99.99_runid_0
   ├── fom_mesh
   ├── fom_test_runid_0
   ├── fom_train_runid_0
   ├── offline_rom
   └── wf.yaml


FOM snippet
^^^^^^^^^^^

When we run the FOM driver, the following C++ code is being executed:

.. literalinclude:: ../../../end-to-end-roms/cpp/run_fom.hpp
   :language: cpp
   :lines: 5-35

Galerkin ROM snippet
^^^^^^^^^^^^^^^^^^^^

When we run the Galerkin driver, the following C++ code is being executed:

.. literalinclude:: ../../../end-to-end-roms/cpp/run_default_galerkin.hpp
   :language: cpp
   :lines: 10-50
   :emphasize-lines: 20,21,22
