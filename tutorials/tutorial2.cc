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

#include "pressio_ode.hpp"
#include <Eigen/Core>

struct MyApp{
  using scalar_type   = double;
  using state_type    = Eigen::VectorXd;
  using velocity_type = state_type;

public:
  void velocity(const state_type & y,
		scalar_type t,
		velocity_type & R) const{
    R(0) = 10. * y(0);
    R(1) = 10. * y(1);
    R(2) = 10. * y(2);
  };

  velocity_type createVelocity() const{
    return velocity_type(3);
  };
};

int main(int argc, char *argv[]){

  // We illustrate here how to leverage the package pressio/ode to do
  // explicit time-integration for a system of ODEs with Eigen data types,
  // which is supported by pressio.
  //
  // Suppose that you need to use some pressio/ode package for doing explicit time
  // integration of a system of ODEs which is implemented as in MyApp above.

  // For the sake of explanation, MyApp at the top of this page is a class
  // that defines the target system of ODEs and meets
  // the API needed by pressio to run explicit time integration, i.e.:
  // (a) it has typedefs that pressio detects for scalar, state and velicity
  // (b) it has two overleads for the velocity() method (one void and one non-void)

  // As long as the user-defined class/app or whatever name you want to call has an
  // API as the one in MyApp, then it can be readily used with pressio.
  // Note that if you try to run an explicit time integration with a user-defined system
  // that does not satisfy the target API, pressio throws a compile-time error.

  // for this tutorial, let us run Forward Euler on the system defined by MyApp.

  // *** Stage (a): define some types ***
  using app_t		= MyApp;
  using scalar_t	= typename app_t::scalar_type;
  using native_state_t  = typename app_t::state_type;
  using native_veloc_t  = typename app_t::velocity_type;

  // *** Stage (b): create the app object ***
  app_t appObj;

  // *** Stage (c): define pressio wrapper types for the state ***
  // in this case, pressio behind the scenes detects what type you
  // are passing as template argument and since it is not (for now) supported,
  // pressio still wraps the object but does not know how to do anythin else.
  using state_t = ::pressio::containers::Vector<native_state_t>;

  // *** Stage (d): create the initial state object ***
  state_t y(3);

  // *** Stage (e): fill the initial state vector ***
  // any pressio wrapper provides the data() method to get a pointer to the wrapped object
  auto yptr = y.data();
  // i can now use regular std vector [] operator to fill in
  (*yptr)[0] = 1.; (*yptr)[1] = 2.; (*yptr)[2] = 3.;

  // *** Stage (f): create the pressio stepper ***
  using ode_tag = ::pressio::ode::explicitmethods::Euler;
  using stepper_t = ::pressio::ode::ExplicitStepper<ode_tag, state_t, app_t>;
  stepper_t stepperObj(y, appObj);

  // *** Stage (g): integrated in time ***
  scalar_t dt = 0.1;
  ::pressio::ode::advanceNSteps(stepperObj, y, 0.0, dt, 1ul);

  // note that for this system and settings, the solution printed should be 2,4,6
  std::cout << "Computed solution: ["
            << (*yptr)[0] << " " << (*yptr)[1] << " " << (*yptr)[2] << "] "
	    << "Expected solution: [2,4,6] "
	    << std::endl;

  return 0;
}
