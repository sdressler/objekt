#ifndef _Hex8_ElemData_hpp_
#define _Hex8_ElemData_hpp_

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

#include <Hex8_enums.hpp>
#include <Hex8.hpp>
#include <ElemData.hpp>

namespace miniFE {

template<typename GlobalOrdinal,typename Scalar>
void
compute_element_matrix_and_vector(ElemData<GlobalOrdinal,Scalar>& elem_data)
{
  Hex8::diffusionMatrix(elem_data.elem_node_coords, elem_data.elem_diffusion_matrix);
  Hex8::sourceVector(elem_data.elem_node_coords, elem_data.elem_source_vector);
}

template<typename GlobalOrdinal,typename Scalar>
void
compute_element_matrix_and_vector(ElemDataPtr<GlobalOrdinal,Scalar>& elem_data)
{
  Hex8::diffusionMatrix(elem_data.elem_node_coords, elem_data.elem_diffusion_matrix);
  Hex8::sourceVector(elem_data.elem_node_coords, elem_data.elem_source_vector);
}

}//namespace miniFE

#endif

