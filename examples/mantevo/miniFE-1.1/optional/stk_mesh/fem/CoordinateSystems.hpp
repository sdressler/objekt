/*------------------------------------------------------------------------*/
/*                 Copyright 2010 Sandia Corporation.                     */
/*  Under terms of Contract DE-AC04-94AL85000, there is a non-exclusive   */
/*  license for use of this work by or on behalf of the U.S. Government.  */
/*  Export of this program may require a license from the                 */
/*  United States Government.                                             */
/*------------------------------------------------------------------------*/


#ifndef stk_mesh_fem_CoordinateSystems_hpp
#define stk_mesh_fem_CoordinateSystems_hpp

//----------------------------------------------------------------------

#include <Shards_Array.hpp>

namespace stk {
namespace mesh {

/** \addtogroup stk_mesh_field_dimension_tags
 *  \{
 */

/**
 *   \brief Implement an shards::ArrayDimTag for Cartesian coordinate dimensions.
 *
 *   A Cartesian coordinate has up to three dimensions in X, Y, Z order.
 */
struct Cartesian : public shards::ArrayDimTag {

  enum { Size = 3 };                    ///< default size

  enum { X = 0 , Y = 1 , Z = 2 };       ///< Identifiers for each dimension

  const char * name() const ;
  std::string to_string( size_type size , size_type index ) const ;
  size_type    to_index(  size_type size , const std::string & ) const ;
  static const Cartesian & tag();       ///< Singleton

private:
  Cartesian() {}
  Cartesian( const Cartesian & );
  Cartesian & operator = ( const Cartesian & );
};

/**
 *   \brief Implement an shards::ArrayDimTag for Cylindrical coordinate dimensions.
 *
 *   A Cylindral coordinate has up to three dimensions in
 *   radius, angle, and longitudinal-distance order.
 */
struct Cylindrical : public shards::ArrayDimTag {

  enum { Radius = 0 , R = 0 ,           ///< Identifiers for each dimension
         Angle = 1 ,  A = 1 ,
         Z = 2 };

  const char * name() const ;
  std::string to_string( size_type size , size_type index ) const ;
  size_type    to_index(  size_type size , const std::string & ) const ;
  static const Cylindrical & tag(); ///< Singleton

private:
  Cylindrical() {}
  Cylindrical( const Cylindrical & );
  Cylindrical & operator = ( const Cylindrical & );
};

/**
 *  \brief Implement an shards::ArrayDimTag for FullTensor.
 *
 * \todo REFACTOR  Where should FullTensor live, in the application,
 *                 in the toolkit or a common application header?
 */
struct FullTensor : public shards::ArrayDimTag {

  enum { Size = 9 };

  enum { XX = 0 , XY = 3 , XZ = 6 ,
         YX = 1 , YY = 4 , YZ = 7 ,
         ZX = 2 , ZY = 5 , ZZ = 8 };

  const char * name() const ;
  std::string to_string( size_type, size_type) const  ;
  size_type    to_index(  size_type, const std::string & ) const  ;
  static const FullTensor & tag(); ///< Singleton

private:
  FullTensor() {}
  FullTensor( const FullTensor & );
  FullTensor & operator = ( const FullTensor & );
};

//----------------------------------------------------------------------

/**
 *  \brief Implement an shards::ArrayDimTag for SymmetricTensor.
 *
 * \todo REFACTOR  Where should SymmetricTensor live, in the application,
 *                 in the toolkit or a common application header?
 */
struct SymmetricTensor : public shards::ArrayDimTag {

  enum { Size = 6 };

  enum { XX = 0 , YY = 1 , ZZ = 2, XY = 3, YZ = 4, XZ = 5 };

  const char * name() const  ;
  std::string to_string( size_type, size_type) const  ;
  size_type    to_index(  size_type , const std::string & ) const ;
  static const SymmetricTensor & tag(); ///< Singleton

private:
  SymmetricTensor() {}
  SymmetricTensor( const SymmetricTensor & );
  SymmetricTensor & operator = ( const SymmetricTensor & );
};

//----------------------------------------------------------------------

/** \} */

} //namespace mesh
} //namespace stk

#endif //stk_mesh_fem_CoordinateSystems_hpp

