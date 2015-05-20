/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2011,2012 University of California, Los Angeles
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation;
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * Author: Alexander Afanasyev <alexander.afanasyev@ucla.edu>
 *         Ilya Moiseenko <iliamo@cs.ucla.edu>
 *         
 */

#include "ccn-security-module.h"
#include "ns3/log.h"
#include "ns3/packet.h"
#include "ns3/ccn-name.h"
#include "ns3/ccn-interest.h"
#include "ns3/ccn-data.h"
 #include "ns3/string.h"
#include <string>
#include <queue>

NS_LOG_COMPONENT_DEFINE ("ccn.sm.SecurityModule");

namespace ns3 {
namespace ccn {


NS_OBJECT_ENSURE_REGISTERED (SecurityModule);

NS_LOG_COMPONENT_DEFINE (SecurityModule::GetLogName ().c_str ());

TypeId
SecurityModule::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::ccn::sm::UtilitySecurity")
    .SetGroupName ("Ccn")
    .SetParent<Object> ()
    .AddConstructor<SecurityModule> ()
    .AddAttribute ("ConfigureSecurity",
                      "Set the XML utility configuration file.   If blank, No caching is default.",
                      StringValue ("NoSecurity.xml"),
                      MakeStringAccessor (&SecurityModule::ConfigureSecurity),
                      MakeStringChecker()) 
    
    ;

  return tid;
}


SecurityModule::~SecurityModule () 
{
}

std::string
SecurityModule::GetLogName ()
{
  return "ccn.sm";
}
void
SecurityModule::Process (Ptr< Interest> interest)
{

  if (!m_securityManagement) {
    return ;
  }

  NS_LOG_FUNCTION (interest->GetName());
  wire::icnx::TLV_INTEREST *tlvInterest;
  tlvInterest = interest->GetTlv();
  if (m_securityManagement)
  {
    m_securityManagement->OnInterestIngress(tlvInterest);
  }

  
}
  void
  SecurityModule::Process (Ptr< InterestResponse > interestResponse) {
  if (!m_securityManagement) {
    return ;
  }

  
  }

void
SecurityModule::Process (Ptr< Data> data)
{
  if (!m_securityManagement) {
    return ;
  }

  NS_LOG_FUNCTION (data->GetName());
//std::cout << "SM [data node " << this->GetObject<Node>()->GetId() << "] result: " << data->GetName() << " maxsize: " << m_maxSize << std::endl;  

  
  ::wire::icnx::TLV_CONTENT *tlvContent;
  tlvContent =  data->GetTlv();

      m_securityManagement->OnDataIngress(tlvContent);
  
}

//need a print based upon dataobject FIXME TODO
void
SecurityModule::Print (std::ostream &os) const
{
NS_LOG_FUNCTION (this);
     Print(os, "");
}

void
SecurityModule::Print (std::ostream &os, const std::string &name) const
{
NS_LOG_FUNCTION (name);
     double val = 0.0;
Ptr<Node> node = this->GetObject<Node> ();
  //std::ostringstream strs;
  //std::string name = nname.toUri();
int32_t nodeNum;
  if (node) {
  nodeNum = node->GetId();
  } else {
    nodeNum = -1;
  }

os<< "N" << nodeNum << "{" << name << "}::";
if (name.empty()) {
//os << (*it)->Print() << "\n";
 m_securityManagement->Print(os) ;
} else {
//os << (*it)->Print(name, val) << "\n";
 m_securityManagement->Print(os, name, val);
}
     //os << strs;
  // }

}
void
SecurityModule::Print (std::ostream &os, Ptr< Data> data) const
{  
NS_LOG_FUNCTION (data->GetName() );
  //Ptr< const ::wire::icnx::TLV_CONTENT> tlvPtr;
  wire::icnx::TLV_CONTENT *tlvContent;
  tlvContent = data->GetTlv();
  std::string name = data->GetStringName();

   Print(os, name);
}


void
SecurityModule::ConfigureSecurity(std::string xmlConfigFilename)
{
NS_LOG_FUNCTION (xmlConfigFilename);
  
  //open file
  pugi::xml_document doc;
  pugi::xml_parse_result result = doc.load_file(xmlConfigFilename.c_str());

  if (!result) {
//JLM FIXME TODO should be a warning log
//std::cout << "SM Load [node " << this->GetObject<Node>()->GetId() << "] result: " << result.description() << std::endl;  
     m_securityManagement = NULL;
     return;  
  }
  //append to current list if new
  //need node number/name
  pugi::xml_node utilitySecurity = doc.child("UtilitySecurity") ;  //only want the utility caching part
  pugi::xml_node utilityConfig = utilitySecurity.child("Utility");

  //needed for multiple blocks
      //only 1 block for now
      m_securityManagement = dynamic_cast< ns3::icnx::utility::UtilityBlock * >(ns3::icnx::utility::UtilityGenerator::CreateNewUtility(utilityConfig));
      if (m_securityManagement) {
        m_securityManagement->OnInit(NULL);
      } //std::ostringstream os;
  
}
//////////////////////////////////////////////////////////////////////



} // namespace ccn
} // namespace ns3
