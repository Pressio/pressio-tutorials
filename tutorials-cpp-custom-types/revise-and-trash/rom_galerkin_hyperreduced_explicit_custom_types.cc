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
void set_zero(CustomVector<ScalarType> & object){
  object.fill(0);
}

template<class ScalarType>
void set_zero(CustomMatrix<ScalarType> & object){
  object.fill(0);
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

} // namespace ops
} // namespace pressio

#include "pressio/rom_galerkin.hpp"

const std::vector<int> indices = {1,3,5,7,9,11,13,15,17,19};

struct TrivialFomOnlyVelocityCustomTypes
{
  using scalar_type       = double;
  using state_type        = CustomVector<scalar_type>;
  using velocity_type     = state_type;
  int N_ = {};

  TrivialFomOnlyVelocityCustomTypes(int N) : N_(N){
    assert((std::size_t)N==(std::size_t)indices.size());
  }

  velocity_type createVelocity() const{ return velocity_type(N_); }

  void velocity(const state_type & u, const scalar_type time, velocity_type & f) const
  {
    for (std::size_t i=0; i<indices.size(); ++i){
     f(i) = u(indices[i]) + time;
    }
  }
};

// for explicit, projector acts on FOM velicity
template<class ScalarType>
struct ProjectorExplicitCustomTypes
{
  using operator_type = CustomMatrix<ScalarType>;
  operator_type matrix_;

  ProjectorExplicitCustomTypes(const operator_type & phi) : matrix_(phi){}

  // result is the projected RHS, so it is a rom type
  template<class operand_type>
  void operator()(const operand_type & operand, ScalarType time, Eigen::VectorXd & result) const
  {
    // obviously not efficient, just for demonstration
    for (std::size_t k=0; k<matrix_.extent(1); ++k)
    {
      result(k) = 0;
      for (std::size_t i=0; i<matrix_.extent(0); ++i){
        result(k) += matrix_(i,k)*operand(i);
      }
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

  using fom_t	= TrivialFomOnlyVelocityCustomTypes;
  using scalar_t    = typename fom_t::scalar_type;
  using fom_state_t = typename fom_t::state_type;

  const int nstencil = 20;
  const int nSample  = 10;
  fom_t fomSystem(nSample);
  fom_state_t fomReferenceState(nstencil);
  fomReferenceState.fill(0);

  using phi_t = CustomMatrix<scalar_t>;
  phi_t phi(nstencil, 3);
  for (std::size_t i=0; i<phi.extent(0); ++i){
    phi(i,0) = 0;
    phi(i,1) = 1;
    phi(i,2) = 2;
  }
  for (std::size_t j=0; j<phi.extent(1); ++j){
    phi(0, j) = -111.;
    phi(2, j) = -111.;
    phi(4, j) = 111.;
    phi(6, j) = 423.;
    phi(8, j) = -21.;
    phi(10, j) = 423.;
    phi(12, j) = -21.;
    phi(14, j) = 423.;
    phi(16, j) = -21.;
    phi(18, j) = -21.;
  }
  auto decoder = pressio::rom::create_time_invariant_linear_decoder<fom_state_t>(phi);
  Eigen::VectorXd romState(3);
  romState[0]=0.;
  romState[1]=1.;
  romState[2]=2.;

  /* projector must be applicable to the *sample* operand */
  phi_t phiSample(nSample, 3);
  for (int i = 0; i < nSample; ++i){
    phiSample(i, 0) = 0;
    phiSample(i, 1) = 1;
    phiSample(i, 2) = 2;
  }
  ProjectorExplicitCustomTypes<scalar_t> proj(phiSample);

  namespace gal = pressio::rom::galerkin;
  auto problem = gal::create_hyperreduced_explicit_problem(
    pressio::ode::StepScheme::ForwardEuler, fomSystem, decoder, romState, fomReferenceState, proj);

  const scalar_t dt = 1.;
  const int num_steps = 2;
  Observer obs;
  pressio::ode::advance_n_steps_and_observe(problem, romState, 0., dt, num_steps, obs);

  // check solution
  std::cout << "Computed solution: ";
  for (int i=0; i<romState.size(); ++i){
    std::cout << romState(i) << ", ";
  }
  std::cout << "\n"
            << "         expected: "
            << "0, 2611, 5222," << std::endl;

  pressio::log::finalize();
  return 0;
}
