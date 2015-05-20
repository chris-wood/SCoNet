/*
 * UtilityFunctions.cc
 *
 *  Created on: Oct 11, 2014
 *      Author: James Mathewson, UCSC
 */

#include "UtilityFunctionsAggregate.h"

namespace ns3 {
namespace icnx {
namespace utility {
  //UtilityBlockData
  UtilityBlockData::UtilityBlockData ()
    : m_numberInterestRequests(0)
    , m_numberCacheHits(0)
    , m_numberEntries(0)
    , m_maxNumEntries(0)
    , m_numDeletedEntries(0)
    , m_accessReadTime(0)
    , m_accessWriteTime(0)
  {
    //need to put all xml files in common accessible directories TODO FIXME
     interestHelper = new Helper_t;
     interestHelper->Configure("InterestTlvRules.xml");
      interestResponseHelper = new Helper_t;
     interestResponseHelper->Configure("InterestResponseTlvRules.xml");
     dataHelper = new Helper_t;
     dataHelper->Configure("DataTlvRules.xml");
 
  }
  UtilityBlockData::~UtilityBlockData(){};
   uint64_t UtilityBlockData::GetNumInterestRequests() const {return m_numberInterestRequests;}
   uint64_t UtilityBlockData::GetNumCacheHits() const { return m_numberCacheHits;}
   uint64_t UtilityBlockData::GetNumEntries() const { return m_numberEntries; }
   uint64_t UtilityBlockData::GetMaxEntries() const { return m_maxNumEntries; }
   uint64_t UtilityBlockData::GetNumDeletedEntries() const { return m_numDeletedEntries;}
   uint64_t UtilityBlockData::GetCurrentTime() const { return (ns3::Simulator::Now()).GetInteger() ;}  //current system Time FIXME TODO
   uint64_t UtilityBlockData::GetPktTime(const keyId_t &name) const { return 0;} 
   
   //last packet arrival time FIXME TODO

   void UtilityBlockData::ModInterestBaseConfigure(const std::string &fileName)
   {
         interestHelper->Configure(fileName);
   }
   void UtilityBlockData::ModInterestResponseBaseConfigure(const std::string &fileName) { interestResponseHelper->Configure(fileName);

   }
   void UtilityBlockData::ModDataBaseConfigure(const std::string &fileName){
std::cout << "\n\n\n" << fileName << "\n\n"  ;
     dataHelper->Configure(fileName);

   }
   const Helper_t * UtilityBlockData::GetInterestHelper() { return interestHelper;}
   const Helper_t * UtilityBlockData::GetInterestResponseHelper() {return interestResponseHelper;}
   const Helper_t * UtilityBlockData::GetDataHelper() { return dataHelper;}



 
 //step function
 //<Utility name="STEP_FN" lowValue="0.0" highValue="1.0" threshold="0.5" />

  UtilityStepFn::UtilityStepFn(pugi::xml_node &xmlConfig) 
    : UtilityFunctionAggregationBase(xmlConfig)
    , m_threshold(0.5)
  {
     Config(xmlConfig);
  }
  UtilityStepFn::~UtilityStepFn() 
  {}

  void
	UtilityStepFn::Config(pugi::xml_node &xmlConfig) 
  {
     m_threshold = xmlConfig.attribute("threshold").as_double(m_threshold);
  }

	keyId_t 
	UtilityStepFn::Name() const 
  { return utilityStepName; }

  double
  UtilityStepFn::functionValue(double a1, double a2) const 
  {
    throw std::invalid_argument("UtilityStepFn is onlyl valid for a single parameter");
    return functionValue(a1); //not really defined ..., avoid compiler warning
  }
  double
  UtilityStepFn::functionValue(double a1) const 
  {
    if (a1 < m_threshold) {
       return m_valueLow;
    }
    return m_valueHigh;
  
  }

 //parent function
 //<Utility name="UTILITYBLOCK" />

  UtilitySum::UtilitySum(pugi::xml_node &xmlConfig) 
    :UtilityFunctionAggregationBase(xmlConfig)
  {
     
  }
	UtilitySum::~UtilitySum()
  {}

  keyId_t
  UtilitySum::Name() const { return utilitySumName; }
 
void
UtilitySum::Print(std::ostream &os) const
{
   if (!m_usePrettyPrint)
   {
      UtilityFunctionAggregationBase::Print(os);
      return;
   }
   int i = 1;
   std::list<UtilityFunctionBase *>::const_iterator it = functionalFactors.begin();

   int end = functionalFactors.size();
   os << " (";
   while (it != functionalFactors.end())
   {
      (*it)->Print(os);
      if (end != i)
      {
        os << " + ";
      }
      i++;
      it++;
   }
   os << " )";

}
 void
UtilitySum::Print(std::ostream &os, const keyId_t &tlvName, double &value) const
{
   if (!m_usePrettyPrint)
   {
      UtilityFunctionAggregationBase::Print(os);
      return;
   }
   int i = 1;
   std::list<UtilityFunctionBase *>::const_iterator it = functionalFactors.begin();

   int end = functionalFactors.size();
   os << " (";
   double childValue = 0.0;
   while (it != functionalFactors.end())
   {
      (*it)->Print(os, tlvName, childValue);
      value = functionValue(value, childValue);
      if (end != i)
      {
        os << " + ";
      }
      i++;
      it++;
   }
   os << " =[";
   os << value;
   os << " ])";


}

 double
 UtilitySum::functionValue(double a1, double a2) const
 {
    return a1+a2;
 }

 //Multiply
  UtilityMult::UtilityMult(pugi::xml_node &xmlConfig)
    :UtilityFunctionAggregationBase(xmlConfig)
  {
     
  }
	UtilityMult::~UtilityMult()
  {}

  keyId_t
  UtilityMult::Name(void) const { return utilityMultName; }
 
void
UtilityMult::Print(std::ostream &os) const
{
   if (!m_usePrettyPrint)
   {
      UtilityFunctionAggregationBase::Print(os);
      return;
   }
   int i = 1;
   std::list<UtilityFunctionBase *>::const_iterator it = functionalFactors.begin();

   int end = functionalFactors.size();
   os << " (";
   while (it != functionalFactors.end())
   {
      (*it)->Print(os);
      if (end != i)
      {
        os << " * ";
      }
      i++;
      it++;
   }
   os << " )";

}
 void
UtilityMult::Print(std::ostream &os, const keyId_t &tlvName, double &value) const
{
   if (!m_usePrettyPrint)
   {
      UtilityFunctionAggregationBase::Print(os);
      return;
   }
   int i = 1;
   std::list<UtilityFunctionBase *>::const_iterator it = functionalFactors.begin();

   int end = functionalFactors.size();
   os << " (";
   double childValue = 0.0;
   while (it != functionalFactors.end())
   {
      (*it)->Print(os, tlvName, childValue);
      value = functionValue(value, childValue);
      if (end != i)
      {
        os << " * ";
      }
      i++;
      it++;
   }
   os << " =[";
   os << value;
   os << " ])";


}
  

 double
 UtilityMult::functionValue(double a1, double a2) const
 {
//std::cout << a1 << " * " << a2 << std::endl;
    return a1*a2;
 }


 //Min
  UtilityMin::UtilityMin(pugi::xml_node &xmlConfig)
    :UtilityFunctionAggregationBase(xmlConfig)
  {
     
  }

	UtilityMin::~UtilityMin()
  {}


  keyId_t
  UtilityMin::Name(void) const { return utilityMinName; }

 double
 UtilityMin::functionValue(double a1, double a2) const 
 {
    if (a1 < a2) {
      return a1;     
    } else
    {
      return a2;
    }
 }


 //Max
  UtilityMax::UtilityMax(pugi::xml_node &xmlConfig)
    :UtilityFunctionAggregationBase(xmlConfig)
  {
     
  }
	UtilityMax::~UtilityMax()
  {}

   keyId_t
  UtilityMax::Name(void) const { return utilitySumName; }
 
 double
 UtilityMax::functionValue(double a1, double a2) const
 {
    if (a1 < a2) {
      return a2;     
    } else
    {
      return a1;
    }
 }






UtilityBlock::UtilityBlock(pugi::xml_node &xmlConfig) 
    :UtilityFunctionAggregationBase(xmlConfig)
  {
     Config(xmlConfig);

  }
  UtilityBlock::~UtilityBlock() 
  {}

  void
	UtilityBlock::Config(pugi::xml_node &xmlConfig) 
  {
   uint64_t maxSize = xmlConfig.attribute("maxsize").as_int(100);    
     m_blockStats.m_maxNumEntries = maxSize;
  
  }

  void
	UtilityBlock::OnInterestIngress(Interest_t *interest)
  {
     
     m_blockStats.m_numberInterestRequests++;
     UtilityFunctionAggregationBase::OnInterestIngress(interest);
  }

	void
	UtilityBlock::OnDataIngress(Content_t *data)
  {
    m_dataIdSet.insert(GetTlvName(const_cast<Content_t *>(data)));
     m_blockStats.m_numberEntries++;
    UtilityFunctionAggregationBase::OnDataIngress(data);
  }

	void
	UtilityBlock::OnDataEgress(Content_t *data)
  {

     m_blockStats.m_numberCacheHits++;
     UtilityFunctionAggregationBase::OnDataEgress(data);
  }

	void
	UtilityBlock::OnDataDeletion(Content_t *data)
  {
     m_blockStats.m_numDeletedEntries++;
     m_blockStats.m_numberEntries--;
     m_dataIdSet.erase((GetTlvName(const_cast<Content_t *>(data))));
     UtilityFunctionAggregationBase::OnDataDeletion(data);
  }
	void
	UtilityBlock::OnDataDeletion(const keyId_t &dataName)
  {

     m_blockStats.m_numDeletedEntries++;
     m_blockStats.m_numberEntries--;
     m_dataIdSet.erase(m_dataIdSet.find(dataName));
     UtilityFunctionAggregationBase::OnDataDeletion(dataName);
  }

  uint64_t
  UtilityBlock::GetMaxSize() 
  { return m_blockStats.m_maxNumEntries; }

	keyId_t 
	UtilityBlock::Name() const
  { return utilityBlockName; }

  double
  UtilityBlock::Value(const keyId_t &name) const
  //UtilityBlock::Value(const Content_t &data) const
  {
   std::list<UtilityFunctionBase *>::const_iterator it = functionalFactors.begin();
      double value = functionValue((*it)->Value(name));
      return value;
  }

  double
  UtilityBlock::functionValue(double a1, double a2) const 
  {
    throw std::invalid_argument("UtilityBlock is onlyl valid for a single parameter");
    return functionValue(a1); //not really defined ..., avoid compiler warning
  }
  double
  UtilityBlock::functionValue(double a1) const 
  {
    return a1;  
  }


  bool
  UtilityBlock::OnInit(UtilityBlockData *) {
   std::list<UtilityFunctionBase *>::const_iterator it = functionalFactors.begin();
      //return functionValue((*it)->OnInit(&m_blockStats));
      return (*it)->OnInit(&m_blockStats);
  
  }

  std::list < pairData_t >
  UtilityBlock::GetNamesByValue(double lowRange, double upperRange) 
  {
     std::list < pairData_t > nameValueList;
     std::set< keyId_t >::const_iterator it = m_dataIdSet.begin();
     this->Compute();
     double err = std::numeric_limits<double>::epsilon() * 2.0;
//std::cout << "Getnamesbyvalue(" << lowRange-err << "," << upperRange+err <<")" << std::endl;
     while (it != m_dataIdSet.end()) {
       double value = Value(*it);
       if ((value+err >= lowRange) && (value-err <= upperRange)) {
          nameValueList.push_back( make_pair(value, *it));
//std::cout << "name-value:" << *it << ":" << value << std::endl;
       }
       it++;
     }
     //FIXME TODO, we should use priority queue here, and have it ordered.
     return nameValueList;
  }

// lowestPrioData_t // std::priority_queue < pairData_t, std::vector<pairData_t>, std::greater<pairData_t>  >
  //UtilityBlock::GetNamesByLowestValue(uint32_t numLowEntries)  
  std::list < pairData_t >
   UtilityBlock::GetNamesByLowestValue(uint32_t numLowEntries)  //JLM FIXME dont really need numLowEntries
  {
     //std::priority_queue < pairData_t, std::vector<pairData_t>, std::greater<pairData_t> > lowestValueQueue;
     std::list < pairData_t > lowestValueQueue;
     std::set< keyId_t >::const_iterator it = m_dataIdSet.begin();
     this->Compute();
     while (it != m_dataIdSet.end()) {
       double value = Value(*it);
//std::cout << "eval: " << *it << ":" << value << std::endl;        
       lowestValueQueue.push_front(make_pair(value, *it));  
//       if (lowestValueQueue.size() > numLowEntries) {
//           lowestValueQueue.pop();    //remove higher priority 
//       }
       it++;
     }
     //copy to list, ordered
//     while (!lowestValueQueue.empty()) {
//        pairData_t top = lowestValueQueue.top();
//         returnList.push_back(top);
//         lowestValueQueue.pop();
//     }
      lowestValueQueue.sort();
     //std::list < pairData_t > returnList;
     // std::list< pairData_t >::iterator it = lowestValueQueue.begin();
      //first = lowestValueQueue.begin();

     // if ((it!=lowestValueQueue.end()) && (returnList.size() <= numLowEntries))
     // {
     //     returnList.push_front(it);
     // }
     //return returnList;
      return lowestValueQueue;
  }
  
  bool
    UtilityBlock::GetLowestNDO(keyId_t &name, double &value)  //JLM FIXME doesnt work correctly
  {
     std::set< keyId_t >::const_iterator it = m_dataIdSet.begin();
     if (it == m_dataIdSet.end()) { return false; }

     this->Compute();
     while (it != m_dataIdSet.end()) {
       double newValue = Value(*it);
       if (value > newValue) {
         name = *it;
         value = newValue;
       }
//std::cout << "eval: " << *it << ":" << value << std::endl;        
       it++;
     }
return true;
//std::cout << "eval: " << name << ":" << value << std::endl;        
  }


 
} /* namespace utility */
} /* namespace icnx */
} /* namespace ns3 */
