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
#include "tlv_factory.h"


namespace wire {
namespace icnx {

class TLV_BASE;

namespace tlvUtils
{
  TLV_BASE *
  CreateTlvFromData (const TLV_RAW_DATA_t *rawData,bool guessIsXml)
  {
    if ((guessIsXml) && (strncmp(reinterpret_cast<const char *>(rawData), "<TLV", 4) == 0) )
    {
       //Probably an XML tlv
       return CreateTlvFromXml(std::string(reinterpret_cast<const char *>(rawData)));
    }

    TLV_TYPE_t type = ntohs (*(reinterpret_cast < const TLV_TYPE_t * >(rawData)));
    TLV_BASE *tlv = CreateTlvFromType(type);
    tlv->NetworkDeserialize(rawData);
    return (CreateTlvFromType(type))->NetworkDeserialize(rawData);
  }

  TLV_BASE *
  CreateTlvFromXml (const std::string &xml)
  {
      pugi::xml_document doc;
      doc.load (xml.c_str());

      pugi::xml_node tlvXml = doc.child ("TLV");
      TLV_TYPE_t type = tlvXml.child ("Type").text ().as_uint ();
      return CreateTlvFromType(type)->XmlImplement(xml);


  }

    TLV_BASE *
  CreateTlvFromXml (std::istream &is)
  {
      int size = is.gcount();  //how big is the input
      char *buffer = new char[size];
      is.get(buffer, size);   //copy the input
      std::string iss(buffer);
      delete [] buffer;
      return CreateTlvFromXml(iss);
  }

  TLV_BASE *
  CreateTlvFromType(wire::icnx::TLV_TYPE_t type)
  {
       switch (type)
      {
         case INTERESTRESP_RETURNCODE_TLV:
           return new ::wire::icnx::TLV_INTERESTRESP_RETURNCODE;
           break;
         case INTEREST_HOP_TLV:
           return new ::wire::icnx::TLV_INTERESTHOP;
           break;
         case INTLIFE_TLV:
           return new ::wire::icnx::TLV_INTLIFE;
           break;
         case PAYLOAD_TLV:
           return new ::wire::icnx::TLV_PAYLOAD;
           break;
         case NAME_TLV:
           return new ::wire::icnx::TLV_NAME;
           break;
         case NAMESEG_TLV:
           return new ::wire::icnx::TLV_NAMESEG;
           break;
         case MESG_TLV:
           return new ::wire::icnx::TLV_MESG;
           break;
         case EXPIRY_TLV:
           return new ::wire::icnx::TLV_EXPIRY;
           break;
         case CACHETIME_TLV:
           return new ::wire::icnx::TLV_CACHETIME;
           break;
         case IPIDM_TLV:
           return new ::wire::icnx::TLV_IPIDM;
           break;
         case PAYLDTYPE_TLV:
           return new ::wire::icnx::TLV_PAYLDTYPE;
           break;          
         case KEYIDRESTR_TLV:
           return new ::wire::icnx::TLV_KEYIDRESTR;
           break;   
         case FLOW_TLV:
           return new ::wire::icnx::TLV_FLOW;
           break;   
         case IPID_TLV:
           return new ::wire::icnx::TLV_IPID;
           break;              
         case PAD_TLV:
           return new ::wire::icnx::TLV_PAD;
           break;   
         case OBJHASHRESTR_TLV:
           return new ::wire::icnx::TLV_OBJHASHRESTR;
           break;     
         case VALIDATION_ALG_TLV:
           return new ::wire::icnx::TLV_VALIDATION_ALG;
           break;  
         case VALIDATION_ALGPAYLOAD_TLV:
           return new ::wire::icnx::TLV_VALIDATION_ALGPAYLOAD;
           break;  
         case CRC32_TLV:
           return new ::wire::icnx::TLV_CRC32;
           break;  
         case CRC32C_TLV:
           return new ::wire::icnx::TLV_CRC32C;
           break;             
         case RFC793_TLV:
           return new ::wire::icnx::TLV_RFC793;
           break;     
         case HMAC_SHA256_TLV:
           return new ::wire::icnx::TLV_HMAC_SHA256;
           break;  
         case VMAC128_TLV:
           return new ::wire::icnx::TLV_VMAC128;
           break;  
         case RSA_SHA256_TLV:
           return new ::wire::icnx::TLV_RSA_SHA256;
           break; 
         case EC_SECP256K1_TLV:
           return new ::wire::icnx::TLV_EC_SECP256K1;
           break;     
         case EC_SECP384R1_TLV:
           return new ::wire::icnx::TLV_EC_SECP384R1;
           break;  
         case KEYID_TLV:
           return new ::wire::icnx::TLV_KEYID;
           break;  
         case PUBLICKEY_TLV:
           return new ::wire::icnx::TLV_PUBLICKEY;
           break;             
         case PUBLICKEYLOC_TLV:
           return new ::wire::icnx::TLV_PUBLICKEYLOCATION;
           break;
         case CERT_TLV:
           return new ::wire::icnx::TLV_CERT;
           break;     
         case LINK_TLV:
           return new ::wire::icnx::TLV_LINK;
           break;  
         case KEYNAME_TLV:
           return new ::wire::icnx::TLV_KEYNAME;
           break;  
         case SIGTIME_TLV:
           return new ::wire::icnx::TLV_SIGTIME;
           break;   
         default: //undefined or is a ranged-value or version-value defined TLV
           //name app check
           if (type >= APP_TLV && type <=APP_TLV_END )
           {
              return new ::wire::icnx::TLV_APP(type, 0, NULL);
           }
           //Interest check
           if (type >= INTEREST_TLV && type <=INTEREST_TLV_END )
           {
               return new ::wire::icnx::TLV_INTEREST(type, 0, NULL);
           }
           if (type >= INTERESTRESP_TLV && type <=INTERESTRESP_TLV_END )
           {
               return new ::wire::icnx::TLV_INTERESTRESPONSE(type, 0, NULL);
           }
           //Data check
           if (type >= CONTENT_TLV && type <= CONTENT_TLV_END )
           {
               return new ::wire::icnx::TLV_CONTENT(type, 0, NULL);
           } 

            //Raw data check
            if (type >= GENERIC_TLV_RAWDATA && type <= GENERIC_TLV_RAWDATA_END)
            {
            	return new ::wire::icnx::TLV_RAWDATA(type, 0, NULL);
            }
           //Endian data check
           if (type >= GENERIC_TLV_ENDIANDATA && type <= GENERIC_TLV_ENDIANDATA_END)
           {
        	   return new ::wire::icnx::TLV_ENDIANDATA(type, 0);
           }
           //TlvData check
           if (type >= GENERIC_TLV_TLVDATA && type <= GENERIC_TLV_TLVDATA_END)
           {
        	   return new ::wire::icnx::TLV_TLVDATA(type, 0, NULL);
           }

             throw std::domain_error("Undefined TLV type!");

           break;

      }

  }

    TLV_TYPE_t
  GetNextRawDataType(TLV_TYPE_t resetNum )
    {
        static TLV_TYPE_t rawType = GENERIC_TLV_RAWDATA;
        if (rawType > GENERIC_TLV_RAWDATA_END)
           { throw std::out_of_range ("Exceed RAWDATA TLV range!"); }

        if (resetNum)
        {
          rawType = resetNum;
          return rawType++;
         }
        return rawType++;
    }

    TLV_TYPE_t
  GetNextEndianDataType(TLV_TYPE_t resetNum )
    {
        static TLV_TYPE_t rawType = GENERIC_TLV_ENDIANDATA;
        if (rawType > GENERIC_TLV_ENDIANDATA_END)
        { throw std::out_of_range ("Exceed ENDIANDATA TLV range!"); }

        if (resetNum)
        {
          rawType = resetNum;
          return rawType++;
         }
        return rawType++;
    }

    TLV_TYPE_t
  GetNextContainerType(TLV_TYPE_t resetNum )
    {
        static TLV_TYPE_t rawType = GENERIC_TLV_TLVDATA;
        if (rawType > GENERIC_TLV_TLVDATA_END)
        { throw std::out_of_range ("Exceed Container TLV range!"); }

        if (resetNum)
        {
          rawType = resetNum;
          return rawType++;
         }
        return rawType++;
    }

}

} //icnx
} //wire
