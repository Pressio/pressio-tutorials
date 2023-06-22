/*
//@HEADER
// ************************************************************************
//
// mesh_read_coords.hpp
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

#ifndef PRESSIODEMOAPPS_MESH_READ_COORDS_HPP_
#define PRESSIODEMOAPPS_MESH_READ_COORDS_HPP_

namespace pressiodemoapps{ namespace impl{

template<class x_t, class y_t, class z_t>
void read_mesh_coordinates(const std::string & meshDir,
			   int dim,
			   x_t& x,
			   y_t& y,
			   z_t& z)
{
  const auto inFile   = meshDir+"/coordinates.dat";
  std::ifstream foundFile(inFile);
  if(!foundFile){
    std::cout << "file not found " << inFile << "\n";
    exit(EXIT_FAILURE);
  }

  std::ifstream source(inFile, std::ios_base::in);
  std::string line;
  while (std::getline(source, line) )
    {
      std::istringstream ss(line);
      std::string colVal;
      ss >> colVal;
      auto thisGid = std::stoi(colVal);
      ss >> colVal; x(thisGid) = std::stod(colVal);
      ss >> colVal; y(thisGid) = std::stod(colVal);

      if (dim<=2){
	z(thisGid) = 0.0;
      }
      else{
	ss >> colVal; z(thisGid) = std::stod(colVal);
      }
    }
  source.close();
}

}}
#endif