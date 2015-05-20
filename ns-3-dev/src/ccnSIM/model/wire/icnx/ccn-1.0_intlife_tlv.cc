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


#include "ccn-1.0_intlife_tlv.h"


namespace wire {
namespace icnx {

TLV_INTLIFE::TLV_INTLIFE ()
{
      m_length = 1;
      m_endianData = 0;
      m_type = INTLIFE_TLV;
      m_serializedDataPtr = NULL;
      m_dataChanged = true;
}
TLV_INTLIFE::~TLV_INTLIFE ()
{
}

TLV_INTLIFE::TLV_INTLIFE (const TLV_RAW_DATA_t *rawData)
  :TLV_ENDIANDATA(rawData)
{
      //m_serializedDataPtr = NULL;
      //NetworkDeserialize (rawData);
}

TLV_INTLIFE::TLV_INTLIFE (uint64_t time)
{
      m_type = INTLIFE_TLV;
  m_endianData = time;
  m_length = GetLength();
  m_dataChanged = true;
}

    TLV_INTLIFE::TLV_INTLIFE (const TLV_INTLIFE & tlv_msg)
  :TLV_ENDIANDATA(tlv_msg)
    {
      //m_serializedDataPtr = NULL;
      //CopyFromTlv (tlv_msg);
    }

    TLV_INTLIFE::TLV_INTLIFE (TLV_TYPE_t type, TLV_LENGTH_t length,
                                    const TLV_RAW_DATA_t * data)
    :TLV_ENDIANDATA(type, length, data)
    {
      //m_type = type;
      //m_serializedDataPtr = NULL;

      //validate length and data
      //SetData (data, length);
    }

    TLV_INTLIFE::TLV_INTLIFE (const std::string &xmlString)
  :TLV_ENDIANDATA(xmlString)
    {
      //m_serializedDataPtr = NULL;
      //XmlImplement(xmlString);
    }


 const std::string 
 TLV_INTLIFE::tlvIdName(void) const { return std::string("TLV_INTLIFE"); }

TLV_INTLIFE &
TLV_INTLIFE::operator = (const TLV_INTLIFE &other)
{
  if (this == &other) return *this;
  CopyFromTlv(other);
  return *this;
}
    TLV_BASE * 
    TLV_INTLIFE::Copy() const
    {
         return new TLV_INTLIFE( *this); 
    }

uint64_t
TLV_INTLIFE::GetTime() const
{
   return m_endianData;
}

void
TLV_INTLIFE::SetTime(uint64_t time)
{
   SetData(time);
}

  

} // namespace ccn
} // namespace ns3

//only used for debugging
#ifdef _SELF_TEST_CCN_INTLIFE_
#include <streambuf>
#include <ostream>
#include <iostream>
#include "ccn-1.0_opt_tlv-lifetime.h"
using namespace std;
int main()
{
   cout << "PERHOP TLV LIFETIME:" << endl;
   wire::icnx::TLV_INTLIFE a;
   wire::icnx::TLV_INTLIFE b(500);
   
   cout << endl << endl << "Hex A()" << endl;
   a.XmlPrint(std::cout);
   a.CreateSerializedBuffer();
   a.NetworkSerializePrint(std::cout, 4, " ");
   
   cout << endl << endl << "Hex B(500)" << endl;
   b.XmlPrint(std::cout);
   b.CreateSerializedBuffer();
   b.NetworkSerializePrint(std::cout, 4, " ");   

   
   wire::icnx::TLV_INTLIFE c(b);
   cout << endl << endl << "Hex C(B)" << endl;
   c.XmlPrint(std::cout);
   c.CreateSerializedBuffer();
   c.NetworkSerializePrint(std::cout, 4, " ");   

   c.SetTime(70000);
   cout << endl << endl << "Hex D(B.settime(70000))" << endl;
   c.XmlPrint(std::cout);
   c.CreateSerializedBuffer();
   c.NetworkSerializePrint(std::cout, 4, " ");

    wire::icnx::TLV_INTLIFE d(b.GetTime());
   cout << endl << endl << "Hex E(c.gettime)" << endl;
   d.XmlPrint(std::cout);
   d.CreateSerializedBuffer();
   d.NetworkSerializePrint(std::cout, 4, " ");   
   cout << flush << endl;

   d.SetTime(250);
   wire::icnx::TLV_INTLIFE e(d.NetworkSerialize());
   cout << endl << endl << "Hex F(d.bin(8bit))" << endl;
   e.XmlPrint(std::cout);
   e.CreateSerializedBuffer();
   e.NetworkSerializePrint(std::cout, 4, " ");   

   d.SetTime(500);
   wire::icnx::TLV_INTLIFE g(d.NetworkSerialize());
   cout << endl << endl << "Hex G(d.bin(16bit))" << endl;
   g.XmlPrint(std::cout);
   g.CreateSerializedBuffer();
   g.NetworkSerializePrint(std::cout, 4, " ");   

   d.SetTime(70000);
   wire::icnx::TLV_INTLIFE h(d.NetworkSerialize());
   cout << endl << endl << "Hex H(d.bin(32bit))" << endl;
   h.XmlPrint(std::cout);
   h.CreateSerializedBuffer();
   h.NetworkSerializePrint(std::cout, 4, " ");     
}
#endif

