/*
 * UtilityFunctions.cc
 *
 *  Created on: Oct 11, 2014
 *      Author: James Mathewson, UCSC
 */

#include "UtilityFunctionsNomod.h"

namespace ns3 {
namespace icnx {
namespace utility {
/*
    m_valueHigh = xmlConfig.attribute("highValue").as_double(m_valueHigh);

const char_t* xml_attribute::as_string(const char_t* def = "") const;

int xml_attribute::as_int(int def = 0) const;
unsigned int xml_attribute::as_uint(unsigned int def = 0) const;
double xml_attribute::as_double(double def = 0) const;
float xml_attribute::as_float(float def = 0) const;
bool xml_attribute::as_bool(bool def = false) const;
long long xml_attribute::as_llong(long long def = 0) const;
unsigned long long xml_attribute::as_ullong(unsigned long long def = 0) const;
   */
  //step function
 //<Utility name="LRU" scaling="relative_age" | "fixed_increment" (default) />
  //both 'scaling' sets the oldest value to zero, and the newest to '1'.
  //but relative_age returns a value in ratio to that.  e.g. if the
  //oldest is 10 and the youngest is 20 timestamp, a value of 12
  //returns (12-10)/(20-10) = 0.2
  //
  //fixed increment returns an equal value, based upon how many are 
  //lower/higher than itself.
  //e.g. if 11 items are inserted, and 5 are below it, and 5 are
  //higher, it will have a value of 0.5, regardless of timestamp.

  //NOTE: FIXME TODO, what if another Data/Interest packet comes in, which was prior received, but contains different information??  e.g. Different freshess (content metadata), or different intra-nodal communications (system metadata).
  LruData::LruData()
  {}
  LruData::~LruData() {}


  UtilityLru::UtilityLru(pugi::xml_node &xmlConfig)
    : UtilityFunctionBase(xmlConfig)
    , m_parentBlock(NULL)
  {
    m_oldestTime = (ns3::Now()).GetInteger();
    m_newestTime = (ns3::Now()).GetInteger();
    m_useFixedIncrement = true;
    m_timeChange = false;
     Config(xmlConfig);
  }
  UtilityLru::~UtilityLru()
  {

  }
   	 void
	UtilityLru::Config(pugi::xml_node &xmlConfig)
  {
    std::string incrementType = xmlConfig.attribute("scaling").as_string("");  //JLM FIXME TODO, put in default
    if (incrementType.compare("fixed_increment"))
    {
       m_useFixedIncrement = false;
       m_nameOption="fI";
    } else {  //default
       m_useFixedIncrement = true;
       m_nameOption="rI";
    }

    //std::cout << " configured to use fixed Increment: " << m_useFixedIncrement << std::endl;
    //GlobalConfig(xmlConfig); 


  }


     void
	UtilityLru::OnInterestIngress(Interest_t *interest)
  {
     //update stats, we dont do anything here.   We dont care about
    //misses, and hits are covered in OnDataEgress

  }
	 void
	UtilityLru::OnDataIngress(Content_t *data)
  {
    //ns3::Time timeStamp = ns3::Now();
    uint64_t timeStamp = (ns3::Now()).GetInteger();
    uint64_t numEntries = m_parentBlock->GetNumEntries()  ;
    if (1 == numEntries)
    {
        m_oldestTime = timeStamp;
        m_newestTime = m_oldestTime;
    }
    LruData lruData;
    keyId_t name = GetTlvName(data);
      lruData.m_objectTimestamp = timeStamp;
      m_scratchpad.SetData(name, lruData);
    m_timeChange = true;
  }

	 void
	UtilityLru::OnDataEgress(Content_t *data)
  {
     //hit
    //ns3::Time timeStamp = ns3::Now();
    uint64_t timeStamp = (ns3::Now()).GetInteger(); //FIXME TODO Time should be gotten from UtilityBlock
    LruData lruData; 
    keyId_t name = GetTlvName(data);
    lruData.m_objectTimestamp = timeStamp;
      m_scratchpad.SetData(name, lruData);
    m_timeChange = true;
     
  }
  bool
  UtilityLru::OnInit(UtilityBlockData *outsideData)
  {
     m_parentBlock = outsideData;
     return true;
  }
	 void
	UtilityLru::OnDataDeletion(Content_t *data)
  {
    //delete entry
    m_scratchpad.EraseData(GetTlvName(data));
    m_timeChange = true;
  }
	void
  UtilityLru::OnDataDeletion(const keyId_t &name)
  {
    //delete entry
    m_scratchpad.EraseData(name);
    m_timeChange = true;
  }


	 keyId_t 
	UtilityLru::Name() const { return utilityLruName;}

	//call Compute before Value, in case need to adjust values in relation to itself
	 void
	UtilityLru::Compute(const keyId_t &name)
  {
//std::cout << "Compute is LRU" << m_timeChange << std::endl;
    if (!m_timeChange) { //no change, no need to recalc
      return;
    }

    uint32_t position = 0;
    if (!m_useFixedIncrement) {
       //O(n)
       if (m_scratchpad.m_utilityData.empty()) { return ; } //no data
       std::map<keyId_t, LruData >::iterator it = m_scratchpad.m_utilityData.begin();
       LruData lruTime = (it->second);
       //ns3::Time theirTime = lruTime.m_objectTimestamp;
       uint64_t theirTime = lruTime.m_objectTimestamp;  //FIXME TODO change to time typedef
       m_oldestTime = theirTime;
       m_newestTime = theirTime;
       it++;
       for(; it != m_scratchpad.m_utilityData.end(); it++) {
            lruTime = (it->second);

            theirTime = lruTime.m_objectTimestamp;
            if ((theirTime < m_oldestTime)) 
            {
               m_oldestTime = theirTime;  
            }
            if ((m_newestTime < theirTime))
            {
               m_newestTime = theirTime;
            }
       }
    } else { //prioritize the info

       //sort data
     std::list< std::pair< uint64_t, keyId_t > > valueList;
     std::map<keyId_t, LruData >::iterator it = m_scratchpad.m_utilityData.begin();

     for(; it != m_scratchpad.m_utilityData.end(); it++) {
          valueList.push_back( make_pair( (it->second).m_objectTimestamp, it->first));
     }
    m_timeChange = false; //recalculated, reset change optimizer
    valueList.sort();
      std::list< std::pair< uint64_t, keyId_t > >::iterator it2 = valueList.begin();
     int i=0;
     for(; it2 != valueList.end(); it2++) {
         it = m_scratchpad.m_utilityData.find(it2->second);
        (it->second).m_position = i++;  
           }

    }
  }

	 double
	UtilityLru::Value(const keyId_t &name) const
{

      if (1 == m_scratchpad.m_utilityData.size()) { return 1.0; }
  LruData lruTime = m_scratchpad.GetData(name);
    if (!m_useFixedIncrement)
    {
      //ns3::Time time = lruTime.m_objectTimestamp;
      uint64_t time = lruTime.m_objectTimestamp;
//std::cout << name << " has a time value of (" << (time) << "-" << m_oldestTime << ") / (" << ((m_newestTime)) << "-" << m_oldestTime << ")" << std::endl;
       return ((double) (time-m_oldestTime ))/((m_newestTime- m_oldestTime));
    } 
    uint64_t numEntries = m_scratchpad.m_utilityData.size();
    //std::cout << "Lru has " << m_scratchpad.m_utilityData.size() << " and the system has " << numEntries << std::endl;
//std::cout << name << " has position value of " <<  ((double) (lruTime.m_position)/(numEntries-1)) << " : " << lruTime.m_position << "/" << numEntries-1 << std::endl;
      //double lruValue = lruTime->m_position+1.0;
      double lruValue = lruTime.m_position;
      return lruValue/(numEntries-1.0); //0-1 values
  }

	 uint64_t
	UtilityLru::EstMemoryUsed(void) {
    return m_scratchpad.EstMemoryUsed(); //numEntries*(sizeof(LruData)+sizeof(LruData *));
  }


  //hash

  UtilityHash::UtilityHash(pugi::xml_node &xmlConfig)  
    : UtilityFunctionBase(xmlConfig)
  {
     Config(xmlConfig);
  }

  UtilityHash::~UtilityHash() {}

  void
  UtilityHash::Config(pugi::xml_node &xmlConfig)
  {
    m_modValue = xmlConfig.attribute("modulus").as_uint(1);
    m_modMatch = xmlConfig.attribute("match").as_uint(1);

    m_nameOption = "%";
    std::ostringstream out;
    out << m_modValue;
    out << "=";
    out << m_modMatch;
    m_nameOption.append(out.str());

  }

  double
  UtilityHash::Value(const keyId_t &name) const 
  {
    //FIXME TODO, have it compare range, not only an exact match
      uint64_t value =  makeHash(name.c_str(), name.size()); 
//std::cout << value << " % " << m_modValue << " = ";
      value %=   m_modValue;
//std::cout << value << "\n";
      return (double) (value == m_modMatch);
      
  }

  uint64_t
  UtilityHash::EstMemoryUsed()
{
  return 0.0;
}

  keyId_t
  UtilityHash::Name() const 
{
return utilityHashName;
}

uint64_t
UtilityHash::makeHash(const char *hash, uint32_t hashLength) const 
{
   //simple hash
   uint64_t qHash=0;
   uint64_t oHash;
   uint32_t start=0;
   uint32_t length=hashLength;
   while (length > 8)
   {
      oHash = *( (uint64_t *) &hash[start]);
      qHash ^= oHash;
     start += 8;
     length -= 8;
   }
   //hash whatever bytes are left
   //std::cout << qHash << "_" << std::endl;
   for(uint32_t i=start; i<hashLength; i++)
   { 
     oHash = hash[i];// << i*8;
     //std::cout << oHash << " : " ;     
      qHash ^= oHash;

   }

  // std::cout << "== " << qHash << std::endl;
   return qHash;

}

//Random
	UtilityRnd::UtilityRnd(pugi::xml_node &xmlConfig) 
   : UtilityFunctionBase(xmlConfig)
   , m_rndType(rndAtStart)
  {
     Config(xmlConfig);
  }
	UtilityRnd::~UtilityRnd() { }
   	 void
	UtilityRnd::Config(pugi::xml_node &xmlConfig) 
{
  std::string val = xmlConfig.attribute("randomType").as_string("");
   if (!val.empty())
   { 
      if (!val.compare("atStart")) {
         m_rndType = rndAtStart;
         m_nameOption="S";
      } else if (!val.compare("atCompute")) {
         m_rndType = rndAtCompute;
         m_nameOption="C";
      } else { 
        throw std::invalid_argument("Invalid Rnd parameter");
      }
   }
   //types can be:
   //atCompute - only compute value when Computed.
   //atStart -get a value and keep it
}

	 void
	UtilityRnd::OnDataIngress(Content_t *data) {
       double rndVal = (double) (std::rand())/(RAND_MAX);
       keyId_t name = GetTlvName(data);
      m_scratchpad.SetData(name, rndVal);
  }  
	 void
	UtilityRnd::OnDataDeletion(Content_t *data) {
    m_scratchpad.EraseData(GetTlvName(data));
  
  }
  
	 void
	UtilityRnd::OnDataDeletion(const keyId_t &name) {   
    m_scratchpad.EraseData(name);
   }

	 void
		UtilityRnd::Compute(const keyId_t &name) 
{
    if (rndAtCompute == m_rndType) {
        if (name.empty()) {
             std::map<keyId_t, double>::iterator it = m_scratchpad.m_utilityData.begin();
             for (;it != m_scratchpad.m_utilityData.end();it++)
             {
                  double rndVal = (double) (std::rand())/(RAND_MAX);
                  m_scratchpad.SetData(it->first, rndVal);


             }
        } else {
            double rndVal =  (double) (std::rand())/(RAND_MAX);
            m_scratchpad.SetData(name, rndVal);

        }
    }
}

	 keyId_t 
	UtilityRnd::Name() const 
{
   return utilityRndName;
}

	 double
	UtilityRnd::Value(const keyId_t &name) const 
{
  return m_scratchpad.GetData(name);
}

	 uint64_t
	UtilityRnd::EstMemoryUsed(void){
  return m_scratchpad.EstMemoryUsed();

  }



//constant
  UtilityConst::UtilityConst(pugi::xml_node &xmlConfig) 
    : UtilityFunctionBase(xmlConfig)
    , m_constValue(1.0)
  { 
     Config(xmlConfig);
  }

  UtilityConst::~UtilityConst(){}
   	 void
	UtilityConst::Config(pugi::xml_node &xmlConfig)
{
  m_constValue = xmlConfig.attribute("cValue").as_double(m_constValue);

}

	 keyId_t 
	UtilityConst::Name() const
  {
     return utilityConstName;
  }

	 double
	UtilityConst::Value(const keyId_t &name) const{ return m_constValue;}

	 uint64_t
	UtilityConst::EstMemoryUsed(void){ return 0;}

   void
  	UtilityConst::Print(std::ostream &os) const { 
    double dummy;
    keyId_t name;
    Print(os, name, dummy);
    }
   void
  	UtilityConst::Print(std::ostream &os, const keyId_t &tlvName, double &value) const {
       if (m_usePrettyPrint)
       {
          os << " " << m_constValue;
          value = m_constValue;

       } else {

         UtilityFunctionBase::Print(os, tlvName, value); 
       }
    }


//qos
  UtilityNameQoS::UtilityNameQoS(pugi::xml_node &xmlConfig){}
  UtilityNameQoS::~UtilityNameQoS(){}
   	 void
	UtilityNameQoS::Config(pugi::xml_node &xmlConfig){}

	 void
	UtilityNameQoS::OnDataIngress(Content_t *data){}
  
	 void
	UtilityNameQoS::OnDataDeletion(Content_t *data){}
  
	 void
	UtilityNameQoS::OnDataDeletion(const keyId_t &name){}
	 void
		UtilityNameQoS::Compute(const keyId_t &name){}

	 keyId_t 
	UtilityNameQoS::Name() const {  }

	 double
	UtilityNameQoS::Value(const keyId_t &name) const{}

	 uint64_t
	UtilityNameQoS::EstMemoryUsed(void){}


//Freshness, use T_EXPIRY
  UtilityFresh::UtilityFresh(pugi::xml_node &xmlConfig)
    : UtilityFunctionBase(xmlConfig)
    , m_decayRate(flat)
    , m_parentBlock(NULL)
{
    Config(xmlConfig);
}
  bool
  UtilityFresh::OnInit(UtilityBlockData *outsideData)
  {
     m_parentBlock = outsideData;
     return true;
  }
  UtilityFresh::~UtilityFresh(){}
   	 void
	UtilityFresh::Config(pugi::xml_node &xmlConfig)
{   
     std::string decayType = xmlConfig.attribute("decay_rate").as_string("linear");
     if (decayType.compare("linear")) {
       m_decayRate = linear;
       m_nameOption ="L";
     } else {
       m_decayRate = flat;
       m_nameOption = "F";
     }
}

	 void
	UtilityFresh::OnDataIngress(Content_t *data)
{
  bool freshExists = (m_parentBlock->GetDataHelper())->ExistTlv(data, "TLV_EXPIRY");; //does T_EXPIRY exist?
    if (freshExists) {
      ::wire::icnx::TLV_EXPIRY * expireTlv =  dynamic_cast< ::wire::icnx::TLV_EXPIRY * >(((m_parentBlock->GetDataHelper())->GetTlv(data, "TLV_EXPIRY")));
       uint64_t timestamp = expireTlv->GetExpirationTime(); //what value does it hold?
       m_scratchpad.SetData(GetTlvName(data), timestamp);
    } else {
      uint64_t max = std::numeric_limits<uint64_t>::max();
       m_scratchpad.SetData(GetTlvName(data), max);

    }
}
  
	 void
	UtilityFresh::OnDataDeletion(Content_t *data){ 
    m_scratchpad.EraseData(GetTlvName(data));
}
  
	 void
	UtilityFresh::OnDataDeletion(const keyId_t &name)
{
    m_scratchpad.EraseData(name);

}

	 keyId_t 
	UtilityFresh::Name() const 
{
    return utilityFreshName;
}

	 double
	UtilityFresh::Value(const keyId_t &name) const
 {
   uint64_t time = m_scratchpad.GetData(name);
   uint64_t arrived = m_parentBlock->GetPktTime(name); 
   uint64_t now = m_parentBlock->GetCurrentTime() ;
   if (now > time) { return 0.0; }

      if (flat == m_decayRate) {
           return 1.0;        
      } else {
          return 1.0 - static_cast<double>((now-arrived)/(time-arrived)); 
      }
 }

	 uint64_t
	UtilityFresh::EstMemoryUsed(void){return m_scratchpad.EstMemoryUsed(); }


//security status
UtilityInterNodeState::UtilityInterNodeState(pugi::xml_node &xmlConfig)
{}
UtilityInterNodeState::~UtilityInterNodeState(){}
   	void
	UtilityInterNodeState::Config(pugi::xml_node &xmlConfig)
  {
  }

	 void
	UtilityInterNodeState::OnDataIngress(Content_t *data){} //Rx
  
	 void
	UtilityInterNodeState::OnDataDeletion(Content_t *data){}
  
	 void
	UtilityInterNodeState::OnDataDeletion(const keyId_t &name){}
	 void
	UtilityInterNodeState::	Compute(const keyId_t &name){}

	 keyId_t 
	UtilityInterNodeState::Name() const{}

	 double
	UtilityInterNodeState::Value(const keyId_t &name) const{}

	 uint64_t
	UtilityInterNodeState::EstMemoryUsed(void){}



} /* namespace utility */
} /* namespace icnx */
} /* namespace ns3 */
