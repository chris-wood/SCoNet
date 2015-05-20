#include "ccn-1.0_tlv-id.h"
#include <ostream>
#include <sstream>
#include <string>
#include <iostream>
#include <string.h>

//types of tlv's (optional/mandatory, 
#define cont "container"
#define end "endValue"
#define optcont "optContainer"
#define optend "optEndValue"

using namespace std;
using namespace wire::icnx;

/*
//InterestResponse
//Return code
Interest
   Optional Lifetime
   Hop Count
  Optional flowlabel
  Optional Pad
  Message
     Name
        NameSeg
        opt IPID
        opt APP 
     opt KeyidRestr
     opt objhashrestr
     IPIDM
     Opt Payload
  Opt Validation Algo
     Opt CRC32
     Opt CRC32C
     Opt RFC793
     Opt HMAC-SHA256
     Opt VMAC128
     Opt RSA-SHA256
     Opt EC-SECP256k1
     Opt EC-SECP384R1
     Opt Keyid
     Opt Publickeyloc
     Opt PublicKey
     Opt Cert
     Opt Link
     Opt Keyname
     Opt sigtime
  Opt Validation Payload
*/

static int count = 0; 
static inline void padFile() { for(int i=0; i< count; i++) { cout << "  "; } }

void makeInterestFile(const char *name, int id, int endRange, const char *type)
{
  padFile();
  string frontBookend="<TLVFORMAT name=\"";
  string endBookend=">";
  cout << frontBookend << name << "\" id=\"" << id << "\" type=\"" << type << "\" endRangeId=\"" << endRange << "\"";
  if (!strcmp(type, end) || !strcmp(type, optend)) {
      cout << "/>" << endl;
  } else {
    cout << endBookend << endl;
    count++;
  }
}

void capInterestFile()
{
  count--;
  padFile();
   cout << "</TLVFORMAT>" << endl;

}
int main()
{
    //needs to follow correct heirarchy
#ifdef INTERESTRESPONSE
  makeInterestFile("TLV_INTERESTRESPONSE", INTERESTRESP_TLV, INTERESTRESP_TLV_END, cont);
    makeInterestFile("TLV_INTERESTRESP_RETURNCODE", INTERESTRESP_RETURNCODE_TLV, INTERESTRESP_RETURNCODE_TLV, end);

#endif

    makeInterestFile("TLV_INTEREST",INTEREST_TLV , INTEREST_TLV_END, cont);
      makeInterestFile("TLV_INTERESTHOP",INTEREST_HOP_TLV ,INTEREST_HOP_TLV, end );
      makeInterestFile("TLV_INTLIFE", INTLIFE_TLV, INTLIFE_TLV, optend );
      makeInterestFile("TLV_FLOW",FLOW_TLV ,FLOW_TLV, optend );
      makeInterestFile("TLV_PAD",PAD_TLV ,PAD_TLV, optend );
      makeInterestFile("TLV_MESG", MESG_TLV,MESG_TLV ,cont);
        makeInterestFile("TLV_NAME",NAME_TLV ,NAME_TLV , cont);
          makeInterestFile("TLV_NAMESEG",NAMESEG_TLV ,NAMESEG_TLV , end);
          makeInterestFile("TLV_IPID",IPID_TLV ,IPID_TLV , optend);
          makeInterestFile("TLV_APP",APP_TLV ,APP_TLV_END , optend);
        capInterestFile();
        makeInterestFile("TLV_PAYLOAD",PAYLOAD_TLV ,PAYLOAD_TLV, optend);
        makeInterestFile("TLV_KEYIDRESTR",KEYIDRESTR_TLV,KEYIDRESTR_TLV, optend);  
        makeInterestFile("TLV_OBJHASHRESTR", OBJHASHRESTR_TLV, OBJHASHRESTR_TLV, optend);
        makeInterestFile("TLV_IPIDM", IPIDM_TLV , IPIDM_TLV, optend);
      capInterestFile();
      makeInterestFile("TLV_VALIDATION_ALG",VALIDATION_ALG_TLV ,VALIDATION_ALG_TLV, optcont);
        makeInterestFile("TLV_CRC32",CRC32_TLV,CRC32_TLV,optend);
        makeInterestFile("TLV_CRC32C",CRC32C_TLV,CRC32C_TLV,optend);
        makeInterestFile("TLV_RFC793",RFC793_TLV,RFC793_TLV,optend);
        makeInterestFile("TLV_HMAC_SHA256",HMAC_SHA256_TLV,HMAC_SHA256_TLV,optend);
        makeInterestFile("TLV_VMAC128",VMAC128_TLV,VMAC128_TLV,optend);
        makeInterestFile("TLV_RSA_SHA256",RSA_SHA256_TLV,RSA_SHA256_TLV,optend);
        makeInterestFile("TLV_EC_SECP256K1",EC_SECP256K1_TLV,EC_SECP256K1_TLV,optend);
        makeInterestFile("TLV_EC_SECP384R1",EC_SECP384R1_TLV,EC_SECP384R1_TLV,optend);
        makeInterestFile("TLV_KEYID",KEYID_TLV,KEYID_TLV,optend);
        makeInterestFile("TLV_PUBLICKEYLOC",PUBLICKEYLOC_TLV,PUBLICKEYLOC_TLV, optend);
        makeInterestFile("TLV_PUBLICKEY",PUBLICKEY_TLV,PUBLICKEY_TLV,optend);
        makeInterestFile("TLV_CERT",CERT_TLV,CERT_TLV,optend);
        makeInterestFile("TLV_LINK",LINK_TLV,LINK_TLV,optend);
        makeInterestFile("TLV_KEYNAME",KEYNAME_TLV,KEYNAME_TLV,optend);
        makeInterestFile("TLV_SIGTIME",SIGTIME_TLV,SIGTIME_TLV,optend);
      capInterestFile(); 
      makeInterestFile("ValPayload", VALIDATION_ALGPAYLOAD_TLV, VALIDATION_ALGPAYLOAD_TLV,optend);
    capInterestFile();
#ifdef INTERESTRESPONSE
  capInterestFile();
#endif

}
