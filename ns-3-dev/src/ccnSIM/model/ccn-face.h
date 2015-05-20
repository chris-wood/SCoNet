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
 * Authors: Alexander Afanasyev <alexander.afanasyev@ucla.edu>
 */

#ifndef CCN_FACE_H
#define CCN_FACE_H

#include <ostream>
#include <algorithm>

#include "ns3/ptr.h"
#include "ns3/object.h"
#include "ns3/nstime.h"
#include "ns3/type-id.h"
#include "ns3/traced-callback.h"
#include "ns3/ccn-name.h"

#include "ns3/ccn-interestresponse.h"
#include <ns3/ccnSIM/model/wire/icnx/ccn-1.0_tlv-id.h>
#include <ns3/ccnSIM/model/wire/icnx/tlv_base.h>
#include <ns3/ccnSIM/model/wire/icnx/ccn-1.0_content_tlv.h>
#include <ns3/ccnSIM/model/wire/icnx/ccn-1.0_interest_tlv.h>
#include <ns3/ccnSIM/model/wire/icnx/ccn-1.0_interestresponse_tlv.h>
#include <ns3/ccnSIM/model/wire/icnx/ccn-1.0_tlv-helper-base.h>
#include <ns3/ccnSIM/model/wire/icnx/tlv_factory.h>

namespace ns3 {

class Packet;
class Node;

namespace ccn {

class Interest;
//class InterestResponse;
class Data;

/**
 * \ingroup ccn
 * \defgroup ccn-face Faces
 */
/**
 * \ingroup ccn-face
 * \brief Virtual class defining CCN face
 *
 * This class defines basic functionality of CCN face. Face is core
 * component responsible for actual delivery of data packet to and
 * from CCN stack
 *
 * \see ccn::AppFace, ccn::NetDeviceFace
 */
class Face :
    public Object
{
public:
  static TypeId
  GetTypeId ();

  /**
   * \brief CCN protocol handlers
   *
   * \param face Face from which packet has been received
   * \param packet Original packet
   */
  typedef Callback<void, Ptr<Face>, Ptr<Interest> > InterestHandler;
  typedef Callback<void, Ptr<Face>, Ptr<InterestResponse> > InterestResponseHandler;
  typedef Callback<void, Ptr<Face>, Ptr<Data> > DataHandler;

  /**
   * \brief Default constructor
   */
  Face (Ptr<Node> node);
  virtual ~Face();

  /**
   * @brief Get node to which this face is associated
   */
  Ptr<Node>
  GetNode () const;

  ////////////////////////////////////////////////////////////////////

  /**
   * \brief Register callback to call when new packet arrives on the face
   *
   * This method should call protocol-dependent registration function
   */
  virtual void
  RegisterProtocolHandlers (const InterestHandler &interestHandler, const DataHandler &dataHandler, const InterestResponseHandler &interestResponseHandler);

  /**
   * \brief Un-Register callback to call when new packet arrives on the face
   *
   * This method should call protocol-dependent registration function
   */
  virtual void
  UnRegisterProtocolHandlers ();

  /**
   * @brief Send out interest through the face
   * @param interest Interest to send out
   * @param packet "payload" that is attached to the interest (can carry some packet tags)
   *
   * @returns true if interest is considered to be send out (enqueued)
   */
  virtual bool
  SendInterest (Ptr< Interest> interest);

  virtual bool
  SendInterestResponse (Ptr< InterestResponse> interestResponse);
  

  /**
   * @brief Send out Dat packet through the face
   * @param data Data packet to send out
   * @param packet Data packet payload, can also carry packet tags
   *
   * @returns true if Data packet is considered to be send out (enqueued)
   */
  virtual bool
  SendData (Ptr< Data> data);

  /**
   * \brief Receive interest from application or another node and forward it up to the CCN stack
   *
   * By default it is called from inside Receive method, but can be used directly, if appropriate
   */
  virtual bool
  ReceiveInterest (Ptr<Interest> interest);
  virtual bool
  ReceiveInterestResponse (Ptr<InterestResponse> interestResponse);

  /**
   * \brief Receive Data packet from application or another node and forward it up to the CCN stack
   *
   * By default it is called from inside Receive method, but can be used directly, if appropriate
   */
  virtual bool
  ReceiveData (Ptr<Data> data);
  ////////////////////////////////////////////////////////////////////

  /**
   * \brief Assign routing/forwarding metric with face
   *
   * \param metric configured routing metric (cost) of this face
   */
  virtual void
  SetMetric (uint16_t metric);

  /**
   * \brief Get routing/forwarding metric assigned to the face
   *
   * \returns configured routing/forwarding metric (cost) of this face
   */
  virtual uint16_t
  GetMetric (void) const;

  /**
   * These are face states and may be distinct from actual lower-layer
   * device states, such as found in real implementations (where the
   * device may be down but ccn face state is still up).
   */

  /**
   * \brief Enable or disable this face
   */
  inline void
  SetUp (bool up = true);

  /**
   * \brief Returns true if this face is enabled, false otherwise.
   */
  inline bool
  IsUp () const;

  /**
   * @brief Get face flags
   *
   * Face flags may indicate various properties of the face.  For example, if the face is an application face,
   * than the returned flags have Face::APPLICATION bit set.
   *
   * @see ccn::Face::Flags for the list of currently defined face flags
   */
  inline uint32_t
  GetFlags () const;

  /**
   * @brief List of currently defined face flags
   */
  enum Flags
    {
      APPLICATION = 1 ///< @brief An application face
    };

  /**
   * @brief Print information about the face into the stream
   * @param os stream to write information to
   */
  virtual std::ostream&
  Print (std::ostream &os) const;

  /**
   * \brief Set face Id
   *
   * Id is purely informative and should not be used for any other purpose
   *
   * \param id id to set
   */
  inline void
  SetId (uint32_t id);

  /**
   * \brief Get face Id
   *
   * Id is purely informative and should not be used for any other purpose
   *
   * \returns id id to set
   */
  inline uint32_t
  GetId () const;

  /**
   * \brief Compare two faces. Only two faces on the same node could be compared.
   *
   * Internal index is used for comparison.
   */
  bool
  operator== (const Face &face) const;

  /**
   * \brief Compare two faces. Only two faces on the same node could be compared.
   *
   * Internal index is used for comparison.
   */
  inline bool
  operator!= (const Face &face) const;

  /**
   * \brief Compare two faces. Only two faces on the same node could be compared.
   *
   * Internal index is used for comparison.
   */
  bool
  operator< (const Face &face) const;

protected:
  /**
   * @brief Send packet down to the stack (towards app or network)
   * Assumes Packet only contains the encoding method and TLV, no other
   * irreleant packet information.   
   */
  virtual bool
  Send (Ptr< Packet> packet);
  //virtual bool
  //Send (wire::icnx::TLV_BASE *tlv, WireFormat format);

  /**
   * @brief Send packet up to the stack (towards forwarding strategy)
   * Assumes Packet only contains the encoding method and TLV, no other
   * irreleant packet information.   Other encapsulation information should
   * have been removed

   */
  virtual bool
  Receive (Ptr<const Packet> p);

  /**
   * @brief Set face flags
   */
  void
  SetFlags (uint32_t flags);

private:
  Face (const Face &); ///< \brief Disabled copy constructor
  Face& operator= (const Face &); ///< \brief Disabled copy operator

protected:
  Ptr<Node> m_node; ///< \brief Smart pointer to Node

private:
  InterestHandler m_upstreamInterestHandler;
  InterestResponseHandler m_upstreamInterestResponseHandler;
  DataHandler m_upstreamDataHandler;
  bool m_ifup;
  uint32_t m_id; ///< \brief id of the interface in CCN stack (per-node uniqueness)
  uint16_t m_metric; ///< \brief metric of the face
  uint32_t m_flags; ///< @brief faces flags (e.g., APPLICATION)
};

std::ostream&
operator<< (std::ostream& os, const Face &face);

inline bool
Face::IsUp (void) const
{
  return m_ifup;
}

inline void
Face::SetUp (bool up/* = true*/)
{
  m_ifup = up;
}

inline uint32_t
Face::GetFlags () const
{
  return m_flags;
}

inline bool
operator < (const Ptr<Face> &lhs, const Ptr<Face> &rhs)
{
  return *lhs < *rhs;
}

void
Face::SetId (uint32_t id)
{
  m_id = id;
}

uint32_t
Face::GetId () const
{
  return m_id;
}

inline bool
Face::operator!= (const Face &face) const
{
  return !(*this == face);
}

} // namespace ccn
} // namespace ns3

#endif // CCN_FACE_H
