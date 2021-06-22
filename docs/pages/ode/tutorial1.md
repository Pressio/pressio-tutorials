
# Explicit Time Integration for Known Types

@m_class{m-block m-info}

@par
This tutorial demonstrates how to use the pressio/ode component
to run explicit time integration for a system of ODEs that uses data types
natively supported in pressio.
By natively supported, we mean data types that pressio knows how to manipulate
and perform computation on, and therefore the user does not need
to provide any auxiliary information.
Here, we use Eigen data types as an example, but the same code
can be used for other natively supported types.

## Problem

We want to integrate in time the following system of ODEs:
@f[
\frac{d \mathbf{y}}{d t} = 10 \mathbf{y}
@f]
where @f$\mathbf{y} \in R^3@f$ is the state and @f$t@f$ is time.

We first write a class representing our target system of ODEs as follows:
```cpp
@codesnippet
../../../tutorials/tut_ode_1.cc
51:72
```
Note that the data type used for the state is an Eigen vector.
The problem class must meet the API needed by pressio to run explicit time integration:
 * it must contain type aliases for scalar, state and velocity (these aliases are detected by pressio)
 * it must contain two overleads of the velocity method (one void and one non-void)
Note that if you try to use a problem class that does not satisfy the target API,
pressio throws a compile-time error.


## Main function
```cpp
@codesnippet
../../../tutorials/tut_ode_1.cc
73:109
```

We remark the following:
* The native state is wrapped in a `pressio::Vector`.
Behind the scenes, pressio detects what type you are passing and since Eigen types are
supported, pressio knows how to operate on them;
* For simplicity, we are using forward Euler, but other explicit schemes are supported.


## Full Code
The full code is available [here](https://github.com/Pressio/pressio-tutorials/blob/main/tutorials/tut_ode_1.cc).
