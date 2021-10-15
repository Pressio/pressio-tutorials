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
#include "custom_data_types.hpp"
#include "pressio/ops.hpp"

namespace pressio{

template<class ScalarType>
struct Traits<CustomVector<ScalarType>>{
  using scalar_type = ScalarType;
  using size_type = std::size_t;
};

template<class ScalarType>
struct Traits<CustomMatrix<ScalarType>>{
  using scalar_type = ScalarType;
  using size_type = std::size_t;
};

namespace ops{

template<class ScalarType>
std::size_t extent(CustomVector<ScalarType> & object, int i){
    return object.extent(i);
}

template<class ScalarType>
std::size_t extent(CustomMatrix<ScalarType> & object, int i){
    return object.extent(i);
}

template<class ScalarType>
void set_zero(CustomVector<ScalarType> & object){
  object.fill(0);
}

template<class ScalarType>
void set_zero(CustomMatrix<ScalarType> & object){
  object.fill(0);
}

template<class ScalarType>
void deep_copy(CustomVector<ScalarType> & dest,
               const CustomVector<ScalarType> & src){
  dest = src;
}

template<class ScalarType>
void deep_copy(CustomMatrix<ScalarType> & dest,
               const CustomMatrix<ScalarType> & src){
  dest = src;
}

template<class ScalarType>
CustomVector<ScalarType> clone(const CustomVector<ScalarType> & src){
  return CustomVector<ScalarType>(src.extent(0));
}

template<class ScalarType>
CustomMatrix<ScalarType> clone(const CustomMatrix<ScalarType> & src){
  return CustomMatrix<ScalarType>(src.extent(0), src.extent(1));
}

template<class ScalarType>
void update(CustomVector<ScalarType> & v,        const ScalarType a,
            const CustomVector<ScalarType> & v1, const ScalarType b)
{
  for (std::size_t i=0; i< v.extent(0); ++i){
    v(i) = a*v(i) + b*v1(i);
  }
}

// z = beta*z + alpha * A * x
// where x is indexable as x(i)
template<class x_t, class ScalarType>
void product(pressio::nontranspose,
             ScalarType alpha,
             const CustomMatrix<ScalarType> & A,
             const x_t & x,
             ScalarType beta,
             CustomVector<ScalarType> & z)
{
  // obviously not efficient, just for demonstration
  for (std::size_t i=0; i<A.extent(0); ++i)
  {
    z(i) = beta*z(i);
    for (std::size_t j=0; j<A.extent(1); ++j){
      z(i) += alpha*A(i,j)*x(j);
    }
  }
}

// z = beta*z + alpha * A^T * x
// where z is indexable as z(i)
template<class z_t, class ScalarType>
void product(pressio::transpose,
             ScalarType alpha,
             const CustomMatrix<ScalarType> & A,
             const CustomVector<ScalarType> & x,
             ScalarType beta,
             z_t & z)
{
  // obviously not efficient, just for demonstration
  for (std::size_t k=0; k<A.extent(1); ++k)
  {
    z(k) = beta*z(k);
    for (std::size_t i=0; i<A.extent(0); ++i){
      z(k) += alpha*A(i,k)*x(i);
    }
  }
}

// C = beta*C + alpha * A^T * B
// where C is indexable as C(i,j)
template<class C_t, class ScalarType>
void product(pressio::transpose,
             pressio::nontranspose,
             ScalarType alpha,
             const CustomMatrix<ScalarType> & A,
             const CustomMatrix<ScalarType> & B,
             ScalarType beta,
             C_t & C)
{
  for (std::size_t i=0; i<A.extent(1); ++i){
    for (std::size_t j=0; j<B.extent(1); ++j)
    {
      C(i,j) *= beta;
      for (std::size_t k=0; k<A.extent(0); ++k){
        C(i,j) += alpha*A(k,i)*B(k,j);
      }
    }
  }
}

}}//end namespace pressio::ops

#include "pressio/rom_galerkin.hpp"

struct TrivialFomOnlyVelocityCustomTypes
{
  using scalar_type    = double;
  using state_type     = CustomVector<scalar_type>;
  using velocity_type  = state_type;
  int N_ = {};

  TrivialFomOnlyVelocityCustomTypes(int N): N_(N){}

  velocity_type createVelocity() const{ return velocity_type(N_); }

  void velocity(const state_type & u, const scalar_type time, velocity_type & f) const{
    for (std::size_t i=0; i<f.extent(0); ++i){
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
  pressio::log::setVerbosity({pressio::log::level::debug});

  using fom_t = TrivialFomOnlyVelocityCustomTypes;
  using scalar_t    = typename fom_t::scalar_type;
  using fom_state_t = typename fom_t::state_type;

  constexpr int N = 10;
  fom_t fomSystem(N);
  fom_state_t fomReferenceState(N);
  fomReferenceState.fill(0);

  using phi_t = ::CustomMatrix<scalar_t>;
  phi_t phi(N, 3);
  for (std::size_t i=0; i<phi.extent(0); ++i){
    phi(i,0) = 0.1;
    phi(i,1) = 0.5;
    phi(i,2) = 0.8;
  }
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
