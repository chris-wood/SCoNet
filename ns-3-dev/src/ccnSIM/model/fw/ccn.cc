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

#include "ccn.h"

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

NS_OBJECT_ENSURE_REGISTERED (CCN);

LogComponent CCN::g_log = LogComponent (CCN::GetLogName ().c_str ());

std::string
CCN::GetLogName ()
{
  return super::GetLogName ()+".CCN";
}


TypeId
CCN::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::ccn::fw::ccn")
    .SetGroupName ("Ccn")
    .SetParent <super> ()
    .AddConstructor <CCN> ()
    ;
  return tid;
}

CCN::CCN ()
{
}

bool
CCN::DoPropagateInterest (Ptr<Face> inFace,
                                Ptr<Interest> interest,
                                Ptr<pit::Entry> pitEntry)
{
	//std::cout<<"ccn\n";
  NS_LOG_FUNCTION (this << interest->GetName ());

  int propagatedCount = 0;

  BOOST_FOREACH (const fib::FaceMetric &metricFace, pitEntry->GetFibEntry ()->m_faces.get<fib::i_metric> ())
    {
      NS_LOG_DEBUG ("Trying " << boost::cref(metricFace));

      if (metricFace.GetStatus () == fib::FaceMetric::CCN_FIB_RED) // all non-read faces are in front
        break;

      if (!TrySendOutInterest (inFace, metricFace.GetFace (), interest, pitEntry))
        {
          continue;
        }
      //std::cout<<" ("<<metricFace.GetRank()<<", "<<metricFace.GetRoutingCost()<<") ";
      propagatedCount++;
      break; // do only once
    }
  std::cout<<"\n";
  NS_LOG_INFO ("Propagated to " << propagatedCount << " faces");
  return propagatedCount > 0;
}

} // namespace fw
} // namespace ccn
} // namespace ns3
