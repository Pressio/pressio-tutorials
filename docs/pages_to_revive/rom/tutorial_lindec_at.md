
# Linear Decoder for Arbitrary Types

@m_class{m-block m-info}

@par
This tutorial shows how to create a linear decoder object when using custom types,
i.e. data types that pressio does not know how to operate on.
Recall that a linear decoder represents a mapping of the form:
@f[
{\mathbf y}_{fom} = \mathbf{\phi} {\mathbf y}_{rom}
@f]
where @f$\mathbf{\phi}@f$ is a matrix (for now assumed constant),
@f$\mathbf{y}_{rom}@f$ is the reduced state (or generalized coordinates),
and @f$\mathbf{y}_{fom}@f$ is the full-order model (FOM) state.
A linear decoder is thus fully defined by its Jacobian matrix.

## Types Description

* @f$\mathbf{y}_{fom}@f$: this is the FOM state and for the purposes of this tutorial,
we assume to be a custom type `MyCustomVector`, which will be shown below.

* @f$\mathbf{\phi}@f$: this is the (dense) matrix of this linear mapping (for ROMs, this can be, e.g., POD modes),
and here we assume to be a custom type `MyCustomMatrix`, which will be shown below.

* @f$\mathbf{y}_{rom}@f$: this is the ROM state.
ROM data strutures generally involve small, dense operators that fit well on a single node.
In pressio, *regardless of what the FOM types are*, the data types of the ROM operators
are pressio wrappers of either Eigen (always enabled) or Kokkos (optional dependency) types.
The user can choose which one to use. The reason of this choice is that ROM operators are
always small and dense and fit well on a single node, as explained [here](\todo).

## Custom Vector Class
```cpp
@codesnippet
../../../tutorials/tut_rom_2.cc
52:75
```

## Custom Matrix Class
```cpp
@codesnippet
../../../tutorials/tut_rom_2.cc
77:111
```
Our custom classes above are admittedly simple. The key point we want to convey here
is that these types are literally "custom", so totally unknown to Pressio.

## Custom Ops Class
Because we are working with custom data types, we need to provide
the necessary operations to do the algebra that pressio needs.
This is done via an ops class as follows:
```cpp
@codesnippet
../../../tutorials/tut_rom_2.cc
113:135
```
The ops class needs to implement a `product` kernel with a specific syntax.
This is what pressio will call when the mapping needs to be applied.
If you are familiar with BLAS, you immediately see that `product` method has a BLAS-like API,
which is done on purpose for consistency.
We remark that the `product` method is templated on the operand, `x`, and that you
are only guaranteeed `x` to support the () operator to reference an element.
For this simple example, you immediately know that `x` will be the
ROM state (or generalized coordinates), and so wonder why we have not
explicitly set the type of `x` to be `pressio::containers::Vector<Eigen::VectorXd>`
since that is the ROM state type as shown in line 160.
There is a reason for this: some ROM implementations in pressio
use [expression templates](https://en.wikipedia.org/wiki/Expression_templates),
so the `applyMapping` method of the decoder is not always called on a ROM state object,
but must also support the case where the operand is an object that behaves like
a vector but is not exactly of the same type as the ROM state.
This is important to keep in mind if you code your own ops.

## Main function
Now we can write the main:
```cpp
@codesnippet
../../../tutorials/tut_rom_2.cc
137:187
```

## Remark
Note that the code above would work for any other "arbitrary" (not known to pressio) type,
whether they are distributed or not. You can basically replace the `MyCustomVector` and `MyCustomMatrix`
with your target types, provide the necessary ops, and you have a working code.

## Full Code
The full tutorial can be found [here](https://github.com/Pressio/pressio-tutorials/blob/master/tutorials/tut_rom_2.cc).



  <!-- // // *** construct decoder  *** -->
  <!-- // // Since the FOM types are "arbitrary", we need to declare the -->
  <!-- // // decoder type by passing as a template argument the type of -->
  <!-- // // class (see MyOps struct in this file) containing kernels for operating on the data. -->
  <!-- // // And we need to pass an object to the constructor of the decoder -->
  <!-- // // an object that knows how to compute the the operations needed. -->

  <!-- // // the wrapped types -->
  <!-- // // what happens in pressio when we wrap native_fom_state_t and native_phi_t? -->
  <!-- // // Practically, both pressio::containers::Vector<native_fom_state_t> -->
  <!-- // // and pressio::containers::DenseMatrix<native_fom_state_t> are "labeled" -->
  <!-- // // as "arbitrary" wrapper types for which pressio does not know how to operate on. -->


<!-- ## Context -->
<!-- A key assumption of projection-based ROMs is to approximate the full-order -->
<!-- model (FOM) state, @f$y_{fom}@f$, as: -->
<!-- @f[ -->
<!-- y_{fom} = g(y_{rom}) -->
<!-- @f] -->

<!-- where @f$y_{rom}@f$ is the reduced state (or generalized coordinates), -->
<!-- and @f$g@f$ is the mapping between the two. -->

<!-- If @f$g@f$ is linear, then we can write: -->
<!-- @f[ -->
<!-- y_{fom} = \phi y_{rom} -->
<!-- @f] -->
<!-- where @f$\phi@f$ is a matrix (for the time being, assume it constant). -->
<!-- The Jacobian of the mapping is: -->
<!-- @f[ -->
<!-- \frac{d y_{fom}}{d y_{rom}} = \phi. -->
<!-- @f] -->

<!-- Graphically, this corresponds to: -->
<!-- @image html tut_f1.png width=35% -->





<!-- This case refers to data types which pressio does *not* know how to manipulate and operate on. -->
<!-- What do we mean by *not natively supported*? If you try to use a pressio -->
<!-- functionality/class usign a data structure type that is **NOT** already supported/known -->
<!-- to pressio, pressio *detects/labels* it as an *arbitrary* type, and -->
<!-- you **have to** provide pressio with kernels to operate on these types. -->



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
