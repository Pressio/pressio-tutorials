/*
//@HEADER
// ************************************************************************
//
// main.cc
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

#include "pressio_cmake_config.h"
#include "pressiodemoapps/swe2d.hpp"
#include "pressio/ode_steppers.hpp"
#include "pressio/ode_advancers.hpp"

#include "parsers.hpp"
#include "myio.hpp"
#include "run_fom_explicit.hpp"
#include "run_fom_implicit.hpp"
#include "run_default_galerkin.hpp"
#include "run_hyperreduced_galerkin.hpp"
#include "run_default_lspg.hpp"
#include "run_hyperreduced_lspg.hpp"
#include <chrono>

template<class AppObjType, class ParserType>
void dispatch(const AppObjType & fomSystem,
	      const ParserType & parser)
{

  if (parser.doingFom()){
    if (pressio::ode::is_explicit_scheme(parser.odeScheme())){
      run_fom_explicit(fomSystem, parser);
    }
    else{
      run_fom_implicit(fomSystem, parser);
    }
  }

  else if (parser.romAlgorithm()=="defaultgalerkin"){
    run_galerkin_default(fomSystem, parser);
  }

  else if (parser.romAlgorithm()=="defaultlspg"){
    run_lspg_default(fomSystem, parser);
  }

  else if (   parser.romAlgorithm()=="hyperreducedgalerkin")
  {
    run_galerkin_hyperreduced(fomSystem, parser);
  }

  else if (parser.romAlgorithm()=="hyperreducedlspg"){
    run_lspg_hyperreduced(fomSystem, parser);
  }

  else{
    throw std::runtime_error("dispatch: invalid branching");
  }
}

std::string check_and_get_inputfile(int argc, char *argv[])
{
  if (argc != 2){
    throw std::runtime_error("Call as: ./exe <path-to-inputfile>");
  }
  const std::string inputFile = argv[1];
  std::cout << "Input file: " << inputFile << "\n";
  assert( file_exists(inputFile) );
  return inputFile;
}

int main(int argc, char *argv[])
{
  PRESSIOLOG_INITIALIZE(pressiolog::LogLevel::info);


  // local scope because it is helps
  {
    const auto inputFile = check_and_get_inputfile(argc, argv);
    auto node = YAML::LoadFile(inputFile);

    const auto problemNode = node["problem"];
    if (!problemNode){ throw std::runtime_error("Missing problem in yaml input!"); }
    const auto problemName = problemNode.as<std::string>();
    std::cout << "Running: " << problemName << std::endl;

    // start timer
    auto t1 = std::chrono::high_resolution_clock::now();

    namespace pda  = pressiodemoapps;
    if (problemName == "2d_swe")
    {
      using scalar_t = double;
      ParserTwoDimShallowWater<scalar_t> parser(node);
      const auto meshObj = pda::load_cellcentered_uniform_mesh_eigen<scalar_t>(parser.meshDir());
      const auto invScheme = string_to_inviscid_flux_scheme(parser.inviscidFluxReconstruction());
      const auto gravity   = parser.gravity();
      const auto coriolis  = parser.coriolis();
      const auto pulseMag  = parser.pulseMagnitude();
      auto fomSystem = pda::create_slip_wall_swe_2d_problem_eigen(meshObj, invScheme,
								  gravity, coriolis, pulseMag);
      dispatch(fomSystem, parser);
    }

    auto t2 = std::chrono::high_resolution_clock::now();
    std::chrono::duration< double > fs = t2 - t1;
    std::cout << "elapsed " << fs.count() << std::endl;
  }

  PRESSIOLOG_FINALIZE();
  return 0;
}
