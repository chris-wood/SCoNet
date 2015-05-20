/* -*- Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil -*- */
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
 * Author: Alexander Afanasyev <alexander.afanasyev@ucla.edu>
 */

#include "ccn-header-helper.h"

#include "ns3/log.h"
#include "ns3/packet.h"
#include "ns3/header.h"
#include "ns3/object.h"

#include "ns3/ccn-interest.h"
#include "ns3/ccn-data.h"
#include <iomanip>

NS_LOG_COMPONENT_DEFINE ("ccn.HeaderHelper");

const uint8_t INTEREST_CCNX_BYTES[]       = {0x80, 0x00};
const uint8_t CONTENT_OBJECT_CCNX_BYTES[] = {0x80, 0x01};

const uint8_t INTEREST_CCNB_BYTES[]       = {0x01, 0xD2};
const uint8_t CONTENT_OBJECT_CCNB_BYTES[] = {0x04, 0x82};

const uint8_t INTEREST_CCNSIM_BYTES[]       = {0x88, 0x00};
const uint8_t CONTENT_OBJECT_CCNSIM_BYTES[] = {0x88, 0x01};

namespace ns3 {
namespace ccn {

HeaderHelper::Type
HeaderHelper::GetCcnHeaderType (Ptr<const Packet> packet)
{
  uint8_t type[2];
  uint32_t read=packet->CopyData (type,2);

  if (read!=2) throw UnknownHeaderException();

  NS_LOG_DEBUG (*packet);
  if (type[0] == INTEREST_CCNB_BYTES[0] && type[1] == INTEREST_CCNB_BYTES[1])
    {
      return HeaderHelper::INTEREST_CCNB;
    }
  else if (type[0] == CONTENT_OBJECT_CCNB_BYTES[0] && type[1] == CONTENT_OBJECT_CCNB_BYTES[1])
    {
      return HeaderHelper::CONTENT_OBJECT_CCNB;
    }
  else if (type[0] == INTEREST_CCNX_BYTES[0] && type[1] == INTEREST_CCNX_BYTES[1])
    {
      return HeaderHelper::INTEREST_CCNX;
    }
  else if (type[0] == CONTENT_OBJECT_CCNX_BYTES[0] && type[1] == CONTENT_OBJECT_CCNX_BYTES[1])
    {
      return HeaderHelper::CONTENT_OBJECT_CCNX;
    }
  else if (type[0] == INTEREST_CCNSIM_BYTES[0] && type[1] == INTEREST_CCNSIM_BYTES[1])
    {
      return HeaderHelper::INTEREST_CCNSIM;
    }
  else if (type[0] == CONTENT_OBJECT_CCNSIM_BYTES[0] && type[1] == CONTENT_OBJECT_CCNSIM_BYTES[1])
    {
      return HeaderHelper::CONTENT_OBJECT_CCNSIM;
    }

  NS_LOG_DEBUG (*packet);
  throw UnknownHeaderException();
}

} // namespace ccn
} // namespace ns3