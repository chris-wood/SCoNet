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
 *
 * Author:  James Mathewson <jlmathew@soe.ucsc.edu>
 *
 */

#ifndef CCN_SECURITY_MODULE_H
#define	CCN_SECURITY_MODULE_H

#include "ns3/object.h"
#include "ns3/ptr.h"
#include "ns3/traced-callback.h"
//JLM TODO FIXME move utility to its own directory
#include "ns3/ccnSIM/model/cs/utility-caching/UtilityFunctions.h"
#include "ns3/ccnSIM/model/cs/utility-caching/UtilityFunctionGenerator.h"
#include "ns3/pugixml.h"
#include "ns3/node.h"
#include <ns3/ccnSIM/ccn.cxx/name.h>
#include "ns3/ccnSIM/model/cs/utility-caching/UtilityFunctionsAggregate.h"

namespace ns3 {

class Packet;

namespace ccn {

class Data;
class Interest;
class InterestResponse;



/**
 * @ingroup ccn-sm
 * \brief Base class for CCN Security Module
 *
 */
class SecurityModule : public Object
{
public:
  /**
   * \brief Interface ID
   *
   * \return interface ID
   */
  static
  TypeId GetTypeId ();

  /**
   * @brief Virtual destructor
   */
  virtual
  ~SecurityModule ();

  virtual void
  Process (Ptr< Interest> interest);

  virtual void
  Process (Ptr< InterestResponse > interestResponse);
  
  virtual void
  Process (Ptr< Data> data);

  void
  ConfigureSecurity(std::string xmlConfigFilename);

  /**
   * \brief Print out content store entries
   */
  virtual void
  Print (std::ostream &os) const;
  virtual void
  Print (std::ostream &os, const std::string &name) const;
  virtual void
  Print (std::ostream &os, Ptr< Data> data) const;

/* Implement when BlockUtility is ready
  virtual void
  PrintStats (std::ostream &os) const;
  virtual void
  PrintStats (std::ostream &os, std::string &name) const;
  virtual void
  PrintStats (std::ostream &os, Ptr< Data> data) const;
*/


  ////////////////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////////////////

  /**
   * @brief Static call to cheat python bindings
   */
  static inline Ptr<SecurityModule>
  GetSecurityModule (Ptr<Object> node);
  void
  ConfigureSecurityModule(std::string xmlConfigFilename);
static std::string
GetLogName ();

protected:

  ns3::icnx::utility::UtilityBlock * m_securityManagement; 
  //for now FIXME TODO we manage deletions here
};

inline std::ostream&
operator<< (std::ostream &os, const SecurityModule &cs)
{
  cs.Print (os);
  return os;
}

inline Ptr<SecurityModule>
SecurityModule::GetSecurityModule (Ptr<Object> node)
{
  return node->GetObject<SecurityModule> ();
}


} // namespace ccn
} // namespace ns3

#endif // CCN_SECURITY_MODULE_H
