
# Linear Decoder for Known Types

@m_class{m-block m-info}

@par
This tutorial shows how to create a linear decoder object when using
data types that are known to pressio.
Recall that a linear decoder represents a mapping of the form:
@f[
{\mathbf y}_{fom} = \mathbf{\phi} {\mathbf y}_{rom}
@f]
where @f$\mathbf{\phi}@f$ is a matrix (for now assumed constant),
@f$\mathbf{y}_{rom}@f$ is the reduced state (or generalized coordinates),
and @f$\mathbf{y}_{fom}@f$ is the full-order model (FOM) state.
A linear decoder is thus fully defined by its Jacobian matrix.


## Types Description

* @f$\mathbf{y}_{fom}@f$: this is the FOM state and, therefore, it is a data structure of your application.
For example, if your application uses Trilinos/Tpetra, the FOM state can be, e.g., a Tpetra vector.
Typically, @f$y_{fom}@f$ is a large (distributed) vector.

* @f$\mathbf{\phi}@f$: this is the (dense) matrix of this linear mapping (for ROMs, this can be, e.g., POD modes).
This should also be a data structure from your application.
For example, if you are using Trilinos/Tpetra, this matrix can be, e.g., a Tpetra MultiVector.
Typically, @f$\phi@f$ is a large (distributed) dense matrix.

* @f$\mathbf{y}_{rom}@f$: this is the ROM state.
ROM data strutures generally involve small, dense operators that fit well on a single node.
In pressio, *regardless of what the FOM types are*, the data types of the ROM operators
are pressio wrappers of either Eigen (always enabled) or Kokkos (optional dependency) types.
The user can choose which one to use. The reason of this choice is that ROM operators are
always small and dense and fit well on a single node, as explained [here](\todo).

For demonstration purposes, here we use Eigen types for the FOM.
For other types already known to pressio, this would work similarly.

## Main function
```cpp
@codesnippet
../../../tutorials/tut_rom_1.cc
51:87
```

## Code Breakdown
The main function starts with type aliasing:
```cpp
@codesnippet
../../../tutorials/tut_rom_1.cc
56:60
```
where we explictly create pressio wrapper types for the FOM state and decoder Jacobian @f$\phi@f$.
This is needed because pressio operates on these wrapper types.

We then create a native matrix:
```cpp
@codesnippet
../../../tutorials/tut_rom_1.cc
62:64
```
Note that this is a native type, so in this case Eigen.

The decoder object is instantiated here:
```cpp
@codesnippet
../../../tutorials/tut_rom_1.cc
67:68
```
Note that the native object `phiNative` is moved from (since Eigen supports move semantics) to avoid a deep copy.
After the move, `phiNative` is left in undefined state, so it should not be used.
One can also do `decoder(phiNative)` which implies a deep copy of the matrix, and obviously,
if the matrix is large avoiding a copy is useful.
<!-- Inside the `LinearDecoder` constructor, the native matrix is moved from to create  -->

To perform the computation, the `LinearDecoder` class exposes an `applyMapping` method:
```cpp
@codesnippet
../../../tutorials/tut_rom_1.cc
75:77
```


## Full Code
The full tutorial can be found [here](https://github.com/Pressio/pressio-tutorials/blob/master/tutorials/tut_rom_1.cc).


<!-- Graphically, this corresponds to: -->
<!-- @image html tut_f1.png width=35% -->
<!-- *A linear decoder in pressio implements this linear mapping.* -->

<!-- ## Scenario A: your FOM types are natively supported in pressio -->

<!-- This case refers to data types which pressio knows how to manipulate and operate on. -->
<!-- Examples include vector and matrix classes in Eigen, Epetra/Tpetra in Trilinos, or Kokkos views. -->
<!-- What do we mean by *natively supported*? If you try to use a pressio -->
<!-- functionality/class using data structure types that are already supported/known -->
<!-- to pressio, pressio *detects* them, and automatically uses the (best) -->
<!-- native kernels to perform computations. -->
<!-- The full list of supported data structures types can be found [here](\todo). -->

<!-- Here, for demonstration purposes, we pretend the FOM uses Eigen types too. -->
<!-- For other FOM types already known to pressio it would work similarly. -->
<!-- The full tutorial can be found [here](https://github.com/Pressio/pressio-tutorials/blob/master/tutorials/tutorial4.cc). -->

<!-- ```cpp -->
<!-- ../../../tutorials/tutorial4.cc -->
<!-- 51:103 -->
<!-- ``` -->


<!-- ## Scenario B: your FOM types are NOT natively supported in pressio -->

<!-- This case refers to data types which pressio does *not* know how to manipulate and operate on. -->
<!-- What do we mean by *not natively supported*? If you try to use a pressio -->
<!-- functionality/class usign a data structure type that is **NOT** already supported/known -->
<!-- to pressio, pressio *detects/labels* it as an *arbitrary* type, and -->
<!-- you **have to** provide pressio with kernels to operate on these types. -->

<!-- Here, for demonstration, we pretend that: -->
<!-- * @f$y_{fom}@f$ is a `std::vector<>` -->
<!-- * @f$\phi@f$ is a `std::vector<std::vector<>>` -->

<!-- Note that the code below would work for any other "arbitrary'' (not known to pressio) type, -->
<!-- whether they are distributed or not. -->
<!-- The full tutorial can be found [here](https://github.com/Pressio/pressio-tutorials/blob/master/tutorials/tutorial5.cc). -->

<!-- ```cpp -->
<!-- ../../../tutorials/tutorial5.cc -->
<!-- 75:139 -->
<!-- ``` -->

<!-- ### The ops struct -->
<!-- In order for pressio to handle the linear mapping, it needs to know -->
<!-- how to operate on @f$\phi@f$. To this end, in the code above, -->
<!-- you need to pass to the `LinearDecoder` constructor an object to handle that computation. -->
<!-- To compute the mapping, pressio will call the `product` method. -->
<!-- ```cpp -->
<!-- ../../../tutorials/tutorial5.cc -->
<!-- 51:73 -->
<!-- ``` -->

<!-- ## Comments -->

<!-- We are constantly working increasing support in pressio for more external libraries. -->
<!-- If you application types are not supported but you would like them to be, -->
<!-- you can file an [issue](https://github.com/Pressio/pressio/issues) to request it. -->


<!-- What do we mean by *knwon*? We mean that when you use a functionality/class -->
<!-- using data structure types that are already supported/known -->
<!-- to pressio, pressio *detects* them, and automatically uses the (best) -->
<!-- native kernels to perform computations. -->
<!-- The full list of supported data structures types can be found [here](\todo). -->
