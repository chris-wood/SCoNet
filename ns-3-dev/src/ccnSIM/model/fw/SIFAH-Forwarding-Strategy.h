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


#ifndef CCNSIM_SIFAH_H
#define CCNSIM_SIFAH_H

#include "nacks.h"
#include "ns3/ccn-data.h"
#include "ns3/log.h"

namespace ns3 {
namespace ccn {
namespace fw {

/**
 * @ingroup ccn-fw
 * @brief Best route strategy
 */
class SIFAH :
    public Nacks
{
private:
  typedef Nacks super;

public:
  static TypeId
  GetTypeId ();

  /**
   * @brief Helper function to retrieve logging name for the forwarding strategy
   */
  static std::string
  GetLogName ();
  
  /**
   * @brief Default constructor
   */
  SIFAH ();
        
  // from super
  virtual void
    OnInterest (Ptr<Face> face,
                Ptr<Interest> interest);

  // from super
  virtual bool
  DoPropagateInterest (Ptr<Face> incomingFace,
                       Ptr<Interest> interest,
                       Ptr<pit::Entry> pitEntry);
  virtual Ptr<pit::Entry>
  CheckPit(Ptr<Interest> interest);

  virtual bool
  CheckFib(Ptr<Face> inFace, Ptr<Interest> interest);

  virtual bool
  CheckHFAR1(Ptr<Face> inFace,Ptr<Interest> interest, Ptr<pit::Entry> pitEntry);

  virtual bool
  CheckHFAR1(int routingCost,Ptr<Interest> interest);

  virtual bool
  CheckHFAR2(int pitHopCount, Ptr<Face> inFace,Ptr<Interest> interest);

  virtual void
  SendNack(Ptr<Interest> interest, Ptr<Face> inFace);

  virtual void
  SendNack(Name prefix, Ptr<Face> inFace);

  virtual void
  WillEraseTimedOutPendingInterest (Ptr<pit::Entry> pitEntry);
/*
  virtual void
  DidExhaustForwardingOptions (Ptr<Face> inFace, Ptr<Interest> interest,
                                                   Ptr<pit::Entry> pitEntry);
*/

protected:
  static LogComponent g_log;
};

} // namespace fw
} // namespace ccn
} // namespace ns3

#endif // CCNSIM_SIFAH_H
