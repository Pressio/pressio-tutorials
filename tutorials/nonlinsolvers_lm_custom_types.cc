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

#include <Eigen/Core>
#include <array>
#include "pressio/ops.hpp"
#include "custom_data_types.hpp"

struct MyRosenbrockSystem
{
  using scalar_type = double;
  using state_type = Eigen::VectorXd;
  using residual_type = CustomVector<scalar_type>;
  using jacobian_type = CustomMatrix<scalar_type>;

  residual_type createResidual() const
  {
    return residual_type(6);
  }
  jacobian_type createJacobian() const
  {
    return jacobian_type(6, 4);
  }

  void residual(const state_type& x, residual_type& res) const
  {
    const auto& x1 = x(0);
    const auto& x2 = x(1);
    const auto& x3 = x(2);
    const auto& x4 = x(3);
    res(0) = 10. * (x4 - x3 * x3);
    res(1) = 10. * (x3 - x2 * x2);
    res(2) = 10. * (x2 - x1 * x1);
    res(3) = (1. - x1);
    res(4) = (1. - x2);
    res(5) = (1. - x3);
  }

  void jacobian(const state_type& x, jacobian_type& JJ) const
  {
    const auto& x1 = x(0);
    const auto& x2 = x(1);
    const auto& x3 = x(2);
    JJ.fill(0.);
    JJ(0, 2) = -20. * x3;
    JJ(0, 3) = 10.;
    JJ(1, 1) = -20. * x2;
    JJ(1, 2) = 10.;
    JJ(2, 0) = -20. * x1;
    JJ(2, 1) = 10.;
    JJ(3, 0) = -1.;
    JJ(4, 1) = -1.;
    JJ(5, 2) = -1.;
  }
};

namespace pressio {
template <class T>
struct Traits<CustomVector<T>>
{
  using scalar_type = T;
};

template <class T>
struct Traits<CustomMatrix<T>>
{
  using scalar_type = T;
};

namespace ops {
template <class T>
CustomVector<T> clone(const CustomVector<T>& src)
{
  return src;
}

template <class T>
CustomMatrix<T> clone(const CustomMatrix<T>& src)
{
  return src;
}

template <class T>
void set_zero(CustomVector<T>& o)
{
  o.fill(0);
}

template <class T>
void set_zero(CustomMatrix<T>& o)
{
  o.fill(0);
}

template <class T>
T norm2(const CustomVector<T>& v)
{
  T norm{0};
  for (std::size_t i = 0; i < v.extent(0); ++i) {
    norm += v(i) * v(i);
  }
  return std::sqrt(norm);
}

template <class HessianType, class T>
void product(
  pressio::transpose,
  pressio::nontranspose,
  T alpha,
  const CustomMatrix<T>& A,
  const T beta,
  HessianType& H)
{
  for (std::size_t i = 0; i < A.extent(1); ++i) {
    for (std::size_t j = 0; j < A.extent(1); ++j) {
      H(i, j) *= beta;
      for (std::size_t k = 0; k < A.extent(0); ++k) {
        H(i, j) += alpha * A(k, i) * A(k, j);
      }
    }
  }
}

template <class HessianType, class T>
HessianType product(
  pressio::transpose,
  pressio::nontranspose,
  T alpha,
  /* const */ CustomMatrix<T>& A)
{
  HessianType H(A.extent(1), A.extent(1));
  product(pressio::transpose(), pressio::nontranspose(), alpha, A, (T)0, H);
  return H;
}

template <class GradientType, class T>
void product(
  pressio::transpose,
  const T alpha,
  const CustomMatrix<T>& A,
  const CustomVector<T>& b,
  const T beta,
  GradientType& g)
{
  for (std::size_t i = 0; i < g.rows(); ++i) {
    g(i) *= beta;
    for (std::size_t k = 0; k < A.extent(0); ++k) {
      g(i) += alpha * A(k, i) * b(k);
    }
  }
}

template <class T>
void update(CustomVector<T>& v, T a, const CustomVector<T>& v1, T b)
{
  for (std::size_t i = 0; i < v.extent(0); ++i) {
    v(i) = v(i) * a + b * v1(i);
  }
}

template <class T>
void scale(CustomVector<T>& v, T factor)
{
  for (std::size_t i = 0; i < v.extent(0); ++i) {
    v(i) = v(i) * factor;
  }
}
}  // namespace ops
}  // namespace pressio

#include "pressio/solvers_linear.hpp"
#include "pressio/solvers_nonlinear.hpp"

int main()
{
  namespace plog = pressio::log;
  namespace pls = pressio::linearsolvers;
  namespace pnonls = pressio::nonlinearsolvers;
  plog::initialize(pressio::logto::terminal);
  plog::setVerbosity({plog::level::info});

  using problem_t = MyRosenbrockSystem;
  problem_t problem;

  using state_t = Eigen::VectorXd;
  state_t x(4);
  x[0] = -0.05;
  x[1] = 1.1;
  x[2] = 1.2;
  x[3] = 1.5;

  using hessian_t = Eigen::MatrixXd;
  using lin_tag = pls::direct::HouseholderQR;
  using lin_solver_t = pls::Solver<lin_tag, hessian_t>;
  lin_solver_t linSolver;

  auto gnSolver = pnonls::create_levenberg_marquardt(problem, x, linSolver);
  gnSolver.setTolerance(1e-5);
  gnSolver.solve(problem, x);

  // check solution
  std::cout
    << "Computed solution: \n "
    << "[" << x(0) << " " << x(1) << " " << x(2) << " " << x(3) << "]\n"
    << "Expected solution: \n "
    << "[1.0000000156741, 0.99999999912477, 0.99999999651993, 0.99999998889888]"
    << std::endl;

  plog::finalize();
  return 0;
}