/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil -*- */
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
 * Authors: Alexander Afanasyev <alexander.afanasyev@ucla.edu>
 */

#ifndef CCN_UDP_FACE_H
#define CCN_UDP_FACE_H

#include "ns3/ccn-face.h"
#include "ns3/socket.h"
#include "ns3/ptr.h"

#include <map>

namespace ns3 {
namespace ccn {
  
/**
 * \ingroup ccn-face
 * \brief Implementation of UDP/IP CCN face
 *
 * \see ccn::AppFace, ccn::NetDeviceFace, ccn::Ipv4Face, ccn::TcpFace
 */
class UdpFace : public Face
{
public:
  static TypeId
  GetTypeId ();
  
  /**
   * \brief Constructor
   *
   * @param node Node associated with the face
   */
  UdpFace (Ptr<Node> node, Ptr<Socket> socket, Ipv4Address address);
  virtual ~UdpFace();

  Ipv4Address
  GetAddress () const;

  virtual bool
  ReceiveFromUdp (Ptr<const Packet> p);

  ////////////////////////////////////////////////////////////////////
  // methods overloaded from ccn::Face
  virtual std::ostream&
  Print (std::ostream &os) const;

protected:
  // also from ccn::Face
  virtual bool
  Send (Ptr<Packet> p);

private:  
  UdpFace (const UdpFace &); ///< \brief Disabled copy constructor
  UdpFace& operator= (const UdpFace &); ///< \brief Disabled copy operator

private:
  Ptr<Socket> m_socket;
  Ipv4Address m_address;
};

} // namespace ccn
} // namespace ns3

#endif // CCN_UDP_FACE_H
