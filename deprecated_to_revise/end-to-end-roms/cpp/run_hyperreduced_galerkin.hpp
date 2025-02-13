/*
//@HEADER
// ************************************************************************
//
// run_hyperreduced_galerkin.hpp
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

#ifndef RUN_HYPRED_GALERKIN_HPP_
#define RUN_HYPRED_GALERKIN_HPP_

#include "pressio/rom_subspaces.hpp"
#include "pressio/rom_galerkin_unsteady.hpp"
#include "observer.hpp"
#include "rom_shared.hpp"

template<class ScalarType, class FomRhsType>
class ExplicitGalerkinHyperReducer
{
  static_assert(pressio::is_vector_eigen<FomRhsType>::value,
		"FomRhsType must be an Eigen vector");

  using matrix_type = Eigen::Matrix<ScalarType, -1, -1, Eigen::ColMajor>;
  matrix_type P_;

public:
  using time_type = ScalarType;
  using right_hand_side_operand_type = FomRhsType;

  ExplicitGalerkinHyperReducer(const std::string & projFile,
			       const int numModes){
    P_ = create_colmajor_eigen_matrix_and_load_from_binary_with_extents<
      ScalarType>(projFile, numModes);
  }

  template<class ResultType>
  std::enable_if_t<pressio::is_vector_eigen<ResultType>::value>
  operator()(const right_hand_side_operand_type & operand,
	     const time_type & /*unused*/,
	     ResultType & result) const
  {
    result = P_.transpose() * operand;
  }
};

template<class FomSystemType, class ParserType>
void run_galerkin_hyperreduced(const FomSystemType & fomSystem,
			       const ParserType & parser)
{
  namespace pode = pressio::ode;
  namespace prom = pressio::rom;
  namespace pgal = pressio::rom::galerkin;

  using scalar_type = typename FomSystemType::scalar_type;
  using reduced_state_type = Eigen::Matrix<scalar_type, Eigen::Dynamic, 1>;

  const auto modeCount   = parser.romModeCount();
  const auto & basisFile = parser.romFullMeshPodBasisFile();
  auto basisFull         = create_basis_and_read_from_file<scalar_type>(basisFile, modeCount);
  auto basisOnStencil    = create_basis_on_stencil_mesh(basisFull, parser);
  auto affineShift       = create_affine_shift_on_stencil_mesh<scalar_type>(parser);
  const auto trialSpace  = prom::create_trial_column_subspace<
     reduced_state_type>(std::move(basisOnStencil), std::move(affineShift), parser.romIsAffine());

  auto reducedState = trialSpace.createReducedState();
  fill_rom_state_from_ascii(parser.romInitialStateFile(), reducedState);

  StateObserver observer(parser.stateSamplingFreq());
  const auto odeScheme = parser.odeScheme();
  if (pressio::ode::is_explicit_scheme(odeScheme))
  {
  using fom_rhs_type = typename FomSystemType::right_hand_side_type;
  using hr_op_t = ExplicitGalerkinHyperReducer<scalar_type, fom_rhs_type>;
  hr_op_t H(parser.romGalerkinHypRedOperatorFile(), modeCount);

  auto problem = pgal::create_unsteady_explicit_problem(odeScheme, trialSpace, fomSystem, H);

  typename FomSystemType::time_type startTime{0};
  const auto numSteps = pode::StepCount{parser.numSteps()};
  pode::advance_n_steps(problem, reducedState, startTime,
			parser.timeStepSize(), numSteps, observer);
  }
  else{
    throw std::runtime_error("Hyper-reduced implicit galerkin not impl yet");
  }
}

#endif
