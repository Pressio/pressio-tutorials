/*
//@HEADER
// ************************************************************************
//
// tutorial3.cc
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

#include "pressio_ode.hpp"
#include <vector>

template<class scalar_t>
struct MyCustomVector
{
  using value_type = scalar_t;
  using size_type = std::size_t;

  MyCustomVector(std::size_t sizeIn)
    : m_data(sizeIn){}

  std::size_t size() const{
    return m_data.size();
  }

  scalar_t & operator[](std::size_t index){
    return m_data[index];
  }

  const scalar_t & operator[](std::size_t index) const{
    return m_data[index];
  }

private:
  std::vector<scalar_t> m_data;
};

template<class scalar_t>
struct MySystem{
  using scalar_type   = scalar_t;
  using state_type    = MyCustomVector<scalar_type>;
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

template <typename scalar_t>
struct MyOps
{
  using v_t = MyCustomVector<scalar_t>;
  using size_t = typename v_t::size_type;

  // compute: v = b*v1
  void update(v_t & v, const v_t & v1, const scalar_t b) const
  {
    for (size_t i=0; i<v.size(); ++i){
      v[i] = b*v1[i];
    }
  }

  // compute: v = a*v + b*v1
  void update(v_t & v, const scalar_t a,
	      const v_t & v1, const scalar_t b) const
  {
    for (size_t i=0; i<v.size(); ++i){
      v[i] = a*v[i] + b*v1[i];
    }
  }
};

int main(int argc, char *argv[])
{
  std::cout << "Running tutorial\n";

  using scalar_type = double;

  // create the system/app object
  using app_t = MySystem<scalar_type>;
  app_t appObj;

  // create and initialize the state
  using native_state_t = typename app_t::state_type;
  using state_t        = ::pressio::containers::Vector<native_state_t>;
  state_t y(3);
  // pressio wrappers expose the data() method, returning a pointer to the wrapped object
  auto & yNative = *y.data();
  // we can use the subscript operator on the native std::vector
  yNative[0] = 1.; yNative[1] = 2.; yNative[2] = 3.;

  // instantiate the class performing operations
  using my_custom_ops = MyOps<scalar_type>;
  my_custom_ops ops;

  // instantiate the stepper
  using ode_tag = ::pressio::ode::explicitmethods::Euler;
  using stepper_t = ::pressio::ode::ExplicitStepper<ode_tag, state_t, app_t, my_custom_ops>;
  stepper_t stepperObj(y, appObj, ops);

  // integrate in time
  const scalar_type dt = 0.1;
  const pressio::ode::types::step_t numSteps = 1;
  ::pressio::ode::advanceNSteps(stepperObj, y, 0.0, dt, numSteps);

  // check solution
  std::cout << "Computed solution: ["
            << yNative[0] << " " << yNative[1] << " " << yNative[2] << "] "
	    << "Expected solution: [2,4,6] "
	    << std::endl;

  return 0;
}
