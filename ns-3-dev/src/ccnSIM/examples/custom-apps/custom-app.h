/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2011-2012 University of California, Los Angeles
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
 */

// custom-app.h

#ifndef CUSTOM_APP_H_
#define CUSTOM_APP_H_

#include "ns3/ccn-app.h"

namespace ns3 {

/**
 * @brief A simple custom application
 *
 * This applications demonstrates how to send Interests and respond with Datas to incoming interests
 *
 * When application starts it "sets interest filter" (install FIB entry) for /prefix/sub, as well as
 * sends Interest for this prefix
 *
 * When an Interest is received, it is replied with a Data with 1024-byte fake payload
 */
class CustomApp : public ccn::App
{
public:
  // register NS-3 type "CustomApp"
  static TypeId
  GetTypeId ();
  
  // (overridden from ccn::App) Processing upon start of the application
  virtual void
  StartApplication ();

  // (overridden from ccn::App) Processing when application is stopped
  virtual void
  StopApplication ();

  // (overridden from ccn::App) Callback that will be called when Interest arrives
  virtual void
  OnInterest (Ptr<const ccn::Interest> interest);

  // (overridden from ccn::App) Callback that will be called when Data arrives
  virtual void
  OnData (Ptr<const ccn::Data> contentObject);

private:
  void
  SendInterest ();
};

} // namespace ns3

#endif // CUSTOM_APP_H_