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

#ifndef _CCN_TLV_CRC32_H_
#define _CCN_TLV_CRC32_H_
/* CCN 1.0 TLV CRC32 HDR packet
                    1                   2                   3
0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
+---------------+---------------+---------------+---------------+
|            T_TYPE             |             Length            |
+---------------+---------------+---------------+---------------+
/                       CRC32                                   /
+---------------+---------------+---------------+---------------+

*/

#include "tlv_rawdata.h"


namespace wire
{
  namespace icnx
  {


/**
 * @ingroup ccn
 * @brief CCN TLV Data Name 
 **/
    class TLV_CRC32:public TLV_RAWDATA
    {
    public:

  /**
   * @brief Copy constructor
   */
      TLV_CRC32 ();
      ~TLV_CRC32 ();

  /**
   * @brief Copy constructor from reference TLV
   **/
      TLV_CRC32 (const TLV_CRC32 & tlv_msg);

  /**
   * @brief Copy constructor from serialized byte data
   **/
        TLV_CRC32 (const TLV_RAW_DATA_t * rawData);

    /**
   * @brief Copy constructor from XML formatted string 
   **/
        TLV_CRC32 (const std::string & xmlString);

  /**
    * @brief Copy constructor from type, length and raw data information
    **/
        TLV_CRC32 (TLV_TYPE_t type, TLV_LENGTH_t length,
                     const TLV_RAW_DATA_t * data);
 /**
   * @brief function returning a new instance of the same TLV Content
   **/
    TLV_BASE * 
    Copy() const;
 /**
   * @brief function returning the parent class of the TLV
   **/
 virtual const std::string 
 tlvIdName() const;

  /**
    * @brief Copy operator for TLV
    *
    **/
      virtual TLV_CRC32 & operator = (const TLV_CRC32 & other);


    };


  }                             // namespace icnx
}                               // namespace ns3

#endif
