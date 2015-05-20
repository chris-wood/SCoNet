/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil -*- */
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
 */

#include "ccn-face.h"

#include "ns3/packet.h"
#include "ns3/log.h"
#include "ns3/node.h"
#include "ns3/assert.h"
#include "ns3/uinteger.h"
#include "ns3/double.h"
#include "ns3/boolean.h"
#include "ns3/simulator.h"
#include "ns3/random-variable.h"
#include "ns3/pointer.h"

#include "ns3/ccn-header-helper.h"
#include "ns3/ccnSIM/utils/ccn-fw-hop-count-tag.h"

#include "ns3/ccn-wire.h"

#include <boost/ref.hpp>

NS_LOG_COMPONENT_DEFINE ("ccn.Face");

namespace ns3 {
namespace ccn {

NS_OBJECT_ENSURE_REGISTERED (Face);

TypeId
Face::GetTypeId ()
{
  static TypeId tid = TypeId ("ns3::ccn::Face")
    .SetParent<Object> ()
    .SetGroupName ("Ccn")
    .AddAttribute ("Id", "Face id (unique integer for the Ccn stack on this node)",
                   TypeId::ATTR_GET, // allow only getting it.
                   UintegerValue (0),
                   MakeUintegerAccessor (&Face::m_id),
                   MakeUintegerChecker<uint32_t> ())
    ;
  return tid;
}

/**
 * By default, Ccn face are created in the "down" state
 *  with no IP addresses.  Before becoming useable, the user must
 * invoke SetUp on them once an Ccn address and mask have been set.
 */
Face::Face (Ptr<Node> node)
  : m_node (node)
  , m_upstreamInterestHandler (MakeNullCallback< void, Ptr<Face>, Ptr<Interest> > ())
  , m_upstreamInterestResponseHandler (MakeNullCallback< void, Ptr<Face>, Ptr<InterestResponse> > ())
  , m_upstreamDataHandler (MakeNullCallback< void, Ptr<Face>, Ptr<Data> > ())
  , m_ifup (false)
  , m_id ((uint32_t)-1)
  , m_metric (0)
  , m_flags (0)
{
  NS_LOG_FUNCTION (this << node);

  NS_ASSERT_MSG (node != 0, "node cannot be NULL. Check the code");
}

Face::~Face ()
{
  NS_LOG_FUNCTION_NOARGS ();
}

Face::Face (const Face &)
{
}

Face& Face::operator= (const Face &)
{
  return *this;
}

Ptr<Node>
Face::GetNode () const
{
  return m_node;
}

void
Face::RegisterProtocolHandlers (const InterestHandler &interestHandler, const DataHandler &dataHandler, const InterestResponseHandler &interestResponseHandler)
{
  NS_LOG_FUNCTION_NOARGS ();

  m_upstreamInterestHandler = interestHandler;
  m_upstreamInterestResponseHandler = interestResponseHandler;
  m_upstreamDataHandler = dataHandler;
}

void
Face::UnRegisterProtocolHandlers ()
{
  NS_LOG_FUNCTION_NOARGS ();

  m_upstreamInterestHandler = MakeNullCallback< void, Ptr<Face>, Ptr<Interest> > ();
  m_upstreamInterestResponseHandler = MakeNullCallback< void, Ptr<Face>, Ptr<InterestResponse> > ();
  m_upstreamDataHandler = MakeNullCallback< void, Ptr<Face>, Ptr<Data> > ();
}


bool
Face::SendInterest (Ptr< Interest> interest)
{
  NS_LOG_FUNCTION (this  << interest->GetName ());

  if (!IsUp ())
    {
      return false;
    }

  return Send(interest->GetPacketSerialize() );
  //return Send (interest->GetTlv(), interest->GetWireFormat() ); //Wire::FromInterest (interest));
}
bool
Face::SendInterestResponse (Ptr< InterestResponse> interestResponse)
{
  NS_LOG_FUNCTION (this );

  if (!IsUp ())
    {
      return false;
    }

  return Send (interestResponse->GetPacketSerialize() ); //Wire::FromData (data));
  //return Send (interestResponse->GetTlv(), interestResponse->GetWireFormat() ); //Wire::FromInterestResponse (interestResponse));
}
bool
Face::SendData (Ptr< Data> data)
{
  NS_LOG_FUNCTION (this << data);

  if (!IsUp ())
    {
      return false;
    }

  return Send (data->GetPacketSerialize() ); //Wire::FromData (data));
  //return Send (data->GetTlv(), data->GetWireFormat() ); //Wire::FromData (data));
}

/*bool
Face::Send(wire::icnx::TLV_BASE *tlv, WireFormat format)
{

      Ptr<Packet> packet  ;  
      EncodingTypeHeader encType(format);
      switch (format) {
        case SERIAL_ENC:
          { 
tlv->XmlPrint(std::cout);
            uint32_t size = tlv->GetSerializedSize();
            const uint8_t * data = tlv->NetworkSerialize();
            packet = Create<Packet> (data, size);
          }
          break;
        case XML_ENC:
          {
tlv->XmlPrint(std::cout);
             std::string xmlString = tlv->XmlPrint();
             packet = Create<Packet> ((const uint8_t *) xmlString.c_str(), xmlString.length());
          }
          break;

        default:
          throw std::out_of_range("Unknown format");
          break;

      }
          
      packet->AddHeader (encType);
      
  Send(packet);
}*/

bool
Face::Send (Ptr<Packet> packet)
{
  FwHopCountTag hopCount;
  bool tagExists = packet->RemovePacketTag (hopCount);
  if (tagExists)
    {
      hopCount.Increment ();
      packet->AddPacketTag (hopCount);
    }


//debug
/* Ptr<Packet> p = packet->Copy();
EncodingTypeHeader wireType;
p->RemoveHeader(wireType);
   WireFormat wireEncodingType = static_cast<WireFormat>(wireType.Value());
    uint32_t size = p->GetSize(); 
    uint8_t *data = new uint8_t[size];
    p->CopyData(data, size);
    std::cout << "SEND PKT [" << wireEncodingType << "]" << " : \n";
    for (int i=0; i<size; i++)
    {
       std::cout << std::hex << std::setw(2) << (int) data[i] << " "; 
    }
    std::cout << std::endl << std::endl; 

//packet->Print(std::cout);
std::cout << std::endl << "------end send----------" << std::endl; */

  return true;
}

bool
Face::Receive (Ptr<const Packet> packet)
{
  NS_LOG_FUNCTION (this << packet << packet->GetSize ());
//packet->Print(std::cout);
//std::cout << std::endl;

  if (!IsUp ())
    {
      // no tracing here. If we were off while receiving, we shouldn't even know that something was there
      return false;
    }

    //Get encoding method
   EncodingTypeHeader wireType;
   //make a copy, sicne we want to remove header, and cant on a const type
   Ptr<Packet> p = packet->Copy();
   p->RemoveHeader(wireType);
   WireFormat wireEncodingType = static_cast<WireFormat>(wireType.Value());

   //The deserialization and ID probably shouldnt be handled in the interface, as 
   //best policy wishes to keep TLV separate from interface, but it removes a lot
   //of code spam elsewhere
    ::wire::icnx::TLV_BASE *tlv=NULL;
    //copy Packet data to a buffer
    uint32_t size = p->GetSize(); 
    uint8_t *data = new uint8_t[size];
    p->CopyData(data, size);

    /* std::cout << "RX PKT [" << wireEncodingType << "]" << " : \n";
    for (int i=0; i<size; i++)
    {
       std::cout << std::hex << std::setw(2) << (int) data[i] << " "; 
    }
    std::cout << "------End rx pkt--------" << std::endl << std::endl;
     */


    std::string xmlTlv;

      switch (wireEncodingType)
      {
      case XML_ENC:   
         {
         std::string xmlTlv;
         xmlTlv.reserve(size);
         xmlTlv = (char *)(data);
         tlv->XmlImplement(xmlTlv);
         }
         break;
      case SERIAL_ENC:   
         tlv = wire::icnx::tlvUtils::CreateTlvFromData(data, false); 
         break;
      default:
          NS_FATAL_ERROR ("Not supported CCN encoding type");
          delete data;
          return false;
      }
          
    delete data;
          //JLM FIXME TODO need to pass content to create a TLV, then get the type.
//Only need the encryption method used to be passed (XML or byte enc)

      ::wire::icnx::TLV_TYPE_t tlvType = tlv->GetType();

          //we have a tlv, lets act on the type
          if ((tlvType <= ::wire::icnx::CONTENT_TLV_END) && (tlvType >= ::wire::icnx::CONTENT_TLV)) 
          {
              Ptr<Data> data = Create<Data> ((dynamic_cast< wire::icnx::TLV_CONTENT * > (tlv)));
              
              return ReceiveData (data);
          } else if ((tlvType <= ::wire::icnx::INTEREST_TLV_END) && (tlvType >= ::wire::icnx::INTEREST_TLV)) 
          {
              Ptr<Interest> interest = Create<Interest> (dynamic_cast< wire::icnx::TLV_INTEREST * >(tlv));
              return ReceiveInterest (interest);
          } else if ((tlvType <= ::wire::icnx::INTERESTRESP_TLV_END) && (tlvType >= ::wire::icnx::INTERESTRESP_TLV)) 
          {
              Ptr<InterestResponse> interestResponse = Create<InterestResponse> (dynamic_cast< wire::icnx::TLV_INTERESTRESPONSE * >(tlv));
              return ReceiveInterestResponse (interestResponse);
          } 
          NS_FATAL_ERROR ("Not supported CCN header type");
          return false;

}

bool
Face::ReceiveInterest (Ptr<Interest> interest)
{
  if (!IsUp ())
    {
      // no tracing here. If we were off while receiving, we shouldn't even know that something was there
      return false;
    }

  m_upstreamInterestHandler (this, interest);
  return true;
}
bool
Face::ReceiveInterestResponse (Ptr<InterestResponse> interestResponse)
{
  if (!IsUp ())
    {
      // no tracing here. If we were off while receiving, we shouldn't even know that something was there
      return false;
    }

  m_upstreamInterestResponseHandler (this, interestResponse);
  return true;
}

bool
Face::ReceiveData (Ptr<Data> data)
{
  if (!IsUp ())
    {
      // no tracing here. If we were off while receiving, we shouldn't even know that something was there
      return false;
    }

  m_upstreamDataHandler (this, data);
  return true;
}

void
Face::SetMetric (uint16_t metric)
{
  NS_LOG_FUNCTION (metric);
  m_metric = metric;
}

uint16_t
Face::GetMetric (void) const
{
  return m_metric;
}

void
Face::SetFlags (uint32_t flags)
{
  m_flags = flags;
}

bool
Face::operator== (const Face &face) const
{
  NS_ASSERT_MSG (m_node->GetId () == face.m_node->GetId (),
                 "Faces of different nodes should not be compared to each other: " << *this << " == " << face);

  return (m_id == face.m_id);
}

bool
Face::operator< (const Face &face) const
{
  NS_ASSERT_MSG (m_node->GetId () == face.m_node->GetId (),
                 "Faces of different nodes should not be compared to each other: " << *this << " == " << face);

  return (m_id < face.m_id);
}

std::ostream&
Face::Print (std::ostream &os) const
{
  os << "id=" << GetId ();
  return os;
}

std::ostream&
operator<< (std::ostream& os, const Face &face)
{
  face.Print (os);
  return os;
}

} // namespace ccn
} // namespace ns3

