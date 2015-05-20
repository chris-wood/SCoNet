/* -*- Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil -*- */
/*
 * Copyright (c) 2014,2015 University of California, Santa Cruz
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

#ifndef _ICN_TLV_TLVDATA_H_
#define _ICN_TLV_TLVDATA_H_
/* CCN 1.0 TLV  HDR packet
 1                   2                   3
 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
 +---------------+---------------+---------------+---------------+
 |            T_TYPE             |             Length            |
 +---------------+---------------+---------------+---------------+
 /               Data (of size Length, containing TLVs)          /
 +---------------+---------------+---------------+---------------+

 This class is for when the data consists of TLVs.

 */

#include <list>
#include "tlv_base.h"
#include "RangeDataTemplate.h"

namespace wire {
namespace icnx {

//For range base operations
typedef RangeData<TLV_TYPE_t> TlvRange;

/**
 * @ingroup icn
 * @brief ICN TLV Data Name \
 * TLV class to contain other TLV's
 **/
class TLV_TLVDATA: public TLV_BASE {
public:

	/**
	 * @brief Copy constructor
	 */
	TLV_TLVDATA();
	~TLV_TLVDATA();

    /**
   * @brief Copy constructor from reference TLV
   **/
	TLV_TLVDATA(const TLV_TLVDATA & tlv_msg);

  /**
   * @brief Copy constructor from serialized byte data
   **/
	TLV_TLVDATA(const TLV_RAW_DATA_t * rawData);

  /**
    * @brief Copy constructor from type, length and raw data information
    **/
	TLV_TLVDATA(TLV_TYPE_t type, TLV_LENGTH_t length,
			const TLV_RAW_DATA_t * data);
  /**
    * @brief Copy constructor from a list of TLVs
    **/
	TLV_TLVDATA(const std::list<TLV_BASE *> &tlvDataList);
  
    /**
   * @brief Copy constructor from XML formatted string 
   **/  
	TLV_TLVDATA(const std::string & xmlString);


	/**
	 * \brief Get Data
	 *
   * Return pointer to internal raw data represention
   * of internal TLV's
	 * 
	 */
	virtual const TLV_RAW_DATA_t *
	GetData() const;

	/**
	 * \brief Is TLV contained 
	 *
   * Return true/false if the specified TLV type
	 * is contained within the Value field
	 */
	virtual bool
	ExistTlvData(TLV_TYPE_t type) const;

	/**
	 * \brief Is TLV contained 
	 *
   * Return true/false if the TLV type in the specified range
	 * is contained within the Value field
	 */
	virtual bool
	ExistTlvData(TLV_TYPE_t typeLow,TLV_TYPE_t typeHigh ) const;

  	/**
	 * \brief Is TLV contained 
	 *
   * Return true/false if the TLV type in the specified range
	 * is contained within the Value field
	 */
	virtual bool
	ExistTlvData(TlvRange &rangeData ) const;

  /**
	 * \brief Get Value field size (full raw sizes of all contained TLV's)
	 */
	virtual TLV_LENGTH_t GetLength() const;

	/**
	 * \brief Return a copy of all specified TLV's contained in Value field
   *
   * Return TLV's in a list, of type TLV_TYPE_t.   A value of ALL_TLVS 
   * returns all TLV's contained.
	 */	
  virtual void
	GetTlvDataLinkedList(std::list<TLV_BASE *> &copy, TLV_TYPE_t type =
			ALL_TLVS) const;

  	/**
	 * \brief Return a copy of all specified TLV pointers contained in Value field
   *
   * Return TLV pointers in a list, in a range of TLV_TYPE_t.  
     */	
  virtual void
	GetTlvDataLinkedList(std::list<TLV_BASE *> &copy, TLV_TYPE_t lower, TLV_TYPE_t upper) const;
  
  /**
	 * \brief Return a copy of all specified TLV pointers contained in Value field
   *
   * Return TLV pointers in a list, in a range of TLV_TYPE_t.  
     */	
  virtual void
	GetTlvDataLinkedList(std::list<TLV_BASE *> &copy, TlvRange &tlvRange) const;


	/**
	 * \brief Return the pointer to the first matched specified TLV's contained in Value field
   * Return a pointer to the matched TLV's in the Value field.
	 */
  virtual TLV_BASE *
	GetFirstTlvData(TLV_TYPE_t type);

  	/**
	 * \brief Return the pointer to the first matched TLV's in the specified range, 
   * contained in Value field
   *
   * Return a pointer to the matched TLV's in the Value field.
	 */
  virtual TLV_BASE *
	GetFirstTlvData(TLV_TYPE_t lower, TLV_TYPE_t upper);

  	/**
	 * \brief Return the pointer to the first matched TLV's in the specified range, 
   * contained in Value field
   *
   * Return a pointer to the matched TLV's in the Value field.
	 */
  virtual TLV_BASE *
	GetFirstTlvData(TlvRange &tlvRange);

  /**
   * \brief Set Data with raw TLV Data and length
   *
   * Set the data with a TLV pkt from raw data.   size
   * param is not used, but kept for api conformance for SetData.
   * @param[in] Set raw data
   */
	virtual void 
    SetData( const TLV_RAW_DATA_t * data, TLV_LENGTH_t size=0);

  /**
   * \brief Set raw Data from a TLV 
   *
   * Set the raw data from another tlv-container type TLV.
   * Creates a deep copy of the TLV before 'set' the data,
   * This allows stack created TLV's to be used.
   * @param[in] Set TLV
   */
	virtual void
    SetData( TLV_BASE * tlv);

  /**
   * \brief Append a TLV (via deep copy) into the 'Value' or 'Data' field
   *
   * Set the raw data from another tlv-container type TLV.
   * @param[in] Set TLV
   */
  virtual void 
    AppendTlvData( TLV_BASE * appendTlv);

  /**
   * \brief Copy a list of TLV pointers to this tlv-container 
   * Copy a linked list of TLV pointers to this-tlv container
   * @param[in] Set TLV list
   */
	virtual void 
    SetTlvData( std::list<TLV_BASE *> &tlvDataList); //linked list tlv

  /**
   * \brief Copy a list of TLV's to this tlv-container 
   * Copy a linked list of TLV's to this-tlv container
   * @param[in] Set TLV list
   */
  virtual void 
    SetData( std::list<TLV_BASE *> &tlvDataList); 
  
  /**
   * \brief Delete all matching TLV's from this Value Tlv container 
   * Delete all specified (or all) TLV's from this tlv container. 
   * @param[in] TLV type to be deleted 
   */	
  virtual void 
    DeleteTlvData(TLV_TYPE_t type = ALL_TLVS);

  /**
   * \brief Delete all  TLV's in the specified range, from this container 
   *
   * Delete all TLV's in the specified range, from this tlv container. 
   * @param[in] TLV types to be deleted 
   */	
  virtual void
    DeleteTlvData(TLV_TYPE_t lowId, TLV_TYPE_t highId);

  /**
   * \brief Delete all  TLV's in the specified range, from this container 
   *
   * Delete all TLV's in the specified range, from this tlv container. 
   * @param[in] TLV Range to be deleted 
   */	
    virtual void
    DeleteTlvData( TlvRange &tlvRange);


 /**
   * @brief Routine to create a byte ordered serialized buffer of TLV format.
   *
   **/
	virtual void CreateSerializedBuffer() const;

    /**
    * @brief Copy tlv data into self
    * @param TLV to be copied into self
    **/
  virtual void CopyFromTlv(const TLV_TLVDATA & tlv);

  /**
    * @brief Copy operator for TLV
    *
    **/
  virtual TLV_TLVDATA & operator =(const TLV_TLVDATA & other);
 /**
   * @brief function returning a new instance of the same TLV Content
   **/
    TLV_BASE * 
    Copy() const;

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

  /**
	 * @brief create raw data buffer based upon set attributes.
	 * 
	 */
  mutable std::list<TLV_BASE *> tlvList;
};

}                             // namespace ccn
}                               // namespace ns3

#endif
