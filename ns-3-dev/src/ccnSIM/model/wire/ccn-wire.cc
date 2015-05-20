/** -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil -*- */
/*
 * Copyright (c) 2013, Regents of the University of California
 *                     Alexander Afanasyev
 *
 * GNU 3.0 license, See the LICENSE file for more information
 *
 * Author: Alexander Afanasyev <alexander.afanasyev@ucla.edu>
 *
 * Modified Mar 20, 2015 James Mathewson <jlmathew@soe.ucsc.edu>
 *
 */

#include "ccn-wire.h"

#include "ns3/global-value.h"
#include "ns3/integer.h"
#include "ns3/ccn-common.h"
#include "ns3/ccn-interest.h"
#include "ns3/ccn-interestresponse.h"
#include "ns3/ccn-data.h"
#include "ns3/ccn-header-helper.h"

#include "ccnx.h"

namespace ns3 {
namespace ccn {

static
GlobalValue g_wireFormat ("ccn::WireFormat",
                          "Default wire format for ccnSIM.  ccnSIM will be accepting packets ",
                          IntegerValue (ns3::ccn::SERIAL_ENC),
                          MakeIntegerChecker<int32_t> ());


Ptr<Packet>
Wire::FromInterestResponse (Ptr< InterestResponse> interestResponse)
{

    if (SERIAL_ENC == interestResponse->GetWireFormat()) {
       return interestResponse->GetPacketSerialize(); //0; //wire::icnx::Interest::ToWire (interest);
    } else if (XML_ENC == interestResponse->GetWireFormat()) {
      assert(0); //fix
      return 0;//wire::icnx::Interest::ToXmlWire(interest);
    } else 
    {
      NS_FATAL_ERROR ("Unsupported format requested");
      return 0;
    }
}
/*
Ptr<InterestResponse>
Wire::ToInterestResponse (Ptr<Packet> packet, int8_t wireFormat)
{

      try
        {
                return 0;//wire::icnx::Interest::FromWire (packet);

          // exception will be thrown if packet is not recognized
        }
      catch (UnknownHeaderException)
        {
          NS_FATAL_ERROR ("Unknown CCN header");
          return 0;
        }
}*/

 //InterestToWire 
  Ptr<Packet>
Wire::FromInterest (Ptr< Interest> interest)
{

    if (SERIAL_ENC == interest->GetWireFormat()) {
       return interest->GetPacketSerialize(); //0; //wire::icnx::Interest::ToWire (interest);
    } else if (XML_ENC == interest->GetWireFormat()) {
      assert(0);
      return 0;//wire::icnx::Interest::ToXmlWire(interest);
    } else 
    {
      NS_FATAL_ERROR ("Unsupported format requested");
      return 0;
    }
}
/*
Ptr<Interest>
Wire::ToInterest (Ptr<Packet> packet, int8_t wireFormat)
{

      try
        {
                return 0;//wire::icnx::Interest::FromWire (packet);

          // exception will be thrown if packet is not recognized
        }
      catch (UnknownHeaderException)
        {
          NS_FATAL_ERROR ("Unknown CCN header");
          return 0;
        }
}
*/
Ptr<Packet>
Wire::FromData (Ptr< Data> data)
{
  if (data->GetWireFormat() == XML_ENC ) {
      return data->GetPacketSerialize(); //0;//wire::icnx::Data::ToXmlWire(data);
    } else if (data->GetWireFormat() == SERIAL_ENC)
    {
      assert(0);
      return 0;//wire::icnx::Data::ToWire(data);

    } else
    {
      NS_FATAL_ERROR ("Unsupported format requested");
      return 0;
    }
}
/*
Ptr<Data>
Wire::ToData (Ptr<Packet> packet, int8_t wireFormat)
{

  if (wireFormat == XML_ENC ) {
    return 0;// wire::icnx::Data::FromXmlWire (packet);
  }
  else if (wireFormat == SERIAL_ENC ) {
    return 0;//wire::icnx::Data::FromWire (packet);
  }
  else
    {
      NS_FATAL_ERROR ("Unsupported format requested");
      return 0;
    }
}
*/

/*
std::string
Wire::FromInterestStr (Ptr<const Interest> interest)
{
  Ptr<Packet> pkt = FromInterest (interest);
  std::string wire;
  wire.resize (pkt->GetSize ());
  pkt->CopyData (reinterpret_cast<uint8_t*> (&wire[0]), wire.size ());

  return wire;
}

Ptr<Interest>
Wire::ToInterestStr (const std::string &wire, int8_t type)
{
  Ptr<Packet> pkt = Create<Packet> (reinterpret_cast<const uint8_t*> (&wire[0]), wire.size ());
  return ToInterest (pkt, type);
}

std::string
Wire::FromDataStr (Ptr<const Data> data)
{
  Ptr<Packet> pkt = FromData (data);
  std::string wire;
  wire.resize (pkt->GetSize ());
  pkt->CopyData (reinterpret_cast<uint8_t*> (&wire[0]), wire.size ());

  return wire;
}

Ptr<Data>
Wire::ToDataStr (const std::string &wire, int8_t type)
{
  Ptr<Packet> pkt = Create<Packet> (reinterpret_cast<const uint8_t*> (&wire[0]), wire.size ());
  return ToData (pkt, type);
}
*/

}
}
