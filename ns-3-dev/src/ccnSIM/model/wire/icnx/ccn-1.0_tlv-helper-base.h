/* -*- Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil -*- */
/*
 * Copyright (c) 2014,2015 University of California, Santa Cruz
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


#ifndef _CCN_TLV_BASE_HELPER_
#define _CCN_TLV_BASE_HELPER_
//xml provided from sudo apt-get install zlib1g-dev zlib1g libpugixml-dev libpugixml1

#include <ostream>
#include <sstream>
#include <string>
#include <stdint.h>
#include <string.h>
#include <iostream>
#include "tlv_base.h"
#include "tlv_container.h"
//#include "pugixml.h"
#include <fstream>      // std::ifstream
#include <map>
#include <list>
#include "ns3/tlv_factory.h"
#include "RangeDataTemplate.h"

namespace wire {
namespace icnx {

  typedef wire::icnx::TlvTypeIdEnum tlvTypeId;

namespace tlvUtils
{

 //should be in a seperate file
//template class RangeData<int>;
typedef RangeData<tlvTypeId> TlvRange ;
typedef RangeData<TLV_TYPE_t> TlvTypeRange ;

 class ConfigData
 {
   public:
    TlvRange parentId;
    TlvRange idRange;
    bool isOptional;
    bool isContainer;
    std::string name;
 };


 class TlvHelperBase //: public ns3::SimpleRefCount<wire::icnx::TLV_BASE>

 {
   public:
   TlvHelperBase();
   ~TlvHelperBase();
   void Configure(const std::string &xmlString); //new
   void SetTlv(TLV_BASE *baseTlv, TLV_BASE *insertTlv, bool deleteTlv = true) const;
   //bool ExistTlv(ns3::Ptr<const TLV_BASE> tlv, tlvTypeId tlvId);
   bool ExistTlv( TLV_BASE* tlv, tlvTypeId tlvId) const;
   bool ExistTlv( TLV_BASE* tlv, TlvRange &tlvId) const;
   bool ExistTlv( TLV_BASE *tlv, const std::string &tlvName) const;
   void DeleteTlv(TLV_BASE *tlv, tlvTypeId tlvId) const ;
   void DeleteTlv(TLV_BASE *tlv, TlvRange &tlvRange) const;
   void DeleteTlv(TLV_BASE *tlv, const std::string &tlvName) const;
   TLV_BASE *
   GetTlv( TLV_BASE *tlv, tlvTypeId tlvId) const; //mod
   TLV_BASE *
   GetFirstTlv(TLV_BASE *tlv, TlvRange &tlvId) const; //mod
   TLV_BASE *
   GetTlv(TLV_BASE *tlv, const std::string &tlvName) const; //mod

  void
  AppendTlv(TLV_BASE *baseTlv, TLV_BASE *appendTlv) const;

  void
  Clear(); //new, clear out info
 
  TLV_BASE*
  CreateValidTlv() const;

  TLV_LENGTH_t
  GetTlvSize(TLV_BASE *tlv, const std::string &tlvName) const;
   TLV_LENGTH_t
   GetTlvSize(TLV_BASE *tlv, tlvTypeId tlvId) const; //mod
   TLV_LENGTH_t
   GetFirstTlvSize(TLV_BASE *tlv, TlvRange &tlvId) const; //mod

     TLV_TYPE_t
  GetTlvType(TLV_BASE *tlv, const std::string &tlvName) const;
   TLV_TYPE_t
   GetTlvType(TLV_BASE *tlv, tlvTypeId tlvId) const; //mod
   TLV_TYPE_t
   GetFirstTlvType(TLV_BASE *tlv, TlvRange &tlvId) const; //mod

  const TLV_RAW_DATA_t *
  GetTlvData(TLV_BASE *tlv, const std::string &tlvName) const;
  const TLV_RAW_DATA_t *
   GetTlvData(TLV_BASE *tlv, tlvTypeId tlvId) const; //mod
   const  TLV_RAW_DATA_t *
GetFirstTlvData(TLV_BASE *tlv, TlvRange &tlvId) const; //mod

   ::wire::icnx::TLV_TYPE_t
  GetId(const std::string &tlvName) const;

   protected:
   TLV_TLVDATA *
   SetParentTlvs(TLV_TLVDATA *baseTlv, tlvTypeId tlvType, bool createTlv=true) const;
   pugi::xml_document doc;

   void
   RecursiveConfigure(pugi::xml_node &tlvFormat, const TlvRange &parentRange);
   
   void TrackTlvOrder();
   //more for a user
   std::map<tlvTypeId, ConfigData> configMap;
   std::map< std::string, TlvRange > nameToIdMap;
   //more for the system to add/delete/exist
   std::map<TlvRange, std::list< TlvRange > >tlvHeirarchyMap;
 };

 //static generators for Interest, Data, and InterestResponse
 const TlvHelperBase  * GetDataTlvHelper();

 const TlvHelperBase  * GetInterestTlvHelper();

 const TlvHelperBase  * GetInterestResponseTlvHelper();

//JLM FIXME TODO add CreateTlvByName(name)

}

} //icnx
} //wire
#endif //_CCN_TLV_BASE_HELPER_FUNCT_

