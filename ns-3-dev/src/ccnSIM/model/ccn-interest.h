/* -*- Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil -*- */
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
 *
 * Modification Author:
 *         James Mathewson <jlmathew@soe.ucsc.edu>
 *
 * Note:  ccn-interest was modified to use icnx vers 1.0, with TLV support.   While
 * most of the code base has been replaced or rewritten, the core used was from
 * the ccn implementation, and thus, proper author credit is observed.
 */

#ifndef _CCN_INTEREST_HEADER_H_
#define _CCN_INTEREST_HEADER_H_

#include "ns3/simple-ref-count.h"
#include "ns3/nstime.h"
#include "ns3/packet.h"
#include "ns3/ptr.h"

#include <ns3/ccnSIM/ccn.cxx/name.h>
#include <ns3/ccnSIM/ccn.cxx/exclude.h>
#include <ns3/ccnSIM/model/wire/icnx/ccn-1.0_tlv-helper-base.h>
#include <ns3/ccnSIM/model/wire/icnx/ccn-1.0_interest_tlv.h>
#include "ns3/ccn-common-object.h"

namespace ns3 {

class Packet;

namespace ccn {

/**
 * @ingroup ccn
 * @brief CCNX 1.0 Interest (wire formats are defined in wire)
 **/
class Interest : public SimpleRefCount<Interest>
{
public:

  /**
   * \brief Constructor
   *
   * Creates a null header
   **/
  Interest (ns3::Ptr<Packet> payload = 0);

  /**
   * @brief Copy constructor
   */
  Interest (const Interest &interest);
  Interest (::wire::icnx::TLV_INTEREST *tlv);

  /**
   * \brief Set interest name
   *
   * @param name smart pointer to Name
   *
   **/
   void
  SetName (ns3::Ptr<Name> name);

  /**
   * \brief Another variant to set interest name
   *
   * @param name const reference to Name object
   *
   **/
   void
  SetName (const Name &name);

  /**
   * \brief Another variant to set interest name
   *
   * @param name const string for Name
   *
   **/
   void
  SetName (const std::string &name);

   /**
   * @brief Get serialized wire format method
   *
   * Return the serialization method of the TLV packets (XML/Byte)
   */
  WireFormat 
  GetWireFormat () const;

  /**
   * @brief Set the serialized wire format method
   *
   * Return the serialization method of the TLV packets
   */
  void
  SetWireFormat(WireFormat format) const;

  
  /**
   * \brief Get interest name
   *
   * Gets name of the interest.
   **/
  const ns3::ccn::Name
  GetName () const;

  /**
   * \brief Get interest name
   *
   * Gets name of the interest as a string.
   **/
   std::string
  GetStringName () const;

  /**
   * @brief Get smart pointer to the interest name (to avoid extra memory usage)
   */
   ns3::Ptr<const Name>
  GetNamePtr () const;



  /**
   * \brief Set InterestLifetime
   * InterestLifetime indicates the (approximate) time remaining before the interest times out.
   * The timeout is relative to the arrival time of the interest at the current node.
   * @param[in] time interest lifetime  
   */ 
   void
  SetInterestLifetime (Time time);

  /**
   * \brief Get InterestLifetime value
   * InterestLifetime indicates the (approximate) time remaining before the interest times out.
   * The timeout is relative to the arrival time of the interest at the current node.
   * This is an optional field, and if it does not exist will return a default of 4s.
   */ 
   Time
  GetInterestLifetime () const;

  //additional functions requested by maziar
  /**
   * \brief Set InterestHopCount value
   * InterestHopCount measures how many hops maximum, from the consumer, before the
   * interest is discarded.
   */ 
   void
  SetInterestHopCount(uint8_t hops);

  /**
   * \brief Get InterestHopCount value
   * InterestHopCount measures how many hops maximum, from the consumer, before the
   * interest is discarded.
   */ 
   uint8_t 
  GetInterestHopCount() const;

  void
  SetInterestExpectedHopCount(uint32_t hops);

  uint32_t
  GetInterestExpectedHopCount() const;

    /**
   * \brief Set InterestPktVersion value
   * Set the version of the interest packet.  Default is version 0.
   */
   void
  SetInterestPktVersion(uint8_t version);

    /**
   * \brief Get InterestPktVersion value
   * Get the version of the interest packet. 
   */   
   uint8_t
  GetInterestPktVersion() const;

    /**
   * \brief Get ObjectHashRestriction tlv
   * Returns the Packet format of the ObjectHash stored in the Meta tlv,
   * as it may be a binary hash value.
   */    
  Ptr<Packet>
  GetObjHashRestr() const;

    /**
   * \brief Set ObjectHashRestr tlv 
   */
  void
  SetObjHashRestr(Ptr<Packet>);

    /**
   * \brief Get ObjectHashRestriction tlv
   * Returns the Packet format of the ObjectHashRestr stored in the Meta tlv,
   * as it may be a binary hash value.
   */    
  Ptr<Packet>
  GetKeyIdRestr() const;

    /**
   * \brief Set KeyIdRestr tlv in the Name section
   * Use the Packet format as the KeyIdRestr stored in the Name tlv,
   * as it may be a binary hash value.
   */
  void
  SetKeyIdRestr(Ptr<Packet>);





  Ptr<Packet>
  GetIPID() const;
  uint32_t
  GetIPID32() const;

  void
  SetIPID(Ptr<Packet>);
  void
  SetIPID32(uint32_t ipidValue);

  bool
  ExistIPID() const;

  void
  DeleteIPID() ;


  void
  DeleteKeyIdRestr();
  void
  DeleteObjHashRestr(); 




  /**
   * @brief Set  "payload" of interest packet
   *
   * This payload can carry packet tags
   */
   void
  SetPayload (ns3::Ptr<Packet> payload);

  /**
   * @brief Get  "payload" to interest packet
   *
   * This payload can carry packet tags
   */
   ns3::Ptr<const Packet>
  GetPayload () const;
  
  /**
   * @brief Get wire formatted packet
   *
   * If wire formatted packet has not been set before, 0 will be returned
   */
  inline ns3::Ptr<const Packet>
  GetWire () const;


  void
  SetTlv (ns3::Ptr<const Packet> packet) const;

  /**
   * @brief Print Interest in plain-text to the specified output stream
   */
   void
  Print (std::ostream &os) const;

  /**
   * @brief Print Interest in plain-text to the specified output stream
   */   
   wire::icnx::TLV_INTEREST*
   GetTlv() const ;

  /**
   * @brief Print Interest in plain-text to the specified output stream
   */   
  void
  SetTlvViaPkt (ns3::Ptr<const Packet> packet) const;

  /**
   * @brief Print Interest in plain-text to the specified output stream
   */  
   void
   SetTlv( wire::icnx::TLV_INTEREST* packet);

  /**
   * @brief Print Interest in plain-text to the specified output stream
   */   
   ns3::Ptr< Packet>
     GetPacketSerialize() const;

private:
  // NO_ASSIGN
  Interest &
  operator = (const Interest &other) { return *this; }
  
private:

  mutable wire::icnx::TLV_INTEREST *m_tlv; //< @brief tlv which holds all interest information
  const wire::icnx::tlvUtils::TlvHelperBase *m_tlvHelper; 
  mutable WireFormat m_wireFormat;

};

inline std::ostream &
operator << (std::ostream &os, const Interest &i)
{
  i.Print (os);
  return os;
}

inline ns3::Ptr<const Packet>
Interest::GetWire () const
{ 
  return GetPacketSerialize();
}



inline void
Interest::SetTlvViaPkt(ns3::Ptr<const Packet> packet) const
{
  //JLM FIXME TODO
  //what if packet==NULL or size ==0?

    uint32_t size = packet->GetSize();
    uint8_t *data = new uint8_t[size];
    packet->CopyData(data, size);
    m_tlv->NetworkDeserialize(data);
    delete data;

}

/**
 * @brief Class for Interest parsing exception 
 */
class InterestException {};

} // namespace ccn
} // namespace ns3

#endif // _CCN_INTEREST_HEADER_H_
