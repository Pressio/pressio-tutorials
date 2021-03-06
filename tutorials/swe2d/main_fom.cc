
#include "pressio_ode_implicit.hpp"
#include "pressio_apps.hpp"
#include "CLI11.hpp"

template <typename state_t>
struct observer
{
  std::ofstream myfile_;

  observer(const std::string filename)
    : myfile_(filename,  std::ios::out | std::ios::binary){}

  template<typename time_t>
  void operator()(const size_t step,
  		  const time_t t,
  		  const state_t & y)
  {
    if (step % 5 == 0){
      const auto & ydata = *y.data();
      for (int i=0;i<y.extent(0);i++){
	myfile_.write(reinterpret_cast<const char*>(&ydata(i)), sizeof(ydata(i)));
      }
      std::cout << "Time = " << t << std::endl;
    }
  }

  void closeFile(){
    myfile_.close();
  }
};

template <typename T>
void printInputArgs(const std::vector<T> & g,
		    const T & p,
		    const std::vector<T> & f)
{
  std::cout << "gravities = ";
  for (auto it : g){
    std::cout << it << " ";
  }
  std::cout << "\n";

  std::cout << "pulse coeff = " << p << std::endl;

  std::cout << "forcing = ";
  for (auto it : f){
    std::cout << it << " ";
  }
  std::cout << "\n";
}

int main(int argc, char *argv[])
{
  CLI::App app{"2D Shallow Water Equations using Eigen data types and Crank Nicolson time stepping"};

  using scalar_t = double;

  int N = 64;
  scalar_t et = 10.;
  scalar_t dt = 0.02;
  std::vector<scalar_t> gravity = {7.5};
  scalar_t pulse  = 0.125;
  std::vector<scalar_t> forcing = {0.2};

  app.add_option("-N,--numCells", N,
		 "Number of cells along each axis: default = 64");

  app.add_option("-T,--finalTime", et,
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
  printInputArgs(gravity, pulse, forcing);

  using app_t	    = ::pressio::apps::swe2d;
  using app_state_t = typename app_t::state_type;
  using app_rhs_t   = typename app_t::velocity_type;
  using app_jacob_t = typename app_t::jacobian_type;

  // create FOM object
  app_t appObj(N);

  // ode types
  using ode_state_t     = pressio::containers::Vector<app_state_t>;
  using ode_residual_t  = pressio::containers::Vector<app_rhs_t>;
  using ode_jacobian_t  = pressio::containers::SparseMatrix<app_jacob_t>;

  using ode_tag = pressio::ode::implicitmethods::CrankNicolson;
  using stepper_t = pressio::ode::ImplicitStepper<
    ode_tag, ode_state_t, ode_residual_t, ode_jacobian_t, app_t>;

  // define solver
  using solver_tag = pressio::solvers::linear::iterative::Bicgstab;
  using lin_solver_t = pressio::solvers::linear::Solver<solver_tag, ode_jacobian_t>;
  lin_solver_t linSolverObj;

  // solve for each parameter
  const auto Nsteps = static_cast<::pressio::ode::types::step_t>(et/dt);
  int fileNo = 0;
  for (const auto & it0 : gravity)
  {
    for (const auto & it2 : forcing)
    {
      // set physical coefficients
      scalar_t params[3] = {it0, pulse, it2};
      appObj.setParams(params);

      // initial condition
      ode_state_t y(appObj.getGaussianIC(params[1]));

      // create stepper
      stepper_t stepperObj(y, appObj);

      std::string filename = "solution";
      filename += std::to_string(fileNo) + ".bin";
      observer<ode_state_t> Obs(filename);
      auto NonLinSolver =
	pressio::solvers::nonlinear::createNewtonRaphson(stepperObj, y, linSolverObj);
      NonLinSolver.setTolerance(1e-11);

      auto startTime = std::chrono::high_resolution_clock::now();
      pressio::ode::advanceNSteps(stepperObj, y, 0., dt, Nsteps, /*Obs,*/ NonLinSolver);
      auto finishTime = std::chrono::high_resolution_clock::now();
      const std::chrono::duration<double> elapsed2 = finishTime - startTime;
      std::cout << "Walltime (single FOM run) = " << elapsed2.count() << '\n';  Obs.closeFile();

      Obs.closeFile();
      fileNo += 1;
    }
  }

  return 0;
}
