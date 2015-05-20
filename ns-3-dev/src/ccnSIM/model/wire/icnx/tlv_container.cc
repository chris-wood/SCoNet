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

#include "tlv_container.h"
#include "tlv_base.h"
#include "tlv_factory.h"



namespace wire
{
  namespace icnx
  {


    TLV_TLVDATA::TLV_TLVDATA ()
    {
      m_serializedDataPtr = NULL;
      m_length = 0;
      m_type = GENERIC_TLV_TLVDATA;
      m_dataChanged = true;
    }

    TLV_TLVDATA::~TLV_TLVDATA ()
    {
      tlvList.clear();
    }

    TLV_TLVDATA::TLV_TLVDATA (const TLV_TLVDATA & tlv_msg)
    {
      m_serializedDataPtr = NULL;
      CopyFromTlv (tlv_msg);

    }

    TLV_TLVDATA::TLV_TLVDATA (const TLV_RAW_DATA_t * rawData)
    {
      m_serializedDataPtr = NULL;
      NetworkDeserialize (rawData);

    }

    TLV_TLVDATA::TLV_TLVDATA (TLV_TYPE_t type, TLV_LENGTH_t length,
                              const TLV_RAW_DATA_t * data)
    {
      m_serializedDataPtr = NULL;
      m_type = type;
        m_dataChanged = true;
      //validate length and data
      if (0 != length)
      { 
        SetData (data, length);
      } else
      {
        m_length = 0;
        tlvList.clear();
      }
    }

    TLV_TLVDATA::TLV_TLVDATA (const std::list<TLV_BASE *> &tlvDataList)
    {
          tlvList.clear();
          tlvList.assign(tlvDataList.begin(), tlvDataList.end());
          m_dataChanged = true;

    }

    TLV_TLVDATA::TLV_TLVDATA (const std::string & xmlString)
    {
      m_serializedDataPtr = NULL;
      XmlImplement (xmlString);

    }


    TLV_LENGTH_t 
    TLV_TLVDATA::GetLength () const
    {
      TLV_LENGTH_t length=0;
          for (std::list<TLV_BASE *>::const_iterator it = tlvList.begin(); it != tlvList.end(); it++)
          {
             length += (*it)->GetSerializedSize();
          }
          return length;
    }
    
    const TLV_RAW_DATA_t *
    TLV_TLVDATA::GetData () const
    {
        CreateSerializedBuffer();
        return &m_serializedDataPtr[TLV_BASE_SIZE];

    }

    void
    TLV_TLVDATA::GetTlvDataLinkedList (std::list<TLV_BASE *> &copy, TLV_TYPE_t lower, TLV_TYPE_t upper) const
    {
        TlvRange tlvRange;
        tlvRange.setLowerBound(lower);
        tlvRange.setUpperBound(upper);
        GetTlvDataLinkedList(copy, tlvRange);
    }

    void
    TLV_TLVDATA::GetTlvDataLinkedList (std::list<TLV_BASE *> &copy, TLV_TYPE_t type) const
    {
        m_dataChanged = true;
        copy.clear();
        if (ALL_TLVS == type) 
        {
          copy.assign(tlvList.begin(), tlvList.end());

        } else
        {
          GetTlvDataLinkedList(copy, type, type);
        }

    }

  void
	TLV_TLVDATA::GetTlvDataLinkedList(std::list<TLV_BASE *> &copy, TlvRange &tlvRange) const
  {
        copy.clear();
          for (std::list<TLV_BASE *>::const_iterator it = tlvList.begin(); it != tlvList.end(); it++)
          {

              TLV_TYPE_t type = (*it)->GetType();
              if (tlvRange.isInRange(type))
              {
                 copy.push_back((*it));
              }
          }

  }
    TLV_BASE *
    TLV_TLVDATA::GetFirstTlvData(TLV_TYPE_t type) 
      {
         return GetFirstTlvData(type, type);
      }

    TLV_BASE *
    TLV_TLVDATA::GetFirstTlvData(TLV_TYPE_t lower, TLV_TYPE_t upper) 
      {
        TlvRange tlvRange;
        tlvRange.setLowerBound(lower);
        tlvRange.setUpperBound(upper);
        return GetFirstTlvData(tlvRange);



      }

    TLV_BASE *
    TLV_TLVDATA::GetFirstTlvData(TlvRange &tlvRange)  
      {
        m_dataChanged = true;
           for (std::list<TLV_BASE *>::const_iterator it = tlvList.begin(); it != tlvList.end(); it++)
          {
              TLV_TYPE_t type = (*it)->GetType();
              if (tlvRange.isInRange(type))
              {
                return (*it);
              }
          }
          return NULL;

      }

    void
      TLV_TLVDATA::SetData (const TLV_RAW_DATA_t * data, TLV_LENGTH_t size)
    {
      m_dataChanged = true;
      m_length = size; //in case more than 1 TLV in payload
      NetworkDataDeserialize(data);
    }

    void 
      TLV_TLVDATA::SetData ( TLV_BASE * tlv)
    {
        tlvList.clear();
        tlvList.push_back(tlv);
        m_dataChanged = true;
    }

    void 
      TLV_TLVDATA::AppendTlvData (TLV_BASE * appendTlv)
    {
      m_dataChanged = true;
      tlvList.push_back(appendTlv);
    }

    void 
      TLV_TLVDATA::SetTlvData ( std::list < TLV_BASE * > &tlvDataList)
    {
          tlvList.clear();
          tlvList.assign(tlvDataList.begin(), tlvDataList.end());
          m_dataChanged = true;

    }

    void 
      TLV_TLVDATA::SetData ( std::list < TLV_BASE * > &tlvDataList)
    {
         SetTlvData(tlvDataList);
    }

    void 
      TLV_TLVDATA::DeleteTlvData (TLV_TYPE_t type)
    {
        m_dataChanged = true;
        if (ALL_TLVS == type) 
        {
          tlvList.clear();

        } else
        {
           DeleteTlvData(type, type);
          
        }
    }
    
    void 
      TLV_TLVDATA::DeleteTlvData ( TlvRange &tlvRange)
    {
        m_dataChanged = true;
        std::list<TLV_BASE *>::iterator it = tlvList.begin();
          while ( it != tlvList.end()) //TODO use remove_if()
          {
              std::list<TLV_BASE *>::iterator delit = tlvList.end();

              TLV_TYPE_t type = (*it)->GetType();
              if (tlvRange.isInRange(type))
              {
                 delit = it; 
              }
              it++;
              if (tlvList.end() != delit) 
              {
                tlvList.erase(delit);
              }
          }
          
    }
    
    void 
      TLV_TLVDATA::DeleteTlvData (TLV_TYPE_t lowId, TLV_TYPE_t highId)
    {
        m_dataChanged = true;
        TlvRange tlvRange;
        tlvRange.setUpperBound(highId);
        tlvRange.setLowerBound(lowId);
        DeleteTlvData(tlvRange);
          
    }
    
    bool 
    TLV_TLVDATA::ExistTlvData (TLV_TYPE_t typeLow, TLV_TYPE_t typeHigh) const
    {
        TlvRange tlvRange;
        tlvRange.setUpperBound(typeHigh);
        tlvRange.setLowerBound(typeLow);
        return ExistTlvData(tlvRange);
    }

    bool 
    TLV_TLVDATA::ExistTlvData (TLV_TYPE_t type) const
    {
        return ExistTlvData(type, type);
    }

    bool 
    TLV_TLVDATA::ExistTlvData (TlvRange &tlvRange) const
    {

        for (std::list<TLV_BASE *>::const_iterator it = tlvList.begin(); it != tlvList.end(); it++)
          {
              TLV_TYPE_t type = (*it)->GetType();
              if (tlvRange.isInRange(type))
              {
                 return true; 
              }
          }
          return false;
    }

    /*
   * @brief Print out the TLV Value field (data ) in an ascii xml format
   *
  **/
  void
  TLV_TLVDATA::XmlPrintData(std::ostream &os, int numspace) const
  {
      std::ios::fmtflags saveFlags = os.flags();
      
      std::string space(numspace, ' ');
      std::string space2(numspace+1, ' ');
      os << space2 << "<Data numEntries=\"" << tlvList.size() <<  "\" >\n";
        for (std::list<TLV_BASE *>::const_iterator it = tlvList.begin(); it != tlvList.end(); it++)
          {
           (*it)->XmlPrint(os, numspace+2);
          }

      os << space2 << "</Data>\n";
      os << space << "</TLV>\n";
        os.flags(saveFlags);


  }

    void
    TLV_TLVDATA::NetworkDataDeserialize (const TLV_RAW_DATA_t * data)
    {
      tlvList.clear();
      if (NULL == data)
        return;
      
       //use the factory to help with the 'data' of TLV's
      unsigned int serialLength = 0;
      unsigned int length = 0;
      while (length < m_length) //m_length is set at the parent method invoking this
      {

          tlvList.push_back((::wire::icnx::tlvUtils::CreateTlvFromData(&data[length])));
          serialLength = (*(tlvList.rbegin()))->GetSerializedSize();
          length += serialLength;

      }
      if (length != m_length)  //something wrong, data should fit TLVs exactly
      {
          throw std::length_error("Tlv deserialized data corrupt");
      }      
    }


    void
    TLV_TLVDATA::XmlDataImplementation (pugi::xml_node &tlvXml)
    {
      tlvList.clear();

      pugi::xml_node tlvData = tlvXml.child("Data");

     //Iterate through each TLV in the data
    //for (pugi::xml_node_iterator tlv = tlvXml.begin(); tlv != tlvXml.end(); ++tlv)
    for (pugi::xml_node_iterator tlv = tlvData.begin(); tlv != tlvData.end(); ++tlv)
    {
      std::ostringstream os;
      tlv->print(os);
       TLV_BASE *tlvL = ((::wire::icnx::tlvUtils::CreateTlvFromXml(os.str())));
       tlvList.push_back(tlvL);
     }
    }


    void TLV_TLVDATA::CreateSerializedBuffer () const
    {
       //Since, by definition, we allow TLV's to be modified by their pointers, we must Serialize every time.
       //if (!m_dataChanged) 
       // return;                 
      m_dataChanged = false;
     if (NULL != m_serializedDataPtr)
        {
          delete[]m_serializedDataPtr;
          m_serializedDataPtr = NULL;
        }
      m_serializedDataPtr = new TLV_RAW_DATA_t[GetSerializedSize()];
            *(reinterpret_cast < TLV_TYPE_t * >(&m_serializedDataPtr[0])) =
        htons (m_type);
      *(reinterpret_cast < TLV_LENGTH_t * >
        (&m_serializedDataPtr[sizeof (TLV_TYPE_t)])) = htons (GetLength());
        int sSize = TLV_BASE_SIZE;
        for (std::list<TLV_BASE *>::const_iterator it = tlvList.begin(); it != tlvList.end(); it++)
          {
             (*it)->CreateSerializedBuffer();
             TLV_LENGTH_t length = (*it)->GetSerializedSize();
             memcpy(&m_serializedDataPtr[sSize], (*it)->NetworkSerialize(), length);
             sSize += length;
          }

    }

    void 
      TLV_TLVDATA::CopyFromTlv (const TLV_TLVDATA & tlv)
    {
      m_dataChanged = true;
      m_type = tlv.GetType ();
      std::list <TLV_BASE *> copy;
      tlv.GetTlvDataLinkedList(copy, ALL_TLVS);
      SetTlvData(copy);
    }

    TLV_TLVDATA & 
      TLV_TLVDATA::operator = (const TLV_TLVDATA & other)
    {
      if (this == &other) return *this;   // Gracefully handle self assignment
      CopyFromTlv (other);
      return *this;      
    }
    TLV_BASE * 
    TLV_TLVDATA::Copy() const
    {
         return new TLV_TLVDATA( *this); 
    }
 const std::string 
 TLV_TLVDATA::tlvIdName(void) const { return std::string("TLV_TLVDATA"); }

 const std::string 
 TLV_TLVDATA::tlvBaseName(void) const { return std::string("TLV_TLVDATA"); }
  }                             // namespace ccn
}                               // namespace ns3

//visual inspected self test
//only used for debugging
#ifdef _SELF_TEST_BASE_TLVDATA_
#include <streambuf>
#include <ostream>
#include <iostream>
#include <list>
#include "tlv_rawdata.h"
#include "tlv_endiandata.h"
#include "tlv_container.h"
#include "tlv_base.h"
#include "ccn-1.0_tlv-id.h"
#include "tlv_factory.h"

using namespace std;

int
main ()
{

  cout << "Self test Base(TlvData)" << endl;
  wire::icnx::TLV_TLVDATA a;
  cout << endl << endl << "Hex A()" << endl;
  a.XmlPrint (std::cout);
  a.CreateSerializedBuffer ();

  const uint8_t *var =
    reinterpret_cast < const uint8_t * >("ABCDEFGHIJKLMNOPQRSTUVXYZ");
  const wire::icnx::TLV_RAWDATA *c = new wire::icnx::TLV_RAWDATA (wire::icnx::tlvUtils::GetNextRawDataType(), 26, var);
  wire::icnx::TLV_RAWDATA *c1 = new wire::icnx::TLV_RAWDATA (wire::icnx::tlvUtils::GetNextRawDataType(), 26, var); 
  wire::icnx::TLV_ENDIANDATA * c3 = new wire::icnx::TLV_ENDIANDATA(wire::icnx::tlvUtils::GetNextEndianDataType(),0xffeeddee); 
  wire::icnx::TLV_TYPE_t id;
  wire::icnx::TLV_TLVDATA *b= new wire::icnx::TLV_TLVDATA();
  {
    id = wire::icnx::tlvUtils::GetNextRawDataType();
    wire::icnx::TLV_RAWDATA *c2 = new wire::icnx::TLV_RAWDATA(id, 26, var); 

    b->SetData(new wire::icnx::TLV_RAWDATA(*c));
    b->AppendTlvData(new wire::icnx::TLV_RAWDATA(*c1));
    b->AppendTlvData(new wire::icnx::TLV_RAWDATA(id, 26, var));
    b->AppendTlvData(c2);
    b->AppendTlvData(c3);
  }
  std::list<wire::icnx::TLV_BASE *> copy;
  b->GetTlvDataLinkedList(copy, id);
  //delete c3;
  cout << "\nHex B(B - 5 TLVs)" << endl;
  b->XmlPrint (std::cout);

  wire::icnx::TLV_TLVDATA *e = new wire::icnx::TLV_TLVDATA(*b);
  wire::icnx::TLV_TLVDATA *e2 = new wire::icnx::TLV_TLVDATA();
  wire::icnx::TLV_TLVDATA *e3 = new wire::icnx::TLV_TLVDATA();
  *e3 = *b;
  e2->SetData(b);
  cout << "\nE(B) " << endl;
  e->XmlPrint(std::cout);
  cout << "\nE2() has B for data " << endl;
  e2->XmlPrint(std::cout);
  cout << "\nE3() = B " << endl;
  e3->XmlPrint(std::cout);

  wire::icnx::TLV_TLVDATA *d = new wire::icnx::TLV_TLVDATA();
  d->SetTlvData(copy);


  cout << "B should pass 2 TLVs, passed " << copy.size() << endl;
  b->DeleteTlvData(id);

  cout << "B should have deleted 2 TLV(type " << id << "), have 3 TLVs "  << endl;
  cout << "B has type " << id << " = false (" << b->ExistTlvData(id) << ")" << endl;
  cout << "B has type " << c3->GetType() << " = true (" << b->ExistTlvData(c3->GetType()) << ")" << endl;

  b->XmlPrint (std::cout);

  
  const wire::icnx::TLV_BASE * c3b = b->GetFirstTlvData(c3->GetType());
  cout << "B should pass 2 TLVs, passed " << copy.size() << endl;
  b->DeleteTlvData(id);

  cout << "B2 should have a single Endian TLV return:"  << endl;

  c3b->XmlPrint (std::cout);

  cout << endl << endl << "Hex D(b->2 TLVs)" << endl;
  //d->CreateSerializedBuffer ();  
  d->XmlPrint (std::cout);

  cout << endl << endl << "Hex E(tlv B) with 5 objects" << endl;
  //e->CreateSerializedBuffer ();
  e->XmlPrint (std::cout);

  cout << endl << endl << "Hex G1(d->serialize with 2 objects" << endl;
/*
   std::cout << " d(" << d->GetType() << "," << d->GetLength() << "):\n";
const unsigned char *j = d->NetworkSerialize();
for(int i=0; i<d->GetLength(); i++)
{
  std::cout << std::hex << std::setw(2) << std::setfill('0') << static_cast<int> (j[i]);
}
std::cout << endl;
*/
  wire::icnx::TLV_TLVDATA *g1 = new wire::icnx::TLV_TLVDATA (d->NetworkSerialize ());
  g1->XmlPrint (std::cout);

  wire::icnx::TLV_TLVDATA *g2 = new wire::icnx::TLV_TLVDATA(d->XmlPrint ());
  cout << endl << endl << "Hex G1(d->xml with 2 objects" << endl;
  g2->XmlPrint (std::cout);


  cout << endl << "next step" << endl;
  wire::icnx::TLV_TLVDATA *g = new wire::icnx::TLV_TLVDATA (e->NetworkSerialize ());
  wire::icnx::TLV_TLVDATA *h = new wire::icnx::TLV_TLVDATA(e->XmlPrint ());
  cout << "delete e" << endl;
  delete e;
  cout << "Hex G(e->serialize) with 5 objects" << endl;

  g->XmlPrint (std::cout);
 
  cout << "constructors seem to work" << endl;


//e->XmlPrint(std::cout);
  cout << endl << endl << "Hex H(e->xmlprint) with 5 objects" << endl;

  h->XmlPrint (std::cout);

 

  //verify change
  uint8_t array[] = { 10, 11, 12, 13, 14, 15, 16, 17, 18, 19 };
  wire::icnx::TLV_RAWDATA * l = new wire::icnx::TLV_RAWDATA (5, 10, array);
  wire::icnx::TLV_RAWDATA * m = new wire::icnx::TLV_RAWDATA (*l);
  cout << endl << endl << "Deep copy verify" << endl;

  l->SetType (7);

  uint8_t array2[] = "ABCDE";
  l->SetData (array2, 5);
  l->XmlPrint (std::cout);
  delete l;
  m->XmlPrint (std::cout);


}

#endif

