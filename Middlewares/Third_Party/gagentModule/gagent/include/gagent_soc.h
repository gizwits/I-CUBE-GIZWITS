#ifndef GAGENT_SOC_H
#define GAGENT_SOC_H
#include "gattypes.h"
#define SDK_USER_VER_LEN        2

typedef void (*gagentUploadDataCb)( int32 result,void *arg,uint8* pszDID);

struct devAttrs
{
    unsigned short mBindEnableTime;
    unsigned char mstrProtocolVer[MCU_PROTOCOLVER_LEN];
    unsigned char mstrP0Ver[MCU_P0VER_LEN];
    unsigned char mstrDevHV[MCU_HARDVER_LEN];
    unsigned char mstrDevSV[MCU_SOFTVER_LEN];
    unsigned char mstrProductKey[PK_LEN];
    unsigned char mstrPKSecret[PKS_LEN];
    unsigned char mDevAttr[MCUATTR_LEN];
    unsigned char mstrSdkVerLow[SDK_USER_VER_LEN];
        //gagent 微信公众号ID,默认为机智云微信宠物屋ID
    uint8 *szWechatDeviceType;
    //gagent 默认连接服务器域名
    uint8 *szGAgentSever;
    //gagent 默认连接服务器端口，默认为80
    uint32 *gagentSeverPort;
    //gagent softap Name ,默认值:XPG-GAgent-xxxx(后面4位为MAC后4位)
    uint8 *szGAgentSoftApName;
    //gagent softap 密码 ,默认值:123456789，若内容为空则热点不加密;
    uint8 *szGAgentSoftApPwd;
    //m2m keepalive 默认值 120s
    uint32 *m2mKeepAliveS;
    //m2m 心跳间隔 默认值为 50s
    uint32 *m2mHeartbeatIntervalS;
    //gagent 时区秒，默认为东八区:8*(60*60)
    int32 *timeZoneS;
    //串口心跳间隔S，默认值 55秒
    uint32 *localHeartbeatIntervalS;
    //串口数据传送ACK时间，默认值600ms+数据长度耗时
    uint32 *localTransferIntervalMS;
};

void gagentInit(struct devAttrs attrs);
int32 gagentUploadData(uint8 *szDID, uint8 *src, uint32 len,uint8 flag, void *arg,gagentUploadDataCb fun );

void gagentGetNTP(_tm *time);
void gagentConfig(unsigned char configType);
void gagentReset(void);
void GAgentEnableBind();

#endif