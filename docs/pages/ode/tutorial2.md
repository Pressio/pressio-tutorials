
# Explicit Time Integration for Arbitrary Types

@m_class{m-block m-info}

@par
This tutorial demonstrates how to use the pressio/ode component
to run explicit time integration for a system of ODEs based on custom types,
i.e. data types that pressio does not know how to operate on.
As mentioned in tutorial1, pressio can handle types for which it has wrappers
as well as unknown arbitrary types. While in the former case pressio
knows how to do linear algebra, in the latter pressio does not know which methods those data structures support.
These operations can be passed to pressio via objects as shown below.

## Custom Vector Class
For the sake of this tutorial, we suppose that you have an application
that uses a custom vector class as follows:
```cpp
@codesnippet
../../../tutorials/tut_ode_2.cc
52:75
```
Our custom vector class above is admittedly very simple. The key point we want to convey
is that the `MyCustomVector` type is totally unknown to Pressio.

## Problem Class
Here, we want to integrate in time the same system of ODEs shown in tutorial1, but now our
problem is implemented using the `MyCustomVector` class as follows:
```cpp
@codesnippet
../../../tutorials/tut_ode_2.cc
77:97
```

## Custom Ops Class
Because we are working with custom data types, we need to provide
the necessary operations to do the algebra that pressio needs.
This is done via an ops class as follows:
```cpp
@codesnippet
../../../tutorials/tut_ode_2.cc
99:121
```
The set of methods that this custom ops class needs to provide depend
on the pressio functionality that you want to use.
In this case, we only need the two overloads above because these are the
only ones needed by the Euler forward.
The list of ops needed for various time schemes is provided **missing**.

## Main function
Now that we have a problem and a custom ops, we can write the main:
```cpp
@codesnippet
../../../tutorials/tut_ode_2.cc
123:163
```
Note how the custom ops object is passed to the stepper constructor.
Pressio will introspect your ops class type to check if it is admissible for the target
integration, and it will warn you at compile time if your ops class type does not
have the right methods needed for the stepper you want.

## Full Code
The full code is available [here](https://github.com/Pressio/pressio-tutorials/blob/main/tutorials/tut_ode_2.cc).




<!-- pressio supports arbitrary type via generic programming and type introspection. -->
<!-- For example, if a vector container wrapper is templated on a Eigen::VectorXd, -->
<!-- pressio detects/recognizes the type and leverages the native algebra of Eigen. -->
<!-- If the user instantiates a vector templated on an -->
<!-- arbitrary vector type (for example user-defined), then pressio is also able to -->
<!-- detect that this is an ''unknown'' type, and the user needs to provide -->
<!-- the necessary operations to do the algebra. -->
