
#include "pressio_ode_implicit.hpp"
#include "pressio_apps.hpp"

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
    auto ydata = *y.data();
    for (int i=0;i<y.extent(0);i++){
      myfile_.write(reinterpret_cast<const char*>(&ydata(i)), sizeof(ydata(i)));
    }
    std::cout << "Time = " << t << std::endl;
  }

  void closeFile(){
    myfile_.close();
  }
};

/*
   2D Shallow Water Equations with Eigen and Crank Nicolson time stepping.
   Solution will be written in binary format to a solution.bin output file.
*/
int main(int argc, char *argv[])
{
  using scalar_t = double;
  using app_t	 = ::pressio::apps::swe2d;
  using app_state_t	= typename app_t::state_type;
  using app_rhs_t	= typename app_t::velocity_type;
  using app_jacob_t	= typename app_t::jacobian_type;

  // Create arrays for parameter sweep
  //parameter controls gravity
  const std::vector<double> params0Array{3.,6.,9.};

  //parameter controls the magnitude of the initial pulse (kept constant here)
  const scalar_t params1 = 0.125;

  // parameter controls the forcing
  const std::vector<double> params2Array{0.05,0.15,0.25};

  // Construct the app
  const int nx = 128;
  const int ny = 128;
  const scalar_t Lx = 5;
  const scalar_t Ly = 5;
  scalar_t params[3];
  params[0] = params0Array[0];
  params[1] = params1;
  params[2] = params2Array[0];
  app_t appObj(Lx,Ly,nx,ny,params);

  const scalar_t t0 = 0;
  const scalar_t et = 10.;
  const scalar_t dt = 0.02;

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
  for (const auto & it0 : params0Array)
  {
    for (const auto & it2 : params2Array)
    {
      params[0] = it0;
      params[2] = it2;
      appObj.setParams(params);

      ode_state_t y(appObj.getGaussianIC(params[1]));

      stepper_t stepperObj(y, appObj);

      std::string filename = "solution";
      filename += std::to_string(fileNo) + ".bin";
      observer<ode_state_t> Obs(filename);
      auto NonLinSolver =
	pressio::solvers::nonlinear::createNewtonRaphson(stepperObj, y, linSolverObj);
      NonLinSolver.setTolerance(1e-11);

      pressio::ode::advanceNSteps(stepperObj, y, t0, dt, Nsteps,Obs, NonLinSolver);
      Obs.closeFile();
      fileNo += 1;
    }
  }
  return 0;
}
