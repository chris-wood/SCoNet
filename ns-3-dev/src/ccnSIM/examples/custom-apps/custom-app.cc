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

// custom-app.cc

#include "custom-app.h"
#include "ns3/ptr.h"
#include "ns3/log.h"
#include "ns3/simulator.h"
#include "ns3/packet.h"

#include "ns3/ccn-app-face.h"
#include "ns3/ccn-interest.h"
#include "ns3/ccn-data.h"

#include "ns3/ccn-fib.h"
#include "ns3/random-variable.h"

NS_LOG_COMPONENT_DEFINE ("CustomApp");

namespace ns3 {

NS_OBJECT_ENSURE_REGISTERED (CustomApp);

// register NS-3 type
TypeId
CustomApp::GetTypeId ()
{
  static TypeId tid = TypeId ("CustomApp")
    .SetParent<ccn::App> ()
    .AddConstructor<CustomApp> ()
    ;
  return tid;
}

// Processing upon start of the application
void
CustomApp::StartApplication ()
{
  // initialize ccn::App
  ccn::App::StartApplication ();

  // Create a name components object for name ``/prefix/sub``
  Ptr<ccn::Name> prefix = Create<ccn::Name> (); // now prefix contains ``/``
  prefix->append ("prefix"); // now prefix contains ``/prefix``
  prefix->append ("sub"); // now prefix contains ``/prefix/sub``

  /////////////////////////////////////////////////////////////////////////////
  // Creating FIB entry that ensures that we will receive incoming Interests //
  /////////////////////////////////////////////////////////////////////////////

  // Get FIB object
  Ptr<ccn::Fib> fib = GetNode ()->GetObject<ccn::Fib> ();

  // Add entry to FIB
  // Note that ``m_face`` is cretaed by ccn::App
  Ptr<ccn::fib::Entry> fibEntry = fib->Add (*prefix, m_face, 0, 0);

  Simulator::Schedule (Seconds (1.0), &CustomApp::SendInterest, this);
}

// Processing when application is stopped
void
CustomApp::StopApplication ()
{
  // cleanup ccn::App
  ccn::App::StopApplication ();
}

void
CustomApp::SendInterest ()
{
  /////////////////////////////////////
  // Sending one Interest packet out //
  /////////////////////////////////////
  
  Ptr<ccn::Name> prefix = Create<ccn::Name> ("/prefix/sub"); // another way to create name

  // Create and configure ccn::Interest
  Ptr<ccn::Interest> interest = Create<ccn::Interest> ();
  UniformVariable rand (0,std::numeric_limits<uint32_t>::max ());
  interest->SetNonce            (rand.GetValue ());
  interest->SetName             (prefix);
  interest->SetInterestLifetime (Seconds (1.0));

  NS_LOG_DEBUG ("Sending Interest packet for " << *prefix);
  
  // Call trace (for logging purposes)
  m_transmittedInterests (interest, this, m_face);

  m_face->ReceiveInterest (interest);
}

// Callback that will be called when Interest arrives
void
CustomApp::OnInterest (Ptr<const ccn::Interest> interest)
{
  ccn::App::OnInterest (interest);
  
  NS_LOG_DEBUG ("Received Interest packet for " << interest->GetName ());

  // Create and configure ccn::Data and ccn::DataTail
  // (header is added in front of the packet, tail is added at the end of the packet)

  // Note that Interests send out by the app will not be sent back to the app !
  
  Ptr<ccn::Data> data = Create<ccn::Data> (Create<Packet> (1024));
  data->SetName (Create<ccn::Name> (interest->GetName ())); // data will have the same name as Interests

  NS_LOG_DEBUG ("Sending Data packet for " << data->GetName ());  

  // Call trace (for logging purposes)
  m_transmittedDatas (data, this, m_face);

  m_face->ReceiveData (data); 
}

// Callback that will be called when Data arrives
void
CustomApp::OnData (Ptr<const ccn::Data> contentObject)
{
  NS_LOG_DEBUG ("Receiving Data packet for " << contentObject->GetName ());

  std::cout << "DATA received for name " << contentObject->GetName () << std::endl;
}

} // namespace ns3
