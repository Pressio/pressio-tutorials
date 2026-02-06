How do I use Pressio to build and run a ROM?
============================================

This tutorial is designed to show the entire, end-to-end
process of building and running a reduced-order model with
Pressio.

We recommend reading this step-by-step explanation alongside
the source code itself, which is fully commented to provide
additional context. The source code for this tutorial
can be found in ``$SRC/full-tutorial/burgers.cpp``

Each step found below corresponds to the same-named
section of the source code. After Step 0, we recommend
you jump to the ``main`` function and follow along with
the steps as they are explained.

.. note::

   You'll notice that the Pressio examples, this one included,
   favor the ``auto`` keyword for type deduction. This is
   because the types in Pressio are often complex template
   types that would be cumbersome to write out in full. We
   strongly recommend using ``auto`` when working with
   Pressio to simplify your code and ensure you avoid
   mistakes with type declarations.

Step 0: Pressio Setup
---------------------

Pressio is a **header-only** library that provides building blocks
to create reduced-order models (ROMs) for large-scale dynamical systems.

Because it is header-only, we depend on user-set macros to
enable or disable features at compile time. These macros may
be set via CMake configuration or directly in the source code
before including any Pressio headers.

A complete dicussion of Pressio's macros can be found in the
`pressio-rom documentation <https://pressio.github.io/pressio-rom/keywords.html>`_.

In this tutorial, we simply define the key macros in the source code,
like so:

.. code-block:: cpp

   #define PRESSIO_ENABLE_LOGGING
   #define PRESSIO_ENABLE_TPL_EIGEN

Once the macros have been defined, we can include the core Pressio
header.

.. code-block:: cpp

   #include "pressio/rom.hpp"

Since ``rom`` is the top-level module of the Pressio ecosystem,
including this file will pull in all of Pressio--from the built-in
logger to the low-level ops to the ODE steppers.
One ``#include`` to rule them all.

From here, we just include a couple other header files--one with
basic helper functions for this tutorial, and another with utilities
for performing hyper-reduction later on.

.. code-block:: cpp

   #include "helpers.h"
   #include "hyperreduction.h"

Next, we'll also include the standard C++ vector header
to store our snapshots.

.. code-block:: cpp

   #include <vector>

And finally, we'll define some convenient type aliases for vectors and matrices
using the Eigen library, which Pressio supports out of the box.

.. code-block:: cpp

   using vector_t = Eigen::VectorXd;
   using matrix_t = Eigen::MatrixXd;

Now, we can jump down to the ``main`` function to start
the tutorial.

First, we initialize the Pressio logger so that we can see
output from the library as we go:

.. code-block:: cpp

   PRESSIOLOG_INITIALIZE(pressiolog::LogLevel::info);

You can change the ``LogLevel`` argument to ``debug`` for more information,
or ``sparse`` for less.

Then we'll just define some types for convenience:

.. code-block:: cpp

   using FomSystem = Burgers1dFom;
   using time_t = typename FomSystem::time_type;

This ``FomSystem`` type is our implementation of the 1-D Burgers'
equation, which meets the Pressio FOM API as described below.

The purpose of defining this abstract ``FomSystem`` type is to
indicate how Pressio is agnostic to the actual FOM implementation.
As long as the FOM meets the required API, Pressio can work with it.

With that, all set-up is complete and we can move on to building
and running the ROM.

Step 1: Define the Full-Order Model (FOM)
-----------------------------------------

The first step in building a ROM is defining the full-order
model (FOM) that we want to reduce.

In this tutorial, we will be working with the 1-D Burgers'
equation with periodic boundary conditions, as explained in
the :doc:`background` section.

Pressio expects a FOM to be defined such that it meets a specific API. There
are a variety of different APIs depending on the type of problem being solved
(e.g., steady vs unsteady, linear vs nonlinear, etc.). These APIs, defined as
C++ concepts, can be found in the
`Pressio documentation <https://pressio.github.io/pressio-rom/components/rom_concepts.html>`_.

We'll use the API for a semi-discrete FOM defined by a system of ODEs:

.. math::

   \frac{d\mathbf{u}}{dt} = \mathbf{f}(\mathbf{u}, t; \mu)

where :math:`\mathbf{u}` is the state vector, :math:`t` is time, and :math:`\mu` is a parameter.

According to the Pressio API for such a FOM, we need to define a class
that meets the following API:

- Three core types: ``time_type``, ``state_type``, and ``rhs_type``
- A method ``createRhs()`` that returns a new right-hand side vector
- A method ``rhs(const state_type & u, time_type t, rhs_type & f)`` that computes the right-hand side

Step 2: Run the FOM to generate snapshots
-----------------------------------------

In order to generate a reduced-order model, we need to collect snapshots
of the FOM solution over time. The ``runFom`` function in the source code
performs this task.

Typically, applications would implement this step (and the prior definition
of the FOM) themselves. In this tutorial, we provide a simple implementation
of a Forward Euler time integrator that advances the FOM in time and
stores snapshots of both the state and the right-hand side at each time step.

.. code-block:: cpp

   for ( int step = 0; step < numSteps; ++step ) {
      stateSnaps.push_back( u );   // Store state snapshot
      auto f = fom.createRhs();    // Create RHS vector
      fom.rhs( u, t, f );          // Compute RHS
      rhsSnaps.push_back( f );     // Store RHS snapshot
      u += dt * f;                 // Update solution
      t += dt;                     // Advance time
    }

We store these snapshots in a ``SnapshotSet`` object that will be used later
to construct the reduced basis via Proper Orthogonal Decomposition (POD).

Step 3: Build the ROM from the snapshot matrix
----------------------------------------------

Before we can use Pressio to construct the ROM, we need to find
the reduced basis from the collected snapshots. This is done using
POD, which identifies the most energetic modes in the snapshot data.

**Snapshots to Trial Space**

In this tutorial, we compute the singular value decomposition (SVD)
of the snapshot matrix and select the first ``r=5`` modes
to form the reduced basis. The selection of the number of modes is
arbitrary--you may experiment with how changing the value of ``r``
affects the results. Heuristically, we find that ``r=5`` typically
captures most of the system's energy.

Once we have the basis, we can use Pressio's
``create_trial_column_subspace`` function to create
a trial subspace object that represents the reduced basis:

.. code-block:: cpp

   auto trialSpace = pressio::rom::create_trial_column_subspace<vector_t>(
      basis, affineShift, isAffine
   );

Here, ``basis`` is a matrix whose columns are the selected POD modes,
and ``affineShift`` is the mean of the snapshots. The ``isAffine``
argument indicates whether we are using an affine trial space.

This is implemented in the ``helpers.h``
file by the ``snapshots_to_trial_space`` function.

**Trial Space to Reduced State**

Next, we can define the reduced state by projecting the FOM's initial condition
onto the reduced basis. To do this, we use Pressio to compute the reduced
state vector:

.. code-block:: cpp

   auto reducedState = trialSpace.createReducedState();

   // reducedState = basis^T * (u0 - affineShift)
   reducedState = trialSpace.basisOfTranslatedSpace().transpose() *
                  (fomInitialState - trialSpace.translationVector());

This is implemented in the ``helpers.h`` file by the
``trial_space_to_reduced_state`` function.

**Determine the Time Integration Scheme**

Since the FOM is typically defined by an application independently of Pressio,
we set up our own Forward Euler time integrator in Step 2. However, now that
we're using Pressio to build and run the ROM, we can use Pressio's built-in
time integrators.

The ``stepScheme`` defined below will be passed to the ROM constructor and used
when we advance the ROM in time.

.. code-block:: cpp

   auto stepScheme = pressio::ode::StepScheme::ForwardEuler;

The step schemes (both implicit and explicit) that are supported by Pressio
can be found in the ``pressio-rom/ode`` module
`here <https://github.com/Pressio/pressio-rom/blob/develop/include/pressio/ode/ode_enum_and_tags.hpp#L70-L81>`_.

**Build the ROM**

With the trial space, reduced state, and time integrator defined, we can now construct the
reduced-order model using Pressio's built-in functions.

We'll start with a default Galerkin ROM, which projects the FOM's dynamics
onto the trial space using a Galerkin projection. Later on, we'll revisit this
step to implement hyper-reduction.

.. code-block:: cpp

   auto rom = pressio::rom::galerkin::create_unsteady_explicit_problem(
      stepScheme,
      trialSpace,
      fom
   );

Step 4: Run the ROM
-------------------

With the ROM constructed, we can now advance it in time using Pressio's
``ode`` module.

**Define the Time Stepping Policy**

We create a stepping policy using the same time step size ``dt``
and number of steps ``numSteps`` as we used for the FOM simulation:

.. code-block:: cpp

    auto policy = pressio::ode::steps_fixed_dt(
        startTime,
        pressio::ode::StepCount( numSteps ),
        dt
    );

There are other ways to define stepping policies in Pressio, such as
using the start time, end time, and step size instead of the number of steps.
These options are documented in the
`ode module <https://github.com/Pressio/pressio-rom/blob/develop/include/pressio/ode/ode_advance_fncs.hpp>`_.

**Optional: Define the Observer**

This step is optional, but it will allow us to monitor the ROM's progress
as it advances in time. Since we want to plot the results at the end, we'll
define an ``Observer`` that collects the ROM state at each time step.

Observers in Pressio are user-defined functors that are called
at each time step during the time integration process. They can be used
to log data, compute quantities of interest, or store states for later analysis.

Observers must meet the API defined by Pressio. Namely, they must implement the
``operator()`` method that takes the current time, step number, and state
as arguments.

Our ``RomObserver`` will create the full state at every time step and
store it in a ``trajectory`` member variable for later.

.. code-block:: cpp

    // Inside of RomObserver struct...
    template <typename StepCountType, typename TimeType, typename StateType>
    void operator()(StepCountType step, TimeType /*time*/, const StateType& reducedState) const
    {
        // Capture every timestep
        auto fullState = trialSpace.createFullStateFromReducedState( reducedState );
        trajectory.push_back( fullState );
        ++stepCount;
    }

The full ``RomObserver`` struct is implemented in the ``helpers.h`` file.

**Advance the ROM in Time**

Finally, we can advance the ROM in time using Pressio's
``advance`` function:

.. code-block:: cpp

   pressio::ode::advance( rom, reducedState, policy, observer );

If you defined an observer, it will be called at each time step.
Otherwise, simply omit the ``observer`` argument.

**Reconstruct the Full-Order Solution**

After the ROM has been advanced in time, the ``reducedState`` we defined
earlier contains the final reduced state at the end of the simulation.

To get the solution in the full-order space, we need to reconstruct it
using the trial space:

.. code-block:: cpp

   auto romSolution = trialSpace.createFullStateFromReducedState( reducedState );

Step 5: Compare ROM and FOM Solutions
-------------------------------------

At this point, we have constructed and run our reduced-order model.
The remaining steps will compare the results to the FOM and plot the outcomes.

We already have the ROM solution at the final timestep, reconstructed back to the full-order space.
Now, to get the FOM solution, we just extract the last snapshot from the
FOM ``SnapshotSet`` we collected earlier. Specifically, we want the last column
of the state snapshot matrix:

.. code-block:: cpp

   auto fomSolution = snapshots.stateSnapshots.col( snapshots.stateSnapshots.cols() - 1 );

With both the FOM and ROM solutions available, we can now compare them
to assess the accuracy of the reduced-order model. We compute the L2 norm of the difference
between the two solutions:

.. code-block:: cpp

   double error = ( fomSolution - romSolution ).norm() / fomSolution.norm();

This relative error gives us a measure of how well the ROM approximates
the FOM solution. You should see something like:

.. code-block:: console

   [info] Relative error between FOM and default ROM: 0.0026138604529909602

In other words, the ROM solution is within about 0.26% of the FOM solution,
which is quite accurate given that we only used 5 POD modes. Plus, for larger
systems, the computational savings from using a ROM can be significant.

Step 6: Analyze the Results
---------------------------------

Our ``Observer`` in Step 4 collected the ROM states at each time step
and stored them in the ``trajectory`` vector. We can write these states
to CSV files for further analysis and plotting.

After running the main executable:

.. code-block:: bash

   cd $BUILD/full-tutorial
   ./burgers

we can generate plots with the output by running the
python script in the same directory. It is hard-coded
to find the output files in the ``output/`` subdirectory.

.. code-block:: bash

   cd $BUILD/full-tutorial
   python plot.py

The resulting plots show how the ROM solutions compare to the FOM at
various points in the simulation. You should see that the ROM
closely tracks the FOM solution throughout the time integration.

The plot will be saved to your current directory as
``burgers_rom_comparison.png``.

.. note::

   You'll find that the hyper-reduced ROM does not perform quite as well
   as the standard ROM. But the tradeoff is that the hyper-reduced ROM
   is computationally cheaper to run, especially for large-scale systems.
