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
Data
  Optional cachetime
  Optional flowlabel
  Optional Pad
  Message
     Name
        NameSeg
        opt IPID
        opt APP 
     Opt payloadType
     opt expirytime
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
void capContentFile()
{
  count--;
  padFile();
   cout << "</TLVFORMAT>" << endl;

}

void makeContentFile(const char *name, int id, int endRange, const char *type)
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

int main()
{
    //needs to follow correct heirarchy
    makeContentFile("TLV_CONTENT", CONTENT_TLV, CONTENT_TLV_END, cont);
      makeContentFile("TLV_CACHETIME", CACHETIME_TLV,CACHETIME_TLV, optend );
      makeContentFile("TLV_FLOW",FLOW_TLV ,FLOW_TLV, optend );
      makeContentFile("TLV_PAD",PAD_TLV ,PAD_TLV, optend );
      makeContentFile("TLV_MESG", MESG_TLV,MESG_TLV ,cont);
        makeContentFile("TLV_NAME",NAME_TLV ,NAME_TLV , cont);
          makeContentFile("TLV_NAMESEG",NAMESEG_TLV ,NAMESEG_TLV , end);
          makeContentFile("TLV_IPID",IPID_TLV ,IPID_TLV , optend);
          makeContentFile("TLV_APP",APP_TLV ,APP_TLV_END , optend);
        capContentFile();
        makeContentFile("TLV_PAYLDTYPE",PAYLDTYPE_TLV ,PAYLDTYPE_TLV, optend);
        makeContentFile("TLV_PAYLOAD",PAYLOAD_TLV ,PAYLOAD_TLV, optend);
        makeContentFile("TLV_EXPIRY", EXPIRY_TLV,EXPIRY_TLV, optend);
      capContentFile();
      makeContentFile("TLV_VALIDATION_ALG",VALIDATION_ALG_TLV ,VALIDATION_ALG_TLV, optcont);
        makeContentFile("TLV_CRC32",CRC32_TLV,CRC32_TLV,optend);
        makeContentFile("TLV_CRC32C",CRC32C_TLV,CRC32C_TLV,optend);
        makeContentFile("TLV_RFC793",RFC793_TLV,RFC793_TLV,optend);
        makeContentFile("TLV_HMAC_SHA256",HMAC_SHA256_TLV,HMAC_SHA256_TLV,optend);
        makeContentFile("TLV_VMAC128",VMAC128_TLV,VMAC128_TLV,optend);
        makeContentFile("TLV_RSA_SHA256",RSA_SHA256_TLV,RSA_SHA256_TLV,optend);
        makeContentFile("TLV_EC_SECP256K1",EC_SECP256K1_TLV,EC_SECP256K1_TLV,optend);
        makeContentFile("TLV_EC_SECP384R1",EC_SECP384R1_TLV,EC_SECP384R1_TLV,optend);
        makeContentFile("TLV_KEYID",KEYID_TLV,KEYID_TLV,optend);
        makeContentFile("PUBLICKEYLOC_TLV",PUBLICKEYLOC_TLV,PUBLICKEYLOC_TLV, optend);
        makeContentFile("TLV_PUBLICKEY",PUBLICKEY_TLV,PUBLICKEY_TLV,optend);
        makeContentFile("TLV_CERT",CERT_TLV,CERT_TLV,optend);
        makeContentFile("TLV_LINK",LINK_TLV,LINK_TLV,optend);
        makeContentFile("TLV_KEYNAME",KEYNAME_TLV,KEYNAME_TLV,optend);
        makeContentFile("TLV_SIGTIME",SIGTIME_TLV,SIGTIME_TLV,optend);
      capContentFile(); 
      makeContentFile("ValPayload", VALIDATION_ALGPAYLOAD_TLV, VALIDATION_ALGPAYLOAD_TLV,optend);
    capContentFile();

}
