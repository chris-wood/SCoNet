/*
 * UtilityFunctions.h
 *
 *  Created on: Oct 11, 2014
 *      Author: James Mathewson, UCSC
 */

#ifndef UTILITYFUNCTIONSAGGREGATE_H_
#define UTILITYFUNCTIONSAGGREGATE_H_
#include "ns3/pugixml.h"
//#include "ns3/ccn-1.0_tlv-helper-functions.h"
#include "ns3/tlv_factory.h"
#include <string>
#include <ostream>
#include <stdint.h>
#include <string>
#include <iostream>
#include <iomanip>
#include <list>
#include <set>
#include <queue>
#include "UtilityFunctions.h"
#include "UtilityFunctionNames.h"
#include "UtilityCachingHelper.h"
#include "ns3/simulator.h"
namespace ns3 {
namespace icnx {
namespace utility {

class UtilityBlock;
class UtilityBlockData 
{
  friend UtilityBlock;
  public:
  UtilityBlockData();
  virtual ~UtilityBlockData();
   uint64_t GetNumInterestRequests() const;
   uint64_t GetNumCacheHits() const;

   uint64_t GetNumEntries() const;
   uint64_t GetMaxEntries() const;
   uint64_t GetNumDeletedEntries() const;
   uint64_t GetCurrentTime() const;
   uint64_t GetPktTime(const keyId_t &name) const;
   void ModInterestBaseConfigure(const std::string &fileName);
   void ModInterestResponseBaseConfigure(const std::string &fileName);
   void ModDataBaseConfigure(const std::string &fileName);
   const Helper_t * GetInterestHelper();
   const Helper_t * GetInterestResponseHelper();
   const Helper_t * GetDataHelper();
  private:
   uint64_t m_numberInterestRequests;
   uint64_t m_numberCacheHits;

   uint64_t m_numberEntries;
   uint64_t m_maxNumEntries;

   uint64_t m_numDeletedEntries;

   double m_accessReadTime;
   double m_accessWriteTime;
   Helper_t *interestHelper, *dataHelper, *interestResponseHelper;

};

//class UtilityBlock : public UtilityFunctionBase

typedef std::pair < double, keyId_t > pairData_t; 
//typedef std::priority_queue < pairData_t, std::vector< pairData_t >, std::greater< pairData_t >  > lowestPrioData_t;
typedef std::priority_queue < pairData_t  > lowestPrioData_t;

class UtilityBlock : public UtilityFunctionAggregationBase
{

public:
	UtilityBlock(pugi::xml_node &xmlConfig);

	virtual ~UtilityBlock();

  keyId_t
  Name(void) const;
   	virtual void
	Config(pugi::xml_node &xmlConfig);
    //need ingress/egress over-rides
  virtual void
	OnInterestIngress(Interest_t *interest); //Rx

	virtual void
	OnDataIngress(Content_t *data); //Rx

	virtual void
	OnDataEgress(Content_t *data);  //Tx

  uint64_t
  GetMaxSize() ;

	virtual void
	OnDataDeletion(Content_t *data);
virtual void
OnDataDeletion(const keyId_t &dataName);

  virtual double
  Value(const keyId_t &name) const;
  virtual bool
  OnInit(UtilityBlockData *);
  std::list < pairData_t >
  GetNamesByValue(double lowRange, double upperRange) ;

  //std::priority_queue < pairData_t, std::vector<pairData_t>, std::greater<pairData_t>  >
    //GetNamesByLowestValue(uint32_t numLowEntries) ;
  std::list < pairData_t >

GetNamesByLowestValue(uint32_t numLowEntries) ;
bool
GetLowestNDO(keyId_t &name, double &value);
protected:
  virtual double
  functionValue(double a1, double a2) const;
  virtual double
  functionValue(double a1) const;
  class UtilityBlockData m_blockStats;
  std::set < keyId_t > m_dataIdSet;
};


class UtilityMax : public UtilityFunctionAggregationBase
{

public:
	UtilityMax(pugi::xml_node &xmlConfig);

	virtual ~UtilityMax();

  keyId_t
  Name(void) const;
protected:
  virtual double
  functionValue(double a1, double a2) const;

};

class UtilityMin : public UtilityFunctionAggregationBase
{

public:
	UtilityMin(pugi::xml_node &xmlConfig);

	virtual ~UtilityMin();
  keyId_t
  Name(void) const;
protected:
  virtual double
  functionValue(double a1, double a2) const;

};

class UtilitySum : public UtilityFunctionAggregationBase
{

public:
	UtilitySum(pugi::xml_node &xmlConfig);

	virtual ~UtilitySum();

  keyId_t
  Name(void) const;
  void
  Print(std::ostream &os, const keyId_t &tlvName, double &value) const;
  void Print(std::ostream &os) const;

protected:
  virtual double
  functionValue(double a1, double a2) const;


};

class UtilityMult : public UtilityFunctionAggregationBase
{

public:
	UtilityMult(pugi::xml_node &xmlConfig);

	virtual ~UtilityMult();
  virtual keyId_t
  Name(void) const;
  void
  Print(std::ostream &os, const keyId_t &tlvName, double &value) const;
  void Print(std::ostream &os) const;

protected:
  virtual double
  functionValue(double a1, double a2) const;

};

class UtilityStepFn : public UtilityFunctionAggregationBase {
public:
	UtilityStepFn(pugi::xml_node &xmlConfig);
	~UtilityStepFn();
   	virtual void
	Config(pugi::xml_node &xmlConfig);
	virtual keyId_t 
	Name() const;

protected:
  virtual double
  functionValue(double a1, double a2) const;
  virtual double
  functionValue(double a1) const;
  double m_threshold;
  double m_valueLow;
  double m_valueHigh;

};


} /* namespace utility */
} /* namespace icnx */
} /* namespace ns3 */
#endif /* UTILITYFUNCTIONS_H_ */
