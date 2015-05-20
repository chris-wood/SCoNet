/*
 * UtilityFunctions.h
 *
 *  Created on: Oct 11, 2014
 *      Author: James Mathewson, UCSC
 */

#ifndef UTILITYFUNCTIONS_H_
#define UTILITYFUNCTIONS_H_
#include "ns3/pugixml.h"
#include "ns3/tlv_factory.h"
#include <string>
#include <ostream>
#include <stdint.h>
#include <string>
#include <iostream>
#include <iomanip>
#include <list>
#include <exception>
#include <vector>
#include <map>
#include "ns3/UtilityCachingHelper.h"

//this should go in a separate helper function
#include "ns3/ccn-1.0_content_tlv.h"
#include "ns3/ccn-1.0_tlv-id.h"

namespace ns3 {
namespace icnx {
namespace utility {


  //
  std::string
  GetTlvName( ::wire::icnx::TLV_CONTENT *data);



//stats per 'UTILITY BLOCK' (stats about the specific data type)



//typedef std::map < std::string, std::vector< UtilitySpecificData * > > UTIL_DATA;
class UtilityBlockData;

class UtilityFunctionBase 
{
protected:
	UtilityFunctionBase(); //not used
public:
	UtilityFunctionBase(::pugi::xml_node &xmlConfig);

	virtual ~UtilityFunctionBase();

  //type of logs:
  //initial configuration
  //per data object Value
  //per insert
  //per delete
  	virtual void
	Log(std::ostream &os) {};

    	virtual void
	Config(pugi::xml_node &xmlConfig);

  //used more for prediction, or passed hidden metadata.  Not defined at this level
  virtual void
	OnInterestIngress(Interest_t *interest); //Rx

  virtual void
	OnInterestResponseIngress(InterestResp_t *interest); //Rx   
  
  virtual void
	OnDataIngress(Content_t *data); //Rx

	virtual void
	OnDataEgress(Content_t *data);  //Tx

	virtual void
	OnDataDeletion(Content_t *data);

virtual void
OnDataDeletion(const std::string &dataName);

  virtual keyId_t
  Name() const;
  //virtual void
  //OnControlIngress(wire::icnx::TLV_CONTROL &control); //reserved

  //throw exceptions, should not be called at this level

  virtual void
  Print(std::ostream &os) const ;

  virtual void 
	Print(std::ostream &os, const keyId_t &tlvName, double &value) const;

	//call Compute before Value, in case need to adjust values in relation to itself
	virtual void
	Compute(const std::string &name="");

	virtual double
	Value(const Content_t *data) const ;

virtual double
Value(const keyId_t &name) const;

  virtual bool
  OnInit(UtilityBlockData *);


	virtual uint64_t
	EstMemoryUsed(void);

protected:
      keyId_t m_alias; //used IF user wants to alias a function
      keyId_t m_name; //name
      double m_defaultValue; //if a function does not apply, what is the default value?
      keyId_t m_nameOption;
      //UTIL_DATA *m_utilityData;
      bool m_usePrettyPrint;
      uint32_t m_instance;

};




//used for utility functions of utility, e.g. MIN(rnd, lru, lfu)
//this holds atomic utilities
class UtilityFunctionAggregationBase : public UtilityFunctionBase{
protected:
  UtilityFunctionAggregationBase();
public:
	UtilityFunctionAggregationBase(pugi::xml_node &xmlConfig);
	//UtilityFunctionAggregationBase();
	virtual ~UtilityFunctionAggregationBase();

	//call Compute before Value, in case need to adjust values in relation to itself
  //compute is for global calculations (all data in utility).   Here to simplify how often
  //we compute
  virtual void
	OnInterestIngress(Interest_t *interest); //Rx

	virtual void
	OnDataIngress(Content_t *data); //Rx
    	virtual void
	Config(pugi::xml_node &xmlConfig);
	virtual void
	OnDataEgress(Content_t *data);  //Tx

	virtual void
	OnDataDeletion(Content_t *data);
	virtual void
	OnDataDeletion(const keyId_t &name);

  virtual bool
  OnInit(UtilityBlockData *);

  void
	Compute(const std::string &name="");

	double
	//Value(const Content_t *data) const;
	Value(const std::string &name) const;

	uint64_t
	EstMemoryUsed(void);
  virtual void
  Print(std::ostream &os) const ;

  virtual void 
	Print(std::ostream &os, const std::string &tlvName, double &value) const;
  

protected:
    std::list<UtilityFunctionBase *> functionalFactors;
    virtual double 
    functionValue(double param1) const;
    virtual double 
    functionValue(double param1, double param2) const;
};


} /* namespace utility */
} /* namespace icnx */
} /* namespace ns3 */
#endif /* UTILITYFUNCTIONS_H_ */
