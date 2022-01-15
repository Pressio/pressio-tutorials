/*
//@HEADER
// ************************************************************************
//
// tutorial1.cc
//                          Pressio
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

#include "pressio/type_traits.hpp"
#include "pressio/ops.hpp"
#include "pressio/rom_galerkin.hpp"

const std::vector<int> indices = {1,3,5,7,9,11,13,15,17,19};

struct TrivialFomVelocityAndJacobianEigen
{
  using scalar_type       = double;
  using state_type        = Eigen::VectorXd;
  using velocity_type     = state_type;
  int N_ = {};

  TrivialFomVelocityAndJacobianEigen(int N) : N_(N){}

  velocity_type createVelocity() const{ return velocity_type(N_); }

  template<class OperandType>
  OperandType createApplyJacobianResult(const OperandType & B) const
  {
    OperandType A(N_, B.cols());
    return A;
  }

  // computes: A = Jac B
  template<class OperandType>
  void applyJacobian(const state_type & state,
                     const OperandType & B,
                     const scalar_type & time,
                     OperandType & A) const
  {
    for (std::size_t i=0; i<indices.size(); ++i){
      for (int j=0; j< A.cols(); ++j){
        A(i,j) = B(indices[i], j);
      }
    }
    A.array() += time;
  }

  void velocity(const state_type & u, const scalar_type time, velocity_type & f) const
  {
    for (std::size_t i=0; i<indices.size(); ++i){
     f(i) = u(indices[i]) + time;
    }
  }
};

// for explicit, projector acts on FOM velicity
struct ProjectorExplicitEigen
{
  using operator_type = Eigen::MatrixXd;
  operator_type matrix_;

  ProjectorExplicitEigen(const operator_type & phi) : matrix_(phi){}

  template<class operand_type, class ScalarType,  class ResultType>
  void operator()(const operand_type & operand, const ScalarType time, ResultType & result) const
  {
    result = matrix_.transpose() * operand;
  }
};

using ProjectorImplicitEigen = ProjectorExplicitEigen;

struct Observer
{
  void operator()(int32_t step, double time, Eigen::VectorXd state)
  {
  	std::cout << "Observer called: \n"
  	<< " step: " << step
  	<< "\n"
  	<< " state = ";
  	for (int i=0; i<state.size(); ++i){
  		std::cout << state(i) << ", ";
  	}
  	std::cout << "\n";
  }
};


int main(int argc, char *argv[])
{
  pressio::log::initialize(pressio::logto::terminal);

  namespace pls    = pressio::linearsolvers;
  namespace pnonls = pressio::nonlinearsolvers;
  namespace prom   = pressio::rom;

  using fom_t = TrivialFomVelocityAndJacobianEigen;
  using scalar_t    = typename fom_t::scalar_type;
  using fom_state_t = typename fom_t::state_type;

  const int nstencil = 20;
  const int nSample  = 10;
  fom_t fomSystem(nSample);
  fom_state_t fomReferenceState(nstencil);
  fomReferenceState.setZero();

  using phi_t = Eigen::MatrixXd;
  phi_t phi(nstencil, 3);
  phi.col(0).setConstant(0.);
  phi.col(1).setConstant(1.);
  phi.col(2).setConstant(2.);
  phi.row(0).setConstant(-111.);
  phi.row(2).setConstant(-111.);
  phi.row(4).setConstant(111.);
  phi.row(6).setConstant(423.);
  phi.row(8).setConstant(-21.);
  phi.row(10).setConstant(423.);
  phi.row(12).setConstant(-21.);
  phi.row(14).setConstant(423.);
  phi.row(16).setConstant(-21.);
  phi.row(18).setConstant(-21.);
  auto decoder = pressio::rom::create_time_invariant_linear_decoder<fom_state_t>(phi);

  Eigen::VectorXd romState(3);
  romState[0]=0.;
  romState[1]=1.;
  romState[2]=2.;

  /* projector must be applicable to the *sample* operand */
  phi_t matForProj(nSample, 3);
  matForProj.col(0).setConstant(0.);
  matForProj.col(1).setConstant(1.);
  matForProj.col(2).setConstant(2.);
  ProjectorExplicitEigen proj(matForProj);

  auto problem = pressio::rom::galerkin::create_hyperreduced_implicit_problem(
    pressio::ode::StepScheme::BDF1, fomSystem, decoder, romState, fomReferenceState, proj);

  using galerkin_jacobian_t = Eigen::MatrixXd;
  using lin_solver_t = pls::Solver<pls::iterative::LSCG, galerkin_jacobian_t>;
  lin_solver_t linearSolverObj;
  auto nonLinSolver = pnonls::create_newton_raphson(problem, romState, linearSolverObj);

  scalar_t dt = 2.;
  Observer obs;
  pressio::ode::advance_n_steps_and_observe(problem, romState, 0.0, dt, 2, obs, nonLinSolver);

  // check solution
  std::cout << "Computed solution: ";
  for (int i=0; i<romState.size(); ++i){
    std::cout << romState(i) << ", ";
  }
  std::cout << "\n"
            << "         expected: "
            << "4.22215e-21, -0.803896, -1.60779," << std::endl;

  pressio::log::finalize();
}
