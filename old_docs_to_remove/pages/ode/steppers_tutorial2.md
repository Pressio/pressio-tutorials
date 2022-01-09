
# Explicit Time Integration for Arbitrary Types

@m_class{m-block m-info}

@par
This tutorial demonstrates how to use the pressio/ode component
to run explicit time integration for a system of ODEs based on custom types,
i.e. data types that pressio does not know how to operate on.

## Custom Vector Class

Suppose that you have an application that uses a custom vector class as follows:

```cpp
@codesnippet
../../../tutorials/custom_data_types.hpp
7:28
```

This custom vector class is very simple but the key point is that
the `CustomVector` is unknown to Pressio.
You can replace `CustomVector` with any type that fits your needs.

## Problem Class
Here, we want to integrate in time the same system of ODEs shown in tutorial1,
but with the problem implemented using the `CustomVector` class as follows:

```cpp
@codesnippet
../../../tutorials/ode_steppers_2.cc
104:124
```

## Specialize trait and ops
Because we are working with custom data types, we need to provide
the necessary operations to do the algebra that pressio needs.
This is done as follows:

```cpp
@codesnippet
../../../tutorials/ode_steppers_2.cc
52:99
```

## Main

```cpp
@codesnippet
../../../tutorials/ode_steppers_2.cc
126:156
```

## Full Code
The full code is available [here](https://github.com/Pressio/pressio-tutorials/blob/main/tutorials/ode_steppers_2.cc).




<!-- pressio supports arbitrary type via generic programming and type introspection. -->
<!-- For example, if a vector container wrapper is templated on a Eigen::VectorXd, -->
<!-- pressio detects/recognizes the type and leverages the native algebra of Eigen. -->
<!-- If the user instantiates a vector templated on an -->
<!-- arbitrary vector type (for example user-defined), then pressio is also able to -->
<!-- detect that this is an ''unknown'' type, and the user needs to provide -->
<!-- the necessary operations to do the algebra. -->
