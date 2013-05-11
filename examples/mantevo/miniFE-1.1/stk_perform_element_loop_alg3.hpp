#ifndef _stk_perform_element_loop_alg3_hpp_
#define _stk_perform_element_loop_alg3_hpp_

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

#include <Hex8_ElemData.hpp>
#include <stk_mesh_description.hpp>
#include <stk_mesh_utils.hpp>

namespace miniFE {

template<typename GlobalOrdinal,typename Scalar>
struct FEComputeOp {
  const std::vector<stk::mesh::Entity*>* elements;
  const stk_mesh_description<Scalar,GlobalOrdinal>* mesh;
  size_t n;

inline void operator()(int i)
{
  ElemDataPtr<GlobalOrdinal,Scalar> elem_data;
  stk::mesh::Entity& elem = *(*elements)[i];
  get_elem_nodes_and_coords_2(*mesh, elem, elem_data);
  compute_element_matrix_and_vector(elem_data);
}
};

template<typename GlobalOrdinal,typename Scalar,
         typename MatrixType, typename VectorType>
struct SumInOp {
  const std::vector<stk::mesh::Entity*>* nodes;
  const stk_mesh_description<Scalar,GlobalOrdinal>* mesh;
  MatrixType* A;
  VectorType* b;
  size_t n;

inline void operator()(int i)
{
  stk::mesh::Entity& node = *(*nodes)[i];
  sum_into_global_linear_system(*mesh, node, *A, *b);
}
};

template<typename MatrixType, typename VectorType>
void
perform_element_loop_alg3(const stk_mesh_description<typename MatrixType::ScalarType, typename MatrixType::GlobalOrdinalType>& mesh,
                     MatrixType& A,
                     VectorType& b,
                     Parameters& /*params*/)
{
  typedef typename MatrixType::GlobalOrdinalType GlobalOrdinal;
  typedef typename MatrixType::ScalarType Scalar;

  //we will loop over all local elements, both owned and ghosted.
  std::vector<stk::mesh::Entity*> elements; 
  stk::mesh::get_entities(mesh.bulk, mesh.fmeta.element_rank(), elements);

  FEComputeOp<GlobalOrdinal,Scalar> FE_Op;
  FE_Op.elements = &elements;
  FE_Op.mesh = &mesh;
  FE_Op.n = elements.size();

#ifdef MINIFE_HAVE_CUDA
  for(size_t ielem = 0; ielem < elements.size(); ++ielem) {
    FE_Op(ielem);
  }
#else
  typedef typename VectorType::ComputeNodeType ComputeNodeType;

  ComputeNodeType& compute_node = b.compute_node;

  compute_node.parallel_for(elements.size(), FE_Op);
#endif

  std::vector<stk::mesh::Entity*> nodes;
  stk::mesh::get_entities(mesh.bulk, mesh.fmeta.node_rank(), nodes);

  SumInOp<GlobalOrdinal,Scalar,MatrixType,VectorType> sum_in_op;
  sum_in_op.nodes = &nodes;
  sum_in_op.mesh = &mesh;
  sum_in_op.A = &A;
  sum_in_op.b = &b;
  sum_in_op.n = nodes.size();

#ifdef MINIFE_HAVE_CUDA
  for(size_t inode = 0; inode < nodes.size(); ++inode) {
    sum_in_op(inode);
  }
#else
  compute_node.parallel_for(nodes.size(), sum_in_op);
#endif
}

}//namespace miniFE

#endif

