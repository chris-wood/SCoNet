/*
 * UtilityFunctions.cc
 *
 *  Created on: Oct 11, 2014
 *      Author: James Mathewson, UCSC
 */

#include "UtilityFunctionGenerator.h"
#include "UtilityFunctions.h"

namespace ns3 {
namespace icnx {
namespace utility {

  /**
    xml format:
    <Utility name="utilityname" 'utilityparameters' />
    e.g.
    //name is constant for utility, instance is optional, helpful for debugging, in
    //case there is more than one type of utility.
    <Utility name="UtilityXYZ" weight="0.3" value="abcd" instance="3" />
    */
  UtilityGenerator::UtilityGenerator() {}
  UtilityGenerator::~UtilityGenerator() {}

UtilityFunctionBase *
UtilityGenerator::CreateNewUtility(pugi::xml_node &xmlConfig)
{
   //UtilityFunctionBase *newUtil = NULL;
   std::string name = xmlConfig.attribute("name").as_string(""); //value();

std::cout << "UtilityGenerator function: " << name << std::endl;
   //start with 
   if (name == utilitySumName ) {
      return new UtilitySum(xmlConfig);

   } else if (name == utilityMultName) {
      return  new UtilityMult(xmlConfig);

   } else if (name == utilityMinName) {
      return  new UtilityMin(xmlConfig);

   } else if (name == utilityMaxName) {
      return new UtilityMax(xmlConfig);

   } else if (name == utilityStepName) {
      return new UtilityStepFn(xmlConfig);

   } else if (name == utilityBlockName) {
      return new UtilityBlock(xmlConfig);

   }//no tlv modification
    else if (name == utilityHashName) {
      return new UtilityHash(xmlConfig);
    } else if (name == utilityLruName) {
      return new UtilityLru(xmlConfig);
   } else if (name == utilityInterNodeName) {
      return new UtilityInterNodeState(xmlConfig);
   } else if (name == utilityHTLName) {
      return new UtilityHTL(xmlConfig);
   } else if (name == utilityConstName) {
      return new UtilityConst(xmlConfig);
   } else if (name == utilityRndName) {
      return new UtilityRnd(xmlConfig);
   } else if (name == utilityQoSName) {
      return new UtilityNameQoS(xmlConfig);
   } else if (name == utilityFreshName) {
      return new UtilityFresh(xmlConfig);
   } 







   //tlv modification
   //not found, throw exception FIXME TODO JLM
std::cout << "unknown utility:" << name << std::endl;
   return NULL; //newUtil;




};

} /* namespace utility */
} /* namespace icnx */
} /* namespace ns3 */
