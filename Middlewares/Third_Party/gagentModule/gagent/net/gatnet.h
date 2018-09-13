#ifndef GAT_NET_H
#define GAT_NET_H
#include "gattypes.h"
#include "iofnet.h"
#include "iofipc.h"
#include "gatconfig.h"
#include "log.h"

#define WIFI_SSID_MAXLEN        32
#define WIFI_BSSID_MAXLEN       32
#define WIFI_KEY_MAXLEN         64

#define CONFIG_SOFTAP           0x01
#define CONFIG_AIRLINK 			0x02
#define CONFIG_WRITEAPN 		0x03
#define CONFIG_WRITESSID 		0x04
typedef struct
{
    uint8 ssid1[WIFI_SSID_MAXLEN];
    uint8 bssid1[WIFI_BSSID_MAXLEN];
    uint8 key1[WIFI_KEY_MAXLEN];
    uint8 ssid2[WIFI_SSID_MAXLEN];
    uint8 bssid2[WIFI_BSSID_MAXLEN];
    uint8 key2[WIFI_KEY_MAXLEN];
}NetCon_t;
extern NetCon_t netConPara;
void GAT_FUN_ATTR_ROM gatNetInit( int8 type );
void GAT_FUN_ATTR_ROM gatNetHandle( iofSig_t *sig );
NetCon_t* GAT_FUN_ATTR_ROM gatNetConnParaGet();
void GAT_FUN_ATTR_ROM gatNetConfig( uint8 typed );
int8 GAT_FUN_ATTR_ROM gatConfigDataParse( uint8*data,uint8 dataLen,int8 *pwd,int8 *info );
uint8 GAT_FUN_ATTR_ROM gatExitTest( void );
uint8 GAT_FUN_ATTR_ROM gatEnterTestMode( void );
void GAT_FUN_ATTR_ROM gatNetWriteConfig( uint8 configType,uint8 *payload,uint16 payloadLen);
#endif