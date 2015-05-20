/* -*- Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil -*- */
/*
 * Copyright (c) 2011-2013 University of California, Los Angeles
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
 *         James Mathewson <jlmathew@soe.ucsc.edu>
 *
*/

#ifndef __CCN_COMMON_OBJECT_DEF__
#define __CCN_COMMON_OBJECT_DEF__

#include "ns3/packet.h"
#include "ns3/ptr.h"
#include "ns3/object.h"
#include "ns3/ccn-common-object.h"

namespace ns3 
{
namespace ccn
{

  //common definitions, like wire format, are here for Data/Interest/InterestResponse
  enum WireFormat {
     UNDEF = 0,
     SERIAL_ENC = 1,
     XML_ENC = 2
  };


  class EncodingTypeHeader : public Header
  {
    public:

     EncodingTypeHeader(WireFormat format = SERIAL_ENC)
     {
            m_wireFormat = static_cast<uint8_t>(format);
     }
     static TypeId GetTypeId(void)
     {
       static TypeId tid = TypeId("ns3::ccn:EncodingTypeHeader")
       .SetGroupName("Ccn")
       .SetParent<Header> ()
       .AddConstructor<EncodingTypeHeader>();

       return tid;
      }

       uint32_t
       GetLength() const
       { return sizeof(m_wireFormat); }

       virtual TypeId
       GetInstanceTypeId(void) const
       {
         return EncodingTypeHeader::GetTypeId();
       }

       virtual uint32_t
        GetSerializedSize(void) const
        {
           return GetLength();
        }
  virtual void
  Print (std::ostream &os) const
  {
    os << "[" << (int) m_wireFormat << "]";
  }

  virtual void
  Serialize (Buffer::Iterator start) const
  {
    start.WriteU8 (m_wireFormat);
  }

  virtual uint32_t
  Deserialize (Buffer::Iterator start)
  {
    m_wireFormat = start.ReadU8 ();
    return 1;
  }
 
  uint8_t
  Value()
  {
    return m_wireFormat;
  }

       private:
       uint8_t m_wireFormat;

  };
  NS_OBJECT_ENSURE_REGISTERED(EncodingTypeHeader);
}
}

#endif
