#ifndef _stk_mesh_description_hpp_
#define _stk_mesh_description_hpp_

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

#include <Hex8.hpp>
#include <utils.hpp>
#include <Hex8_box_utils.hpp>
#include <ElemData.hpp>

#include <stk_mesh/base/MetaData.hpp>
#include <stk_mesh/base/BulkData.hpp>
#include <stk_mesh/base/Field.hpp>
#include <stk_mesh/base/FieldData.hpp>
#include <stk_mesh/base/FieldParallel.hpp>
#include <stk_mesh/base/GetEntities.hpp>
#include <stk_mesh/base/SetOwners.hpp>

#include <stk_mesh/fem/FEMMetaData.hpp>
#include <stk_mesh/fem/FEMHelpers.hpp>
#include <stk_mesh/fem/Stencils.hpp>
#include <stk_mesh/fem/CoordinateSystems.hpp>
#include <stk_mesh/fem/TopologyDimensions.hpp>

#include <Shards_BasicTopologies.hpp>

namespace miniFE {

//
//class stk_mesh_description holds mesh data for the local mpi process.
//
//The stk_mesh_description constructor creates the stk::mesh objects and
//initializes them with mesh data (mesh-entities such as nodes and elements,
//as well as field-data such as coordinates).
//
//This file also provides these functions:
// - generate_matrix_structure
//          fills a sparse matrix structure using element-node connectivity
// - assemble_FE_data
//          element-loop over local elements, compute element-diffusion-
//          matrix and element-source-vector, assembles them into
//          global sparse linear-system
//
template<typename Scalar, typename GlobalOrdinal>
class stk_mesh_description {
public:
  typedef stk::mesh::Field<GlobalOrdinal> OrdinalFieldType;
  typedef stk::mesh::Field<Scalar, stk::mesh::Cartesian> CoordFieldType;
  typedef stk::mesh::Field<Scalar, stk::mesh::Cartesian> ElemMatFieldType;
  typedef stk::mesh::Field<Scalar*,stk::mesh::ElementNode> CoordGatherFieldType;

  stk::mesh::fem::FEMMetaData  fmeta ;
  stk::mesh::BulkData     bulk ;
  CoordFieldType        & coord_field ;
  ElemMatFieldType      * elem_mat_field ;
  OrdinalFieldType      & ordinal_field ;
  CoordGatherFieldType  & coord_gather_field ;
  stk::mesh::Part       & elem_block ;
  std::set<GlobalOrdinal> bc_rows ;

  // Constructor:
  // Generate the mesh meta data.
  stk_mesh_description( stk::ParallelMachine comm, bool create_elem_mat_field = false)
    : fmeta( Hex8::spatialDim, stk::mesh::fem::entity_rank_names(Hex8::spatialDim) ),
      // Meta data must be constructed first before
      // any of the following constructors and declarations.
      bulk( stk::mesh::fem::FEMMetaData::get_meta_data(fmeta) , comm ),
      coord_field(   fmeta.declare_field<CoordFieldType>("Coordinates") ),
      elem_mat_field(NULL),
      ordinal_field( fmeta.declare_field<OrdinalFieldType>("Ordinal") ),
      coord_gather_field( fmeta.declare_field<CoordGatherFieldType>("GatherCoordinates") ),
      elem_block( stk::mesh::fem::declare_part<shards::Hexahedron<8> >(fmeta, "block1") ),
      bc_rows()
  {
    //put coord-field on all nodes with the spatial dimension
    stk::mesh::put_field( coord_field, fmeta.node_rank(), fmeta.universal_part(), Hex8::spatialDim);

    if (create_elem_mat_field) {
      elem_mat_field = &fmeta.declare_field<ElemMatFieldType>("ElemData");
      unsigned fsize = Hex8::numNodesPerElem*(Hex8::numNodesPerElem+2);
      stk::mesh::put_field(*elem_mat_field, fmeta.element_rank(), fmeta.universal_part(), fsize);
    }

    //put coord-gather-field on all elements:
    stk::mesh::put_field( coord_gather_field, fmeta.element_rank(), fmeta.universal_part(), Hex8::numNodesPerElem);

    // Set up relation between coord-gather-field on elements
    // and coord-field on nodes such that the coord-gather-field
    // pointers are set to the element's node's coord-field.
    // This field relation provides fast access to element-node-coord data.
    fmeta.declare_field_relation( coord_gather_field, stk::mesh::fem::get_element_node_stencil(Hex8::spatialDim), coord_field);

    //put matrix-equation-number ordinal-field on all nodes:
    stk::mesh::put_field( ordinal_field, fmeta.node_rank(), fmeta.universal_part());

    // We are finished creating/modifying meta-data:
    fmeta.commit();
  }

  void get_owned_nodes( std::vector<stk::mesh::Entity*> & owned_nodes ) const
  {
    owned_nodes.clear();
    const stk::mesh::Selector select_owned( fmeta.locally_owned_part() );
    stk::mesh::get_selected_entities( select_owned, bulk.buckets(fmeta.node_rank()), owned_nodes);
  }

  //Destructor:
  ~stk_mesh_description() {}

};//class stk_mesh_description

}//namespace miniFE

#endif
