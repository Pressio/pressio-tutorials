
#ifndef PRESSIO_TUTORIALS_ENDTOEND_WORKFLOW_RUN_FOM_EXPLICIT_HPP_
#define PRESSIO_TUTORIALS_ENDTOEND_WORKFLOW_RUN_FOM_EXPLICIT_HPP_

#include "pressio/ode_steppers_explicit.hpp"
#include "pressio/ode_advancers.hpp"
#include "observer.hpp"

template<class FomSystemType, class ParserType>
void run_fom_explicit(const FomSystemType & fomSystem,
		      const ParserType & parser)
{
  namespace pode = pressio::ode;

  using state_t = typename FomSystemType::state_type;
  state_t state = fomSystem.initialCondition();
  write_vector_to_binary(state, "initial_state.bin");

  StateObserver stateObs(parser.stateSamplingFreq());
  const auto startTime = static_cast<typename FomSystemType::scalar_type>(0);

  const auto odeScheme = parser.odeScheme();
  using rhs_t = typename FomSystemType::right_hand_side_type;
  RhsObserver rhsObs(parser.rhsSamplingFreq());

  auto stepperObj = pode::create_explicit_stepper(odeScheme, fomSystem);
  pode::advance_n_steps(stepperObj, state, startTime,
			parser.timeStepSize(),
			pressio::ode::StepCount(parser.numSteps()),
			stateObs, rhsObs);
}

#endif
