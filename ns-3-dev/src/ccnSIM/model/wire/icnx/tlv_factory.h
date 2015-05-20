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
#ifndef _ICN_TLV_FACTORY_
#define _ICN_TLV_FACTORY_
//xml provided from sudo apt-get install zlib1g-dev zlib1g libpugixml-dev libpugixml1

#include <ostream>
#include <sstream>
#include <string>
#include <stdint.h>
#include <string.h>
#include "ccn-1.0_tlv-id.h"
#include "tlv_base.h"
#include "tlv_endiandata.h"
#include "tlv_rawdata.h"
#include "tlv_container.h"
#include "pugixml.h"
#include "ccn-1.0_interest_tlv.h"
#include "ccn-1.0_interestresponse_tlv.h"
#include "ccn-1.0_interestresp_returncode_tlv.h"
#include "ccn-1.0_content_tlv.h"
#include "ccn-1.0_app_tlv.h"
#include "ccn-1.0_intlife_tlv.h"
#include "ccn-1.0_interest_hop_tlv.h"
#include "ccn-1.0_mesg_tlv.h"
#include "ccn-1.0_namesegment_tlv.h"
#include "ccn-1.0_payload_tlv.h"
#include "ccn-1.0_name_tlv.h"
#include "ccn-1.0_expiry_tlv.h"
#include "ccn-1.0_cachetime_tlv.h"
#include "ccn-1.0_ipidm_tlv.h"
#include "ccn-1.0_payldtype_tlv.h"
#include "ccn-1.0_keyidrestr_tlv.h"
#include "ccn-1.0_flow_tlv.h"
#include "ccn-1.0_ipid_tlv.h"
#include "ccn-1.0_pad_tlv.h"
#include "ccn-1.0_objhashrestr_tlv.h"
#include "ccn-1.0_crc32_tlv.h"
#include "ccn-1.0_cert_tlv.h"
#include "ccn-1.0_crc32c_tlv.h"
#include "ccn-1.0_ec_secp256k1_tlv.h"
#include "ccn-1.0_ec_secp384r1_tlv.h"
#include "ccn-1.0_hmac_sha256_tlv.h"
#include "ccn-1.0_keyname_tlv.h"
#include "ccn-1.0_keyid_tlv.h"
#include "ccn-1.0_link_tlv.h"
#include "ccn-1.0_publickey_tlv.h"
#include "ccn-1.0_publickeylocation_tlv.h"
#include "ccn-1.0_rfc793_tlv.h"
#include "ccn-1.0_rsa_sha256_tlv.h"
#include "ccn-1.0_sigtime_tlv.h"
#include "ccn-1.0_validationalg_tlv.h"
#include "ccn-1.0_validationalgpayld_tlv.h"
#include "ccn-1.0_vmac128_tlv.h"

namespace wire {
namespace icnx {

//This class generates all TLV's
class TLV_BASE;

namespace tlvUtils
{
  /**
    * @brief Create Tlv from Data
    *
    * Decides if the data is an XML encoding or byte encoding,
    * the calls the correct routiens (CreateTlvFromXml or CreateTlvFromData)
    **/
  TLV_BASE *
  CreateTlvFromData (const TLV_RAW_DATA_t *rawData, bool guessIsXml=true);

    /**
    * @brief Create Tlv from XML formatted string 
    *
    * @param xml TLV XML formatted string
    *
    **/
  TLV_BASE *
  CreateTlvFromXml (const std::string &xml);

      /**
    * @brief Create Tlv from XML formatted string 
    *
    * @param istream containing xml TLV XML formatted string
    *
    **/
    TLV_BASE *
  CreateTlvFromXml (const std::istream &iSXml);

    std::string
  GetTlvName(wire::icnx::TLV_TYPE_t type);

      /**
    * @brief Create Tlv from specified type
    *
    * @param type specified TLV type
    *
    **/
  TLV_BASE *
  CreateTlvFromType(wire::icnx::TLV_TYPE_t type);

  TLV_TYPE_t
  GetNextRawDataType(wire::icnx::TLV_TYPE_t resetNum = 0);

  TLV_TYPE_t
  GetNextEndianDataType(wire::icnx::TLV_TYPE_t resetNum = 0);

  TLV_TYPE_t
  GetNextContainerType(wire::icnx::TLV_TYPE_t resetNum = 0);
};

} //icnx
} //wire
#endif 
