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

#include "UtilityCachingHelper.h"


namespace ns3 {
namespace icnx {
namespace utility {

  //TODO JLM FIXME, for now, these go here.   Should go in a more common place,
  //to add utility caching specific cases.
  std::string
  GetTlvName( Content_t *data) {
      const Helper_t *m_tlvHelper =  ::wire::icnx::tlvUtils::GetInterestTlvHelper();
     if (m_tlvHelper->ExistTlv(data, wire::icnx::NAME_TLV)) {
         ::wire::icnx::TLV_NAME *nameTlv = dynamic_cast< wire::icnx::TLV_NAME * >(m_tlvHelper->GetTlv(data, "TLV_NAME"));
         return nameTlv->GetName();
      
     }
    throw std::invalid_argument("Interest Packet missing NAME!");

    } 

  std::string
  GetTlvName(  wire::icnx::TLV_INTEREST *interest) {
      const Helper_t *m_tlvHelper =  ::wire::icnx::tlvUtils::GetInterestTlvHelper(); 
           if (m_tlvHelper->ExistTlv(interest, "TLV_NAME")) {
         ::wire::icnx::TLV_NAME *nameTlv = dynamic_cast<wire::icnx::TLV_NAME *>(m_tlvHelper->GetTlv(interest, "TLV_NAME"));
         return nameTlv->GetName();
      
  }
    throw std::invalid_argument("Interest Packet missing NAME!");

    } 

  std::string
  GetTlvName(  wire::icnx::TLV_INTERESTRESPONSE *interestResponse) {
      const Helper_t *m_tlvHelper =  ::wire::icnx::tlvUtils::GetInterestResponseTlvHelper(); 
           if (m_tlvHelper->ExistTlv(interestResponse, "TLV_NAME")) {
         ::wire::icnx::TLV_NAME *nameTlv = dynamic_cast<wire::icnx::TLV_NAME *>(m_tlvHelper->GetTlv(interestResponse, "TLV_NAME"));
         return nameTlv->GetName();
      
  }
    throw std::invalid_argument("Interest Packet missing NAME!");


  }

}
}
}
