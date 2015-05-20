/* -*- Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil -*- */
/*
 * Copyright (c) 2011-2013 University of California, Los Angeles
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
 * Note:  ccn-data was modified to use icnx vers 1.0, with TLV support.   While
 * most of the code base has been replaced or rewritten, the core used was from
 * the ccn implementation, and thus, proper author credit is observed.
*/

#include "ccn-data.h"

#include "ns3/log.h"

//         #include <boost/foreach.hpp>

NS_LOG_COMPONENT_DEFINE ("ccn.Data");

namespace ns3 {
namespace ccn {

Data::Data (Ptr<Packet> payload)
  : m_wireFormat (SERIAL_ENC)
  , m_tlv(NULL)
  , m_tlvHelper(NULL)
{
  m_tlvHelper = ::wire::icnx::tlvUtils::GetDataTlvHelper();
  if ((payload == 0) || (0 == payload->GetSize())) // just in case
    {
  m_tlv = dynamic_cast< wire::icnx::TLV_CONTENT *>( m_tlvHelper->CreateValidTlv());
      } else { //Packet has our TLV
       SetTlvViaPkt(payload); 

      }

  //debug
 /* SetExpiry(500);
  SetCacheTime(99);
  uint64_t expiry = GetExpiry64();
  uint64_t ct = GetCacheTime64();
  std::cout << "exp(500): " << expiry << ", ct(99): "<< ct << std::endl;
  m_tlv->XmlPrint(std::cout);
  DeleteExpiry();
  DeleteCacheTime();
  m_tlv->XmlPrint(std::cout);
  assert(0); */
}

Data::Data(::wire::icnx::TLV_CONTENT *tlv)
  : m_wireFormat (SERIAL_ENC)
  , m_tlv(NULL)
  , m_tlvHelper(NULL)
{
    m_tlvHelper = ::wire::icnx::tlvUtils::GetInterestTlvHelper();
     SetTlv(dynamic_cast<wire::icnx::TLV_CONTENT *>(tlv->Copy()));
}
void
Data::SetWireFormat(WireFormat format) const {
  m_wireFormat = format;
}

WireFormat 
Data::GetWireFormat () const {
return m_wireFormat;
}
Data::Data (const Data &other)
  : m_wireFormat (SERIAL_ENC)
  , m_tlv(NULL)
  , m_tlvHelper(NULL)
{
  m_tlvHelper = ::wire::icnx::tlvUtils::GetInterestTlvHelper();
  SetTlv(dynamic_cast< wire::icnx::TLV_CONTENT * >( (other.GetTlv())->Copy()) );
}

  void
Data::SetName (Ptr<Name> name)
{
  std::string nameStr = name->toUri();
  SetName(nameStr);
}
void
Data::SetName (const std::string &name)
{
     ::wire::icnx::TLV_NAME *nameTlv = new ::wire::icnx::TLV_NAME;
  nameTlv->SetName(name);
  m_tlvHelper->SetTlv(m_tlv, nameTlv);


}

void
Data::SetName (const Name &name)
{
    std::string nameStr = name.toUri();
  SetName(nameStr);

}

const ns3::ccn::Name
Data::GetName () const
{
  const std::string name = GetStringName();
  return Name(name);
}
std::string
Data::GetStringName () const
{
     if (m_tlvHelper->ExistTlv(m_tlv, "TLV_NAME")) {
         ::wire::icnx::TLV_NAME *nameTlv = dynamic_cast<wire::icnx::TLV_NAME *>(m_tlvHelper->GetTlv(m_tlv, "TLV_NAME"));
         return nameTlv->GetName();
      
  }
    throw std::invalid_argument("Data Packet missing NAME!");
  
}


void
Data::SetDataType(uint8_t type)
{
   SetPayloadType(type);
}

   void
  Data::SetDataPktVersion(uint8_t version) 
{
  wire::icnx::TLV_TYPE_t newType = version+ wire::icnx::CONTENT_TLV;
    if (newType > wire::icnx::CONTENT_TLV_END) {
       throw std::invalid_argument("Interest version out of range");
    }
    m_tlv->SetType(newType);
}

   uint8_t
  Data::GetDataPktVersion() const 
{
  return (m_tlv->GetType() - wire::icnx::CONTENT_TLV);

}


uint8_t
Data::GetPayloadType() const
{
    if (m_tlvHelper->ExistTlv(m_tlv, "TLV_PAYLDTYPE")) {
         ::wire::icnx::TLV_PAYLDTYPE *typeTlv = dynamic_cast<wire::icnx::TLV_PAYLDTYPE *>(m_tlvHelper->GetTlv(m_tlv, "TLV_PAYLDTYPE"));
         return typeTlv->GetPayloadType();
    } 
    return 0; //default value
   
}

void
Data::SetPayloadType(uint8_t type)
{
   ::wire::icnx::TLV_PAYLDTYPE::PAYLOAD_TYPE_t pType = static_cast< ::wire::icnx::TLV_PAYLDTYPE::PAYLOAD_TYPE_t > (type);
   ::wire::icnx::TLV_PAYLDTYPE *typeTlv = new ::wire::icnx::TLV_PAYLDTYPE(pType);
   m_tlvHelper->SetTlv(m_tlv,typeTlv);
    
}

  void
  Data::SetExpiry(ns3::Time ns3Time) 
{
  SetExpiry(ns3Time.GetMilliSeconds()); 
}
  void
  Data::SetExpiry(uint64_t timeExp) 
{
   ::wire::icnx::TLV_EXPIRY *expTlv = new ::wire::icnx::TLV_EXPIRY;
   expTlv->SetExpirationTime(timeExp);
   m_tlvHelper->SetTlv(m_tlv, expTlv);
}
  ns3::Time
  Data::GetExpiry() const 
  {
    return ns3::Time(GetExpiry64());
  }
  uint64_t
  Data::GetExpiry64() const 
  {
     if (m_tlvHelper->ExistTlv(m_tlv, "TLV_EXPIRY"))
     {  
       wire::icnx::TLV_EXPIRY *expiry;
       expiry = dynamic_cast< wire::icnx::TLV_EXPIRY * >(m_tlvHelper->GetTlv(m_tlv,"TLV_EXPIRY"));
       return expiry->GetExpirationTime();
     } 
         
     throw std::out_of_range("Expiry does not exist!");
    
  }
  bool
  Data::ExistExpiry() const 
{     
  return   (m_tlvHelper->ExistTlv(m_tlv, "TLV_EXPIRY"));
}
  void
  Data::DeleteExpiry() 
{
   m_tlvHelper->DeleteTlv(m_tlv, "TLV_EXPIRY");
}

  void
  Data::SetCacheTime(ns3::Time ns3Time) 
{
   SetCacheTime(ns3Time.GetMilliSeconds());
}
  void
  Data::SetCacheTime(uint64_t timeExp) 
{
   ::wire::icnx::TLV_CACHETIME *ctTlv = new ::wire::icnx::TLV_CACHETIME(timeExp);
   m_tlvHelper->SetTlv(m_tlv, ctTlv);
}
  ns3::Time 
  Data::GetCacheTime() const
{
  return ns3::Time(GetCacheTime64());
}
  uint64_t
  Data::GetCacheTime64() const 
{
     if (m_tlvHelper->ExistTlv(m_tlv, "TLV_CACHETIME"))
     {  
       wire::icnx::TLV_CACHETIME *ct;
       ct = dynamic_cast< wire::icnx::TLV_CACHETIME * > (m_tlvHelper->GetTlv(m_tlv,"TLV_CACHETIME"));
       return ct->GetCacheTime();
     } 
         
     throw std::out_of_range("CacheTime does not exist!");

}
  bool
  Data::ExistCacheTime() const 
{
  return   (m_tlvHelper->ExistTlv(m_tlv, "TLV_CACHETIME"));

}
  void
  Data::DeleteCacheTime()
{
  m_tlvHelper->DeleteTlv(m_tlv, "TLV_CACHETIME");
}



void
Data::Print (std::ostream &os) const
{
  os << "Data[" << GetStringName() << "]";
}

void
Data::SetPayload (Ptr<Packet> payload)
{
  uint32_t size = payload->GetSize();
  uint8_t *data = new uint8_t[size];
  payload->CopyData(data, size);
  delete data;
}

Ptr<const Packet>
Data::GetPayload () const
{
  uint32_t size  = m_tlvHelper->GetTlvSize(m_tlv, "TLV_PAYLOAD");
  uint8_t *rawData  = new uint8_t[size];
  memcpy(rawData, m_tlvHelper->GetTlvData(m_tlv, wire::icnx::PAYLOAD_TLV), size);
  ns3::Ptr<const Packet> payload = Create<Packet>(rawData, size);
  delete rawData;
  return payload;
}


::wire::icnx::TLV_CONTENT*
Data::GetTlv() const
{
   return m_tlv;
}

void
Data::SetTlv( ::wire::icnx::TLV_CONTENT * tlv) 
{
  m_tlv = tlv;
}


ns3::Ptr< Packet>
Data::GetPacketSerialize() const
{
   uint32_t size = m_tlv->GetSerializedSize();
   Ptr<Packet> p;
   
   //add TLV
   switch (m_wireFormat) {
        case SERIAL_ENC:
          { 
          p = Create<Packet> (m_tlv->NetworkSerialize(), size);
          }
          break;
        case XML_ENC:
          {
//m_tlv->XmlPrint(std::cout);
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


} // namespace ccn
} // namespace ns3
