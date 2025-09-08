/*
//@HEADER
// ************************************************************************
//
// observer.hpp
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

#ifndef PRESSIO_TUTORIALS_ENDTOEND_WORKFLOW_OBSERVER_HPP_
#define PRESSIO_TUTORIALS_ENDTOEND_WORKFLOW_OBSERVER_HPP_

class StateObserver
{
public:
  StateObserver(const std::string & f0, int freq)
    : myfile_(f0,  std::ios::out | std::ios::binary),
      sampleFreq_(freq){}

  explicit StateObserver(int freq)
    : myfile_("state_snapshots.bin",  std::ios::out | std::ios::binary),
      sampleFreq_(freq){}

  ~StateObserver(){ myfile_.close(); }

  template<typename TimeType, typename ObservableType>
  std::enable_if_t< pressio::is_vector_eigen<ObservableType>::value >
  operator()(pressio::ode::StepCount step,
	     const TimeType /*timeIn*/,
	     const ObservableType & state) const
  {
    if (step.get() % sampleFreq_ == 0){
      const std::size_t ext = state.size()*sizeof(typename ObservableType::Scalar);
      myfile_.write(reinterpret_cast<const char*>(&state(0)), ext);
    }
  }

private:
  mutable std::ofstream myfile_;
  const int sampleFreq_ = {};
};

class RhsObserver
{
public:
  RhsObserver(const std::string & f0, int freq)
    : myfile_(f0,  std::ios::out | std::ios::binary),
      sampleFreq_(freq){}

  explicit RhsObserver(int freq)
    : myfile_("rhs_snapshots.bin", std::ios::out | std::ios::binary),
      sampleFreq_(freq){}

  ~RhsObserver(){ myfile_.close(); }

  template<typename TimeType, typename ObservableType>
  std::enable_if_t< pressio::is_vector_eigen<ObservableType>::value >
  operator()(pressio::ode::StepCount step,
	     pressio::ode::IntermediateStepCount /*imStepCount*/,
	     const TimeType /*timeIn*/,
	     const ObservableType & rhs) const
  {
    if (step.get() % sampleFreq_ == 0){
      const std::size_t ext = rhs.size()*sizeof(typename ObservableType::Scalar);
      myfile_.write(reinterpret_cast<const char*>(&rhs(0)), ext);
    }
  }

private:
  mutable std::ofstream myfile_;
  const int sampleFreq_ = {};
};

#endif
