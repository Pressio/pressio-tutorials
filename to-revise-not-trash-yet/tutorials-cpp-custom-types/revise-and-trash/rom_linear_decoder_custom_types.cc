/*
//@HEADER
// ************************************************************************
//
// tutorial1.cc
//                          Pressio
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

#include "custom_data_types.hpp"
#include "pressio/ops.hpp"

namespace pressio{ 

template<class ScalarType> 
struct Traits<CustomVector<ScalarType>>{
  using scalar_type = ScalarType;
};

template<class ScalarType> 
struct Traits<CustomMatrix<ScalarType>>{
  using scalar_type = ScalarType;
};

namespace ops{

// z = beta*z + alpha * A * x
// where x is subscritable as x(i)
template<class x_t, class ScalarType>
void product(pressio::nontranspose,
       ScalarType alpha,
       const CustomMatrix<ScalarType> & A,
       const x_t & x,
       ScalarType beta,
       CustomVector<ScalarType> & z)
{
  // obviously not efficient, just for demonstration
  for (std::size_t i=0; i<A.extent(0); ++i)
  {
    z(i) = beta*z(i);
    for (std::size_t j=0; j<A.extent(1); ++j){
      z(i) += alpha*A(i,j)*x(j);
    }
  }
}

}}//end namespace pressio::ops

#include "pressio/rom_decoder.hpp"

int main(int argc, char *argv[])
{
  std::cout << "Running tutorial\n";
  using scalar_t	    = double;
  using fom_state_t	  = CustomVector<scalar_t>;
  using decoder_jac_t	= CustomMatrix<scalar_t>;

  // create matrix and fill with ones
  decoder_jac_t A(10, 3);
  A.fill(1.);

  // decoder
  auto decoder = pressio::rom::create_time_invariant_linear_decoder<fom_state_t>(A);

  // construct reduced state
  // typically, pressio reduced states for ROMs use Eigen or Kokkos (if enabled)
  using rom_state_t = Eigen::VectorXd;
  rom_state_t yRom(A.extent(1));
  yRom.setConstant(2.);

  // apply mapping
  fom_state_t yFom(A.extent(0));
  decoder.applyMapping(yRom, yFom);

  // check solution
  for (auto i=0; i<yFom.extent(0); ++i){
    std::cout << yFom(i) << " expected: " << 6. << "\n";
  }

  return 0;
}
