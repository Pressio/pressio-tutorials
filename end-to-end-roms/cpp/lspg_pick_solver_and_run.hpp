
#ifndef RUN_LSPG_PICK_SOLVER_AND_RUN_HPP_
#define RUN_LSPG_PICK_SOLVER_AND_RUN_HPP_

/* NOTE:
   1. the formatting here matters because the rst docs
      use literalincludes so if you change somehting below
      it is likely you impact the documentation

   2. the comments below are used by the rst documentaion
      in non contiguous literalinclude statetements so that
      we can make the documentation more clear

   3. do NOT erase these comments, and do NOT move them
      or you impact the rst docs

   // branch taken in this demo
   // branch not used in this demo
*/

template<class LspgStepperType, class ParserType, class RomStateType>
void lspg_pick_solver_and_run(const ParserType & parser,
			      LspgStepperType & lspgStepper,
			      RomStateType & reducedState)
{
  namespace pode   = pressio::ode;
  namespace pnlins = pressio::nonlinearsolvers;

  using scalar_type = typename pressio::Traits<RomStateType>::scalar_type;
  const auto startTime = static_cast<typename LspgStepperType::independent_variable_type>(0);
  const pressio::ode::StepCount numSteps(parser.numSteps());
  StateObserver observer(parser.stateSamplingFreq());

  if (parser.nonlinearSolver() == "GaussNewton")
  {
    using hessian_t       = Eigen::Matrix<scalar_type, -1, -1>;
    using solver_tag      = pressio::linearsolvers::direct::HouseholderQR;
    using linear_solver_t = pressio::linearsolvers::Solver<solver_tag, hessian_t>;
    linear_solver_t linearSolver;

    auto solver = pnlins::create_gauss_newton(lspgStepper, linearSolver);
    solver.setStoppingCriterion(pnlins::Stop::WhenGradientAbsoluteNormBelowTolerance);
    solver.setTolerance(parser.nonlinearSolverTolerance());

    pode::advance_n_steps(lspgStepper, reducedState, startTime,
			  parser.timeStepSize(), numSteps,
			  observer, solver);
  }

  else if (parser.nonlinearSolver() == "GaussNewtonQR")
  {
    using mat_t = typename LspgStepperType::jacobian_type;
    using qr_solver_t = pressio::qr::QRSolver<mat_t, pressio::qr::Householder>;
    qr_solver_t qrSolver;
    auto solver = pnlins::create_gauss_newtonQR(lspgStepper, qrSolver);
    solver.setStoppingCriterion(pnlins::Stop::WhenGradientAbsoluteNormBelowTolerance);
    solver.setTolerance(parser.nonlinearSolverTolerance());

    pode::advance_n_steps(lspgStepper, reducedState, startTime,
			  parser.timeStepSize(), numSteps,
			  observer, solver);
  }

  else if (parser.nonlinearSolver() == "LevenbergMarquardt")
  {
    using hessian_t       = Eigen::Matrix<scalar_type, -1, -1>;
    using solver_tag      = pressio::linearsolvers::direct::HouseholderQR;
    using linear_solver_t = pressio::linearsolvers::Solver<solver_tag, hessian_t>;
    linear_solver_t linearSolver;

    auto solver = pnlins::create_levenberg_marquardt(lspgStepper, linearSolver);
    solver.setUpdatingCriterion(pnlins::Update::LMSchedule2);
    solver.setStoppingCriterion(pnlins::Stop::WhenGradientAbsoluteNormBelowTolerance);
    solver.setTolerance(parser.nonlinearSolverTolerance());

    pode::advance_n_steps(lspgStepper, reducedState, startTime,
			  parser.timeStepSize(), numSteps,
			  observer, solver);
  }
}
#endif
