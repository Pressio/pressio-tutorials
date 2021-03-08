
# End-to-end Workflow: train, explore, reduce

@m_class{m-block m-info}

@par
This end-to-end analysis shows how to use the LSPG approach to
accelerate a forward model of the SWE. <br>
In this tutorial, we cover the following steps (each with a dedicated section below):
1. Use Pressio's time marching schemes to execute solves of the SWEs to construct training data.
2. Use supporting Python scripts to analyze the training data and construct POD basis vectors.
3. Run the SWE FOM at a test point in parameter space.
4. Construct and run a standard LSPG ROM for the same test parameter instance.
5. Construct and run a hyper-reduced LSPG ROM for same test parameter instance.


# Prerequisites
After successfully builing the tutorials, set the following env var:

```bash
export SWE2D_DIR=<where-you-built-pressio-tutorials>/tutorials/swe2d
```

# Step 1: Compute FOM training data
<!-- In the offline phase, we: -->
<!-- 1. Extract training data and perform proper orthogonal decomposition to find a basis for the ROM -->
<!-- 2. Extract training data and perform proper orthogonal decomposition to find a basis for the ROM -->
<!-- 3. Select indices for hyper-reduction by Q-sampling -->
<!-- We now walk through this outline phase. -->

<!-- A full-order model that can be used to solve the SWEs is located in the -->
<!-- packages/apps section of the Pressio repo (see [here](https://github.com/Pressio/pressio/tree/swe2d/packages/apps/src/swe2d) ). -->
<!-- The full-order model employs a first-order finite volume -->
<!-- discretization with the Rusanov flux scheme at the cell interfaces. -->

For this step, we use the executable obtained from the [FOM driver](./md_pages_swe_fom.html)
to evaluate the FOM for 9 parameter instances on the
grid @f$\mu_1 \times \mu_3 = [3,6,9]\times [0.05,0.15,0.25]@f$,
over the time interval @f$t \in (0,10)@f$ and save the solutions to file.

```bash
cd $SWE2D_DIR/offline_phase
./run_fom -N 64 --gravity 3. 6. 9. --forcing 0.05 0.15 0.25
```
This will take some time to run, depending on your system.
If successful, a series of `solution#.bin` files should be written.
These solution files contain the FOM solutions at every time step
for each of the nine training parameter instances.
To view the results of one sample simulation, we can do:
```bash
cd $SWE2D_DIR/offline_phase
python3 viewSolutionAndMakePlots.py
```
This script will bring up a live animation of the solution for the first parameter instance.

# Step 2: Compute the bases and build the sample mesh

We now need to construct the basis vectors used in the ROM.
```bash
cd $SWE2D_DIR/offline_phase
python3 makeBasisAndHyperReducedBasis.py
```
This script loads in the snapshots generated in Step 1. and performs POD to obtain the ROM basis.
Additionally, this script selects cells for the sample mesh employed in hyper-reduction,
and saves the relevant information of this sample mesh to file.
Specifically, it generates the following files:
1. `info.txt` This file contains information on the size of the ROM, the size of the sample mesh, and the size of the sample mesh and stencil mesh.
2. `basis.txt` This file contains the basis vectors for the ROM on the global mesh
3. `sample_mesh_gids.txt` This file contains the global IDs of the indices used for the sample mesh
4. `sample_mesh_plus_stencil_gids.txt` This file contains the global IDs of the indices used for the sample *and* stencil mesh
5. `PhiSamplePlusStencil.txt` This file contains the ROM basis, but only at the sample mesh plus stencil mesh
Additionally, this script will create a file (*samplemesh.png) depicting the sample and stencil mesh.
Cells in black are the sample mesh, while cells in red are on the stencil mesh.

@image html samplemesh.png width=50%



# Step 3: Run FOM at test point

<!-- With the offline stage complete, we can now run our ROMs for novel parameter instances. -->
<!-- We will first run a standard LSPG ROM without hyper-reduction, followed by an LSPG ROM with hyper-reduction. -->
<!-- To set a novel parameter instance, we switch to the online directory and look at the *novel_params.txt* file -->
<!-- ```bash -->
<!-- cd $SWE2D_DIR/online_phase/ -->
<!-- vim novel_params.txt -->
<!-- ``` -->
<!-- By default, we have the novel parameter instance set to be @f$\mu_1 = 7.5, \; \mu_2=0.125, \; \mu_3 = 0.2@f$. -->
<!-- The rest of this tutorial will present results for this parameter instance, but the user is encouraged to play -->
<!-- around with different parameters and see how it impacts the results. Before we run the ROM, -->
<!-- we first run a FOM for this new parameter instance so we can assess the accuracy of our ROM -->
<!-- (of course, in a practical scenario we would not do this step!). -->
<!-- We do not provide a detailed explanation on this driver script, since it closely follows that written previously. -->
We run the FOM at a new parameter instance:
 ```bash
cd $SWE2D_DIR/online_phase/fom
./run_fom -N 64 --gravity 7.5 -p 0.125 --forcing 0.2
```
This will run the FOM and save the solution to file.
The FOM was tested on a 2.7 GHz 12-Core Intel Xeon E5 core, and took 152 seconds to run.
The rest of this tutorial will present results for this parameter instance,
but the user is encouraged to play around with different parameters and see how it impacts the results.


# Step 4: RUN a standard LSPG ROM

To run a standard LSPG ROM, we use the executable obtained from the [LSPG driver](./md_pages_swe_lspg.html)
In summary, this code couples the application to Pressio, loads in the basis information
we generated in the offline phase, and couples to Pressio's ROM capabilities to run an LSPG ROM.
To run the LSPG ROM, we can do:
```bash
cd $SWE2D_DIR/online_phase/lspg_rom
cp ../../offline_phase/basis.txt .
./run_lspg -k 10 -N 64 --gravity 7.5 -p 0.125 -f 0.2
python3 viewSolutionAndMakePlots.py
```
Notet that we pass @f$-k = 10@f$ to use 10 POD modes for each DOF as basis to run the ROM.
This process saves the generalized coordinates of the ROM to the `solution.bin` file,
and `viewSolutionAndMakePlots.py` plots the height of the water surface for a given spatial location
as a function of time, and saves the plot to *result.png*. This plot looks as follows:
@image html result_lspg.png width=50%

The ROM was tested on a 2.7 GHz 12-Core Intel Xeon E5 core, and took 179 seconds to run.
We immediately note that our *ROM is slower than the FOM!*
This, of course, is due to the well known bottleneck associated with nonlinear systems.
To gain computational speedups, we need hyper-reduction. We now detail this.


# Step 4: RUN the hyper-reduced LSPG ROM

We now run construct and run a hyper-reduced LSPG ROM.
To do this, we again need to write a driver file, which here we call [run_lspg_with_hyperreduction.cc](https://github.com/Pressio/pressio-tutorials/blob/swe2d_tutorial/tutorials/swe2d/online_phase/lspg_hyperReducedRom/run_lspg_with_hyperreduction.cc).
A step-by-step tutorial for what is entailed in constructing this driver file is provided [here](./md_pages_tutorials_tutorial3_lspg_hyper.html).
In summary, this file loads the basis on the *stencil mesh*, loads in information about the *sample mesh* and *stencil mesh*,
and then constructs and runs an LSPG ROM employing the collocation hyper-reduction technique.

To run the LSPG ROM with hyper-reduction, we move to the *lspg_hyperReducedRom* directory,
copy over our basis and sample mesh information, and then run our ROM.
```bash
cd $SWE2D_DIR/online_phase/lspg_hyperReducedRom
cp ../../offline_phase/supporting_python_scripts/*.txt .
./run_lspg_with_hyperreduction
python3 viewSolutionAndMakePlots.py
```
If successful, the following plot will be generated.
@image html result_lspgHyper.png width=50%
Running the ROM on the same 2.7 GHz 12-Core Intel Xeon E5 core machine took 20 seconds, which is about a 7.5x speedup over the FOM!

This completes our tutorial on ROMs for the shallow water equations.
