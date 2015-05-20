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

#ifndef _TLV_INTERESTHOP_H_
#define _TLV_INTERESTHOP_H_
/* CCN 1.0  Interest HOP type (INT) HDR packet
                    1                   2                   3
0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
+---------------+---------------+---------------+---------------+
|          T_INT_INTERESTHOP    |             length (1)        |
+---------------+---------------+---------------+---------------+
/                                                               /
/                Payload Value (1 octets)                       /
/                                                               /
+---------------+---------------+---------------+---------------+
http://www.icnx.org/pubs/icnx-mosko-tlvmessages-01.pdf

Replacing the interest hop field in the fixed header!

*/
//#include "ccn-1.0_tlv-id.h"
//#include "tlv_base.h"
#include "tlv_endiandata.h"


namespace wire {
namespace icnx {


/**
 * @ingroup ccn
 * @brief CCN TLV OPT per hop lifetime (wire formats are defined in wire)
 **/
class TLV_INTERESTHOP:public TLV_ENDIANDATA
{
public:


  /**
   * @brief Copy constructor
   */
  TLV_INTERESTHOP ();
  ~TLV_INTERESTHOP ();

   /**
   * @brief Copy constructor from reference TLV
   **/
  TLV_INTERESTHOP (const TLV_INTERESTHOP &tlv_msg);

   /**
   * @brief Copy constructor from serialized byte data
   **/
  TLV_INTERESTHOP (const TLV_RAW_DATA_t *rawData);

            /**
   * @brief Copy constructor from XML formatted string 
   **/
  TLV_INTERESTHOP (const std::string &xmlString);

  /**
    * @brief Copy constructor from an 8bit hop count value
    *
    **/
  TLV_INTERESTHOP (uint8_t type);


  /**
   * \brief Get Maximum allowed Interest Hops
   * 
   */
  uint8_t 
  GetInterestHops () const;

 /**
   * @brief function returning a new instance of the same TLV Content
   **/
    TLV_BASE * 
    Copy() const;
  /**
   * \brief Set Maximum allowed Interest Hops
   * 
   */
  virtual void 
  SetInterestHops (uint8_t type);

/**
  * @brief function returns a string name for the class
  **/
 virtual const std::string 
 tlvIdName() const;

    /**
   * @brief Get TLV data size
   * Return data length of tlv 
   * 
   */
      virtual TLV_LENGTH_t GetLength () const;

  /**
    * @brief Copy operator for TLV
    *
    **/
 TLV_INTERESTHOP &
  operator = (const TLV_INTERESTHOP &other); 

};

} // namespace icnx
} // namespace wire

#endif // _TLV_INTERESTHOP_H_
