/*
 * UtilityFunctions.h
 *
 *  Created on: Oct 11, 2014
 *      Author: James Mathewson, UCSC
 */

#ifndef UTILITYFUNCTIONGENERATOR_H_
#define UTILITYFUNCTIONGENERATOR_H_
#include "ns3/pugixml.h"
//#include "ns3/ccn-1.0_tlv-helper-functions.h"
#include "ns3/tlv_factory.h"
#include <string>
//#include "ns3/log.h"
//#include "ns3/traced-callback.h"
#include <ostream>
#include <stdint.h>
#include <string>
#include <iostream>
#include <iomanip>
#include <list>
#include "UtilityFunctions.h"
#include "UtilityFunctionsNomod.h"
#include "UtilityFunctionsMod.h"
#include "UtilityFunctionsAggregate.h"
#include "UtilityFunctionNames.h"

namespace ns3 {
namespace icnx {
namespace utility {


//generator
class UtilityGenerator
{
public:
    static UtilityFunctionBase *
    CreateNewUtility(pugi::xml_node &xmlConfig);
private:  //only 1 copy, this is a factory/generator (singleton)
    UtilityGenerator();
    ~UtilityGenerator();
};

} /* namespace utility */
} /* namespace icnx */
} /* namespace ns3 */
#endif /* UTILITYFUNCTIONSGENERATOR_H_ */
