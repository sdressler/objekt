/*------------------------------------------------------------------------*/
/*                 Copyright 2010, 2011 Sandia Corporation.                     */
/*  Under terms of Contract DE-AC04-94AL85000, there is a non-exclusive   */
/*  license for use of this work by or on behalf of the U.S. Government.  */
/*  Export of this program may require a license from the                 */
/*  United States Government.                                             */
/*------------------------------------------------------------------------*/

#ifndef stk_mesh_EntityImpl_hpp
#define stk_mesh_EntityImpl_hpp

#include <stk_util/util/PairIter.hpp>

#include <stk_util/environment/ReportHandler.hpp>

#include <stk_mesh/base/Types.hpp>
#include <stk_mesh/base/Relation.hpp>
#include <stk_mesh/base/Trace.hpp>

#include <algorithm>

namespace stk {
namespace mesh {
namespace impl {

/** \addtogroup stk_mesh_module
 * \{
 */

class EntityImpl {
public:

  ~EntityImpl();
  EntityImpl( const EntityKey & arg_key );

  // Exposed in external interface:
  EntityRank entity_rank() const { return stk::mesh::entity_rank( m_key ); }
  EntityId identifier() const { return stk::mesh::entity_id( m_key ); }
  const EntityKey & key() const { return m_key ; }
  PairIterRelation relations() const { return PairIterRelation(m_relation); }
  PairIterRelation relations( unsigned rank ) const ;
  PairIterRelation node_relations( ) const
  {
    std::vector<Relation>::const_iterator i = m_relation.begin();
    std::vector<Relation>::const_iterator e = m_relation.end();
  
    const Relation::raw_relation_id_type hi = Relation::raw_relation_id(1, 0);
    e = std::lower_bound( i , e , hi , LessRelation() );
  
    return PairIterRelation( i , e );
  }

  PairIterEntityComm comm() const { return PairIterEntityComm( m_comm ); }
  PairIterEntityComm sharing() const ;
  PairIterEntityComm comm( const Ghosting & sub ) const ;

  Bucket & bucket() const
  {
    ThrowAssert(m_bucket); //don't want to return a reference to a null bucket
    return *m_bucket ;
  }

  Bucket* bucket_ptr() const
  {
    return m_bucket; // allow for NULL return value
  }

  bool is_bucket_valid() const { return m_bucket != NULL; }
  unsigned bucket_ordinal() const { return m_bucket_ord ; }
  unsigned owner_rank() const { return m_owner_rank ; }
  size_t synchronized_count() const { return m_sync_count ; }

  // The two relation methods below need to be called symmetically, ideally
  // through EntityRepository which will enforce the symmetry.

  bool destroy_relation( Entity & e_to, const RelationIdentifier local_id);
  bool declare_relation( Entity & e_to,
                         const RelationIdentifier local_id,
                         unsigned sync_count,
                         bool is_back_relation = false);

  // Communication info access:
  bool insert( const EntityCommInfo & );
  bool erase(  const EntityCommInfo & ); ///< Erase this entry
  bool erase(  const Ghosting & );       ///< Erase this ghosting info.
  void comm_clear_ghosting(); ///< Clear ghosting
  void comm_clear(); ///< Clear everything

  void set_bucket_and_ordinal( Bucket * in_bucket, unsigned ordinal )
  {
    TraceIfWatching("stk::mesh::impl::EntityRepository::set_bucket_and_ordinal", LOG_ENTITY, key());

    m_bucket = in_bucket;
    m_bucket_ord = ordinal;
  }

  // return true if entity was actually modified
  bool set_owner_rank( unsigned in_owner_rank )
  {
    TraceIfWatching("stk::mesh::impl::EntityRepository::set_owner_rank", LOG_ENTITY, key());

    if ( in_owner_rank != m_owner_rank ) {
      m_owner_rank = in_owner_rank;
      return true;
    }
    return false;
  }

  void set_sync_count( size_t sync_count )
  {
    TraceIfWatching("stk::mesh::impl::EntityRepository::set_sync_count", LOG_ENTITY, key());

    m_sync_count = sync_count;
  }

  // Change log access:
  EntityModificationLog log_query() const { return m_mod_log ; }

  void log_clear()
  {
    TraceIfWatching("stk::mesh::impl::EntityRepository::log_clear", LOG_ENTITY, key());

    m_mod_log = EntityLogNoChange;
  }

  void log_deleted()
  {
    TraceIfWatching("stk::mesh::impl::EntityRepository::log_deleted", LOG_ENTITY, key());

    m_mod_log = EntityLogDeleted;
  }

  /**
   * Takes an entity that has been marked for deletion and reactivates it. IE
   * takes an entity in the deleted state and changes it to modified.
   */
  void log_resurrect();

  /**
   * Mark this entity as modified (only changes from EntityLogNoChange
   * to EntityLogModified). Propagates the modification to higher-ranking
   * entities related to this entity. In other words, based on our
   * modification model, all entities that have modified_entity in their
   * closure must also be marked as modified.
   */
  void log_modified_and_propagate();

  /** \brief  Log that this entity was created as a parallel copy. */
  void log_created_parallel_copy();

  bool marked_for_destruction() const;

 private:

  const EntityKey         m_key ;        ///< Globally unique key
  RelationVector          m_relation ;   ///< This entity's relationships
  EntityCommInfoVector    m_comm ;       ///< This entity's communications
  Bucket                * m_bucket ;     ///< Bucket for the entity's field data
  unsigned                m_bucket_ord ; ///< Ordinal within the bucket
  unsigned                m_owner_rank ; ///< Owner processors' rank
  size_t                  m_sync_count ; ///< Last membership change
  EntityModificationLog   m_mod_log ;

  EntityImpl();
  EntityImpl( const EntityImpl & ); ///< Copy constructor not allowed
  EntityImpl & operator = ( const EntityImpl & ); ///< Assignment operator not allowed
};

//----------------------------------------------------------------------

/** \} */

} // namespace impl
} // namespace mesh
} // namespace stk

//----------------------------------------------------------------------
//----------------------------------------------------------------------

#endif /* stk_mesh_EntityImpl_hpp */
