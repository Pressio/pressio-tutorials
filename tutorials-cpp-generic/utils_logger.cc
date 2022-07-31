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

// for the tutorials, the
// PRESSIO_LOG_ACTIVE_MIN_LEVEL is set to 2 (=info)

// this is needed to include the logger code
#include <pressio/utils.hpp>

int main(int argc, char *argv[])
{
  namespace plog = pressio::log;
  plog::initialize(pressio::logto::terminal);

  /* recall that:
  PRESSIO_LOG_LEVEL_TRACE     0
  PRESSIO_LOG_LEVEL_DEBUG     1
  PRESSIO_LOG_LEVEL_INFO      2
  PRESSIO_LOG_LEVEL_WARN      3
  PRESSIO_LOG_LEVEL_ERROR     4
  PRESSIO_LOG_LEVEL_CRITICAL  5
  PRESSIO_LOG_LEVEL_OFF       6
  */

  PRESSIOLOG_INFO( "prints because info is == PRESSIO_LOG_ACTIVE_MIN_LEVEL");
  PRESSIOLOG_DEBUG("this should NOT print because debug < PRESSIO_LOG_ACTIVE_MIN_LEVEL");
  PRESSIOLOG_WARN( "this should print print because warn > PRESSIO_LOG_ACTIVE_MIN_LEVEL");
  PRESSIOLOG_ERROR("this should print print because error > PRESSIO_LOG_ACTIVE_MIN_LEVEL");

  double a = 1.1;
  PRESSIOLOG_INFO( "logger uses fmtlibrary so you can do a= {:.6e}", a);

  plog::setVerbosity({plog::level::warn});
  PRESSIOLOG_INFO( "this should NOT print because we reset min level");

  plog::finalize();
  return 0;
}
