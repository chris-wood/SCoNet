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
 * Author: Alexander Afanasyev <alexander.afanasyev@ucla.edu>
 *
 * Modification Author:
 *         James Mathewson <jlmathew@soe.ucsc.edu>
 *
 * Note:  ccn-interest was modified to use icnx vers 1.0, with TLV support.   While
 * most of the code base has been replaced or rewritten, the core used was from
 * the ccn implementation, and thus, proper author credit is observed.
 */

#include "ccn-interestresponse.h"

#include "ns3/log.h"
#include "ns3/unused.h"
#include "ns3/packet.h"

NS_LOG_COMPONENT_DEFINE ("ccn.InterestResponse");

namespace ns3 {
namespace ccn {

InterestResponse::InterestResponse (ns3::Ptr<Packet> payload)
  : m_wireFormat (SERIAL_ENC)
  , m_tlv(NULL)
  , m_tlvHelper(NULL)
{
  m_tlvHelper = ::wire::icnx::tlvUtils::GetInterestResponseTlvHelper();
  if ((payload == 0) || (0 == payload->GetSize())) // just in case
    {

  m_tlv = dynamic_cast< wire::icnx::TLV_INTERESTRESPONSE *>( m_tlvHelper->CreateValidTlv());
      } else { //Packet has our TLV
       SetTlvViaPkt(payload); 
      }

//m_tlv->XmlPrint(std::cout);
}
void
InterestResponse::SetWireFormat(WireFormat format) const {
  m_wireFormat = format;
}
InterestResponse::InterestResponse(::wire::icnx::TLV_INTERESTRESPONSE *tlv)
  : m_wireFormat (SERIAL_ENC)
  , m_tlv(NULL)
  , m_tlvHelper(NULL)
{
    m_tlvHelper = ::wire::icnx::tlvUtils::GetInterestResponseTlvHelper();
     SetTlv(dynamic_cast<wire::icnx::TLV_INTERESTRESPONSE *>(tlv->Copy()));
}
WireFormat 
InterestResponse::GetWireFormat () const {
return m_wireFormat;
}

InterestResponse::InterestResponse (const InterestResponse &interest)
  : m_wireFormat (SERIAL_ENC)
  , m_tlv(NULL)
  , m_tlvHelper(NULL)
{
  NS_LOG_FUNCTION ("correct copy constructor");
  m_tlvHelper = ::wire::icnx::tlvUtils::GetInterestResponseTlvHelper();
  SetTlv(dynamic_cast<wire::icnx::TLV_INTERESTRESPONSE *>( (interest.GetTlv())->Copy()) );

}

   void
  InterestResponse::SetInterest (Ptr< Interest> interest)
{
   ::wire::icnx::TLV_INTEREST *interestTlv = interest->GetTlv();
   ::wire::icnx::TLV_BASE *interestTlv2 = interestTlv->Copy();

   m_tlvHelper->SetTlv(m_tlv, interestTlv2);
    
}



  Ptr< Interest>
  InterestResponse::GetInterest () const
{
   ::wire::icnx::TLV_INTEREST *interestTlv = dynamic_cast< ::wire::icnx::TLV_INTEREST * > (m_tlvHelper->GetTlv(m_tlv, "TLV_INTEREST"));
   //this is done as Copy() returns a TLV_BASE pointer.
   ::wire::icnx::TLV_INTEREST *interestTlvCpy = dynamic_cast< ::wire::icnx::TLV_INTEREST * > (interestTlv->Copy());
   Ptr< Interest> intObject = Create<Interest> ();
   intObject->SetTlv(interestTlvCpy);

   return intObject;

}



ns3::Ptr< Packet>
InterestResponse::GetPacketSerialize() const
{
   uint32_t size = m_tlv->GetSerializedSize();
   Ptr<Packet> p;
   
   switch (m_wireFormat) {
        case SERIAL_ENC:
          { 
          p = Create<Packet> (m_tlv->NetworkSerialize(), size);
          }
          break;
        case XML_ENC:
          {
             std::string xmlString = m_tlv->XmlPrint();
             p = Create<Packet> ((const uint8_t *) xmlString.c_str(), xmlString.length());
          }
          break;

        default:
          throw std::out_of_range("Unknown format");
          break;

      }

   //add type header
   EncodingTypeHeader encType(m_wireFormat);
   p->AddHeader(encType);
   return p;

}

void
InterestResponse::Print (std::ostream &os) const
{
  os << "InterestResponse[" << GetInterest()->GetStringName() << "]";
  return;
}

::wire::icnx::TLV_INTERESTRESPONSE *
InterestResponse::GetTlv() const
{
   return m_tlv;
}

void
InterestResponse::SetTlv(::wire::icnx::TLV_INTERESTRESPONSE * tlv) 
{
  //copy pointer, correct action?
   m_tlv = tlv;
}

   void
  InterestResponse::SetInterestResponseHopCount(uint8_t hops) 
{

  //overloaded with interest hop count, its the same thing!
    Ptr< Interest> interest = GetInterest();
    interest->SetInterestHopCount(hops);
    SetInterest(interest);
}
   uint8_t 
  InterestResponse::GetInterestResponseHopCount() const 
{ 
      Ptr< Interest> interest = GetInterest();
      return interest->GetInterestHopCount();
}


   void
  InterestResponse::SetInterestResponsePktVersion(uint8_t version) 
{
  wire::icnx::TLV_TYPE_t newType = version+ wire::icnx::INTERESTRESP_TLV;
    if (newType > wire::icnx::INTERESTRESP_TLV_END) {
       throw std::invalid_argument("InterestResponse version out of range");
    }
    m_tlv->SetType(newType);
}

  void
  InterestResponse::SetReturnCode(uint8_t code) 
{
  ::wire::icnx::TLV_INTERESTRESP_RETURNCODE::INTERESTRESP_RC_t returnHops = static_cast< ::wire::icnx::TLV_INTERESTRESP_RETURNCODE::INTERESTRESP_RC_t > (code);
   ::wire::icnx::TLV_INTERESTRESP_RETURNCODE *intHopTlv = new ::wire::icnx::TLV_INTERESTRESP_RETURNCODE(returnHops);
   m_tlvHelper->SetTlv(m_tlv, intHopTlv);
}
   uint8_t 
  InterestResponse::GetReturnCode() const 
{
    if (m_tlvHelper->ExistTlv(m_tlv, "TLV_INTERESTRESP_RETURNCODE")) {
         ::wire::icnx::TLV_INTERESTRESP_RETURNCODE *rcTlv = dynamic_cast<wire::icnx::TLV_INTERESTRESP_RETURNCODE *>(m_tlvHelper->GetTlv(m_tlv, "TLV_INTERESTRESP_RETURNCODE"));
         return rcTlv->GetReturnCode();
      
  }
    throw std::invalid_argument("InterestResponse Packet missing Hop count!");
}



   uint8_t
  InterestResponse::GetInterestResponsePktVersion() const 
{
  return (m_tlv->GetType() - wire::icnx::INTERESTRESP_TLV);
}



} // namespace ccn
} // namespace ns3

