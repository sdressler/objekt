#ifndef _stk_perform_element_loop_alg1_hpp_
#define _stk_perform_element_loop_alg1_hpp_

//@HEADER
// ************************************************************************
// 
//               miniFE: simple finite-element assembly and linear-solve
//                 Copyright (2006) Sandia Corporation
// 
// Under terms of Contract DE-AC04-94AL85000, there is a non-exclusive
// license for use of this work by or on behalf of the U.S. Government.
// 
// This library is free software; you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as
// published by the Free Software Foundation; either version 2.1 of the
// License, or (at your option) any later version.
//  
// This library is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
//  
// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307
// USA
// Questions? Contact Michael A. Heroux (maherou@sandia.gov) 
// 
// ************************************************************************
//@HEADER

#ifdef MINIFE_HAVE_TBB

#include <Hex8_ElemData.hpp>
#include <stk_mesh_description.hpp>
#include <stk_mesh_utils.hpp>

#include <LockingMatrix.hpp>
#include <LockingVector.hpp>

namespace miniFE {

/** FEAssembleOp
 */
template<typename GlobalOrdinal,typename Scalar,
         typename MatrixType, typename VectorType>
struct FEAssembleOp {
  const std::vector<stk::mesh::Entity*>* elements;
  const stk_mesh_description<Scalar, GlobalOrdinal>* mesh;
  LockingMatrix<MatrixType>* A;
  LockingVector<VectorType>* b;

inline void operator()(int i)
{
  ElemData<GlobalOrdinal,Scalar> elem_data;
  stk::mesh::Entity& elem = *(*elements)[i];
  get_elem_nodes_and_coords_1(*mesh, elem, elem_data);
  compute_element_matrix_and_vector(elem_data);
  sum_into_global_linear_system(elem_data, *A, *b);
}
};

template<typename MatrixType, typename VectorType>
void
perform_element_loop_alg1(const stk_mesh_description<typename MatrixType::ScalarType, typename MatrixType::GlobalOrdinalType>& mesh,
                     MatrixType& A,
                     VectorType& b,
                     Parameters& /*params*/)
{
  typedef typename MatrixType::GlobalOrdinalType GlobalOrdinal;
  typedef typename MatrixType::ScalarType Scalar;

  //we will loop over all local elements, both owned and ghosted.
  std::vector<stk::mesh::Entity*> elements;
  stk::mesh::get_entities(mesh.bulk, mesh.fmeta.element_rank(), elements);

  LockingMatrix<MatrixType> lockingA(A);
  LockingVector<VectorType> lockingb(b);

  FEAssembleOp<GlobalOrdinal,Scalar,MatrixType,VectorType> FE_Op;
  FE_Op.elements = &elements;
  FE_Op.mesh = &mesh;
  FE_Op.A = &lockingA;
  FE_Op.b = &lockingb;
  
#ifdef MINIFE_HAVE_CUDA
  for(size_t ielem = 0; ielem < elements.size(); ++ielem) {
    FE_Op(ielem);
  }
#else
  typedef typename VectorType::ComputeNodeType ComputeNodeType;

  ComputeNodeType& compute_node = b.compute_node;

  compute_node.parallel_for(elements.size(), FE_Op); 
#endif
}

}//namespace miniFE

#else
#error "This file shouldn't be compiled if MINIFE_HAVE_TBB isn't defined."
#endif
#endif

