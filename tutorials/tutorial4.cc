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

#include "pressio_rom.hpp"

int main(int argc, char *argv[])
{
  /*
   --scope--:
   creating a linear decoder (or mapping)

   --why and what--:
   one of the main assumptions of projection-based ROMs
   is to approximate a FOM state, yFom, as:

     yFom = g(yRom)

   where yRom is the reduced state, also called generalized coordinates,
   and g() is a mapping between the two.
   If g() is linear, then we can write:

     yFom = phi * yRom

   where phi is a matrix (for the time being, assume it constant).
   A linear decoder in pressio represents exactly this linear mapping.
   The Jacobian of the mapping is: d(yFom)/d(yRom) = phi.

   --details--:
   Here we demonstate how to create a linear decoder object using Eigen types.
   For other types already known to pressio (e.g. Trilinos), it would work similarly.
   If you work with an arbitrary type currrently unknown to pressio, see tutorial5.cc
  */

  // *** define some types ***
  // here we assume your FOM application uses an Eigen vector for the state
  // and an Eigen matrix as the type for the Jacobian
  using native_fom_state_t = Eigen::VectorXd;
  using native_phi_t	   = Eigen::MatrixXd;
  // the wrapped types
  using fom_state_t	= pressio::containers::Vector<native_fom_state_t>;
  using decoder_jac_t	= pressio::containers::DenseMatrix<native_phi_t>;

  // *** fill phi ***
  // for simplicity, let's create a native phi with 10 rows and 3 columns.
  native_phi_t phiNative(6, 2);

  // *** construct decoder  ***
  using decoder_t = pressio::rom::LinearDecoder<decoder_jac_t, fom_state_t>;
  // here we demonstrate moving phiNative to avoid a deep copy, but one can
  // also do `decoderObj(phi)` which implies a copy of the matrix.
  // Obviously, if the matrix is large avoiding a copy is useful.
  decoder_t decoder(std::move(phi));

  // *** construct reduced state  ***
  // typically, pressio reduced states for ROMs use Eigen or Kokkos (if enabled)
  using rom_state_t = pressio::containers::Vector<Eigen::VectorXd>;
  rom_state_t yRom(2);
  // set yRom = 2.
  pressio::ops::fill(yRom, 2.);

  // *** apply mapping ***
  fom_state_t yFom(6);
  decoder.applyMapping(yRom, yFom);

  // *** check solution ***
  // yFom should be = [4. 4. .... 4.]
  for (auto i=0; i<yFom.extent(0); ++i)
    std::cout << yFom(i) << "\n";

  return 0;
}
