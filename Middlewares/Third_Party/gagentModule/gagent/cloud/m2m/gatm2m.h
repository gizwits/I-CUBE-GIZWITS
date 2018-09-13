#ifndef _GAT_M2M_H
#define _GAT_M2M_H
#include "gattypes.h"
#include "timer_subsystem.h"
#include "mqttlib.h"
#include "gserver.h"
#include "tcp.h"
#include "dp.h"

typedef struct _m2mInfo_st
{
    //重连m2m的次数
    uint8  reConnectTime;
    uint8  szM2MDomain[DOMAIN_LEN+1];
    uint8  szM2MIp[IP_LEN+1];
    int16  m2mMsgId;
    int32  m2mFd;
    uint32 m2mPort;
    //gagent 掉线时间
    uint32 gatM2MDownTime;
    int32 wanClinetNum;
    int32  mqttPingTime;
    mqtt_broker_handle_t mqttBroker;
    gatTimer_st m2mRecTimer;
    gatTimer_st m2mReConnectTimer;
    gatTimer_st m2mHeartBeatTimer;
}m2mInfo_st;

#define CLOUD_HT_TIMEOUT                (120*ONE_SECOND)     /* 同云端保持心跳最大周期 */
/* 同云端心跳间隔不是均匀.第一次间隔较长。丢失第一次心跳后，之后的心跳频率较快，直到同云端恢复心跳 */
#define CLOUD_HT_1ST_PRD                (50)      /* 正常的一次心跳间隔50秒 */
#define CLOUD_REPING_TIME               (10*ONE_SECOND)
#define CLOUD_LOSTPING_CNT              ( (CLOUD_HT_TIMEOUT-CLOUD_REPING_TIME)/CLOUD_REPING_TIME )
/* GAgent 掉线重连间隔 10s */
#define M2M_RECONNECT_INTERVAL          (10*ONE_SECOND)
/* GAgent M2M 接收超时 5s*/
#define M2M_RECEVICE_TIMEOUT            (5*ONE_SECOND)
/* GAgent 重连次数 10次 */
#define M2M_RECONNECT_NUM               (10)



m2mInfo_st* m2mInfoGet();
void m2mInfoSet( m2mInfo_st *pInfo );
int8* gatGetM2MDomain( void );
int8 gatSetM2MDomain( uint8 *szDoMain );
int32 gatGetM2MPort( void );
int8  gatSetM2MPort( int32 port );

// void m2mTest();
void gatM2MInit();
void gatM2MReInit( );
void gatM2MServerStart( void );
void gatM2MServerStop( void );
int32 gatM2MReStartServer( );
void gatM2MStopReStartServer( );
int32 m2mLogin( const int8 *name, const int8 *pwd);
void onM2MLoginCb( int8 result );
void onM2MSubTopicCb( int16 msgId );
void onM2MHeartBeatCb( void );
void onM2MUserDataRecCb( uint8 *szToPic,uint8 *pData,int32 dataLen );
void onM2MHeartBeatCb( void );

void onAppOnLineNumCb( cliId_t *cli, int8 *data, int32 len,void *arg );
void onOTA41EventNoticCb( cliId_t *cli, int8 *data, int32 len,void *arg );
void onOTA4EventNoticCb( cliId_t *cli, int8 *data, int32 len,void *arg );
void gatResCloudCtrl( cliId_t *cli, int8 *data, int32 len,void *arg );
void gatM2MCommand( cliId_t *cli, int8 *data, int32 len,void *arg );

#endif /* _GAT_CLOUD_H */

