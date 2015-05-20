/* -*- Mode: C++{} c-file-style: "gnu"{} indent-tabs-mode:nil -*- */
/*
 * Copyright (c) 2014 University of California, Santa Cruz
 *
 * This program is free software{} you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation{}
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY{} without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program{} if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * Author: James Mathewson <jlmathew@soe.ucsc.edu>
 *
 *
 */


#include "ccn-1.0_app_tlv.h"


namespace wire
{
  namespace icnx
  {

    TLV_APP::TLV_APP ()
    {
      m_type = APP_TLV;
      m_dataChanged = true;

    }


    TLV_APP::~TLV_APP ()
    {
    }

    TLV_APP::TLV_APP (const TLV_APP & tlv_msg)
      :TLV_RAWDATA(tlv_msg)
    {
      //m_DataPtr = NULL;
      //CopyFromTlv (tlv_msg);
    }

    TLV_APP::TLV_APP (const std::string & xmlString)
      :TLV_RAWDATA(xmlString)
    {
     // m_serializedDataPtr = NULL;
     // m_DataPtr = NULL;
     // XmlImplement (xmlString);
    }

    TLV_APP::TLV_APP (const TLV_RAW_DATA_t * rawData)
      :TLV_RAWDATA(rawData)
    {
      //m_DataPtr = NULL;
      //NetworkDeserialize (rawData);
    }

    TLV_APP::TLV_APP (TLV_TYPE_t type, TLV_LENGTH_t length,
                              const TLV_RAW_DATA_t * data)
      :TLV_RAWDATA(type, length, data)
    {
     // m_DataPtr = NULL;
      //m_type = type;
      //SetData (data, length);

    }


    TLV_APP & TLV_APP::operator = (const TLV_APP & other)
    {
      if (this == &other) return *this;   // Gracefully handle self assignment
      CopyFromTlv (other);
      return *this;
    }
    TLV_BASE * 
    TLV_APP::Copy() const
    {
         return new TLV_APP( *this); 
    }

 const std::string 
 TLV_APP::tlvIdName(void) const { return std::string("TLV_APP"); }



  }                             // namespace icnx
}                               // namespace ns3


//visual inspected self test
//only used for debugging
#ifdef _SELF_TEST_TLV_APP_
#include <streambuf>
#include <ostream>
#include <iostream>
#include "ccn-1.0_name_appcomp_tlv.h"
#include "tlv_rawdata.h"
#include "tlv_base.h"
using namespace std;

int
main ()
{
  cout << "Self test Name KeyId Data" << endl;
  wire::icnx::TLV_APP a;
  const uint8_t *var =
    reinterpret_cast < const uint8_t * >("ABCDEFGHIJKLMNOPQRSTUVXYZ");
  wire::icnx::TLV_APP b (257, 26, var); //add 1 for NULL for strings
  wire::icnx::TLV_APP c (b);

  wire::icnx::TLV_APP d (b.NetworkSerialize ());
  wire::icnx::TLV_APP e (b.XmlPrint ());

  wire::icnx::TLV_APP f;
  f.SetData ("0123456789");
  wire::icnx::TLV_APP *g = new wire::icnx::TLV_APP;
  *g = f;

  cout << "constructors seem to work" << endl;

  //verify constructors
  cout << endl << endl << "Hex A()" << endl;
  a.XmlPrint (std::cout);
  a.CreateSerializedBuffer ();
  a.NetworkSerializePrint (std::cout, 4, " ");

  cout << endl << endl << "Hex B(257,27, A-Z)" << endl;
  b.XmlPrint (std::cout);
  b.CreateSerializedBuffer ();
  b.NetworkSerializePrint (std::cout, 4, " ");


  cout << endl << endl << "Hex C(b)" << endl;
  c.CreateSerializedBuffer ();
  c.XmlPrint (std::cout);
  c.NetworkSerializePrint (std::cout, 4, " ");


  cout << endl << endl << "Hex D(via b.Serialize)" << endl;
  d.XmlPrint (std::cout);
  d.CreateSerializedBuffer ();
  d.NetworkSerializePrint (std::cout, 4, " ");


  cout << endl << endl << "Hex E(via b.Xml)" << endl;
  e.XmlPrint (std::cout);
  e.CreateSerializedBuffer ();
  e.NetworkSerializePrint (std::cout, 4, " ");

  cout << endl << endl << "Hex F()" << endl;
  f.XmlPrint (std::cout);
  f.CreateSerializedBuffer ();
  f.NetworkSerializePrint (std::cout, 4, " ");

  cout << endl << endl << "Hex G(g=f)" << endl;
  g->XmlPrint (std::cout);
  g->CreateSerializedBuffer ();
  g->NetworkSerializePrint (std::cout, 4, " ");

    //template test
  double rawt = 3.141596;
  wire::icnx::TLV_APP k;
  g->SetObjectData (rawt, true);
  k = *g;
  k.SetObjectData (rawt, false);

  cout << endl << endl << "Hex G(double 3.141596, true-shallowcopy)" << endl;
  g->XmlPrint (std::cout);
  g->CreateSerializedBuffer ();
  g->NetworkSerializePrint (std::cout, 4, " ");
  double value;
  g->GetObjectData (value, true);
  cout << "3.141596 (true); read back " << value << endl;

  cout << endl << endl << "Hex G(double 3.141596, false-redirection)" << endl;
  k.XmlPrint (std::cout);
  k.CreateSerializedBuffer ();
  k.NetworkSerializePrint (std::cout, 4, " ");
  k.GetObjectData (value, false);
  cout << "3.141596 (false); read back " << value << endl;

  //verify change
  uint8_t array[] = { 10, 11, 12, 13, 14, 15, 16, 17, 18, 19 };
  wire::icnx::TLV_APP * h = new wire::icnx::TLV_APP (5, 10, array);
  wire::icnx::TLV_APP * j = new wire::icnx::TLV_APP (*h);
  cout << endl << endl << "Deep copy verify" << endl;

  //change test
  cout << endl << "G(7,5, ABCDE)" << endl;
  g->SetType (7);

  uint8_t array2[] = "ABCDE";
  g->SetData (array2, 5);
  g->XmlPrint (std::cout);
  g->CreateSerializedBuffer ();
  g->NetworkSerializePrint (std::cout, 4, " ");

}

#endif