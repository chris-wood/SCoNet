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
 *
 *
 * Modified to support ccnSim
 * Author: Maziar Mirzazad-Barijough <maziar@soe.ucsc.edu>
 * University of California Santa Cruz
 */
#ifndef CCNSIM_NACKS_H
#define CCNSIM_NACKS_H

#include "ns3/ccn-forwarding-strategy.h"

namespace ns3 {
namespace ccn {
namespace fw {

/**
 * @ingroup ccn-fw
 * @brief Implementation of experimental NACK messages (enables with EnableNACKs option)
 */
class Nacks :
    public ForwardingStrategy
{
private:
  typedef ForwardingStrategy super;
  
public:
  static TypeId
  GetTypeId ();
  
  // from super
  virtual void
  OnData (Ptr<Face> face,
              Ptr<Data> data);

  virtual void
  OnInterestResponse(Ptr<Face>, Ptr<InterestResponse>);

protected:
  // from super
  virtual void
  DidReceiveDuplicateInterest (Ptr<Face> inFace,
                               Ptr<Interest> interest,
                               Ptr<pit::Entry> pitEntry);

/*
  // from super
  virtual void
  DidExhaustForwardingOptions (Ptr<Face> inFace,
                               Ptr<Interest> interest,
                               Ptr<pit::Entry> pitEntry);
*/

  virtual void
  OnNack (Ptr<Face> inFace,
          Ptr<Data> nack);
/*
  virtual void
  DidReceiveValidNack (Ptr<Face> inFace,
                       uint32_t nackCode,
                       Ptr<Data> nack,
                       Ptr<pit::Entry> pitEntry);
  */
protected:  
  bool m_nacksEnabled;

  TracedCallback<Ptr<Data>,
                 Ptr<const Face> > m_outNacks; ///< @brief trace of outgoing NACKs

  TracedCallback<Ptr<Data>,
                 Ptr<const Face> > m_inNacks; ///< @brief trace of incoming NACKs

  TracedCallback<Ptr<Data>,
                 Ptr<const Face> > m_dropNacks; ///< @brief trace of dropped NACKs
};

} // namespace fw
} // namespace ccn
} // namespace ns3

#endif // CCNSIM_NACKS
