#include "pressio_ode_implicit.hpp"
#include "pressio_apps.hpp"
std::string checkStr {"PASSED"};
template <typename state_t>
struct observer{
  std::ofstream myfile_;
  observer(const std::string filename): myfile_(filename,  std::ios::out | std::ios::binary){}

  void operator()(size_t step,
  		  double t,
  		  const state_t & y){
    auto ydata = *y.data();
    for (int i=0;i<y.extent(0);i++){
      myfile_.write(reinterpret_cast<const char*>(&ydata(i)),sizeof(ydata(i)));
    }
    std::cout << "Time = " << t << std::endl; 
  }

  void closeFile(){
    myfile_.close();
  }
};

/*
   Regression test for the 2D Shallow Water Equations with Eigen data structures
   and Crank Nicolson time stepping. Solution will be written to a solution.bin 
   output file. 
*/

int main(int argc, char *argv[]){
  using scalar_t = double;
  using app_t	 = ::pressio::apps::swe2d;
  using app_state_t	= typename app_t::state_type;
  using app_rhs_t	= typename app_t::velocity_type;
  using app_jacob_t	= typename app_t::jacobian_type;


  int nx = 128;
  int ny = 128;
  scalar_t params[3];
  std::ifstream params_file("../novel_params.txt");
  for (int i = 0; i < 3; i++){
    params_file >> params[i];
  }
  scalar_t Lx = 5;
  scalar_t Ly = 5;
  scalar_t mu_ic = params[1];
  app_t appObj(Lx,Ly,nx,ny,params);
  scalar_t t = 0;
  scalar_t et = 10.;
  scalar_t dt = 0.02;

  // types for ode
  using ode_state_t = pressio::containers::Vector<app_state_t>;
  using ode_res_t   = pressio::containers::Vector<app_rhs_t>;
  using ode_jac_t   = pressio::containers::SparseMatrix<app_jacob_t>;

  using ode_tag = pressio::ode::implicitmethods::CrankNicolson;
  using stepper_t = pressio::ode::ImplicitStepper<
    ode_tag, ode_state_t, ode_res_t, ode_jac_t, app_t>;

  // define solver
  using lin_solver_t = pressio::solvers::linear::Solver<
    pressio::solvers::linear::iterative::Bicgstab, ode_jac_t>;
  lin_solver_t linSolverObj;

  auto Nsteps = static_cast<::pressio::ode::types::step_t>(et/dt);
  appObj.setParams(params);
  ode_state_t y(appObj.getGaussianIC(params[1]));
  stepper_t stepperObj(y, appObj);

  std::string filename = "solution_fom.bin";
  observer<ode_state_t> Obs(filename);
  auto NonLinSolver=
  pressio::solvers::nonlinear::createNewtonRaphson(stepperObj, y,linSolverObj);
  NonLinSolver.setTolerance(1e-11);

  auto startTime = std::chrono::high_resolution_clock::now();
  pressio::ode::advanceNSteps(stepperObj, y, t, dt, Nsteps,Obs, NonLinSolver);
  auto finishTime = std::chrono::high_resolution_clock::now();
  const std::chrono::duration<double> elapsed2 = finishTime - startTime;
  std::cout << "Walltime (single FOM run) = " << elapsed2.count() << '\n';  Obs.closeFile();
  return 0;
}