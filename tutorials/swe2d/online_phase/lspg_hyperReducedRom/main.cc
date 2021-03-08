
#include "pressio_rom_lspg.hpp"
#include "pressio_apps.hpp"
#include "read_basis.hpp"
#include "rom_time_integration_observer.hpp"
#include "CLI11.hpp"

auto readSampleMeshGlobalIndices(const int size,
				 const std::string & fileName)
  -> std::vector<int>
{
  std::vector<int> result(size);
  std::ifstream files(fileName);
  for (int i =0; i < size ; i++){
    files >> result[i];
  }
  files.close();
  return result;
}

auto readStencilMeshGlobalIndices(const int size,
				  const std::string & fileName)
  -> std::vector<int>
{
  std::vector<int> result(size);
  std::ifstream files(fileName);
  for (int i =0; i < size ; i++){
    files >> result[i];
  }
  files.close();
  return result;
}

auto readSampleMeshIndicesRelativeToStencil(const int size,
					    const std::string & fileName)
  -> pressio::containers::Vector<Eigen::Matrix<int,-1,1>>
{
  Eigen::Matrix<int, -1,1> v(size);
  std::ifstream files(fileName);
  for (int i =0; i < size ; i++){
    files >> v(3*i);
    v(3*i) = v(3*i)*3;
    v(3*i+1) = v(3*i)+1;
    v(3*i+2) = v(3*i)+2;
  }
  files.close();
  pressio::containers::Vector<Eigen::Matrix<int,-1,1>> result(std::move(v));
  return result;
}

int main(int argc, char *argv[])
{
  pressio::log::initialize(pressio::logto::terminal);
  CLI::App app{"Hyper-reduced LSPG ROM of 2D Shallow Water Equations"};

  using scalar_t = double;
  int N = 64;
  scalar_t finalTime = 10.;
  scalar_t dt = 0.02;
  scalar_t gravity = 7.5;
  scalar_t pulse   = 0.125;
  scalar_t forcing = 0.2;

  app.add_option("-N,--numCells", N,
		 "Number of cells along each axis: default = 64");

  app.add_option("-T,--finalTime", finalTime,
		 "Simulation time: default = 10.");

  app.add_option("--dt", dt,
		 "Time step size: default = 0.02");

  app.add_option("-g,--gravity", gravity,
		 "Gravity value(s) to simulate: default = 7.5");

  app.add_option("-p,--pulse", pulse,
		 "Pulse magnitude, default = 0.125");

  app.add_option("-f,--forcing", forcing,
		 "Forcing value(s) to simulate: default = 0.2");

  CLI11_PARSE(app, argc, argv);

  // -------------------------------------------------------
  // Read information for ROM and hyper-reducction
  // -------------------------------------------------------
  int romSize;
  int sampleMeshSize;
  int sampleMeshPlusStencilSize;
  std::ifstream info_file("info_file.txt");
  info_file >> romSize;
  std::cout << romSize << std::endl;;
  info_file >> sampleMeshSize;
  info_file >> sampleMeshPlusStencilSize;
  info_file.close();
  std::cout << "| romSize = " << romSize
            << "| |  sampleMeshSize = " << sampleMeshSize
            << "| |  sampleMeshPlusStencilSize " << sampleMeshPlusStencilSize
            << std::endl;

  // Required for the app
  /*
    read in the global indices (gids) of the sample mesh.
    Note that we could alternatively extract these
    from the information contained in sm_rel_lids and and smps_gids
  */
  auto sm_gids = readSampleMeshGlobalIndices
    (sampleMeshSize, "sample_mesh_gids.txt");

  // Required for the app
  /*
    read in the global indices (gids) of the stencil mesh.
    The underlying app is a structured solver, and these indices
    allow us to find the correct location on a structured grid.
  */
  auto smps_gids = readStencilMeshGlobalIndices
    (sampleMeshPlusStencilSize, "sample_mesh_plus_stencil_gids.txt");

  // Required for Pressio
  /*
    read in the indices of the sample mesh relative to the stencil mesh,
    i.e., if the 4th element of the stencil mesh is the zeroth element
    of the sample mesh, then the zeroth entry would be sm_rel_lids(0) = 4;
  */
  auto sampleMeshRelIds = readSampleMeshIndicesRelativeToStencil
    (3*sampleMeshSize, "sample_mesh_relative_indices.txt");

  // Construct the app
  using fom_t = pressio::apps::swe2d_hyper<std::vector<int>>;
  const scalar_t params[3] = {gravity, pulse, forcing};
  fom_t appObj(N, params, sm_gids, smps_gids);

  // -------------------------------------------------------
  // read basis on the sample+stencil mesh
  // -------------------------------------------------------
  using decoder_jac_t	= pressio::containers::MultiVector<Eigen::MatrixXd>;
  decoder_jac_t phi = readBasis("PhiSamplePlusStencil.txt",
				romSize,
				sampleMeshPlusStencilSize*3);
  const int numBasis = phi.numVectors();
  if( numBasis != romSize ) return 0;

  // ------------------------------------------------------
  // construct decoder
  // ------------------------------------------------------
  using native_state_t  = typename fom_t::state_type;
  using fom_state_t  = pressio::containers::Vector<native_state_t>;
  using decoder_t = pressio::rom::LinearDecoder<decoder_jac_t, fom_state_t>;
  decoder_t decoderObj(phi);

  // create the reference vector (on the stencil mesh).
  native_state_t yRef(appObj.getGaussianIC(params[1]));
  // For post processing, we also make a vector on the full mesh
  native_state_t yRefFull(appObj.getGaussianICFull(params[1]));

  // -------------------------------------------------------
  // create ROM problem
  // -------------------------------------------------------
  using lspg_state_t = pressio::containers::Vector<Eigen::Matrix<scalar_t,-1,1>>;
  // define ROM state
  lspg_state_t yROM(romSize);
  // initialize to zero (reference state is the initial condition)
  pressio::ops::fill(yROM, 0.0);

  // define LSPG type
  using ode_tag  = pressio::ode::implicitmethods::CrankNicolson;
  auto lspgProblem = pressio::rom::lspg::createHyperReducedProblemUnsteady
    <ode_tag>(appObj, decoderObj, yROM, yRef, sampleMeshRelIds);

  // linear solver
  using eig_dyn_mat     = Eigen::Matrix<scalar_t, -1, -1>;
  using hessian_t       = pressio::containers::DenseMatrix<eig_dyn_mat>;
  using solver_tag      = pressio::solvers::linear::iterative::LSCG;
  using linear_solver_t = pressio::solvers::linear::Solver<solver_tag, hessian_t>;
  linear_solver_t linSolverObj;

  // GaussNewton solver with normal equations
  auto solver = pressio::rom::lspg::createGaussNewtonSolver(lspgProblem, yROM, linSolverObj);
  solver.setTolerance(1e-8);
  solver.setMaxIterations(10);

  // define observer to monitor time evolution of generalized coordinates
  observer<lspg_state_t,native_state_t> Obs(yRefFull);

  // solve
  const auto Nsteps = static_cast<::pressio::ode::types::step_t>(finalTime/dt);
  auto startTime = std::chrono::high_resolution_clock::now();
  pressio::rom::lspg::solveNSequentialMinimizations(lspgProblem, yROM, 0.0, dt, Nsteps, Obs, solver);
  auto finishTime = std::chrono::high_resolution_clock::now();
  const std::chrono::duration<double> elapsed2 = finishTime - startTime;
  std::cout << "Walltime (single ROM run) = " << elapsed2.count() << '\n';

  auto yFomFinal = lspgProblem.fomStateReconstructorCRef()(yROM);
  auto solNorm = (*yFomFinal.data()).norm();
  std::cout << std::setprecision(14) << solNorm << std::endl;

  return 0;
}
