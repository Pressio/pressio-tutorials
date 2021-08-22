/*
//@HEADER
// ************************************************************************
//
// tutorial2.cc
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

#include <vector>
#include "pressio/type_traits.hpp"

struct MyCustomVector
{
  using value_type = double;
  using size_type = std::size_t;

  MyCustomVector(std::size_t sizeIn)
    : m_data(sizeIn){}

  std::size_t size() const{
    return m_data.size();
  }

  value_type & operator[](std::size_t index){
    return m_data[index];
  }

  const value_type & operator[](std::size_t index) const{
    return m_data[index];
  }

private:
  std::vector<value_type> m_data;
};

namespace pressio{

template<> struct Traits<MyCustomVector>{
  using scalar_type = typename MyCustomVector::value_type;
};

namespace ops{
void deep_copy(MyCustomVector & dest, const MyCustomVector & from){
  dest = from;
}

MyCustomVector clone(const MyCustomVector & src){
  return MyCustomVector(src.size());
};

void update(MyCustomVector & v,        const double a,
            const MyCustomVector & v1, const double b)
{
  for (size_t i=0; i<v.size(); ++i){
    v[i] = a*v[i] + b*v1[i];
  }
}
}} //end namespace pressio::ops

#include "pressio/ode_advancers.hpp"
#include "pressio/ode_steppers_explicit.hpp"

struct MySystem
{
  using scalar_type   = double;
  using state_type    = MyCustomVector;
  using velocity_type = state_type;

public:
  void velocity(const state_type & y,
                scalar_type t,
                velocity_type & rhs) const
  {
    constexpr scalar_type ten{10};
    rhs[0] = ten * y[0];
    rhs[1] = ten * y[1];
    rhs[2] = ten * y[2];
  };

  velocity_type createVelocity() const{
    return velocity_type(3);
  };
};

int main(int argc, char *argv[])
{
  std::cout << "Running tutorial\n";

  // create the system/app object
  using app_t = MySystem;
  app_t appObj;

  // create and initialize the state
  using state_t = typename app_t::state_type;
  state_t y(3);
  y[0] = 1.; y[1] = 2.; y[2] = 3.;

  // instantiate the stepper
  namespace pode = pressio::ode;
  auto stepperObj = pode::create_forward_euler_stepper(y, appObj);

  // integrate in time
  const double dt = 0.1;
  const pressio::ode::step_count_type numSteps = 1;
  ::pressio::ode::advance_n_steps(stepperObj, y, 0.0, dt, numSteps);

  // check solution
  std::cout << "Computed solution: ["
            << y[0] << " " << y[1] << " " << y[2] << "] "
	    << "Expected solution: [2,4,6] "
	    << std::endl;

  return 0;
}
