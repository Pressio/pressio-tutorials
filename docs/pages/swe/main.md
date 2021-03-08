
# Shallow Water Equations Demo

## Description
We consider the shallow water equations (SWE) on the spatial domain
@f$\Omega = [-\frac{L}{2},\frac{L}{2}] \times  [-\frac{L}{2},\frac{L}{2}]@f$:
@f[
\begin{split}
&\frac{\partial h}{\partial t} + \frac{\partial}{\partial x }(  h u) + \frac{\partial}{\partial y }( h v) = 0,\\
&\frac{\partial h u}{\partial t} + \frac{\partial}{\partial x} (h u^2 + \frac{1}{2} \mu_1 h^2) + \frac{\partial}{\partial y }( h u v) = \mu_3 hv,\\
&\frac{\partial h v}{\partial t} + \frac{\partial}{\partial x} (h u v) + \frac{\partial}{\partial y }( h v^2 +  \frac{1}{2} \mu_1 h^2) = \mu_3 hu.
\end{split}
@f]

In the above, @f$h : \Omega \rightarrow \mathbb{R}@f$ is the height of the water surface,
@f$u : \Omega \rightarrow \mathbb{R}@f$ is the x-velocity, and @f$v : \Omega \rightarrow \mathbb{R}@f$ is the y-velocity.
The system has three parameters:
* @f$\mu_1@f$ is the gravity parameter
* @f$\mu_2@f$ controls the magnitude of the initial pulse
* @f$\mu_3@f$ controls the magnitude of the Coriolis forcing

@image html swetut_f1.gif width=90%


## Objective and Breakdown

@m_class{m-block m-success}

@par To describe, via a concrete example, all stages involved in a typical end-to-end ROM workflow.
This demo is broken down into the following parts:
1. A step-by-step walk through of the code to solve the FOM of the SWE: [here](./md_pages_swe_fom.html)
2. A step-by-step walk through of the code to construct and run a standard LSPG: [here](./md_pages_swe_lspg.html)
3. A step-by-step walk through of the code to construct and run a hyper-reduced LSPG: [here](./md_pages_swe_lspg.html)
4. Putting all pieces together: a step-by-step walk through of the full end-to-end ROM workflow: [here](./md_pages_swe_endtoend.html)

<!-- 1. Interface an SWE solver, written with Eigen data structures, to Pressio. -->
<!-- 2. Use Pressio's time marching schemes to execute solves of the SWEs to construct training data. -->
<!-- 3. Use supporting Python scripts to analyze the training data and construct ROM basis vectors. -->
<!-- 4. Construct and run a standard LSPG ROM for novel parameter training instances. -->
<!-- 5. Construct and run a hyper-reduced LSPG ROM for novel parameter training instances. -->
