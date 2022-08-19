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

struct TrivialFomOnlyVelocityEigen
{
  using scalar_type	      = double;
  using state_type	      = Eigen::VectorXd;
  using velocity_type     = state_type;
  int N_ = {};

  TrivialFomOnlyVelocityEigen(int N): N_(N){}

  velocity_type createVelocity() const{ return velocity_type(N_); }

  void velocity(const state_type & u, const scalar_type time, velocity_type & f) const
  {
    for (auto i=0; i<f.rows(); ++i){
	   f(i) = u(i) + time;
    }
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

  using fom_t	= TrivialFomOnlyVelocityEigen;
  using scalar_t    = typename fom_t::scalar_type;
  using fom_state_t	= typename fom_t::state_type;

  constexpr int N = 10;
  fom_t fomSystem(N);
  fom_state_t fomReferenceState(N);
  fomReferenceState.setZero();

  using phi_t = Eigen::MatrixXd;
  phi_t phi(N, 3);
  phi.col(0).setConstant(0.1);
  phi.col(1).setConstant(0.5);
  phi.col(2).setConstant(0.8);
  auto decoder = pressio::rom::create_time_invariant_linear_decoder<fom_state_t>(phi);

  Eigen::VectorXd romState(3);
  romState[0]=0.;
  romState[1]=1.;
  romState[2]=2.;

  constexpr auto odescheme = pressio::ode::StepScheme::ForwardEuler;
  auto problem = pressio::rom::galerkin::create_default_explicit_problem
    (odescheme, fomSystem, decoder, romState, fomReferenceState);

  const scalar_t dt = 1.;
  const int num_steps = 3;
  Observer obs;
  pressio::ode::advance_n_steps_and_observe(problem, romState, 0., dt, num_steps, obs);

  pressio::log::finalize();
}
