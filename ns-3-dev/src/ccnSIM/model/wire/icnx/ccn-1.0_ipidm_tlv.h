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

#ifndef _TLV_IPIDM_H_
#define _TLV_IPIDM_H_
/* CCN 1.0  Interest Payload ID Method HDR packet
                    1                   2                   3
0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
+---------------+---------------+---------------+---------------+
|           T_IPIDM             |             length (1)        |
+---------------+---------------+---------------+---------------+
/                  IPIDM Value (1 octets)                       /
+---------------+---------------+---------------+---------------+

Replacing the interestresponse return code in the fixed header!

*/
//#include "ccn-1.0_tlv-id.h"
//#include "tlv_base.h"
#include "tlv_endiandata.h"


namespace wire {
namespace icnx {


/**
 * @ingroup ccn
 * @brief CCN TLV 
 **/
class TLV_IPIDM:public TLV_ENDIANDATA
{
public:

  enum IPIDM_t //:uint8_t //for c++ 11 only
  {
      AppSpecific = 0,
      Nonce = 1,
      RFC6920 = 2,
  };

  /**
   * @brief Copy constructor
   */
  TLV_IPIDM ();
  ~TLV_IPIDM ();

   /**
   * @brief Copy constructor from reference TLV
   **/
  TLV_IPIDM (const TLV_IPIDM &tlv_msg);

   /**
   * @brief Copy constructor from serialized byte data
   **/
  TLV_IPIDM (const TLV_RAW_DATA_t *rawData);

            /**
   * @brief Copy constructor from XML formatted string 
   **/
  TLV_IPIDM (const std::string &xmlString);

  /**
    * @brief Copy constructor from Interest Payload ID Method type
    *
    **/
  TLV_IPIDM (IPIDM_t type);


  /**
   * \brief Get Interest Payload ID Method type
   * 
   */
  IPIDM_t
  GetIpidmCode () const;

 /**
   * @brief function returning a new instance of the same TLV Content
   **/
    TLV_BASE * 
    Copy() const;
  /**
   * \brief Set Interest Payload ID Method type
   * 
   */
  virtual void 
  SetIpidmCode (IPIDM_t type);

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
 TLV_IPIDM &
  operator = (const TLV_IPIDM &other); 

};

} // namespace icnx
} // namespace wire

#endif // _TLV_IPIDM_H_
