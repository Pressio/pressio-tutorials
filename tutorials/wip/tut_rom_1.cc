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
#include <Eigen/Core>

int main(int argc, char *argv[])
{
  std::cout << "Running tutorial\n";

  using native_fom_state_t = Eigen::VectorXd;
  using native_phi_t	   = Eigen::MatrixXd;
  // wrap native types with pressio containers
  using fom_state_t = pressio::containers::Vector<native_fom_state_t>;
  using phi_t       = pressio::containers::DenseMatrix<native_phi_t>;

  // create native phi, and for simplicity fill with ones
  native_phi_t phiNative(6, 3);
  phiNative.setConstant(1.);

  // construct linear decoder
  using decoder_t = pressio::rom::LinearDecoder<phi_t, fom_state_t>;
  decoder_t decoder(std::move(phiNative));

  // construct the reduced state, fill with 2s
  using rom_state_t = pressio::containers::Vector<Eigen::VectorXd>;
  rom_state_t yRom(3);
  pressio::ops::fill(yRom, 2.);

  // apply mapping: computes yFom = phi yRom
  fom_state_t yFom(6);
  decoder.applyMapping(yRom, yFom);

  // check
  for (auto i=0; i<yFom.extent(0); ++i)
    std::cout << "i= " << i
	      << ", yFom(i) = "  << yFom(i)
	      << ", expected = " << 6.
	      << "\n";

  return 0;
}
