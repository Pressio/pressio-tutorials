
# Default Galerkin with Explicit Time Integration for Custom Types

@m_class{m-block m-info}

@par
This tutorial demonstrates how to


## Custom Data Class

Suppose that you have an application that uses arbitrary types.
For the sake of this tutorial, let's define these to be:

```cpp
@codesnippet
../../../tutorials/custom_data_types.hpp
7:52
```

## Specialize trait and ops
Because we are working with custom data types, we need to provide
the necessary operations to do the algebra that pressio needs.
This is done via specialization as follows:
```cpp
@codesnippet
../../../tutorials/rom_default_explicit_galerkin_custom_types.cc
70:199
```

## Main

```cpp
@codesnippet
../../../tutorials/rom_default_explicit_galerkin_custom_types.cc
201:256
```
