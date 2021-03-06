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
 *          Ilya Moiseenko <iliamo@cs.ucla.edu>
 */

#include "green-yellow-red.h"

#include "ns3/ccn-pit.h"
#include "ns3/ccn-pit-entry.h"
#include "ns3/ccn-interest.h"
#include "ns3/ccn-interestresponse.h"
#include "ns3/ccn-data.h"
#include "ns3/ccn-pit.h"
#include "ns3/ccn-fib.h"
#include "ns3/ccn-content-store.h"

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

NS_LOG_COMPONENT_DEFINE ("ccn.fw.GreenYellowRed");

namespace ns3 {
namespace ccn {
namespace fw {

NS_OBJECT_ENSURE_REGISTERED (GreenYellowRed);

TypeId
GreenYellowRed::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::ccn::fw::GreenYellowRed")
    .SetGroupName ("Ccn")
    .SetParent<Nacks> ()

    ;
  return tid;
}

bool
GreenYellowRed::DoPropagateInterest (Ptr<Face> inFace,
                                     Ptr<Interest> interest,
                                     Ptr<pit::Entry> pitEntry)
{
  NS_LOG_FUNCTION (this);
  NS_ASSERT_MSG (m_pit != 0, "PIT should be aggregated with forwarding strategy");

  int propagatedCount = 0;

  BOOST_FOREACH (const fib::FaceMetric &metricFace, pitEntry->GetFibEntry ()->m_faces.get<fib::i_metric> ())
    {
      if (metricFace.GetStatus () == fib::FaceMetric::CCN_FIB_RED ||
          metricFace.GetStatus () == fib::FaceMetric::CCN_FIB_YELLOW)
        break; //propagate only to green faces

      if (!TrySendOutInterest (inFace, metricFace.GetFace (), interest, pitEntry))
        {
          continue;
        }

      propagatedCount++;
      break; // propagate only one interest
    }

  return propagatedCount > 0;
}

void
GreenYellowRed::WillSatisfyPendingInterest (Ptr<Face> inFace,
                                            Ptr<pit::Entry> pitEntry)
{
  if (inFace != 0)
    {
      // Update metric status for the incoming interface in the corresponding FIB entry
      pitEntry->GetFibEntry ()->UpdateStatus (inFace, fib::FaceMetric::CCN_FIB_GREEN);
    }

  super::WillSatisfyPendingInterest (inFace, pitEntry);
}

void
GreenYellowRed::WillEraseTimedOutPendingInterest (Ptr<pit::Entry> pitEntry)
{
  NS_LOG_DEBUG ("WillEraseTimedOutPendingInterest for " << pitEntry->GetPrefix ());

  for (pit::Entry::out_container::iterator face = pitEntry->GetOutgoing ().begin ();
       face != pitEntry->GetOutgoing ().end ();
       face ++)
    {
      // NS_LOG_DEBUG ("Face: " << face->m_face);
      pitEntry->GetFibEntry ()->UpdateStatus (face->m_face, fib::FaceMetric::CCN_FIB_YELLOW);
    }

  super::WillEraseTimedOutPendingInterest (pitEntry);
}

void
GreenYellowRed::DidReceiveValidNack (Ptr<Face> inFace,
                                     uint32_t nackCode,
                                     Ptr<Interest> nack,
                                     Ptr<pit::Entry> pitEntry)
{
 // super::DidReceiveValidNack (inFace, nackCode, nack, pitEntry);

  //JLM FIXME TODO, are these the only correct responses?
  if (inFace != 0 &&
      (nackCode != InterestResponse::No_Route &&
       nackCode != InterestResponse::Path_Error ))
    {
      pitEntry->GetFibEntry ()->UpdateStatus (inFace, fib::FaceMetric::CCN_FIB_YELLOW);
    }
}


} // namespace fw
} // namespace ccn
} // namespace ns3
