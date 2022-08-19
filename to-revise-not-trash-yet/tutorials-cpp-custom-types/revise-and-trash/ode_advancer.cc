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
struct BasicSteppableClass{
public:
  void operator()(std::vector<ScalarType> & state,
                  const ScalarType time,
                  const ScalarType dt,
                  const pressio::ode::step_count_type step) const
  {
    // for simplicity, let's say that taking one step
    // corresponds to adding 1 to the state
    state[0] += dt;
    state[1] += dt;
    state[2] += dt;
  };
};

template<class ScalarType>
struct SteppableClassThatNeedsSomethingElse{
public:

  template<class SomethingElse>
  void operator()(std::vector<ScalarType> & state,
                  const ScalarType time,
                  const ScalarType dt,
                  const pressio::ode::step_count_type step,
		  SomethingElse & my_auxiliary_object) const
  {
    // just for demonstration, we might need auxiliary object
    // to do something, but we don't know the type in advance
    state[0] += 1;
    state[1] += 1;
    state[2] += 1;

    my_auxiliary_object.aComplicatedMethod(state);
  };
};

struct Foo
{
  template<class ScalarType>
  void aComplicatedMethod(std::vector<ScalarType> & state){
    // do something
  }
};

void f1()
{
  /*
    time-step is fixed
   */
  namespace pode = pressio::ode;
  using scalar_type = double;

  BasicSteppableClass<scalar_type> stepper;
  std::vector<scalar_type> state = {1,2,3};
  const scalar_type dt = 0.1;
  const scalar_type start_time = 0.0;
  const pode::step_count_type num_steps = 10;
  pode::advance_n_steps(stepper, state, start_time, dt, num_steps);
  std::cout << "Final state: ["
	    << state[0] << " " << state[1] << " " << state[2] << "] "
	    << std::endl;
}

void f2()
{
  /*
    time-step is fixed, and stepper needs an auxiliary object
   */
  namespace pode = pressio::ode;
  using scalar_type = double;

  SteppableClassThatNeedsSomethingElse<scalar_type> stepper;
  std::vector<scalar_type> state = {1,2,3};
  const scalar_type dt = 0.1;
  const scalar_type start_time = 0.0;
  const pode::step_count_type num_steps = 10;

  Foo aux;
  pode::advance_n_steps(stepper, state, start_time, dt, num_steps, aux);
  std::cout << "Final state: ["
	    << state[0] << " " << state[1] << " " << state[2] << "] "
	    << std::endl;
}

void f3()
{
  /*
    time-step is specified by the user with a functor
   */
  namespace pode = pressio::ode;
  using scalar_type = double;

  BasicSteppableClass<scalar_type> stepper;
  std::vector<scalar_type> state = {1,2,3};
  const scalar_type dt = 0.1;
  const scalar_type start_time = 0.0;
  const pode::step_count_type num_steps = 10;

  auto dt_setter = [](const pode::step_count_type step,
		      const scalar_type & time,
		      scalar_type & dt)
  {
    // step starts at 1 for the first step

    // set the time step size you want at each step, time
    dt = (scalar_type) step;
  };

  pode::advance_n_steps(stepper, state, start_time, dt_setter, num_steps);
  std::cout << "Final state: ["
	    << state[0] << " " << state[1] << " " << state[2] << "] "
	    << std::endl;
}

struct MyObserver{
  template<class ScalarType>
  void operator()(const pressio::ode::step_count_type step,
                  const ScalarType time,
		  const std::vector<ScalarType> & state) const
  {
    // do what you want with the state, e.g. print it
    std::cout << "Step = " << step << '\n';
    std::cout << "state: ["
	    << state[0] << " " << state[1] << " " << state[2] << "] "
	    << '\n';
  }
};

void f4()
{
  /*
    time-step is fixed, user passes an observer to monitor state
   */
  namespace pode = pressio::ode;
  using scalar_type = double;

  BasicSteppableClass<scalar_type> stepper;
  std::vector<scalar_type> state = {1,2,3};
  const scalar_type dt = 0.1;
  const scalar_type start_time = 0.0;
  const pode::step_count_type num_steps = 10;

  MyObserver monitor;
  pode::advance_n_steps_and_observe(stepper, state, start_time, dt, num_steps, monitor);
}

void f5()
{
  /*
    time-step is provided
   */
  namespace pode = pressio::ode;
  using scalar_type = double;

  BasicSteppableClass<scalar_type> stepper;
  std::vector<scalar_type> state = {1,2,3};
  const scalar_type dt = 0.1;
  const scalar_type start_time = 0.0;
  const scalar_type final_time = 2.8;
  const pode::step_count_type num_steps = 10;

  auto dt_setter = [](const pode::step_count_type step,
		      const scalar_type & time,
		      scalar_type & dt)
  {
    // step starts at 1 for the first step

    // set the time step size you want at each step, time
    dt = (scalar_type) step + (scalar_type) 0.5;
    std::cout << " step = " << step << " dt = " << dt << '\n';
  };

  pode::advance_to_target_time(stepper, state, start_time, final_time, dt_setter);
}

int main(int argc, char *argv[])
{
  pressio::log::initialize(pressio::logto::terminal);
  f1();
  f2();
  f3();
  f4();
  f5();
  pressio::log::finalize();
  return 0;
}
