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
 */

#include "ccn-content-store.h"
#include "ns3/log.h"
#include "ns3/packet.h"
#include "ns3/ccn-name.h"
#include "ns3/ccn-interest.h"
#include "ns3/ccn-data.h"
 #include "ns3/string.h"
#include <string>
#include <queue>

NS_LOG_COMPONENT_DEFINE ("ccn.cs.ContentStore");

namespace ns3 {
namespace ccn {


NS_OBJECT_ENSURE_REGISTERED (ContentStore);

NS_LOG_COMPONENT_DEFINE (ContentStore::GetLogName ().c_str ());

TypeId
ContentStore::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::ccn::cs::UtilityCaching")
    .SetGroupName ("Ccn")
    .SetParent<Object> ()
    .AddConstructor<ContentStore> ()
    .AddAttribute ("ConfigureUtilities",
                      "Set the XML utility configuration file.   If blank, No caching is default.",
                      StringValue ("NoCache.xml"),
                      MakeStringAccessor (&ContentStore::ConfigureCache),
                      MakeStringChecker()) 
    .AddTraceSource ("CacheHits", "Trace called every time there is a cache hit",
                     MakeTraceSourceAccessor (&ContentStore::m_cacheHitsTrace))

    .AddTraceSource ("CacheMisses", "Trace called every time there is a cache miss",
                     MakeTraceSourceAccessor (&ContentStore::m_cacheMissesTrace))
    
    ;

  return tid;
}


ContentStore::~ContentStore () 
{
}

std::string
ContentStore::GetLogName ()
{
  return "ccn.cs";
}
Ptr<Data>
ContentStore::Lookup (Ptr< Interest> interest)
{
if (0 == m_maxSize)  //no cache, ergo no hits.
    return NULL;

  if (!m_cacheManagement) {
    return 0;
  }

  NS_LOG_FUNCTION (interest->GetName());
  wire::icnx::TLV_INTEREST *tlvInterest;
  tlvInterest = interest->GetTlv();
  if (m_cacheManagement)
  {
    m_cacheManagement->OnInterestIngress(tlvInterest);
  }

    Ptr < Data > data = m_contentStorage->GetContent(interest->GetName());
  if (0 == data) //it == m_cacheStorageMap.end()) //not found
  {
      this->m_cacheMissesTrace (interest, ns3::Now());
  } else {

    //JLM FIXME TODO, check utilities if content is still fresh!
      
    std::string status = "Cache hit for NDO:";
    status.append(interest->GetStringName());
NS_LOG_FUNCTION(status); //std::cout << interest->GetName() << " found in cache! " << data->GetName() << std::endl;

      this->m_cacheHitsTrace (interest, data, ns3::Now());
   }
   return data;
  
}
  void
  ContentStore::Process (Ptr< InterestResponse > interestResponse) {
NS_LOG_FUNCTION (this);
  //TODO FIXME
assert(0);
  }

bool
ContentStore::Add (Ptr< Data> data, bool updateValue)
{
  if (!m_cacheManagement) {
    return false;
  }

  NS_LOG_FUNCTION (data->GetName());
//std::cout << "CS ADD [node " << this->GetObject<Node>()->GetId() << "] result: " << data->GetName() << " maxsize: " << m_maxSize << std::endl;  
  if (0 == m_maxSize) {
      return false;
  }

  
//std::cout << "CS ADD [node " << this->GetObject<Node>()->GetId() << "] result: " << data->GetName() << std::endl;  
  ::wire::icnx::TLV_CONTENT *tlvContent;
  tlvContent =  data->GetTlv();

      m_cacheManagement->OnDataIngress(tlvContent);


  //not stored, we may add it
  if ((! m_contentStorage->HasContent(data->GetName()) || updateValue))
  {
    //evaluate if we should place content in storage
    std::string dataName = data->GetStringName();
    //evaluate data object
    m_cacheManagement->Compute(dataName);
    double value = m_cacheManagement->Value(dataName);
//std::cout << "Add " << dataName << " with " << m_size+1 << " elements" << std::endl;
    //we wont cache it
    if (value <= m_purgeValue)
    {
      m_cacheManagement->OnDataDeletion(data->GetTlv()); //dataName);
      return false;
    }

    //have we exceeded our maximum size?  If yes, we need to purge
     m_size++;
     //physically save data object now
     m_contentStorage->InsertContent(data);

     //we could purge all obsolete content, and if size > maxsize, then purge the bottom x content
     //but to avoid many calculatiosn, we will skip it, and only compute/purge if size > maxsize
     if (m_size > m_maxSize) {
        //evaluate all data objects
        m_cacheManagement->Compute();
          std::string name; 
          bool hasNDO = m_cacheManagement->GetLowestNDO(name, value);
          if (hasNDO) {
            m_cacheManagement->OnDataDeletion(name);
            m_contentStorage->DeleteContent(ns3::ccn::Name(name)); 
//std::cout << "Deleting: " << name << " with a value of " << value << std::endl;
            m_size--;
          }
        } 
    return false;

  } 

  return true;
  
}

//need a print based upon dataobject FIXME TODO
void
ContentStore::Print (std::ostream &os) const
{
NS_LOG_FUNCTION (this);
     Print(os, "");
}

void
ContentStore::Print (std::ostream &os, const std::string &name) const
{
NS_LOG_FUNCTION (name);
  //for (std::list <ns3::icnx::utility::UtilityFunctionBase *>::const_iterator it = m_cacheManagement.begin(); it != m_cacheManagement.end(); it++)
   //{
     double val = 0.0;
Ptr<Node> node = this->GetObject<Node> ();
  //std::ostringstream strs;
  //std::string name = nname.toUri();
int32_t nodeNum;
  if (node) {
  nodeNum = node->GetId();
  } else {
    nodeNum = -1;
  }

os<< "N" << nodeNum << "{" << name << "}::";
if (name.empty()) {
//os << (*it)->Print() << "\n";
 m_cacheManagement->Print(os) ;
} else {
//os << (*it)->Print(name, val) << "\n";
 m_cacheManagement->Print(os, name, val);
}
     //os << strs;
  // }

}
void
ContentStore::Print (std::ostream &os, Ptr< Data> data) const
{  
NS_LOG_FUNCTION (data->GetName() );
  //Ptr< const ::wire::icnx::TLV_CONTENT> tlvPtr;
  wire::icnx::TLV_CONTENT *tlvContent;
  //tlvPtr = data->GetTlv();
  tlvContent = data->GetTlv();
  //tlvContent = const_cast<wire::icnx::TLV_CONTENT *>(PeekPointer(tlvPtr));
  std::string name = data->GetStringName();

   Print(os, name);
}

//UtilityCachingControl
//This checks if any condition has change sufficiently to run the 'control' portion, typically to purge/move content


uint32_t
ContentStore::GetSize () const
{
NS_LOG_FUNCTION (m_size );
  return m_size; //return actual size
}


/*
//This is for utility caching
<UtilityCaching>

  //define the utility branches for cache management
  <UtilityBlock name="ram cache" instance="1" name="Lru stuff" weight="1.0" maxsize="default">
     //utility heirarchy
     <Utility type="MULT">  //0.9*1.1*LRU()
         <Utility type="LRU" scaling="fixed_increment" instance="1" alias="fixedLru" computePeriod="100ms"/>
         <Utility type="Constant" instance="1" value="0.9" />
         <Utility type="Constant" instance="2" value="1.1" />
     </Utility>
  </UtilityBlock>
  <UtilityBlock name="Prefetch"  instance="2" weight="0.3" maxsize="10">
      <Utility type="Rnd" rndValue="Initial" computePeriod="compute" />
  </UtilityBlock>
  //define actions and watermarks on branches
  //action, which branch instance, entrymark (in entries) or watermark
  //watermark is utility value, threshold is based upon #cache entries
  //when the cache has 12 entries, purge the lowest 3 (until 9 entries are left)
  //purge means to delete the associated named data objects
  <Control action="Purge" instance="1" HighEntryThreshold="12" LowEntryThreshold="9"/> 

  //Delete means to delete the tracking info in the utility branch, but not delete the corresponding
  //named data objects.    LowWatermark means affect all objects with a value less than/equal to 0.1,
  //or the number of entries is 12+ (then, reduce to 11 entries). priority is order of execution
//JLM FIXME TODO update control actions
  <Control action="Delete" instance="2" priority="1" LowWatermark="0.1" /> //delete all entries with a value <= 0.1 
  <Control action="Delete" instance="2" priority="2" HighEntryThreshold="12"/> //delete all entries with more than 12+ entries

  //move an entrie to a higher/lower cache
  <Control action="Move" instance="2" destinationInstance="3" LowWatermark="0.1" HighWatermark="0.2"  HighEntryThreshold="12"/> //delete all entries with a value <= 0.1 
  //Both Copy and Move will send a copy of current utility values?  FIXME TODO JLM how to ensure state info is passed along with data.
  //Move will MOVE (make a copy in new branch, then delete from old branch), while copy will make a copy into the new branch.
  //the purpose of copy is to simulate a traditional cache vs persistent caching system.
  <Control action="Copy" instance="2" destinationInstance="4" HighWatermark="0.9"  HighEntryThreshold="12"/> //delete all entries with a value <= 0.1 

</UtilityCaching>
*/

//JLM FIXME TODO need to get cache size and time values
void
ContentStore::ConfigureCache(std::string xmlConfigFilename)
{
NS_LOG_FUNCTION (xmlConfigFilename);
  
  //open file
  pugi::xml_document doc;
  pugi::xml_parse_result result = doc.load_file(xmlConfigFilename.c_str());

  if (!result) {
//JLM FIXME TODO should be a warning log
//std::cout << "CS Load [node " << this->GetObject<Node>()->GetId() << "] result: " << result.description() << std::endl;  
     m_cacheManagement = NULL;
     return;  
  }
  //append to current list if new
  //need node number/name
  pugi::xml_node utilityCaching = doc.child("UtilityCaching") ;  //only want the utility caching part
  //$std::string m_maxSize = utilityCaching.attribute("maxsize").as_int(100); //default 100 entries if not specified
  m_contentStorage = new CcnRamStorage; //FIXME TODO should be generated by choice
  //pugi::xml_node utilityConfig = utilityCaching.child("UtilityBlock");
  pugi::xml_node utilityConfig = utilityCaching.child("Utility");

  //needed for multiple blocks
   //for (pugi::xml_node utilityConfig = utilityCaching.child("Utility"); utilityConfig; utilityConfig = utilityConfig.next_sibling("Utility"))
   //{
     // m_cacheManagement->push_back(ns3::icnx::utility::UtilityGenerator::CreateNewUtility(utilityConfig, blockNumber++));
   //}
      //only 1 block for now
      m_cacheManagement = dynamic_cast< ns3::icnx::utility::UtilityBlock * >(ns3::icnx::utility::UtilityGenerator::CreateNewUtility(utilityConfig));
      //FIXME TODO hack to manage cache, should be done by top layer of the utilities
      if (m_cacheManagement) {
        m_size = 0;
        //m_maxSize = utilityCaching.attribute("maxsize").as_int(100); //default 100 entries if not specified
        m_maxSize = m_cacheManagement->GetMaxSize(); //utilityCaching.attribute("maxsize").as_int(100); //default 100 entries if not specified
        m_cacheManagement->OnInit(NULL);
      } else { //no cache
        m_size = 0;
        m_maxSize = 0;
      }
//std::ostringstream os;
Print (std::cout); // << std::endl;    
  //modify existing one if existing

  //delete existing

  //copy relevant to new one 

}
//////////////////////////////////////////////////////////////////////



} // namespace ccn
} // namespace ns3
