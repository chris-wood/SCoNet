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

#ifndef _CCN_TLV_OBJHASHRESTR_H_
#define _CCN_TLV_OBJHASHRESTR_H_
/* CCN 1.0 TLV DATA OBJHASHRESTR HDR packet
                    1                   2                   3
0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
+---------------+---------------+---------------+---------------+
|            T_TYPE             |             Length (32 or 0)  |
+---------------+---------------+---------------+---------------+
/                   OBJHASHRESTR DATA  (SHA-256)                /
+---------------+---------------+---------------+---------------+
*/

#include "tlv_rawdata.h"

/**
* We dont want to pollute the TLV library with actual implementations unless necessary
* So, we define a SHA256 digest as a container for our methods, and put actual SHA-256
 *calculations in the data/interest/interest response object.   Thus, we use an array
 *of 32 bytes.
**/


namespace wire
{
  namespace icnx
  {


/**
 * @ingroup ccn
 * @brief CCN TLV Data Name 
 **/
    class TLV_OBJHASHRESTR:public TLV_RAWDATA
    {
    public:

  /**
   * @brief Copy constructor
   */
      TLV_OBJHASHRESTR ();
      ~TLV_OBJHASHRESTR ();

  /**
   * @brief Copy constructor from reference TLV
   **/
      TLV_OBJHASHRESTR (const TLV_OBJHASHRESTR & tlv_msg);

  /**
   * @brief Copy constructor from serialized byte data
   **/
        TLV_OBJHASHRESTR (const TLV_RAW_DATA_t * rawData);

    /**
   * @brief Copy constructor from XML formatted string 
   **/
        TLV_OBJHASHRESTR (const std::string & xmlString);

 /**
    * @brief Copy constructor from type, length and raw data information
    *
    * @param length must be zero for no Value field or 32 for SHA-256 digest
    **/
        TLV_OBJHASHRESTR (TLV_TYPE_t type, TLV_LENGTH_t length,
                     const TLV_RAW_DATA_t * data);

 /**
   * @brief function returning a new instance of the same TLV Content
   **/
    TLV_BASE * 
    Copy() const;
      //SetData
	/**
	 * \brief Set raw Data and length
	 * Set the raw data and size of the TLV pkt
	 * @param[in] Set raw data, set size
	 */
	virtual void SetData(const TLV_RAW_DATA_t * data, TLV_LENGTH_t size=32);

 /**
   * \brief Get SHA256 manifest
   * 
   */



 /**
   * \brief Set SHA256 manifest
   * 
   */



    /**
   * @brief Get TLV data size
   * Return data length of tlv 
   * 
   */
      virtual TLV_LENGTH_t GetLength () const;

/**
  * @brief function returns a string name for the class
  **/
 virtual const std::string 
 tlvIdName() const;

  /**
    * @brief Copy operator for TLV
    *
    **/
      virtual TLV_OBJHASHRESTR & operator = (const TLV_OBJHASHRESTR & other);


    };







  }                             // namespace ccn
}                               // namespace ns3

#endif // _CCN_BASE_HEADER_H_
