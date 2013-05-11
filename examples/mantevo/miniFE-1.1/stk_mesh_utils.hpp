#ifndef _stk_mesh_utils_hpp_
#define _stk_mesh_utils_hpp_

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
#ifdef MINIFE_HAVE_TBB
#include <LockingMatrix.hpp>
#include <LockingVector.hpp>
#endif

namespace miniFE {

template<typename GlobalOrdinal, typename Scalar>
void
get_elem_nodes_and_coords_1(const stk_mesh_description<Scalar,GlobalOrdinal>& mesh,
                          stk::mesh::Entity& elem,
                          ElemData<GlobalOrdinal,Scalar>& elem_data)
{
  stk::mesh::PairIterRelation node_rel = elem.relations(mesh.fmeta.node_rank());
  double** coords_ptr = stk::mesh::field_data(mesh.coord_gather_field, elem);

  //Obtain the coordinates of each node that is connected to this element.
  //The coordinate data is needed by the functions that do the
  //finite-element calculations.
  unsigned offset = 0;
  for(size_t i=0; i<Hex8::numNodesPerElem; ++i) {
    elem_data.elem_node_ids[i] = *stk::mesh::field_data(mesh.ordinal_field, *node_rel[i].entity());

    const Scalar* node_coords = *coords_ptr++;
    elem_data.elem_node_coords[offset++] = node_coords[0];
    elem_data.elem_node_coords[offset++] = node_coords[1];
    elem_data.elem_node_coords[offset++] = node_coords[2];
  }
}

template<typename GlobalOrdinal, typename Scalar>
void
get_elem_nodes_and_coords_2(const stk_mesh_description<Scalar,GlobalOrdinal>& mesh,
                          stk::mesh::Entity& elem,
                          ElemDataPtr<GlobalOrdinal,Scalar>& elem_data)
{
  stk::mesh::PairIterRelation node_rel = elem.relations(mesh.fmeta.node_rank());
  double** coords_ptr = stk::mesh::field_data(mesh.coord_gather_field, elem);
  Scalar* stk_elem_data = stk::mesh::field_data(*mesh.elem_mat_field, elem);
  elem_data.elem_diffusion_matrix = stk_elem_data;
  unsigned offset = Hex8::numNodesPerElem*Hex8::numNodesPerElem;
  elem_data.elem_source_vector = stk_elem_data+offset;
  offset += Hex8::numNodesPerElem;
  Scalar* s_node_ids = stk_elem_data+offset;

  //Obtain the coordinates of each node that is connected to this element.
  //The coordinate data is needed by the functions that do the
  //finite-element calculations.
  offset = 0;
  for(size_t i=0; i<Hex8::numNodesPerElem; ++i) {
    elem_data.elem_node_ids[i] = *stk::mesh::field_data(mesh.ordinal_field, *node_rel[i].entity());
    s_node_ids[i] = elem_data.elem_node_ids[i];

    const Scalar* node_coords = *coords_ptr++;
    elem_data.elem_node_coords[offset++] = node_coords[0];
    elem_data.elem_node_coords[offset++] = node_coords[1];
    elem_data.elem_node_coords[offset++] = node_coords[2];
  }
}

template<typename GlobalOrdinal, typename Scalar,
         typename MatrixType, typename VectorType>
void
sum_into_global_linear_system(const stk_mesh_description<Scalar,GlobalOrdinal>& mesh,
                          stk::mesh::Entity& node,
                          MatrixType& A, VectorType& b)
{
  stk::mesh::PairIterRelation elem_rel = node.relations(mesh.fmeta.element_rank());

  GlobalOrdinal row = *stk::mesh::field_data(mesh.ordinal_field, node);
  GlobalOrdinal cols[Hex8::numNodesPerElem];

  for(size_t el=0; el<elem_rel.size(); ++el) {
    stk::mesh::Entity& elem = *elem_rel[el].entity();
    Scalar* elem_data = stk::mesh::field_data(*mesh.elem_mat_field, elem);
    Scalar* diffusionMatrix = elem_data;
    unsigned offset = Hex8::numNodesPerElem*Hex8::numNodesPerElem;
    Scalar* sourceVector = elem_data+offset;
    offset += Hex8::numNodesPerElem;
    Scalar* scols = elem_data+offset;

    unsigned elem_node_index = elem_rel[el].identifier();

    Scalar* row_coefs = &diffusionMatrix[elem_node_index*Hex8::numNodesPerElem];

    for(size_t i=0; i<Hex8::numNodesPerElem; ++i) {
      cols[i] = (GlobalOrdinal)scols[i];
    }

    size_t len = Hex8::numNodesPerElem;
    sum_into_row(row, len, cols, row_coefs, A);
    sum_into_vector(1, &row, &sourceVector[elem_node_index], b);
  }
}

#ifdef MINIFE_HAVE_TBB
template<typename GlobalOrdinal, typename Scalar,
         typename MatrixType, typename VectorType>
void
sum_into_global_linear_system(const stk_mesh_description<Scalar,GlobalOrdinal>& mesh,
                          stk::mesh::Entity& node,
                          LockingMatrix<MatrixType>& A, LockingVector<VectorType>& b)
{
  stk::mesh::PairIterRelation elem_rel = node.relations(mesh.fmeta.element_rank());

  GlobalOrdinal row = *stk::mesh::field_data(mesh.ordinal_field, node);
  GlobalOrdinal cols[Hex8::numNodesPerElem];

  for(size_t el=0; el<elem_rel.size(); ++el) {
    stk::mesh::Entity& elem = *elem_rel[el].entity();
    Scalar* elem_data = stk::mesh::field_data(*mesh.elem_mat_field, elem);
    Scalar* diffusionMatrix = elem_data;
    unsigned offset = Hex8::numNodesPerElem*Hex8::numNodesPerElem;
    Scalar* sourceVector = elem_data+offset;
    offset += Hex8::numNodesPerElem;
    Scalar* scols = elem_data+offset;

    unsigned elem_node_index = elem_rel[el].identifier();

    Scalar* row_coefs = &diffusionMatrix[elem_node_index*Hex8::numNodesPerElem];

    for(size_t i=0; i<Hex8::numNodesPerElem; ++i) {
      cols[i] = (GlobalOrdinal)scols[i];
    }

    size_t len = Hex8::numNodesPerElem;
    A.sum_in(row, len, cols, row_coefs);
    b.sum_in(1, &row, &sourceVector[elem_node_index]);
  }
}
#endif

//----------------------------------------------------------------------

namespace {

template<typename OrdinalType>
OrdinalType global_offset( stk::ParallelMachine comm ,
                            OrdinalType local_len )
{
  OrdinalType local_offset = local_len ; // Inclusive scan
#ifdef HAVE_MPI
  MPI_Datatype mpi_dtype = TypeTraits<OrdinalType>::mpi_type();
  MPI_Scan( & local_len , & local_offset , 1 , mpi_dtype , MPI_SUM , comm );
#endif
  // Returns inclusive scan, subtract for offset to begining
  local_offset -= local_len ;
  return local_offset ;
}

template<typename ValueType>
ValueType global_sum( stk::ParallelMachine comm ,
                      ValueType local_val )
{
  ValueType global_val = local_val ;
#ifdef HAVE_MPI
  MPI_Datatype mpi_dtype = TypeTraits<ValueType>::mpi_type();
  MPI_Allreduce(&local_val, &global_val, 1, mpi_dtype, MPI_SUM, comm );
#endif
  return global_val ;
}

}

void communicate_field_data( const stk::mesh::BulkData & mesh ,
                             const std::vector< const stk::mesh::FieldBase * > & fields );

//----------------------------------------------------------------------

template<typename Scalar, typename GlobalOrdinal>
void
populate_mesh( stk_mesh_description<Scalar,GlobalOrdinal> & mesh ,
               const int global_box_in[][2] ,
               const int local_box_in[][2] )
{
  const int numprocs = mesh.bulk.parallel_size();
  const int myproc   = mesh.bulk.parallel_rank();

  //------------------------------
  // Change the mesh bulk data to the 'ok to modify' state.
  mesh.bulk.modification_begin();

  //Global domain has num-nodes in x-, y-, and z-dimensions:
  int global_node_nx = global_box_in[0][1]+1;
  int global_node_ny = global_box_in[1][1]+1;
  int global_node_nz = global_box_in[2][1]+1;

  //work-arrays that will hold element-connectivity data:
  stk::mesh::EntityId elem_node_ids[Hex8::numNodesPerElem];
  Scalar elem_node_coords[Hex8::numNodesPerElem*Hex8::spatialDim];

  //3-D loop over the local portion of the problem domain:
  for(int iz=local_box_in[2][0]; iz<local_box_in[2][1]; ++iz) {
    for(int iy=local_box_in[1][0]; iy<local_box_in[1][1]; ++iy) {
      for(int ix=local_box_in[0][0]; ix<local_box_in[0][1]; ++ix) {

        //Each element has 8 nodes locally-numbered 0 .. 7.
        //We use coordinates to obtain the node-0 for an element, and then
        //we can find out everything else about the element from that.

        stk::mesh::EntityId node0ID =
            get_id<stk::mesh::EntityId>( global_node_nx,
                                   global_node_ny,
                                   global_node_nz,
                                   ix, iy, -iz);

        if (node0ID < 0) {
          //This should never happen... do we need this error-check?
          std::cerr << "ERROR, negative node0ID" << std::endl;
          continue;
        }

        //Get this element's connected-nodes:
        get_hex8_node_ids(global_node_nx, global_node_ny, node0ID, &elem_node_ids[0]);

        //Get the coordinates for these nodes:
        get_hex8_node_coords_3d(ix, iy, -iz, &elem_node_coords[0]);

        //convert ids from 0-based to 1-based (stk::mesh deals in 1-based ids):
        ++node0ID;
        for(int i=0; i<Hex8::numNodesPerElem; ++i) ++elem_node_ids[i];

        //declare an element entity, specifying that it belongs to
        //the elem_block mesh-part, and set its connected-nodes:
        //(note that here we use node0ID as the element-ID)
        stk::mesh::Entity& elem =
          stk::mesh::fem::declare_element( mesh.bulk, mesh.elem_block, node0ID, elem_node_ids);

        //get iterators for the element's node relations (connected nodes):
        stk::mesh::PairIterRelation rel = elem.relations(mesh.fmeta.node_rank());

        //set the coordinates for each node:
        unsigned offset = 0;
        for(int i=0; i<Hex8::numNodesPerElem; ++i) {
          stk::mesh::Entity& node = *rel[i].entity();
          Scalar* data = stk::mesh::field_data(mesh.coord_field, node);
          data[0] = elem_node_coords[offset++];
          data[1] = elem_node_coords[offset++];
          data[2] = elem_node_coords[offset++];
        }
      }
    }
  }

  // Finished creating mesh-entities, let the mesh resolve
  // parallel sharing and generate the one-layer ghosting.
  mesh.bulk.modification_end();
 //------------------------------
  //obtain all locally-owned nodes:
  std::vector<stk::mesh::Entity*> owned_nodes;
  mesh.get_owned_nodes( owned_nodes );

  //we will store a global matrix row-number in the ordinal field on
  //each node.
  //First, figure out this processor's first global matrix row:
  GlobalOrdinal row =
    global_offset<GlobalOrdinal>( mesh.bulk.parallel() , owned_nodes.size() );

  //now set a row-number on each node:
  for(std::vector<stk::mesh::Entity*>::iterator it = owned_nodes.begin();
      it != owned_nodes.end(); ++it, ++row) {
    *stk::mesh::field_data(mesh.ordinal_field, **it) = row;
  }

  // Send the ordinal field value for matrix rows from owner to all non-owned copies
  std::vector<const stk::mesh::FieldBase*> fields(1, & mesh.ordinal_field);
  miniFE::communicate_field_data( mesh.bulk, fields);

  std::vector<stk::mesh::Entity*> all_nodes;
  stk::mesh::get_entities(mesh.bulk, mesh.fmeta.node_rank(), all_nodes);

  //miniFE will set a dirichlet boundary-condition on each node that lies
  //on the x==0 face of the global problem domain. For convenience,
  //store the row-numbers for those nodes in a set for access later.
  for(std::vector<stk::mesh::Entity*>::iterator it = all_nodes.begin();
      it != all_nodes.end(); ++it) {
    if (std::abs(stk::mesh::field_data(mesh.coord_field, **it)[0]) < 0.01) {
      mesh.bc_rows.insert(*stk::mesh::field_data(mesh.ordinal_field, **it));
    }
  }
}

}//namespace miniFE

#endif

