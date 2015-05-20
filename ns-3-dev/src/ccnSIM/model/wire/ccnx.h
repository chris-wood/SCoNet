/** -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil -*- */
/* 
 * Copyright (c) 2013, Regents of the University of California
 *                     Alexander Afanasyev
 * 
 * GNU 3.0 license, See the LICENSE file for more information
 * 
 * Author: Alexander Afanasyev <alexander.afanasyev@ucla.edu>
 */

#ifndef CCN_WIRE_CCNXSIM_H
#define CCN_WIRE_CCNXSIM_H

#include "ns3/ccn-common.h"
#include "ns3/ccn-interest.h"
#include "ns3/ccn-interestresponse.h"
#include "ns3/ccn-data.h"
#include "icnx/ccn-1.0_interest_tlv.h"
#include "icnx/ccn-1.0_interestresponse_tlv.h"
#include "icnx/ccn-1.0_content_tlv.h"

CCN_NAMESPACE_BEGIN

/**
 * @brief Namespace encapsulating wire operations
 */
namespace wire {

/**
 * @brief Namespace for ccnSIM wire format operations
 */
namespace icnx {
/*
class InterestResponse : public Header
{
public:
  InterestResponse ();
  InterestResponse (Ptr<ccn::InterestResponse> interestResponse);

  Ptr<ccn::InterestResponse>
  GetInterestResponse ();

  static Ptr<Packet>
  ToWire (Ptr<const ccn::InterestResponse> interestResponse);
  static Ptr<Packet>
  ToXmlWire (Ptr<const ccn::InterestResponse> interestResponse);
  static Ptr<ccn::InterestResponse>
  FromWire (Ptr<Packet> packet);
  static Ptr<ccn::InterestResponse>
  FromXmlWire (Ptr<Packet> packet);
  
  // from Header
  static TypeId GetTypeId (void); 
  virtual TypeId GetInstanceTypeId (void) const; 
  virtual void Print (std::ostream &os) const;
  virtual uint32_t GetSerializedSize (void) const;
  virtual void Serialize (Buffer::Iterator start) const;
  virtual uint32_t Deserialize (Buffer::Iterator start);

private:
  Ptr<ccn::InterestResponse> m_interestResponse;
  //Ptr< ::wire::icnx::TLV_INTEREST >  m_tlv;
};

class Interest : public Header
{
public:
  Interest ();
  Interest (Ptr<ccn::Interest> interest);

  Ptr<ccn::Interest>
  GetInterest ();

  static Ptr<Packet>
  ToWire (Ptr<const ccn::Interest> interest);
  static Ptr<Packet>
  ToXmlWire (Ptr<const ccn::Interest> interest);
  static Ptr<ccn::Interest>
  FromWire (Ptr<Packet> packet);
  static Ptr<ccn::Interest>
  FromXmlWire (Ptr<Packet> packet);
  
  // from Header
  static TypeId GetTypeId (void); 
  virtual TypeId GetInstanceTypeId (void) const; 
  virtual void Print (std::ostream &os) const;
  virtual uint32_t GetSerializedSize (void) const;
  virtual void Serialize (Buffer::Iterator start) const;
  virtual uint32_t Deserialize (Buffer::Iterator start);

private:
  Ptr<ccn::Interest> m_interest;
};

class Data : public Header
{
public:
  Data ();
  Data (Ptr<ccn::Data> data);

  Ptr<ccn::Data>
  GetData ();

  static Ptr<Packet>
  ToWire (Ptr<ccn::Data> data);

  static Ptr<ccn::Data>
  FromWire (Ptr<Packet> packet);
  static Ptr<Packet>
  ToXmlWire (Ptr< ccn::Interest> interest);
  static Ptr<ccn::Interest>
  FromXmlWire (Ptr<Packet> packet);  
  // from Header
  static TypeId GetTypeId (void);
  virtual TypeId GetInstanceTypeId (void) const;
  virtual void Print (std::ostream &os) const;
  virtual uint32_t GetSerializedSize (void) const;
  virtual void Serialize (Buffer::Iterator start) const;
  virtual uint32_t Deserialize (Buffer::Iterator start);

private:
  Ptr<ccn::Data> m_data; 
  //Ptr< ::wire::icnx::TLV_CONTENT > m_tlv;
};
*/
} // ccnSIM
} // wire

CCN_NAMESPACE_END

#endif // CCN_WIRE_CCNSIM_H
