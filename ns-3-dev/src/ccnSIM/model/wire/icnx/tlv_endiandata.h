/* -*- Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil -*- */
/*
 * Copyright (c) 2014, 2015 University of California, Santa Cruz
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

#ifndef _ICN_TLV_ENDIANDATA_H_
#define _ICN_TLV_ENDIANDATA_H_
/* CCN 1.0 TLV packet
                    1                   2                   3
0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
+---------------+---------------+---------------+---------------+
|            T_TYPE             |             Length            |
+---------------+---------------+---------------+---------------+
/               Endian Data  (1,2,4,8 bytes)                    /
+---------------+---------------+---------------+---------------+

For data with endianess.

*/
#include <stdlib.h>
#include "tlv_base.h"

/* Define the types in a fixed TLV header */


namespace wire
{
  namespace icnx
  {


/**
 * @ingroup ccn
 * @brief ICN TLV Data Name 
 * TLV class to handle Endian (big/little endian matters) data.
 **/
    class TLV_ENDIANDATA:public TLV_BASE
    {
    public:

  /**
   * @brief Copy constructor
   */
      TLV_ENDIANDATA ();
      ~TLV_ENDIANDATA ();
  /**
   * @brief Copy constructor from reference TLV
   **/
      TLV_ENDIANDATA (const TLV_ENDIANDATA & tlv_msg);
  /**
   * @brief Copy constructor from serialized byte data
   **/
      TLV_ENDIANDATA (const TLV_RAW_DATA_t * rawData);

      /**
   * @brief Copy constructor from XML formatted string 
   **/
      TLV_ENDIANDATA (const std::string &xmlString);
      TLV_ENDIANDATA (TLV_TYPE_t type, TLV_LENGTH_t length,
                      const TLV_RAW_DATA_t * data);

      /**
        *
        * @brief Copy constructor based upon type and 64bit data
        **/
      TLV_ENDIANDATA (TLV_TYPE_t type, uint64_t data);

   /**
   * \brief Get TLV size (size of type/length field, plus length)
   * Return full (data+header) length of tlv 
   * 
   */
   //   virtual TLV_LENGTH_t GetSerializedSize () const;

   /**
   * @brief Get TLV data size
   * Return data length of tlv 
   * 
   */
      virtual TLV_LENGTH_t GetLength () const;
	/**
	 * \brief Get Data
	 * Return object data pointer to internal data structure
	 * GetData() is to remain tlv compatibility with all data types
	 */
      virtual const TLV_RAW_DATA_t *GetRawData (bool networkFormat=false) const;

 	/**
	 * \brief Get Data
	 * Return object data pointer to internal data structure
	 * GetData() is to remain tlv compatibility with all data types
	 */
      virtual const TLV_RAW_DATA_t *GetData (bool networkFormat=false) const;


 	/**
	 * \brief Get 8bit Numeric Data
	 * Return 8bit data via reference
	 */
      virtual void GetNumericData (uint8_t & data) const;

 	/**
	 * \brief Get 16bit Numeric Data
	 * Return 16bit data via reference
	 */
      virtual void GetNumericData (uint16_t & data) const;

 	/**
	 * \brief Get 32bit Numeric Data
	 * Return 32bit data via reference
	 */
      virtual void GetNumericData (uint32_t & data) const;

 	/**
	 * \brief Get 64bit Numeric Data
	 * Return 64bit data via reference
	 */
      virtual void GetNumericData (uint64_t & data) const;

	/**
	 * \brief Set raw Data and length
	 * Set the raw data and size of the TLV pkt
	 * @param[in] Set raw data, set size
	 */
      virtual void SetData (const TLV_RAW_DATA_t * data, TLV_LENGTH_t size);
  /**
   * \brief Set Data (8bit)
   * Set the endian data. SetData() is Base tlv compliant  
   * @param[in] Set data (8bit)
   */
      virtual void SetData (uint8_t byteData);
  /**
   * \brief Set Data (8bit)
   * Set the endian data. SetNumericData() is unique to endian tlv's.
   * Functionally identical to SetData()
   * @param[in] Set data (8bit)
   */
      virtual void SetNumericData (uint8_t byteData);

  /**
   * \brief Set Data (16bit)
   * Set the endian data. SetData() is Base tlv compliant  
   * @param[in] Set data (16bit)
   */

      virtual void SetData (uint16_t byteData);
  /**
   * \brief Set Data (16bit)
   * Set the endian data. SetNumericData() is unique to endian tlv's.
   * Functionally identical to SetData()
   * @param[in] Set data (16bit)
   */
      virtual void SetNumericData (uint16_t byteData);

  /**
   * \brief Set Data (32bit)
   * Set the endian data. SetData() is Base tlv compliant  
   * @param[in] Set data (32bit)
   */

      virtual void SetData (uint32_t byteData);
  /**
   * \brief Set Data (32bit)
   * Set the endian data. SetNumericData() is unique to endian tlv's.
   * Functionally identical to SetData()
   * @param[in] Set data (32bit)
   */
      virtual void SetNumericData (uint32_t byteData);
  /**
   * \brief Set Data (64bit)
   * Set the endian data. SetData() is Base tlv compliant  
   * @param[in] Set data (64bit)
   */
      virtual void SetData (uint64_t byteData);
  /**
   * \brief Set Data (64bit)
   * Set the endian data. SetNumericData() is unique to endian tlv's.
   * Functionally identical to SetData()
   * @param[in] Set data (64bit)
   */
      virtual void SetNumericData (uint64_t byteData);

      /**
   * @brief Routine to create a byte ordered serialized buffer of TLV format.
   *
   **/
      virtual void CreateSerializedBuffer () const;

  /**
    * @brief Copy tlv data into self
    * @param TLV to be copied into self
    **/
      virtual void CopyFromTlv (const TLV_ENDIANDATA & tlv);

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


  /**
    * @brief Copy operator for TLV
    *
    **/
      virtual TLV_ENDIANDATA & 
      operator = (const TLV_ENDIANDATA & other);

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

      uint64_t m_endianData;
      //other persistent data for const data* return types
      mutable uint8_t u8data;
      mutable uint16_t u16data;
      mutable uint32_t u32data;
      mutable uint32_t u32endiandata[2];
      mutable uint64_t u64data;


    };




  }                             // namespace ccn
}                               // namespace ns3

#endif // _CCN_BASE_HEADER_H_
