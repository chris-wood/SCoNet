/* -*- Mode: C++{} c-file-style: "gnu"{} indent-tabs-mode:nil -*- */
/*
 * Copyright (c) 2014, 2015 University of California, Santa Cruz
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


#include "tlv_endiandata.h"


namespace wire
{
  namespace icnx
  {


    TLV_ENDIANDATA::TLV_ENDIANDATA ()
    {
      m_length = 1;
      m_endianData = 0;
      m_type = GENERIC_TLV_ENDIANDATA;
      m_serializedDataPtr = NULL;
      m_dataChanged = true;

    }

    TLV_ENDIANDATA::TLV_ENDIANDATA (const TLV_ENDIANDATA & tlv_msg)
    {
      m_serializedDataPtr = NULL;
      CopyFromTlv (tlv_msg);
    }

    TLV_ENDIANDATA::TLV_ENDIANDATA (const std::string &xmlString)
    {
      m_serializedDataPtr = NULL;
      XmlImplement(xmlString);
    }

    TLV_ENDIANDATA::TLV_ENDIANDATA (const TLV_RAW_DATA_t * rawData)
    {
      m_serializedDataPtr = NULL;
      NetworkDeserialize (rawData);
    }


    TLV_ENDIANDATA::TLV_ENDIANDATA (TLV_TYPE_t type, TLV_LENGTH_t length,
                                    const TLV_RAW_DATA_t * data)
    {
      m_type = type;
      m_serializedDataPtr = NULL;

      //validate length and data
      SetData (data, length);
    }

    TLV_ENDIANDATA::TLV_ENDIANDATA (TLV_TYPE_t type, uint64_t data)
    {
      m_type = type;
      m_serializedDataPtr = NULL;
      m_endianData = data;
      m_length = GetLength();
    }

    TLV_ENDIANDATA::~TLV_ENDIANDATA ()
    {

    }

    TLV_LENGTH_t TLV_ENDIANDATA::GetLength () const
    {
      if (m_endianData <= UINT8_MAX)
        {
          return sizeof (uint8_t);
        }
      else if (m_endianData <= UINT16_MAX)
        {
          return sizeof (uint16_t);
        }
      else if (m_endianData <= UINT32_MAX)
        {
          return sizeof (uint32_t);
        }
      else if (m_endianData <= UINT64_MAX)
        {
          return sizeof (uint64_t);
        }
      else
        {
          throw
            std::out_of_range ("Endian data greater than 64bits, invalid!");
        }

    }

    const TLV_RAW_DATA_t *
      TLV_ENDIANDATA::GetRawData (bool networkEndianFormat) const
    {
      switch (GetLength ())
        {
          case sizeof (uint8_t):
            u8data = static_cast < uint8_t > (m_endianData);
            return reinterpret_cast < TLV_RAW_DATA_t * >(&u8data);
            break;
          case sizeof (uint16_t):
            u16data = static_cast < uint16_t > (m_endianData);
            if (networkEndianFormat)
            {
                u16data = htons(u16data);
            }
            return reinterpret_cast < TLV_RAW_DATA_t * >(&u16data);
            break;
          case sizeof (uint32_t):
            u32data = static_cast < uint32_t > (m_endianData);
            if (networkEndianFormat)
            {
                u32data = htonl(u32data);
            }
            return reinterpret_cast < TLV_RAW_DATA_t * >(&u32data);
            break;
          case sizeof (uint64_t):
            if (networkEndianFormat)
            {
                u32endiandata[0] = htonl(static_cast<uint32_t>(m_endianData >> 32));
                u32endiandata[1] = htonl(static_cast<uint32_t>(m_endianData&0xffffffff));
                return reinterpret_cast <TLV_RAW_DATA_t *>(u32endiandata);
            } else
            {
              u64data = m_endianData;
              return reinterpret_cast < TLV_RAW_DATA_t * >(&u64data);
            }

            break;
          default:throw std::out_of_range
              ("Invalid internal data.  Not of size 8/16/32/64bits!");
        }

    }

    const TLV_RAW_DATA_t *
      TLV_ENDIANDATA::GetData (bool networkEndianFormat) const
    {
      return GetRawData (networkEndianFormat);
    }

    void 
      TLV_ENDIANDATA::GetNumericData (uint8_t & data) const
    {
      data = static_cast < uint8_t > (m_endianData);
    }
    void 
      TLV_ENDIANDATA::GetNumericData (uint16_t & data) const
    {
      data = static_cast < uint16_t > (m_endianData);
    }
    void 
      TLV_ENDIANDATA::GetNumericData (uint32_t & data) const
    {
      data = static_cast < uint32_t > (m_endianData);
    }
    void 
      TLV_ENDIANDATA::GetNumericData (uint64_t & data) const
    {
      data = m_endianData;
    }

    void
      TLV_ENDIANDATA::SetData (const TLV_RAW_DATA_t * data, TLV_LENGTH_t size)
    {
      m_dataChanged = true;
      if (size > sizeof (m_endianData))
        {
          throw
            std::out_of_range ("Endian data greater than 64bits, invalid!");
        }
      switch (size)
        {
          case sizeof (uint8_t):
            m_endianData = *(reinterpret_cast < const uint8_t * >(data));
            break;
          case sizeof (uint16_t):
            m_endianData = *(reinterpret_cast < const uint16_t * >(data));
            break;
          case sizeof (uint32_t):
            m_endianData = *(reinterpret_cast < const uint32_t * >(data));
            break;
          case sizeof (uint64_t):
            m_endianData = *(reinterpret_cast < const uint64_t * >(data));
            break;
          default:
            throw
              std::
              out_of_range ("Only data of size 8/16/32/64bits is allowed!");
        }
      m_length = GetLength ();
    }

    void TLV_ENDIANDATA::SetData (uint8_t bit8Data)
    {
      m_dataChanged = true;
      m_endianData = bit8Data;
    }
    void TLV_ENDIANDATA::SetNumericData (uint8_t bit8Data)
    {
      m_dataChanged = true;
      m_endianData = bit8Data;
    }

    void TLV_ENDIANDATA::SetData (uint16_t bit16Data)
    {
      m_dataChanged = true;
      m_endianData = bit16Data;
    }
    void TLV_ENDIANDATA::SetNumericData (uint16_t bit16Data)
    {
      m_dataChanged = true;
      m_endianData = bit16Data;
    }

    void TLV_ENDIANDATA::SetData (uint32_t bit32Data)
    {
      m_dataChanged = true;
      m_endianData = bit32Data;
    }
    void TLV_ENDIANDATA::SetNumericData (uint32_t bit32Data)
    {
      m_dataChanged = true;
      m_endianData = bit32Data;
    }

    void TLV_ENDIANDATA::SetData (uint64_t bit64Data)
    {
      m_dataChanged = true;
      m_endianData = bit64Data;
    }
    void TLV_ENDIANDATA::SetNumericData (uint64_t bit64Data)
    {
      m_dataChanged = true;
      m_endianData = bit64Data;
    }

      void
      TLV_ENDIANDATA::NetworkDataDeserialize (const TLV_RAW_DATA_t * data)
    {
      m_dataChanged = true;
      if (sizeof (uint8_t) == m_length)
        {
          m_endianData =
            static_cast < uint8_t >
            (*data);
        }
      else if (sizeof (uint16_t) == m_length)
        {
          m_endianData =
            ntohs (* (reinterpret_cast < const uint16_t * > 
                    (data)));
        }
      else if (sizeof (uint32_t) == m_length)
        {
          m_endianData =
            ntohl (* (reinterpret_cast < const uint32_t *>
                    (data)));
        }
      else if (sizeof (uint64_t) == m_length)
        {
          //network format is big endian
          uint32_t high32 =
            ntohl (* (reinterpret_cast < const uint32_t * >
                    (data)));
          uint32_t low32 =
            ntohl (* (reinterpret_cast < const uint32_t * >
                    (&data [sizeof (uint32_t)])));
          m_endianData = (static_cast < uint64_t > (high32) << 32) + low32;
        }
      else
        {
          std::cout << "type is " << m_type << "\nsize is of " << m_length << "\n";
          throw
            std::out_of_range ("TLV_ENDIANDATA::NetworkDeserialize : Only data of size 8/16/32/64bits is allowed!");
        }
    }

    void TLV_ENDIANDATA::XmlPrintData (std::ostream & os, int numspace) const
    {
      std::ios::fmtflags saveFlags = os.flags();
      
      std::string space(numspace, ' ');
      std::string space2(numspace+1, ' ');

      os << space2 << "<Data>";
      os << std::setw(GetLength()*2)<<std::setfill('0')<<std::hex<< m_endianData;
      os << "</Data>\n";
      os << space << "</TLV>\n";
        os.flags(saveFlags);

    }


    
    void TLV_ENDIANDATA::CreateSerializedBuffer () const
    {
      if (!m_dataChanged)
        return;                 //no change, use same serialized buffer
      m_dataChanged = false;
      if (NULL != m_serializedDataPtr)
        {
          delete[]m_serializedDataPtr;
          m_serializedDataPtr = NULL;
        }
      TLV_LENGTH_t length = GetLength ();
      m_serializedDataPtr = new TLV_RAW_DATA_t[GetSerializedSize()];

      *(reinterpret_cast < TLV_TYPE_t * >(&m_serializedDataPtr[0])) =
        htons (m_type);
      *(reinterpret_cast < TLV_LENGTH_t * > 
        (&m_serializedDataPtr[sizeof (TLV_TYPE_t)])) =
        htons (length);
      memcpy (&m_serializedDataPtr[TLV_BASE_SIZE], GetRawData (true), length);


    }

   void 
    TLV_ENDIANDATA::XmlDataImplementation (pugi::xml_node &tlvXml)
    {

      m_endianData = strtoull(tlvXml.child("Data").text().get(), NULL, 16);
    }

    void 
    TLV_ENDIANDATA::CopyFromTlv (const TLV_ENDIANDATA & tlv)
    {
      m_dataChanged = true;
      tlv.GetNumericData (m_endianData);
      m_length = tlv.GetLength ();
      m_type = tlv.GetType ();
      m_serializedDataPtr = NULL;
    }


    TLV_ENDIANDATA & 
    TLV_ENDIANDATA::operator = (const TLV_ENDIANDATA & other)
    {
      if (this == &other) return *this;   // Gracefully handle self assignment
      CopyFromTlv(other);
      return *this;
    }

    TLV_BASE * 
    TLV_ENDIANDATA::Copy() const
    {
         return new TLV_ENDIANDATA( *this); 
    }
    
 const std::string 
 TLV_ENDIANDATA::tlvIdName(void) const { return std::string("TLV_ENDIANDATA"); }

 const std::string 
 TLV_ENDIANDATA::tlvBaseName(void) const { return std::string("TLV_ENDIANDATA"); }

  }                             // namespace ccn
}                               // namespace ns3





//visual inspected self test
//only used for debugging
#ifdef _SELF_TEST_BASE_ENDIANDATA_
#include <streambuf>
#include <ostream>
#include <iostream>
#include "tlv_endiandata.h"
using namespace std;
int main()
{
   cout << "Self test Base(EndianData)" << endl;
   unsigned char data='A';
  

   cout << "constructors seem to work" << endl;

   //verify constructors
   wire::icnx::TLV_ENDIANDATA a;
   cout << endl << endl << "Hex A()" << endl;
   //a.CreateSerializedBuffer();
   a.XmlPrint(std::cout);


   cout << endl << endl << "Hex B()" << endl;
   wire::icnx::TLV_ENDIANDATA b(6, (uint64_t) 0xffeeddccbbaa9988);
   //b.CreateSerializedBuffer();
   b.XmlPrint(std::cout);

   cout << endl << endl << "Hex E(via b.Xml)" << endl;
   wire::icnx::TLV_ENDIANDATA e(b.XmlPrint());
   //e.CreateSerializedBuffer();
   e.XmlPrint(std::cout);
   
   cout << endl << endl << "Hex C(b)" << endl;
   wire::icnx::TLV_ENDIANDATA c(b);
   //c.CreateSerializedBuffer();
   c.XmlPrint(std::cout);

  cout << endl << endl << "Hex A(a=e)" << endl;
   a = e;
  //a.CreateSerializedBuffer();
  cout << a.XmlPrint() << endl;
   
   cout << endl << endl << "Hex D(via b.Serialize)" << endl;
   b.XmlPrint(std::cout);
   wire::icnx::TLV_ENDIANDATA d(b.NetworkSerialize());
   //d.CreateSerializedBuffer();
   d.XmlPrint(std::cout);
   b.XmlPrint(std::cout);

      cout << endl << endl << "Hex D(broken via via b.Serialize)" << endl;
   b.XmlPrint(std::cout);
   wire::icnx::TLV_ENDIANDATA d2(b.NetworkSerialize());
   d2.CreateSerializedBuffer();
   d2.XmlPrint(std::cout);
   b.XmlPrint(std::cout);

   cout << endl << endl << "Hex F(4, 'A')" << endl;
   wire::icnx::TLV_ENDIANDATA f(4, sizeof(char), &data);
   f.XmlPrint(std::cout);
   //f.CreateSerializedBuffer();

   cout << endl << endl << "Hex G(3, 0x11223344)" << endl;
   wire::icnx::TLV_ENDIANDATA g(3, 0x11223344);
   //g.CreateSerializedBuffer();
   g.XmlPrint(std::cout);

   cout << endl << endl << "Hex G2(via g.serialize)" << endl;
   wire::icnx::TLV_ENDIANDATA g2(g.NetworkSerialize());
   //g2.CreateSerializedBuffer();
   g2.XmlPrint(std::cout);


   //verify change
   data='B';
   a.SetData(data);
   a.SetType(15);
   cout << endl << endl << "Hex A(0xf:B)" << endl;
   a.CreateSerializedBuffer();
   a.XmlPrint(std::cout);
  
   wire::icnx::TLV_ENDIANDATA *h = new wire::icnx::TLV_ENDIANDATA(5, (uint16_t) 0x0403);
   wire::icnx::TLV_ENDIANDATA *j = new wire::icnx::TLV_ENDIANDATA(*h);
   cout << endl << endl << "Deep copy verify" << endl;

   h->SetType(7);
   uint16_t data16 = 0x0605;
   h->SetData(data16);
   h->CreateSerializedBuffer();
   h->XmlPrint(std::cout);
   delete h;
   j->CreateSerializedBuffer();
   j->XmlPrint(std::cout);

}

#endif


