/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2011 University of California, Los Angeles
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
 * Author: Ilya Moiseenko <iliamo@cs.ucla.edu>
 *         Alexander Afanasyev <alexander.afanasyev@ucla.edu>
 */

#ifndef CCN_PRODUCER_H
#define CCN_PRODUCER_H

#include "ccn-app.h"

#include "ns3/ptr.h"
#include "ns3/ccn-name.h"
#include "ns3/ccn-data.h"

namespace ns3 {
namespace ccn {

/**
 * @ingroup ccn-apps
 * @brief A simple Interest-sink applia simple Interest-sink application
 *
 * A simple Interest-sink applia simple Interest-sink application,
 * which replying every incoming Interest with Data packet with a specified
 * size and name same as in Interest.cation, which replying every incoming Interest
 * with Data packet with a specified size and name same as in Interest.
 */
class Producer : public App
{
public:
  static TypeId
  GetTypeId (void);

  Producer ();

  // inherited from CcnApp
  void OnInterest (Ptr<const Interest> interest);

protected:
  // inherited from Application base class.
  virtual void
  StartApplication ();    // Called at time specified by Start

  virtual void
  StopApplication ();     // Called at time specified by Stop

  /**
  * \brief Check if the Interest's KeyID matches the content KeyID 
  * 
  */
  virtual bool
  KeyIdCheck(Ptr<const Packet> interestKeyId);

  /**
  * \brief Check if the Interest's Hash matches the content Hash 
  * 
  */
  virtual bool
  ObjectHashCheck(Ptr<const Packet> interestHash);
  /**
  * \brief Check the Name Nonce  
  * 
  */
  virtual bool
  NameNonceCheck(uint32_t interestNonce);
private:
  Name m_prefix;
  Name m_postfix;
  uint32_t m_virtualPayloadSize;
  Time m_freshness;

  uint32_t m_signature;
  Name m_keyLocator;

  Ptr<Packet> m_payload;
  Ptr<const Packet> m_packet;
  
  uint8_t m_type;
  bool m_nack;
  bool m_metaData;
  uint32_t m_nameNonce;
  uint8_t m_keyId;
  uint8_t m_objectHash;
  uint32_t m_keyIdSize;
  uint32_t m_objectHashSize;
};

} // namespace ccn
} // namespace ns3

#endif // CCN_PRODUCER_H
