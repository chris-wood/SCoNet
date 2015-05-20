/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil -*- */
/*
 * Copyright (c) 2013, Regents of the University of California
 *                     Alexander Afanasyev
 *
 * GNU v3.0 license, See the LICENSE file for more information
 *
 * Author: Alexander Afanasyev <alexander.afanasyev@ucla.edu>
 */

#ifndef CCN_CCNCXX_DETAIL_REGISTERED_PREFIX_CONTAINER_H
#define CCN_CCNCXX_DETAIL_REGISTERED_PREFIX_CONTAINER_H

#include <ns3/ccnSIM/utils/trie/trie-with-policy.h>
#include <ns3/ccnSIM/utils/trie/counting-policy.h>

namespace ns3 {
namespace ccn {
namespace detail {

struct RegisteredPrefixEntry : public SimpleRefCount< RegisteredPrefixEntry >
{
public:
  RegisteredPrefixEntry (Ptr<const Name> prefix)
    : m_prefix (prefix)
  { }
  
  void
  AddCallback (ApiFace::InterestCallback callback)
  { 
    m_callback = callback;
  }

  void
  ClearCallback ()
  {
    m_callback = ApiFace::InterestCallback ();
  }

  Ptr<const Name>
  GetPrefix () const
  {
    return m_prefix;
  }
  
public:
  ApiFace::InterestCallback m_callback;
  Ptr<const Name> m_prefix;
};


struct RegisteredPrefixContainer :
    public ccnSIM::trie_with_policy<Name,
                                    ccnSIM::smart_pointer_payload_traits< RegisteredPrefixEntry >,
                                    ccnSIM::counting_policy_traits>
{
};

} // detail
} // ccn
} // ns3

#endif // CCN_CCNCXX_DETAIL_REGISTERED_PREFIX_CONTAINER_H
