#ifndef _SparseMatrix_functions_hpp_
#define _SparseMatrix_functions_hpp_

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

#include <cstddef>
#include <vector>
#include <set>
#include <algorithm>
#include <sstream>
#include <fstream>

#include <Vector.hpp>
#include <Vector_functions.hpp>
#include <ElemData.hpp>
#include <SparseMatrix.hpp>
#include <FusedMatvecDotOp.hpp>
#include <MatvecOp.hpp>
#include <MatrixInitOp.hpp>
#include <MatrixCopyOp.hpp>
#include <exchange_externals.hpp>
#include <mytimer.hpp>

#ifdef MINIFE_HAVE_TBB
#include <LockingMatrix.hpp>
#endif

#ifdef HAVE_MPI
#include <mpi.h>
#endif

namespace miniFE {

template<typename MatrixType>
void init_matrix(MatrixType& M,
                 const std::vector<typename MatrixType::GlobalOrdinalType>& rows,
                 const std::vector<typename MatrixType::LocalOrdinalType>& row_offsets,
                 const std::vector<int>& row_coords,
                 int global_nodes_x,
                 int global_nodes_y,
                 int global_nodes_z,
                 typename MatrixType::GlobalOrdinalType global_nrows,
                 const simple_mesh_description<typename MatrixType::GlobalOrdinalType>& mesh)
{
  MatrixInitOp<MatrixType> mat_init;
  mat_init.rows = &rows[0];
  mat_init.row_offsets = &row_offsets[0];
  mat_init.row_coords = &row_coords[0];
  mat_init.global_nodes_x = global_nodes_x;
  mat_init.global_nodes_y = global_nodes_y;
  mat_init.global_nodes_z = global_nodes_z;
  mat_init.global_nrows = global_nrows;
  mat_init.dest_rows = &M.rows[0];
  mat_init.dest_rowoffsets = &M.row_offsets[0];
  mat_init.dest_cols = &M.packed_cols[0];
  mat_init.dest_coefs = &M.packed_coefs[0];
  mat_init.n = M.rows.size();
  mat_init.mesh = &mesh;
#ifdef MINIFE_HAVE_CUDA
  for(size_t i=0; i<mat_init.n; ++i) {
    mat_init(i);
  }
#else
  M.compute_node.parallel_for(mat_init.n, mat_init);
#endif
  M.row_offsets[mat_init.n] = row_offsets[mat_init.n];
}

template<typename MatrixType>
void copy_matrix(const MatrixType& src, MatrixType& dest)
{
  dest.has_local_indices = src.has_local_indices;
  dest.rows.resize(src.rows.size());
  dest.row_offsets.resize(src.row_offsets.size());
  dest.row_offsets_external = src.row_offsets_external;
  dest.packed_cols.resize(src.packed_cols.size());
  dest.packed_coefs.resize(src.packed_cols.size());
  dest.num_cols = src.num_cols;
#ifdef HAVE_MPI
  dest.external_index = src.external_index;
  dest.external_local_index = src.external_local_index;
  dest.elements_to_send = src.elements_to_send;
  dest.neighbors = src.neighbors;
  dest.recv_length = src.recv_length;
  dest.send_length = src.send_length;
  dest.send_buffer = src.send_buffer;
  dest.request = src.request;
#endif
  MatrixCopyOp<MatrixType> mat_copy;
  mat_copy.src_rows = &src.rows[0];
  mat_copy.src_rowoffsets = &src.row_offsets[0];
  mat_copy.src_cols = &src.packed_cols[0];
  mat_copy.src_coefs = &src.packed_coefs[0];
  mat_copy.dest_rows = &dest.rows[0];
  mat_copy.dest_rowoffsets = &dest.row_offsets[0];
  mat_copy.dest_cols = &dest.packed_cols[0];
  mat_copy.dest_coefs = &dest.packed_coefs[0];
  mat_copy.n = src.rows.size();
#ifdef MINIFE_HAVE_CUDA
  for(size_t i=0; i<mat_copy.n; ++i) {
    mat_copy(i);
  }
#else
  src.compute_node.parallel_for(mat_copy.n, mat_copy);
#endif
  dest.row_offsets[mat_copy.n] = src.row_offsets[mat_copy.n];
}

template<typename T,
         typename U>
void sort_with_companions(ptrdiff_t len, T* array, U* companions)
{
  ptrdiff_t i, j, index;
  U companion;

  for (i=1; i < len; i++) {
    index = array[i];
    companion = companions[i];
    j = i;
    while ((j > 0) && (array[j-1] > index))
    {
      array[j] = array[j-1];
      companions[j] = companions[j-1];
      j = j - 1;
    }
    array[j] = index;
    companions[j] = companion;
  }
}

template<typename MatrixType>
void write_matrix(const std::string& filename, 
                  MatrixType& mat)
{
  int numprocs = 1, myproc = 0;
#ifdef HAVE_MPI
  MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
  MPI_Comm_rank(MPI_COMM_WORLD, &myproc);
#endif

  std::ostringstream osstr;
  osstr << filename << "." << numprocs << "." << myproc;
  std::string full_name = osstr.str();
  std::ofstream ofs(full_name.c_str());

  size_t nrows = mat.rows.size();
  size_t nnz = mat.packed_coefs.size();

  for(int p=0; p<numprocs; ++p) {
    if (p == myproc) {
      if (p == 0) {
        ofs << nrows << " " << nnz << std::endl;
      }
      for(size_t i=0; i<nrows; ++i) {
        for(size_t j=mat.row_offsets[i]; j<mat.row_offsets[i+1]; ++j) {
          ofs << mat.rows[i] << " " << mat.packed_cols[j] << " " << mat.packed_coefs[j] << std::endl;
        }
      }
    }
#ifdef HAVE_MPI
    MPI_Barrier(MPI_COMM_WORLD);
#endif
  }
}

template<typename MatrixType>
typename MatrixType::ScalarType
row_sum(const MatrixType& mat,
        typename MatrixType::GlobalOrdinalType row)
{
  typedef typename MatrixType::GlobalOrdinalType GlobalOrdinal;
  typedef typename MatrixType::ScalarType Scalar;

  typename std::vector<GlobalOrdinal>::const_iterator row_iter =
      std::lower_bound(mat.rows.begin(), mat.rows.end(), row);

  if (row_iter == mat.rows.end() || *row_iter != row) {
    return 0;
  }

  ptrdiff_t local_row = row_iter - mat.rows.begin();
  ptrdiff_t row_len = mat.row_offsets[local_row+1]-mat.row_offsets[local_row];

  const Scalar* mat_row_coefs = &mat.packed_coefs[mat.row_offsets[local_row]];

  Scalar return_val = 0;
  for(ptrdiff_t i=0; i<row_len; ++i) {
    return_val += std::abs(mat_row_coefs[i]);
  }
  return return_val;
}

template<typename MatrixType>
void
sum_into_row(typename MatrixType::GlobalOrdinalType row,
             size_t num_indices,
             const typename MatrixType::GlobalOrdinalType* col_inds,
             const typename MatrixType::ScalarType* coefs,
             MatrixType& mat)
{
  typedef typename MatrixType::GlobalOrdinalType GlobalOrdinal;
  typedef typename MatrixType::ScalarType Scalar;

  ptrdiff_t local_row = -1;
  //first see if we can get the local-row index using fast direct lookup:
  if (mat.rows.size()>=1) {
    ptrdiff_t idx = row - mat.rows[0];
    if (idx < mat.rows.size() && mat.rows[idx] == row) {
      local_row = idx;
    }
  }

  //if we didn't get the local-row index using direct lookup, try a
  //more expensive binary-search:
  if (local_row == -1) {
    typename std::vector<GlobalOrdinal>::iterator row_iter =
        std::lower_bound(mat.rows.begin(), mat.rows.end(), row);

    //if we still haven't found row, it's not local so jump out:
    if (row_iter == mat.rows.end() || *row_iter != row) {
      return;
    }

    local_row = row_iter - mat.rows.begin();
  }

  ptrdiff_t row_len = mat.row_offsets[local_row+1]-mat.row_offsets[local_row];

  GlobalOrdinal* mat_row_cols = &mat.packed_cols[mat.row_offsets[local_row]];
  Scalar* mat_row_coefs = &mat.packed_coefs[mat.row_offsets[local_row]];

  for(size_t i=0; i<num_indices; ++i) {
    GlobalOrdinal* loc = std::lower_bound(mat_row_cols, mat_row_cols+row_len,
                                          col_inds[i]);
    if (loc-mat_row_cols < row_len && *loc == col_inds[i]) {
      mat_row_coefs[loc-mat_row_cols] += coefs[i];
    }
  }
}

                  
template<typename MatrixType>
void
sum_in_elem_matrix(size_t num,
                   const typename MatrixType::GlobalOrdinalType* indices,
                   const typename MatrixType::ScalarType* coefs,
                   MatrixType& mat)
{
  size_t offset = 0;

  for(size_t i=0; i<num; ++i) {
    sum_into_row(indices[i], num,
                 &indices[0], &coefs[offset], mat);
    offset += num;
  }
}

template<typename GlobalOrdinal, typename Scalar,
         typename MatrixType, typename VectorType>
void
sum_into_global_linear_system(ElemData<GlobalOrdinal,Scalar>& elem_data,
                              MatrixType& A, VectorType& b)
{
  sum_in_elem_matrix(elem_data.nodes_per_elem, elem_data.elem_node_ids,
                     elem_data.elem_diffusion_matrix, A);
  sum_into_vector(elem_data.nodes_per_elem, elem_data.elem_node_ids,
                  elem_data.elem_source_vector, b);
}

#ifdef MINIFE_HAVE_TBB
template<typename MatrixType>
void
sum_in_elem_matrix(size_t num,
                   const typename MatrixType::GlobalOrdinalType* indices,
                   const typename MatrixType::ScalarType* coefs,
                   LockingMatrix<MatrixType>& mat)
{
  size_t offset = 0;

  for(size_t i=0; i<num; ++i) {
    mat.sum_in(indices[i], num, &indices[0], &coefs[offset]);
    offset += num;
  }
}

template<typename GlobalOrdinal, typename Scalar,
         typename MatrixType, typename VectorType>
void
sum_into_global_linear_system(ElemData<GlobalOrdinal,Scalar>& elem_data,
                              LockingMatrix<MatrixType>& A, LockingVector<VectorType>& b)
{
  sum_in_elem_matrix(elem_data.nodes_per_elem, elem_data.elem_node_ids,
                     elem_data.elem_diffusion_matrix, A);
  sum_into_vector(elem_data.nodes_per_elem, elem_data.elem_node_ids,
                  elem_data.elem_source_vector, b);
}
#endif

template<typename MatrixType>
void
add_to_diagonal(typename MatrixType::ScalarType value, MatrixType& mat)
{
  for(size_t i=0; i<mat.rows.size(); ++i) {
    sum_into_row(mat.rows[i], 1, &mat.rows[i], &value, mat);
  }
}

template<typename MatrixType>
double
parallel_memory_overhead_MB(const MatrixType& A)
{
  typedef typename MatrixType::GlobalOrdinalType GlobalOrdinal;
  typedef typename MatrixType::LocalOrdinalType LocalOrdinal;
  double mem_MB = 0;

#ifdef HAVE_MPI
  double invMB = 1.0/(1024*1024);
  mem_MB = invMB*A.external_index.size()*sizeof(GlobalOrdinal);
  mem_MB += invMB*A.external_local_index.size()*sizeof(GlobalOrdinal);
  mem_MB += invMB*A.elements_to_send.size()*sizeof(GlobalOrdinal);
  mem_MB += invMB*A.neighbors.size()*sizeof(int);
  mem_MB += invMB*A.recv_length.size()*sizeof(LocalOrdinal);
  mem_MB += invMB*A.send_length.size()*sizeof(LocalOrdinal);

  double tmp = mem_MB;
  MPI_Allreduce(&tmp, &mem_MB, 1, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);
#endif

  return mem_MB;
}

template<typename MatrixType>
void rearrange_matrix_local_external(MatrixType& A)
{
  //This function will rearrange A so that local entries are contiguous at the front
  //of A's memory, and external entries are contiguous at the back of A's memory.
  //
  //A.row_offsets will describe where the local entries occur, and
  //A.row_offsets_external will describe where the external entries occur.

  typedef typename MatrixType::GlobalOrdinalType GlobalOrdinal;
  typedef typename MatrixType::LocalOrdinalType LocalOrdinal;
  typedef typename MatrixType::ScalarType Scalar;

  size_t nrows = A.rows.size();
  std::vector<LocalOrdinal> tmp_row_offsets(nrows*2);
  std::vector<LocalOrdinal> tmp_row_offsets_external(nrows*2);

  LocalOrdinal num_local_nz = 0;
  LocalOrdinal num_extern_nz = 0;

  //First sort within each row of A, so that local entries come
  //before external entries within each row.
  //tmp_row_offsets describe the locations of the local entries, and
  //tmp_row_offsets_external describe the locations of the external entries.
  //
  for(size_t i=0; i<nrows; ++i) {
    GlobalOrdinal* row_begin = &A.packed_cols[A.row_offsets[i]];
    GlobalOrdinal* row_end = &A.packed_cols[A.row_offsets[i+1]];

    Scalar* coef_row_begin = &A.packed_coefs[A.row_offsets[i]];

    tmp_row_offsets[i*2] = A.row_offsets[i];
    tmp_row_offsets[i*2+1] = A.row_offsets[i+1];
    tmp_row_offsets_external[i*2] = A.row_offsets[i+1];
    tmp_row_offsets_external[i*2+1] = A.row_offsets[i+1];

    ptrdiff_t row_len = row_end - row_begin;

    sort_with_companions(row_len, row_begin, coef_row_begin);

    GlobalOrdinal* row_iter = std::lower_bound(row_begin, row_end, nrows);

    LocalOrdinal offset = A.row_offsets[i] + row_iter-row_begin;
    tmp_row_offsets[i*2+1] = offset;
    tmp_row_offsets_external[i*2] = offset;

    num_local_nz += tmp_row_offsets[i*2+1]-tmp_row_offsets[i*2];
    num_extern_nz += tmp_row_offsets_external[i*2+1]-tmp_row_offsets_external[i*2];
  }

  //Next, copy the external entries into separate arrays.

  std::vector<GlobalOrdinal> ext_cols(num_extern_nz);
  std::vector<Scalar> ext_coefs(num_extern_nz);
  std::vector<LocalOrdinal> ext_offsets(nrows+1);
  LocalOrdinal offset = 0;
  for(size_t i=0; i<nrows; ++i) {
    ext_offsets[i] = offset;
    for(LocalOrdinal j=tmp_row_offsets_external[i*2];
                     j<tmp_row_offsets_external[i*2+1]; ++j) {
      ext_cols[offset] = A.packed_cols[j];
      ext_coefs[offset++] = A.packed_coefs[j];
    }
  }
  ext_offsets[nrows] = offset;

  //Now slide all local entries down to the beginning of A's packed arrays

  A.row_offsets.resize(nrows+1);
  offset = 0;
  for(size_t i=0; i<nrows; ++i) {
    A.row_offsets[i] = offset;
    for(LocalOrdinal j=tmp_row_offsets[i*2]; j<tmp_row_offsets[i*2+1]; ++j) {
      A.packed_cols[offset] = A.packed_cols[j];
      A.packed_coefs[offset++] = A.packed_coefs[j];
    }
  }
  A.row_offsets[nrows] = offset;

  //Finally, copy the external entries back into A.packed_cols and
  //A.packed_coefs, starting at the end of the local entries.

  for(LocalOrdinal i=offset; i<offset+ext_cols.size(); ++i) {
    A.packed_cols[i] = ext_cols[i-offset];
    A.packed_coefs[i] = ext_coefs[i-offset];
  }

  A.row_offsets_external.resize(nrows+1);
  for(size_t i=0; i<=nrows; ++i) A.row_offsets_external[i] = ext_offsets[i] + offset;
}

//------------------------------------------------------------------------
template<typename MatrixType>
void
zero_row_and_put_1_on_diagonal(MatrixType& A, typename MatrixType::GlobalOrdinalType row)
{
  ptrdiff_t local_row = row - A.rows[0];
  if (local_row < 0 || local_row >= A.rows.size()) return;

  typedef typename MatrixType::GlobalOrdinalType GlobalOrdinal;
  typedef typename MatrixType::LocalOrdinalType LocalOrdinal;
  typedef typename MatrixType::ScalarType Scalar;

  LocalOrdinal offset = A.row_offsets[local_row];
  size_t row_len = A.row_offsets[local_row+1] - offset;

  GlobalOrdinal* cols = &A.packed_cols[offset];
  Scalar* coefs = &A.packed_coefs[offset];

  for(size_t i=0; i<row_len; ++i) {
    if (cols[i] == row) coefs[i] = 1;
    else coefs[i] = 0;
  }
}

//------------------------------------------------------------------------
template<typename MatrixType>
void
get_row_pointers(MatrixType& A,
                 typename MatrixType::GlobalOrdinalType row,
                 size_t& row_length,
                 typename MatrixType::GlobalOrdinalType*& cols,
                 typename MatrixType::ScalarType*& coefs)
{
  ptrdiff_t local_row = row - A.rows[0];
  if (local_row < 0 || local_row >= A.rows.size()) {
    row_length = 0;
    return;
  }

  typename MatrixType::LocalOrdinalType offset = A.row_offsets[local_row];

  row_length = A.row_offsets[local_row+1] - offset;
  cols = &A.packed_cols[offset];
  coefs = &A.packed_coefs[offset];
}

//------------------------------------------------------------------------
template<typename MatrixType,
         typename VectorType,
         typename MeshType>
void
impose_dirichlet_x0(typename MatrixType::ScalarType value,
                    MatrixType& A,
                    VectorType& b,
                    int global_nx,
                    int global_ny,
                    int global_nz,
                    const MeshType& mesh)
{
  typedef typename MatrixType::GlobalOrdinalType GlobalOrdinal;
  typedef typename MatrixType::LocalOrdinalType LocalOrdinal;
  typedef typename MatrixType::ScalarType Scalar;

  const std::set<GlobalOrdinal>& bc_rows = mesh.bc_rows;

  GlobalOrdinal first_local_row = A.rows.size()>0 ? A.rows[0] : 0;
  GlobalOrdinal last_local_row  = A.rows.size()>0 ? A.rows[A.rows.size()-1] : -1;

  typename std::set<GlobalOrdinal>::const_iterator
    bc_iter = bc_rows.begin(), bc_end = bc_rows.end();
  for(; bc_iter!=bc_end; ++bc_iter) {
    GlobalOrdinal row = *bc_iter;
    if (row >= first_local_row && row <= last_local_row) {
      size_t local_row = row - first_local_row;
      b.coefs[local_row] = value;
      zero_row_and_put_1_on_diagonal(A, row);
    }
  }

  for(size_t i=0; i<A.rows.size(); ++i) {
    GlobalOrdinal row = A.rows[i];

    if (bc_rows.find(row) != bc_rows.end()) continue;

    size_t row_length = 0;
    GlobalOrdinal* cols = NULL;
    Scalar* coefs = NULL;
    get_row_pointers(A, row, row_length, cols, coefs);

    Scalar sum = 0;
    for(size_t j=0; j<row_length; ++j) {
      if (bc_rows.find(cols[j]) != bc_rows.end()) {
        sum += coefs[j];
        coefs[j] = 0;
      }
    }

    b.coefs[i] -= sum*value;
  }
}

static timer_type exchtime = 0;

//------------------------------------------------------------------------
//Compute matrix vector product y = A*x and return dot(x,y), where:
//
// A - input matrix
// x - input vector
// y - result vector
//
template<typename MatrixType,
         typename VectorType>
typename TypeTraits<typename VectorType::ScalarType>::magnitude_type
matvec_and_dot(MatrixType& A,
               VectorType& x,
               VectorType& y)
{
  timer_type t0 = mytimer();
  exchange_externals(A, x);
  exchtime += mytimer()-t0;

  typedef typename TypeTraits<typename VectorType::ScalarType>::magnitude_type magnitude;
  typedef typename MatrixType::ScalarType ScalarType;
  typedef typename MatrixType::GlobalOrdinalType GlobalOrdinalType;
  typedef typename MatrixType::LocalOrdinalType LocalOrdinalType;
  typedef typename MatrixType::ComputeNodeType ComputeNodeType;

  ComputeNodeType& comp_node = A.compute_node;

  FusedMatvecDotOp<MatrixType,VectorType> mvdotop;

  mvdotop.n = A.rows.size();
  mvdotop.Arowoffsets = comp_node.get_buffer(&A.row_offsets[0], A.row_offsets.size());
  mvdotop.Acols       = comp_node.get_buffer(&A.packed_cols[0], A.packed_cols.size());
  mvdotop.Acoefs      = comp_node.get_buffer(&A.packed_coefs[0], A.packed_coefs.size());
  mvdotop.x = comp_node.get_buffer(&x.coefs[0], x.coefs.size());
  mvdotop.y = comp_node.get_buffer(&y.coefs[0], y.coefs.size());
  mvdotop.beta = 0;

  comp_node.parallel_reduce(mvdotop.n, mvdotop);

#ifdef HAVE_MPI
  magnitude local_dot = mvdotop.result, global_dot = 0;
  MPI_Datatype mpi_dtype = TypeTraits<magnitude>::mpi_type();  
  MPI_Allreduce(&local_dot, &global_dot, 1, mpi_dtype, MPI_SUM, MPI_COMM_WORLD);
  return global_dot;
#else
  return mvdotop.result;
#endif
}

//------------------------------------------------------------------------
//Compute matrix vector product y = A*x where:
//
// A - input matrix
// x - input vector
// y - result vector
//
template<typename MatrixType,
         typename VectorType>
struct matvec_std {
void operator()(MatrixType& A,
            VectorType& x,
            VectorType& y)
{
  exchange_externals(A, x);

  typedef typename MatrixType::ScalarType ScalarType;
  typedef typename MatrixType::GlobalOrdinalType GlobalOrdinalType;
  typedef typename MatrixType::LocalOrdinalType LocalOrdinalType;
  typedef typename MatrixType::ComputeNodeType ComputeNodeType;

  ComputeNodeType& comp_node = A.compute_node;

  MatvecOp<MatrixType,VectorType> mvop;

  mvop.n = A.rows.size();
  mvop.Arowoffsets = comp_node.get_buffer(&A.row_offsets[0], A.row_offsets.size());
  mvop.Acols       = comp_node.get_buffer(&A.packed_cols[0], A.packed_cols.size());
  mvop.Acoefs      = comp_node.get_buffer(&A.packed_coefs[0], A.packed_coefs.size());
  mvop.x = comp_node.get_buffer(&x.coefs[0], x.coefs.size());
  mvop.y = comp_node.get_buffer(&y.coefs[0], y.coefs.size());
  mvop.beta = 0;

  comp_node.parallel_for(mvop.n, mvop);
}
};

template<typename MatrixType,
         typename VectorType>
void matvec(MatrixType& A, VectorType& x, VectorType& y)
{
  matvec_std<MatrixType,VectorType> mv;
  mv(A, x, y);
}

template<typename MatrixType,
         typename VectorType>
struct matvec_overlap {
void operator()(MatrixType& A,
                    VectorType& x,
                    VectorType& y)
{
#ifdef HAVE_MPI
  begin_exchange_externals(A, x);
#endif

  typedef typename MatrixType::ScalarType ScalarType;
  typedef typename MatrixType::GlobalOrdinalType GlobalOrdinalType;
  typedef typename MatrixType::LocalOrdinalType LocalOrdinalType;
  typedef typename MatrixType::ComputeNodeType ComputeNodeType;

  ComputeNodeType& comp_node = A.compute_node;

  MatvecOp<MatrixType,VectorType> mvop;

  mvop.n = A.rows.size();
  mvop.Arowoffsets = comp_node.get_buffer(&A.row_offsets[0], A.row_offsets.size());
  mvop.Acols       = comp_node.get_buffer(&A.packed_cols[0], A.packed_cols.size());
  mvop.Acoefs      = comp_node.get_buffer(&A.packed_coefs[0], A.packed_coefs.size());
  mvop.x = comp_node.get_buffer(&x.coefs[0], x.coefs.size());
  mvop.y = comp_node.get_buffer(&y.coefs[0], y.coefs.size());
  mvop.beta = 0;

  comp_node.parallel_for(mvop.n, mvop);

#ifdef HAVE_MPI
  finish_exchange_externals(A.neighbors.size());

  mvop.Arowoffsets = comp_node.get_buffer(&A.row_offsets_external[0], A.row_offsets_external.size());
  mvop.beta = 1;

  comp_node.parallel_for(A.rows.size(), mvop);
#endif
}
};

}//namespace miniFE

#endif

