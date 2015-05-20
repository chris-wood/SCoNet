/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2011,2012 University of California, Los Angeles
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
 */
#include "ccn-content-storage.h"

namespace ns3 {
namespace ccn {

CcnRamStorage::CcnRamStorage()
{

}
CcnRamStorage::~CcnRamStorage()
{

}

     Ptr<Data> 
    CcnRamStorage::GetContent(const StorageKey &contentName) const 
{
    std::map < const StorageKey, StorageFormat >::const_iterator mapIt;
     mapIt = m_cacheStorageMap.find(contentName);
     if (mapIt == m_cacheStorageMap.end()) {
       return 0; 
     } else {
       Ptr<Data> data = Create<Data>();
       ::wire::icnx::TLV_CONTENT *tlv = new ::wire::icnx::TLV_CONTENT(mapIt->second);
       data->SetTlv(tlv);
       return data;
     }
}

     bool
    CcnRamStorage::HasContent(const StorageKey &contentName) const 
{
    std::map < const StorageKey, StorageFormat >::const_iterator mapIt;
     mapIt = m_cacheStorageMap.find(contentName);
     if (mapIt == m_cacheStorageMap.end()) {
        return false;
     } 
     return true;
}

     void
    CcnRamStorage::InsertContent( Ptr<Data> data)
{
    m_cacheStorageMap[data->GetName()] = (data->GetTlv())->XmlPrint();
}
    
     void
    CcnRamStorage::DeleteContent(const StorageKey &contentName)
{ 
     m_cacheStorageMap.erase(contentName);
}
  

}
}
