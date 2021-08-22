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
#include "pressio/ode_steppers_explicit.hpp"
#include <Eigen/Core>

template<class scalar_t>
struct MySystem{
  using scalar_type   = scalar_t;
  using state_type    = Eigen::Matrix<scalar_t,-1,1>;
  using velocity_type = state_type;

public:
  void velocity(const state_type & y,
                const scalar_type t,
                velocity_type & rhs) const
  {
    constexpr scalar_type ten{10};
    rhs(0) = ten * y(0);
    rhs(1) = ten * y(1);
    rhs(2) = ten * y(2);
  };

  velocity_type createVelocity() const{
    return velocity_type(3);
  };
};

int main(int argc, char *argv[])
{
  std::cout << "Running tutorial\n";

  using scalar_type = double;

  // create the system/app object
  using app_t = MySystem<scalar_type>;
  app_t appObj;

  // create and initialize the state
  using state_t = typename app_t::state_type;
  state_t y(3);
  y(0) = 1.; y(1) = 2.; y(2) = 3.;

  // instantiate the stepper
  namespace pode = pressio::ode;
  auto stepperObj = pode::create_forward_euler_stepper(y, appObj);

  // integrate in time
  const scalar_type dt = 0.1;
  const scalar_type startTime = 0.0;
  const pode::step_count_type numSteps = 1;
  pode::advance_n_steps(stepperObj, y, startTime, dt, numSteps);

  // check solution
  std::cout << "Computed solution: ["
            << y(0) << " " << y(1) << " " << y(2) << "] "
            << "Expected solution: [2,4,6] "
            << std::endl;

  return 0;
}
