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

#include "pressio/solvers_linear.hpp"
#include "pressio/solvers_nonlinear.hpp"
#include <Eigen/Core>

struct MySystem
{
  using scalar_type = double;
  using state_type = Eigen::VectorXd;
  using residual_type = state_type;
  using jacobian_type = Eigen::SparseMatrix<scalar_type>;

  residual_type createResidual() const {
    return residual_type(2);
  }

  jacobian_type createJacobian() const {
    return jacobian_type(2, 2);
  }

  void residual(const state_type& x,
                residual_type& res) const
  {
    res(0) =  x(0)*x(0)*x(0) + x(1) - 1.0;
    res(1) = -x(0) + x(1)*x(1)*x(1) + 1.0;
  }

  void jacobian(const state_type& x, jacobian_type& jac) const {
    jac.coeffRef(0, 0) = 3.0*x(0)*x(0);
    jac.coeffRef(0, 1) =  1.0;
    jac.coeffRef(1, 0) = -1.0;
    jac.coeffRef(1, 1) = 3.0*x(1)*x(1);
  }
};

int main()
{
  namespace plog   = pressio::log;
  namespace pls    = pressio::linearsolvers;
  namespace pnonls = pressio::nonlinearsolvers;

  plog::initialize(pressio::logto::terminal);
  plog::setVerbosity({plog::level::info});

  using problem_t  = MySystem;
  problem_t problemObj;

  using state_t    = problem_t::state_type;
  state_t y(2);
  y(0) = 0.001; y(1) = -0.1;

  // linear solver
  using jacobian_t = problem_t::jacobian_type;
  using lin_solver_t = pls::Solver<pls::iterative::LSCG, jacobian_t>;
  lin_solver_t linearSolverObj;
  // nonlinear solvers
  auto nonLinSolver = pnonls::create_newton_raphson(problemObj, y, linearSolverObj);
  nonLinSolver.solve(problemObj, y);

  // check solution
  std::cout << "Computed solution: ["
            << y(0) << " " << y(1) << " " << "] "
            << "Expected solution: [1., 0.] "
            << std::endl;

  plog::finalize();
  return 0;
}
