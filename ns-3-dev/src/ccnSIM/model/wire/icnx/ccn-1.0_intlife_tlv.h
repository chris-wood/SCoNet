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

#ifndef _CCN_LIFETIME_H_
#define _CCN_LIFETIME_H_
/* CCN 1.0 OPT TLV PER HOP HDR packet
                    1                   2                   3
0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
+---------------+---------------+---------------+---------------+
|          T_INT_LIFE           |             length            |
+---------------+---------------+---------------+---------------+
/                                                               /
/                   Lifetime (length octets)                    /
/                                                               /
+---------------+---------------+---------------+---------------+
http://www.icnx.org/pubs/icnx-mosko-tlvmessages-01.pdf

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
class TLV_INTLIFE:public TLV_ENDIANDATA
{
public:

  /**
   * @brief Copy constructor
   */
  TLV_INTLIFE ();
  ~TLV_INTLIFE ();

   /**
   * @brief Copy constructor from reference TLV
   **/
  TLV_INTLIFE (const TLV_INTLIFE &tlv_msg);

   /**
   * @brief Copy constructor from serialized byte data
   **/
  TLV_INTLIFE (const TLV_RAW_DATA_t *rawData);
 
            /**
   * @brief Copy constructor from XML formatted string 
   **/
  TLV_INTLIFE (const std::string &xmlString);

    /**
    * @brief Copy constructor from type, length and raw data information
    **/
        TLV_INTLIFE (TLV_TYPE_t type, TLV_LENGTH_t length,
                     const TLV_RAW_DATA_t * data);

  /**
    * @brief Copy constructor from a 64 bit milisecond time interal
    *
    **/
  TLV_INTLIFE (uint64_t time);


  /**
   * \brief Get TLV long time
   * Return time in milliseconds 
   * 
   */
  uint64_t
  GetTime () const;

 /**
   * @brief function returning a new instance of the same TLV Content
   **/
    TLV_BASE * 
    Copy() const;
  /**
   * \brief Set Tlv  Time
   * Set time of tlv
   * 
   */
  virtual void 
  SetTime (uint64_t time);

/**
  * @brief function returns a string name for the class
  **/
 virtual const std::string 
 tlvIdName() const;

  /**
    * @brief Copy operator for TLV
    *
    **/
  virtual TLV_INTLIFE &
  operator = (const TLV_INTLIFE &other); 

};

} // namespace icnx
} // namespace wire

#endif 
