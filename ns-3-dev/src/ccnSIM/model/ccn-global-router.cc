/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil -*- */
/*
 * Copyright (c) 2011 UCLA
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
 */

#include "ccn-global-router.h"

#include "ns3/ccn-l3-protocol.h"
#include "ns3/ccn-face.h"
#include "ns3/ccn-name.h"

#include "ns3/channel.h"

using namespace boost;

namespace ns3 {
namespace ccn {

uint32_t GlobalRouter::m_idCounter = 0;

NS_OBJECT_ENSURE_REGISTERED (GlobalRouter);

TypeId 
GlobalRouter::GetTypeId ()
{
  static TypeId tid = TypeId ("ns3::ccn::GlobalRouter")
    .SetGroupName ("Ccn")
    .SetParent<Object> ()
  ;
  return tid;
}

GlobalRouter::GlobalRouter ()
{
  m_id = m_idCounter;
  m_idCounter ++;
}

void
GlobalRouter::NotifyNewAggregate ()
{
  if (m_ccn == 0)
    {
      m_ccn = GetObject<L3Protocol> ();
    }
  Object::NotifyNewAggregate ();
}

uint32_t
GlobalRouter::GetId () const
{
  return m_id;
}

Ptr<L3Protocol>
GlobalRouter::GetL3Protocol () const
{
  return m_ccn;
}

void
GlobalRouter::AddLocalPrefix (Ptr< Name > prefix)
{
  m_localPrefixes.push_back (prefix);
}

void
GlobalRouter::AddIncidency (Ptr< Face > face, Ptr< GlobalRouter > gr)
{
  m_incidencies.push_back (make_tuple (this, face, gr));
}

GlobalRouter::IncidencyList &
GlobalRouter::GetIncidencies ()
{
  return m_incidencies;
}

const GlobalRouter::LocalPrefixList &
GlobalRouter::GetLocalPrefixes () const
{
  return m_localPrefixes;
}

// void
// GlobalRouter::AddIncidencyChannel (Ptr< CcnFace > face, Ptr< Channel > channel)
// {
//   m_incidenciesChannel.push_back (make_tuple (face, channel));
// }

} // namespace ccn
} // namespace ns3
