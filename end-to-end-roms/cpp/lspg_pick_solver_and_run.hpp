/*
//@HEADER
// ************************************************************************
//
// lspg_pick_solver_and_run.hpp
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

#ifndef RUN_LSPG_PICK_SOLVER_AND_RUN_HPP_
#define RUN_LSPG_PICK_SOLVER_AND_RUN_HPP_

template<class LspgStepperType, class ParserType, class RomStateType>
void lspg_pick_solver_and_run(const ParserType & parser,
			      LspgStepperType & lspgStepper,
			      RomStateType & reducedState)
{
  namespace pode   = pressio::ode;
  namespace pnlins = pressio::nlsol;

  using scalar_type = typename pressio::Traits<RomStateType>::scalar_type;
  const auto startTime = static_cast<typename LspgStepperType::independent_variable_type>(0);
  const pressio::ode::StepCount numSteps(parser.numSteps());
  StateObserver observer(parser.stateSamplingFreq());

  if (parser.nonlinearSolver() == "GaussNewton")
  {
  using hessian_t       = Eigen::Matrix<scalar_type, -1, -1>;
  using solver_tag      = pressio::linsol::direct::HouseholderQR;
  using linear_solver_t = pressio::linsol::Solver<solver_tag, hessian_t>;
  linear_solver_t linearSolver;

  auto solver = pressio::nlsol::create_gauss_newton_solver(lspgStepper, linearSolver);
  solver.setStopCriterion(pnlins::Stop::WhenAbsolutel2NormOfGradientBelowTolerance);
  solver.setStopTolerance(parser.nonlinearSolverTolerance());

  auto policy = pode::steps_fixed_dt(startTime, numSteps, parser.timeStepSize());
  pode::advance(lspgStepper, reducedState, policy, solver, observer);
  }

  else if (parser.nonlinearSolver() == "GaussNewtonQR")
  {
  using mat_t = typename LspgStepperType::jacobian_type;
  using qr_solver_t = pressio::qr::QRSolver<mat_t, pressio::qr::Householder>;
  qr_solver_t qrSolver;
  auto solver = pressio::nlsol::experimental::create_gauss_newton_qr_solver(lspgStepper, qrSolver);
  solver.setStopCriterion(pnlins::Stop::WhenAbsolutel2NormOfGradientBelowTolerance);
  solver.setStopTolerance(parser.nonlinearSolverTolerance());

  auto policy = pode::steps_fixed_dt(startTime, numSteps, parser.timeStepSize());
  pode::advance(lspgStepper, reducedState, policy, solver, observer);
  }

  else if (parser.nonlinearSolver() == "LevenbergMarquardt")
  {
  using hessian_t       = Eigen::Matrix<scalar_type, -1, -1>;
  using solver_tag      = pressio::linsol::direct::HouseholderQR;
  using linear_solver_t = pressio::linsol::Solver<solver_tag, hessian_t>;
  linear_solver_t linearSolver;

  auto solver = pressio::nlsol::create_levenberg_marquardt_solver(lspgStepper, linearSolver);
  solver.setUpdateCriterion(pnlins::Update::LMSchedule2);
  solver.setStopCriterion(pnlins::Stop::WhenAbsolutel2NormOfGradientBelowTolerance);
  solver.setStopTolerance(parser.nonlinearSolverTolerance());

  auto policy = pode::steps_fixed_dt(startTime, numSteps, parser.timeStepSize());
  pode::advance(lspgStepper, reducedState, policy, solver, observer);
  }
}
#endif
