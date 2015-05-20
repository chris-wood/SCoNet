/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil -*- */
/*
 * Copyright (c) 2013, Regents of the University of California
 *                     Alexander Afanasyev
 *
 * GNU v3.0 license, See the LICENSE file for more information
 *
 * Author: Alexander Afanasyev <alexander.afanasyev@ucla.edu>
 */

#include "ccnSIM-api.h"
#include "ns3/core-module.h"
#include "ns3/ccnSIM-module.h"
#include "ns3/point-to-point-module.h"

#include <boost/lexical_cast.hpp>

NS_LOG_COMPONENT_DEFINE ("ccn.ApiTest");

namespace ns3
{

class ApiTestClient : public Application
{
public:
  static TypeId
  GetTypeId ()
  {
    static TypeId tid = TypeId ("ns3::ccn::test::ApiTestClient")
      .SetParent<Application> ()
      .AddConstructor<ApiTestClient> ()
      ;
    
    return tid;
  }

  ApiTestClient ()
    : datas (0)
    , timeouts (0)
  {
  }
  
protected:
  void
  StartApplication ()
  {
    m_face = Create<ccn::ApiFace> (GetNode ());
    
    Simulator::Schedule (Seconds (0.1), &ApiTestClient::SendPacket, this, std::string ("/1"));
    Simulator::Schedule (Seconds (5.0), &ApiTestClient::SendPacket, this, std::string ("/2"));
  }

  void
  StopApplication ()
  {
    m_face->Shutdown ();
    m_face = 0;
  }

private:
  void
  GotData (Ptr<const ccn::Interest>, Ptr<const ccn::Data>)
  {
    datas++;
  }

  void
  GotTimeout (Ptr<const ccn::Interest>)
  {
    timeouts++;
  }

  void
  SendPacket (const std::string &prefix)
  {
    Ptr<ccn::Interest> interest = Create<ccn::Interest> ();
    interest->SetName (Create<ccn::Name> (prefix));
    interest->SetInterestLifetime (Seconds (0.5));

    m_face->ExpressInterest (interest,
                             MakeCallback (&ApiTestClient::GotData, this),
                             MakeCallback (&ApiTestClient::GotTimeout, this));
  }

public:
  uint32_t datas;
  uint32_t timeouts;
  
private:
  Ptr<ccn::ApiFace> m_face;
};

NS_OBJECT_ENSURE_REGISTERED (ApiTestClient);

void
ApiTest::Check0 (Ptr<Application> app)
{
  NS_TEST_ASSERT_MSG_EQ (DynamicCast<ApiTestClient> (app)->datas, 0, "");
  NS_TEST_ASSERT_MSG_EQ (DynamicCast<ApiTestClient> (app)->timeouts, 0, "");
}

void
ApiTest::Check1 (Ptr<Application> app)
{
  NS_TEST_ASSERT_MSG_EQ (DynamicCast<ApiTestClient> (app)->datas, 1, "");
  NS_TEST_ASSERT_MSG_EQ (DynamicCast<ApiTestClient> (app)->timeouts, 0, "");
}

void
ApiTest::Check2 (Ptr<Application> app)
{
  NS_TEST_ASSERT_MSG_EQ (DynamicCast<ApiTestClient> (app)->datas, 1, "");
  NS_TEST_ASSERT_MSG_EQ (DynamicCast<ApiTestClient> (app)->timeouts, 1, "");
}


void
ApiTest::DoRun ()
{
  Config::SetDefault ("ns3::PointToPointNetDevice::DataRate", StringValue ("1Mbps"));
  Config::SetDefault ("ns3::PointToPointChannel::Delay", StringValue ("10ms"));
  Config::SetDefault ("ns3::DropTailQueue::MaxPackets", StringValue ("20"));

  // Creating nodes
  NodeContainer nodes;
  nodes.Create (3);

  // Connecting nodes using two links
  PointToPointHelper p2p;
  p2p.Install (nodes.Get (0), nodes.Get (1));
  p2p.Install (nodes.Get (1), nodes.Get (2));

  // Install CCN stack on all nodes
  ccn::StackHelper ccnHelper;
  ccnHelper.SetDefaultRoutes (true);
  ccnHelper.InstallAll ();

  // Installing applications

  // Consumer
  ccn::AppHelper consumerHelper ("ns3::ccn::test::ApiTestClient");
  ApplicationContainer apps = consumerHelper.Install (nodes.Get (0)); // first node

  // Producer
  ccn::AppHelper producerHelper ("ns3::ccn::Producer");
  // Producer will reply to all requests starting with /prefix
  producerHelper.SetPrefix ("/");
  producerHelper.SetAttribute ("Postfix", StringValue ("/unique/postfix"));
  producerHelper.SetAttribute ("PayloadSize", StringValue("1024"));
  producerHelper.Install (nodes.Get (2)).Stop (Seconds (4.0)); // last node

  Simulator::Schedule (Seconds (0.0001), &ApiTest::Check0, this, apps.Get (0));
  Simulator::Schedule (Seconds (0.2000), &ApiTest::Check1, this, apps.Get (0));
  Simulator::Schedule (Seconds (5.6100), &ApiTest::Check2, this, apps.Get (0));

  Simulator::Stop (Seconds (20.0));

  Simulator::Run ();
  Simulator::Destroy ();
}

}
