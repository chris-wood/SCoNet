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

// dumb-requester.cc

#include "dumb-requester.h"
#include "ns3/ptr.h"
#include "ns3/log.h"
#include "ns3/simulator.h"
#include "ns3/packet.h"
#include "ns3/random-variable.h"
#include "ns3/string.h"

#include "ns3/ccn-app-face.h"
#include "ns3/ccn-interest.h"
#include "ns3/ccn-data.h"

NS_LOG_COMPONENT_DEFINE ("DumbRequester");

namespace ns3 {

NS_OBJECT_ENSURE_REGISTERED (DumbRequester);

// register NS-3 type
TypeId
DumbRequester::GetTypeId ()
{
  static TypeId tid = TypeId ("DumbRequester")
    .SetParent<ccn::App> ()
    .AddConstructor<DumbRequester> ()

    .AddAttribute ("Prefix", "Requested name",
                   StringValue ("/dumb-interest"),
                   ccn::MakeNameAccessor (&DumbRequester::m_name),
                   ccn::MakeNameChecker ())
    ;
  return tid;
}

DumbRequester::DumbRequester ()
  : m_isRunning (false)
{
}

// Processing upon start of the application
void
DumbRequester::StartApplication ()
{
  // initialize ccn::App
  ccn::App::StartApplication ();

  m_isRunning = true;
  Simulator::ScheduleNow (&DumbRequester::SendInterest, this);
}

// Processing when application is stopped
void
DumbRequester::StopApplication ()
{
  m_isRunning = false;
  // cleanup ccn::App
  ccn::App::StopApplication ();
}

void
DumbRequester::SendInterest ()
{
  if (!m_isRunning) return;
  
  /////////////////////////////////////
  // Sending one Interest packet out //
  /////////////////////////////////////
  
  Ptr<ccn::Name> prefix = Create<ccn::Name> (m_name); // another way to create name

  // Create and configure ccn::Interest
  Ptr<ccn::Interest> interest = Create<ccn::Interest> ();
  UniformVariable rand (0,std::numeric_limits<uint32_t>::max ());
  interest->SetNonce            (rand.GetValue ());
  interest->SetName             (prefix);
  interest->SetInterestLifetime (Seconds (1.0));

  NS_LOG_DEBUG ("Sending Interest packet for " << *prefix);
  

  // Call trace (for logging purposes)
  m_transmittedInterests (interest, this, m_face);

  // Forward packet to lower (network) layer
  m_face->ReceiveInterest (interest);

  Simulator::Schedule (Seconds (1.0), &DumbRequester::SendInterest, this);
}

void
DumbRequester::OnData (Ptr<const ccn::Data> contentObject)
{
  NS_LOG_DEBUG ("Receiving Data packet for " << contentObject->GetName ());
}


} // namespace ns3
