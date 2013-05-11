#ifndef _stk_assemble_FE_data_hpp_
#define _stk_assemble_FE_data_hpp_

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
#include <stk_mesh_description.hpp>

#ifdef MINIFE_HAVE_TBB
#include <stk_perform_element_loop_alg1.hpp>
#endif
#include <stk_perform_element_loop_alg3.hpp>

namespace miniFE {

template<typename MatrixType,
         typename VectorType>
void
assemble_FE_data(const stk_mesh_description<typename MatrixType::ScalarType, typename MatrixType::GlobalOrdinalType>& mesh,
                 MatrixType& A,
                 VectorType& b,
                 Parameters& params)
{
  bool use_alg1 = params.use_elem_mat_fields==0;
  if (use_alg1) {
#ifndef MINIFE_HAVE_TBB
    throw std::runtime_error("ERROR: Since TBB is not enabled, use_elem_mat_fields==0 is not available. Throwing an exception. (You need to use the option use_elem_mat_fields=1)");
#endif
  }

  if (params.use_elem_mat_fields==0) {
#ifdef MINIFE_HAVE_TBB
    perform_element_loop_alg1(mesh, A, b, params);
#endif
  }
  else {
    perform_element_loop_alg3(mesh, A, b, params);
  }
}

}//namespace miniFE

#endif

