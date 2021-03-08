
# FOM Driver

@m_class{m-block m-info}

@par
This page walks through the code to
solve the FOM of the SWE using Pressio's time marching schemes. <br>
The full code is available [here](https://github.com/Pressio/pressio-tutorials/blob/swe2d_tutorial/tutorials/swe2d/main_fom.cc).

First, we include the relevant headers.
```cpp
@codesnippet
../../../tutorials/swe2d/main_fom.cc
2:4
```

The main function starts by processing the command line arguments: this allows one
to set up several parameters,
but for all of the them we have default values as shown below.
```cpp
@codesnippet
../../../tutorials/swe2d/main_fom.cc
53:85
```

Then, we create some convenient types aliases.
Here, we start by defining our application to be the swe2d app,
and then extract the relevant types from the application class.
After extracting the relevant types, we create an object of the problem of interest
by specifying the number of cells to use along each axis.
Be default, the app class uses a physical domain (0,5).
```cpp
@codesnippet
../../../tutorials/swe2d/main_fom.cc
87:93
```

Next, we construct a Crank Nicolson time stepper that we will use to march the problem in time.
In Pressio, the steppers (1) act on Pressio data types that wrap the native datatype and (2)
are templated on the time scheme types for the state, residual, Jacobian, and application.
As such, we first define the relevant Pressio wrapped data types, and then define the stepper type.
```cpp
@codesnippet
../../../tutorials/swe2d/main_fom.cc
96:102
```

Next, we create the linear solver that is used within the nonlinear one.
Here, we use the stabilized bi-conjugate gradient method.
```cpp
@codesnippet
../../../tutorials/swe2d/main_fom.cc
104:107
```

Finally, we define the time grid, loop over the parameter instances, and solve the FOM.
```cpp
@codesnippet
../../../tutorials/swe2d/main_fom.cc
109:148
```

This completes our description of writing the FOM coupler.
<!-- Click [here](./md_pages_swe_main.html) to return to the SWE main page. -->
