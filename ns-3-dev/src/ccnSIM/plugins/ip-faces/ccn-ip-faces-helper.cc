/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil -*- */
/*
 * Copyright (c) 2013 University of California, Los Angeles
 *                    Alexander Afanasyev
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
 * Author:  Alexander Afanasyev <alexander.afanasyev@ucla.edu>
 */

#include "ccn-ip-faces-helper.h"
#include "ccn-ip-face-stack.h"

#include "ns3/ccn-stack-helper.h"
#include "ns3/node-container.h"
#include "ns3/log.h"
#include "ns3/simulator.h"
#include "ccn-tcp-face.h"
#include "ccn-udp-face.h"

NS_LOG_COMPONENT_DEFINE ("ccn.IpFacesHelper");

using namespace std;

namespace ns3 {
namespace ccn {

void
IpFacesHelper::Install (Ptr<Node> node)
{
  Ptr<IpFaceStack> stack = CreateObject<IpFaceStack> ();
  node->AggregateObject (stack);
}

void
IpFacesHelper::Install (const NodeContainer &nodes)
{
  for (NodeContainer::Iterator node = nodes.Begin ();
       node != nodes.End ();
       node ++)
    {
      Install (*node);
    }
}

void
IpFacesHelper::InstallAll ()
{
  Install (NodeContainer::GetGlobal ());
}


struct TcpPrefixRegistrator : SimpleRefCount<TcpPrefixRegistrator>
{
  TcpPrefixRegistrator (Ptr<Node> node, const std::string &prefix, int16_t metric, int32_t rank)
    : m_node (node)
    , m_prefix (prefix)
    , m_metric (metric)
  	, m_rank (rank)
  {
  }

  void
  Run (Ptr<Face> face)
  {
    ccn::StackHelper::AddRoute (m_node, m_prefix, face, m_metric, m_rank);
  }
private:
  Ptr<Node> m_node;
  std::string m_prefix;
  int16_t m_metric;
  int32_t m_rank;
};

static void
ScheduledCreateTcp (Ptr<Node> node, Ipv4Address address, const std::string &prefix, int16_t metric, int32_t rank)
{
  Ptr<IpFaceStack> stack = node->GetObject<IpFaceStack> ();
  NS_ASSERT_MSG (stack != 0, "ccn::IpFaceStack needs to be installed on the node");

  Ptr<Face> face = stack->GetTcpFaceByAddress (address);
  if (face == 0)
    {
      Ptr<TcpPrefixRegistrator> registrator = Create<TcpPrefixRegistrator> (node, prefix, metric, rank);
      stack->CreateOrGetTcpFace (address, MakeCallback (&TcpPrefixRegistrator::Run, registrator));
    }
  else
    {
      ccn::StackHelper::AddRoute (node, prefix, face, metric, rank);
    }
}

void
IpFacesHelper::CreateTcpFace (const Time &when, Ptr<Node> node, Ipv4Address address, const std::string &prefix, int16_t metric/* = 1*/, int32_t rank/* = 1*/)
{
  Simulator::ScheduleWithContext (node->GetId (), when, ScheduledCreateTcp, node, address, prefix, metric, rank);
}

static void
ScheduledCreateUdp (Ptr<Node> node, Ipv4Address address, const std::string &prefix, int16_t metric, int32_t rank)
{
  Ptr<IpFaceStack> stack = node->GetObject<IpFaceStack> ();
  NS_ASSERT_MSG (stack != 0, "ccn::IpFaceStack needs to be installed on the node");

  Ptr<Face> face = stack->CreateOrGetUdpFace (address);
  ccn::StackHelper::AddRoute (node, prefix, face, metric, rank);
}

void
IpFacesHelper::CreateUdpFace (const Time &when, Ptr<Node> node, Ipv4Address address, const std::string &prefix, int16_t metric/* = 1*/, int32_t rank/* = 1*/)
{
  Simulator::ScheduleWithContext (node->GetId (), when, ScheduledCreateUdp, node, address, prefix, metric, rank);
}

} // namespace ccn
} // namespace ns3
