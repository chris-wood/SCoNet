/** -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil -*- */
/* 
 * Copyright (c) 2013, Regents of the University of California
 *                     Alexander Afanasyev
 * 
 * GNU 3.0 license, See the LICENSE file for more information
 * 
 * Author: Alexander Afanasyev <alexander.afanasyev@ucla.edu>
 */

#include "ccnx.h"

using namespace std;

#include <ns3/header.h>
#include <ns3/packet.h>
#include <ns3/log.h>

//#include "ccnsim/wire-ccnsim.h"
/*
NS_LOG_COMPONENT_DEFINE ("ccn.wire.icnx");

CCN_NAMESPACE_BEGIN

namespace wire {
namespace icnx {

NS_OBJECT_ENSURE_REGISTERED (Interest);
NS_OBJECT_ENSURE_REGISTERED (InterestResponse);
NS_OBJECT_ENSURE_REGISTERED (Data);

///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////
InterestResponse::InterestResponse ()
  : m_interestResponse (Create<ccn::InterestResponse> ())
{
}

InterestResponse::InterestResponse (Ptr<ccn::InterestResponse> interestResponse)
  : m_interestResponse (interestResponse)
{
}

Ptr<ccn::InterestResponse>
InterestResponse::GetInterestResponse ()
{
  return m_interestResponse;
}


TypeId
InterestResponse::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::ccn::Interest::Ccnx")
    .SetGroupName ("Ccn")
    .SetParent<Header> ()
    .AddConstructor<Interest> ()
    ;
  return tid;
}

TypeId
InterestResponse::GetInstanceTypeId (void) const
{
  return GetTypeId ();
}

Ptr<Packet>
InterestResponse::ToWire (Ptr<const ccn::InterestResponse> interestResponse)
{
  Ptr<const Packet> p = interestResponse->GetWire ();
  
  if (!p)
    {
      Ptr<Packet> packet = Create<Packet> (*interestResponse->GetPacketSerialize());
      InterestResponse wireEncoding (ConstCast<ccn::InterestResponse> (interestResponse));
      packet->AddHeader (wireEncoding);
      interestResponse->SetWire (packet);

      p = packet;
    }
  
  return p->Copy ();
  
}
Ptr<Packet>
InterestResponse::ToXmlWire (Ptr<const ccn::InterestResponse> interestResponse)
{
}
Ptr<ccn::InterestResponse>
InterestResponse::FromXmlWire (Ptr<Packet> packet)
{}

Ptr<ccn::InterestResponse>
InterestResponse::FromWire (Ptr<Packet> packet)
{
  Ptr<ccn::InterestResponse> interestResponse = Create<ccn::InterestResponse> ();
  
  Ptr<Packet> wire = packet->Copy ();

  InterestResponse wireEncoding (interestResponse);
  packet->RemoveHeader (wireEncoding);

  interestResponse->SetTlvViaPkt (packet);
  interestResponse->SetWire (wire);
  
  return interestResponse;
}

uint32_t
InterestResponse::GetSerializedSize (void) const
{
  size_t size = 2; 
  NS_LOG_INFO ("Serialize size = " << size);
  return size;
}
    
void
InterestResponse::Serialize (Buffer::Iterator start) const
{
  start.WriteU8 (0x8a); // version
  start.WriteU8 (0x00); // packet type

}

uint32_t
InterestResponse::Deserialize (Buffer::Iterator start)
{
  Buffer::Iterator i = start;
  
  if (i.ReadU8 () != 0x8a)
    throw new InterestResponseException ();

  if (i.ReadU8 () != 0x00)
    throw new InterestResponseException ();

}

void
InterestResponse::Print (std::ostream &os) const
{
  m_interestResponse->Print (os);
}

Interest::Interest ()
  : m_interest (Create<ccn::Interest> ())
{
}

Interest::Interest (Ptr<ccn::Interest> interest)
  : m_interest (interest)
{
}

Ptr<ccn::Interest>
Interest::GetInterest ()
{
  return m_interest;
}


TypeId
Interest::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::ccn::Interest::Ccnx")
    .SetGroupName ("Ccn")
    .SetParent<Header> ()
    .AddConstructor<Interest> ()
    ;
  return tid;
}

TypeId
Interest::GetInstanceTypeId (void) const
{
  return GetTypeId ();
}

Ptr<Packet>
Interest::ToWire (Ptr<const ccn::Interest> interest)
{
  Ptr<const Packet> p = interest->GetWire ();
  
  if (!p)
    {
      Ptr<Packet> packet = Create<Packet> (*interest->GetPacketSerialize());
      Interest wireEncoding (ConstCast<ccn::Interest> (interest));
      packet->AddHeader (wireEncoding);
      interest->SetWire (packet);

      p = packet;
    }
  
  return p->Copy ();
  
}
Ptr<Packet>
Interest::ToXmlWire (Ptr<const ccn::Interest> interest)
{
}
Ptr<ccn::Interest>
Interest::FromXmlWire (Ptr<Packet> packet)
{}

Ptr<ccn::Interest>
Interest::FromWire (Ptr<Packet> packet)
{
  Ptr<ccn::Interest> interest = Create<ccn::Interest> ();
  
  Ptr<Packet> wire = packet->Copy ();

  Interest wireEncoding (interest);
  packet->RemoveHeader (wireEncoding);

  interest->SetTlvViaPkt (packet);
  interest->SetWire (wire);
  
  return interest;
}

uint32_t
Interest::GetSerializedSize (void) const
{
  size_t size = 2; 
  NS_LOG_INFO ("Serialize size = " << size);
  return size;
}
    
void
Interest::Serialize (Buffer::Iterator start) const
{
  start.WriteU8 (0x80); // version
  start.WriteU8 (0x00); // packet type

}

uint32_t
Interest::Deserialize (Buffer::Iterator start)
{
  Buffer::Iterator i = start;
  
  if (i.ReadU8 () != 0x80)
    throw new InterestException ();

  if (i.ReadU8 () != 0x00)
    throw new InterestException ();

}

void
Interest::Print (std::ostream &os) const
{
  m_interest->Print (os);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////


TypeId
Data::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::ccn::Data::icnx")
    .SetGroupName ("Ccn")
    .SetParent<Header> ()
    .AddConstructor<Data> ()
    ;
  return tid;
}

TypeId
Data::GetInstanceTypeId (void) const
{
  return GetTypeId ();
}
  

Data::Data ()
  : m_data (Create<ccn::Data> ())
{
}

Data::Data (Ptr<ccn::Data> data)
  : m_data (data)
{
}

Ptr<ccn::Data>
Data::GetData ()
{
 return m_data;
}

Ptr<Packet>
Data::ToWire (Ptr< ccn::Data> data)
{
  Ptr<const Packet> p = data->GetWire ();
  if (!p)
    {
      Ptr<Packet> packet = Create<Packet> (*data->GetPacketSerialize());
      Data wireEncoding (ConstCast<ccn::Data> (data));
      packet->AddHeader (wireEncoding);
      data->SetWire (packet);

      p = packet;
    }
  return p->Copy ();
}

Ptr<ccn::Data>
Data::FromWire (Ptr<Packet> packet)
{
  Ptr<ccn::Data> data = Create<ccn::Data> ();
  Ptr<Packet> wire = packet->Copy ();

  Data wireEncoding (data);
  packet->RemoveHeader (wireEncoding);

  data->SetTlvViaPkt (packet);
  data->SetWire (wire);

  return data;
}

uint32_t
Data::GetSerializedSize () const
{
   size_t size = 2; 
 
  NS_LOG_INFO ("Serialize size = " << size);
  return size;
}

void
Data::Serialize (Buffer::Iterator start) const
{
  start.WriteU8 (0x80); // version
  start.WriteU8 (0x01); // packet type
}

uint32_t
Data::Deserialize (Buffer::Iterator start)
{
  Buffer::Iterator i = start;

  if (i.ReadU8 () != 0x80)
    throw new DataException ();

  if (i.ReadU8 () != 0x01)
    throw new DataException ();

  return 2;
}

void
Data::Print (std::ostream &os) const
{
  m_data->Print (os);
}

} // ccnSIM
} // wire
*/
//CCN_NAMESPACE_END
