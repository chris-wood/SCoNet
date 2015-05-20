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
 * Author:  Alexander Afanasyev <alexander.afanasyev@ucla.edu>

 * Modified to support ccnSim
 * Author: Maziar Mirzazad-Barijough <maziar@soe.ucsc.edu>
 * University of California Santa Cruz
 *
 */

#include "nacks.h"

#include "ns3/ccn-pit.h"
#include "ns3/ccn-pit-entry.h"
#include "ns3/ccn-interest.h"
#include "ns3/ccn-interestresponse.h"
#include "ns3/ccn-data.h"
#include "ns3/ccn-pit.h"
#include "ns3/ccn-fib.h"
#include "ns3/ccn-content-store.h"
#include "ns3/ccnSIM/utils/ccn-fw-hop-count-tag.h"

#include "ns3/assert.h"
#include "ns3/ptr.h"
#include "ns3/log.h"
#include "ns3/simulator.h"
#include "ns3/boolean.h"
#include "ns3/string.h"

#include <boost/ref.hpp>
#include <boost/foreach.hpp>
#include <boost/lambda/lambda.hpp>
#include <boost/lambda/bind.hpp>
#include <boost/tuple/tuple.hpp>
namespace ll = boost::lambda;

NS_LOG_COMPONENT_DEFINE ("ccn.fw.Nacks");

namespace ns3 {
namespace ccn {
namespace fw {

NS_OBJECT_ENSURE_REGISTERED (Nacks);

TypeId
Nacks::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::ccn::fw::Nacks")
    .SetGroupName ("Ccn")
    .SetParent<ForwardingStrategy> ()

    ////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////

    .AddTraceSource ("OutNacks",  "OutNacks",  MakeTraceSourceAccessor (&Nacks::m_outNacks))
    .AddTraceSource ("InNacks",   "InNacks",   MakeTraceSourceAccessor (&Nacks::m_inNacks))
    .AddTraceSource ("DropNacks", "DropNacks", MakeTraceSourceAccessor (&Nacks::m_dropNacks))

    .AddAttribute ("EnableNACKs", "Enabling support of NACKs",
                   BooleanValue (false),
                   MakeBooleanAccessor (&Nacks::m_nacksEnabled),
                   MakeBooleanChecker ())
    ;
  return tid;
}

void
Nacks::OnInterestResponse(Ptr<Face> inFace, Ptr<InterestResponse> response)
{
	 Ptr<pit::Entry> pitEntry = m_pit->Lookup (*response);
	  if (pitEntry == 0)
	    {
	      // somebody is doing something bad
	      //m_dropNacks (response, inFace);
	      return;
	    }
	  else{
		  SatisfyPendingInterest(inFace,response,pitEntry);
	  }
}

void
Nacks::OnData (Ptr<Face> inFace,
                   Ptr<Data> data)
{
//	data->Print(std::cout);
/*  if (data->GetDataNack())
    OnNack (inFace, data);
  else*/
    super::OnData (inFace, data);
}

void
Nacks::OnNack (Ptr<Face> inFace,
               Ptr<Data> nack)
{
  // NS_LOG_FUNCTION (inFace << nack->GetName ());
  m_inNacks (nack, inFace);
 // std::cout<<"recieved Nack :"<<inFace->GetNode()->GetId()<<"\n";
  Ptr<pit::Entry> pitEntry = m_pit->Lookup (*nack);
  if (pitEntry == 0)
    {
      // somebody is doing something bad
      m_dropNacks (nack, inFace);
      return;
    }
  else{
	  SatisfyPendingInterest(inFace,nack,pitEntry);
  }

  //DidReceiveValidNack (inFace, nack->GetDataNack(), nack, pitEntry);
}

void
Nacks::DidReceiveDuplicateInterest (Ptr<Face> inFace,
                                    Ptr<Interest> interest,
                                    Ptr<pit::Entry> pitEntry)
{
  super::DidReceiveDuplicateInterest (inFace, interest, pitEntry);

  /*if (m_nacksEnabled)
    {
      NS_LOG_DEBUG ("Sending NACK_LOOP");
      Ptr<Data> nack = Create<Data> (*interest);
      nack->SetNack (Interest::NACK_LOOP);

      inFace->SendInterest (nack);
      m_outNacks (nack, inFace);
    }*/
}

/*void
Nacks::DidExhaustForwardingOptions (Ptr<Face> inFace,
                                    Ptr<Interest> interest,
                                    Ptr<pit::Entry> pitEntry)
{
  if (m_nacksEnabled)
    {
      Ptr<Interest> nack = Create<Interest> (*interest);
      nack->SetNack (Interest::NACK_GIVEUP_PIT);

      BOOST_FOREACH (const pit::IncomingFace &incoming, pitEntry->GetIncoming ())
        {
          NS_LOG_DEBUG ("Send NACK for " << boost::cref (nack->GetName ()) << " to " << boost::cref (*incoming.m_face));
          incoming.m_face->SendInterest (nack);
          m_outNacks (nack, incoming.m_face);
        }

      pitEntry->ClearOutgoing (); // to force erasure of the record
    }

  super::DidExhaustForwardingOptions (inFace, interest, pitEntry);
}*/

/*void
Nacks::DidReceiveValidNack (Ptr<Face> inFace,
                            uint32_t nackCode,
                            Ptr<Data> nack,
                            Ptr<pit::Entry> pitEntry)
{
  NS_LOG_DEBUG ("nackCode: " << nackCode << " for [" << nack->GetName () << "]");

  // If NACK is NACK_GIVEUP_PIT, then neighbor gave up trying to and removed it's PIT entry.
  // So, if we had an incoming entry to this neighbor, then we can remove it now
  if (nackCode == Interest::NACK_GIVEUP_PIT)
    {
      pitEntry->RemoveIncoming (inFace);
    }

  if (nackCode == Interest::NACK_LOOP ||
      nackCode == Interest::NACK_CONGESTION ||
      nackCode == Interest::NACK_GIVEUP_PIT)
    {
      pitEntry->SetWaitingInVain (inFace);

      if (!pitEntry->AreAllOutgoingInVain ()) // not all ougtoing are in vain
        {
          NS_LOG_DEBUG ("Not all outgoing are in vain");
          // suppress
          // Don't do anything, we are still expecting data from some other face
          m_dropNacks (nack, inFace);
          return;
        }

      Ptr<Interest> interest = Create<Interest> (*nack);
      interest->SetNack (Interest::NORMAL_INTEREST);

      bool propagated = DoPropagateInterest (inFace, interest, pitEntry);
      if (!propagated)
        {
          DidExhaustForwardingOptions (inFace, interest, pitEntry);
        }
    }
}*/

} // namespace fw
} // namespace ccn
} // namespace ns3
