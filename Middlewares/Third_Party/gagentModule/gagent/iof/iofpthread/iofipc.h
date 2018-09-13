#ifndef _IOF_IPC_H
#define _IOF_IPC_H
#include "gatlist.h"
#include "gattypes.h"
/* KEYID,用于创建信号量时需要 */
#define GAT_KEYID_CLD               (0x20)
#define GAT_KEYID_HTTP_SEND         (0x21)
#define GAT_KEYID_HTTP_REC          (0x22)
#define GAT_KEYID_MQTT              (0X23)
#define GAT_KEYID_MQTT_HAL_REC      (0x24)
#define GAT_KEYID_TIMER             (0x25)
#define GAT_KEYID_LOCAL             (0x26)
#define GAT_KEYID_LANUDP            (0x27)
#define GAT_KEYID_LANTCP            (0x28)
#define GAT_KEYID_LOCAL_NODE        (0X29)
#define GAT_KEYID_SNCORRE           (0X2A)
#define GAT_KEYID_LANCLIENT         (0X2B)
#define GAT_KEYID_CONFIGDATA        (0x2C)

#define GAT_KEYID_SKTSND            (0x30)
#define GAT_KEYID_SKTSND_SYNC       (0x31)
#define GAT_KEYID_SKTRCV            (0x38)
#define GAT_KEYID_SKTRCV_DATA       (0x39)



#define GAT_KEYID_UDPSND            (0x40)
#define GAT_KEYID_UDPSND_SYNC       (0x41)
#define GAT_KEYID_UDPRCV            (0x42)
#define GAT_KEYID_UDPRCV_DATA       (0x43)
#define GAT_KEYID_LOCALSEND_SYNC    (0X44)

#define GAT_KEYID_TCP_CH_SYNC       (0x50)
#define GAT_KEYID_TCPSND_SYNC       (0x51)
#define GAT_KEYID_TCPRCV_DATA       (0x52)
#define GAT_KEYID_TCPSEM_SYNC       (0x53)
#define GAT_KEYID_LOCAL_RECV        (0x54)
#define GAT_KEYID_LOCAL_SEND        (0x55)


#define GAT_KEYID_MSG       (0x10)
#define MSG_LEN_MAX         (64)

/* SIG */
#define SIG_CMD_HTTP            0       /* HTTP消息 */
#define SIG_CMD_SKTCON          1       /* socket connect消息 */
#define SIG_CMD_TCPSEND_CB      2       /* 发送完成 */
#define SIG_CMD_TCPREC          3       /* tcp接收到数据 */
#define SIG_CMD_DP              4       /* 数据平面消息 */
#define SIG_CMD_MQTT            5       /* MQTT消息 */
#define SIG_CMD_TEST            6       /* Just for test */
#define SIG_CMD_APPTIMER        7       /* 低精度定时器消息*/
#define SIG_CMD_UDPREC          8       /* udp接收消息 */
#define SIG_CMD_LOCAL           9       /* local 模块有数据触发 */
#define SIG_CMD_NET             10
#define SIG_CMD_LOCAL_SHFIN     11      /* local 模块握手结束*/
#define SIG_CMD_SOCKETEVT       12      /* socket 事件通知，目前只有断开事件*/
#define SIG_CMD_WANOTA          13      /* 启动大循环OTA */
#define SIG_CMD_NETUP           14      /* 网络初始化成功 */
#define SIG_CMD_DNS             15      /* 启动DNS任务消息 */

#define MAX_SEM_COUNT           40

typedef struct
{
    long int msgType;    //linux消息
    char data[MSG_LEN_MAX];
}msgData_t;

typedef struct
{
    int32 fd;
    int32 result;
    httpRes_t cb;
    void *arg;
}httpSigVal_t;

typedef struct
{
    void *pMqttDataNode;
}mqttSigVal_t;

typedef struct
{
    int32 fd;
    int32 result;
    gatTcpConnCb cb;
}connSigVal_t;

typedef struct
{
    int32 fd;
    int32 result;
    gatTcpSendCb cb;
    void *arg;
}sktTxSigVal_t;

typedef struct
{
    int32 fd;
    char *data;
    int32 len;
    int32 result;
    gatTcpRecCb cb;
}sktRxSigVal_t;

typedef struct
{
    int32 fd;
    struct _udpAddr_t_ to;
    char *data;
    int32 len;
    int32 result;
    gatUdpRecCb cb;
}udpRxSigVal_t;

//定时器相关结构体
typedef struct
{
    void** param;
}timerSigVal_t;

typedef enum
{
    PROT_INTERANL,      /* 小循环协议 */
    PROT_EXTERNAL,      /* 大循环协议 */
    PROT_DEVICE,        /* 串口设备协议 */
}protType_t;

typedef struct
{
    protType_t type;
    int32 fd;
    int32 len;
    char *data;
}dpSigVal_t;
typedef struct
{
    void *param;
}localSigVal_t;

typedef struct
{
    uint16 event;
    int32 param;
}netSigVal_t;

typedef struct
{
    int32 fd;
}socketEvt_t;

typedef struct
{
    int8 type;
    int32 status;
}otaSigVal_t;

typedef struct
{
    int32 cmd;
    union
    {
        httpSigVal_t  httpSigVal;
        mqttSigVal_t  mqttSigVal;
        connSigVal_t  conSigVal;
        sktTxSigVal_t sktTxSigVal;
        sktRxSigVal_t sktRxSigVal;
        //udpRxSigVal_t udpRxSigVal;
        dpSigVal_t    dpSigVal;
        timerSigVal_t appTimerVal;
        localSigVal_t localSigVal;
        netSigVal_t   netSigVal;
        socketEvt_t   socketVal;
        otaSigVal_t   otaVal;
    }val;
}iofSig_t;


//extern osMessageQId  MsgBox;


int32 iofMsgCreat(int32 interKeyId);
//osMessageQId gatMsgId( void );
int32 iofSemInit(lock_t *lock, int32 initVal, int32 semSubId);
int32 iofCountingSemInit(lock_t *lock, int32 initVal, int32 semSubId);
int32 iofSendSig(int32 taskId, iofSig_t *sig);
int32 iofRecSig(int32 taskId, iofSig_t *sig);
int32 iofLockInit(lock_t *lock, int32 subId);
int32 iofLock(lock_t *lock);
int32 iofUnlock(lock_t *lock);

#endif  /* _IOF_IPC_H */

