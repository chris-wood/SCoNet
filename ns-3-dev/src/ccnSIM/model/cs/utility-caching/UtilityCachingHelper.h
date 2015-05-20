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
#ifndef UTILITYFUNCTIONCACHEHELPER_H_
#define UTILITYFUNCTIONCACHEHELPER_H_

#include <string>

#include "ns3/ccn-1.0_tlv-id.h"
#include <ns3/ccnSIM/model/wire/icnx/ccn-1.0_tlv-helper-base.h>

namespace ns3 {
namespace icnx {

  class TLV_INTEREST;
  class TLV_INTERESTRESPONSE;
  class TLV_CONTENT;

namespace utility {

    typedef ::std::string keyId_t;
  typedef ::wire::icnx::TLV_CONTENT Content_t;
  typedef ::wire::icnx::TLV_INTEREST Interest_t;
  typedef ::wire::icnx::TLV_INTERESTRESPONSE InterestResp_t;
  typedef ::wire::icnx::tlvUtils::TlvHelperBase Helper_t;

   template<class key, class UtilitySpecificData>
   class commonLocalScratchpad {  
     public:  //make this public, as its templated per function,
       //and the data should be open to each function
  ::std::map< key, UtilitySpecificData > m_utilityData;
     public:
   UtilitySpecificData    
   GetData(const key &keyName) const {
         typename std::map < key, UtilitySpecificData >::const_iterator it;
         it = m_utilityData.find(keyName);
         if (it == m_utilityData.end()) {  //name doesnt exist
           std::string msg = keyName; //really need to make this more generic
           msg.append(": data not found");
           throw std::out_of_range(msg);
         }
         return (it->second);  //avoid creating entry if not found, add NULL check
     
   }
   bool ExistData(const key &keyName) {
         typename std::map < key, UtilitySpecificData >::const_iterator it;
         it = m_utilityData.find(keyName);
         if (it == m_utilityData.end()) {  //name doesnt exist
            return false;
         }
         return true;
   }
   void SetData(const key &keyName, UtilitySpecificData &updatedData) {
    m_utilityData[keyName] = updatedData;
 
   }
   void EraseData(const key &keyName) {
      m_utilityData.erase(keyName); 
   }
   uint64_t size() {
        return m_utilityData.size();
   }

   uint64_t EstMemoryUsed() {
     uint64_t memUsed = 0;
         typename std::map < key, UtilitySpecificData >::const_iterator it;
         for(it = m_utilityData.begin(); it!=m_utilityData.end(); it++) {
            memUsed+=(it->first).size();
            memUsed+=sizeof(it->second);
         }
         return memUsed;
   }
   };
  
  keyId_t
  GetTlvName(  Interest_t *interest);

  keyId_t
  GetTlvName(  InterestResp_t *interest);
  keyId_t
  GetTlvName(  Content_t *data);

  }
}
}
#endif
