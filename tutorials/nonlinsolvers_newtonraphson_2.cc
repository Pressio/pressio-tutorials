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

#include <array>

struct MyCustomVector{
  double & operator[](int i){ return d_[i]; }
  const double & operator[](int i)const { return d_[i]; }

  void fill(double value){ d_.fill(value); }

private:
  std::array<double, 2> d_ = {};
};

struct MyCustomMatrix{

  double & operator()(int i, int j){ return d_[2*i+j]; }
  const double & operator()(int i, int j) const { return d_[2*i+j]; }

  void fill(double value){ d_.fill(value); }

private:
  std::array<double, 4> d_ = {};
};

#include "pressio/type_traits.hpp"

namespace pressio{
template<> struct Traits<MyCustomVector>{
  using scalar_type = double;
};

template<> struct Traits<MyCustomMatrix>{
  using scalar_type = double;
};

namespace ops{
MyCustomVector clone(const MyCustomVector & src){ return src; }
MyCustomMatrix clone(const MyCustomMatrix & src){ return src; }
void set_zero(MyCustomVector & o){ o.fill(0); }
void set_zero(MyCustomMatrix & o){ o.fill(0); }

double norm2(const MyCustomVector & v){
  return std::sqrt(v[0]*v[0] + v[1]*v[1]);
}

void update(MyCustomVector & v, double a,
	    const MyCustomVector & v1, double b){
  v[0] = v[0]*a + b*v1[0];
  v[1] = v[1]*a + b*v1[1];
}

void scale(MyCustomVector & v, double factor){
  v[0] = v[0]*factor;
  v[1] = v[1]*factor;
}
}}//end namespace pressio::ops

#include "pressio/solvers_nonlinear.hpp"

struct MySystem
{
  using scalar_type = double;
  using state_type = MyCustomVector;
  using residual_type = state_type;
  using jacobian_type = MyCustomMatrix;

  residual_type createResidual() const { return {}; }
  jacobian_type createJacobian() const { return {}; }

  void residual(const state_type& x,
                residual_type& res) const
  {
    res[0] =  x[0]*x[0]*x[0] + x[1] - 1.0;
    res[1] = -x[0] + x[1]*x[1]*x[1] + 1.0;
  }

  void jacobian(const state_type& x, jacobian_type& J) const
  {
    J(0,0) = 3.0*x[0]*x[0];
    J(0,1) =  1.0;
    J(1,0) = -1.0;
    J(1,1) = 3.0*x[1]*x[1];
  }
};

struct LinearSolver{
  using matrix_type = MyCustomMatrix;

  void solve(const matrix_type & M, const MyCustomVector & rhs, MyCustomVector & x)
  {
    const auto a = M(0,0);
    const auto b = M(0,1);
    const auto c = M(1,0);
    const auto d = M(1,1);
    const auto det = a*d - b*c;

    x[0] = (d*rhs[0]  - b*rhs[1])/det;
    x[1] = (-c*rhs[0] + a*rhs[1])/det;
  }
};

int main()
{
  namespace plog   = pressio::log;
  namespace pnonls = pressio::nonlinearsolvers;

  plog::initialize(pressio::logto::terminal);
  plog::setVerbosity({plog::level::info});

  using problem_t  = MySystem;
  using state_t    = problem_t::state_type;
  problem_t sys;
  state_t y;
  y[0] = 0.001;
  y[1] = -0.1;

  LinearSolver linearSolverObj;
  auto nonLinSolver = pnonls::create_newton_raphson(sys, y, linearSolverObj);
  nonLinSolver.solve(sys, y);

  // check solution
  std::cout << "Computed solution: ["
            << y[0] << " " << y[1] << " " << "] "
            << "Expected solution: [1., 0.] "
            << std::endl;

  plog::finalize();
  return 0;
}
