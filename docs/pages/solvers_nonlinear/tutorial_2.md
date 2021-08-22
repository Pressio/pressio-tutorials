
# Newton-Raphson with Custom Types

@m_class{m-block m-info}

@par
This tutorial demonstrates how to use
the Newton-Raphson solver from pressio/solvers_nonlinear
using custom data types.

## Custom data types

```cpp
@codesnippet
../../../tutorials/nonlinsolvers_newtonraphson_2.cc
49:70
```

## Problem class

```cpp
@codesnippet
../../../tutorials/nonlinsolvers_newtonraphson_2.cc
107:131
```

## Specialize trait and ops
Because we are working with custom data types, we need to provide
the necessary operations to do the algebra that pressio needs.
This is done via specialization as follows:

```cpp
@codesnippet
../../../tutorials/nonlinsolvers_newtonraphson_2.cc
72:103
```

## Custom Linear solver
In this example, we use a custom linear solver.

```cpp
@codesnippet
../../../tutorials/nonlinsolvers_newtonraphson_2.cc
133:147
```

## Main

```cpp
@codesnippet
../../../tutorials/nonlinsolvers_newtonraphson_2.cc
149:176
```

## Full Code
The full code is available TODO.
