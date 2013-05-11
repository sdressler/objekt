#ifndef _MatrixInitOp_hpp_
#define _MatrixInitOp_hpp_

#include <simple_mesh_description.hpp>
#include <box_utils.hpp>

#include <algorithm>

template<typename GlobalOrdinal>
void sort_if_needed(GlobalOrdinal* list,
                    GlobalOrdinal list_len)
{
  bool need_to_sort = false;
  for(GlobalOrdinal i=list_len-1; i>=1; --i) {
    if (list[i] < list[i-1]) {
      need_to_sort = true;
      break;
    }
  }

  if (need_to_sort) {
    std::sort(list,list+list_len);
  }
}

template<typename MatrixType>
struct MatrixInitOp {
  typedef typename MatrixType::GlobalOrdinalType GlobalOrdinalType;
  typedef typename MatrixType::LocalOrdinalType LocalOrdinalType;
  typedef typename MatrixType::ScalarType ScalarType;

  const GlobalOrdinalType* rows;
  const LocalOrdinalType*  row_offsets;
  const int*               row_coords;

  int global_nodes_x;
  int global_nodes_y;
  int global_nodes_z;

  GlobalOrdinalType global_nrows;

  GlobalOrdinalType* dest_rows;
  LocalOrdinalType*  dest_rowoffsets;
  GlobalOrdinalType* dest_cols;
  ScalarType*        dest_coefs;
  int n;

  const miniFE::simple_mesh_description<GlobalOrdinalType>* mesh;

  inline void operator()(int i)
  {
    dest_rows[i] = rows[i];
    int offset = row_offsets[i];
    dest_rowoffsets[i] = offset;
    int ix = row_coords[i*3];
    int iy = row_coords[i*3+1];
    int iz = row_coords[i*3+2];
    int nnz = 0;
    for(int sz=-1; sz<=1; ++sz)
      for(int sy=-1; sy<=1; ++sy)
        for(int sx=-1; sx<=1; ++sx) {
          GlobalOrdinalType col_id =
              miniFE::get_id<GlobalOrdinalType>(global_nodes_x, global_nodes_y, global_nodes_z,
                                   ix+sx, iy+sy, -iz-sz);
          if (col_id >= 0 && col_id < global_nrows) {
            GlobalOrdinalType col = mesh->map_id_to_row(col_id);
            dest_cols[offset+nnz] = col;
            dest_coefs[offset+nnz] = 0;
            ++nnz;
          }
        }

    sort_if_needed(&dest_cols[offset], nnz);
  }
};

#endif

