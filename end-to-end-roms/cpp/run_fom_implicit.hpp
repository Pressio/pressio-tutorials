
#ifndef PRESSIO_TUTORIALS_ENDTOEND_WORKFLOW_RUN_FOM_IMPLICIT_HPP_
#define PRESSIO_TUTORIALS_ENDTOEND_WORKFLOW_RUN_FOM_IMPLICIT_HPP_

#include "pressio/ode_steppers_implicit.hpp"
#include "pressio/ode_advancers.hpp"
#include "observer.hpp"

template<class FomSystemType, class ParserType>
void run_fom_implicit(const FomSystemType & appObj,
		      const ParserType & parser)
{
  namespace pode = pressio::ode;

  using state_t = typename FomSystemType::state_type;
  state_t state = appObj.initialCondition();
  write_vector_to_binary(state, "initial_state.bin");

  StateObserver stateObs(parser.stateSamplingFreq());
  const auto startTime = static_cast<typename FomSystemType::scalar_type>(0);
  const auto odeScheme = parser.odeScheme();

  // namespace plins  = pressio::linearsolvers;
  // namespace pnlins = pressio::nonlinearsolvers;

  // auto stepperObj = pode::create_implicit_stepper(odeScheme, state, appObj);

  // using jacob_t      = typename FomSystemType::jacobian_type;
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

#endif
