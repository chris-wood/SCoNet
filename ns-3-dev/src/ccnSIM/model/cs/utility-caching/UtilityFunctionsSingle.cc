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
 * UtilityFunctions.cc
 *
 *  Created on: Oct 11, 2014
 *      Author: James Mathewson, UCSC
 */

#include "UtilityFunctionsSingle.h"

namespace ns3 {
namespace icnx {
namespace utility {


/*
    m_valueHigh = xmlConfig.attribute("highValue").as_double(m_valueHigh);

const char_t* xml_attribute::as_string(const char_t* def = "") const;

int xml_attribute::as_int(int def = 0) const;
unsigned int xml_attribute::as_uint(unsigned int def = 0) const;
double xml_attribute::as_double(double def = 0) const;
float xml_attribute::as_float(float def = 0) const;
bool xml_attribute::as_bool(bool def = false) const;
long long xml_attribute::as_llong(long long def = 0) const;
unsigned long long xml_attribute::as_ullong(unsigned long long def = 0) const;
   */

//TODO FIXME remove this completely, not needed
  UtilityFunctionSingleUtility::UtilityFunctionSingleUtility(pugi::xml_node &xmlConfig)
  : UtilityFunctionBase(xmlConfig)
  {}
  UtilityFunctionSingleUtility::UtilityFunctionSingleUtility() {}
  UtilityFunctionSingleUtility::~UtilityFunctionSingleUtility(){}

} /* namespace utility */
} /* namespace icnx */
} /* namespace ns3 */
