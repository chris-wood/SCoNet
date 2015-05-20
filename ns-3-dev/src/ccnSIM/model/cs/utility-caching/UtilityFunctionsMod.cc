/*
 * UtilityFunctions.cc
 *
 *  Created on: Oct 11, 2014
 *      Author: James Mathewson, UCSC
 */

#include "UtilityFunctionsMod.h"

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
//HTL
  	UtilityHTL::UtilityHTL(pugi::xml_node &xmlConfig) 
        : UtilityFunctionBase(xmlConfig)
    , m_parentBlock(NULL)
    , m_myId(0)
    , m_maxHops(99)
 {
    Config(xmlConfig);
 }
	UtilityHTL::~UtilityHTL() {}
   	 void
	UtilityHTL::Config(pugi::xml_node &xmlConfig) 
  {
     
  }
  bool
  UtilityHTL::OnInit(UtilityBlockData *outsideData)
  {
     m_parentBlock = outsideData;
     outsideData->ModDataBaseConfigure("DataHtlRules.xml");
      const Helper_t *dataHelper = m_parentBlock->GetDataHelper();
     m_myId = dataHelper->GetId("TLV_HTL");
     return true;
  }

	void
	UtilityHTL::OnDataDeletion(Content_t *data) {   m_scratchpad.EraseData(GetTlvName(data));
}
  
	void
	UtilityHTL::OnDataDeletion(const keyId_t &name) {  m_scratchpad.EraseData(name);
}

  void
	UtilityHTL::OnDataIngress(Content_t *data) 
  {
      //if HTL tlv doesnt exist, create it
      const Helper_t *dataHelper = m_parentBlock->GetDataHelper();
      uint64_t hops = 0;
      if (dataHelper->ExistTlv(data, "TLV_HTL")) {
        ::wire::icnx::TLV_ENDIANDATA * tlv =  dynamic_cast< ::wire::icnx::TLV_ENDIANDATA * >(((m_parentBlock->GetDataHelper())->GetTlv(data, "TLV_HTL")));
         tlv->GetNumericData(hops);      
          }
         ++hops;
         ::wire::icnx::TLV_ENDIANDATA  *tlv = new ::wire::icnx::TLV_ENDIANDATA(m_myId, hops);
        dataHelper->SetTlv(data, tlv);
        m_scratchpad.SetData(GetTlvName(data), hops);

  }  

	 keyId_t 
	UtilityHTL::Name() const { return utilityHTLName; }

	 double
	UtilityHTL::Value(const keyId_t &name) const {
      return (double) m_scratchpad.GetData(name)/m_maxHops;
  }

	 uint64_t
	UtilityHTL::EstMemoryUsed(void){ return m_scratchpad.EstMemoryUsed();}

} /* namespace utility */
} /* namespace icnx */
} /* namespace ns3 */
