/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2011-2012 University of California, Los Angeles
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
// ccn-grid.cc
#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/point-to-point-layout-module.h"
#include "ns3/ccnSIM-module.h"
#include "ns3/packet.h"

using namespace ns3;

/**
 * This scenario simulates a grid topology (using PointToPointGrid module)
 *
 * (consumer) -- ( ) ----- ( )
 *     |          |         |
 *    ( ) ------ ( ) ----- ( )
 *     |          |         |
 *    ( ) ------ ( ) -- (producer)
 *
 * All links are 1Mbps with propagation 10ms delay. 
 *
 * FIB is populated using CcnGlobalRoutingHelper.
 *
 * Consumer requests data from producer with frequency 100 interests per second
 * (interests contain constantly increasing sequence number).
 *
 * For every received interest, producer replies with a data packet, containing
 * 1024 bytes of virtual payload.
 *
 * To run scenario and see what is happening, use the following command:
 *
 *     NS_LOG=ccn.Consumer:ccn.Producer ./waf --run=ccn-grid
 */

int
main (int argc, char *argv[])
{
  // Setting default parameters for PointToPoint links and channels
  Config::SetDefault ("ns3::PointToPointNetDevice::DataRate", StringValue ("1Mbps"));
  Config::SetDefault ("ns3::PointToPointChannel::Delay", StringValue ("10ms"));
  Config::SetDefault ("ns3::DropTailQueue::MaxPackets", StringValue ("10"));

  // Read optional command-line parameters (e.g., enable visualizer with ./waf --run=<> --visualize
  CommandLine cmd;
  cmd.Parse (argc, argv);

  // Creating 3x3 topology
  PointToPointHelper p2p;
  PointToPointGridHelper grid (3, 3, p2p);
  grid.BoundingBox(100,100,200,200);

  Packet::EnablePrinting ();
Packet::EnableChecking ();

  // Install CCN stack on all nodes
  ccn::StackHelper ccnHelper;
  ccnHelper.SetForwardingStrategy ("ns3::ccn::fw::BestRoute");
  ccnHelper.InstallAll ();

  //ccnHelper.EnablePcapAll ("prefix");
AsciiTraceHelper ascii;
  p2p.EnableAsciiAll (ascii.CreateFileStream ("myfirst.tr"));
p2p.EnablePcapAll ("myfirst");

  // Installing global routing interface on all nodes
  ccn::GlobalRoutingHelper ccnGlobalRoutingHelper;
  ccnGlobalRoutingHelper.InstallAll ();

  // Getting containers for the consumer/producer
  Ptr<Node> producer = grid.GetNode (2, 2);
  NodeContainer consumerNodes;
  consumerNodes.Add (grid.GetNode (0,0));

  // Install CCN applications
  std::string prefix = "/prefix";

  ccn::AppHelper consumerHelper ("ns3::ccn::ConsumerCbr");
  consumerHelper.SetPrefix (prefix);
  consumerHelper.SetAttribute ("Frequency", StringValue ("5")); // 100 interests a second
  consumerHelper.Install (consumerNodes);

  ccn::AppHelper producerHelper ("ns3::ccn::Producer");
  producerHelper.SetPrefix (prefix);
  producerHelper.SetAttribute ("PayloadSize", StringValue("1024"));
  producerHelper.Install (producer);

  // Add /prefix origins to ccn::GlobalRouter
  ccnGlobalRoutingHelper.AddOrigins (prefix, producer);

  // Calculate and install FIBs
  ccn::GlobalRoutingHelper::CalculateRoutes ();

  Simulator::Stop (Seconds (10.0));
  ccn::L3AggregateTracer::InstallAll ("aggregate-trace.txt", Seconds (0.1));
  ccn::L3RateTracer::InstallAll ("rate-trace.txt", Seconds (0.1));
  Simulator::Run ();
  Simulator::Destroy ();

  return 0;
}
