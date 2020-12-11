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

#include "pressio_containers.hpp"
#include <Eigen/Core>

int main(int argc, char *argv[])
{
  std::cout << "Running tutorial 1\n";

  /*
    Pressio makes use of container wrappers to wrap arbitrary data types.
    These wrappers are thin layers.

    Pressio has predefined knowledge about data structures
    from specific libraries, e.g. Trilinos, Eigen, Kokkos.
    Over time, this support will be extended.
    If your application uses vector/matrix/multivector classes of one
    of these libraries, you can easily use pressio as follows.

    Suppose that you have an application that is based on Eigen.
    Therefore, a (dynamic) vector object "a" in your application looks like:

    Eigen::VectorXd a;

    Now, suppose now that you need to use some functionality in pressio.
    To do so, you need to wrap your object with a pressio container.
    e.g. ::pressio::containers::Vector<Eigen::VectorXd> aW(a);

    If the type you are wrapping is one of the already supported by pressio,
    then you have seamless access to all pressio functionalities, since pressio
    behind the scenes knows how to do algebra with these objects and in fact
    leverages the native algebra functionalities of the target library.
    If the type you are wrapping is NOT already known to pressio,
    then you can still wrap it, but in order to instantiate and use pressio
    functionalities you need to provide functionalities to tell pressio
    how to do operations with your data types.
    See tutorials{3,5} for examples showing using arbitrary types.
  */

  // this is dynamic double array in Eigen
  Eigen::VectorXd a(5);
  a.setConstant(2.2);

  // the corresponding pressio wrapper can be created as follows
  // Note that here pressio makes a deep copy of the object.
  // This is because the target ROM algorithms are all implemented
  // such that pressio owns the data and queries the full-order application
  // to computed things.
  ::pressio::containers::Vector<Eigen::VectorXd> aW(a);

  // we can verify that a deep copy is made
  for (auto i=0; i<aW.extent(0); ++i)
    std::cout << "aW(i) = " << aW(i)
	      << " expected = " << 2.2
	      << std::endl;

  // note that above we used the "extent" method
  // extent(i) works for all pressio container wrappers
  // - extent(i) for 1d objects,
  // - extent(i,j) for 2d objects

  return 0;
}
