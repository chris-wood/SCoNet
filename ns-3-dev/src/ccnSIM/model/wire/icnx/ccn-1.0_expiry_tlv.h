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

#ifndef _CCN_EXPIRY_H_
#define _CCN_EXPIRY_H_
/* CCN 1.0 CACHE EXPIRY HDR packet
                    1                   2                   3
0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
+---------------+---------------+---------------+---------------+
|          T_EXPIRY             |             length            |
+---------------+---------------+---------------+---------------+
/                                                               /
/                   Lifetime (length octets)                    /
/                                                               /
+---------------+---------------+---------------+---------------+

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
class TLV_EXPIRY:public TLV_ENDIANDATA
{
public:

  /**
   * @brief Copy constructor
   */
  TLV_EXPIRY ();
  ~TLV_EXPIRY ();

   /**
   * @brief Copy constructor from reference TLV
   **/
  TLV_EXPIRY (const TLV_EXPIRY &tlv_msg);

   /**
   * @brief Copy constructor from serialized byte data
   **/
  TLV_EXPIRY (const TLV_RAW_DATA_t *rawData);
 
            /**
   * @brief Copy constructor from XML formatted string 
   **/
  TLV_EXPIRY (const std::string &xmlString);

    /**
    * @brief Copy constructor from type, length and raw data information
    **/
        TLV_EXPIRY (TLV_TYPE_t type, TLV_LENGTH_t length,
                     const TLV_RAW_DATA_t * data);

  /**
    * @brief Copy constructor from a 64 bit milisecond time interal
    *
    **/
  TLV_EXPIRY (uint64_t time);

 /**
   * @brief function returning a new instance of the same TLV Content
   **/
    TLV_BASE * 
    Copy() const;
  /**
   * \brief Get TLV long time
   * Return time in milliseconds 
   * 
   */
  uint64_t
  GetExpirationTime () const;


  /**
   * \brief Set Tlv  Time
   * Set time of tlv
   * 
   */
  virtual void 
  SetExpirationTime (uint64_t time);

/**
  * @brief function returns a string name for the class
  **/
 virtual const std::string 
 tlvIdName() const;

  /**
    * @brief Copy operator for TLV
    *
    **/
  virtual TLV_EXPIRY &
  operator = (const TLV_EXPIRY &other); 

};

} // namespace icnx
} // namespace wire

#endif 
