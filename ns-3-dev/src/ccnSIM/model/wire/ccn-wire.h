/** -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil -*- */
/*
 * Copyright (c) 2013, Regents of the University of California
 *                     Alexander Afanasyev
 *
 * GNU 3.0 license, See the LICENSE file for more information
 *
 * Author: Alexander Afanasyev <alexander.afanasyev@ucla.edu>
 */

#ifndef CCN_WIRE_H
#define CCN_WIRE_H

#include "ns3/buffer.h"

#include "ns3/ccn-common.h"
#include "ns3/ccn-name.h"
#include "ns3/ccn-interest.h"
#include "ns3/ccn-interestresponse.h"
#include "ns3/ccn-data.h"
#include <ns3/ccn-common-object.h>

#include <ns3/ccnSIM/model/wire/icnx/ccn-1.0_content_tlv.h>
#include <ns3/ccnSIM/model/wire/icnx/ccn-1.0_tlv-helper-base.h>

CCN_NAMESPACE_BEGIN

struct Wire
{



  static Ptr<Packet>
  FromInterestResponse (Ptr< InterestResponse> interestResponse);

  //static Ptr<InterestResponse>
  //ToInterestResponse (Ptr<Packet> packet, int8_t wireFormat);

  static Ptr<Packet>
  FromInterest (Ptr< Interest> interest);

  //static Ptr<Interest>
  //ToInterest (Ptr<Packet> packet, int8_t wireFormat);

  static Ptr<Packet>
  FromData (Ptr< Data> data);

  //static Ptr<Data>
  //ToData (Ptr<Packet> packet, int8_t wireFormat);


  // Helper methods for Python, JLM FIXME TODO necessary?
/*  static std::string
  FromInterestStr (Ptr<const Interest> interest);

  static Ptr<Interest>
  ToInterestStr (const std::string &wire, int8_t wireFormat);

  static std::string
  FromDataStr (Ptr<const Data> data);

  static Ptr<Data>
  ToDataStr (const std::string &wire, int8_t wireFormat);
*/

};

/*inline std::string
PacketToBuffer (Ptr<const Packet> pkt)
{
  std::string buffer;
  buffer.resize (pkt->GetSize ());
  pkt->CopyData (reinterpret_cast<uint8_t*> (&buffer[0]), buffer.size ());

  return buffer;
}

inline Ptr<Packet>
BufferToPacket (const std::string &buffer)
{
  return Create<Packet> (reinterpret_cast<const uint8_t*> (&buffer[0]), buffer.size ());
}
*/

CCN_NAMESPACE_END

#endif // CCN_WIRE_H
