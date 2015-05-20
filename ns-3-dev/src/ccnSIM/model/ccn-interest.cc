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

#include "ccn-interest.h"

#include "ns3/log.h"
#include "ns3/unused.h"
#include "ns3/packet.h"

NS_LOG_COMPONENT_DEFINE ("ccn.Interest");

namespace ns3 {
namespace ccn {

Interest::Interest (ns3::Ptr<Packet> payload/* = Create<Packet> ()*/)
  : m_wireFormat (SERIAL_ENC)
  , m_tlv(NULL)
  , m_tlvHelper(NULL)
{
  m_tlvHelper = ::wire::icnx::tlvUtils::GetInterestTlvHelper();
  if ((payload == 0) || (0 == payload->GetSize())) // just in case
    {

  m_tlv = dynamic_cast< wire::icnx::TLV_INTEREST *>( m_tlvHelper->CreateValidTlv());
      } else { //Packet has our TLV
       SetTlvViaPkt(payload); 
      }
//debug
//m_tlv->XmlPrint(std::cout);
/*
SetIPID32(8192);
uint32_t value = GetIPID32();
std::cout << "return value (8192) is " << value << std::endl;
m_tlv->XmlPrint(std::cout);
DeleteIPID();
m_tlv->XmlPrint(std::cout);
assert(0);*/
/*
//Testing and validation
  //test exist
  wire::icnx::TLV_BASE *tlvPtr = m_tlvHelper->CreateValidTlv();
  std::cout << "exist Name (true) test: " << m_tlvHelper->ExistTlv(tlvPtr, "TLV_NAME") << std::endl; 
  std::cout << "exist NameSeg (true) test: " << m_tlvHelper->ExistTlv(tlvPtr, wire::icnx::NAMESEG_TLV)<< std::endl; 
  wire::icnx::tlvUtils::TlvRange tlvRange2(wire::icnx::INTEREST_TLV, wire::icnx::INTEREST_TLV_END);
  std::cout << "exist Interest (true) test: " << m_tlvHelper->ExistTlv(tlvPtr, tlvRange2 )<< std::endl; 

  std::cout << "exist KeyId (false) test: " << m_tlvHelper->ExistTlv(tlvPtr, "TLV_KEYID") << std::endl; 
  std::cout << "exist IPIDM (false) test: " << m_tlvHelper->ExistTlv(tlvPtr, wire::icnx::IPIDM_TLV)<< std::endl; 
  wire::icnx::tlvUtils::TlvRange tlvRange(wire::icnx::PAYLOAD_TLV, wire::icnx::PAYLOAD_TLV);
  std::cout << "exist Payload (false) test: " << m_tlvHelper->ExistTlv(tlvPtr, tlvRange )<< std::endl; 

  //test add
  //prototype
#include <ns3/ccnSIM/model/wire/icnx/tlv_factory.h>

  wire::icnx::TLV_BASE *appendTlv;
  appendTlv = ::wire::icnx::tlvUtils::CreateTlvFromType ( wire::icnx::NAMESEG_TLV );
  m_tlvHelper->AppendTlv(tlvPtr, appendTlv);
  std::cout << "Appended NAMESEG" << std::endl;

  wire::icnx::TLV_PAYLOAD *payloadTlv = new ::wire::icnx::TLV_PAYLOAD;
  m_tlvHelper->SetTlv(tlvPtr, payloadTlv );
  std::cout << "Set Payload" << std::endl;

  //test delete
  m_tlvHelper->DeleteTlv(tlvPtr, "TLV_INTERESTHOP");
  std::cout << "Deleted interesthop" << std::endl;

  m_tlvHelper->DeleteTlv(tlvPtr, wire::icnx::NAME_TLV);
  std::cout << "Deleted names!" << std::endl;



  //test Get
  wire::icnx::TLV_MESG *mesg = dynamic_cast<wire::icnx::TLV_MESG *>(m_tlvHelper->GetTlv(tlvPtr, "TLV_MESG"));
  wire::icnx::TLV_NAME *name = new ::wire::icnx::TLV_NAME;
  name->SetName("/test/this/works");
  mesg->AppendTlvData(name);
  std::cout << "added name of /test/this/works to mesg" << std::endl;
  

  //modify TLV manually

  tlvPtr->XmlPrint(std::cout);
assert(0);
*/
   

}

Interest::Interest (const Interest &interest)
  : m_wireFormat (SERIAL_ENC)
  , m_tlv(NULL)
  , m_tlvHelper(interest.m_tlvHelper)
{
  NS_LOG_FUNCTION ("correct copy constructor");
  m_tlvHelper = ::wire::icnx::tlvUtils::GetInterestTlvHelper();
  SetTlv(dynamic_cast<wire::icnx::TLV_INTEREST *>( (interest.GetTlv())->Copy()) );

}

Interest::Interest(::wire::icnx::TLV_INTEREST *tlv)
  : m_wireFormat (SERIAL_ENC)
  , m_tlv(NULL)
  , m_tlvHelper(NULL)
{
    m_tlvHelper = ::wire::icnx::tlvUtils::GetInterestTlvHelper();
     SetTlv(dynamic_cast<wire::icnx::TLV_INTEREST *>(tlv->Copy()));
}

void
Interest::SetName (ns3::Ptr<Name> name)
{
  std::string nameStr = name->toUri();
  SetName(nameStr);
}

void
Interest::SetName (const Name &name)
{
  std::string nameStr = name.toUri();
  SetName(nameStr);

}
void
Interest::SetName (const std::string &name)
{
     ::wire::icnx::TLV_NAME *nameTlv = new ::wire::icnx::TLV_NAME;
  nameTlv->SetName(name);
  m_tlvHelper->SetTlv(m_tlv, nameTlv);


}
std::string
Interest::GetStringName () const
{
     if (m_tlvHelper->ExistTlv(m_tlv, "TLV_NAME")) {
         ::wire::icnx::TLV_NAME *nameTlv = dynamic_cast<wire::icnx::TLV_NAME *>(m_tlvHelper->GetTlv(m_tlv, "TLV_NAME"));
         return nameTlv->GetName();
      
  }
    throw std::invalid_argument("Interest Packet missing NAME!");
}
 
const ns3::ccn::Name
Interest::GetName () const
{

  const std::string name = GetStringName();
  return Name(name);
}


void
Interest::SetInterestLifetime (Time lifetime)
{

   ::wire::icnx::TLV_INTLIFE *intLifeTlv = new ::wire::icnx::TLV_INTLIFE;
   uint64_t time = lifetime.GetMilliSeconds();
  intLifeTlv->SetTime(time);
  m_tlvHelper->SetTlv(m_tlv, intLifeTlv);
}

Time
Interest::GetInterestLifetime () const
{

  //return default time, if it doesnt exist, 4s
  if (m_tlvHelper->ExistTlv(m_tlv, "TLV_INTLIFE")) {
         ::wire::icnx::TLV_INTLIFE *intLifeTlv = dynamic_cast<wire::icnx::TLV_INTLIFE *>(m_tlvHelper->GetTlv(m_tlv, "TLV_INTLIFE"));
         uint64_t time = intLifeTlv->GetTime();
         return MilliSeconds(time);
      
  }
     return Seconds(4);
}



void
Interest::SetPayload (ns3::Ptr<Packet> payload)
{
  ::wire::icnx::TLV_LENGTH_t size = payload->GetSize();
  uint8_t *data = new uint8_t[size];
  payload->CopyData(data, size);
  ::wire::icnx::TLV_PAYLOAD *payloadTlv = new ::wire::icnx::TLV_PAYLOAD(wire::icnx::PAYLOAD_TLV, size, data);
  m_tlvHelper->SetTlv(m_tlv, payloadTlv);
  delete data;
}

ns3::Ptr<const Packet>
Interest::GetPayload () const
{

  uint32_t size  = m_tlvHelper->GetTlvSize(m_tlv, "TLV_PAYLOAD");
  uint8_t *rawData  = new uint8_t[size];
  memcpy(rawData, m_tlvHelper->GetTlvData(m_tlv, wire::icnx::PAYLOAD_TLV), size);
  ns3::Ptr<const Packet> payload = Create<Packet>(rawData, size);
  delete rawData;
  return payload;
}

ns3::Ptr< Packet>
Interest::GetPacketSerialize() const
{
   uint32_t size = m_tlv->GetSerializedSize();
   Ptr<Packet> p;
   
   //add TLV
   switch (m_wireFormat) {
        case SERIAL_ENC:
          { 
//std::cout << "Interst pkt serialized\n";
//m_tlv->XmlPrint(std::cout);
          
          p = Create<Packet> (m_tlv->NetworkSerialize(), size);
          }
          break;
        case XML_ENC:
          {
m_tlv->XmlPrint(std::cout);
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
Interest::Print (std::ostream &os) const
{
  os << "Interest[" << GetStringName() << "]";
  return;
}

WireFormat 
Interest::GetWireFormat () const {
return m_wireFormat;
}

void
Interest::SetWireFormat(WireFormat format) const {
  m_wireFormat = format;
}


//ns3::Ptr< const ::wire::icnx::TLV_INTEREST>
::wire::icnx::TLV_INTEREST *
Interest::GetTlv() const
{
   return m_tlv;
}

void
//Interest::SetTlv(ns3::Ptr<const ::wire::icnx::TLV_INTEREST> tlv) 
Interest::SetTlv(::wire::icnx::TLV_INTEREST * tlv) 
{
   //this deep copies the TLV, should we copy pointer, or let it deep copy?
   //m_tlv->NetworkDeserialize(tlv->NetworkSerialize());
  //copy pointer, correct action?
   m_tlv = tlv;
}

   void
  Interest::SetInterestHopCount(uint8_t hops) 
{
   ::wire::icnx::TLV_INTERESTHOP *intHopTlv = new ::wire::icnx::TLV_INTERESTHOP(hops);
   m_tlvHelper->SetTlv(m_tlv, intHopTlv);
}
   uint8_t 
  Interest::GetInterestHopCount() const 
{
    if (m_tlvHelper->ExistTlv(m_tlv, "TLV_INTERESTHOP")) {
         ::wire::icnx::TLV_INTERESTHOP *intHopTlv = dynamic_cast<wire::icnx::TLV_INTERESTHOP *>(m_tlvHelper->GetTlv(m_tlv, "TLV_INTERESTHOP"));
         return intHopTlv->GetInterestHops();
      
  }
    throw std::invalid_argument("Interest Packet missing Hop count!");
}

//believe this should be in interest response
  void
  Interest::SetInterestExpectedHopCount(uint32_t hops)
{
  return;
  assert(0);
}
   uint32_t
  Interest::GetInterestExpectedHopCount() const
{
  //JLM FIXME for MAZIAR TODO, not valid call?
  return 6;
  assert(0);
}

   void
  Interest::SetInterestPktVersion(uint8_t version) 
{
  wire::icnx::TLV_TYPE_t newType = version+ wire::icnx::INTEREST_TLV;
    if (newType > wire::icnx::INTEREST_TLV_END) {
       throw std::invalid_argument("Interest version out of range");
    }
    m_tlv->SetType(newType);
}
   uint8_t
  Interest::GetInterestPktVersion() const 
{
  return (m_tlv->GetType() - wire::icnx::INTEREST_TLV);

}

  Ptr<Packet>
  Interest::GetIPID() const
{
  if (m_tlvHelper->ExistTlv(m_tlv, "TLV_IPID"))
  {  
    wire::icnx::TLV_IPID *ipid;
    ipid = dynamic_cast< wire::icnx::TLV_IPID * > (m_tlvHelper->GetTlv(m_tlv,"TLV_IPID"));
    return Create<Packet> (ipid->GetData(), ipid->GetLength());
  } else
  {
    return  Create<Packet>(); //empty packet
  }

}
  uint32_t
  Interest::GetIPID32() const
{
   Ptr<Packet> ipid32 = GetIPID();
   //assume its a string formatted string
   uint32_t ipid = atoll((const char *) ipid32->PeekData());
   return ipid;
}

  void
  Interest::SetIPID(Ptr<Packet> ipid)
{
  ::wire::icnx::TLV_LENGTH_t size = ipid->GetSize();
  uint8_t *data = new uint8_t[size];
  ipid->CopyData(data, size);
  ::wire::icnx::TLV_IPID *ipidTlv = new ::wire::icnx::TLV_IPID(wire::icnx::IPID_TLV, size, data);
  m_tlvHelper->SetTlv(m_tlv, ipidTlv);
  delete data;

}
  void
  Interest::SetIPID32(uint32_t ipidValue)
{
   char num[20];
   sprintf(num, "%lu", ipidValue);
   Ptr < Packet > ipid32 = Create<Packet>((const uint8_t *) num, strlen(num));
   SetIPID(ipid32);
}

  bool
  Interest::ExistIPID() const
{
    return   (m_tlvHelper->ExistTlv(m_tlv, "TLV_IPID"));

}

  void
  Interest::DeleteIPID() 
{
   m_tlvHelper->DeleteTlv(m_tlv, "TLV_IPID");
}



  void
  Interest::DeleteKeyIdRestr()
{
   m_tlvHelper->DeleteTlv(m_tlv, "TLV_KEYIDRESTR");

}
  void
  Interest::DeleteObjHashRestr()
{
   m_tlvHelper->DeleteTlv(m_tlv, "TLV_OBJHASHRESTR");

}





  Ptr<Packet>
  Interest::GetObjHashRestr() const
{
  if (m_tlvHelper->ExistTlv(m_tlv, "TLV_OBJHASHRESTR"))
  {  
    wire::icnx::TLV_OBJHASHRESTR *objhash;
    objhash = dynamic_cast< wire::icnx::TLV_OBJHASHRESTR * > (m_tlvHelper->GetTlv(m_tlv,"TLV_OBJHASHRESTR"));
    if (objhash->GetLength() != 32) {
        throw std::out_of_range("ObjecthashRest NOT 32 bytes long!");
    }
    return Create<Packet> (objhash->GetData(), objhash->GetLength());
  } else
  {
    return  Create<Packet>(); //empty packet
  }
}

  void
  Interest::SetObjHashRestr(Ptr<Packet> hash)
{
  if (32 != hash->GetSize())
  {
     throw std::out_of_range("ObjectHashRest set is NOT 32 bytes long!");
  }
    wire::icnx::TLV_OBJHASHRESTR *objhash ; 
    objhash = new wire::icnx::TLV_OBJHASHRESTR;
    objhash->SetData(hash->PeekData(), hash->GetSize());
    m_tlvHelper->SetTlv(m_tlv, objhash);

}

  Ptr<Packet>
  Interest::GetKeyIdRestr() const
{
  if (m_tlvHelper->ExistTlv(m_tlv, "TLV_KEYIDRESTR"))
  {  
    wire::icnx::TLV_KEYIDRESTR *keyidr;
    keyidr = dynamic_cast< wire::icnx::TLV_KEYIDRESTR * > (m_tlvHelper->GetTlv(m_tlv,"TLV_KEYIDRESTR"));
    return Create<Packet> (keyidr->GetData(), keyidr->GetLength());
  } else
  {
    return  Create<Packet>(); //empty packet
  }
}

  void
  Interest::SetKeyIdRestr(Ptr<Packet> keyid)
{
  if (0 == keyid->GetSize())  //should we delete KeyIdRest?
  {
     throw std::out_of_range("KeyIdRest can not be 0 bytes ... I think");
  }
    wire::icnx::TLV_KEYIDRESTR *keyidr ; 
    keyidr = new wire::icnx::TLV_KEYIDRESTR;
    keyidr->SetData(keyid->PeekData(), keyid->GetSize());
    m_tlvHelper->SetTlv(m_tlv, keyidr);

}






} // namespace ccn
} // namespace ns3

