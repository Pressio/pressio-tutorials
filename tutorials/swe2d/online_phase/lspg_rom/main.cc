
#include "pressio_rom_lspg.hpp"
#include "pressio_apps.hpp"
#include "read_basis.hpp"
#include "rom_time_integration_observer.hpp"
#include "CLI11.hpp"

int main(int argc, char *argv[])
{
  CLI::App app{"LSPG ROM of 2D Shallow Water Equations"};

  using scalar_t = double;
  int N = 64;
  int romSizePerDof = 10;
  scalar_t finalTime = 10.;
  scalar_t dt = 0.02;
  scalar_t gravity = 7.5;
  scalar_t pulse   = 0.125;
  scalar_t forcing = 0.2;

  app.add_option("-k,--romSize", romSizePerDof,
		 "Number of modes for each dof to use: default = 10");

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

  pressio::log::initialize(pressio::logto::terminal);

  // -------------------------------------------------------
  // create FOM object
  // -------------------------------------------------------
  using fom_t = pressio::apps::swe2d;
  const scalar_t params[3] = {gravity, pulse, forcing};
  fom_t appObj(N, params);

  // -------------------------------------------------------
  // read basis
  // -------------------------------------------------------
  using decoder_jac_t	= pressio::containers::MultiVector<Eigen::MatrixXd>;
  const int romSizeTotal = romSizePerDof*3;
  decoder_jac_t phi = readBasis("basis.txt", romSizeTotal, appObj.numDofs());
  const int numBasis = phi.numVectors();
  if( numBasis != romSizeTotal ) return 0;

  // -------------------------------------------------------
  // create decoder obj
  // -------------------------------------------------------
  using native_state_t  = typename fom_t::state_type;
  using fom_state_t  = pressio::containers::Vector<native_state_t>;
  using decoder_t = pressio::rom::LinearDecoder<decoder_jac_t, fom_state_t>;
  decoder_t decoderObj(phi);

  // -------------------------------------------------------
  // create reference state
  // -------------------------------------------------------
  native_state_t yRef(appObj.getGaussianIC(pulse));

  // -------------------------------------------------------
  // create ROM problem
  // -------------------------------------------------------
  using lspg_state_t = pressio::containers::Vector<Eigen::Matrix<scalar_t,-1,1>>;
  // define ROM state
  lspg_state_t yROM(romSizeTotal);
  // initialize to zero (reference state is IC)
  pressio::ops::fill(yROM, 0.0);

  // define LSPG type
  using ode_tag  = pressio::ode::implicitmethods::CrankNicolson;
  auto lspgProblem = pressio::rom::lspg::createDefaultProblemUnsteady
    <ode_tag>(appObj, decoderObj, yROM, yRef);

  // linear solver
  using eig_dyn_mat	= Eigen::Matrix<scalar_t, -1, -1>;
  using hessian_t	= pressio::containers::DenseMatrix<eig_dyn_mat>;
  using solver_tag      = pressio::solvers::linear::iterative::LSCG;
  using linear_solver_t = pressio::solvers::linear::Solver<solver_tag, hessian_t>;
  linear_solver_t linSolverObj;

  // GaussNewton solver with normal equations
  auto solver = pressio::rom::lspg::createGaussNewtonSolver(lspgProblem, yROM, linSolverObj);
  solver.setTolerance(1e-10);
  solver.setMaxIterations(10);

  // create observer (see rom_time_integration_observer.hpp in parent dir)
  observer<lspg_state_t,native_state_t> Obs(yRef);

  // solve
  const auto Nsteps = static_cast<::pressio::ode::types::step_t>(finalTime/dt);
  auto startTime = std::chrono::high_resolution_clock::now();
  pressio::rom::lspg::solveNSequentialMinimizations(lspgProblem, yROM, 0.0, dt, Nsteps, Obs, solver);
  auto finishTime = std::chrono::high_resolution_clock::now();
  const std::chrono::duration<double> elapsed2 = finishTime - startTime;
  std::cout << "Walltime (single ROM run) = " << elapsed2.count() << '\n';

  return 0;
}
