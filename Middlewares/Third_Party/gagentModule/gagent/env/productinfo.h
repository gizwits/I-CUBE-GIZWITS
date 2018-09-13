#ifndef _PRODUCTINFO_H_
#define _PRODUCTINFO_H_
#include "gattypes.h"

#define GAT_MAGICNUM                0xAA55AA55


#define ENV_NAME_WECHATID       "wechatId"
#define ENV_NAME_SERVER         "server"
#define ENV_NAME_PORT           "port"
#define ENV_NAME_APNAME         "apName"
#define ENV_NAME_APPWD          "apPwd"
#define ENV_NAME_M2MKEEPAL      "m2mKeepAL"
#define ENV_NAME_M2MHT          "m2mHT"
#define ENV_NAME_TZ             "tz"
#define ENV_NAME_LOCALHT        "LocalHT"
#define ENV_NAME_LOCALACKT      "LocalAckT"


typedef struct _gatHead_t
{
    uint32 magicNum;
    uint32 infoLen;
    union
    {
        //Reserve for future
        uint8 buf[256];
    }rsvd;
}gatEntHead_t;

typedef struct
{
    gatEntHead_t headInfo;
    uint8 *pInfo;
}gatEnv_t;

typedef enum
{
    KW_PK = 0,
    KW_PASSCODE,
    KW_PD,
    kW_GSERVER,
    KW_GSERVER_IP,
    kW_GSERVER_PORT,
    KW_OLD_PD,
    KW_OLD_PASSCODE,
    KW_AUTHKEY,
    KW_TZSEC,
    KW_WIFI_SSID1,
    KW_WIFI_BSSID1,
    KW_WIFI_KEY1,
    KW_WIFI_SSID2,
    KW_WIFI_BSSID2,
    KW_WIFI_KEY2,
    KW_GFLAG,
	KW_REBOOT_REASON,
    KW_MAX,
}keyWord_t;


typedef struct
{
    uint16 keyWord;
    uint8 keyString[5+1];
    uint8 *pValue;
}configArray_t;

typedef struct
{
    //gagent 微信公众号ID,默认为机智云微信宠物屋ID
    uint8 szWechatDeviceType[15+1];
    //gagent 默认连接服务器域名
    uint8 szGAgentSever[DOMAIN_LEN+1];
    //gagent 默认连接服务器端口，默认为80
    uint32 gagentSeverPort;
    //gagent softap Name ,默认值:XPG-GAgent-xxxx(后面4位为MAC后4位)
    uint8 szGAgentSoftApName[65-4];
    //gagent softap 密码 ,默认值:123456789，若内容为空则热点不加密;
    uint8 szGAgentSoftApPwd[32+1];
    //m2m keepalive 默认值 120s
    uint32 m2mKeepAliveS;
    //m2m 心跳间隔 默认值为 50s
    uint32 m2mHeartbeatIntervalS;
    //gagent 时区秒，默认为东八区:8*(60*60)
    int32 timeZoneS;
    //串口心跳间隔S，默认值 55秒
    uint32 localHeartbeatIntervalS;
    //串口数据传送ACK时间，默认值600ms+数据长度耗时
    uint32 localTransferIntervalMS;
}productDefaultInfo_t;


productinfo_st* gatProductInfoGet( void );
int8 gatProductConfiInfoSave( );
void gatProductInfoInit( void );
uint8 gatProductInfoUpdateFromEnv( productinfo_st *pInfo );
void gatCreatNewPassCode( int8 passCodeLen,uint8 *pSZPassCode );
void gatProductInfoSetGServerIp( uint8 *szIp );
void gatConfigFlagSet( uint16 flag );
void gatTimeZoneSecSet( int32 tzSec );
int8 gatGServerDoMainSet( uint8 *szDoMain,uint32 port );
productDefaultInfo_t* gatProductDefaultInfoGet( void );
void gatSaveRebootReason( gatReBoot_t reason );
void gatProductDefaultInfoInit(void);
void gatLocalProductDefaultInfoSet(int8 *data, int32 len);
int8 gatProductDefaultInfoSet(char *envName,void *pAttrVal);
#endif
