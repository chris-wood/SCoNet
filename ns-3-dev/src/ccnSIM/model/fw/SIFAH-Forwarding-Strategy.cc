/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2014 University of California, Santa Cruz
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
 * Author: Maziar Mirzazad-Barijough <maziar@soe.ucsc.edu>
 *
 */

#include "SIFAH-Forwarding-Strategy.h"

#include "ns3/ccn-interest.h"
#include "ns3/ccn-pit.h"
#include "ns3/ccn-pit-entry.h"

#include "ns3/assert.h"
#include "ns3/log.h"

#include <boost/foreach.hpp>
#include <boost/lambda/lambda.hpp>
#include <boost/lambda/bind.hpp>
namespace ll = boost::lambda;

namespace ns3 {
namespace ccn {
namespace fw {

NS_OBJECT_ENSURE_REGISTERED (SIFAH);

LogComponent SIFAH::g_log = LogComponent (SIFAH::GetLogName ().c_str ());

std::string
SIFAH::GetLogName ()
{
  return super::GetLogName ()+".SIFAH";
}


TypeId
SIFAH::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::ccn::fw::SIFAH")
    .SetGroupName ("Ccn")
    .SetParent <super> ()
    .AddConstructor <SIFAH> ()
    ;
  return tid;
}

SIFAH::SIFAH ()
{
}

void
SIFAH::OnInterest (Ptr<Face> inFace, Ptr<Interest> interest){

	NS_LOG_FUNCTION (inFace << interest->GetName ());
	  m_inInterests (interest, inFace);



	  if(CheckCS(inFace,interest)) return;

	  Ptr<pit::Entry> pitEntry = CheckPit (interest);

	  bool similarInterest = true;
	  if (pitEntry == 0)
	    {

		  if(!CheckFib(inFace, interest)) return;///sending nack inside CheckFib
	      similarInterest = false;
	      pitEntry = m_pit->Create (interest);
	      if (pitEntry != 0)
	        {
	          DidCreatePitEntry (inFace, interest, pitEntry);
	        }
	      else
	        {
	    	  std::cout<<"FailedToCreatePitEntry\n";
	          FailedToCreatePitEntry (inFace, interest);
	          return;
	        }


	    }

	  if(!similarInterest){
		  if(!CheckHFAR1(inFace, interest, pitEntry)) return;///Sending Nack inside CHECK for possible loops
		  // Check for Interest Hop Limit
		  	  uint8_t hopLimit= interest->GetInterestHopCount();
		  	  if(hopLimit==0) return;

		  	  //Decrease Interest Hop Limit
		  	  interest->SetInterestHopCount(hopLimit-1);

		  	  PropagateInterest (inFace, interest, pitEntry);
	  }
	  else{
		  if(!CheckHFAR2(pitEntry->GetHopCount(), inFace, interest)) return;///send nack inside check
	      pitEntry->AddIncoming (inFace/*, interest->GetInterestLifetime ()*/);
	      // update PIT entry lifetime
	      pitEntry->UpdateLifetime (interest->GetInterestLifetime ());

	      // Suppress this interest if we're still expecting data from some other face
	      NS_LOG_DEBUG ("Suppress interests");
	      m_dropInterests (interest, inFace);
	      return;
	  }




}

bool
SIFAH::DoPropagateInterest (Ptr<Face> inFace,
                                Ptr<Interest> interest,
                                Ptr<pit::Entry> pitEntry)
{
  NS_LOG_FUNCTION (this << interest->GetName ());

  // No real need to call parent's (green-yellow-red's strategy) method, since it is incorporated
  // in the logic of the BestRoute strategy
  //
  // // Try to work out with just green faces
  // bool greenOk = super::DoPropagateInterest (inFace, interest, origPacket, pitEntry);
  // if (greenOk)
  //   return true;

  int propagatedCount = 0;

  BOOST_FOREACH (const fib::FaceMetric &metricFace, pitEntry->GetFibEntry ()->m_faces.get<fib::i_rank> ())
    {
	  //int routingMetric=metricFace.GetRoutingCost();
	  int routingMetric=metricFace.GetRank();
	  if(!CheckHFAR1(routingMetric,interest)) continue;
      NS_LOG_DEBUG ("Trying " << boost::cref(metricFace));

      if (metricFace.GetStatus () == fib::FaceMetric::CCN_FIB_RED) // all non-read faces are in front
        continue;


      interest->SetInterestExpectedHopCount(routingMetric);
      pitEntry->SetHopCount(routingMetric);
      if (!TrySendOutInterest (inFace, metricFace.GetFace (), interest, pitEntry))
        {
          continue;
        }

      propagatedCount++;
      break; // do only once
    }

  NS_LOG_INFO ("Propagated to " << propagatedCount << " faces");
  return propagatedCount > 0;
}

Ptr<pit::Entry>
SIFAH::CheckPit(Ptr<Interest> interest){
	return m_pit->Lookup (*interest);
}

bool
SIFAH::CheckFib(Ptr<Face> inFace, Ptr<Interest> interest){
	Ptr<fib::Entry> entry=m_fib->LongestPrefixMatch(*interest);
	if(entry==0) {
		 /*Send Nack*/
		//std::cout<<"sending nack!!! CheckFib"<<"\n";
		SendNack(interest,inFace);
		m_dropInterests (interest, inFace);
		return false;
	}
	return true;
}

bool
SIFAH::CheckHFAR1(Ptr<Face> inFace,Ptr<Interest> interest, Ptr<pit::Entry> pitEntry){
	Ptr<Face> Face;
	bool result=false;
	BOOST_FOREACH (const fib::FaceMetric &metricFace, pitEntry->GetFibEntry ()->m_faces.get<fib::i_rank> ())
	    {
		  if(!CheckHFAR1(metricFace.GetRoutingCost(),interest)) continue;
		  //if (metricFace.GetStatus () == fib::FaceMetric::CCN_FIB_RED) // all non-read faces are in front
		  //        break;
		  result = true;
	      Face= metricFace.GetFace();
	      break;
	    }

	if(!result) {

		 /*Send Nack*/
		//std::cout<<"sending nack!!! HFAR1"<<"\n";
		SendNack(interest,inFace);
		m_dropInterests (interest, inFace);
	}

	return (Face!=0);
}

bool
SIFAH::CheckHFAR1(int routingCost,Ptr<Interest> interest){

	int interestHopCount=interest->GetInterestExpectedHopCount();
	if(interestHopCount>routingCost)
		return true;
	return false;
}

bool
SIFAH::CheckHFAR2(int pitHopCount, Ptr<Face> inFace, Ptr<Interest> interest){
	bool result;
	int interestHopCount=interest->GetInterestExpectedHopCount();
		if(interestHopCount>pitHopCount)
			result= true;
		else
			result=false;
	//bool result=true;

	if(!result) {
		 /*Send Nack*/
		//std::cout<<"sending nack!!!HFAR2"<<"\n";
		SendNack(interest,inFace);
		m_dropInterests (interest, inFace);
	}

	return result;
}

void
SIFAH::SendNack(Ptr<Interest> interest, Ptr<Face> inFace){

				Ptr<InterestResponse> response=Create<InterestResponse>();
				response->SetInterest(interest);
				response->SetInterestResponsePktVersion(1);
				inFace->SendInterestResponse(response);
}

void
SIFAH::SendNack(Name prefix, Ptr<Face> inFace){

			  std::cout<<"sending nack: "<<inFace->GetNode()->GetId()<<"\n";
				Ptr<InterestResponse> response=Create<InterestResponse>();
				response->SetInterestResponsePktVersion(1);
        Ptr< Interest > interest = Create<Interest> ();
			  Ptr<Name> interestName = Create<Name> (prefix);
        interest->SetName(interestName);

				inFace->SendInterestResponse(response);
        
} 

void
SIFAH::WillEraseTimedOutPendingInterest (Ptr<pit::Entry> pitEntry)
{
  NS_LOG_DEBUG ("WillEraseTimedOutPendingInterest for " << pitEntry->GetPrefix ());

  for (pit::Entry::out_container::iterator face = pitEntry->GetOutgoing ().begin ();
       face != pitEntry->GetOutgoing ().end ();
       face ++)
    {
      // NS_LOG_DEBUG ("Face: " << face->m_face);
      //pitEntry->GetFibEntry ()->UpdateStatus (face->m_face, fib::FaceMetric::CCN_FIB_YELLOW);
      SendNack(pitEntry->GetPrefix(),face->m_face);
    }

  super::WillEraseTimedOutPendingInterest (pitEntry);
}

/*void
SIFAH::DidExhaustForwardingOptions (Ptr<Face> inFace,
                                                 Ptr<Interest> interest,
                                                 Ptr<pit::Entry> pitEntry)
{
  NS_LOG_FUNCTION (this << boost::cref (*inFace));
  if (pitEntry->AreAllOutgoingInVain ())
    {
	  send Nacks

      m_dropInterests (interest, inFace);

      // All incoming interests cannot be satisfied. Remove them
      pitEntry->ClearIncoming ();

      // Remove also outgoing
      pitEntry->ClearOutgoing ();

      // Set pruning timout on PIT entry (instead of deleting the record)
      m_pit->MarkErased (pitEntry);
    }
}*/

} // namespace fw
} // namespace ccn
} // namespace ns3
