#ifndef _stk_generate_matrix_structure_hpp_
#define _stk_generate_matrix_structure_hpp_

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

#include <stk_mesh_description.hpp>
#include <stk_mesh_utils.hpp>

namespace miniFE {

template<typename MatrixType>
int
generate_matrix_structure(
  const stk_mesh_description< typename MatrixType::ScalarType,
                              typename MatrixType::GlobalOrdinalType>& mesh,
                          MatrixType& A)
{
  const int myproc = mesh.bulk.parallel_rank();

  int threw_exc = 0;
  try {

  typedef typename MatrixType::GlobalOrdinalType GlobalOrdinal;

  //The locally-owned portion of the global sparse matrix has a row for
  //each locally-owned node in the mesh.
  std::vector<stk::mesh::Entity*> owned_nodes;
  mesh.get_owned_nodes( owned_nodes );

  GlobalOrdinal nrows = owned_nodes.size();
  try {
    A.rows.resize(nrows);
    A.row_offsets.resize(nrows+1);
    A.packed_cols.reserve(nrows*27);
    A.packed_coefs.reserve(nrows*27);
  }
  catch(std::exception& exc) {
    std::ostringstream osstr;
    osstr << "One of A.rows.resize, A.row_offsets.resize, A.packed_cols.reserve or A.packed_coefs.reserve: nrows=" <<nrows<<": ";
    osstr << exc.what();
    std::string str1 = osstr.str();
    throw std::runtime_error(str1);
  }

  size_t nnz = 0;
  for(size_t i=0; i<owned_nodes.size(); ++i) {
    stk::mesh::Entity& node = *owned_nodes[i];
    A.rows[i] = *stk::mesh::field_data(mesh.ordinal_field, node);

    //this row of the matrix has a column-index for each node that is
    //connected to an element that *this* node is connected to.
    //So we loop over this node's element-relations, and then loop over
    //those elements' node relations:
    stk::mesh::PairIterRelation rel = node.relations(mesh.fmeta.element_rank());
    std::set<GlobalOrdinal> column_ids;
    for(size_t j=0; j<rel.size(); ++j) {
      stk::mesh::Entity& elem = *rel[j].entity();
      stk::mesh::PairIterRelation node_rel = elem.relations(mesh.fmeta.node_rank());

      for(size_t k=0; k<node_rel.size(); ++k) {
        column_ids.insert(*stk::mesh::field_data(mesh.ordinal_field, *node_rel[k].entity()));
      }
    }

    A.row_offsets[i] = nnz;
    nnz += column_ids.size();

    A.packed_cols.insert(A.packed_cols.end(), column_ids.begin(), column_ids.end());
  }
  A.row_offsets[owned_nodes.size()] = nnz;
  A.packed_coefs.resize(nnz);

  }
  catch(...) {
    std::cout << "proc " << myproc << " threw an exception in generate_matrix_structure, probably due to running out of memory." << std::endl;
    threw_exc = 1;
  }

  threw_exc = global_sum<int>( mesh.bulk.parallel() , threw_exc );

  if (threw_exc) {
    return 1;
  }

  return 0;
}

}//namespace miniFE

#endif

