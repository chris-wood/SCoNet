/* -*- Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil -*- */
/*
 * Copyright (c) 2014 University of California, Santa Cruz
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
 * Author: James Mathewson <jlmathew@soe.ucsc.edu>
 *
 * 
 */

/*
 * UtilityFunctions.h
 *
 *  Created on: Oct 11, 2014
 *      Author: James Mathewson, UCSC
 */

#ifndef UTILITYFUNCTIONSSINGLE_H_
#define UTILITYFUNCTIONSSINGLE_H_
#include "ns3/pugixml.h"
#include "ns3/tlv_factory.h"
#include <string>
#include <ostream>
#include <stdint.h>
#include <string>
#include <iostream>
#include <iomanip>
#include <list>
#include "UtilityFunctions.h"
#include "ns3/ccn-1.0_tlv-id.h"

namespace ns3 {
namespace icnx {
namespace utility {

      
class UtilityFunctionSingleUtility : public UtilityFunctionBase {
protected:
  UtilityFunctionSingleUtility();


public:
	UtilityFunctionSingleUtility(pugi::xml_node &xmlConfig);
  ~UtilityFunctionSingleUtility();

};


} /* namespace utility */
} /* namespace icnx */
} /* namespace ns3 */
#endif /* UTILITYFUNCTIONS_H_ */
