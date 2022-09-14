
#ifndef RUN_LSPG_PICK_SOLVER_AND_RUN_HPP_
#define RUN_LSPG_PICK_SOLVER_AND_RUN_HPP_

template<
  class RomProblemType,
  class ParserType,
  class RomStateType
  >
void lspg_pick_solver_and_run(const ParserType & parser,
			      RomProblemType & problem,
			      RomStateType & reducedState)
{
  using scalar_t = typename pressio::Traits<RomStateType>::scalar_type;
  namespace pode = pressio::ode;
  namespace pnlins = pressio::nonlinearsolvers;

  StateObserver observer(parser.stateSamplingFreq());

  const pressio::ode::StepCount numSteps(parser.numSteps());
  const auto startTime = static_cast<typename RomProblemType::independent_variable_type>(0);

  if (parser.nonlinearSolver() == "GaussNewton")
  {
    using hessian_t = Eigen::Matrix<scalar_t, -1, -1>;
    using solver_tag = pressio::linearsolvers::direct::HouseholderQR;
    using linear_solver_t = pressio::linearsolvers::Solver<solver_tag, hessian_t>;
    linear_solver_t linearSolver;

    auto solver = pnlins::create_gauss_newton(problem, linearSolver);
    solver.setStoppingCriterion(pnlins::Stop::WhenGradientAbsoluteNormBelowTolerance);
    solver.setTolerance(parser.nonlinearSolverTolerance());
    //solver.setUpdatingCriterion(pnlins::Update::Armijo);

    pode::advance_n_steps(problem, reducedState, 0.,
			  parser.timeStepSize(), numSteps,
			  observer, solver);
  }

  else if (parser.nonlinearSolver() == "GaussNewtonQR")
  {

    using mat_t = typename RomProblemType::jacobian_type;
    using qr_solver_t = pressio::qr::QRSolver<mat_t, pressio::qr::Householder>;
    qr_solver_t qrSolver;
    auto solver = pnlins::create_gauss_newtonQR(problem, qrSolver);
    solver.setStoppingCriterion(pnlins::Stop::WhenGradientAbsoluteNormBelowTolerance);
    solver.setTolerance(parser.nonlinearSolverTolerance());

    pode::advance_n_steps(problem, reducedState, 0.,
			  parser.timeStepSize(), numSteps,
			  observer, solver);
  }

  else if (parser.nonlinearSolver() == "LevenbergMarquardt")
  {
    using hessian_t = Eigen::Matrix<scalar_t, -1, -1>;
    using solver_tag = pressio::linearsolvers::direct::HouseholderQR;
    using linear_solver_t = pressio::linearsolvers::Solver<solver_tag, hessian_t>;
    linear_solver_t linearSolver;

    auto solver = pnlins::create_levenberg_marquardt(problem, linearSolver);
    solver.setUpdatingCriterion(pnlins::Update::LMSchedule2);
    solver.setStoppingCriterion(pnlins::Stop::WhenGradientAbsoluteNormBelowTolerance);
    solver.setTolerance(parser.nonlinearSolverTolerance());

    pode::advance_n_steps(problem, reducedState, 0.,
			  parser.timeStepSize(), numSteps,
			  observer, solver);
  }
}

#endif
