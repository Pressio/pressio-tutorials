
# Default Galerkin with explicit time stepping

@m_class{m-block m-info}

@par
This tutorial shows how to create and solve a time-explicit *default* Galerkin problem.

# What is a default Galerkin problem?

pressio4py supports different variants of Galerkin, as we will show in subsequent tutorials.
The "default" qualification in pressio4py refers to a
formulation that does *not* use hyper-reduction.
Suppose that your full-order model (FOM) is written as
@f[
\frac{d \boldsymbol{y}}{dt} =
\boldsymbol{f}(\boldsymbol{y},t; \boldsymbol{\mu}),
\quad \boldsymbol{y}(0;\boldsymbol{\mu}) = \boldsymbol{y}(\boldsymbol{\mu}),
@f]

where @f$y@f$ is the FOM state and @f$f(...)@f$ is the FOM velocity.
Both @f$y@f$ and @f$f@f$ are large, see figure below.
@image html tut_f2.png width=30%

@m_class{m-block m-info}

@par
pressio4py defines a *default Galerkin* problem as:
@f[
\dot{\hat{\mathbf{y}}}(t;\mathbf{\mu}) =
\mathbf{\phi}^T
\mathbf{f}
\Big(\mathbf{y}_{ref}(\mathbf{\mu})
+ \mathbf{\phi}\hat{\mathbf{y}} \Big)
@f]

where @f$\hat{y}@f$ is the reduced state, also called generalized coordinates,
@f$y@f$ is the full-order model (FOM) state,
@f$y_{ref}@f$ is a reference FOM state, @f$\phi@f$ is the orthonormal basis, and
@f$f(...)@f$ is the FOM velocity. Schematically, this system corresponds
to the figure below.
@image html tut_f3.png width=65%


# How to create a default Galerkin problem?

To create a default Galerkin problem object, one needs:
1. a FOM object satisfying the API described [here]()
2. a linear decoder (see [this tutorial](./md_pages_tutorials_tutorial1.html))
3. a rom state
4. a FOM reference state
5. [optional] an object with specific kernels when the FOM types are not natively supported by pressio.<br/>
See [this page](./md_pages_custom_ops_default_gal_exp.html) for more information on this.

Synopsis:

```cpp
using ode_tag = pressio::ode::explicitmethods::Euler;

using pressio::rom::galerkin::createDefaultProblem;
auto Problem = createDefaultProblem<ode_tag>(fomObj, decoder, yRom, yRef, [, opsObject]);
```
Note the function is templated on the ode tag.
To select a different time stepping scheme, one can change the tag.
To see the list of currently supported explicit stepping schemes, see \todo.


# How to solve a default Galerkin problem?

Once the target problem object is created, the reduced system
can be integrated in time. Here we provide the most basic function
to do so, which advances the system for a fixed number of steps.
Synopsis:

```cpp
// solve for fixed number of steps and time step
pressio::rom::galerkin::solveNSteps(problem,     # problem object
								    yRom,        # rom state to advance
								    t0,          # initial time
									dt,          # time step
									Nsteps       # number of steps
									[, observer] # optional observer
								   )
```
The optional argument allows one to pass an "observer" object whose
purpose is to monitor the evolution of the reduced state.
The observer is called back by pressio4py during the time integration
at every time step. This can be useful to, e.g., save the
generalized coordinates, or usign them to perfom some other operation.


# Putting all steps together

```cpp
// create adapter/FOM object
// ...

// create the decoder
decoder = /*see, e.g., tutorial for linear decoder */

// create ROM state: here we use Eigen
using rom_state_t = pressio::containers::Vector<Eigen::VectorXd>;
rom_state_t yRom(/*whatever rom size needed*/);

// create the Galerkin problem
using ode_tag = pressio::ode::explicitmethods::Euler;

using pressio::rom::galerkin::createDefaultProblem;
auto Problem = createDefaultProblem<ode_tag>(fomObj, decoder, yRom, yRef, [, opsObject]);

// to solve, we set as an example, t0=0, dt=0.1, Nstep = 100
pressio::rom::galerkin::solveNSteps(Problem, yRom, 0., 0.1, 100)
```
Note the function is templated on the ode tag.
To select a different time stepping scheme, one can change the tag.
To see the list of currently supported explicit stepping schemes, see \todo.



<!-- The observer class must meee the following API: -->
<!-- ```py -->
<!-- class OdeObserver: -->
<!--   def __init__(self): pass -->

<!--   def __call__(self, timeStep, time, romState): -->
<!-- 	# do what you want with romState -->
<!-- ``` -->
<!-- Note that we are working on enriching the API to integrate in time. -->
<!-- For example, we will soon support function class to advance the problem -->
<!-- until a condition is met, or until a target time is reached. -->


<!-- # Want to see all the above pieces in action? -->

<!-- Look at [this demo](./md_pages_demos_demo1.html) that uses -->
<!-- default Galerkin for a 1d PDE. -->


<!-- # Some considerations -->
<!-- @m_class{m-block m-warning} -->

<!-- @par -->
<!-- One might wonder how the above formulation can be efficient, -->
<!-- given that the right-hand side of the reduced system scales -->
<!-- with the FOM degrees of freedom. -->
<!-- This is true: the reduced system obtained from a -->
<!-- *default* problem reduces the spatial degrees of freedom, -->
<!-- but is typically not efficient because at every evaluation of the RHS, -->
<!-- it requires a large matrix vector product. -->
<!-- Thus, a default Galerkin is typically used for exploratory -->
<!-- analysis when computational efficiency is **not** a primary -->
<!-- goal, e.g. to test the feasibility of ROMs for a target problem, -->
<!-- or try different basis. -->
<!-- When computational efficiency is critical, one needs to -->
<!-- resort to hyper-reduction techniques to reduce the cost of the matrix-vector -->
<!-- product. This is covered in subsequent tutorials. -->
