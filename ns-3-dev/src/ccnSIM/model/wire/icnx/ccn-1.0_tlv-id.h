

#ifndef _ICN_ID_NAME_
#define _ICN_ID_NAME_
#include <stdint.h>

namespace wire {
namespace icnx {

  /* Define the types in a fixed TLV header */

typedef uint16_t TLV_LENGTH_t;
typedef uint16_t TLV_TYPE_t;
typedef uint8_t TLV_RAW_DATA_t ;
typedef uint8_t TLV_VERSION;

#define range_4095 0xfff
#define range_255  0xff

//enum TlvTypeIdEnum : TLV_TYPE_t //only with c++ 11
enum TlvTypeIdEnum
{
    //TODO regenerate, make 0 undefined
    //Generic TLVs have a range, to embedded new undefined types
    GENERIC_TLV_RAWDATA=0,
    GENERIC_TLV_RAWDATA_END = GENERIC_TLV_RAWDATA+range_4095,
    GENERIC_TLV_TLVDATA=GENERIC_TLV_RAWDATA_END+1,
    GENERIC_TLV_TLVDATA_END=GENERIC_TLV_TLVDATA+range_4095,
    GENERIC_TLV_ENDIANDATA=GENERIC_TLV_TLVDATA_END+1,
    GENERIC_TLV_ENDIANDATA_END=GENERIC_TLV_ENDIANDATA+range_4095,

    //HEADERS and range based TLV's
    CONTENT_TLV = GENERIC_TLV_ENDIANDATA_END+1,
    CONTENT_TLV_END = CONTENT_TLV+range_255,
    INTEREST_TLV = CONTENT_TLV_END+1,
    INTEREST_TLV_END = INTEREST_TLV+range_255,
    INTERESTRESP_TLV = INTEREST_TLV_END+1, 
    INTERESTRESP_TLV_END = INTERESTRESP_TLV+range_255,  


    //ccnx 1.0 range based TLV's
    APP_TLV = INTERESTRESP_TLV_END+1,
    APP_TLV_END = APP_TLV+range_4095,

    //CONTENT ONLY
    CACHETIME_TLV,   
    PAYLDTYPE_TLV,   
    EXPIRY_TLV,    

    //INTEREST ONLY
    INTEREST_HOP_TLV, 
    INTLIFE_TLV, 
    KEYIDRESTR_TLV,     
    OBJHASHRESTR_TLV,   
    IPIDM_TLV,  

    //INTEREST RESPONSE ONLY (NOT SHARED WITH INTEREST)
    INTERESTRESP_RETURNCODE_TLV, 

    //SHARED
    MESG_TLV,
    FLOW_TLV, 
    PAD_TLV,  
    NAME_TLV,
    NAMESEG_TLV,
    PAYLOAD_TLV,
    IPID_TLV, 

    //VALIDATION ALGORITHM
    VALIDATION_ALG_TLV,
    VALIDATION_ALGPAYLOAD_TLV,
    //not 100% how to handle these unique types (validation types),
    //for now, use rawdata tlv format  JLM FIXME TODO
    //need better definitions or usage
    CRC32_TLV,
    CRC32C_TLV,
    RFC793_TLV,
    HMAC_SHA256_TLV,
    VMAC128_TLV,
    RSA_SHA256_TLV,
    EC_SECP256K1_TLV,
    EC_SECP384R1_TLV,

    //validation depedent data
    KEYID_TLV,       
    PUBLICKEY_TLV,  
    PUBLICKEYLOC_TLV,  
    CERT_TLV,      
    LINK_TLV,       
    KEYNAME_TLV,    
    SIGTIME_TLV,   //todo jlm fixme todo 

    //reserved for system/control
    INVALID_TLV = 0xfffd,
    UNDEF_TLV= 0xfffe,
    ALL_TLVS = 0xffff,
};

} //icnx
} //wire

#endif
