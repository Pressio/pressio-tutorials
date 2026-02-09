End-to-end Tutorial: 1-D Burgers' Equation
==========================================

This tutorial demonstrates how to set up and run a reduced order model (ROM)
for the 1-D Burgers' equation using the Pressio library. The tutorial covers
the following key steps:

1. Setting up the full order model (FOM).
2. Running the FOM to generate a snapshot matrix.
3. Building both basic and hyper-reduced ROMs.
4. Running the ROMs and capturing trajectories.
5. Comparing the ROM solutions with the FOM solution.
6. Writing trajectories to CSV files for further analysis.

The tutorial is implemented in C++ and leverages Pressio's capabilities for
model reduction and time integration.

.. toctree::
   :maxdepth: 1

   background
   start
   understand
   hyperreduction
