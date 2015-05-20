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

#include "ccn-1.0_validationalg_tlv.h"
namespace wire
{
  namespace icnx
  {

    TLV_VALIDATION_ALG::TLV_VALIDATION_ALG ()
    {
      m_serializedDataPtr = NULL;
      m_length = 0;
      m_type = VALIDATION_ALG_TLV;
      m_dataChanged = true;

    }

    TLV_VALIDATION_ALG::~TLV_VALIDATION_ALG ()
    {

    }

    TLV_VALIDATION_ALG::TLV_VALIDATION_ALG (const TLV_VALIDATION_ALG & tlv_msg)
      : TLV_TLVDATA(tlv_msg)
    {
      //m_serializedDataPtr = NULL;
      //CopyFromTlv (tlv_msg);

    }

    TLV_VALIDATION_ALG::TLV_VALIDATION_ALG (const TLV_RAW_DATA_t * rawData)
      :TLV_TLVDATA(rawData)
    {
      //m_serializedDataPtr = NULL;
      //NetworkDeserialize (rawData);

    }

    TLV_VALIDATION_ALG::TLV_VALIDATION_ALG (TLV_TYPE_t type, TLV_LENGTH_t length,
                              const TLV_RAW_DATA_t * data)
      : TLV_TLVDATA(type, length, data)
    {
     /* m_serializedDataPtr = NULL;
      m_type = type;
      //validate length and data
      if (0 != length)
      {
        SetData (data, length);
      } else
      {
        m_dataChanged = true;
        m_length = 0;
        tlvList.clear();
      }*/
    }

    TLV_VALIDATION_ALG::TLV_VALIDATION_ALG (const std::list<TLV_BASE *> &tlvDataList)
      : TLV_TLVDATA(tlvDataList)
    {
         // tlvList.clear();
          //tlvList.assign(tlvDataList.begin(), tlvDataList.end());
          //m_dataChanged = true;

    }

    TLV_VALIDATION_ALG::TLV_VALIDATION_ALG (const std::string & xmlString)
      :TLV_TLVDATA(xmlString)
    {
      //m_serializedDataPtr = NULL;
      //XmlImplement (xmlString);

    }
    TLV_BASE * 
    TLV_VALIDATION_ALG::Copy() const
    {
         return new TLV_VALIDATION_ALG( *this); 
    }

 const std::string 
 TLV_VALIDATION_ALG::tlvIdName() const { return std::string("TLV_VALIDATION_ALG"); }

    TLV_VALIDATION_ALG & TLV_VALIDATION_ALG::operator = (const TLV_VALIDATION_ALG & other)
    {
      if (this == &other) return *this;   // Gracefully handle self assignment
      CopyFromTlv (other);
      return *this;      
    }

  }                             // namespace icn
}                               // namespace ns3

//visual inspected self test
//only used for debugging
#ifdef _SELF_TEST_VALIDATION_ALG_TLV_
#include <streambuf>
#include <ostream>
#include <iostream>
#include <list>
#include "tlv_rawdata.h"
#include "tlv_endiandata.h"
#include "tlv_container.h"
#include "tlv_base.h"
#include "ccn-1.0_tlv-id.h"
#include "ccn-1.0_content_tlv.h"

using namespace std;

int
main ()
{

  cout << "Self test Content(TlvData)" << endl;
  wire::icnx::TLV_VALIDATION_ALG a;
  cout << endl << endl << "Hex A()" << endl;
  a.XmlPrint (std::cout);
  a.CreateSerializedBuffer ();

  const uint8_t *var =
    reinterpret_cast < const uint8_t * >("ABCDEFGHIJKLMNOPQRSTUVXYZ");
  const wire::icnx::TLV_RAWDATA *c = new wire::icnx::TLV_RAWDATA (wire::icnx::GENERIC_TLV_RAWDATA_FAKE1, 26, var); 
  wire::icnx::TLV_RAWDATA *c1 = new wire::icnx::TLV_RAWDATA (wire::icnx::GENERIC_TLV_RAWDATA_FAKE2, 26, var); 
  wire::icnx::TLV_ENDIANDATA * c3 = new wire::icnx::TLV_ENDIANDATA(wire::icnx::GENERIC_TLV_ENDIANDATA,0xffeeddee); 
  wire::icnx::TLV_VALIDATION_ALG *b= new wire::icnx::TLV_VALIDATION_ALG();
  {
    wire::icnx::TLV_RAWDATA *c2 = new wire::icnx::TLV_RAWDATA(wire::icnx::GENERIC_TLV_RAWDATA_FAKE3, 26, var); 

    b->SetData(new wire::icnx::TLV_RAWDATA(*c));
    b->AppendTlvData(new wire::icnx::TLV_RAWDATA(*c1));
    b->AppendTlvData(new wire::icnx::TLV_RAWDATA(wire::icnx::GENERIC_TLV_RAWDATA_FAKE3, 26, var));
    b->AppendTlvData(c2);
    b->AppendTlvData(c3);
  }
  boost::ptr_list<wire::icnx::TLV_BASE> copy;
  b->GetTlvDataLinkedList(copy, wire::icnx::GENERIC_TLV_RAWDATA_FAKE3);
  //delete c3;
  //should all pointers be boost smart pointers?
  cout << "\nHex B(B - 5 TLVs)" << endl;
  b->XmlPrint (std::cout);
  b->CreateSerializedBuffer ();
  b->NetworkSerializePrint (std::cout, 4, " ");

  wire::icnx::TLV_VALIDATION_ALG *e = new wire::icnx::TLV_VALIDATION_ALG();
  e->SetData(new wire::icnx::TLV_VALIDATION_ALG(*b));
  wire::icnx::TLV_VALIDATION_ALG *d = new wire::icnx::TLV_VALIDATION_ALG();
  d->SetTlvData(copy);


  cout << "B should pass 2 TLVs, passed " << copy.size() << endl;
  b->DeleteTlvData(wire::icnx::GENERIC_TLV_RAWDATA_FAKE3);

  cout << "B should have deleted 2 TLV(type 6), have 3 TLVs "  << endl;

  b->XmlPrint (std::cout);
  b->CreateSerializedBuffer ();
  b->NetworkSerializePrint (std::cout, 4, " ");

  //cout << "Hex c(257,26, A-Z)" << endl;
  //c.XmlPrint (std::cout);
  //c.CreateSerializedBuffer ();
  //c.NetworkSerializePrint (std::cout, 4, " ");
  //cout << endl << endl << "Hex c1(25,26, A-Z)" << endl;
  //c1.XmlPrint (std::cout);
  //c1.CreateSerializedBuffer ();
  //c1.NetworkSerializePrint (std::cout, 4, " ");
  //cout << endl << endl << "Hex c2(7,26, A-Z)" << endl;
  //c2->XmlPrint (std::cout);
  //c2->CreateSerializedBuffer ();
  //c2->NetworkSerializePrint (std::cout, 4, " ");
  //cout << endl << endl << "Hex c3(15,endian 0xffeeddcc)" << endl;
  //c3->XmlPrint (std::cout);
  //c3->CreateSerializedBuffer ();
  //c3->NetworkSerializePrint (std::cout, 4, " ");


  cout << endl << endl << "Hex D(b->2 TLVs)" << endl;
  d->CreateSerializedBuffer ();
  d->XmlPrint (std::cout);
  d->NetworkSerializePrint (std::cout, 4, " ");

  cout << endl << endl << "Hex E(tlv B) with 5 objects" << endl;
  e->CreateSerializedBuffer ();
  e->XmlPrint (std::cout);
  e->NetworkSerializePrint (std::cout, 4, " ");

  

  cout << endl << endl << "Hex G1(d->serialize with 2 objects" << endl;
  wire::icnx::TLV_VALIDATION_ALG *g1 = new wire::icnx::TLV_VALIDATION_ALG (d->NetworkSerialize ());
  g1->CreateSerializedBuffer ();
  g1->XmlPrint (std::cout);
  g1->NetworkSerializePrint (std::cout, 4, " ");

  wire::icnx::TLV_VALIDATION_ALG *g2 = new wire::icnx::TLV_VALIDATION_ALG(d->XmlPrint ());
  cout << endl << endl << "Hex G1(d->xml with 2 objects" << endl;
  g2->CreateSerializedBuffer ();
  g2->XmlPrint (std::cout);
  g2->NetworkSerializePrint (std::cout, 4, " ");


  cout << endl << "next step" << endl;
  wire::icnx::TLV_VALIDATION_ALG *g = new wire::icnx::TLV_VALIDATION_ALG (e->NetworkSerialize ());
  wire::icnx::TLV_VALIDATION_ALG *h = new wire::icnx::TLV_VALIDATION_ALG(e->XmlPrint ());
  cout << "delete e" << endl;
  delete e;
  cout << "Hex G(e->serialize) with 5 objects" << endl;

  g->CreateSerializedBuffer ();
  g->XmlPrint (std::cout);
  g->NetworkSerializePrint (std::cout, 4, " ");
 
  cout << "constructors seem to work" << endl;


//e->XmlPrint(std::cout);
  cout << endl << endl << "Hex H(e->xmlprint) with 5 objects" << endl;

  h->CreateSerializedBuffer ();
  h->XmlPrint (std::cout);
  h->NetworkSerializePrint (std::cout, 4, " ");

 

  //verify change
  uint8_t array[] = { 10, 11, 12, 13, 14, 15, 16, 17, 18, 19 };
  wire::icnx::TLV_RAWDATA * l = new wire::icnx::TLV_RAWDATA (5, 10, array);
  wire::icnx::TLV_RAWDATA * m = new wire::icnx::TLV_RAWDATA (*l);
  cout << endl << endl << "Deep copy verify" << endl;

  l->SetType (7);

  uint8_t array2[] = "ABCDE";
  l->SetData (array2, 5);
  l->XmlPrint (std::cout);
  l->CreateSerializedBuffer ();
  l->NetworkSerializePrint (std::cout, 4, " ");
  delete l;
  m->XmlPrint (std::cout);
  m->CreateSerializedBuffer ();
  m->NetworkSerializePrint (std::cout, 4, " ");


}

#endif

