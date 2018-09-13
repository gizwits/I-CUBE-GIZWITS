#ifndef _MQTTXPG_H
#define _MQTTXPG_H
#include "gatplatform.h"
#include "timer_subsystem.h"
#include "gattypes.h"
#include "iofipc.h"
#include "tcp.h"
#include "gatlist.h"
#include "mqttlib.h"

typedef void (*m2mConnCb)( int8 result );

typedef enum _m2mInfoType_t
{
    DID_TYPE=0,
    PASSCODE_TYPE,
    DOMAIN_TYPE,
    PORT_TYPE,
    IP_TYPE,
    SOCKET_TYPE
}m2mInfoType_t;


typedef struct _mqttPublish_t
{
    uint8 *pPayload;                                               /* mqtt 数据包业务载体 */
    uint8 *pszToPic;                                               /* mqtt 数据包来源topic */
    int32 toPicLen;                                                /* mqtt topic 的长度 */
    int32 payloadLen;                                              /* mqtt业务数据长度 */
}mqttPublish_t;

typedef struct _mqttPINGRESP
{
        void(*fun)(void);// m2mHeartBeatCb cb;
}mqttPingResp_t;

typedef struct _mqttConnAck_t
{
    uint8 result;
}mqttConnAck_t;

typedef struct _mqttSubAck_t
{
    uint16 msgId;
}mqttSubAck_t;

//mqtt 数据接收节点
typedef struct _gatMqttNodeR
{
    // struct gatListHead list;
    uint8 *pData;                                                     /* mqtt 原始数据包载体指针 */
    int32 dataLen;                                                    /* mqtt 原始数据包长度 */
    union
    {
        mqttPublish_t  pubLish;
        mqttConnAck_t  connAck;
        mqttSubAck_t   subAck;
        mqttPingResp_t pingRes;
    }mqtt;
}gatMqttNodeR_t;

//mqtt 接收缓存
typedef struct _gatMqttHalRec_t
{
    lock_t lock;
    uint8 *pData;
    int32 offset;              /* 已读取的正文内容总长度 */
    int32 dataLen;             /* 完整数据包的长度 */
    union
    {
        mqttPublish_t  pubLish;
        mqttConnAck_t  connAck;
        mqttSubAck_t   subAck;
        mqttPingResp_t pingRes;
    }mqtt;
}gatMqttHalRec_t;

//mqtt 接收数据列表
typedef struct _gatMqttNodeRList
{
    lock_t lock;
    struct gatListHead head;
    gatMqttHalRec_t cacheBuf;
}gatMqttNodeRList;



/* m2m connect Necessary information */
// typedef struct _m2mInfo_t
// {
//     int16   m2mMsgId;
//     int32 mqttPingTime;
//     mqtt_broker_handle_t mqttBroker;
//     gatTimer_st m2mHeartBeatTimer;
// }mqttInfo_t;

typedef struct _cloudData_t
{
    uint32 wanClinetNum;
    gatTcpSendCb sencFunCb;//send data success callback function

}cloudData_t;
int8 cloudSubS2C(void);
int8 cloudSubApp2Dev(void);
int8 cloudSubApp2Mod(void);
int8 mqttSubTopicS( uint8 (*topic)[48],int8 topicNum );
void gatMqttInit( void );
void gatMqttReInit( void );
void gatM2MHeartBeatStart( void );
void gatM2MHeartBeatStop( void );
void gatM2mStart( uint8 *szM2mDom,uint32 m2mPort );
int32 mqttExtractPacket( uint8 *buf,const int32 dataLen );
int32 mqttConnBrokerInit( const uint8 *clientid,const uint8 *username,const char* password,int32 fd,mqttSendFun fun );
int8 gatCloudGizDataSend( uint8 *szTopic, uint16 cmd,uint8 flag,uint32 sn,uint8 *pData,uint32 len,gatTcpSendCb fun,void *arg );
int8 mqttSendPubData( uint8 *pszTopic,uint8 *pData,uint32 len,int32 flag,gatTcpSendCb fun,void *arg );
void mqttEventNotify2GAgent( void );
void mqttDataHandle( gatMqttHalRec_t *pNode );
void gatCleanMqttCacheBuf( void );
#endif  /* _MQTTGAT_H */
