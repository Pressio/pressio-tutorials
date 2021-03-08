
# Hyperreduced LSPG ROM Driver

@m_class{m-block m-info}

@par
This page walks through an example code to construct and run LSPR ROM of the SWE with *collocation-based hyperreduction*. The full code is available [here](https://github.com/Pressio/pressio-tutorials/blob/swe2d_tutorial/tutorials/swe2d/online_phase/lspg_hyperReducedRom/main.cc).

First, we include the relevant headers.
```cpp
@codesnippet
../../../tutorials/swe2d/online_phase/lspg_hyperReducedRom/main.cc
2:6
```

The main function starts by processing the command line arguments
and we initialize the pressio logger.
```cpp
@codesnippet
../../../tutorials/swe2d/online_phase/lspg_hyperReducedRom/main.cc
59:91
```

Next, we read information on the ROM and hyper-reduction, namely the
global indices of the target sample mesh, stencil mesh, and relative sample mesh information.
```cpp
@codesnippet
../../../tutorials/swe2d/online_phase/lspg_hyperReducedRom/main.cc
97:122
```
In the above, we have:
1. `sampleMesh_gids`: contains the global x,y indices of the sample mesh indices (this is required by the app).
Note that this vector only contains information on the x,y cells used in the sample mesh, **not** the number of conserved variables.

2. `sampleMeshPlusStencil_gids`: contains the global IDs of the stencil mesh (this is also required by the app).
Note that this vector only contains information on the x,y cells used in the stencil mesh, but not the number of conserved variables.

3. `sampleMeshRedIds`: contains the IDs of the sample mesh *relative* to the IDs of the stencil mesh.
Note that this vector will contain information about not only the x,y cells, but also the conserved variables,
and hence it has *3sampleMeshSize* entries. This data is required by Pressio, and is discussed more below.
What do we mean by *information about the sample mesh indices, relative to the stencil mesh*
For example, let's say our FOM has the (zero-based) indices
@f[ \mathcal{I}_{\text{FOM}} = \begin{Bmatrix} 0& 1 & 2 & 3 & 4 & 5 & 6 & 7 & 8 & 9  \end{Bmatrix}@f]
Let's say our sample mesh uses indices 3, 5, and 7,
 @f[ \mathcal{I}_{\text{sample}} = \begin{Bmatrix}  3 &  5  & 7 \end{Bmatrix}@f]

Let's suppose we use a three-point stencil in our numerical method.
In this case, the stencil mesh would be
@f[ \mathcal{I}_{\text{stencil}} = \begin{Bmatrix}  2& 3 & 4& 5 &6  & 7 & 8\end{Bmatrix}@f]

The indices of the *sample mesh* relative to the indices of the *stencil mesh* are thus
@f[ \mathcal{I}_{\text{rel}} =\begin{Bmatrix} 1 & 3 & 5 \end{Bmatrix}@f]
Why do we need this information? In Pressio, when we perform hyperreduction, we need to load
in the basis on the *stencil mesh* so that we can reconstruct the state at these points,
and eventually evaluate the velocity/residual. However, we only then evaluate the residual
at the *sample mesh*, and thus we need to know what indices in the *stencil mesh* correspond
to those that are on the *sample mesh*. Makes sense? Great!


Next, we construct the application/FOM problem object.
```cpp
@codesnippet
../../../tutorials/swe2d/online_phase/lspg_hyperReducedRom/main.cc
124:128
```
Note that here we use the [swe2d_hyper](.) class, opposed to the [swe2d](.) application.
These two applications are the same, with the exception that the *swe2d_hyper* application
loads in indices for the sample and stencil mesh, and only computes the velocity/Jacobians at these points.
We note that, in the last line where we construct the application,
the constructor additionally requires the sample mesh information.


Next, we read in the basis: for hyper-reduction, we only read in the basis
on the *stencil mesh*. The basis on the full mesh is not required to solve the problem.
```cpp
@codesnippet
../../../tutorials/swe2d/online_phase/lspg_hyperReducedRom/main.cc
129:138
```

We then use the basis information to construct the decoder.
For hyper-reduced LSPG the decoder, in essence, is used to compute the product
@f$\boldsymbol \Phi_{\text{stencil}} \hat{\boldsymbol x} @f$, where @f$\boldsymbol \Phi_{\text{stencil}}@f$
is the basis on the *stencil mesh*.
```cpp
@codesnippet
../../../tutorials/swe2d/online_phase/lspg_hyperReducedRom/main.cc
140:146
```

Next we create our reference state. For hyper-reduction, Pressio requires the reference
state on the *stencil_mesh*. If we want to view the entire solution field, however,
we will also need the reference state on the full mesh. Here, we create both entities:
```cpp
@codesnippet
../../../tutorials/swe2d/online_phase/lspg_hyperReducedRom/main.cc
148:151
```

We now create the ROM state and initialize it
to zero (again, the initial conditions are the reference state), and create the
LSPG problem object. We again use the Crank Nicolson time marching scheme.
Note that to construct the LSPG ROM proble, the constructor needs the relative sample mesh indices.
```cpp
@codesnippet
../../../tutorials/swe2d/online_phase/lspg_hyperReducedRom/main.cc
153:165
```

The remainder of our driver follows closely that which was written for LSPG.
We define our linear and nonlinear solvers:
```cpp
@codesnippet
../../../tutorials/swe2d/online_phase/lspg_hyperReducedRom/main.cc
167:195
```

This completes our description of writing the LSPG hyper-reduction driver.
#Click [here](./md_pages_tutorials_tutorial3.html) to return to the SWE tutorial.
