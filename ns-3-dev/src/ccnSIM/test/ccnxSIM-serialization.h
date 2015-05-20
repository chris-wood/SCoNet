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
 * Author: Ilya Moiseenko <iliamo@cs.ucla.edu>
 *         Alexander Afanasyev <alexander.afanasyev@ucla.edu>
 */

#ifndef CCNXSIM_SERIALIZATION_H
#define CCNXSIM_SERIALIZATION_H

#include "ns3/test.h"

namespace ns3
{

class CcnxInterestSerializationTest : public TestCase
{
public:
  CcnxInterestSerializationTest ()
    : TestCase ("Interest Serialization Test")
  {
  }
    
private:
  virtual void DoRun ();
};

class CcnxDataSerializationTest : public TestCase
{
public:
  CcnxDataSerializationTest ()
    : TestCase ("Data Serialization Test")
  {
  }
    
private:
  virtual void DoRun ();
};

}

#endif // CCNSIM_SERIALIZATION_H
