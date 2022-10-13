#include "pressio/solvers_linear.hpp"
#include "pressio/solvers_nonlinear.hpp"
#include <Eigen/Core>

struct MyRosenbrockSystem
{
  using scalar_type   = double;
  using state_type    = Eigen::VectorXd;
  using residual_type = state_type;
  using jacobian_type = Eigen::MatrixXd;

  state_type createState() const{
    auto s = state_type(4);
    s.setZero();
    return s;
  };

  residual_type createResidual() const{
    auto r = residual_type(6);
    r.setZero();
    return r;
  }

  jacobian_type createJacobian() const{
    auto j = jacobian_type(6, 4);
    j.setZero();
    return j;
  }

  void residual(const state_type& x, residual_type & res) const
  {
    const auto & x1 = x(0);
    const auto & x2 = x(1);
    const auto & x3 = x(2);
    const auto & x4 = x(3);
    res(0) = 10.*(x4 - x3*x3);
    res(1) = 10.*(x3 - x2*x2);
    res(2) = 10.*(x2 - x1*x1);
    res(3) = (1.-x1);
    res(4) = (1.-x2);
    res(5) = (1.-x3);
  }

  void jacobian(const state_type & x, jacobian_type & JJ) const
  {
    const auto & x1 = x(0);
    const auto & x2 = x(1);
    const auto & x3 = x(2);
    JJ.setZero();

    JJ(0,2) = -20.*x3;
    JJ(0,3) = 10.;
    JJ(1,1) = -20.*x2;
    JJ(1,2) = 10.;
    JJ(2,0) = -20.*x1;
    JJ(2,1) = 10.;
    JJ(3,0) = -1.;
    JJ(4,1) = -1.;
    JJ(5,2) = -1.;
  }
};

int main()
{
  namespace plog   = pressio::log;
  namespace pls    = pressio::linearsolvers;
  namespace pnonls = pressio::nonlinearsolvers;

  plog::initialize(pressio::logto::terminal);
  plog::setVerbosity({plog::level::info});

  MyRosenbrockSystem problem;

  using hessian_t      = Eigen::MatrixXd;
  using linear_solver_tag = pls::direct::HouseholderQR;
  using linear_solver_t   = pls::Solver<linear_solver_tag, hessian_t>;
  linear_solver_t linearSolver;
  auto gnSolver = pnonls::create_gauss_newton(problem, linearSolver);
  gnSolver.setTolerance(1e-5);

  auto state = problem.createState();
  state(0) = -0.05; state(1) = 1.1; state(2) = 1.2; state(3) = 1.5;
  gnSolver.solve(problem, state);

  // check solution
  std::cout << "Computed solution: \n "
	    << "["
	    << std::setprecision(14)
	    << state(0) << " "
	    << state(1) << " "
	    << state(2) << " "
	    << state(3) << " " << "] \n"
	    << "Expected solution: \n "
	    << "[1.0000000156741, 0.99999999912477, 0.99999999651993, 0.99999998889888]"
	    << std::endl;

  plog::finalize();
  return 0;
}
