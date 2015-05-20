/*
 * UtilityFunctions.h
 *
 *  Created on: Oct 11, 2014
 *      Author: James Mathewson, UCSC
 */

#ifndef UTILITYFUNCTIONSNOMOD_H_
#define UTILITYFUNCTIONSNOMOD_H_
#include "ns3/pugixml.h"
#include "ns3/tlv_factory.h"
#include <string>
#include <ostream>
#include <stdint.h>
#include <string>
#include <iostream>
#include <iomanip>
#include <list>
#include <cstdlib>
#include "UtilityFunctionsSingle.h"
//#include <timer.h>
#include "UtilityFunctionNames.h"
#include "ns3/simulator.h"
#include "ns3/nstime.h"
#include "ns3/UtilityCachingHelper.h"
#include "UtilityFunctionsAggregate.h"
#include <exception>
#include <queue>

namespace ns3 {
namespace icnx {
namespace utility {


//LRU specific data
class LruData { //: public UtilitySpecificData {
  public:
    LruData();
    ~LruData();
    //time_t m_objectTimestamp;
    //ns3::Time m_objectTimestamp; //FIXME TODO should it be a union to save space?
    uint64_t m_objectTimestamp;
    uint32_t m_position;
};

class UtilityLru : public UtilityFunctionBase {
public:
	UtilityLru(pugi::xml_node &xmlConfig);
	~UtilityLru();
   //<Utility name="LRU"  instance="1" scaling="relative_age" | "fixed_increment" (default) />
  //both 'scaling' sets the oldest value to zero, and the newest to '1'.
  //but relative_age returns a value in ratio to that.  e.g. if the
  //oldest is 10 and the youngest is 20 timestamp, a value of 12
  //returns (12-10)/(20-10) = 0.2
  //
  //fixed increment returns an equal value, based upon how many are 
  //lower/higher than itself.
  //e.g. if 10 items are inserted, and 5 are below it, and 5 are
  //higher, it will have a value of 0.5, regardless of timestamp.

   	virtual void
	Config(pugi::xml_node &xmlConfig);

    virtual void
	OnInterestIngress(Interest_t *interest); //Rx

	virtual void
	OnDataIngress(Content_t *data); //Rx

	virtual void
	OnDataEgress(Content_t *data);  //Tx

	virtual void
	OnDataDeletion(Content_t *data);
  
	virtual void
	OnDataDeletion(const keyId_t &name);

  //virtual void
  //OnControlIngress(wire::icnx::TLV_CONTROL *control); //reserved

	virtual keyId_t 
	Name() const;

	//call Compute before Value, in case need to adjust values in relation to itself
	virtual void
		Compute(const keyId_t &name="");

  virtual bool
  OnInit(UtilityBlockData *);

	virtual double
	Value(const keyId_t &name) const;

	virtual uint64_t
	EstMemoryUsed(void);

protected:
  uint64_t m_oldestTime, m_newestTime;
  bool m_useFixedIncrement;
  bool m_timeChange;
  UtilityBlockData *m_parentBlock;
  //UtilityLru(); 
  class commonLocalScratchpad< keyId_t, LruData > m_scratchpad;

};


//hash of name
class UtilityHash : public UtilityFunctionBase {
public:
	UtilityHash(pugi::xml_node &xmlConfig);
	~UtilityHash();
   	virtual void
	Config(pugi::xml_node &xmlConfig);

	virtual keyId_t 
	Name() const;

	virtual double
	Value(const keyId_t &name) const;

	virtual uint64_t
	EstMemoryUsed(void);

protected:
  uint64_t
  makeHash(const char *hash, uint32_t hashLength) const;

private:
  //FIXME TODO should have a matching range
  uint64_t m_modValue;
  uint64_t m_modMatch;
};


//Random
class UtilityRnd : public UtilityFunctionBase {
public:
	UtilityRnd(pugi::xml_node &xmlConfig);
	~UtilityRnd();

  enum randType_t {
     invalid = 0,
     rndAtStart,
     rndAtCompute

  };
   	virtual void
	Config(pugi::xml_node &xmlConfig);

	virtual void
	OnDataIngress(Content_t *data); //Rx
  
	virtual void
	OnDataDeletion(Content_t *data);
  
	virtual void
	OnDataDeletion(const keyId_t &name);
	virtual void
		Compute(const keyId_t &name="");

	virtual keyId_t 
	Name() const;

	virtual double
	Value(const keyId_t &name) const;

	virtual uint64_t
	EstMemoryUsed(void);


private:
  int m_rndType;
    class commonLocalScratchpad< keyId_t, double > m_scratchpad;

};


//constant
class UtilityConst : public UtilityFunctionBase {
public:
	UtilityConst(pugi::xml_node &xmlConfig);
	~UtilityConst();
   	virtual void
	Config(pugi::xml_node &xmlConfig);

  virtual void
  Print(std::ostream &os) const;
  virtual void
  Print(std::ostream &os, const keyId_t &tlvName, double &value) const;
	virtual keyId_t 
	Name() const;

	virtual double
	Value(const keyId_t &name) const;

	virtual uint64_t
	EstMemoryUsed(void);


private:
  double m_constValue;
};


//name based qos
class UtilityNameQoS : public UtilityFunctionBase {
public:
	UtilityNameQoS(pugi::xml_node &xmlConfig);
	~UtilityNameQoS();
   	virtual void
	Config(pugi::xml_node &xmlConfig);

	virtual void
	OnDataIngress(Content_t *data); //Rx
  
	virtual void
	OnDataDeletion(Content_t *data);
  
	virtual void
	OnDataDeletion(const keyId_t &name);
	virtual void
		Compute(const keyId_t &name="");

	virtual keyId_t 
	Name() const;

	virtual double
	Value(const keyId_t &name) const;

	virtual uint64_t
	EstMemoryUsed(void);


private:
};
//TODO FIXME need intra-nodal communication
//under a common TLV name or allow customization?

//used by internodal communication, like SecTrustCache
class UtilityInterNodeState : public UtilityFunctionBase {
public:
	UtilityInterNodeState(pugi::xml_node &xmlConfig);
	~UtilityInterNodeState();
   	virtual void
	Config(pugi::xml_node &xmlConfig);

	virtual void
	OnDataIngress(Content_t *data); //Rx
  
	virtual void
	OnDataDeletion(Content_t *data);
  
	virtual void
	OnDataDeletion(const keyId_t &name);
	virtual void
		Compute(const keyId_t &name="");

	virtual keyId_t 
	Name() const;

	virtual double
	Value(const keyId_t &name) const;

	virtual uint64_t
	EstMemoryUsed(void);


private:
};

//cacheable status, Freshness, use T_EXPIRY
class UtilityFresh : public UtilityFunctionBase {
public:
	UtilityFresh(pugi::xml_node &xmlConfig);
	~UtilityFresh();
   	virtual void
	Config(pugi::xml_node &xmlConfig);

	virtual void
	OnDataIngress(Content_t *data); //Rx
  
	virtual void
	OnDataDeletion(Content_t *data);
  
	virtual void
	OnDataDeletion(const keyId_t &name);

  virtual bool
  OnInit(UtilityBlockData *);

	virtual keyId_t 
	Name() const;

	virtual double
	Value(const keyId_t &name) const;

	virtual uint64_t
	EstMemoryUsed(void);

  enum decayRate_t
  {
    flat,
    linear

  };

private:
    decayRate_t m_decayRate;
    class commonLocalScratchpad< keyId_t, uint64_t > m_scratchpad;
    UtilityBlockData *m_parentBlock;
};







} /* namespace utility */
} /* namespace icnx */
} /* namespace ns3 */
#endif /* UTILITYFUNCTIONS_H_ */
