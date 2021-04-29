
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
1. `basis.txt` This file contains the basis vectors for the ROM on the global mesh
2. `sample_mesh_gids.txt` This file contains the global IDs of the indices used for the sample mesh
3. `sample_mesh_plus_stencil_gids.txt` This file contains the global IDs of the indices used for the sample *and* stencil mesh
4. `PhiSamplePlusStencil.txt` This file contains the ROM basis, but only at the
sample mesh plus stencil mesh. Additionally, this script will create
a file (*samplemesh.png) depicting the sample and stencil mesh which we show below.
Cells in bright green are the sample mesh, while purple cells are on the stencil mesh.

@image html swe_samplemesh.png width=75%


# Step 3: Run FOM at test point
We run the FOM at a new parameter instance (the user is encouraged
to play around with different parameters and see how it impacts the results):

 ```bash
cd $SWE2D_DIR/online_phase/fom
./run_fom -N 64 --gravity 7.5 -p 0.125 --forcing 0.2
```

@m_class{m-frame}

@par Approximate FOM runtime (with data collection disabled):
Running the FOM on a 2.4 GHz 8-Core Intel Core i9 using a single thread takes about @m_span{m-text m-danger}20 seconds@m_endspan.


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

@image html swe_result_lspg.png width=75%

@m_class{m-frame}

@par Approximate runtime (with data collection disabled):
Running on a 2.4 GHz 8-Core Intel Core i9 using a single thread takes about @m_span{m-text m-danger}27 seconds@m_endspan.

We immediately note that our *ROM is slower than the FOM!*
This is due to the well-known bottleneck associated with nonlinear systems.
To gain computational speedups, we need hyper-reduction, as shown below.


# Step 4: RUN the hyper-reduced LSPG ROM

To run a hypre-reduced LSPG ROM, we use the executable obtained from the [hyper-reduced LSPG driver](./md_pages_swe_hrlspg.html)
In summary, this executable loads the basis on the *stencil mesh*, loads in information
about the *sample mesh* and *stencil mesh*, and then constructs and runs
an LSPG ROM employing the collocation hyper-reduction technique.

To run the LSPG ROM with hyper-reduction, we can do:
```bash
cd $SWE2D_DIR/online_phase/lspg_hyperReducedRom
cp ../../offline_phase/*.txt .
./run_lspg_with_hyperreduction -k 10 -N 64 --gravity 7.5 -p 0.125 -f 0.2
python3 viewSolutionAndMakePlots.py
```
If successful, the following plot will be generated.
@image html swe_result_lspgHyper.png width=75%

@m_class{m-frame}

@par Approximate runtime (with data collection disabled):
On a 2.4 GHz 8-Core Intel Core i9 using a single thread takes about @m_span{m-text m-danger}3 seconds@m_endspan:
a 6.5X speedup over the FOM!

This completes our tutorial on ROMs for the shallow water equations.
