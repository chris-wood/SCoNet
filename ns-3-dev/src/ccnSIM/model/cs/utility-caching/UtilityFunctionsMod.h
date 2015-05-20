/*
 * UtilityFunctionsMod.h
 *
 * For utilities which modify a TLV
 *  Created on: Oct 11, 2014
 *      Author: James Mathewson, UCSC
 */

#ifndef UTILITYFUNCTIONSMOD_H_
#define UTILITYFUNCTIONSMOD_H_
#include "ns3/pugixml.h"
#include "ns3/tlv_factory.h"
#include <string>
#include <ostream>
#include <stdint.h>
#include <string>
#include <iostream>
#include <iomanip>
#include <list>
#include "UtilityFunctionsSingle.h"
//#include <timer.h>
#include "UtilityFunctionNames.h"
#include "ns3/simulator.h"
#include "ns3/nstime.h"
#include "ns3/UtilityCachingHelper.h"
#include "UtilityFunctionsAggregate.h"
#include <exception>
#include "UtilityFunctions.h"
#include "ns3/ccn-1.0_tlv-id.h"
#include <ns3/ccnSIM/model/wire/icnx/ccn-1.0_tlv-helper-base.h>

namespace ns3 {
namespace icnx {
namespace utility {

//hops from source
class UtilityHTL : public UtilityFunctionBase {
public:
	UtilityHTL(pugi::xml_node &xmlConfig);
	~UtilityHTL();
   	virtual void
	Config(pugi::xml_node &xmlConfig);

	virtual void
	OnDataIngress(Content_t *data); //Rx
  

	virtual keyId_t 
	Name() const;
  bool
  OnInit(UtilityBlockData *outsideData);

	virtual void
	OnDataDeletion(Content_t *data);
  
	virtual void
	OnDataDeletion(const keyId_t &name);

	virtual double
	Value(const keyId_t &name) const;

	virtual uint64_t
	EstMemoryUsed(void);


private:
   UtilityBlockData* m_parentBlock;
  class commonLocalScratchpad< keyId_t, uint64_t > m_scratchpad;
  ::wire::icnx::TLV_TYPE_t m_myId;
  uint64_t m_maxHops;

};


} /* namespace utility */
} /* namespace icnx */
} /* namespace ns3 */
#endif /* UTILITYFUNCTIONS_H_ */
