#ifndef _Hex8_hpp_
#define _Hex8_hpp_

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

#ifndef KERNEL_PREFIX 
#define KERNEL_PREFIX
#endif

#include <gauss_pts.hpp>
#include <matrix_algebra_3x3.hpp>
#include <Hex8_enums.hpp>

namespace miniFE {

namespace Hex8 {

template<typename Scalar>
KERNEL_PREFIX void shape_fns(const Scalar* x, Scalar* values_at_nodes)
{
  //dangerous assumption that values_at_nodes has length numNodesPerElem
  //also, non-general assumption that x has length 3 (hard-coded spatialDim)

  const Scalar u = 1.0 - x[0];
  const Scalar v = 1.0 - x[1];
  const Scalar w = 1.0 - x[2];

  const Scalar up1 = 1.0 + x[0];
  const Scalar vp1 = 1.0 + x[1];
  const Scalar wp1 = 1.0 + x[2];

  const Scalar one_eighth = 0.125; // 1.0/8.0;

  values_at_nodes[0] = one_eighth *   u *   v * wp1;
  values_at_nodes[1] = one_eighth * up1 *   v * wp1;
  values_at_nodes[2] = one_eighth * up1 *   v *   w;
  values_at_nodes[3] = one_eighth *   u *   v *   w;
  values_at_nodes[4] = one_eighth *   u * vp1 * wp1;
  values_at_nodes[5] = one_eighth * up1 * vp1 * wp1;
  values_at_nodes[6] = one_eighth * up1 * vp1 *   w;
  values_at_nodes[7] = one_eighth *   u * vp1 *   w;
}

template<typename Scalar>
KERNEL_PREFIX void gradients(const Scalar* x, Scalar* values_per_fn)
{
  //dangerous assumption that values_per_fn has length 24 (numNodesPerElem*spatialDim)
  //, and that spatialDim == 3

  const Scalar u = 1.0 - x[0];
  const Scalar v = 1.0 - x[1];
  const Scalar w = 1.0 - x[2];

  const Scalar up1 = 1.0 + x[0];
  const Scalar vp1 = 1.0 + x[1];
  const Scalar wp1 = 1.0 + x[2];

  const Scalar one_eighth = 0.125; // 1.0/8.0;

//fn 0
  values_per_fn[0] = -one_eighth *  v *  wp1;
  values_per_fn[1] = -one_eighth *  u *  wp1;
  values_per_fn[2] =  one_eighth *  u *  v;
//fn 1
  values_per_fn[3] =  one_eighth *  v   *  wp1;
  values_per_fn[4] = -one_eighth *  up1 *  wp1;
  values_per_fn[5] =  one_eighth *  up1 *  v;
//fn 2
  values_per_fn[6] =  one_eighth *  v   *  w;
  values_per_fn[7] = -one_eighth *  up1 *  w;
  values_per_fn[8] = -one_eighth *  up1 *  v;
//fn 3
  values_per_fn[9]  = -one_eighth *  v   *  w;
  values_per_fn[10] = -one_eighth *  u   *  w;
  values_per_fn[11] = -one_eighth *  u   *  v;
//fn 4
  values_per_fn[12] = -one_eighth *  vp1 * wp1;
  values_per_fn[13] =  one_eighth *  u   * wp1;
  values_per_fn[14] =  one_eighth *  u   * vp1;
//fn 5
  values_per_fn[15] =  one_eighth *  vp1 * wp1;
  values_per_fn[16] =  one_eighth *  up1 * wp1;
  values_per_fn[17] =  one_eighth *  up1 * vp1;
//fn 6
  values_per_fn[18] =  one_eighth *  vp1 * w;
  values_per_fn[19] =  one_eighth *  up1 * w;
  values_per_fn[20] = -one_eighth *  up1 * vp1;
//fn 7
  values_per_fn[21] = -one_eighth *  vp1 * w;
  values_per_fn[22] =  one_eighth *  u   * w;
  values_per_fn[23] = -one_eighth *  u   * vp1;
}

template<typename Scalar>
KERNEL_PREFIX void gradients_and_jacobian(const Scalar* pt,
                            const Scalar* elemNodeCoords,
                            Scalar* grad_vals,
                            Scalar* J)
{
/**
  pt is the point at which the jacobian is to be computed.
*/

  //assumptions on the lengths of input arguments:
  //pt has length spatialDim,
  //elemNodeCoords has length numNodesPerElem*spatialDim,
  //grad_vals has length numNodesPerElem*spatialDim, and
  //J has length spatialDim*spatialDim

  const Scalar zero = 0;
  miniFE::fill(J, J+spatialDim*spatialDim, zero);

  gradients(pt, grad_vals);

  size_t i_X_spatialDim = 0;
  for(size_t i=0; i<numNodesPerElem; ++i) {
//    size_t offset = 0;
//    for(size_t gd=0; gd<spatialDim; ++gd) {
//
//      Scalar gval = grad_vals[i_X_spatialDim+gd];
//
//      for(size_t jd=0; jd<spatialDim; ++jd) {
//        J[offset++] += gval*elemNodeCoords[i_X_spatialDim+jd];
//      }
//    }
    //for optimization, unroll the above double-loop over spatialDim:
    //(another hard-coded assumption that spatialDim == 3)
    J[0] += grad_vals[i_X_spatialDim+0]*elemNodeCoords[i_X_spatialDim+0];
    J[1] += grad_vals[i_X_spatialDim+0]*elemNodeCoords[i_X_spatialDim+1];
    J[2] += grad_vals[i_X_spatialDim+0]*elemNodeCoords[i_X_spatialDim+2];

    J[3] += grad_vals[i_X_spatialDim+1]*elemNodeCoords[i_X_spatialDim+0];
    J[4] += grad_vals[i_X_spatialDim+1]*elemNodeCoords[i_X_spatialDim+1];
    J[5] += grad_vals[i_X_spatialDim+1]*elemNodeCoords[i_X_spatialDim+2];

    J[6] += grad_vals[i_X_spatialDim+2]*elemNodeCoords[i_X_spatialDim+0];
    J[7] += grad_vals[i_X_spatialDim+2]*elemNodeCoords[i_X_spatialDim+1];
    J[8] += grad_vals[i_X_spatialDim+2]*elemNodeCoords[i_X_spatialDim+2];

    i_X_spatialDim += spatialDim;
  }
}

template<typename Scalar>
KERNEL_PREFIX void diffusionMatrix(const Scalar* elemNodeCoords,
                     Scalar* elem_mat)
{
  int len = numNodesPerElem * numNodesPerElem;
  const Scalar zero = 0;
  miniFE::fill(elem_mat, elem_mat+len, zero);

  Scalar gpts[numGaussPointsPerDim];
  Scalar gwts[numGaussPointsPerDim];

  gauss_pts(numGaussPointsPerDim, gpts, gwts);

  const Scalar k = 2.0;
  Scalar detJ = 0.0;

  Scalar grad_vals[numNodesPerElem*spatialDim];
  Scalar dpsidx[numNodesPerElem], dpsidy[numNodesPerElem], dpsidz[numNodesPerElem];

  Scalar J[spatialDim*spatialDim];
  Scalar invJ[spatialDim*spatialDim];

  //The following nested loop implements equations 3.4.5 and 3.4.7 on page 88
  //of Reddy & Gartling, "The Finite Element Method in Heat Transfer and Fluid
  //Dynamics", 2nd edition,
  //to compute the element diffusion matrix for the steady conduction equation.

  Scalar pt[spatialDim];

#ifdef MINIFE_DEBUG
  Scalar volume = zero;
#endif

  for(size_t ig=0; ig<numGaussPointsPerDim; ++ig) {
    pt[0] = gpts[ig];
    Scalar wi = gwts[ig];

    for(size_t jg=0; jg<numGaussPointsPerDim; ++jg) {
      pt[1] = gpts[jg];
      Scalar wi_wj = wi*gwts[jg];

      for(size_t kg=0; kg<numGaussPointsPerDim; ++kg) {
        pt[2] = gpts[kg];
        Scalar wi_wj_wk = wi_wj*gwts[kg];
    
        gradients_and_jacobian(pt, elemNodeCoords, grad_vals, J);

        inverse_and_determinant3x3(J, invJ, detJ);
#ifdef MINIFE_DEBUG
        volume += detJ;
#endif
        Scalar k_detJ_wi_wj_wk = k*detJ*wi_wj_wk;

        for(int i=0; i<numNodesPerElem; ++i) {
          dpsidx[i] = grad_vals[i*3+0] * invJ[0] +
                      grad_vals[i*3+1] * invJ[1] +
                      grad_vals[i*3+2] * invJ[2];
          dpsidy[i] = grad_vals[i*3+0] * invJ[3] +
                      grad_vals[i*3+1] * invJ[4] +
                      grad_vals[i*3+2] * invJ[5];
          dpsidz[i] = grad_vals[i*3+0] * invJ[6] +
                      grad_vals[i*3+1] * invJ[7] +
                      grad_vals[i*3+2] * invJ[8];
        }

        for(int m=0; m<numNodesPerElem; ++m) {
          for(int n=0; n<numNodesPerElem; ++n) {
            elem_mat[m*numNodesPerElem+n] += k_detJ_wi_wj_wk *
                                  ((dpsidx[m] * dpsidx[n]) +
                                   (dpsidy[m] * dpsidy[n]) +
                                   (dpsidz[m] * dpsidz[n]));
          }
        }

      }//for kg
    }//for jg
  }//for ig

#ifdef MINIFE_DEBUG
//  std::cout << "element volume: " << volume << std::endl;
  if (std::abs(volume - 1) > 1.e-7) {
    std::cout << "element volume is "<<volume<<", expected 1.0."<<std::endl;
  }
#endif
//debugging: print out the element-matrix.
//  std::cout << "elem_mat:"<<std::endl;
//  for(int i=0; i<numNodesPerElem; ++i) {
//    for(int j=0; j<numNodesPerElem; ++j) {
//      std::cout << elem_mat[i*numNodesPerElem+j] << " ";
//    }
//    std::cout<<std::endl;
//  }
//  std::cout << "detJ_total: " << detJ_total << std::endl;
}

template<typename Scalar>
KERNEL_PREFIX void sourceVector(const Scalar* elemNodeCoords,
                  Scalar* elem_vec)
{
  int len = numNodesPerElem;
  const Scalar zero = 0;
  miniFE::fill(elem_vec, elem_vec+len, zero);

  Scalar gpts[numGaussPointsPerDim];
  Scalar gwts[numGaussPointsPerDim];

  Scalar psi[numNodesPerElem];
  Scalar grad_vals[numNodesPerElem*spatialDim];

  Scalar J[spatialDim*spatialDim];

  gauss_pts(numGaussPointsPerDim, gpts, gwts);

  Scalar Q = 1.0;

  Scalar pt[spatialDim];

  for(size_t ig=0; ig<numGaussPointsPerDim; ++ig) {
    pt[0] = gpts[ig];
    Scalar wi = gwts[ig];

    for(size_t jg=0; jg<numGaussPointsPerDim; ++jg) {
      pt[1] = gpts[jg];
      Scalar wj = gwts[jg];

      for(size_t kg=0; kg<numGaussPointsPerDim; ++kg) {
        pt[2] = gpts[kg];
        Scalar wk = gwts[kg];
    
        shape_fns(pt, psi);
        gradients_and_jacobian(pt, elemNodeCoords, grad_vals, J);
        Scalar detJ = determinant3x3(J);
    
        Scalar term = Q*detJ*wi*wj*wk;

        for(int i=0; i<numNodesPerElem; ++i) {
          elem_vec[i] += psi[i]*term;
        }
      }
    }
  }
}

}//namespace Hex8

}//namespace miniFE

#endif

