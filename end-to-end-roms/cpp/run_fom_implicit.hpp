/*
//@HEADER
// ************************************************************************
//
// run_fom_implicit.hpp
//                     		  Pressio
//                             Copyright 2019
//    National Technology & Engineering Solutions of Sandia, LLC (NTESS)
//
// Under the terms of Contract DE-NA0003525 with NTESS, the
// U.S. Government retains certain rights in this software.
//
// Pressio is licensed under BSD-3-Clause terms of use:
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions
// are met:
//
// 1. Redistributions of source code must retain the above copyright
// notice, this list of conditions and the following disclaimer.
//
// 2. Redistributions in binary form must reproduce the above copyright
// notice, this list of conditions and the following disclaimer in the
// documentation and/or other materials provided with the distribution.
//
// 3. Neither the name of the copyright holder nor the names of its
// contributors may be used to endorse or promote products derived
// from this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
// FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
// COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
// INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
// (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
// SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
// HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
// STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
// IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.
//
// Questions? Contact Francesco Rizzi (fnrizzi@sandia.gov)
//
// ************************************************************************
//@HEADER
*/

#ifndef PRESSIO_TUTORIALS_ENDTOEND_WORKFLOW_RUN_FOM_IMPLICIT_HPP_
#define PRESSIO_TUTORIALS_ENDTOEND_WORKFLOW_RUN_FOM_IMPLICIT_HPP_

#include "pressio/ode_steppers_implicit.hpp"
#include "pressio/ode_advancers.hpp"
#include "observer.hpp"

template<class FomSystemType, class ParserType>
void run_fom_implicit(const FomSystemType & fomSystem,
		      const ParserType & parser)
{
  namespace pode   = pressio::ode;
  namespace plins  = pressio::linearsolvers;
  namespace pnlins = pressio::nonlinearsolvers;

  auto state = fomSystem.initialCondition();
  write_vector_to_binary(state, "initial_state.bin");

  // stepper
  const auto odeScheme = parser.odeScheme();
  assert(!pressio::ode::is_explicit_scheme(odeScheme));
  auto stepperObj = pode::create_implicit_stepper(odeScheme, fomSystem);

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
  StateObserver observer(parser.stateSamplingFreq());
  const auto startTime = static_cast<typename FomSystemType::scalar_type>(0);
  pode::advance_n_steps(stepperObj, state, startTime,
			parser.timeStepSize(),
			pode::StepCount(parser.numSteps()),
			observer, nonLinearSolver);
}

#endif
