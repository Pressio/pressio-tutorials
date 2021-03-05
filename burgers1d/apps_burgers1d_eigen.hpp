/*
//@HEADER
// ************************************************************************
//
// burgers1d_eigen.hpp
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

#ifndef PRESSIO_TUTORIALS_BURGERS1D_EIGEN_HPP_
#define PRESSIO_TUTORIALS_BURGERS1D_EIGEN_HPP_

#include "Eigen/Dense"
#include "Eigen/SparseCore"
#include <iostream>

namespace pressio{ namespace apps{

class Burgers1dEigen{
  using eigVec = Eigen::VectorXd;
  using mv_t   = Eigen::MatrixXd;
  using int_t  = int32_t;

public:
  using scalar_type	= double;
  using state_type	= eigVec;
  using velocity_type	= eigVec;
  using jacobian_type	= Eigen::SparseMatrix<scalar_type, Eigen::RowMajor, int>;

  typedef Eigen::Triplet<scalar_type> Tr;

public:
  explicit Burgers1dEigen(eigVec params, int_t Ncell=1000)
    : mu_(params), Ncell_(Ncell){
    this->setup();
  }

  Burgers1dEigen() = delete;
  ~Burgers1dEigen() = default;

public:
  void setup(){
    dx_ = (xR_ - xL_)/static_cast<scalar_type>(Ncell_);
    dxInv_ = 1.0/dx_;

    // grid
    xGrid_.resize(Ncell_);
    for (int_t i=0; i<Ncell_; ++i)
      xGrid_(i) = dx_*i + dx_*0.5;

    // init condition
    U_.resize(Ncell_);
    for (int_t i=0; i<Ncell_; ++i)
      U_(i) = 1.0;
    U0_ = U_;
  };

  state_type const & getInitialState() const {
    return U0_;
  };

  void velocity(const state_type & u,
		const scalar_type /* t */,
    velocity_type & rhs) const;

  velocity_type velocity(const state_type & u,
			 const scalar_type t) const{
    velocity_type RR(Ncell_);
    this->velocity(u, t, RR);
    return RR;
  }

  // computes: A = Jac B where B is a Eigen::MatrixXd
  void applyJacobian(const state_type & y,
		     const mv_t & B,
		     scalar_type t,
         mv_t & A) const{
    auto JJ = jacobian(y, t);
    A = JJ * B;
  }

  // computes: A = Jac B where B is a Eigen::MatrixXd
  mv_t applyJacobian(const state_type & y,
		     const mv_t & B,
		     scalar_type t) const{
    mv_t A( y.size(), B.cols() );
    applyJacobian(y, B, t, A);
    return A;
  }

  void jacobian(const state_type & u,
		const scalar_type /*t*/,
    jacobian_type & jac) const;

  jacobian_type jacobian(const state_type & u,
			 const scalar_type t) const{

    jacobian_type JJ(u.size(), u.size());
    this->jacobian(u, t, JJ);
    return JJ;
  }

private:
  eigVec mu_; // parameters
  const scalar_type xL_ = 0.0; //left side of domain
  const scalar_type xR_ = 100.0; // right side of domain
  int_t Ncell_; // # of cells
  scalar_type dx_; // cell size
  scalar_type dxInv_; // inv of cell size
  eigVec xGrid_; // mesh points coordinates
  mutable std::vector<Tr> tripletList;
  mutable state_type U_; // state vector
  mutable state_type U0_; // initial state vector

};//end class

}} //namespace pressio::apps
#endif