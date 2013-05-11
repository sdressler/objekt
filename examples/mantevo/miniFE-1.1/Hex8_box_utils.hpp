#ifndef _Hex8_box_utils_hpp_
#define _Hex8_box_utils_hpp_

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

#include <stdexcept>

#include <box_utils.hpp>
#include <ElemData.hpp>
#include <simple_mesh_description.hpp>
#include <Hex8.hpp>

namespace miniFE {


template<typename GlobalOrdinal>
void get_hex8_node_ids(int nx, int ny,
                       GlobalOrdinal node0,
                       GlobalOrdinal* elem_node_ids)
{
  elem_node_ids[0] = node0;
  elem_node_ids[1] = node0 + 1;
  elem_node_ids[2] = node0 + 1 + nx*ny;
  elem_node_ids[3] = node0 +     nx*ny;
  elem_node_ids[4] = node0 + nx;
  elem_node_ids[5] = node0 + nx + 1;
  elem_node_ids[6] = node0 + nx + nx*ny + 1;
  elem_node_ids[7] = node0 + nx + nx*ny;
}

template<typename Scalar>
void get_hex8_node_coords_3d(int x, int y, int z,
                             Scalar* elem_node_coords)
{
  //Input: x,y,z are the coordinates of local-node 0 for a Hex8.

  elem_node_coords[0] = x;
  elem_node_coords[1] = y;
  elem_node_coords[2] = z;

  elem_node_coords[3] = x + 1;
  elem_node_coords[4] = y;
  elem_node_coords[5] = z;

  elem_node_coords[6] = x + 1;
  elem_node_coords[7] = y;
  elem_node_coords[8] = z - 1;

  elem_node_coords[9] = x;
  elem_node_coords[10] = y;
  elem_node_coords[11] = z - 1;

  elem_node_coords[12] = x;
  elem_node_coords[13] = y + 1;
  elem_node_coords[14] = z;

  elem_node_coords[15] = x + 1;
  elem_node_coords[16] = y + 1;
  elem_node_coords[17] = z;

  elem_node_coords[18] = x + 1;
  elem_node_coords[19] = y + 1;
  elem_node_coords[20] = z - 1;

  elem_node_coords[21] = x;
  elem_node_coords[22] = y + 1;
  elem_node_coords[23] = z - 1;
}

template<typename GlobalOrdinal, typename Scalar>
void
get_elem_nodes_and_coords(const simple_mesh_description<GlobalOrdinal>& mesh,
                          GlobalOrdinal elemID,
                          GlobalOrdinal* node_ords, Scalar* node_coords)
{
  int global_nodes_x = mesh.global_box[0][1]+1;
  int global_nodes_y = mesh.global_box[1][1]+1;
  int global_nodes_z = mesh.global_box[2][1]+1;
 
  if (elemID < 0) {
    //I don't think this can happen, but check for the sake of paranoia...
    throw std::runtime_error("get_elem_nodes_and_coords ERROR, negative elemID");
  }
  int elem_x, elem_y, elem_z;
  get_coords(elemID, global_nodes_x-1, global_nodes_y-1, global_nodes_z-1,
             elem_x, elem_y, elem_z);
  GlobalOrdinal nodeID = get_id<GlobalOrdinal>(global_nodes_x, global_nodes_y, global_nodes_z, elem_x, elem_y, elem_z);

  get_hex8_node_ids(global_nodes_x, global_nodes_y, nodeID, node_ords);

  //Map node-IDs to rows because each processor has a non-contiguous block of
  //node-ids, but needs a contiguous block of row-numbers:
//#ifdef MINIFE_DEBUG
//  std::cout << "Nodes: " << std::endl;
//#endif
  for(int i=0; i<Hex8::numNodesPerElem; ++i) {
//#ifdef MINIFE_DEBUG
//    std::cout << node_ords[i] << " ";
//#endif
    node_ords[i] = mesh.map_id_to_row(node_ords[i]);
  }
//#ifdef MINIFE_DEBUG
//  std::cout << std::endl;
//#endif

  int global_elems_x = mesh.global_box[0][1];
  int global_elems_y = mesh.global_box[1][1];
  int global_elems_z = mesh.global_box[2][1];
 
  int ix,iy,iz;
  get_coords<GlobalOrdinal>(elemID, global_elems_x,global_elems_y,global_elems_z,
                            ix,iy,iz);
  get_hex8_node_coords_3d(ix, iy, iz, node_coords);
//#ifdef MINIFE_DEBUG
//  int offset = 0;
//  for(int i=0; i<Hex8::numNodesPerElem; ++i) {
//    std::cout << "(";
//    for(int j=0; j<Hex8::spatialDim; ++j) {
//      std::cout << node_coords[offset++]<<" ";
//    }
//    std::cout << ")";
//  }
//  std::cout << std::endl;
//#endif
}

template<typename GlobalOrdinal, typename Scalar>
void
get_elem_nodes_and_coords(const simple_mesh_description<GlobalOrdinal>& mesh,
                          GlobalOrdinal elemID,
                          ElemData<GlobalOrdinal,Scalar>& elem_data)
{
  get_elem_nodes_and_coords(mesh, elemID, elem_data.elem_node_ids, elem_data.elem_node_coords);
}

}//namespace miniFE

#endif

