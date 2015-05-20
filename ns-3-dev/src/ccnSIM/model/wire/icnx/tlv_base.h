/* -*- Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil -*- */
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
 * Author: James Mathewson <jlmathew@soe.ucsc.edu>
 *
 * 
 */

#ifndef _ICN_TLV_BASE_H_
#define _ICN_TLV_BASE_H_
/** @mainpage Base TLV Packet
* Based upon an CCNX 1.0 TLV packet
*                     1                   2                   3
* 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
* +---------------+---------------+---------------+---------------+
* |            T_TYPE             |             Length            |
* +---------------+---------------+---------------+---------------+
*
* This class defines the base syntax and handles type/length field of
* a TLV.    Derived classes defined HOW the data is used.
**/

#include <ostream>
#include <stdint.h>
#include <string>
#include <sstream>
#include <netinet/in.h>
#include <string.h>
#include <iostream>
#include <iomanip>
#include <assert.h>
#include <stdlib.h>
#include <stdexcept>
#include "ccn-1.0_tlv-id.h"
#include "pugixml.h"
#include "ns3/simple-ref-count.h"


namespace wire {
namespace icnx {

/**
 * @class TLV_BASE
 * @ingroup icnx
 * @brief ICNX TLV Base class 
 *
 * This base class supports the basic TLV function, 16 bit TYPE,
 * 16 bit Length, and variable length VALUE fields.
 *
 **/

class TLV_BASE : public ns3::SimpleRefCount<wire::icnx::TLV_BASE>
//class TLV_BASE  //used for simple testing and verificatio
{
public:

  /**
   * @brief Copy constructor
   */
  TLV_BASE ();
  
  virtual ~TLV_BASE ();

  /**
   * @brief Copy constructor from reference TLV
   **/
  TLV_BASE (const TLV_BASE &tlv);

    /**
   * @brief Copy constructor from XML formatted string 
   **/
  TLV_BASE (const std::string &xmlString);

  /**
   * @brief Copy constructor from serialized byte data
   **/
  TLV_BASE(const TLV_RAW_DATA_t * rawData);

  /**
    * @brief Copy constructor prototype 
    **/
 	TLV_BASE(TLV_TYPE_t type, TLV_LENGTH_t length,
	  const TLV_RAW_DATA_t * data);

  /**
   * @brief Set Type
   * @param type Set type of TLV message
   * 
   **/
  virtual void 
  SetType (TLV_TYPE_t type);

  /**
   * \brief Get Type
   * Return type of tlv msg
   * 
   */
  virtual TLV_TYPE_t
  GetType () const;

  /**
   * @brief Get TLV data length
   * @Return length of tlv value field in bytes
   * 
   **/
  virtual TLV_LENGTH_t
  GetLength () const;

  /**
	 * \brief Set raw Data and length
	 * Set the raw data via memcpy and size of the TLV pkt
	 * @param[in] Set raw data, set size
	 */
	virtual void SetData(const TLV_RAW_DATA_t * data, TLV_LENGTH_t size);

  	/**
	 * \brief Get Data
	 * Return object data pointer to internal data structure
	 * GetData() is to remain tlv compatibility with all data types
	 */
  virtual const TLV_RAW_DATA_t *GetData() const;

  /**
   * @brief Get TLV size (size of type/length field, plus length)
   * Return The serialized length of tlv in bytes 
   * 
   **/
  virtual TLV_LENGTH_t
  GetSerializedSize () const;

  /**
   * @brief Serialization routine which creates a TLV pointer from from raw data
   *
   * @param data stream to create a correct TLV class.
   * @Return Pointer to a new TLV objected instantiated from raw data via deep copy
   * 
   **/
  virtual TLV_BASE *
  NetworkDeserialize(const TLV_RAW_DATA_t *data) ;


  /**
   * @brief Create a TLV object from an istream object (e.g. cin)
   *
   * @param iss input data stream containing XML formated as string
   * @Return Pointer to a new TLV objected instantiated from input stream
   * 
   * Each derived data type (raw, container, endian) calls their own
   * data interpretation routine.
   **/
  virtual TLV_BASE *
  XmlImplement( std::istream &iss); 

  /**
   * @brief Create a TLV object from an string object with XML code
   *
   * @param xmlString input data stream containing XML formated as string
   * @Return Pointer to a new TLV objected instantiated from raw data
   * 
   **/
  virtual  TLV_BASE*
  XmlImplement (const std::string &xmlString);

  /**
   * @brief convert TLV fields to network byte fields
   * @Return A pointer to the TLV's internal buffer, which represents itself as a byte serialization stream.
   *
   **/
  virtual const TLV_RAW_DATA_t *
  NetworkSerialize () const;

virtual  void
  XmlPrint (std::ostream &os , int space=0) const;

virtual  void
  Print (std::ostream &os , int space=0) const;
  /**
   * @brief Print TLV in plain-text XML format to the return string
   * @return String containing an ascii XML format of the TLV
   **/
virtual std::string 
  XmlPrint () const;

/**
  * @brief output redirect for TLV pointer
  *
  **/
friend std::ostream &
operator << (std::ostream &os, const TLV_BASE &i);

/**
  * @brief input redirect for TLV
  *
  **/
friend std::istream &
operator >> (std::istream *iss, TLV_BASE &i);

 /**
   * @brief Routine to create a byte ordered serialized buffer of TLV format.
   *
   **/
  virtual void
  CreateSerializedBuffer() const;  

  /**
    * @brief Copy tlv data into self
    * @param TLV to be copied into self
    **/
  virtual void
  CopyFromTlv(const TLV_BASE &tlv);
  
  /**
    * @brief Copy operator for TLV
    *
    **/

  TLV_BASE &
  operator = (const TLV_BASE &other); 

/**
  * @brief function returns a string name for the class
  **/
 virtual const std::string 
 tlvIdName() const;

 /**
   * @brief function returning the parent class of the TLV
   **/
 virtual const std::string 
 tlvBaseName() const;

 /**
   * @brief function returning a new instance of the same TLV Content
   **/
    TLV_BASE * 
    Copy() const;

protected:

  TLV_TYPE_t m_type;
  TLV_LENGTH_t m_length;
  uint32_t m_serializedDataSize;
  mutable TLV_RAW_DATA_t *m_serializedDataPtr; /**< Serialized Data Pointer for easy usage */
  static const int TLV_BASE_SIZE = sizeof(TLV_LENGTH_t)+sizeof(TLV_TYPE_t);  /**< static value for the length of TYPE+LENGTH fields */
  mutable bool m_dataChanged; /**< variable to compute when a change to the TLV occurred to remove redundant serializations */
  
  /*
   * @brief Print out the TLV header (type/length) in an ascii xml format
   *
  **/
  virtual void
  XmlPrintHeader(std::ostream &os, int numspace=0) const;

  /*
   * @brief Print out the TLV Value field (data ) in an ascii xml format
   *
  **/
  virtual void
  XmlPrintData(std::ostream &os, int numspace=0) const;

  /*
   * @brief Print out the TLV Value field (data ) in an ascii xml format
   *
  **/
	virtual void
	NetworkDataDeserialize(const TLV_RAW_DATA_t *data);

    /*
   * @brief Implement Xml into our Value/data field 
   *
  **/
    virtual void
  XmlDataImplementation(pugi::xml_node &tlvXml);

};



} // namespace icnx
} // namespace ns3

#endif // _CCN_BASE_HEADER_H_
