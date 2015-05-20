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
#ifndef CCN_CONTENT_STORAGE_H
#define	CCN_CONTENT_STORAGE_H

#include "ns3/ccn-common-object.h"
#include <ns3/ccnSIM/ccn.cxx/name.h>
#include "ns3/ccn-data.h"
#include <ns3/ccnSIM/model/wire/icnx/ccn-1.0_content_tlv.h>

namespace ns3 {
namespace ccn {

//ram based storage
// typedef  const ::std::string StorageFormat;
// typedef const ::ns3::ccn::Name StorageKey;
 typedef  ::std::string StorageFormat;
 typedef ::ns3::ccn::Name StorageKey;

 class CcnStorage  {
    public:

    virtual Ptr<Data> 
    GetContent(const StorageKey &contentName) const =0;

    virtual bool
    HasContent(const StorageKey &contentName) const =0;

    virtual void
    InsertContent( Ptr<Data> )=0;
    
    virtual void
    DeleteContent(const StorageKey &contentName)=0;


 };

 
 class CcnRamStorage : public CcnStorage {
    public:
    CcnRamStorage();
    virtual ~CcnRamStorage();

    virtual Ptr<Data>
    GetContent(const StorageKey &contentName) const;

    virtual bool
    HasContent(const StorageKey &contentName) const;

    virtual void
    InsertContent( Ptr<Data> data);
    
    virtual void
    DeleteContent(const StorageKey &contentName);


    private:
// typedef  const ::std::string StorageFormat;
// typedef const ::ns3::ccn::Name StorageKey;
    std::map < const StorageKey, StorageFormat > m_cacheStorageMap;
        

 };

}
}



#endif
