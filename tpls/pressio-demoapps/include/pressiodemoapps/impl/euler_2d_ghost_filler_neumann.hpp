/*
//@HEADER
// ************************************************************************
//
// euler_2d_ghost_filler_neumann.hpp
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

#ifndef PRESSIODEMOAPPS_GHOST_FILLER_EE2D_NEUMANN_HPP_
#define PRESSIODEMOAPPS_GHOST_FILLER_EE2D_NEUMANN_HPP_

namespace pressiodemoapps{
namespace impleuler2d{

template<class state_t, class mesh_t, class ghost_t>
class Ghost2dNeumannFiller
{

public:
  Ghost2dNeumannFiller() = delete;
  Ghost2dNeumannFiller(const int stencilSize,
		       int numDofPerCell,
		       const state_t & stateIn,
		       const mesh_t & meshIn,
		       ghost_t & ghostLeft,
		       ghost_t & ghostFront,
		       ghost_t & ghostRight,
		       ghost_t & ghostBack)
    : m_stencilSize(stencilSize),
      m_numDofPerCell(numDofPerCell),
      m_state(stateIn),
      m_meshObj(meshIn),
      m_ghostLeft(ghostLeft),
      m_ghostFront(ghostFront),
      m_ghostRight(ghostRight),
      m_ghostBack(ghostBack)
  {}

  template<class index_t>
  void operator()(index_t smPt, int gRow)
  {
    if (m_numDofPerCell == 4){
      fourDofImpl(smPt, gRow);
    }
  }

private:

  template<class index_t>
  void fourDofImpl(index_t smPt, int gRow)
  {
    const auto & graph = m_meshObj.graph();
    const auto cellGID = graph(smPt, 0);
    const auto uIndex  = cellGID*m_numDofPerCell;

    const auto left0  = graph(smPt, 1);
    const auto front0 = graph(smPt, 2);
    const auto right0 = graph(smPt, 3);
    const auto back0  = graph(smPt, 4);

    if (left0 == -1)
    {
      m_ghostLeft(gRow, 0) = m_state(uIndex);
      m_ghostLeft(gRow, 1) = m_state(uIndex+1);
      m_ghostLeft(gRow, 2) = m_state(uIndex+2);
      m_ghostLeft(gRow, 3) = m_state(uIndex+3);
    }

    if (front0 == -1)
    {
      m_ghostFront(gRow, 0) = m_state(uIndex);
      m_ghostFront(gRow, 1) = m_state(uIndex+1);
      m_ghostFront(gRow, 2) = m_state(uIndex+2);
      m_ghostFront(gRow, 3) = m_state(uIndex+3);
    }

    if (right0 == -1)
    {
      m_ghostRight(gRow, 0) = m_state(uIndex);
      m_ghostRight(gRow, 1) = m_state(uIndex+1);
      m_ghostRight(gRow, 2) = m_state(uIndex+2);
      m_ghostRight(gRow, 3) = m_state(uIndex+3);
    }

    if (back0 == -1)
    {
      m_ghostBack(gRow, 0) = m_state(uIndex);
      m_ghostBack(gRow, 1) = m_state(uIndex+1);
      m_ghostBack(gRow, 2) = m_state(uIndex+2);
      m_ghostBack(gRow, 3) = m_state(uIndex+3);
    }

    if (m_stencilSize >= 5){
      const auto left1  = graph(smPt, 5);
      const auto front1 = graph(smPt, 6);
      const auto right1 = graph(smPt, 7);
      const auto back1  = graph(smPt, 8);

      if (left1 == -1){
	const auto ind = right0*m_numDofPerCell;
	m_ghostLeft(gRow, 4) = m_state(ind);
	m_ghostLeft(gRow, 5) = m_state(ind+1);
	m_ghostLeft(gRow, 6) = m_state(ind+2);
	m_ghostLeft(gRow, 7) = m_state(ind+3);
      }

      if (front1 == -1){
	const auto ind = back0*m_numDofPerCell;
	m_ghostFront(gRow, 4) = m_state(ind);
	m_ghostFront(gRow, 5) = m_state(ind+1);
	m_ghostFront(gRow, 6) = m_state(ind+2);
	m_ghostFront(gRow, 7) = m_state(ind+3);
      }

      if (right1 == -1){
	const auto ind = left0*m_numDofPerCell;
	m_ghostRight(gRow, 4) = m_state(ind);
	m_ghostRight(gRow, 5) = m_state(ind+1);
	m_ghostRight(gRow, 6) = m_state(ind+2);
	m_ghostRight(gRow, 7) = m_state(ind+3);
      }

      if (back1 == -1){
	const auto ind = front0*m_numDofPerCell;
	m_ghostBack(gRow, 4) = m_state(ind);
	m_ghostBack(gRow, 5) = m_state(ind+1);
	m_ghostBack(gRow, 6) = m_state(ind+2);
	m_ghostBack(gRow, 7) = m_state(ind+3);
      }
    }

    if (m_stencilSize == 7){
      const auto left1  = graph(smPt, 5);
      const auto front1 = graph(smPt, 6);
      const auto right1 = graph(smPt, 7);
      const auto back1  = graph(smPt, 8);
      const auto left2  = graph(smPt, 9);
      const auto front2 = graph(smPt, 10);
      const auto right2 = graph(smPt, 11);
      const auto back2  = graph(smPt, 12);

      if (left1 == -1){
	const auto ind = right0*m_numDofPerCell;
	m_ghostLeft(gRow, 4) = m_state(ind);
	m_ghostLeft(gRow, 5) = m_state(ind+1);
	m_ghostLeft(gRow, 6) = m_state(ind+2);
	m_ghostLeft(gRow, 7) = m_state(ind+3);
      }

      if (front1 == -1){
	const auto ind = back0*m_numDofPerCell;
	m_ghostFront(gRow, 4) = m_state(ind);
	m_ghostFront(gRow, 5) = m_state(ind+1);
	m_ghostFront(gRow, 6) = m_state(ind+2);
	m_ghostFront(gRow, 7) = m_state(ind+3);
      }

      if (right1 == -1){
	const auto ind = left0*m_numDofPerCell;
	m_ghostRight(gRow, 4) = m_state(ind);
	m_ghostRight(gRow, 5) = m_state(ind+1);
	m_ghostRight(gRow, 6) = m_state(ind+2);
	m_ghostRight(gRow, 7) = m_state(ind+3);
      }

      if (back1 == -1){
	const auto ind = front0*m_numDofPerCell;
	m_ghostBack(gRow, 4) = m_state(ind);
	m_ghostBack(gRow, 5) = m_state(ind+1);
	m_ghostBack(gRow, 6) = m_state(ind+2);
	m_ghostBack(gRow, 7) = m_state(ind+3);
      }

      if (left2 == -1){
	const auto ind = right1*m_numDofPerCell;
	m_ghostLeft(gRow, 8)  = m_state(ind);
	m_ghostLeft(gRow, 9)  = m_state(ind+1);
	m_ghostLeft(gRow, 10) = m_state(ind+2);
	m_ghostLeft(gRow, 11) = m_state(ind+3);
      }

      if (front2 == -1){
	const auto ind = back1*m_numDofPerCell;
	m_ghostFront(gRow, 8) = m_state(ind);
	m_ghostFront(gRow, 9) = m_state(ind+1);
	m_ghostFront(gRow, 10) = m_state(ind+2);
	m_ghostFront(gRow, 11) = m_state(ind+3);
      }

      if (right2 == -1){
	const auto ind = left1*m_numDofPerCell;
	m_ghostRight(gRow, 8) = m_state(ind);
	m_ghostRight(gRow, 9) = m_state(ind+1);
	m_ghostRight(gRow, 10) = m_state(ind+2);
	m_ghostRight(gRow, 11) = m_state(ind+3);
      }

      if (back2 == -1){
	const auto ind = front1*m_numDofPerCell;
	m_ghostBack(gRow, 8) = m_state(ind);
	m_ghostBack(gRow, 9) = m_state(ind+1);
	m_ghostBack(gRow, 10) = m_state(ind+2);
	m_ghostBack(gRow, 11) = m_state(ind+3);
      }
    }
  }

private:
  int m_stencilSize;
  int m_numDofPerCell;
  const state_t & m_state;
  const mesh_t & m_meshObj;
  ghost_t & m_ghostLeft;
  ghost_t & m_ghostFront;
  ghost_t & m_ghostRight;
  ghost_t & m_ghostBack;
};

}}
#endif
