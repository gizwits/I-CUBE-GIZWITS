#ifndef _GAGENTTYPES_H
#define _GAGENTTYPES_H
#include "log.h"
#include "gatplatformType.h"
#ifndef NULL
#define NULL (void*)0
#endif
typedef signed char     int8;
typedef unsigned char   uint8;
typedef signed short    int16;
typedef unsigned short  uint16;
typedef signed int      int32;
typedef unsigned int    uint32;
typedef int             lock_t;

/* 全局错误码 */
#define GAT_OK                  0
#define GAT_ERR_FAIL            (-1)    /**/
#define GAT_ERR_TIMEOUT         (-2)
#define GAT_ERR_NORES           (-3)    /* no resourse */
#define GAT_ERR_PARAM           (-4)    /* param err */

#define GAT_SZOK                ("ok")
#define GAT_SZERROR             ("error")

//gserver error code
#define HTTP_ERRORCODE_MAC_HAD_REG        9001
#define HTTP_ERRORCODE_PK_INVALID         9002
#define HTTP_ERRORCODE_NOTFOUND           9014
#define HTTP_ERRORCODE_DEV_DISABLED       9023
#define HTTP_ERRORCODE_REG_PROGRESSING    9046
#define HTTP_ERRORCODE_ENCRYPTED_ENABLED  9048

/********time define***********/
#define ONE_MINUTE                      (60 * ONE_SECOND)
#define ONE_HOUR                        (60 * ONE_MINUTE)

#define INVALID_SOCKET         (-1)

typedef enum
{
    GAT_IDLE,       /* IDLE */
    GAT_READY,      /* 通道已建立 */
    GAT_SENDING,    /* 发送中 */
    GAT_RECVING,    /* 接收中 */
    GAT_BUSY,
    GAT_DONE,
}gatChSta_t;

typedef enum
{
    GAT_MODE_IDLE      = (0),       /* GAgent Startup and befor connect route */
    GAT_MODE_RUN       = (1),       /* GAgent after GAT_MODE_IDLE and connect route */
    GAT_MODE_AUTH      = (2),       /* GAgent in auth mode */
    GAT_MODE_FILE_PUSH = (3),
    GAT_MODE_FILE_PULL = (4),
    GAT_MODE_TEST      = (5),
    GAT_MODE_CONFIG    = (6),
    GAT_MODE_OTA       = (7),
    GAT_MODE_DBG       = (8),

}gatMode_t;

typedef enum
{
    GAT_REBOOT_USER = (0),          /* 用户调用的重启 */
    GAT_REBOOT_AIRLINK = (1),       /* airlink 配置重启 */
    GAT_REBOOT_SOFTAP,              /* softap  配置重启 */
    GAT_REBOOT_WRITECONFIG,         /* write config  配置重启 */
    GAT_REBOOT_LOCALTIMEOUT,        /* local 心跳超时重启 */
    GAT_REBOOT_EXCEP,               /* 异常重启 */
    GAT_REBOOT_NETWORK,             /* 网络故障异常*/
    GAT_REBOOT_INSOFTAPCONFIG,      /* 进入softap配置模式引起的重启 */
    GAT_REBOOT_INAIRLINKCONFIG,     /* 进入Airlink配置模式引起的重启 */
    GAT_REBOOT_TESTMODECONFIG,      /* 进入产测模式引起的重启 */
    GAT_REBOOT_BAKROUTER,           /* 切换备份路由引起的重启 */
    GAT_REBOOT_SHAKEHAND,           /* 握手失败引起的重启 */
    GAT_REBOOT_DEVOTA,              /* 设备OTA引起的重启 */
}gatReBoot_t;


#define PK_LEN           32
#define PKS_LEN          32
#define MAC_LEN          16
#define MAC_MAXLEN       32
#define PASSCODE_LEN     10
#define PASSCODE_MAXLEN  32
#define DID_LEN          22
#define CLIENTID_MAXLEN  23
#define DOMAIN_LEN       60
#define IP_LEN           15
#define PORT_STR_LEN     10
#define AUTHkEY_LEN      32

#define MCU_PROTOCOLVER_LEN     8
#define MCU_P0VER_LEN           8
#define MCU_HARDVER_LEN         8
#define MCU_SOFTVER_LEN         8
#define MCUATTR_LEN             8

#define SECURE_BUF_LEN       33
#define SECURE_BUF_START      3
#define SECURE_SHOWBUF_LEN      6

#define MCU_MD5_UNMATCH           -1
#define MCU_FIRMWARE_TYPE_UNMATCH -2
#define MCU_FIRMWARE_TYPE_HEX      1
#define MCU_FIRMWARE_TYPE_BIN      2
typedef struct
{
    uint16 year;
    uint8 month;
    uint8 day;
    uint8 hour;
    uint8 minute;
    uint8 second;
    uint32 ntp;
}_tm;

typedef int32 (*uploadDataCb)( int32 result, void *arg );

typedef struct _udpAddr_t_
{
    int16 sin_family;   /* rsvd 2 bytes */
    uint16 port;
    uint32 ip;
    uint8 rsvd[8];
}udpAddr_t;

typedef struct _gServerInfo_t
{
    uint8 szGServerIp[IP_LEN+1];
}gServerInfo_st;

typedef struct _productinfo_st
{
    /*信息有对应关系*/
    uint8 szPK[PK_LEN+1];
    uint8 szPKs[PKS_LEN+1];
    uint8 szMac[MAC_LEN+1];
    uint8 szPassCode[PASSCODE_MAXLEN+1];
    uint8 szDID[DID_LEN+1];
    uint8 szGServerDomain[DOMAIN_LEN+1];
    uint8 szGServerIp[IP_LEN+1];
    uint8 szGServerPort[PORT_STR_LEN+1];
    uint8 oldszPassCode[PASSCODE_MAXLEN+1];
    uint8 oldszDID[DID_LEN+1];
    uint8 szAuthKey[AUTHkEY_LEN+1];
}productinfo_st;


typedef struct _gatRunTInfo_t
{
    uint16 gatStatus;
    uint16 flag;
    uint32 clock;
    int32  tzSec;
    int32 onboardingBCTime;
    int32 startupBCTime;
    int32 rebootReason;
}gatRunTInfo_t;

typedef struct _gatTimer_t
{
   int32 (*timerCB)(struct _gatTimer_t *gatTimer ); /* 回调函数返回值表明该回调被重新激活的时间 */
   uint32 period;              /* 期望循环触发的周期, 绝对时间*/
   uint8* szName;               /* 定时器名称 */
   void* param;                /* 用作回调函数的参数或参数指针 */
}gatTimer_st;

typedef struct _gatUpload_t
{
    uint8 flag; /* 1:上报到大循环; 2:上报到小循环;*/
    uploadDataCb fun;
    uint8 *pAvailable;
    uint32 clientNum;
    void *arg;
}gatUpload_t;

typedef struct _ApHostList_str
{
     uint8 ssid[64+1];
     uint8 ApPower; /* 0-100 min:0;max:100 */
}ApAttr_t;
typedef struct _NetHostList_str
{
     uint8 ApNum;
     ApAttr_t* ApList;
}APList_t;


typedef void (*httpRes_t)(int32 result, uint8 *header, uint8 *body, int32 bodyLen, uint8 beFinished, void *arg);
/******************************************************************************
 * @function tcp通道建立连接回调函数.该函数执行时间应尽可能短，
             否则可能会影响http接收任务
 * @param[in] result- 0-发送成功;<0-失败
                      -1-失败;-2-超时;-3-没有资源
 *****************************************************************************/
typedef void (*gatTcpConnCb)(int32 fd, int32 result);
/******************************************************************
* @function 声明tcp发送回调函数，通道建立时指定,不需要时指定 *为NULL
* @param[in] fd- socket通道文件描述符
* @param[in] result- 0：发送成功；-1：发送失败；-2：超时
* @param[in] arg- 调用者传入的用户数据
******************************************************************/
typedef void (*gatTcpSendCb)(int32 fd, int32 result, void *arg);
/******************************************************************************
 * @function tcp接收回调函数,通道建立时指定,不需要时指定为NULL
 * @param[in] fd- 描述符
 * @param[in] recData- 接收数据缓存.该函数退出时被释放.
 * @param[in] dataLen- 接收数据长度
 * @param[in] arg- 用户数据,不需要是传入NULL
 *****************************************************************************/
typedef void (*gatTcpRecCb)(int32 fd, uint8 *recData, int32 dataLen);
typedef void (*gatUdpRecCb)(int32 fd, struct _udpAddr_t_ *from, uint8 *recData, int32 dataLen);

/**************************************************************************************
result : 0  get ack OK
result : -1 get ack Time out
result : -2 底层缓存满了
result : -3 内存不足
**************************************************************************************/
typedef void (*gatLocalSendCb)( int32 fd, int32 result, void *arg );
typedef void (*gatLocalRecCb)( int32 fd,uint8 *pData,int32 dataLen );

#endif /* _GAGENTTYPES_H */

