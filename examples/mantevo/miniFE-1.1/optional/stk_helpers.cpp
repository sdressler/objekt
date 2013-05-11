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

#include <stk_util/parallel/ParallelComm.hpp>

#include <stk_mesh/base/MetaData.hpp>
#include <stk_mesh/base/BulkData.hpp>
#include <stk_mesh/base/Field.hpp>
#include <stk_mesh/base/FieldData.hpp>
#include <stk_mesh/base/EntityComm.hpp>

namespace miniFE {

void communicate_field_data( const stk::mesh::BulkData & mesh ,
                             const std::vector< const stk::mesh::FieldBase * > & fields )
{
  if ( fields.empty() ) { return; }

  const unsigned parallel_size = mesh.parallel_size();
  const unsigned parallel_rank = mesh.parallel_rank();

  // Sizing for send and receive
  
  const unsigned zero = 0 ;
  std::vector<unsigned> send_size( parallel_size , zero );
  std::vector<unsigned> recv_size( parallel_size , zero );
  std::vector<unsigned> procs ;
  
  for ( std::vector<stk::mesh::Entity*>::const_iterator
        i =  mesh.entity_comm().begin() ;
        i != mesh.entity_comm().end() ; ++i ) {
    stk::mesh::Entity & e = **i ;

    unsigned size = 0 ;
    for ( std::vector<const stk::mesh::FieldBase *>::const_iterator
          fi = fields.begin() ; fi != fields.end() ; ++fi ) {
      const stk::mesh::FieldBase & f = **fi ; 
      size += stk::mesh::field_data_size( f , e );
    }

    if ( size ) {
      if ( e.owner_rank() == parallel_rank ) {
        // owner sends
        stk::mesh::comm_procs( e , procs );
        for ( std::vector<unsigned>::iterator
              ip = procs.begin() ; ip != procs.end() ; ++ip ) {
          send_size[ *ip ] += size ;
        }
      }
      else { 
        // non-owner receives
        recv_size[ e.owner_rank() ] += size ;
      }
    }
  }

  // Allocate send and receive buffers:
  
  stk::CommAll sparse ;

  {
    const unsigned * const s_size = & send_size[0] ;
    const unsigned * const r_size = & recv_size[0] ;
    sparse.allocate_buffers( mesh.parallel(), parallel_size / 4 , s_size, r_size);
  }

  // Send packing:

  for ( std::vector<stk::mesh::Entity*>::const_iterator
        i =  mesh.entity_comm().begin() ;
        i != mesh.entity_comm().end() ; ++i ) {
    stk::mesh::Entity & e = **i ;

    if ( e.owner_rank() == parallel_rank ) {

      stk::mesh::comm_procs( e , procs );

      for ( std::vector<const stk::mesh::FieldBase *>::const_iterator
            fi = fields.begin() ; fi != fields.end() ; ++fi ) {
        const stk::mesh::FieldBase & f = **fi ;
        const unsigned size = stk::mesh::field_data_size( f , e );

        if ( size ) {
          unsigned char * ptr =
            reinterpret_cast<unsigned char *>(stk::mesh::field_data( f , e ));

          for ( std::vector<unsigned>::iterator
                ip = procs.begin() ; ip != procs.end() ; ++ip ) {

            stk::CommBuffer & b = sparse.send_buffer( *ip );
            b.pack<unsigned char>( ptr , size );
          }
        }
      }
    }
  }

  // Communicate:

  sparse.communicate();

  // Unpack for recv:

  for ( std::vector<stk::mesh::Entity*>::const_iterator
        i =  mesh.entity_comm().begin() ;
        i != mesh.entity_comm().end() ; ++i ) {
    stk::mesh::Entity & e = **i ;
    if ( e.owner_rank() != parallel_rank ) {

      for ( std::vector<const stk::mesh::FieldBase *>::const_iterator
            fi = fields.begin() ; fi != fields.end() ; ++fi ) {
        const stk::mesh::FieldBase & f = **fi ;
        const unsigned size = stk::mesh::field_data_size( f , e );

        if ( size ) {
          unsigned char * ptr =
            reinterpret_cast<unsigned char *>(stk::mesh::field_data( f , e ));

          stk::CommBuffer & b = sparse.recv_buffer( e.owner_rank() );
          b.unpack<unsigned char>( ptr , size );
        }
      }
    }
  }
}

} // namespace miniFE


