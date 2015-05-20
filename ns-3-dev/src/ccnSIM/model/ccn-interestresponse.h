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

#ifndef _CCN_INTERESTRESPONSE_HEADER_H_
#define _CCN_INTERESTRESPONSE_HEADER_H_

#include "ns3/simple-ref-count.h"
#include "ns3/nstime.h"
#include "ns3/packet.h"
#include "ns3/ptr.h"
#include "ns3/ccn-interest.h"

#include <ns3/ccnSIM/model/wire/icnx/ccn-1.0_tlv-helper-base.h>
#include <ns3/ccnSIM/model/wire/icnx/ccn-1.0_interest_tlv.h>
#include "ns3/ccn-common-object.h"

namespace ns3 {

class Packet;

namespace ccn {

/**
 * @ingroup ccn
 * @brief CCNX 1.0 InterestResponse (wire formats are defined in wire)
 **/
class InterestResponse : public SimpleRefCount<InterestResponse>
{
public:

  /**
   * \brief Constructor
   *
   * Creates a null header
   **/
  InterestResponse (ns3::Ptr<Packet> payload = 0);

  /**
   * @brief Copy constructor
   */
  InterestResponse (const InterestResponse &interest);

  InterestResponse (::wire::icnx::TLV_INTERESTRESPONSE *tlv);


  /**
   * @brief Set the serialized wire format method
   *
   * Return the serialization method of the TLV packets
   */
void
SetWireFormat(WireFormat format) const ;

   /**
   * @brief Get serialized wire format method
   *
   * Return the serialization method of the TLV packets (XML/Byte)
   */
WireFormat 
GetWireFormat () const ;


  //Return code
  enum IntRespRetCode
    {
      No_Route = 0,
      HopLimit_Exceeded,
      No_Resources,
      Path_Error,
      Prohibited,
      Congested,
      MTU_Too_Large
    };

  /**
   * \brief Set InterestResponseLifetime
   * InterestResponseLifetime indicates the (approximate) time remaining before the interest times out.
   * The timeout is relative to the arrival time of the interest at the current node.
   * @param[in] time interest lifetime  
   */ 
   void
  SetInterest (Ptr< Interest > interest);

  /**
   * \brief Get InterestResponseLifetime value
   * InterestResponseLifetime indicates the (approximate) time remaining before the interest times out.
   * The timeout is relative to the arrival time of the interest at the current node.
   * This is an optional field, and if it does not exist will return a default of 4s.
   */ 
  Ptr< Interest >
  GetInterest () const;

  //additional functions requested by maziar
  /**
   * \brief Set InterestResponseHopCount value
   * InterestResponseHopCount measures how many hops maximum, from the consumer, before the
   * interest is discarded.
   */ 
   void
  SetInterestResponseHopCount(uint8_t hops);

  /**
   * \brief Get InterestResponseHopCount value
   * InterestResponseHopCount measures how many hops maximum, from the consumer, before the
   * interest is discarded.
   */ 
   uint8_t 
  GetInterestResponseHopCount() const;

  /**
   * \brief Set return code value
   *  ReturnCode is the code for the reason, in which, on data was given
   */ 
   void
  SetReturnCode (uint8_t code);
  /**
   * \brief Get return code value
   *  ReturnCode is the code for the reason, in which, on data was given
   */ 
   uint8_t 
  GetReturnCode() const;


    /**
   * \brief Set InterestResponsePktVersion value
   * Set the version of the interest packet.  Default is version 0.
   */
   void
  SetInterestResponsePktVersion(uint8_t version);

    /**
   * \brief Get InterestResponsePktVersion value
   * Get the version of the interest packet. 
   */   
   uint8_t
  GetInterestResponsePktVersion() const;


  void
  SetTlv (ns3::Ptr<const Packet> packet) const;

  /**
   * @brief Print InterestResponse in plain-text to the specified output stream
   */
   void
  Print (std::ostream &os) const;

  /**
   * @brief Print InterestResponse in plain-text to the specified output stream
   */   
   wire::icnx::TLV_INTERESTRESPONSE*
   GetTlv() const ;

  /**
   * @brief Print InterestResponse in plain-text to the specified output stream
   */   
  void
  SetTlvViaPkt (ns3::Ptr<const Packet> packet) const;

  /**
   * @brief Print InterestResponse in plain-text to the specified output stream
   */  
   void
   //SetTlv(ns3::Ptr< const wire::icnx::TLV_INTERESTRESPONSE> packet);
   SetTlv( wire::icnx::TLV_INTERESTRESPONSE* packet);

  /**
   * @brief Print InterestResponse in plain-text to the specified output stream
   */   
   ns3::Ptr< Packet>
     GetPacketSerialize() const;

private:
  // NO_ASSIGN
  InterestResponse &
  operator = (const InterestResponse &other) { return *this; }
  
private:
  mutable wire::icnx::TLV_INTERESTRESPONSE *m_tlv; //< @brief tlv which holds all interest information
  const wire::icnx::tlvUtils::TlvHelperBase *m_tlvHelper; 
  mutable WireFormat m_wireFormat;

};

inline std::ostream &
operator << (std::ostream &os, const InterestResponse &i)
{
  i.Print (os);
  return os;
}


inline void
InterestResponse::SetTlvViaPkt(ns3::Ptr<const Packet> packet) const
{
    uint32_t size = packet->GetSize();
    uint8_t *data = new uint8_t[size];
    packet->CopyData(data, size);
    m_tlv->NetworkDeserialize(data);
    delete data;

}


} // namespace ccn
} // namespace ns3

#endif // _CCN_INTERESTRESPONSE_HEADER_H_
