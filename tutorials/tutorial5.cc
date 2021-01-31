/*
//@HEADER
// ************************************************************************
//
// tutorial5.cc
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

#include "pressio_rom.hpp"

template <typename scalar_t>
struct MyOps
{
  // z = beta*z + alpha * A * x
  // where x is something that is subscritable as x(i)
  template< typename x_t>
  void product(pressio::nontranspose,
	       scalar_t alpha,
	       const std::vector<std::vector<scalar_t>> & A,
	       const x_t & x,
	       scalar_t beta,
	       std::vector<scalar_t> & z) const
  {
    // obviously not efficient, just for demonstration
    for (std::size_t i=0; i<A.size(); ++i)
    {
      z[i] += beta*z[i];
      for (std::size_t j=0; j<A[i].size(); ++j){
       z[i] += alpha*A[i][j]*x(j);
      }
    }
  }
};

int main(int argc, char *argv[])
{
  std::cout << "Running tutorial 5\n";

  /*
   creating a linear decoder (or mapping) object representing

     yFom = phi * yRom

   where yFom is the FOM state, yRom is the reduced state,
   and phi is a matrix (for the time being, assume it constant).
  */

  // *** define types ***
  // the FOM application uses std::vector
  using scalar_t	   = double;
  using native_fom_state_t = std::vector<scalar_t>;
  using native_phi_t	   = std::vector<std::vector<scalar_t>>;

  // the wrapped types
  // what happens in pressio when we wrap native_fom_state_t and native_phi_t?
  // Practically, both pressio::containers::Vector<native_fom_state_t>
  // and pressio::containers::DenseMatrix<native_fom_state_t> are "labeled"
  // as "arbitrary" wrapper types for which pressio does not know how to operate on.
  using fom_state_t   = pressio::containers::Vector<native_fom_state_t>;
  using decoder_jac_t = pressio::containers::DenseMatrix<native_phi_t>;

  // *** fill phi ***
  // create a native phi and fill with ones
  native_phi_t phiNative(6);
  for (auto & iRow : phiNative){
    iRow.resize(3, 1.);
  }

  // *** construct decoder  ***
  // Since the FOM types are "arbitrary", we need to declare the
  // decoder type by passing as a template argument the type of
  // class (see MyOps struct in this file) containing kernels for operating on the data.
  // And we need to pass an object to the constructor of the decoder
  // an object that knows how to compute the the operations needed.
  using ops_t     = MyOps<scalar_t>;
  using decoder_t = pressio::rom::LinearDecoder<decoder_jac_t, fom_state_t, ops_t>;
  ops_t ops;
  decoder_t decoder(phiNative, ops);

  // *** construct reduced state  ***
  // use Eigen for the ROM state
  using rom_state_t = pressio::containers::Vector<Eigen::VectorXd>;
  rom_state_t yRom(3);
  // fill with 2.
  pressio::ops::fill(yRom, 2.);

  // *** apply mapping ***
  fom_state_t yFom(6);
  decoder.applyMapping(yRom, yFom);

  // *** check solution ***
  // yFom should be = [4. 4. .... 4.]
  for (auto i=0; i<6; ++i)
    std::cout << "i= " << i
	      << ", yFom(i) = "  << yFom(i)
	      << ", expected = " << 4.
	      << "\n";

  return 0;
}
