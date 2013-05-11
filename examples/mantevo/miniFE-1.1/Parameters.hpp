#ifndef _parameters_hpp_
#define _parameters_hpp_

#include <string>

namespace miniFE {

struct Parameters {
  Parameters()
   : nx(5), ny(nx), nz(nx), numthreads(1),
     mv_overlap_comm_comp(0), use_locking(0),
     load_imbalance(0), name(), elem_group_size(1),
     use_elem_mat_fields(1)
  {}

  int nx;
  int ny;
  int nz;
  int numthreads;
  int mv_overlap_comm_comp;
  int use_locking;
  float load_imbalance;
  std::string name;
  int elem_group_size;
  int use_elem_mat_fields;

  unsigned long size() { return sizeof(int) * 8 + sizeof(float) + name.size(); }
};//struct Parameters

}//namespace miniFE

#endif

