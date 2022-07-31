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
#include <vector>

template<class scalar_t>
struct MyCustomVector
{
  using value_type = scalar_t;
  using size_type = std::size_t;

  MyCustomVector(std::size_t sizeIn)
    : m_data(sizeIn){}

  std::size_t size() const{
    return m_data.size();
  }

  scalar_t & operator[](std::size_t index){
    return m_data[index];
  }

  const scalar_t & operator[](std::size_t index) const{
    return m_data[index];
  }

private:
  std::vector<scalar_t> m_data;
};

template<class scalar_t>
struct MyCustomMatrix
{
  using value_type = scalar_t;
  using size_type = std::size_t;

  MyCustomMatrix(std::size_t nRows, std::size_t nCols)
    : m_data(nRows)
  {
    for (auto & iRow : m_data){ iRow.resize(3, 0.); }
  }

  void setConstant(scalar_t value){
    for (auto & iRow : m_data){
      for (auto & iCol : iRow){
	iCol = value;
      }
    }
  }

  std::size_t extent(int dim) const{
    return (dim==0) ? m_data.size() : m_data.front().size();
  }

  scalar_t & operator()(std::size_t row, std::size_t col){
    return m_data[row][col];
  }

  const scalar_t & operator()(std::size_t row, std::size_t col) const{
    return m_data[row][col];
  }

private:
  std::vector<std::vector<scalar_t>> m_data;
};

template <typename scalar_t>
struct MyOps
{
  // compue: z = beta*z + alpha * A * x
  // - x is an array-like object with the () operator to reference an element
  template< typename x_t>
  void product(pressio::nontranspose,
               scalar_t alpha,
               const MyCustomMatrix<scalar_t> & A,
               const x_t & x,
               scalar_t beta,
               MyCustomVector<scalar_t> & z) const
  {
    // obviously not efficient, just for demonstration
    for (std::size_t i=0; i<A.extent(0); ++i)
    {
      z[i] = beta*z[i];
      for (std::size_t j=0; j<A.extent(1); ++j){
        z[i] += alpha*A(i,j)*x(j);
      }
    }
  }
};

int main(int argc, char *argv[])
{
  std::cout << "Running tutorial\n";

  // the FOM types
  using scalar_t           = double;
  using native_fom_state_t = MyCustomVector<scalar_t>;
  using native_phi_t       = MyCustomMatrix<scalar_t>;
  // wrapper types aliases
  using fom_state_t        = pressio::containers::Vector<native_fom_state_t>;
  using decoder_jac_t      = pressio::containers::DenseMatrix<native_phi_t>;

  // create a native phi and fill with ones
  native_phi_t phiNative(6, 3);
  phiNative.setConstant(1.);

  // ops and decoder
  using ops_t     = MyOps<scalar_t>;
  using decoder_t = pressio::rom::LinearDecoder<decoder_jac_t, fom_state_t, ops_t>;
  ops_t ops;
  decoder_t decoder(phiNative, ops);

  // construct reduced state, fill with 2s
  using rom_state_t = pressio::containers::Vector<Eigen::VectorXd>;
  rom_state_t yRom(3);
  pressio::ops::fill(yRom, 2.);

  // apply mapping
  fom_state_t yFom(6);
  decoder.applyMapping(yRom, yFom);

  // check solution
  for (auto i=0; i<6; ++i)
    std::cout << "i= " << i
	      << ", yFom(i) = "  << yFom(i)
	      << ", expected = " << 6.
	      << "\n";

  return 0;
}
