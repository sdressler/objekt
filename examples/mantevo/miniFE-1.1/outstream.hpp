#ifndef _outstream_hpp_
#define _outstream_hpp_

#include <fstream>
#include <sstream>

inline
std::ostream& outstream(int np=1, int p=0)
{
  static bool first = true;
  static std::ostringstream oss;
  if (first) oss << "minife_debug."<<np<<"."<<p;
  static std::ofstream ofs(oss.str().c_str(), std::ios::out);
  first = false;
  return ofs;
}

#endif
