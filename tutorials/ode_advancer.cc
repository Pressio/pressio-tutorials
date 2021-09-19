/*
//@HEADER
// ************************************************************************
//
// tutorial1.cc
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

#include "pressio/ode_advancers.hpp"
#include <vector>

template<class ScalarType>
struct SteppableClass
{

public:
  void operator()(std::vector<ScalarType> & state,
		  const ScalarType time,
		  const ScalarType dt,
		  const pressio::ode::step_count_type step) const
  {
    // for simplicity, let's say that taking one step
    // corresponds to adding 1 to the state
    state[0] += 1;
    state[1] += 1;
    state[2] += 1;
  };
};

int main(int argc, char *argv[])
{
  pressio::log::initialize(pressio::logto::terminal);
  namespace pode = pressio::ode;

  using scalar_type = double;
  SteppableClass<scalar_type> stepper;

  std::vector<scalar_type> state = {1,2,3};
  const scalar_type dt = 0.1;
  const scalar_type start_time = 0.0;
  const pode::step_count_type num_steps = 10;
  pode::advance_n_steps(stepper, state, start_time, dt, num_steps);

  std::cout << "Final state: ["
            << state[0] << " " << state[1] << " " << state[2] << "] "
            << std::endl;

  pressio::log::finalize();
  return 0;
}
