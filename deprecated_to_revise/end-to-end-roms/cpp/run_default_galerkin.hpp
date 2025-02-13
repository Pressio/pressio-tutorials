/*
//@HEADER
// ************************************************************************
//
// run_default_galerkin.hpp
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

#ifndef RUN_DEFAULT_GALERKIN_HPP_
#define RUN_DEFAULT_GALERKIN_HPP_

#include "pressio/rom_subspaces.hpp"
#include "pressio/rom_galerkin_unsteady.hpp"
#include "observer.hpp"
#include "rom_shared.hpp"

template<class FomSystemType, class ParserType>
void run_galerkin_default(const FomSystemType & fomSystem,
			  const ParserType & parser)
{
  namespace pode = pressio::ode;
  namespace prom = pressio::rom;
  namespace pgal = pressio::rom::galerkin;

  using scalar_type = typename FomSystemType::scalar_type;
  using reduced_state_type = Eigen::Matrix<scalar_type, Eigen::Dynamic, 1>;

  auto basis = create_basis_and_read_from_file<
         scalar_type>(parser.romFullMeshPodBasisFile(), parser.romModeCount());
  auto affineShift = create_affine_shift_and_read_from_file<
         scalar_type>(parser.romAffineShiftFile());
  const auto trialSpace  = prom::create_trial_column_subspace<
         reduced_state_type>(std::move(basis), std::move(affineShift), parser.romIsAffine());

  auto reducedState = trialSpace.createReducedState();
  fill_rom_state_from_ascii(parser.romInitialStateFile(), reducedState);

  StateObserver observer(parser.stateSamplingFreq());
  const auto odeScheme = parser.odeScheme();
  if (pressio::ode::is_explicit_scheme(odeScheme))
  {
  auto problem = pgal::create_unsteady_explicit_problem(odeScheme, trialSpace, fomSystem);

  typename FomSystemType::time_type startTime{0};
  const auto numSteps = pode::StepCount{parser.numSteps()};
  pode::advance_n_steps(problem, reducedState, startTime,
			parser.timeStepSize(), numSteps, observer);
  }
  else{
    throw std::runtime_error("Default implicit galerkin not impl yet");
  }
}

#endif
