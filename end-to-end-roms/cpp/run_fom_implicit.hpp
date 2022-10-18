
#ifndef PRESSIO_TUTORIALS_ENDTOEND_WORKFLOW_RUN_FOM_IMPLICIT_HPP_
#define PRESSIO_TUTORIALS_ENDTOEND_WORKFLOW_RUN_FOM_IMPLICIT_HPP_

#include "pressio/ode_steppers_implicit.hpp"
#include "pressio/ode_advancers.hpp"
#include "observer.hpp"

template<class FomSystemType, class ParserType>
void run_fom_implicit(const FomSystemType & appObj,
		      const ParserType & parser)
{
  namespace pode   = pressio::ode;
  namespace plins  = pressio::linearsolvers;
  namespace pnlins = pressio::nonlinearsolvers;

  auto state = appObj.initialCondition();
  write_vector_to_binary(state, "initial_state.bin");

  // stepper
  const auto odeScheme = parser.odeScheme();
  assert(!pressio::ode::is_explicit_scheme(odeScheme));
  auto stepperObj = pode::create_implicit_stepper(odeScheme, appObj);

  // linear solver
  using fom_jacobian_t = typename FomSystemType::jacobian_type;
  using linear_solver_t = plins::Solver<plins::iterative::Bicgstab, fom_jacobian_t>;
  linear_solver_t linearSolver;

  // nonlinear solver
  const auto nonlinSolverType = parser.nonlinearSolver();
  assert(nonlinSolverType == "NewtonRaphson");
  auto nonLinearSolver = pnlins::create_newton_raphson(stepperObj, linearSolver);
  nonLinearSolver.setTolerance(parser.nonlinearSolverTolerance());

  // advance in time
  StateObserver stateObs(parser.stateSamplingFreq());
  const auto startTime = static_cast<typename FomSystemType::scalar_type>(0);
  pode::advance_n_steps(stepperObj, state, startTime,
			parser.timeStepSize(),
			pressio::ode::StepCount(parser.numSteps()),
			stateObs, nonLinearSolver);
}

#endif
