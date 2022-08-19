/*
//@HEADER
// ************************************************************************
//
// tutorial4.cc
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
#include "ODE_IMPLICIT"
#include "SOLVERS_NONLINEAR"
#include "ROM_LSPG"

int main(int argc, char *argv[]){

  //
  // This tutorial describes how to apply BDF1-LSPG to Burger1d.
  // Specifically, we show a reproductive rom test: this means that
  // the rom is run for the same conditions used to generate the basis and
  // using the same time-advancing. For Burgers1d, the ROM result
  // should match the full-order model solution ''exactly''.
  // That this is not the case when linear basis do not provive
  // a good enough representation of the full order dynamics.
  //
  // Note:
  // (a) POD basis are precomputed by solving the full order model
  //	 with parameters {5., 0.02, 0.02} and 20 cells
  //	 using bdf1 for 10 steps with dt=0.01
  //
  // (b) At each ROM time step, we solve the least-square problem using normal-equations.
  //
  //
  // ------------------------------------
  // Stage (a): define some types
  // ------------------------------------
  // This is the full-order model (fom) type
  using fom_t		= pressio::apps::Burgers1dEigen;
  // get the scalar type
  using scalar_t	= typename fom_t::scalar_type;
  // get the type of the native state
  using native_state_t	= typename fom_t::state_type;

  // the lspg state type
  using lspg_state_t	= pressio::containers::Vector<native_state_t>;

  // the decoder jacobian type
  using eig_dyn_mat	= Eigen::Matrix<scalar_t, -1, -1>;
  using decoder_jac_t	= pressio::containers::MultiVector<eig_dyn_mat>;
  // we use here a linear decoder
  using decoder_t	= pressio::rom::LinearDecoder<decoder_jac_t>;


  // // ------------------------------------
  // // Stage (b): create the app object
  // // ------------------------------------
  // constexpr int32_t numCell = 20;
  // // construct the Burgers1d problem with specific parameters
  // fom_t appobj( {5.0, 0.02, 0.02}, numCell);

  // // read gold basis from file
  // std::vector<std::vector<double>> U0;
  // readMatrixFromFile("basis.txt", U0, romSize);
  // Eigen::MatrixXd U(U0.size(), U0[0].size());
  // for (size_t i=0; i<U0.size(); i++)
  //   for (size_t j=0; j<U0[i].size(); j++)
  //     U(i,j) = U0[i][j];

  // // read from file the jacobian of the decoder
  // constexpr int romSize = 11;
  // // store modes computed before from file
  // decoder_jac_t phi =
  //   pressio::rom::test::eigen::readBasis("basis.txt", romSize, numCell);
  // const int numBasis = phi.numVectors();
  // if( numBasis != romSize ) return 0;

  // // create decoder obj
  // decoder_t decoderObj(phi);


  // // for this problem, my reference state = initial state
  // auto & yRef = appobj.getInitialState();

  // // define ROM state
  // lspg_state_t yROM(romSize);
  // // initialize to zero (this has to be done)
  // yROM.putScalar(0.0);

  // // define LSPG type
  // constexpr auto ode_case  = pressio::ode::ImplicitEnum::Euler;
  // using lspg_problem_types = pressio::rom::DefaultLSPGTypeGenerator<
  //   fom_t, ode_case, decoder_t, lspg_state_t>;
  // pressio::rom::LSPGUnsteadyProblemGenerator<lspg_problem_types> lspgProblem(
  //     appobj, yRef, decoderObj, yROM, t0);

  // using lspg_stepper_t = typename lspg_problem_types::lspg_stepper_t;

  // // linear solver
  // using eig_dyn_mat  = Eigen::Matrix<scalar_t, -1, -1>;
  // using hessian_t  = pressio::containers::Matrix<eig_dyn_mat>;
  // using solver_tag   = pressio::solvers::linear::iterative::LSCG;
  // using linear_solver_t = pressio::solvers::iterative::EigenIterative<solver_tag, hessian_t>;
  // linear_solver_t linSolverObj;

  // // GaussNewton solver
  // // hessian comes up in GN solver, it is (J phi)^T (J phi)
  // // rom is solved using eigen, hessian is wrapper of eigen matrix
  // using eig_dyn_mat  = Eigen::Matrix<scalar_t, -1, -1>;
  // using gnsolver_t   = pressio::solvers::iterative::GaussNewton<
  //   lspg_stepper_t, linear_solver_t>;
  // gnsolver_t solver(lspgProblem.getStepperRef(), yROM, linSolverObj);
  // solver.setTolerance(1e-13);
  // // I know this should converge in few iters every step
  // solver.setMaxIterations(2);

  // // integrate in time
  // pressio::ode::integrateNSteps(lspgProblem.getStepperRef(), yROM, 0.0, dt, 10, solver);

  // // compute the fom corresponding to our rom final state
  // auto yFomFinal = lspgProblem.yFomReconstructor_(yROM);

  // // this is a reproducing ROM test, so the final reconstructed state
  // // has to match the FOM solution obtained with euler, same time-step, for 10 steps
  // // const auto trueY = pressio::apps::test::Burg1DtrueImpEulerN20t010;
  // const auto trueY = pressio::apps::test::Burgers1dImpGoldStatesBDF1::get(numCell, dt, 0.10);
  // for (auto i=0; i<yFomFinal.size(); i++){
  //   if (std::abs(yFomFinal[i] - trueY[i]) > 1e-10)
  //     checkStr = "FAILED";
  // }


  return 0;
}
