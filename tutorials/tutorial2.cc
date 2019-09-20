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

#include "CONTAINERS_ALL"
#include "ODE_EXPLICIT"
#include <vector>

struct MyApp{
  using scalar_type   = double;
  using state_type    = std::vector<scalar_type>;
  using velocity_type = state_type;

public:
  void velocity(const state_type & y,
		scalar_type t,
		velocity_type & R) const{
    R[0] = 10. * y[0];
    R[1] = 10. * y[1];
    R[2] = 10. * y[2];
  };

  velocity_type velocity(const state_type & y,
			 scalar_type t) const{
    velocity_type R(y);
    velocity(y, t, R);
    return R;
  };
};


struct updateOps{
  using v_t = std::vector<double>;

    static void do_update(v_t & v,
			  const v_t & v1, const double b){
    for (size_t i=0; i<v.size(); ++i)
      v[i] = b*v1[i];
  }

  static void do_update(v_t & v, const double a,
			const v_t & v1, const double b){
    for (size_t i=0; i<v.size(); ++i)
      v[i] = a*v[i] + b*v1[i];
  }
};

struct myops{
  using update_op = updateOps;
};


int main(int argc, char *argv[]){

  // We illustrate here how to leverage the package pressio/ode to do
  // explicit time-integration for a system of ODEs with arbitrary data types.
  //
  // As mentioned in tutorial1, pressio can handle types for which we have wrappers
  // as well as unknown arbitrary types. While for the former case pressio
  // knows how to do linear algebra, for the latter case, i.e. unknown types,
  // pressio does not know which methods those data structures support.
  //
  // pressio supports arbitrary type via generic programming and type introspection.
  // For example, if a vector container wrapper is templated on a Eigen::VectorXd,
  // pressio detects that and this information propagtes through the classes such that
  // when operators act on this vector or any other wrapper, pressio leverages the
  // native algebra of Eigen. If the user instantiates a vector templated on an
  // arbitrary vector type (for example user-defined), then pressio is also able to
  // detect that this is an ''unknonw'' type as far as pressio is concerned, and
  // therefore, the user needs to provide the necessary operations to do the algebra.
  // These operations can be passed to pressio either as function objects or static methods.
  // This discussion will be clear from the example below.

  // Suppose that you need to use some pressio/ode package for doing explicit time
  // integration of a system of ODEs which is implemented using the C++ STL.

  // for the sake of explanation, MyApp at the top of this page is a class
  // that defines the target system of ODEs using std::vectors<>, and meets
  // the API needed by pressio to run explicit time integration, i.e.:
  // (a) it has typedefs that pressio detects for scalar, state and velicity
  // (b) it has two overleads for the velocity() method (one void and one non-void)
  //
  // as long as the user-defined class/app or whatever name you want to call has an
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
  using state_t = containers::Vector<native_state_t>;

  // *** Stage (d): create the initial state object ***
  state_t y(3);

  // *** Stage (e): fill the initial state vector ***
  // any pressio wrapper provides the data() method to get a pointer to the wrapped object
  auto yptr = y.data();
  // i can now use regular std vector [] operator to fill in
  (*yptr)[0] = 1.; (*yptr)[1] = 2.; (*yptr)[2] = 3.;

  // *** Stage (f): create the pressio stepper ***
  // since we are using a custom data stucture type, we also need to inform pressio
  // how to do operations, like vector additions, etc.
  // To this end, we use a class "myops" see above, with static methods
  // with specific signatures tht pressio will look for.
  // pressio will complain at compile time if:
  // - you forget to pass the myops template to the stepper
  // - your ops class type does not have the right methods needed for the stepper you want
  // - your ops class type does not have methods that match what pressio needs.
  // pressio will introspect your type to check if it is an admissible ops class

  using my_custom_ops = myops<scalar_t>

  using stepper_t = ode::ExplicitStepper<
    ode::ExplicitEnum::Euler, state_t, app_t, res_t, scalar_t, my_custom_ops>;
  stepper_t stepperObj(y, appObj);

  // *** Stage (g): integrated in time ***
  scalar_t dt = 0.1;
  ode::integrateNSteps(stepperObj, y, 0.0, dt, 1ul);

  // note that for this system and settings, the solution printed should be 2,4,6
  std::cout << (*yptr2)[0] << " "
	    << (*yptr2)[1] << " "
	    << (*yptr2)[2] << std::endl;

  return 0;
}