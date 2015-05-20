/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2011,2012 University of California, Los Angeles
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation;
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * Author: Alexander Afanasyev <alexander.afanasyev@ucla.edu>
 *         Ilya Moiseenko <iliamo@cs.ucla.edu>
 *
 * Modification Author:  James Mathewson <jlmathew@soe.ucsc.edu>
 *
 */

#ifndef CCN_CONTENT_STORE_H
#define	CCN_CONTENT_STORE_H

#include "ns3/object.h"
#include "ns3/ptr.h"
#include "ns3/traced-callback.h"
#include "ns3/ccnSIM/model/cs/utility-caching/UtilityFunctions.h"
#include "ns3/ccnSIM/model/cs/utility-caching/UtilityFunctionGenerator.h"
#include "ns3/pugixml.h"
#include "ns3/node.h"
#include "ns3/ccn-content-storage.h"
#include <ns3/ccnSIM/ccn.cxx/name.h>
#include "ns3/ccnSIM/model/cs/utility-caching/UtilityFunctionsAggregate.h"

namespace ns3 {

class Packet;

namespace ccn {

class Data;
class Interest;
class InterestResponse;



/**
 * @ingroup ccn-cs
 * \brief Base class for CCN content store
 *
 * Particular implementations should implement Lookup, Add, and Print methods
 */
class ContentStore : public Object
{
public:
  /**
   * \brief Interface ID
   *
   * \return interface ID
   */
  static
  TypeId GetTypeId ();

  /**
   * @brief Virtual destructor
   */
  virtual
  ~ContentStore ();

  /**
   * \brief Find corresponding CS entry for the given interest
   *
   * \param interest Interest for which matching content store entry
   * will be searched
   *
   * If an entry is found, it is promoted to the top of most recent
   * used entries index, \see m_contentStore
   */
  virtual Ptr<Data>
  Lookup (Ptr< Interest> interest);

  virtual void
  Process (Ptr< InterestResponse > interestResponse);
  
  //virtual void
  //ProcessResponse (Ptr< InterestResponse > interestResponse);

  /**
   * \brief Add a new content to the content store.
   *
   * \param header Fully parsed Data
   * \param packet Fully formed Ccn packet to add to content store
   * (will be copied and stripped down of headers)
   * @returns true if an existing entry was updated, false otherwise
   */
  virtual bool
  Add (Ptr< Data> data, bool updateValue = false);

  // /*
  //  * \brief Add a new content to the content store.
  //  *
  //  * \param header Interest header for which an entry should be removed
  //  * @returns true if an existing entry was removed, false otherwise
  //  */
  // virtual bool
  // Remove (Ptr<Interest> header);

  /**
   * \brief Print out content store entries
   */
  virtual void
  Print (std::ostream &os) const;
  virtual void
  Print (std::ostream &os, const std::string &name) const;
  virtual void
  Print (std::ostream &os, Ptr< Data> data) const;

/* Implement when BlockUtility is ready
  virtual void
  PrintStats (std::ostream &os) const;
  virtual void
  PrintStats (std::ostream &os, std::string &name) const;
  virtual void
  PrintStats (std::ostream &os, Ptr< Data> data) const;
*/
  /**
   * @brief Get number of entries in content store
   */
  virtual uint32_t
  GetSize () const;


  ////////////////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////////////////

  /**
   * @brief Static call to cheat python bindings
   */
  static inline Ptr<ContentStore>
  GetContentStore (Ptr<Object> node);
  void
  ConfigureCache(std::string xmlConfigFilename);
  void
  ClearCache();
  void
  ClearUtilitiesInCache();
static std::string
GetLogName ();

protected:
  //legacy
  TracedCallback<Ptr< Interest>,
                 Ptr< Data> , ns3::Time > m_cacheHitsTrace; ///< @brief trace of cache hits

  TracedCallback< Ptr< Interest>, ns3::Time > m_cacheMissesTrace; ///< @brief trace of cache misses
//TODO FIXME need time to respond timers
 
  //ns3::icnx::utility::UtilityFunctionBase * m_cacheManagement; 
  ns3::icnx::utility::UtilityBlock * m_cacheManagement; 
  //for now FIXME TODO we manage deletions here
  uint64_t m_size, m_maxSize;
  double m_purgeValue;
  CcnStorage *m_contentStorage; //std::map < std::string, Ptr< Data> > m_cacheStorageMap;
};

inline std::ostream&
operator<< (std::ostream &os, const ContentStore &cs)
{
  cs.Print (os);
  return os;
}

inline Ptr<ContentStore>
ContentStore::GetContentStore (Ptr<Object> node)
{
  return node->GetObject<ContentStore> ();
}


} // namespace ccn
} // namespace ns3

#endif // CCN_CONTENT_STORE_H
