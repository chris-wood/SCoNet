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


#include "ccn-1.0_interestresp_returncode_tlv.h"



namespace wire {
namespace icnx {

TLV_INTERESTRESP_RETURNCODE::TLV_INTERESTRESP_RETURNCODE ()
{
     m_length = 1;
      m_endianData = 0;
      m_type = INTERESTRESP_RETURNCODE_TLV;
      m_serializedDataPtr = NULL;
      m_dataChanged = true;
}
TLV_INTERESTRESP_RETURNCODE::~TLV_INTERESTRESP_RETURNCODE ()
{
}

TLV_INTERESTRESP_RETURNCODE::TLV_INTERESTRESP_RETURNCODE (const TLV_RAW_DATA_t *rawData)
  :TLV_ENDIANDATA(rawData)
{
      //m_serializedDataPtr = NULL;
     // NetworkDeserialize (rawData);
}

TLV_INTERESTRESP_RETURNCODE::TLV_INTERESTRESP_RETURNCODE (INTERESTRESP_RC_t type)
{
  m_type = INTERESTRESP_RETURNCODE_TLV;
  m_endianData = static_cast<uint8_t>(type);
  m_length = GetLength();
  m_dataChanged = true;
}



    TLV_INTERESTRESP_RETURNCODE::TLV_INTERESTRESP_RETURNCODE (const TLV_INTERESTRESP_RETURNCODE & tlv_msg)
  :TLV_ENDIANDATA(tlv_msg)
    {
      //m_serializedDataPtr = NULL;
      //CopyFromTlv (tlv_msg);
    }


    TLV_INTERESTRESP_RETURNCODE::TLV_INTERESTRESP_RETURNCODE (const std::string &xmlString)
  :TLV_ENDIANDATA(xmlString)
    {
     // m_serializedDataPtr = NULL;
     // XmlImplement(xmlString);
    }

    TLV_BASE * 
    TLV_INTERESTRESP_RETURNCODE::Copy() const
    {
         return new TLV_INTERESTRESP_RETURNCODE( *this); 
    }


TLV_INTERESTRESP_RETURNCODE &
TLV_INTERESTRESP_RETURNCODE::operator = (const TLV_INTERESTRESP_RETURNCODE &other)
{
  if (this == &other) return *this;
      CopyFromTlv(other);
  return *this;
}

    TLV_LENGTH_t
    TLV_INTERESTRESP_RETURNCODE::GetLength () const
    {
      return sizeof(uint8_t); //only allowed size
    }

TLV_INTERESTRESP_RETURNCODE::INTERESTRESP_RC_t
TLV_INTERESTRESP_RETURNCODE::GetReturnCode() const
{
   //since only c++11 can scope enums, we ensure our data is only 8 bits
   uint8_t value =  static_cast<uint8_t>(m_endianData);
   return static_cast<INTERESTRESP_RC_t>(value);
}

void
TLV_INTERESTRESP_RETURNCODE::SetReturnCode(INTERESTRESP_RC_t code)
{
   m_endianData = static_cast<uint8_t>(code);
}


 const std::string 
 TLV_INTERESTRESP_RETURNCODE::tlvIdName(void) const { return std::string("TLV_INTERESTRESP_RETURNCODE"); }  

} // namespace icnx
} // namespace ns3

//only used for debugging
#ifdef _SELF_TEST_TLV_INTERESTRESP_RETURNCODE_
#include <streambuf>
#include <ostream>
#include <iostream>
#include "ccn-1.0_interesthop_tlv.h"
using namespace std;
int main()
{
   cout << "INTEREST TLV LIFETIME:" << endl;
   wire::icnx::TLV_INTERESTRESP_RETURNCODE a;
   wire::icnx::TLV_INTERESTRESP_RETURNCODE b(8);
   
   cout << endl << endl << "Hex A()" << endl;
   a.XmlPrint(std::cout);
   a.CreateSerializedBuffer();
   a.NetworkSerializePrint(std::cout, 4, " ");
   
   cout << endl << endl << "Hex B(8)" << endl;
   b.XmlPrint(std::cout);
   b.CreateSerializedBuffer();
   b.NetworkSerializePrint(std::cout, 4, " ");   

   
   wire::icnx::TLV_INTERESTRESP_RETURNCODE c(b);
   cout << endl << endl << "Hex C(B)" << endl;
   c.XmlPrint(std::cout);
   c.CreateSerializedBuffer();
   c.NetworkSerializePrint(std::cout, 4, " ");   

   c.SetInterestHops(12);
   cout << endl << endl << "Hex D(B(8))" << endl;
   c.XmlPrint(std::cout);
   c.CreateSerializedBuffer();
   c.NetworkSerializePrint(std::cout, 4, " ");

    wire::icnx::TLV_INTERESTRESP_RETURNCODE d(b.GetInterestHops());
   cout << endl << endl << "Hex E(c.gethops)" << endl;
   d.XmlPrint(std::cout);
   d.CreateSerializedBuffer();
   d.NetworkSerializePrint(std::cout, 4, " ");   
   cout << flush << endl;

   d.SetInterestHops(200);
   wire::icnx::TLV_INTERESTRESP_RETURNCODE e(d.NetworkSerialize());
   cout << endl << endl << "Hex F(d.bin(200))" << endl;
   e.XmlPrint(std::cout);
   e.CreateSerializedBuffer();
   e.NetworkSerializePrint(std::cout, 4, " ");   

   d.SetInterestHops(13);
   wire::icnx::TLV_INTERESTRESP_RETURNCODE g(d.NetworkSerialize());
   cout << endl << endl << "Hex G(d.bin(13))" << endl;
   g.XmlPrint(std::cout);
   g.CreateSerializedBuffer();
   g.NetworkSerializePrint(std::cout, 4, " ");   

   d.SetInterestHops((257));
   wire::icnx::TLV_INTERESTRESP_RETURNCODE h(d.NetworkSerialize());
   cout << endl << endl << "Hex H(d.bin(257->1))" << endl;
   h.XmlPrint(std::cout);
   h.CreateSerializedBuffer();
   h.NetworkSerializePrint(std::cout, 4, " ");     
}
#endif

