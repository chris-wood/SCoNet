/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2011,2012 University of California, Los Angeles
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
 */

#include "ccnSIM-pit.h"
#include "ns3/core-module.h"
#include "ns3/ccnSIM-module.h"
#include "ns3/point-to-point-module.h"

#include <boost/lexical_cast.hpp>

NS_LOG_COMPONENT_DEFINE ("ccn.PitTest");

namespace ns3
{

class PitTestClient : public ccn::App
{
protected:
  void
  StartApplication ()
  {
    ccn::App::StartApplication ();

    // add default route
    Ptr<ccn::fib::Entry> fibEntry = GetNode ()->GetObject<ccn::Fib> ()->Add (ccn::Name ("/"), m_face, 0,0);
    fibEntry->UpdateStatus (m_face, ccn::fib::FaceMetric::CCN_FIB_GREEN);
    
    Simulator::Schedule (Seconds (0.1), &PitTestClient::SendPacket, this, std::string("/1"), 1);
    Simulator::Schedule (Seconds (0.2), &PitTestClient::SendPacket, this, std::string("/2"), 1);
    Simulator::Schedule (Seconds (0.3), &PitTestClient::SendPacket, this, std::string("/3"), 1);
    Simulator::Schedule (Seconds (0.4), &PitTestClient::SendPacket, this, std::string("/1"), 2);
  }

  void
  StopApplication ()
  {
    ccn::App::StopApplication ();
  }

private:
  void
  SendPacket (const std::string &prefix, uint32_t nonce)
  {
    Ptr<ccn::Interest> interest = Create<ccn::Interest> ();
    interest->SetName (Create<ccn::Name> (prefix));
    interest->SetNonce (nonce);
    interest->SetInterestLifetime (Seconds (0.5));

    m_face->ReceiveInterest (interest);
  }
};

void
PitTest::Test (Ptr<ccn::Fib> fib)
{
  NS_TEST_ASSERT_MSG_EQ (fib->GetSize (), 1, "There should be only one entry");

  Ptr<const ccn::fib::Entry> fibEntry = fib->Begin ();
  NS_TEST_ASSERT_MSG_EQ (fibEntry->GetPrefix (), ccn::Name ("/"), "prefix should be /");
}

void
PitTest::Check0 (Ptr<ccn::Pit> pit)
{
  // NS_LOG_DEBUG (*GetNode ()->GetObject<ccn::Pit> ());
  NS_TEST_ASSERT_MSG_EQ (pit->GetSize (), 0, "There should 0 entries in PIT");
}

void
PitTest::Check1 (Ptr<ccn::Pit> pit)
{
  NS_TEST_ASSERT_MSG_EQ (pit->GetSize (), 1, "There should 1 entry in PIT");
}

void
PitTest::Check2 (Ptr<ccn::Pit> pit)
{
  // NS_LOG_DEBUG (*GetNode ()->GetObject<ccn::Pit> ());
  NS_TEST_ASSERT_MSG_EQ (pit->GetSize (), 2, "There should 2 entries in PIT");
}

void
PitTest::Check3 (Ptr<ccn::Pit> pit)
{
  // NS_LOG_DEBUG (*GetNode ()->GetObject<ccn::Pit> ());
  NS_TEST_ASSERT_MSG_EQ (pit->GetSize (), 3, "There should 3 entries in PIT");
}


void
PitTest::DoRun ()
{
  Ptr<Node> node = CreateObject<Node> ();
  Ptr<Node> nodeSink = CreateObject<Node> ();
  PointToPointHelper p2p;
  p2p.Install (node, nodeSink);
  
  ccn::StackHelper ccn;
  ccn.Install (node);
  ccn.Install (nodeSink);

  ccn::StackHelper::AddRoute (node, "/", 0, 0,0);

  Ptr<Application> app1 = CreateObject<PitTestClient> ();
  app1->SetStartTime (Seconds (0.0));
  node->AddApplication (app1);

  Simulator::Schedule (Seconds (0.0001), &PitTest::Test, this, node->GetObject<ccn::Fib> ());
    
  Simulator::Schedule (Seconds (0.01), &PitTest::Check0, this, node->GetObject<ccn::Pit> ());

  Simulator::Schedule (Seconds (0.11), &PitTest::Check1, this, node->GetObject<ccn::Pit> ());
  Simulator::Schedule (Seconds (0.21), &PitTest::Check2, this, node->GetObject<ccn::Pit> ());
  Simulator::Schedule (Seconds (0.31), &PitTest::Check3, this, node->GetObject<ccn::Pit> ());

  Simulator::Schedule (Seconds (0.61), &PitTest::Check3, this, node->GetObject<ccn::Pit> ());
  Simulator::Schedule (Seconds (0.71), &PitTest::Check2, this, node->GetObject<ccn::Pit> ());
  Simulator::Schedule (Seconds (0.81), &PitTest::Check1, this, node->GetObject<ccn::Pit> ());

  Simulator::Schedule (Seconds (0.91), &PitTest::Check0, this, node->GetObject<ccn::Pit> ());

  Simulator::Stop (Seconds (10.0));
  Simulator::Run ();
  Simulator::Destroy ();
}

}
