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

#ifndef _ICN_TLV_RAWDATA_H_
#define _ICN_TLV_RAWDATA_H_
/* CCN 1.0 TLV packet
 1                   2                   3
 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
 +---------------+---------------+---------------+---------------+
 |            T_TYPE             |             Length            |
 +---------------+---------------+---------------+---------------+
 /               Data (of size Length)                           /
 +---------------+---------------+---------------+---------------+

 Base class for TLV's, which data is non Endian nor a Container
 */

#include "tlv_base.h"

namespace wire {
namespace icnx {

/**
 * @ingroup icn
 * @brief ICN TLV Data Name
 * TLV class with non endian data (e.g. string)
 **/
class TLV_RAWDATA: public TLV_BASE {
public:

	/**
	 * @brief Copy constructor
	 */
	TLV_RAWDATA();
	~TLV_RAWDATA();
  /**
   * @brief Copy constructor from reference TLV
   **/
  TLV_RAWDATA(const TLV_RAWDATA & tlv_msg);
  /**
   * @brief Copy constructor from serialized byte data
   **/
	TLV_RAWDATA(const TLV_RAW_DATA_t * rawData);
    /**
   * @brief Copy constructor from XML formatted string 
   **/
	TLV_RAWDATA(const std::string & xmlString);
 
  /**
    * @brief Copy constructor from type, length and raw data information
    **/
	TLV_RAWDATA(TLV_TYPE_t type, TLV_LENGTH_t length,
			const TLV_RAW_DATA_t * data);

	/**
	 * \brief Get Data
	 * Return object data pointer to internal data structure
	 * GetData() is to remain tlv compatibility with all data types
	 */
  virtual const TLV_RAW_DATA_t *GetData() const;

  	/**
	 * \brief Get copy of Data
	 * Return object data pointer (allocated) with a copy of the internal data structure
   * Data is allocated with new[], and up to the user to delete properly.
	 */
  virtual TLV_RAW_DATA_t *GetCopyData() const;

	//we only want the character std::string
	/**
	 * \brief Get string Data
	 * Return string representation of internal data.
	 * Functionally same as GetData(string)
	 */
	virtual std::string 
  GetStrData() const;


	/**
	 * \brief Set string Data
	 * Store (and copy) the string representation to internal data.
   */
	virtual void SetData(const std::string & strData);


	/**
	 * \brief GetObjectData
	 * Template function to retrieve ANY data type in a tlv.
	 * The parameter 'shallowCopy' is HOW it will be copied.
   * If 'shallowCopy' is true, the default C++ shallowCopy method is used
   * (aka pointer values are not properly copied).   If false, then the redirect
   * operator is used to copy data (e.g. data << stringRepresentationData).
   * 
   * @param[in] Object Content to be set into TLV, and type of copy.
   */
  template<class TLV_DATA_OBJECT> void
	GetObjectData(TLV_DATA_OBJECT & obj, bool shallowCopy = true);

	/**
	 * \brief Set raw Data and length
	 * Set the raw data and size of the TLV pkt
	 * @param[in] Set raw data, set size
	 */
	virtual void SetData(const TLV_RAW_DATA_t * data, TLV_LENGTH_t size);

	//use io input for class instantiation
	/**
	 * \brief SetObjectData
	 * Template function to store ANY data type in a tlv.
	 * The parameter 'shallowCopy' is HOW it will be copies.
   * If 'shallowCopy' is true, the default C++ shallowCopy method is used
   * (aka pointer values are not copies).   If false, then the string redirect
   * is used to copy data (e.g. string representationData << data).
   * 
   * @param[in] Object Content to be set into TLV, and type of copy.
   */
	template<class TLV_DATA_OBJECT> void
	SetObjectData(const TLV_DATA_OBJECT & obj, bool shallowCopy = true);


 /**
   * @brief Routine to create a byte ordered serialized buffer of TLV format.
   *
   **/
	virtual void CreateSerializedBuffer() const;
  
  /**
    * @brief Copy tlv data into self
    * @param TLV to be copied into self
    **/
  virtual void
   CopyFromTlv(const TLV_RAWDATA & tlv);

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
    * @brief Copy operator for TLV
    *
    **/

  TLV_RAWDATA &
  operator = (const TLV_RAWDATA &other); 

   /**
   * @brief function returning a new instance of the same TLV Content
   **/
    TLV_BASE * 
    Copy() const;

protected:
    /*
   * @brief Implement Xml into our Value/data field 
   *
  **/
    virtual void
  XmlDataImplementation(pugi::xml_node &tlvXml);

  /*
   * @brief Interpret raw data into tlv data field
   *
  **/
	virtual void
	NetworkDataDeserialize(const TLV_RAW_DATA_t * data);

    /*
   * @brief Print out the TLV Value field (data ) in an ascii xml format
   *
  **/
  virtual void
  XmlPrintData(std::ostream &os, int numspace=0) const;

	TLV_RAW_DATA_t * m_DataPtr; /**< This pointer holds the raw data Value field of the TLV */

};

}                             // namespace ccn
}                               // namespace ns3

#endif // _CCN_BASE_HEADER_H_
