/*
//@HEADER
// ************************************************************************
//
// myio.hpp
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

#ifndef PRESSIO_TUTORIALS_MYIO_HPP_
#define PRESSIO_TUTORIALS_MYIO_HPP_

#include "pressio/type_traits.hpp"
#include "Eigen/Dense"
#include <chrono>

bool file_exists(const std::string & fileIn){
  std::ifstream infile(fileIn);
  return (infile.good() != 0);
}

template<class ScalarType>
auto create_colmajor_eigen_matrix_and_load_from_binary_with_extents(const std::string & fileName,
								    int numColsToRead)
{
  using matrix_type = Eigen::Matrix<ScalarType, -1, -1, Eigen::ColMajor>;

  if (!file_exists(fileName)){
    throw std::runtime_error("Cannot find file: " + fileName);
  }

  matrix_type M;
  using sc_t  = typename matrix_type::Scalar;
  std::ifstream fin(fileName, std::ios::in | std::ios::binary);
  fin.exceptions(std::ifstream::failbit | std::ifstream::badbit);

  std::size_t rows={};
  std::size_t cols={};
  fin.read((char*) (&rows),sizeof(std::size_t));
  fin.read((char*) (&cols),sizeof(std::size_t));
  M.resize(rows, numColsToRead);

  // we can do this read because file is binary and we are working
  // with colmajor matrix
  const auto nBytes = rows*numColsToRead*sizeof(sc_t);
  fin.read( (char *) M.data(), nBytes );

  if (!fin){
    std::cout << std::strerror(errno) << std::endl;
    throw std::runtime_error("ERROR READING binary file");
  }
  else{
    std::cout << fin.gcount() << " bytes read\n";
  }
  fin.close();
  return M;
}

template<class ScalarType>
auto create_stdvector_and_read_from_ascii(const std::string & fileName)
{

  if (!file_exists(fileName)){
    throw std::runtime_error("Cannot find file: " + fileName);
  }
  std::ifstream source;
  source.open(fileName, std::ios_base::in);
  std::string line, value;
  std::vector<ScalarType> v;
  while (std::getline(source, line) ){
    std::istringstream in(line);
    in >> value;
    if (std::is_floating_point<ScalarType>::value){
      v.push_back(std::atof(value.c_str()));
    }
    else{
      v.push_back(std::atoi(value.c_str()));
    }
  }
  source.close();

  return v;
}

template<class T>
std::enable_if_t<pressio::is_vector_eigen<T>::value>
write_vector_to_binary(const T & v,
		       const std::string & fileName)
{
  std::ofstream myfile(fileName);
  const std::size_t ext = v.size()*sizeof(typename T::Scalar);
  myfile.write(reinterpret_cast<const char*>(&v(0)), ext);
  myfile.close();
}

#endif
