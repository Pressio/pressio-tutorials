
# LSPG ROM Driver

@m_class{m-block m-info}

@par
This page walks through the code to construct and run a standard LSPG ROM of the SWE. <br>
The full code is available [here](https://github.com/Pressio/pressio-tutorials/blob/swe2d_tutorial/tutorials/swe2d/online_phase/lspg_rom/run_lspg.cc).

First, we include the relevant headers.
```cpp
@codesnippet
../../../tutorials/swe2d/online_phase/lspg_rom/main.cc
2:6
```

Next, is the main function, where we first process the command line arguments.
<!-- To do this, we define the domain, number of grid points, load in parameter, etc. -->
```cpp
@codesnippet
../../../tutorials/swe2d/online_phase/lspg_rom/main.cc
8:42
```

We initialize the pressio logger.

```cpp
@codesnippet
../../../tutorials/swe2d/online_phase/lspg_rom/main.cc
44:45
```

We construct the app object, and read in the bases.
For standard LSPG, we need to read in the bases for all mesh points.
Here, we assume the basis exists in a local file named *basis.txt*.
The following lines of code read in the basis:
```cpp
@codesnippet
../../../tutorials/swe2d/online_phase/lspg_rom/main.cc
46:60
```

Now that we have the basis, we create a Pressio decoder object.
In this instance where we have a linear basis, the decoder, in essence,
computes the product @f$\boldsymbol \Phi \hat{\boldsymbol x} @f$. We construct a decoder as follows
```cpp
@codesnippet
../../../tutorials/swe2d/online_phase/lspg_rom/main.cc
62:68
```
Next, we set the reference state for the ROM, @f$\boldsymbol x^{ref}@f$.
Note that this reference state will be used when we reconstruct the state
vector, i.e., the state will be reconstructed as
@f[\hat{ \boldsymbol x} = \boldsymbol \Phi \hat{\boldsymbol x} + \boldsymbol x^{ref}@f]
In this case we set the reference state to be the initial condition,
and set the reference states as
 ```cpp
@codesnippet
../../../tutorials/swe2d/online_phase/lspg_rom/main.cc
70:73
```

We now begin constructing the ROM problem.
We start by setting our LSPG state type to be a Pressio vector wrapping an Eigen vector.
We note that this is decoupled from the FOM, and we could alternatively use, e.g., Kokkos with a host backend.
In the following snippet of code, we set this type, initialize the ROM state, and set it to be zero.
Note that this last step is due to the fact that we employ the initial condition as our ROM reference state,
so the initial conditions for our ROM state are zero.
```cpp
@codesnippet
../../../tutorials/swe2d/online_phase/lspg_rom/main.cc
75:83
```

We now construct the LSPG problem.
To do this, we have to define the time marching scheme,
which here we set to be the second-order Crank Nicolson method.
We define this time stepping scheme and construct the LSPG ROM as follows
```cpp
@codesnippet
../../../tutorials/swe2d/online_phase/lspg_rom/main.cc
84:87
```

Now, we need to define the linear and nonlinear solvers use in our LSPG problem.
Here, we employ a least-squares conjugate gradient solver to use within
the Gauss--Newton solver for our LSPG ROM, where we
additionally set the convergence tolerance and maximum number of iterations
```cpp
@codesnippet
../../../tutorials/swe2d/online_phase/lspg_rom/main.cc
89:99
```

Next, we construct an observer.

```cpp
@codesnippet
../../../tutorials/swe2d/online_phase/lspg_rom/main.cc
101:103
```
Observers act like hooks, and are called at the end of each time step/minimization problem.
The observer class can be found [here](\todo).
When initialized, our observer writes the reference state to file in `state_ref.bin`.
When this observer is called after each time step, it will then write the ROM solution to a `solution.bin` file.
<!-- ```cpp -->
<!-- ../../../tutorials/swe2d/online_phase/rom_time_integration_observer.hpp -->
<!-- ``` -->

Next we define information for our time marching, e.g. initial time, time step,
end time, and total number of time steps, and solve the problem.
The code advances the ROM in time for *Nsteps* time steps.
Note that, as LSPG consists of solving a series of residual minimization problems,
so we call this `solveNSequentialMinimizations`:
```cpp
@codesnippet
../../../tutorials/swe2d/online_phase/lspg_rom/main.cc
104:113
```

<!-- Next, we construct a Crank Nicolson time stepper that we will use to march the problem in time. -->
<!-- In Pressio, the steppers (1) act on Pressio data types that wrap the native datatype and (2) -->
<!-- are templated on the time scheme types for the state, residual, Jacobian, and application. -->
<!-- As such, we first define the relevant Pressio wrapped data types, and then define the stepper type. -->
<!-- ```cpp -->
<!-- ../../../../pressio-tutorials/tutorials/swe2d/offline_phase/run_fom_for_training_params.cc -->
<!-- 54:62 -->
<!-- ``` -->

<!-- Next, we define the linear solver type, and construct the solver. -->
<!-- Here, we use the stabilized bi-conjugate gradient method. -->
<!-- ```cpp -->
<!-- ../../../../pressio-tutorials/tutorials/swe2d/offline_phase/run_fom_for_training_params.cc -->
<!-- 64:67 -->
<!-- ``` -->

<!-- Finally, we define the relevant information for our time grid, loop over the parameter instances, and then solve the FOM. -->
<!-- ```cpp -->
<!-- ../../../../pressio-tutorials/tutorials/swe2d/offline_phase/run_fom_for_training_params.cc -->
<!-- 68:103 -->
<!-- ``` -->

This completes our description of writing the LSPG coupler.
#Click [here](./md_pages_tutorials_tutorial3.html) to return to the SWE tutorial.
