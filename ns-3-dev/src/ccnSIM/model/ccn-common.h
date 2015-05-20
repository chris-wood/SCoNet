/** -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil -*- */
/* 
 * Copyright (c) 2013, Regents of the University of California
 *                     Alexander Afanasyev
 * 
 * BSD license, See the doc/LICENSE file for more information
 * 
 * Author: Alexander Afanasyev <alexander.afanasyev@ucla.edu>
 */

#ifndef CCN_COMMON_H
#define CCN_COMMON_H

#include "ns3/nstime.h"
#include "ns3/simulator.h"

#define CCNSIM_MODE 1

#define CCN_NAMESPACE_BEGIN  namespace ns3 { namespace ccn {
#define CCN_NAMESPACE_END    } /*ccn*/ } /*ns3*/ 

/**
 * @brief NS-3 namespace
 */
namespace ns3 {

/**
 * @brief ccnSIM namespace
 */
namespace ccn {
}

}

CCN_NAMESPACE_BEGIN

typedef Time TimeInterval;

namespace time
{

inline Time
NowUnixTimestamp ()
{
  return Simulator::Now ();
}

}

CCN_NAMESPACE_END

#endif // CCN_COMMON_H
