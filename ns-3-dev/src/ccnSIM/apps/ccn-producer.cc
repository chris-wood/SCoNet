/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
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
 * Author: Ilya Moiseenko <iliamo@cs.ucla.edu>
 *         Alexander Afanasyev <alexander.afanasyev@ucla.edu>
 *
 * Modified to support ccnSim
 * Author: Ehsan Hemmati <ehsan@soe.ucsc.edu>
 * 2014 University of California, Santa Cruz
 */

#include "ccn-producer.h"
#include "ns3/log.h"
#include "ns3/ccn-interest.h"
#include "ns3/ccn-data.h"
#include "ns3/string.h"
#include "ns3/uinteger.h"
#include "ns3/packet.h"
#include "ns3/simulator.h"

#include "ns3/ccn-app-face.h"
#include "ns3/ccn-fib.h"

#include "ns3/ccnSIM/utils/ccn-fw-hop-count-tag.h"

#include <boost/ref.hpp>
#include <boost/lambda/lambda.hpp>
#include <boost/lambda/bind.hpp>
namespace ll = boost::lambda;

NS_LOG_COMPONENT_DEFINE ("ccn.Producer");

namespace ns3 {
namespace ccn {

NS_OBJECT_ENSURE_REGISTERED (Producer);

TypeId
Producer::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::ccn::Producer")
    .SetGroupName ("Ccn")
    .SetParent<App> ()
    .AddConstructor<Producer> ()
    .AddAttribute ("Prefix","Prefix, for which producer has the data",
                   StringValue ("/"),
                   MakeNameAccessor (&Producer::m_prefix),
                   MakeNameChecker ())
    .AddAttribute ("Postfix", "Postfix that is added to the output data (e.g., for adding producer-uniqueness)",
                   StringValue ("/"),
                   MakeNameAccessor (&Producer::m_postfix),
                   MakeNameChecker ())
    .AddAttribute ("PayloadSize", "Virtual payload size for Content packets",
                   UintegerValue (1024),
                   MakeUintegerAccessor (&Producer::m_virtualPayloadSize),
                   MakeUintegerChecker<uint32_t> ())
    /*.AddAttribute ("Freshness", "Freshness of data packets, if 0, then unlimited freshness",
                   TimeValue (Seconds (0)),
                   MakeTimeAccessor (&Producer::m_freshness),
                   MakeTimeChecker ()) */
    .AddAttribute ("Signature", "Fake signature, 0 valid signature (default), other values application-specific",
                   UintegerValue (0),
                   MakeUintegerAccessor (&Producer::m_signature),
                   MakeUintegerChecker<uint32_t> ())
    /*.AddAttribute ("KeyLocator", "Name to be used for key locator.  If root, then key locator is not used",
                   NameValue (),
                   MakeNameAccessor (&Producer::m_keyLocator),
                   MakeNameChecker ())
	 .AddAttribute ("MetaData", "Do we have meta data?",
		 	   	   BooleanValue (false),
				   MakeBooleanAccessor (&Producer::m_metaData),
				   MakeBooleanChecker ())*/
	.AddAttribute ("Type", "Default type is Data",
                   UintegerValue (0),
                   MakeUintegerAccessor (&Producer::m_type),
                   MakeUintegerChecker<uint8_t> ())
	.AddAttribute ("KeyIdRestr", "KeyIdRestriction",
			       UintegerValue (0),
				   MakeUintegerAccessor (&Producer::m_keyId),
				   MakeUintegerChecker<uint8_t>())
	.AddAttribute ("ObjectHashRestr", "ContentObjectHashRestriction",
				   UintegerValue (0),
				   MakeUintegerAccessor (&Producer::m_objectHash),
				   MakeUintegerChecker<uint8_t>())
	.AddAttribute ("KeyIdRestrSize", "KeyID Size",
				   UintegerValue (0),
				   MakeUintegerAccessor (&Producer::m_keyIdSize),
				   MakeUintegerChecker<uint32_t> ())
	.AddAttribute ("ObjectHashSize", "The ContentObjectHash path segment",
				   UintegerValue (0),
				   MakeUintegerAccessor (&Producer::m_objectHashSize),
				   MakeUintegerChecker<uint32_t> ())
	;
  return tid;
}

Producer::Producer ()
{
  // NS_LOG_FUNCTION_NOARGS ();
}

// inherited from Application base class.
void
Producer::StartApplication ()
{
  NS_LOG_FUNCTION_NOARGS ();
  NS_ASSERT (GetNode ()->GetObject<Fib> () != 0);

  App::StartApplication ();

  NS_LOG_DEBUG ("NodeID: " << GetNode ()->GetId ());

  Ptr<Fib> fib = GetNode ()->GetObject<Fib> ();

  Ptr<fib::Entry> fibEntry = fib->Add (m_prefix, m_face, 0, 0);

  fibEntry->UpdateStatus (m_face, fib::FaceMetric::CCN_FIB_GREEN);

  // // make face green, so it will be used primarily
  // StaticCast<fib::FibImpl> (fib)->modify (fibEntry,
  //                                        ll::bind (&fib::Entry::UpdateStatus,
  //                                                  ll::_1, m_face, fib::FaceMetric::CCN_FIB_GREEN));
}

void
Producer::StopApplication ()
{
  NS_LOG_FUNCTION_NOARGS ();
  NS_ASSERT (GetNode ()->GetObject<Fib> () != 0);

  App::StopApplication ();
}


void
Producer::OnInterest (Ptr<const Interest> interest)
{
  App::OnInterest (interest); // tracing inside
  NS_LOG_FUNCTION (this << interest);
  //debug
  //std::cout <<  "node:" << GetNode ()->GetId () << "sees interest!" << std::endl;
  if (!m_active) return;

  if (!KeyIdCheck(interest->GetKeyIdRestr()) ) {
			  return;
  }

  if (!ObjectHashCheck(interest->GetObjHashRestr()) ) {
			  return;
  }
/*
  if (!NameNonceCheck(interest->GetNameNonce())){
       return;
  } 
*/
  Ptr<Data> data = Create<Data> (); //Create<Packet> (m_virtualPayloadSize));
  Ptr<Name> dataName = Create<Name> (interest->GetName ());
  dataName->append (m_postfix);
  data->SetName (dataName);
  //data->SetFreshness (m_freshness);
  //data->SetTimestamp (Simulator::Now());

  //data->SetSignature (m_signature);
  //uint8_t *data = new uint8_t[m_virtualPayloadSize];
  //rawData = new uint8_t[size];
  //m_payload = Create<Packet>(m_virtualPayloadSize);
  //data->SetPayload (m_payload);
  
  data->SetDataType (m_type);
  //if (m_nack){
	  //data->SetDataNACK (m_nack);
   // assert(0); //should be interest response sent back
  //}
  //data->SetWire (m_packet);
//  data->SetNameKeyID(m_keyID);
//  data->SetNameNonce(m_nameNonce);
//  data->SetObjectHash(m_objectHash);



  //if (m_keyLocator.size () > 0)
    //{
      //data->SetKeyLocator (Create<Name> (m_keyLocator));
    //}

  NS_LOG_INFO ("node("<< GetNode()->GetId() <<") respodning with Data: " << data->GetName ());

  // Echo back FwHopCountTag if exists
  FwHopCountTag hopCountTag;
  //if (interest->GetPayload ()->PeekPacketTag (hopCountTag))
  //  {
  //    data->GetPayload ()->AddPacketTag (hopCountTag);
  //  }

  m_face->ReceiveData (data);
  m_transmittedDatas (data, this, m_face);
}

bool
Producer::KeyIdCheck(Ptr<const Packet> interestKeyId)
{

		return true;
	
}

bool
Producer::ObjectHashCheck(Ptr<const Packet> interestHash)
{
     
		return true;
	
}

bool
Producer::NameNonceCheck(const uint32_t interestNonce)
{

		return true;
	
}

} // namespace ccn
} // namespace ns3
