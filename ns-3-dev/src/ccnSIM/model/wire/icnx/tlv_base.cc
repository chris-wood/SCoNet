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


#include "tlv_base.h"

namespace wire {
namespace icnx {

TLV_BASE::TLV_BASE ()
  : m_type(INVALID_TLV)
  , m_length(0)
  , m_serializedDataSize(TLV_BASE_SIZE)
  , m_serializedDataPtr(NULL)
  , m_dataChanged(true)
{

}

TLV_BASE::TLV_BASE (const TLV_BASE &tlv)
  : m_type(tlv.GetType())
  , m_length(tlv.m_length)
  , m_serializedDataSize(TLV_BASE_SIZE)
  , m_serializedDataPtr(NULL)
  , m_dataChanged(true)
{

}

TLV_BASE::TLV_BASE (const std::string & xmlString)
    { 
      m_dataChanged = true;
      m_serializedDataPtr = NULL;
      XmlImplement(xmlString);
    }

TLV_BASE::~TLV_BASE()
{
      if (NULL != m_serializedDataPtr)
        {
          delete[]m_serializedDataPtr;
        }  
}


void
TLV_BASE::CopyFromTlv(const TLV_BASE &other)
{
  m_length=other.GetLength();
  m_type=other.GetType();
  m_serializedDataSize=TLV_BASE_SIZE;
  m_serializedDataPtr=NULL;
  m_dataChanged=true;

}

  void
  TLV_BASE::SetType (TLV_TYPE_t type)
{
   m_type = type;
   m_dataChanged=true;

}

  TLV_TYPE_t
  TLV_BASE::GetType () const
{
   return m_type;
}

  TLV_LENGTH_t
  TLV_BASE::GetLength () const
{
   return m_length;
}

  TLV_LENGTH_t
  TLV_BASE::GetSerializedSize () const
{
   return TLV_BASE_SIZE+GetLength();
}

void
TLV_BASE::SetData(const TLV_RAW_DATA_t *data, TLV_LENGTH_t size)
{
   assert(0); //JLM FIXME TODO assert
}

const TLV_RAW_DATA_t *
TLV_BASE::GetData() const
{  
  return NULL;
}
void
TLV_BASE::NetworkDataDeserialize(const TLV_RAW_DATA_t *data)
{
   assert(0);
}

 void
TLV_BASE::XmlPrint (std::ostream &os, int numspace) const
{
  XmlPrintHeader(os, numspace);
  XmlPrintData(os, numspace);
}

std::string 
TLV_BASE::XmlPrint () const
{
      std::ostringstream sout;
      XmlPrint (sout);
      return sout.str ();
}
 const std::string 
 TLV_BASE::tlvIdName(void) const { return std::string("TLV_BASE"); }

 const std::string 
 TLV_BASE::tlvBaseName(void) const { return std::string("TLV_BASE"); }

void
TLV_BASE::XmlPrintHeader(std::ostream &os, int numspace) const
{
      std::ios::fmtflags saveFlags = os.flags();
      std::string space(numspace, ' ');
      std::string space2(numspace+1, ' ');
 
  os << space << "<TLV name=\"" << tlvIdName () << "\" >\n";
  os << space2 << "<Type>" << std::dec << GetType() << "</Type>\n";
  os << space2 << "<Length>" << std::dec << GetLength() << "</Length>\n";
        os.flags(saveFlags);
  }

void
TLV_BASE::XmlPrintData(std::ostream &os, int numspace) const
{
      std::ios::fmtflags saveFlags = os.flags();
      std::string space(numspace, ' ');
      std::string space2(numspace+1, ' ');  
      os << space2 << "<Data></Data>\n";
  os << space << "/TLV>\n";
          os.flags(saveFlags);
  assert(0);

}

void 
TLV_BASE::Print(std::ostream &os, int numspace) const
{
    XmlPrint(os, numspace);
}

void TLV_BASE::CreateSerializedBuffer () const
    {
       if (!m_dataChanged)
        return;                 //no change, use same serialized buffer
      m_dataChanged = false;
     if (NULL != m_serializedDataPtr)
        {
          delete[]m_serializedDataPtr;
          m_serializedDataPtr = NULL;
        }
      m_serializedDataPtr = new TLV_RAW_DATA_t[TLV_BASE_SIZE];
      *(reinterpret_cast < TLV_TYPE_t * >(&m_serializedDataPtr[0])) =
        htons (m_type);
      m_serializedDataPtr[2] = 0;
      m_serializedDataPtr[3] = 0;
      //no data, size of 0, always
      assert(0);
    }


  const TLV_RAW_DATA_t *
  TLV_BASE::NetworkSerialize () const
{
      CreateSerializedBuffer ();
      return m_serializedDataPtr;

}


    TLV_BASE *
    TLV_BASE::XmlImplement (const std::string &xmlString)
    {
      std::istringstream sin (xmlString);
      return XmlImplement (sin);
    }

  void
  TLV_BASE::XmlDataImplementation(pugi::xml_node &tlvXml)
{ 
  assert(0); // shouldnt be called here 
}

  TLV_BASE *
  TLV_BASE::XmlImplement( std::istream &iss)
    {
      m_dataChanged = true;
      pugi::xml_document doc;
      doc.load (iss);

      pugi::xml_node tlvXml = doc.child ("TLV");

      m_type = tlvXml.child ("Type").text ().as_uint ();
      m_length = tlvXml.child ("Length").text ().as_uint ();

      XmlDataImplementation(tlvXml);
      return this;
    }

  TLV_BASE *
  TLV_BASE::NetworkDeserialize(const TLV_RAW_DATA_t *data) 
  {
       m_dataChanged = true;
      m_type = ntohs (*(reinterpret_cast < const TLV_TYPE_t * >(data)));
      m_length =
        ntohs (*(reinterpret_cast < const TLV_LENGTH_t * >
                 (&data[sizeof (TLV_TYPE_t)])));
      if (0 != m_length)
        {
        NetworkDataDeserialize(&data[TLV_BASE_SIZE]);
        }
      return this;

  }


  TLV_BASE &
  TLV_BASE::operator = (const TLV_BASE &other) 
{
  if (this == &other) return *this;   // Gracefully handle self assignment
  m_dataChanged = true;
   CopyFromTlv(other);
   return *this;
}

    TLV_BASE * 
    TLV_BASE::Copy() const
    {
         return new TLV_BASE( *this); 
    }

std::ostream &
operator << (std::ostream &os, const TLV_BASE &i) { 
  i.XmlPrint(os);
  return os;
}

std::istream &
operator >> (std::istream &iss, TLV_BASE &i)
{
  i.XmlImplement(iss);
  return iss;
}

} // namespace icnx
} // namespace wire

