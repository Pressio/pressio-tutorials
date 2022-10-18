/*
//@HEADER
// ************************************************************************
//
// rom_shared.hpp
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

#ifndef ROM_SHARED_HPP_
#define ROM_SHARED_HPP_

#include "myio.hpp"

template<class ScalarType>
auto create_basis_and_read_from_file(const std::string & fileName,
				     int numModes)
{
  return create_colmajor_eigen_matrix_and_load_from_binary_with_extents<
    ScalarType>(fileName, numModes);
}

template<class ScalarType>
auto create_affine_shift_and_read_from_file(const std::string & fileName)
{
  const auto v = create_stdvector_and_read_from_ascii<ScalarType>(fileName);
  Eigen::Matrix<ScalarType, -1, 1> result(v.size());
  for (std::size_t i=0; i<v.size(); ++i){ result[i] = v[i]; }
  return result;
}

template<class ScalarType>
void fill_rom_state_from_ascii(const std::string & fileName,
			       Eigen::Matrix<ScalarType, -1, 1> & romState)
{
  const auto v = create_stdvector_and_read_from_ascii<ScalarType>(fileName);
  assert(v.size() == (std::size_t) romState.size());
  for (int i=0; i<v.size(); ++i){ romState[i] = v[i]; }
}

template <typename T = int32_t>
auto create_cell_gids_vector_and_fill_from_ascii(const std::string & fileName)
{
  using vector_type = Eigen::Matrix<T, -1, 1>;

  const auto v = create_stdvector_and_read_from_ascii<T>(fileName);
  vector_type result(v.size());
  for (std::size_t i=0; i<v.size(); ++i){ result[i] = v[i]; }
  return result;
}

template<class OperandType, class CellGidsVectorType>
auto create_and_reduce_on_stencil_mesh(const OperandType & operand,
				       const CellGidsVectorType & stencilMeshGids,
				       const int numDofsPerCell)
{

  const auto totStencilDofs = stencilMeshGids.size()*numDofsPerCell;
  OperandType result(totStencilDofs, operand.cols());
  for (int i=0; i<stencilMeshGids.size(); ++i)
  {
    for (int k=0; k<numDofsPerCell; ++k){
      const int row = i*numDofsPerCell + k;
      const int ind = stencilMeshGids[i]*numDofsPerCell + k;
      for (int j=0; j<operand.cols(); ++j){
	result(row, j) = operand(ind, j);
      }
    }
  }
  return result;
}

template<class ScalarType, class ParserType>
auto create_basis_on_stencil_mesh(const Eigen::Matrix<ScalarType, -1, -1, Eigen::ColMajor> & phiFull,
				  const ParserType & parser)
{
  const auto & readFile = parser.romStencilMeshGidsFile();
  const auto stencilMeshGids = create_cell_gids_vector_and_fill_from_ascii(readFile);
  return create_and_reduce_on_stencil_mesh(phiFull, stencilMeshGids,
					   parser.numDofsPerCell());
}

template<class ScalarType, class ParserType>
auto create_affine_shift_on_stencil_mesh(const ParserType & parser)
{
  const auto affineShiftFile = parser.romAffineShiftFile();
  auto affineShiftTmp = create_affine_shift_and_read_from_file<ScalarType>(affineShiftFile);
  const auto stencilMeshGids = create_cell_gids_vector_and_fill_from_ascii(parser.romStencilMeshGidsFile());
  return create_and_reduce_on_stencil_mesh(affineShiftTmp, stencilMeshGids,
					   parser.numDofsPerCell());
}

#endif
