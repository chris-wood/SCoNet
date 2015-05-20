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
 *
 * Modified to support ccnSim
 * Author: Ehsan Hemmati <ehsan@soe.ucsc.edu>
 * 2014 University of California, Santa Cruz
 */

#include "ccn-consumer.h"
#include "ns3/ptr.h"
#include "ns3/log.h"
#include "ns3/simulator.h"
#include "ns3/packet.h"
#include "ns3/callback.h"
#include "ns3/string.h"
#include "ns3/boolean.h"
#include "ns3/uinteger.h"
#include "ns3/integer.h"
#include "ns3/double.h"
#include "ns3/pointer.h"


#include "ns3/ccn-app-face.h"
#include "ns3/ccn-interest.h"
#include "ns3/ccn-data.h"
#include "ns3/ccn-interestresponse.h"
//include "ns3/ccnSIM/utils/ccn-fw-hop-count-tag.h"
#include "ns3/ccnSIM/utils/ccn-rtt-mean-deviation.h"

#include <boost/ref.hpp>

#include "ns3/names.h"

NS_LOG_COMPONENT_DEFINE ("ccn.Consumer");

namespace ns3 {
namespace ccn {

NS_OBJECT_ENSURE_REGISTERED (Consumer);

TypeId
Consumer::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::ccn::Consumer")
    .SetGroupName ("Ccn")
    .SetParent<App> ()
    .AddAttribute ("StartSeq", "Initial sequence number",
                   IntegerValue (0),
                   MakeIntegerAccessor(&Consumer::m_seq),
                   MakeIntegerChecker<int32_t>())

    .AddAttribute ("Prefix","Name of the Interest",
                   StringValue ("/"),
                   MakeNameAccessor (&Consumer::m_interestName),
                   MakeNameChecker ())
    .AddAttribute ("LifeTime", "LifeTime for interest packet",
                   StringValue ("2s"),
                   MakeTimeAccessor (&Consumer::m_interestLifeTime),
                   MakeTimeChecker ())

    .AddAttribute ("RetxTimer",
                   "Timeout defining how frequent retransmission timeouts should be checked",
                   StringValue ("50ms"),
                   MakeTimeAccessor (&Consumer::GetRetxTimer, &Consumer::SetRetxTimer),
                   MakeTimeChecker ())
    .AddAttribute ("HopLimit", "Interest HopLimit",
                   UintegerValue (253),
                   MakeUintegerAccessor(&Consumer::m_hops),
                   MakeUintegerChecker<uint8_t>())
    .AddAttribute ("ExpHops", "Expected Hop count",
                   UintegerValue (254),
                   MakeUintegerAccessor(&Consumer::m_exphops),
                   MakeUintegerChecker<uint8_t>())
    .AddAttribute ("version", "Version",
                   IntegerValue (0),
                   MakeUintegerAccessor(&Consumer::m_version),
                   MakeUintegerChecker<uint8_t>())
    /*.AddAttribute ("NameNonce", "A binary component representing a unique value to differentiate names",
    	           UintegerValue (0),
		   MakeUintegerAccessor (&Consumer::m_nameNonce),
	           MakeUintegerChecker<uint32_t> ())*/
    .AddAttribute ("KeyIdRestrSize", "KeyID Size",
		   UintegerValue (0),
	           MakeUintegerAccessor (&Consumer::m_keyIdSize),
	           MakeUintegerChecker<uint32_t> ())
    .AddAttribute ("KeyIdRestrt", "Key ID",
		   UintegerValue (0),
		   MakeUintegerAccessor (&Consumer::m_keyId),
		   MakeUintegerChecker<uint8_t>())
    .AddAttribute ("ObjectHashSize", "The ContentObjectHash path segment",
           UintegerValue (0),
		   MakeUintegerAccessor (&Consumer::m_objectHashSize),
		   MakeUintegerChecker<uint32_t> ())
    .AddAttribute ("ObjectHash", "The ContentObjectHash path segment",
		   UintegerValue (0),
		   MakeUintegerAccessor (&Consumer::m_objectHash),
		   MakeUintegerChecker<uint8_t>())
                    
    .AddTraceSource ("LastRetransmittedInterestDataDelay", "Delay between last retransmitted Interest and received Data",
                   MakeTraceSourceAccessor (&Consumer::m_lastRetransmittedInterestDataDelay))

    .AddTraceSource ("FirstInterestDataDelay", "Delay between first transmitted Interest and received Data",
                   MakeTraceSourceAccessor (&Consumer::m_firstInterestDataDelay))
    ;

  return tid;
}
Consumer::Consumer ()
  : m_rand (0, std::numeric_limits<uint32_t>::max ())
  , m_seq (0)
  , m_seqMax (0) // don't request anything
{
  NS_LOG_FUNCTION_NOARGS ();

  m_rtt = CreateObject<RttMeanDeviation> ();
}

void
Consumer::SetRetxTimer (Time retxTimer)
{
  m_retxTimer = retxTimer;
  if (m_retxEvent.IsRunning ())
    {
      // m_retxEvent.Cancel (); // cancel any scheduled cleanup events
      Simulator::Remove (m_retxEvent); // slower, but better for memory
    }

  // schedule even with new timeout
  m_retxEvent = Simulator::Schedule (m_retxTimer,
                                     &Consumer::CheckRetxTimeout, this);
}

Time
Consumer::GetRetxTimer () const
{
  return m_retxTimer;
}

void
Consumer::CheckRetxTimeout ()
{
  Time now = Simulator::Now ();

  Time rto = m_rtt->RetransmitTimeout ();
  // NS_LOG_DEBUG ("Current RTO: " << rto.ToDouble (Time::S) << "s");

  while (!m_seqTimeouts.empty ())
    {
      SeqTimeoutsContainer::index<i_timestamp>::type::iterator entry =
        m_seqTimeouts.get<i_timestamp> ().begin ();
      if (entry->time + rto <= now) // timeout expired?
        {
          uint32_t seqNo = entry->seq;
          m_seqTimeouts.get<i_timestamp> ().erase (entry);
          OnTimeout (seqNo);
        }
      else
        break; // nothing else to do. All later packets need not be retransmitted
    }

  m_retxEvent = Simulator::Schedule (m_retxTimer,
                                     &Consumer::CheckRetxTimeout, this);
}

// Application Methods
void
Consumer::StartApplication () // Called at time specified by Start
{
  NS_LOG_FUNCTION_NOARGS ();

  // do base stuff
  App::StartApplication ();

  ScheduleNextPacket ();
}

void
Consumer::StopApplication () // Called at time specified by Stop
{
  NS_LOG_FUNCTION_NOARGS ();

  // cancel periodic packet generation
  Simulator::Cancel (m_sendEvent);

  // cleanup base stuff
  App::StopApplication ();
}

void
Consumer::SendPacket ()
{
  if (!m_active) return;

  NS_LOG_FUNCTION_NOARGS ();

  uint32_t seq=std::numeric_limits<uint32_t>::max (); //invalid

  while (m_retxSeqs.size ())
    {
      seq = *m_retxSeqs.begin ();
      m_retxSeqs.erase (m_retxSeqs.begin ());
      break;
    }

  if (seq == std::numeric_limits<uint32_t>::max ())
    {
      if (m_seqMax != std::numeric_limits<uint32_t>::max ())
        {
          if (m_seq >= m_seqMax)
            {
              return; // we are totally done
            }
        }

      seq = m_seq++;
    }

  //
  Ptr<Name> nameWithSequence = Create<Name> (m_interestName);
  nameWithSequence->appendSeqNum (seq);
  //

  Ptr<Interest> interest = Create<Interest> ();
  //interest->SetNonce               (m_rand.GetValue ());
  interest->SetName                (nameWithSequence);
  interest->SetInterestLifetime    (m_interestLifeTime);
  interest->SetInterestHopCount    (m_hops); //Plus Two AppFaces
  //interest->SetInterestExpectedHopCount(m_exphops + 1); // Plus One AppFace
  interest->SetInterestPktVersion  (m_version);

  // Set a random KeyID  
  if (m_keyIdSize > 0){
	  Ptr<Packet> m_nameKeyID = Create<Packet>(m_keyIdSize);
	  interest->SetKeyIdRestr(m_nameKeyID);
  }
  //Creat a random hash
  if (m_objectHashSize > 0){
	  Ptr<Packet> m_objectHash = Create<Packet>(m_objectHashSize);
	  interest->SetObjHashRestr(m_objectHash);
  }
  // NS_LOG_INFO ("Requesting Interest: \n" << *interest);
  NS_LOG_INFO ("> Interest for " << seq);

  WillSendOutInterest (seq);  

  m_transmittedInterests (interest, this, m_face);
  m_face->ReceiveInterest (interest);

  ScheduleNextPacket ();
}

///////////////////////////////////////////////////
//          Process incoming packets             //
///////////////////////////////////////////////////


void
Consumer::OnData (Ptr<const Data> data)
{
  if (!m_active) return;

  App::OnData (data); // tracing inside

  NS_LOG_FUNCTION (this << data);

  // NS_LOG_INFO ("Received content object: " << boost::cref(*data));


  uint32_t seq = data->GetName ().get (-1).toSeqNum ();
  NS_LOG_INFO ("< DATA for " << seq);

  SeqTimeoutsContainer::iterator entry = m_seqLastDelay.find (seq);
  if (entry != m_seqLastDelay.end ())
    {
      m_lastRetransmittedInterestDataDelay (this, seq, Simulator::Now () - entry->time);
    }

  entry = m_seqFullDelay.find (seq);
  if (entry != m_seqFullDelay.end ())
    {
      m_firstInterestDataDelay (this, seq, Simulator::Now () - entry->time, m_seqRetxCounts[seq]);
    }

  m_seqRetxCounts.erase (seq);
  m_seqFullDelay.erase (seq);
  m_seqLastDelay.erase (seq);

  m_seqTimeouts.erase (seq);
  m_retxSeqs.erase (seq);

  m_rtt->AckSeq (SequenceNumber32 (seq));
}

void
Consumer::OnInterestResponse (Ptr<const InterestResponse> interestResponse)
{
  if (!m_active) return;

  App::OnInterestResponse (interestResponse); // tracing inside

  // NS_LOG_INFO ("Received InterestResponse: " << boost::cref(*interestResponse));
  uint32_t seq =interestResponse->GetInterest()->GetName().get(-1).toSeqNum();
  NS_LOG_INFO ("< InterestResponse for " << seq);
  // std::cout << Simulator::Now ().ToDouble (Time::S) << "s -> " << "interestResponse for " << seq << "\n";

  // put in the queue of interests to be retransmitted
  // NS_LOG_INFO ("Before: " << m_retxSeqs.size ());
  m_retxSeqs.insert (seq);
  // NS_LOG_INFO ("After: " << m_retxSeqs.size ());

  m_seqTimeouts.erase (seq);

  m_rtt->IncreaseMultiplier ();             // Double the next RTO ??
  ScheduleNextPacket ();
}

void
Consumer::OnTimeout (uint32_t sequenceNumber)
{
  NS_LOG_FUNCTION (sequenceNumber);
  // std::cout << Simulator::Now () << ", TO: " << sequenceNumber << ", current RTO: " << m_rtt->RetransmitTimeout ().ToDouble (Time::S) << "s\n";

  m_rtt->IncreaseMultiplier ();             // Double the next RTO
  m_rtt->SentSeq (SequenceNumber32 (sequenceNumber), 1); // make sure to disable RTT calculation for this sample
  m_retxSeqs.insert (sequenceNumber);
  ScheduleNextPacket ();
}

void
Consumer::WillSendOutInterest (uint32_t sequenceNumber)
{
  NS_LOG_DEBUG ("Trying to add " << sequenceNumber << " with " << Simulator::Now () << ". already " << m_seqTimeouts.size () << " items");

  m_seqTimeouts.insert (SeqTimeout (sequenceNumber, Simulator::Now ()));
  m_seqFullDelay.insert (SeqTimeout (sequenceNumber, Simulator::Now ()));

  m_seqLastDelay.erase (sequenceNumber);
  m_seqLastDelay.insert (SeqTimeout (sequenceNumber, Simulator::Now ()));

  m_seqRetxCounts[sequenceNumber] ++;

  m_rtt->SentSeq (SequenceNumber32 (sequenceNumber), 1);
}

bool
Consumer::NameKeyIdCheck(Ptr<const Packet> dataKeyId)
{

		return true;
	
}

bool
Consumer::NameHashCheck(Ptr<const Packet> dataHash)
{
     
		return true;
	
}

bool
Consumer::NameNonceCheck(const uint32_t dataNonce)
{

		return true;
	
}

} // namespace ccn
} // namespace ns3
