
#ifndef PRESSIO_TUTORIALS_ENDTOEND_WORKFLOW_RUN_FOM_HPP_
#define PRESSIO_TUTORIALS_ENDTOEND_WORKFLOW_RUN_FOM_HPP_

template<class FomType, class ParserType>
void run_fom(FomType & appObj, const ParserType & parser)
{
  namespace pode = pressio::ode;

  // initial condition
  using state_t = typename FomType::state_type;
  state_t state = appObj.initialCondition();
  // always write to file the initial condition
  {
    std::ofstream myfile("initial_state.bin");
    const std::size_t ext = state.size()*sizeof(typename state_t::Scalar);
    myfile.write(reinterpret_cast<const char*>(&state(0)), ext);
    myfile.close();
  }

  StateObserver stateObs(parser.stateSamplingFreq());
  const auto startTime = static_cast<typename FomType::scalar_type>(0);

  const auto odeScheme = parser.odeScheme();
  if (pressio::ode::is_explicit_scheme(odeScheme))
  {
     using rhs_t = typename FomType::right_hand_side_type;
     RhsObserver rhsObs(parser.rhsSamplingFreq());

    auto stepperObj = pode::create_explicit_stepper(odeScheme, appObj);
    pode::advance_n_steps(stepperObj, state, startTime,
			  parser.timeStepSize(),
			  pressio::ode::StepCount(parser.numSteps()),
			  stateObs, rhsObs);
  }
  else{
    // namespace plins  = pressio::linearsolvers;
    // namespace pnlins = pressio::nonlinearsolvers;

    // auto stepperObj = pode::create_implicit_stepper(odeScheme, state, appObj);

    // using jacob_t      = typename FomType::jacobian_type;
    // using lin_solver_t = plins::Solver<plins::iterative::Bicgstab, jacob_t>;
    // lin_solver_t linSolverObj;

    // // make nonlinear solver
    // const auto nonlinSolverType = parser.fomNonlinearSolverType();
    // if (nonlinSolverType != "NewtonRaphson"){
    //   throw std::runtime_error("FOM requires NewtonRaphson");
    // }

    // auto NonLinSolver = pnlins::create_newton_raphson(stepperObj, state, linSolverObj);
    // const auto tol = parser.fomNonlinearSolverTolerance();
    // NonLinSolver.setTolerance(tol);

    // pode::advance_n_steps_and_observe(stepperObj, state, startTime,
    // 				      parser.fomTimeStepSize(),
    // 				      parser.fomNumSteps(),
    // 				      stateObs, NonLinSolver);
  }
}

#endif
