Hyper-Reduction
===============

This tutorial builds off of the information presented in
:doc:`understand` to demonstrate how to set up and run a hyper-reduced
reduced-order model (ROM) for the 1-D Burgers' equation using the Pressio library.

If you have not completed that initial tutorial, please do so before proceeding.

This page will follow the same structure as the previous tutorial,
going through the enumerated steps but only highlighting the differences
needed to implement hyper-reduction.

First: Why use hyper-reduction?
-------------------------------

As discussed above, a typical ROM uses only the state snapshots
to build the reduced basis.

This reduces the dimensionality of the system, but
we still evaluate the full-order RHS at each time step,
which can be computationally expensive. **Hyper-reduction**
addresses this issue by approximating the RHS using a
reduced basis constructed from the RHS snapshots.

So instead of a reduced state basis alone, we will
also build a reduced basis for the RHS using the
RHS snapshots we collected earlier.

With this, we can begin the tutorial.

Steps 0-1: As Before
--------------------

Setting up Pressio and constructing your FOM is done exactly
the same, regardless of the type of ROM you will build.

Step 2: Run the FOM to generate snapshots
-----------------------------------------

In the original tutorial, we defined a ``SnapshotSet``
to collect snapshots for both the ``state`` and the
right-hand side ``rhs`` of the full-order model (FOM).

You may have noticed that the default ROM did not use
the RHS snapshots at all. However, for hyper-reduction,
we will need these RHS snapshots to build the
hyper-reduction basis.

We will explain this further in subsequent steps.

If you are not implementing hyper-reduction, there is no
need to store the RHS snapshots. You can simply return
the state snapshots directly.

Step 3: Build the ROM from the snapshot matrix
----------------------------------------------

The process begins identically to before: we use the
state snapshots to compute the trial space, and then use
that to get the reduced state. We will also use the same
``ForwardEuler`` time stepper as before.

The key difference is in the definition of a **hyper-reducer**.

The hyper-reducer is a functor that takes in a sampled
FOM RHS vector and projects it into a reduced space.

The process is similar to that of the trial space: we
compute a POD basis from the RHS snapshots using SVD,
and then use that basis to project the FOM RHS onto
a lower-dimensional subspace.

There are refined techniques for selecting which rows
of the RHS to sample, such as the Discrete Empirical
Interpolation Method (DEIM). For simplicity in this tutorial,
we will use a basic sampling approach that selects rows at
a uniform interval (or stride).

Here is a general outline of how the hyper-reducer functor
is defined:

.. code-block:: cpp

    auto svdRhs = /* compute SVD of RHS snapshot matrix */;
    auto rhsBasis = /* extract leading left singular vectors from SVD */;

    auto sampleIndices = /* select sample indices uniformly */;
    auto sampledBasis = /* extract rows of rhsBasis at sampleIndices */;

    auto trialBasis /* aka Phi */ = trialSpace.basisOfTranslatedSpace();
    auto hypredMatrix = /* (Phi^T rhsBasis) * pinv(sampledBasis) */;

    // Construct hyperreducer functor
    ExplicitGalerkinHyperReducer hyperReducer(hypredMatrix, sampleIndices);

This code is implemented fully in the ``hyperreduction.h`` file
by the ``buildHyperReducer`` function.

The ``ExplicitGalerkinHyperReducer`` above is a
user-defined functor that implements an ``operator()`` method
that takes in a sampled FOM RHS vector and outputs the reduced RHS vector.

Like with the other Pressio interfaces, you can define your
hyperreducer in any way you like, as long as it meets the API requirements.

Now that we have defined the hyper-reducer, we can
build the ROM almost identically to before, but passing
the hyperreducer functor as an additional argument:

.. code-block:: cpp

    auto rom = pressio::rom::galerkin::create_default_problem_with_hyperreduction(
        stepScheme,
        trialSpace,
        fom,
        hyperReducer
    );

Step 4-6: As Before
-------------------

The rest of the steps are identical regardless of whether
you are using hyper-reduction or not. You can refer to the
:doc:`understand` page for details on how to run the ROM,
reconstruct the full-order solution, compare to the FOM,
and examine the output files.
