/*
//@HEADER
// ************************************************************************
//
// swe_rusanov_flux_values_function.hpp
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

#ifndef PRESSIODEMOAPPS_SWE_FLUXES_HPP_
#define PRESSIODEMOAPPS_SWE_FLUXES_HPP_

namespace pressiodemoapps{ namespace implswe2d{

template<typename T, class normal_t, class sc_t>
void swe_rusanov_flux_three_dof(T & F,
			    const T & qL,
			    const T & qR,
			    const normal_t & n,
			    const sc_t gravity)
{

  const sc_t one  = static_cast<sc_t>(1);
  const sc_t two  = static_cast<sc_t>(2);
  const sc_t half = one/two;

  constexpr sc_t es{1.e-30};
  sc_t FL[3];
  sc_t FR[3];

  // left
  const auto hL = qL(0);
  const auto uL = qL(1)/(hL + es);
  const auto vL = qL(2)/(hL + es);
  const auto unL = uL*n[0] + vL*n[1];
  const auto pL = 0.5*gravity*hL*hL;
  FL[0] = hL*unL;
  FL[1] = hL*unL*uL + pL*n[0];
  FL[2] = hL*unL*vL + pL*n[1];

  // right
  const sc_t hR = qR(0);
  const sc_t uR = qR(1)/(hR + es);
  const sc_t vR = qR(2)/(hR + es);
  const sc_t unR = uR*n[0] + vR*n[1];
  const sc_t pR = 0.5*gravity*hR*hR;
  FR[0] = hR*unR;
  FR[1] = hR*unR*uR + pR*n[0];
  FR[2] = hR*unR*vR + pR*n[1];

  // compute flux
  const sc_t hm = 0.5*(hL + hR);
  const sc_t um = (unL*std::pow(hL,0.5) + unR*std::pow(hR,0.5) )/(std::pow(hL,0.5) + std::pow(hR,0.5)  + es);
  const sc_t smax = std::sqrt(um*um) + std::sqrt( std::pow(std::sqrt(gravity*hm),2.) ); //use this instead of abs for complex step
  F(0) = half*( FL[0] + FR[0] + smax*(qL(0) - qR(0)) );
  F(1) = half*( FL[1] + FR[1] + smax*(qL(1) - qR(1)) );
  F(2) = half*( FL[2] + FR[2] + smax*(qL(2) - qR(2)) );
}

}}
#endif
