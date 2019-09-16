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

#include "CONTAINERS_ALL"
#include "ODE_ALL"
#include "SOLVERS_NONLINEAR"

int main(int argc, char *argv[]){

  // // parse input file
  // InputParser parser;
  // int err = parser.parse(argc, argv);
  // if (err == 1) return 1;

  // // get the problem name and stepper
  // const auto problemName    = parser.problemName_;
  // const auto odeStepperName = parser.odeStepperName_;

  // if ( problemName.compare("ms") == 0 ){
  //   // types
  //   using pt = MSTypes;

  //   // functors to use
  //   typename pt::adv_fnct_t advFunctor;
  //   typename pt::src_fnct_t srcFunctor(parser.D_);

  //   if (odeStepperName.compare("RungeKutta4") == 0 ){
  //     runRungeKutta4<pt>(parser, srcFunctor, advFunctor);
  //   }
  //   if (odeStepperName.compare("bdf1") == 0 ){
  //     runBDF1<pt>(parser, srcFunctor, advFunctor);
  //   }
  // }
  // if ( problemName.compare("chemABC") == 0 ){
  //   // types
  //   using pt = ChemTypes;

  //   // functors to use
  //   typename pt::adv_fnct_t advFunctor;
  //   typename pt::src_fnct_t srcFunctor(parser.K_);

  //   if (odeStepperName.compare("RungeKutta4") == 0){
  //     runRungeKutta4<pt>(parser, srcFunctor, advFunctor);
  //   }
  //   if (odeStepperName.compare("bdf1") == 0 ){
  //     runBDF1<pt>(parser, srcFunctor, advFunctor);
  //   }
  // }
  // else{
  //   std::cout << "Invalid problemName in input file " << std::endl;
  // }

  return 0;
}
