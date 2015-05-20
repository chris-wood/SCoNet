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


#include "tlv_rawdata.h"

namespace wire
{
  namespace icnx
  {

    TLV_RAWDATA::TLV_RAWDATA ()
    {
      m_DataPtr = NULL;
      m_type = GENERIC_TLV_RAWDATA;
    }

    
    TLV_RAWDATA::~TLV_RAWDATA ()
    {


    }

    TLV_RAWDATA::TLV_RAWDATA (const TLV_RAWDATA & tlv_msg)
    {
      m_DataPtr = NULL;
      CopyFromTlv (tlv_msg);
    }

    TLV_RAWDATA::TLV_RAWDATA (const TLV_RAW_DATA_t * rawData)
    {
      m_DataPtr = NULL;
      NetworkDeserialize (rawData);
    }

    TLV_RAWDATA::TLV_RAWDATA (TLV_TYPE_t type, TLV_LENGTH_t length,
                              const TLV_RAW_DATA_t * data)
    {
      m_DataPtr = NULL;
      m_type = type;
      SetData (data, length);

    }

    TLV_RAWDATA::TLV_RAWDATA (const std::string & xmlString)
    {
      m_DataPtr = NULL;
      XmlImplement (xmlString);
    }


    const TLV_RAW_DATA_t *
    TLV_RAWDATA::GetData () const
    {
      return m_DataPtr;
    }


    TLV_RAW_DATA_t *
    TLV_RAWDATA::GetCopyData () const
    {
      TLV_RAW_DATA_t *returnCopy = new TLV_RAW_DATA_t [GetLength()];
      memcpy(returnCopy, m_DataPtr, GetLength());
      return returnCopy;
    }


    std::string 
    TLV_RAWDATA::GetStrData () const
    {
      std::string strData;
      if (0 != m_length)
        {
          strData.resize (m_length);
          memcpy (&strData[0], m_DataPtr, m_length);
        }
      return strData;
    }

    template < class TLV_DATA_OBJECT > void
      TLV_RAWDATA::GetObjectData (TLV_DATA_OBJECT & obj, bool shallowCopy)
    {
      if (shallowCopy)
        {
          memcpy (&obj, m_DataPtr, m_length);
        }
      else
        {
          std::istringstream is;
          std::string buffer = GetStrData();
          is.str (buffer);
          is >> obj;

        }
    }

    void TLV_RAWDATA::SetData (const TLV_RAW_DATA_t * data, TLV_LENGTH_t size)
    {
      m_dataChanged = true;
      if (NULL != m_DataPtr)
        {
          delete[]m_DataPtr;
          m_DataPtr = NULL;
        }
      m_length = size;
      if (0 == size)
        {
          return;
        }
      m_DataPtr = new TLV_RAW_DATA_t[m_length];
      memcpy (m_DataPtr, data, m_length);
    }


    void TLV_RAWDATA::SetData (const std::string & strData)
    {
      m_dataChanged = true;
      if (NULL != m_DataPtr)
        {
          delete[]m_DataPtr;
          m_DataPtr = NULL;
        }
      if (strData.empty ())
        {
          m_length = 0;
          return;
        }

      m_length = strData.size () + 1;
      m_DataPtr = new TLV_RAW_DATA_t[m_length];
      memcpy (m_DataPtr, strData.c_str (), strData.size ());
      m_DataPtr[m_length] = '\0'; //Null terminate string
    }

    template < class TLV_DATA_OBJECT > void
      TLV_RAWDATA::SetObjectData (const TLV_DATA_OBJECT & obj,
                                  bool shallowCopy)
    {
      m_dataChanged = true;
      if (NULL != m_DataPtr)
        {
          delete[]m_DataPtr;
          m_DataPtr = NULL;
        }
      if (shallowCopy)
        {
          m_length = sizeof (obj);
          m_DataPtr = new TLV_RAW_DATA_t[m_length];
          memcpy (m_DataPtr, &obj, m_length);
        }
      else
        {
          std::ostringstream os;
          os << obj << std::flush;
          SetData (os.str ());

        }

    }


    void
    TLV_RAWDATA::NetworkDataDeserialize (const TLV_RAW_DATA_t * data)
    {
      if (NULL != m_DataPtr)
        {
          delete[]m_DataPtr;
        }
      m_DataPtr = new TLV_RAW_DATA_t[m_length];
      memcpy (m_DataPtr, data, m_length);
    }

    void
    TLV_RAWDATA::XmlDataImplementation(pugi::xml_node &tlvXml)
    {
      if (NULL != m_DataPtr)
        {
          delete[]m_DataPtr;
        }
      m_DataPtr = new TLV_RAW_DATA_t[m_length];

      std::string buffer = std::string (tlvXml.child ("Data").text ().get ());
      //should be even in size
      if ((buffer.length () % 2) || (buffer.length () != (m_length) * 2))
        {
          throw
            std::
            length_error ("Xml Buffer length does not correspond with size!");
        }
      //convert ascii hex to binary format
      for (unsigned int i = 0; i < buffer.length (); i += 2)
        {
          std::string byte = buffer.substr (i, 2);
          uint8_t bit8 =
            static_cast < uint8_t > (strtoul (byte.c_str (), NULL, 16));
          m_DataPtr[i / 2] = bit8;
        }
    }


    void TLV_RAWDATA::XmlPrintData (std::ostream & os, int numspace) const
    {
      std::ios::fmtflags saveFlags = os.flags();
      
      std::string space(numspace, ' ');
      std::string space2(numspace+1, ' ');
      os << space2 << "<Data>";
      for (unsigned int i = 0; i < GetLength(); i++)
        {
          os << std::setw (2) << std::setfill ('0') << std::
            hex << static_cast < int >(m_DataPtr[i]);
        }
      os << "</Data>\n";
      os << space << "</TLV>\n";
        os.flags(saveFlags);

    }


    void TLV_RAWDATA::CreateSerializedBuffer () const
    {
      if (!m_dataChanged)
        return;                 //no change, use same serialized buffer
      m_dataChanged = false;
      if (NULL != m_serializedDataPtr)
        {
          delete[]m_serializedDataPtr;
          m_serializedDataPtr = NULL;
        }
      m_serializedDataPtr = new TLV_RAW_DATA_t[GetSerializedSize ()];

      *(reinterpret_cast < TLV_TYPE_t * >(&m_serializedDataPtr[0])) =
        htons (m_type);
      *(reinterpret_cast < TLV_LENGTH_t * >
        (&m_serializedDataPtr[sizeof (TLV_TYPE_t)])) = htons (m_length);
      if (m_length)
        {
          memcpy (&m_serializedDataPtr[TLV_BASE_SIZE], m_DataPtr,
                  m_length);
        }
    }

    void TLV_RAWDATA::CopyFromTlv (const TLV_RAWDATA & tlv)
    {
      m_dataChanged = true;
      m_type = tlv.GetType ();
      SetData (tlv.GetData (), tlv.GetLength ());
    }

  TLV_RAWDATA &
  TLV_RAWDATA::operator = (const TLV_RAWDATA &other) 
{
  if (this == &other) return *this;   // Gracefully handle self assignment
   m_dataChanged = true;
   CopyFromTlv(other);
   return *this;
}

    TLV_BASE * 
    TLV_RAWDATA::Copy() const
    {
         return new TLV_RAWDATA( *this); 
    }

 const std::string 
 TLV_RAWDATA::tlvIdName(void) const { return std::string("TLV_RAWDATA"); }

 const std::string 
 TLV_RAWDATA::tlvBaseName(void) const { return std::string("TLV_RAWDATA"); }
 
  }                             // namespace ccn
}                               // namespace ns3


//visual inspected self test
//only used for debugging
#ifdef _SELF_TEST_BASE_RAWDATA_
#include <streambuf>
#include <ostream>
#include <iostream>
#include "tlv_rawdata.h"
using namespace std;

int
main ()
{
  cout << "Self test Base(RawData)" << endl;
  wire::icnx::TLV_RAWDATA a;
  const uint8_t *var =
    reinterpret_cast < const uint8_t * >("ABCDEFGHIJKLMNOPQRSTUVXYZ");
  wire::icnx::TLV_RAWDATA b (257, 26, var); //add 1 for NULL for strings
  wire::icnx::TLV_RAWDATA c (b);

  wire::icnx::TLV_RAWDATA d (b.NetworkSerialize ());
  wire::icnx::TLV_RAWDATA e (b.XmlPrint ());

  wire::icnx::TLV_RAWDATA f;
  f.SetData ("0123456789");
  wire::icnx::TLV_RAWDATA g;
  g = f;

  cout << "constructors seem to work" << endl;

  wire::icnx::TLV_BASE zz = b;
  //zz.CreateSerializedBuffer();
  //zz.XmlPrint(std::cout);

  //verify constructors
  cout << endl << endl << "Hex A()" << endl;
  //a.CreateSerializedBuffer ();
  a.XmlPrint (std::cout, 4);

  cout << endl << endl << "Hex B(257,27, A-Z)" << endl;
  //b.CreateSerializedBuffer ();
  b.XmlPrint (std::cout, 4);


  cout << endl << endl << "Hex C(b)" << c.tlvIdName() << endl;
  //c.CreateSerializedBuffer ();
  c.XmlPrint (std::cout, 4);


  cout << endl << endl << "Hex D(via b.Serialize)" << endl;
  //d.CreateSerializedBuffer ();
  d.XmlPrint (std::cout, 4);


  cout << endl << endl << "Hex E(via b.Xml)" << endl;
  //e.XmlPrint (std::cout);
  //e.CreateSerializedBuffer ();
  e.XmlPrint (std::cout, 4);

  cout << endl << endl << "Hex F()" << endl;
  //f.CreateSerializedBuffer ();
  f.XmlPrint (std::cout, 4);

  cout << endl << endl << "Hex G(g=f)\n" << g << endl;
  //g.CreateSerializedBuffer ();
  //g.XmlPrint (std::cout, 4);

  //template test
  double rawt = 3.141596;
  double value;
  wire::icnx::TLV_RAWDATA k;
  k = g;
  k.SetType(5);
  g.SetObjectData (rawt, true);
  rawt *= 10;
  k.SetObjectData (rawt, false);

  cout << endl << endl << "Hex G(double 3.141596, true-shallowcopy)" << endl;
  //g.CreateSerializedBuffer ();
  g.XmlPrint (std::cout, 4);
  g.GetObjectData (value, true);
  cout << "3.141596 (true); read back " << value << endl;

  cout << endl << endl << "Hex K(double 3.141596, false-redirection)" << endl;
  //k.CreateSerializedBuffer ();
  k.XmlPrint (std::cout, 4);
  k.GetObjectData (value, false);
  cout << "31.41596 (false); read back " << value << endl;

  //verify change
  uint8_t array[] = { 10, 11, 12, 13, 14, 15, 16, 17, 18, 19 };
  wire::icnx::TLV_RAWDATA * h = new wire::icnx::TLV_RAWDATA (5, 10, array);
  wire::icnx::TLV_RAWDATA * j = new wire::icnx::TLV_RAWDATA (*h);
  wire::icnx::TLV_RAWDATA * h2 = h;
  cout << endl << endl << "Deep copy verify" << endl;

  h->SetType (7);

  uint8_t array2[] = "ABCDE";
  h->SetData (array2, 5);
  //h->CreateSerializedBuffer ();
  h->XmlPrint (std::cout, 4);
  h2->XmlPrint (std::cout, 4);
  delete h;
  //j->CreateSerializedBuffer ();
  j->XmlPrint (std::cout, 4);

}

#endif
