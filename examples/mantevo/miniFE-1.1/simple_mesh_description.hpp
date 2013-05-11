
#ifndef _simple_mesh_description_hpp_
#define _simple_mesh_description_hpp_

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

#include <utils.hpp>
#include <set>
#include <map>

namespace miniFE {

template<typename GlobalOrdinal>
class simple_mesh_description {
public:
  simple_mesh_description(const int global_box_in[][2], const int local_box_in[][2])
  {
   int local_node_box[3][2];
    for(int i=0; i<3; ++i) {
      global_box[i][0] = global_box_in[i][0];
      global_box[i][1] = global_box_in[i][1];
      local_box[i][0] = local_box_in[i][0];
      local_box[i][1] = local_box_in[i][1];
      local_node_box[i][0] = local_box_in[i][0];
      local_node_box[i][1] = local_box_in[i][1];
      //num-owned-nodes == num-elems+1 in this dimension if the elem box is not empty
      //and we are at the high end of the global range in that dimension:
      if (local_box_in[i][1] > local_box_in[i][0] && local_box_in[i][1] == global_box[i][1]) local_node_box[i][1] += 1;
    }

    int max_node_x = global_box[0][1]+1;
    int max_node_y = global_box[1][1]+1;
    int max_node_z = global_box[2][1]+1;
    create_map_id_to_row(max_node_x, max_node_y, max_node_z, local_node_box,
                         map_ids_to_rows);

    if (local_node_box[0][0] <= 1) {
      if (local_node_box[2][0] > 0) --local_node_box[2][0];
      if (local_node_box[1][0] > 0) --local_node_box[1][0];
      if (local_node_box[2][1] < max_node_z) ++local_node_box[2][1];
      if (local_node_box[1][1] < max_node_y) ++local_node_box[1][1];

      for(int iz=local_node_box[2][0]; iz<local_node_box[2][1]; ++iz) {
        for(int iy=local_node_box[1][0]; iy<local_node_box[1][1]; ++iy) {
          GlobalOrdinal nodeID = get_id<GlobalOrdinal>(max_node_x, max_node_y, max_node_z,
             0, iy, -iz);
          bc_rows.insert(map_id_to_row(nodeID));
        }
      }
    }
  }

  GlobalOrdinal map_id_to_row(const GlobalOrdinal& id) const
  {
    return find_row_for_id(id, map_ids_to_rows);
  }

  std::set<GlobalOrdinal> bc_rows;
  std::map<GlobalOrdinal,GlobalOrdinal> map_ids_to_rows;
  int global_box[3][2];
  int local_box[3][2];
};//class simple_mesh_description

}//namespace miniFE

#endif
