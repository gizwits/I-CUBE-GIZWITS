#ifndef WAN_OTA_H_
#define WAN_OTA_H_
#include "gattypes.h"
#include "iofnet.h"
#include "iofipc.h"
#include "log.h"
#include "httpgat.h"
#include "productinfo.h"
#include "gatconfig.h"
#include "local.h"
#include "ioffile.h"
#include "local.h"
#define OtaWanPrintf(level,format, args...)\
{\
    if(GAT_LOGLEVEL>=level) \
        Log( "[OTA-Wan] "format, ##args);\
}

/* OTA 升级类型 */
typedef enum
{
    OTATYPE_WIFI = 1,
    OTATYPE_MCU = 2,
    OTATYPE_INVALID
}otaType_st;

#define DOWNLOAD_URL_LEN        256
/* ota */
#define OTA_RELOAD_CNT          12

#define OTA_RELOAD_INTERVAL     (5*ONE_SECOND)

typedef struct _wanOta_st
{
    uint8 szHost[DOWNLOAD_URL_LEN/2];       
    uint8 szPath[DOWNLOAD_URL_LEN];
    uint8 szIP[IP_LEN+1];                   
    uint32 port;
    uint32 firmwareSize;
    uint32 bodyRecLen;
}wanOta_st;

void gatOTAEventHandle( iofSig_t *pSig );
int32 gatGetMD5( uint8 *httpbuf,uint8 *MD5,int8 *strMD5);
int32 gatGetFileType(uint8 *httpbuf);
wanOta_st* GAT_FUN_ATTR_ROM gatWanOtaParmGet( );
int8 gatWanOtaStatusGet( );
#endif
