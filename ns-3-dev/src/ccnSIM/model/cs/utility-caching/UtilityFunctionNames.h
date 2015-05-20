#include <string>
#ifndef UTILITYFUNCTIONSNAMES_H_
#define UTILITYFUNCTIONSNAMES_H_

namespace ns3 {
namespace icnx {
namespace utility {

//aggregate utilities
static const std::string utilityBlockName="UTILITYBLOCK";
static const std::string utilitySumName="SUM";
static const std::string utilityMultName="MULT";
static const std::string utilityMinName="MIN";
static const std::string utilityMaxName="MAX";

//pretty or more human readable versions for aggregate functions
static const std::string utilityPrettySumName=" + ";
static const std::string utilityPrettyMultName="*";

//StepFunction(0 or 1)
static const std::string utilityStepName="STEP_FN";

//these utilities do not modify the TLV
static const std::string utilityLruName="LRU";
static const std::string utilityHashName="HASH";
static const std::string utilityRndName="RND";
static const std::string utilityConstName="CONSTANT";
static const std::string utilityHTLName="HTL"; //
static const std::string utilityInterNodeName="InterNode";
static const std::string utilityQoSName="QoS";
static const std::string utilityFreshName="Fresh"; 

//these utilities do modify the TLV
//Hop distance to source/producer

//Hop distance to destination/consumer

}
}
}
#endif
