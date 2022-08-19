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

struct TrivialFomVelocityAndJacobianEigen
{
  using scalar_type       = double;
  using state_type        = Eigen::VectorXd;
  using velocity_type     = state_type;
  int N_ = {};
  const std::vector<int> indices_to_corrupt_ = {};

  TrivialFomVelocityAndJacobianEigen(int N, std::vector<int> ind)
    : N_(N), indices_to_corrupt_(ind){}

  velocity_type createVelocity() const{ return velocity_type(N_); }

  template<class OperandType>
  OperandType createApplyJacobianResult(const OperandType & B) const
  {
    return OperandType(B.rows(), B.cols());
  }

  // computes: A = Jac B
  template<class OperandType>
  void applyJacobian(const state_type & state,
                     const OperandType & B,
                     const scalar_type & time,
                     OperandType & A) const
  {
    A = B;
    A.array() += time;
    // corrupt some to ensure masking works
    for (auto & it : indices_to_corrupt_){
     A.row(it).setConstant(-1114);
    }
  }

  void velocity(const state_type & u, const scalar_type time, velocity_type & f) const
  {
    for (auto i=0; i<f.rows(); ++i){
     f(i) = u(i) + time;
    }
    // corrupt some to ensure masking works
    for (auto & it : indices_to_corrupt_){
     f(it) = -1114;
    }
  }
};

// for explicit, masker acts on FOM velicity
struct MaskerExplicitEigen
{
  const std::vector<int> sample_indices_ = {};
  using operand_type = typename TrivialFomVelocityAndJacobianEigen::velocity_type;

  MaskerExplicitEigen(std::vector<int> sample_indices) : sample_indices_(sample_indices) {}

  operand_type createApplyMaskResult(const operand_type & operand) const{
    return operand_type(sample_indices_.size());
  }

  template<class TimeType>
  void operator()(const operand_type & operand, TimeType time, operand_type & result) const
  {
    for (std::size_t i=0; i<sample_indices_.size(); ++i){
      result(i) = operand(sample_indices_[i]);
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
  void operator()(const operand_type & operand, const ScalarType time,
    ResultType & result) const
  {
    result = matrix_.transpose() * operand;
  }
};

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

  using fom_t	= TrivialFomVelocityAndJacobianEigen;
  using scalar_t    = typename fom_t::scalar_type;
  using fom_state_t = typename fom_t::state_type;

  constexpr int nFull = 20;
  const std::vector<int> sample_indices = {0,2,4,6,8,10,12,14,16,18};
  const int nMasked = sample_indices.size();
  /* corrupt indices are those that we mess up on purpose */
  const std::vector<int> corrupt_indices = {1,7,13,19};

  fom_t fomSystem(nFull, corrupt_indices);
  fom_state_t fomReferenceState(nFull);
  fomReferenceState.setZero();

  /* the decoder must be valid to reconstruct fhe full fom */
  using phi_t = Eigen::MatrixXd;
  phi_t phiFull(nFull, 3);
  phiFull.col(0).setConstant(0.);
  phiFull.col(1).setConstant(1.);
  phiFull.col(2).setConstant(2.);
  /* corrupting the entries in phiFull is only way we */
  /* can ensure the masking works properly */
  phiFull.row(1).setConstant(-111.);
  phiFull.row(7).setConstant(111.);
  phiFull.row(11).setConstant(423.);
  phiFull.row(17).setConstant(-21.);
  auto decoder = pressio::rom::create_time_invariant_linear_decoder<fom_state_t>(phiFull);

  Eigen::VectorXd romState(3);
  romState[0]=0.;
  romState[1]=1.;
  romState[2]=2.;
  /* projector must be applicable to the *masked* operand*/
  /* so we need to use only certain rows of phi*/
  phi_t phiSample(nMasked, 3);
  for (int i = 0; i < nMasked; ++i){
    phiSample(i, 0) = phiFull(sample_indices[i],0);
    phiSample(i, 1) = phiFull(sample_indices[i],1);
    phiSample(i, 2) = phiFull(sample_indices[i],2);
  }

  MaskerExplicitEigen masker(sample_indices);
  ProjectorExplicitEigen proj(phiSample);

  namespace gal = pressio::rom::galerkin;
  auto problem = gal::create_masked_explicit_problem(pressio::ode::StepScheme::ForwardEuler,
    fomSystem, decoder, romState, fomReferenceState, proj, masker);

  const scalar_t dt = 1.;
  const int num_steps = 2;
  Observer obs;
  pressio::ode::advance_n_steps_and_observe(
    problem, romState, 0., dt, num_steps, obs);

  // check solution
  std::cout << "Computed solution: ";
  for (int i=0; i<romState.size(); ++i){
    std::cout << romState(i) << ", ";
  }
  std::cout << "\n"
            << "         expected: "
            << "0, 2611, 5222," << std::endl;

  pressio::log::finalize();
}
