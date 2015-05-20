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
 * Authors:     Alexander Afanasyev <alexander.afanasyev@ucla.edu>
                Ilya Moiseenko <iliamo@cs.ucla.edu>
 */

#ifndef CCN_APP_FACE_H
#define CCN_APP_FACE_H

#include "ccn-face.h"
#include "ns3/traced-callback.h"
#include "ns3/ccn-common-object.h"
namespace ns3 {

class Packet;

namespace ccn {

class Interest;
class InterestResponse;
class Data;

typedef Interest InterestHeader;
typedef Interest InterestResponseHeader;
typedef Data DataHeader;

class App;

/**
 * \ingroup ccn-face
 * \brief Implementation of application Ccn face
 *
 * This class defines basic functionality of Ccn face. Face is core
 * component responsible for actual delivery of data packet to and
 * from Ccn stack
 *
 * \see AppFace, CcnNetDeviceFace, CcnIpv4Face, CcnUdpFace
 */
class AppFace  : public Face
{
public:
  static TypeId
  GetTypeId ();

  /**
   * \brief Default constructor
   */
  AppFace (Ptr<App> app);
  virtual ~AppFace();
  
  ////////////////////////////////////////////////////////////////////
  // methods overloaded from Face
  virtual bool
  SendInterest (Ptr< Interest> interest);
  virtual bool
  SendInterestResponse (Ptr< InterestResponse> interestResponse);

  virtual bool
  SendData (Ptr< Data> data);

public:
  virtual std::ostream&
  Print (std::ostream &os) const;
  ////////////////////////////////////////////////////////////////////
 
private:
  AppFace ();
  AppFace (const AppFace &); ///< \brief Disabled copy constructor
  AppFace& operator= (const AppFace &); ///< \brief Disabled copy operator

private:
  Ptr<App> m_app;
};

} // namespace ccn
} // namespace ns3

#endif // CCN_APP_FACE_H
