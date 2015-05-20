/*
 * UtilityFunctions.h
 *
 *  Created on: Oct 11, 2014
 *      Author: James Mathewson, UCSC
 */

#include "UtilityFunctions.h"
#include "UtilityFunctionGenerator.h"

namespace ns3 {
namespace icnx {
namespace utility {


//define the core parent functions-utilities
  //base class default values
  

   
//Aggregation
UtilityFunctionBase::UtilityFunctionBase(::pugi::xml_node &xmlConfig)
  : m_usePrettyPrint(true)
{ 

}

UtilityFunctionBase::UtilityFunctionBase() { }

UtilityFunctionBase::~UtilityFunctionBase() {}

std::string
UtilityFunctionBase::Name() const 
{ throw ::std::invalid_argument("Base has no name"); }


void
UtilityFunctionBase::OnInterestIngress(Interest_t  *interest)
{ //throw ::std::invalid_argument("UtilityFunctioBase::OnInterestIngress should never be called");
}

void
UtilityFunctionBase::OnDataIngress(Content_t  *data)
{
  //throw ::std::invalid_argument("UtilityFunctioBase::OnDataIngress should never be called");
} //Rx

void
UtilityFunctionBase::OnDataEgress(Content_t *data)
{
  //throw ::std::invalid_argument("UtilityFunctioBase::OnDataEgress should never be called");
}

//FIXME TODO
void
UtilityFunctionBase::OnDataDeletion(const keyId_t &dataName)
{
  //throw ::std::invalid_argument("UtilityFunctioBase::OnDataDeletion should never be called");

}

void
UtilityFunctionBase::OnDataDeletion(Content_t *data)
{
  //throw ::std::invalid_argument("UtilityFunctioBase::OnDataDeletion should never be called");
  OnDataDeletion(GetTlvName(const_cast<Content_t *>(data)));

}
  void
	UtilityFunctionBase::OnInterestResponseIngress(InterestResp_t *interest) {}

bool
UtilityFunctionBase::OnInit(UtilityBlockData *blockDataPtr)
{
   return true;
}
void
UtilityFunctionBase::Print(std::ostream &os) const
{
        std::ios::fmtflags saveFlags = os.flags();

  if (m_alias.empty()) {
  os << Name();
  } else {
    os << m_alias;
  }

  if (m_instance) {
    os << m_instance;
  }
  if (!m_nameOption.empty()) {
    os << "_";
    os << m_nameOption;
  }
  os << "()";
          os.flags(saveFlags);
}
void
UtilityFunctionBase::Print(std::ostream &os, const keyId_t &tlvName, double &value) const
{
  if (m_alias.empty()) {
    os << m_name;
  } else {
    os << m_alias;
  }
  if (m_instance) {
    os << m_instance;
  }
   if (!m_nameOption.empty()) {
     os << "_"; //output.append("_");
     os << m_nameOption; //output.append(m_nameOption);
   }
  os << "(=";
  os << Value(tlvName);
  os << ")";
  
}

//FIXME TODO change string to keyid_t
  void
UtilityFunctionBase::Compute(const std::string &name)
{
}

double
UtilityFunctionBase::Value(const std::string &name) const
{
  throw std::invalid_argument("UtilityFunctioBase::Value should never be called");
  return -1.0;
}


double
UtilityFunctionBase::Value(const Content_t *data) const 
{
  //JLM FIXME TODO hate doing this, but not enough time to redo the helper
  std::string name = GetTlvName(const_cast<Content_t *>(data));
      return Value(name);
}
uint64_t
UtilityFunctionBase::EstMemoryUsed(void)
{
  throw std::invalid_argument("UtilityFunctioBase::EstMemoryUsed should never be called");
  return 0;
}

//<Utility name="" instance="1" />
void
UtilityFunctionBase::Config(pugi::xml_node &xmlConfig)
{
  //get default instances from config file
  m_instance = xmlConfig.attribute("instance").as_int(0);
  m_alias = xmlConfig.attribute("alias").as_string("");
    m_name = xmlConfig.attribute("name").as_string("BADUTIL");
  //if an algorithm doesnt apply to data, what value shall we give it?
  m_defaultValue = xmlConfig.attribute("defaultValue").as_double(0.0);
}

//Aggregation
UtilityFunctionAggregationBase::UtilityFunctionAggregationBase(pugi::xml_node &xmlConfig)
  :UtilityFunctionBase::UtilityFunctionBase(xmlConfig)
{
  Config(xmlConfig);
}

void
UtilityFunctionAggregationBase::Config(pugi::xml_node &xmlConfig)
{
   for (pugi::xml_node utilityConfig = xmlConfig.child("Utility"); utilityConfig; utilityConfig = utilityConfig.next_sibling("Utility"))
   {
      functionalFactors.push_back(UtilityGenerator::CreateNewUtility(utilityConfig));
   }

}
UtilityFunctionAggregationBase::UtilityFunctionAggregationBase() { }

UtilityFunctionAggregationBase::~UtilityFunctionAggregationBase()
{
   functionalFactors.clear();
}


void
UtilityFunctionAggregationBase::OnInterestIngress(Interest_t *interest)
{
   std::list<UtilityFunctionBase *>::iterator it = functionalFactors.begin();
   while (it != functionalFactors.end())
   {
      (*it)->OnInterestIngress(interest);
      it++;
   }

}

void
UtilityFunctionAggregationBase::OnDataIngress(Content_t  *data)
{
   std::list<UtilityFunctionBase *>::iterator it = functionalFactors.begin();
   while (it != functionalFactors.end())
   {
      (*it)->OnDataIngress(data);
      it++;
   }

} //Rx

void
UtilityFunctionAggregationBase::OnDataEgress(Content_t *data)
{
   std::list<UtilityFunctionBase *>::iterator it = functionalFactors.begin();
   while (it != functionalFactors.end())
   {
      (*it)->OnDataEgress(data);
      it++;
   }

}

void
UtilityFunctionAggregationBase::OnDataDeletion(Content_t *data)
{
   std::list<UtilityFunctionBase *>::iterator it = functionalFactors.begin();
   while (it != functionalFactors.end())
   {
      (*it)->OnDataDeletion(data);
      it++;
   }

}
void
UtilityFunctionAggregationBase::OnDataDeletion(const keyId_t &name) 
{
   std::list<UtilityFunctionBase *>::iterator it = functionalFactors.begin();
   while (it != functionalFactors.end())
   {
      (*it)->OnDataDeletion(name);
      it++;
   }

}
bool
UtilityFunctionAggregationBase::OnInit(UtilityBlockData *blockDataPtr) 
{
   std::list<UtilityFunctionBase *>::iterator it = functionalFactors.begin();
   bool success = true;
   while (it != functionalFactors.end() && success)
   {
      success =(*it)->OnInit(blockDataPtr);
      it++;
   }
   return success;
}


double
//UtilityFunctionAggregationBase::Value(const Content_t *data) const
UtilityFunctionAggregationBase::Value(const keyId_t &name) const
{
   std::list<UtilityFunctionBase *>::const_iterator it = functionalFactors.begin();
   double value = 1.0;
   double secValue;
     while (it != functionalFactors.end())
       {
         secValue = (*it)->Value(name);
//std::cout << Name() << " = " << (*it)->Name() << "(" << secValue << ") * "; 

         value = functionValue(secValue, value);
//std::cout << "returns " <<  value  << std::endl; 
         it++;
       }
     return value;


}

uint64_t
UtilityFunctionAggregationBase::EstMemoryUsed(void)
{
  uint64_t sum = 0;
   std::list<UtilityFunctionBase *>::iterator it = functionalFactors.begin();
   while (it != functionalFactors.end())
   {
      sum += (*it)->EstMemoryUsed();
      it++;
   }
   return sum;
}

void
UtilityFunctionAggregationBase::Compute(const keyId_t  &name)
{
   std::list<UtilityFunctionBase *>::iterator it = functionalFactors.begin();
   while (it != functionalFactors.end())
   {
      (*it)->Compute(name);
      it++;
   }

}
void
UtilityFunctionAggregationBase::Print(std::ostream &os) const
{
   std::list<UtilityFunctionBase *>::const_iterator it = functionalFactors.begin();

   //FIXME TODO check for alias, dont go deeper if it exists
   os << Name(); //std::string output = Name();
   if (!m_nameOption.empty()) {
     os << "_"; //output.append("_");
     os << m_nameOption; //output.append(m_nameOption);
   }
   os << "[ "; //output.append("[ ");
   if (it != functionalFactors.end())
   {
      (*it)->Print(os);
      it++;
   }
   while (it != functionalFactors.end())
   {
      //output.append((*it)->Print());
      //output.append(", ");
      os << ", ";
      (*it)->Print(os);
      it++;
   }
   /*if (!output.empty()) //remove last 2 characters if we printed
   {
      output = output.substr(0, output.length()-2);
   }*/
   os << " ]"; //output.append(" ]");
   //return output;

}
 void
UtilityFunctionAggregationBase::Print(std::ostream &os, const keyId_t &tlvName, double &value) const
{
   std::list<UtilityFunctionBase *>::const_iterator it = functionalFactors.begin();
   os << Name(); //std::string output = Name();
   if (!m_nameOption.empty()) {
     os << "_"; //output.append("_");
     os << m_nameOption; //output.append(m_nameOption);
   }
   os << "[ "; //output.append("[ ");
   double childValue = 0.0;
   if (functionalFactors.size() == 1) {
       (*it)->Print(os, tlvName, value);
     value = functionValue(value);

   } else {
      if (it != functionalFactors.end())
      {
      (*it)->Print(os, tlvName, value);
      it++;
      }
     while (it != functionalFactors.end())
     {
      //output.append((*it)->Print());
      //output.append(", ");
      os << ", ";
      (*it)->Print(os, tlvName, childValue);
      it++;
      value += functionValue(value, childValue);
      }

   }
   //value = Value
   os << " ]={"; //output.append(" ]=");
   os << value; //std::ostringstream strs;
   os << "} ";

}
double 
UtilityFunctionAggregationBase::functionValue(double param1) const
{ return param1; }
double 
UtilityFunctionAggregationBase::functionValue(double param1, double param2) const
{ 
  throw std::invalid_argument("UtilityFunctionAggregationBase::functionValue should never be called");
  return 0.0; 
}  //should never be called

} /* namespace utility */
} /* namespace icnx */
} /* namespace ns3 */
