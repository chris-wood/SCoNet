/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil -*- */
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
 * Author: Ilya Moiseenko <iliamo@cs.ucla.edu>
 *         Alexander Afanasyev <alexander.afanasyev@ucla.edu>
 *
 * Modification Author:
 *         James Mathewson <jlmathew@soe.ucsc.edu>
 *
 * Note:  ccn-data was modified to use icnx vers 1.0, with TLV support.   While
 * most of the code base has been replaced or rewritten, the core used was from
 * the ccn implementation, and thus, proper author credit is observed.
 */

#ifndef _CCN_CONTENT_OBJECT_HEADER_H_
#define _CCN_CONTENT_OBJECT_HEADER_H_

#include "ns3/simple-ref-count.h"
#include "ns3/nstime.h"
#include "ns3/packet.h"
#include "ns3/ptr.h"

#include <ns3/ccn-name.h>
#include <ns3/ccnSIM/model/wire/icnx/ccn-1.0_content_tlv.h>
#include <ns3/ccnSIM/model/wire/icnx/ccn-1.0_tlv-helper-base.h>
#include "ns3/ccn-common-object.h"

namespace ns3 {
namespace ccn {

/**
 * @ingroup ccn
 * @brief Data header
 */
class Data : public SimpleRefCount<Data>
{
public:
  /**
   * @brief Constructor
   *
   * Converts packet into a content TLV
   **/
  Data (Ptr<Packet> packet = 0);
  Data (::wire::icnx::TLV_CONTENT *tlv);

  /**
   * @brief Copy constructor
   */
  Data (const Data &other);
    /**
   * \brief Set Data PktVersion value
   * Set the version of the Data packet.  Default is version 0.
   */
   void
  SetDataPktVersion(uint8_t version);

    /**
   * \brief Get Data PktVersion value
   * Get the version of the Data packet. 
   */   
   uint8_t
  GetDataPktVersion() const;

  //Data(const uint8_t *data, uint16_t length);

  /**
   * \brief Set content object name
   *
   * Sets name of the content object
   **/
   void
  SetName (Ptr<Name> name);

  /**
   * @brief Another, less efficient, variant of setting content object name
   *
   * Sets name of the content object
   */
   void
  SetName (const Name &name);
  void
SetName (const std::string &name);

  /**
   * @brief Get name of the content object
   */
  const  Name
  GetName () const;


  /**
   * @brief Get name of the content object
   */
   std::string
  GetStringName () const;


  uint8_t
  GetVersion() const;
  
  void
  SetVersion(uint8_t version);

  /**
   * @brief set payload of TLV with the data packet
   *
   *    */
   void
  SetPayload (Ptr<Packet> payload);

   /**
   * @brief set payload of TLV with raw data
   *
   *    */
  void
  SetPayload(uint8_t *data, uint16_t size);

  /**
   * @brief Get payload of data TLV, convert to packet format
   *
   */
   Ptr<const Packet>
  GetPayload () const;

  /**
   * @brief Get payload of data TLV, pass pointer to byte format
   *
   */ 
   const uint8_t *
   GetPayload(uint16_t &size) const;


uint8_t
GetPayloadType() const;

void
SetPayloadType(uint8_t type);

void
SetDataType(uint8_t type);


  /**
   * @brief Get serialized wire formatted packet
   *
   * Serialize TLV into wire format via ascii XML or endian orientated byte stream
   */

      ns3::Ptr< Packet>
     GetPacketSerialize() const;

  /**
   * @brief Get serialized wire format method
   *
   * Return the serialization method of the TLV packets (XML/Byte)
   */
void
SetWireFormat(WireFormat format) const ;

  WireFormat 
GetWireFormat () const ;

  /**
   * @brief 'Set' the wire formatted packet into TLV format
   *
   * Set the method for Data serialization (XML/Byte)
   */
     void
  SetTlvViaPkt (ns3::Ptr<const Packet> packet) const;

  /**
   * @brief Print Data in plain-text to the specified output stream
   */
  void
  Print (std::ostream &os) const;


   /**
     * @brief Return the pointer to the actual TLV representing Data
     *
     * Changes to the TLV pointer will affect the correspodning Data object
     **/
   wire::icnx::TLV_CONTENT *
   GetTlv() const ;

//   ns3::Ptr< ::wire::icnx::TLV_CONTENT > GetTlv() const;

   /**
     * Set the Data object with the TLV specified
     *
     * Changes to the TLV pointer will affect the correspodning Data object
     **/
  //SetTlv (ns3::Ptr<const Packet> packet) const;
   void SetTlv( ::wire::icnx::TLV_CONTENT *tlv );

   Ptr<const Packet>
  GetXmlWire () const;
void
SetXmlWire (ns3::Ptr<const Packet> packet) ;


  void
  SetExpiry(ns3::Time ns3Time);
  void
  SetExpiry(uint64_t timeExp);
  ns3::Time
  GetExpiry() const;
  uint64_t
  GetExpiry64() const;
  bool
  ExistExpiry() const;
  void
  DeleteExpiry();

  void
  SetCacheTime(ns3::Time ns3Time);
  void
  SetCacheTime(uint64_t timeExp);
  ns3::Time 
  GetCacheTime() const;
  uint64_t
  GetCacheTime64() const;
  bool
  ExistCacheTime() const;
  void
  DeleteCacheTime();
  

private:
  // NO_ASSIGN
  Data &
  operator = (const Data &other) { return *this; }
  
private:

  mutable wire::icnx::TLV_CONTENT *m_tlv; //< @brief tlv which holds all interest information
  const wire::icnx::tlvUtils::TlvHelperBase *m_tlvHelper; 
  mutable WireFormat m_wireFormat;

};

inline std::ostream &
operator << (std::ostream &os, const Data &d)
{
  d.Print (os);
  return os;
}



inline void
Data::SetTlvViaPkt(ns3::Ptr<const Packet> packet) const
{
    uint32_t size = packet->GetSize();
    uint8_t *data = new uint8_t[size];
    packet->CopyData(data, size);
     m_tlv->NetworkDeserialize(data);
    delete data;

}
} // namespace ccn
} // namespace ns3

#endif // _CCN_CONTENT_OBJECT_HEADER_H_
