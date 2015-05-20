/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2013 University of California, Los Angeles
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
 *       : Saran Tarnoi <saran.tarnoi@gmail.com>
 */

#include "ccn-link-control-helper.h"

#include "ns3/assert.h"
#include "ns3/names.h"
#include "ns3/point-to-point-net-device.h"
#include "ns3/point-to-point-channel.h"
#include "ns3/channel.h"
#include "ns3/log.h"

#include "ns3/ccn-l3-protocol.h"
#include "ns3/ccn-net-device-face.h"

NS_LOG_COMPONENT_DEFINE ("ccn.LinkControlHelper");

namespace ns3 {
namespace ccn {

void
LinkControlHelper::FailLink (Ptr<Node> node1, Ptr<Node> node2)
{
  NS_LOG_FUNCTION (node1 << node2);
  
  NS_ASSERT (node1 != 0);
  NS_ASSERT (node2 != 0);
  
  Ptr<ccn::L3Protocol> ccn1 = node1->GetObject<ccn::L3Protocol> ();
  Ptr<ccn::L3Protocol> ccn2 = node2->GetObject<ccn::L3Protocol> ();

  NS_ASSERT (ccn1 != 0);
  NS_ASSERT (ccn2 != 0);

  // iterate over all faces to find the right one
  for (uint32_t faceId = 0; faceId < ccn1->GetNFaces (); faceId++)
    {
      Ptr<ccn::NetDeviceFace> ndFace = ccn1->GetFace (faceId)->GetObject<ccn::NetDeviceFace> ();
      if (ndFace == 0) continue;

      Ptr<PointToPointNetDevice> nd1 = ndFace->GetNetDevice ()->GetObject<PointToPointNetDevice> ();
      if (nd1 == 0) continue;

      Ptr<Channel> channel = nd1->GetChannel ();
      if (channel == 0) continue;

      Ptr<PointToPointChannel> ppChannel = DynamicCast<PointToPointChannel> (channel);

      Ptr<NetDevice> nd2 = ppChannel->GetDevice (0);
      if (nd2->GetNode () == node1)
        nd2 = ppChannel->GetDevice (1);

      if (nd2->GetNode () == node2)
        {
          Ptr<ccn::Face> face1 = ccn1->GetFaceByNetDevice (nd1);
          Ptr<ccn::Face> face2 = ccn2->GetFaceByNetDevice (nd2);

          face1->SetUp (false);
          face2->SetUp (false);
          break;
        }
    }
}
void
LinkControlHelper::FailLinkByName (const std::string &node1, const std::string &node2)
{
  FailLink (Names::Find<Node> (node1), Names::Find<Node> (node2));
}

void
LinkControlHelper::UpLink (Ptr<Node> node1, Ptr<Node> node2)
{
  NS_LOG_FUNCTION (node1 << node2);

  NS_ASSERT (node1 != 0);
  NS_ASSERT (node2 != 0);
  
  Ptr<ccn::L3Protocol> ccn1 = node1->GetObject<ccn::L3Protocol> ();
  Ptr<ccn::L3Protocol> ccn2 = node2->GetObject<ccn::L3Protocol> ();

  NS_ASSERT (ccn1 != 0);
  NS_ASSERT (ccn2 != 0);

  // iterate over all faces to find the right one
  for (uint32_t faceId = 0; faceId < ccn1->GetNFaces (); faceId++)
    {
      Ptr<ccn::NetDeviceFace> ndFace = ccn1->GetFace (faceId)->GetObject<ccn::NetDeviceFace> ();
      if (ndFace == 0) continue;

      Ptr<PointToPointNetDevice> nd1 = ndFace->GetNetDevice ()->GetObject<PointToPointNetDevice> ();
      if (nd1 == 0) continue;

      Ptr<Channel> channel = nd1->GetChannel ();
      if (channel == 0) continue;

      Ptr<PointToPointChannel> ppChannel = DynamicCast<PointToPointChannel> (channel);

      Ptr<NetDevice> nd2 = ppChannel->GetDevice (0);
      if (nd2->GetNode () == node1)
        nd2 = ppChannel->GetDevice (1);

      if (nd2->GetNode () == node2)
        {
          Ptr<ccn::Face> face1 = ccn1->GetFaceByNetDevice (nd1);
          Ptr<ccn::Face> face2 = ccn2->GetFaceByNetDevice (nd2);

          face1->SetUp (true);
          face2->SetUp (true);
          break;
        }
    }
}

void
LinkControlHelper::UpLinkByName (const std::string &node1, const std::string &node2)
{
  UpLink (Names::Find<Node> (node1), Names::Find<Node> (node2));
}

}
}
