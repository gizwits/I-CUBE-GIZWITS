#ifndef _GAGENT_H
#define _GAGENT_H

#include "gatplatform.h"
#include "gatlist.h"
#include "gatutilcode.h"
#include "nwk_manage.h"
#include "gdns.h"
#include "iofsocket.h"
#include "iofipc.h"
#include "dp.h"
#include "gatconfig.h"
#include "productinfo.h"
/* Macro of gagent protocol */
#define HI_CMD_ONBOARDING           0x0001
#define HI_CMD_ONBOARDING_ACK       0x0002
#define HI_CMD_DIS                  0x0003
#define HI_CMD_DIS_ACK              0x0004
#define HI_CMD_ONLINE               0x0005
#define HI_CMD_BIND                 0x0006
#define HI_CMD_BIND_ACK             0x0007
#define HI_CMD_LOGIN                0x0008
#define HI_CMD_LOGIN_ACK            0x0009
#define HI_CMD_APLIST               0X000C
#define HI_CMD_APLIST_ACK           0X000D
#define HI_CMD_DEVINFO              0x0013
#define HI_CMD_DEVINFO_ACK          0x0014
#define HI_CMD_HT                   0x0015
#define HI_CMD_HT_ACK               0x0016
#define HI_CMD_TEST                 0X0017
#define HI_CMD_REPLY_TEST           0X0018
#define HI_CMD_AIR_BROADCAST        0X0019
#define HI_CMD_SET_DOMAIN           0x001c
#define HI_CMD_DOMAIN_ACK           0x001d
#define	HI_CMD_OTACHECK 			0x001e
#define	HI_CMD_OTACHECK_ACK			0x001f
#define	HI_CMD_OTAUPGRADE 			0x0020
#define	HI_CMD_OTAUPGRADE_ACK		0x0021
#define HI_CMD_PAYLOAD90            0x0090
#define HI_CMD_UPLOAD_PAYLOAD       0x0091
#define HI_CMD_PAYLOAD93            0x0093
#define HI_CMD_UPLOADACK94          0x0094

#define HI_CMD_LOGLEVEL             0x0010
#define HI_CMD_LOGLEVEL_ACK         0x0011
#define HI_CMD_HTTP_OTA_V4          0x020E
#define HI_CMD_HTTP_OTA_V41         0x0211
#define HI_CMD_APPCLIENTREQ_NUM     0x020F
#define HI_CMD_APPCLIENTRES_NUM     0x0210
#define HI_CMD_LOG2SER              0x0012
#define HI_CMD_BASEINFO2SER         0x0001
#define HI_CMD_ECE                  0x0212
#define HI_CMD_CHILD_LOGIN          0x0213
#define HI_CMD_CHILD_LOGIN_ACK      0x0214
#define HI_CMD_CHILD_SIGNOUT        0x0215
#define HI_CMD_CHILD_SIGNOUT_ACK    0x0216
#define HI_CMD_M2M_CONTROL_COMMAND  0x0217

//GAgent Status
#define WIFI_MODE_AP                  (1<<0)
#define WIFI_MODE_STATION             (1<<1)
#define WIFI_MODE_ONBOARDING          (1<<2)
#define WIFI_MODE_BINDING             (1<<3)
#define WIFI_STATION_CONNECTED        (1<<4)
#define WIFI_CLOUD_CONNECTED          (1<<5)
#define WIFI_LISTFLAG                 (1<<6)
#define WIFI_LEVEL                    (7<<8)
#define WIFI_CLIENT_ON                (1<<11)
#define WIFI_MODE_TEST                (1<<12)

#define NWK_WIFI_AP_UP                (1<<0)
#define NWK_WIFI_AP_DOWN              (1<<1)
#define NWK_WIFI_STA_UP               (1<<2)
#define NWK_WIFI_STA_DOWN             (1<<3)
#define NWK_WIFI_STA_CONNECTED        (1<<4)
#define NWK_WIFI_STA_DISCONNECTED     (1<<5)
#define NWK_WIFI_ONBOARDING_UP        (1<<6)
#define NWK_WIFI_ONBOARDING_DOWN      (1<<7)
#define NWK_CLOUD_CONNECTED           (1<<8)
#define NWK_CLOUD_DISCONNECTED        (1<<9)
#define NWK_CLIENT_UP                 (1<<10)
#define NWK_CLIENT_DOWN               (1<<11)
#define NWK_WIFI_LISTFLAG             (1<<12)
#define NWK_WIFI_SIGNALFLAG           (1<<13)
#define NWK_CLIENT_BING_UP            (1<<14)
#define NWK_CLIENT_BING_DOWN          (1<<15)
#define NWK_TEST_UP                   (1<<16)
#define NWK_TEST_DOWN                 (1<<17)
#define NWK_EVT_STATUS_MASK           (0x000FFFFF)

#define GAGENT_WIFI_CONFIG_AP         (1<<2) /* 是否使用softAP配置过 作用：模块可以选择最近配置的SSID去连接并且发送配置广播包, (配置成功设置，广播完成清除)*/
#define GAGENT_WIFI_CONFIG_AIRLINK    (1<<3) /* 是否使用airlink配置过  作用：模块可以选择最近配置的SSID去连接并且发送配置广播包,(配置成功设置，广播完成清除)*/
#define GAGENT_WIFI_CONFIG_FLAG       (1<<4) /* 是否配网过*/


int32 gatMsgId( void );
int32 gatInit( void );
uint16 gatStatusSet( uint16 status,int8 flag );
uint16 gatStatusGet( void );
int32  gatSetCurMode( gatMode_t mode );
int32  gatResetCurMode( gatMode_t mode );
void gatSetSoftVer( uint8 *szVer );
uint8* gatGetSoftVer( void );
gatRunTInfo_t* gatRunTimeDataGet( void );
void gatCleanConfigData( void );
void gatMainLoop( iofSig_t *sig );
int32 gatDataSend( cliId_t *cli,uint8 *pData,int32 dataLen,uint8 flag, uploadDataCb cb,void *arg );
void gatDealUserPayload90( cliId_t *cli, int8 *data, int32 len,void *arg );
void gatDealUserPayload93( cliId_t *cli,int8 *data, int32 len,void *arg );
int32 gatLanLoginClientNumGet( void );
int32 gatWanClinetNumGet( void );
int32 gatParsingGServerErrcode( uint8 *Http_recevieBuf, int32 *errcode );
void gatReBoot( gatReBoot_t reason );
#endif /* _GAGENT_H */
