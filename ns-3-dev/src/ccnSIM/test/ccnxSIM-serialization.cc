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
 * Author: Alexander Afanasyev <alexander.afanasyev@ucla.edu>
 *         Ilya Moiseenko <iliamo@cs.ucla.edu>
 */

#include "ns3/core-module.h"
#include "ns3/ccnSIM-module.h"
#include "ccnxSIM-serialization.h"

#include <boost/lexical_cast.hpp>
#include "ns3/ccnSIM/model/wire/ccnsim.h"

using namespace std;

namespace ns3 {

using namespace ccn;

NS_LOG_COMPONENT_DEFINE ("ccnx.Serialization");

void
CcnxInterestSerializationTest::DoRun ()
{
  Ptr<Interest> source = Create<Interest> ();

  source->SetName (Create<Name> (boost::lexical_cast<Name> ("/test/test2")));
  NS_TEST_ASSERT_MSG_EQ (source->GetName (), boost::lexical_cast<Name> ("/test/test2"), "set/get name failed");


  source->SetInterestLifetime (Seconds (100));
  NS_TEST_ASSERT_MSG_EQ (source->GetInterestLifetime (), Seconds (100), "set/get interest lifetime failed");

  source->SetInterestHopCount(13);
  NS_TEST_ASSERT_MSG_EQ (source->GetInterestHopCount(), 13, "set/get hop count failed");

  source->SetInterestPktVersion(5);
  NS_TEST_ASSERT_MSG_EQ(source->GetInterestPktVersion(), 5, "set/get version failed");


  NS_TEST_ASSERT_MSG_EQ (source->GetWire (), 0, "Wire should be empty");
  NS_TEST_ASSERT_MSG_NE (source->GetPayload (), 0, "Payload should not be empty");

  Ptr<Packet> packet = wire::ccnSIM::Interest::ToWire (source);

  NS_TEST_ASSERT_MSG_NE (source->GetWire (), 0, "Wire should not be empty now");

  Ptr<Interest> target = wire::ccnSIM::Interest::FromWire (packet);

  NS_TEST_ASSERT_MSG_EQ (source->GetName ()            , target->GetName ()            , "source/target name failed");

  NS_TEST_ASSERT_MSG_EQ (source->GetWire (), 0, "Wire should be empty");
  NS_TEST_ASSERT_MSG_NE (source->GetPayload (), 0, "Payload should not be empty");

  //test string name

  //test name-nonce

  //test name-hash

  //test raw TLV manipulation


  //test xml TLV manipulation



}

void
CcnxDataSerializationTest::DoRun ()
{
  Ptr<Data> source = Create<Data> (Create<Packet> (1024));

  source->SetName (Create<Name> (boost::lexical_cast<Name> ("/test/test2/1")));
  NS_TEST_ASSERT_MSG_EQ (source->GetName (), boost::lexical_cast<Name> ("/test/test2/1"), "set/get name failed");


  source->SetTimestamp (Seconds (100));
  NS_TEST_ASSERT_MSG_EQ (source->GetTimestamp (), Seconds (100), "set/get timestamp failed");

  NS_TEST_ASSERT_MSG_EQ (source->GetSignature (), 0, "initialization of signature failed");

  Ptr<Packet> packet = wire::ccnSIM::Data::ToWire (source);
  int size = packet->GetSize ();

  source->SetSignature (10);
  NS_TEST_ASSERT_MSG_EQ (source->GetSignature (), 10, "set/get signature failed");

  packet = wire::ccnSIM::Data::ToWire (source);
  NS_TEST_ASSERT_MSG_EQ (packet->GetSize (), static_cast<unsigned int> (size + 4), "Signature size should have increased by 4");

  Ptr<Data> target = wire::ccnSIM::Data::FromWire (packet);

  NS_TEST_ASSERT_MSG_EQ (source->GetName ()     , target->GetName ()     , "source/target name failed");
  NS_TEST_ASSERT_MSG_EQ (source->GetTimestamp (), target->GetTimestamp (), "source/target timestamp failed");
  NS_TEST_ASSERT_MSG_EQ (source->GetSignature (), target->GetSignature (), "source/target signature failed");
}

}
