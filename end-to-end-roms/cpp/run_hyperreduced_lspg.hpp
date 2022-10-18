/*
//@HEADER
// ************************************************************************
//
// run_hyperreduced_lspg.hpp
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

#ifndef RUN_HYPER_REDUCED_LSPG_HPP_
#define RUN_HYPER_REDUCED_LSPG_HPP_

#include "pressio/rom_subspaces.hpp"
#include "pressio/rom_lspg_unsteady.hpp"
#include "observer.hpp"
#include "lspg_pick_solver_and_run.hpp"
#include "rom_shared.hpp"

template<class IntType>
IntType find_index(const Eigen::Matrix<IntType, -1, 1> & vector,
		   IntType target)
{
  for (IntType j=0; j<vector.size(); ++j){
    if (vector[j] == target){
      return j;
    }
  }

  return std::numeric_limits<IntType>::max();
}

template<class ScalarType>
struct HypRedUpdater
{
  using vec_operand_type = Eigen::Matrix<ScalarType, -1, 1>;
  using mat_ll_operand_type = Eigen::Matrix<ScalarType, -1, -1>;
  std::vector<int> indices_;
  int numDofsPerCell_ = {};

  template<class ParserType>
  explicit HypRedUpdater(const ParserType & parser)
    : numDofsPerCell_(parser.numDofsPerCell())
  {
    const auto & stfile = parser.romStencilMeshGidsFile();
    const auto & safile = parser.romSampleMeshGidsFile();
    const auto stencilMeshGids = create_cell_gids_vector_and_fill_from_ascii(stfile);
    const auto sampleMeshGids  = create_cell_gids_vector_and_fill_from_ascii(safile);

    indices_.resize(sampleMeshGids.size());
    std::cout << "updater \n";
    for (std::size_t i=0; i<indices_.size(); ++i){
      const auto index = find_index<int>(stencilMeshGids, sampleMeshGids[i]);
      assert(index != std::numeric_limits<int>::max());
      indices_[i] = index;
    }
  }

  void updateSampleMeshOperandWithStencilMeshOne(vec_operand_type & a,
						 ScalarType alpha,
						 const vec_operand_type & b,
						 ScalarType beta) const
  {
    for (std::size_t i=0; i<indices_.size(); ++i){
      const std::size_t r = i*numDofsPerCell_;
      const std::size_t g = indices_[i]*numDofsPerCell_;
      for (std::size_t k=0; k<numDofsPerCell_; ++k){
	a(r+k) = alpha*a(r+k) + beta*b(g+k);
      }
    }
  }

  void updateSampleMeshOperandWithStencilMeshOne(mat_ll_operand_type & a,
						 ScalarType alpha,
						 const mat_ll_operand_type & b,
						 ScalarType beta) const
  {
    for (std::size_t j=0; j<b.cols(); ++j){
      for (std::size_t i=0; i<indices_.size(); ++i){
	const std::size_t r = i*numDofsPerCell_;
	const std::size_t g = indices_[i]*numDofsPerCell_;
	for (std::size_t k=0; k<numDofsPerCell_; ++k){
	  a(r+k,j) = alpha*a(r+k,j) + beta*b(g+k,j);
	}
      }
    }
  }
};

template<class FomSystemType, class ParserType>
void run_lspg_hyperreduced(const FomSystemType & fomSystem,
			   const ParserType & parser)
{
  namespace pode = pressio::ode;
  namespace prom = pressio::rom;
  namespace plspg = pressio::rom::lspg;

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

  const auto odeScheme = parser.odeScheme();
  HypRedUpdater<scalar_type> hrUpdater(parser);
  auto problem = plspg::create_unsteady_problem(odeScheme, trialSpace, fomSystem, hrUpdater);

  lspg_pick_solver_and_run(parser, problem.lspgStepper(), reducedState);
}

#endif
