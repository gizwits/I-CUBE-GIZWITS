#ifndef SIMULATOR_H_
#define SIMULATOR_H_
#include "gagent.h"
#include "gattypes.h"
#include "iofnet.h"
#include "log.h"
#include "httpgat.h"
#include "mqttgat.h"
#include "productinfo.h"
#include "gserver.h"


/*模拟器模块打印*/
#define simuPrintf(level,format,args...)\
{\
    if(GAT_LOGLEVEL>=level)\
    {\
        if(GAT_LOGLEVEL>=GAT_MEMLEVEL )\
        {\
         Log("[%06d]",dev_free_heap_size() );\
        }\
        Log("[Simulator] "format,##args); \
    }\
}
#define HTTP_BUF_LEN       1024
//允许最多的子设备数量
#define     DEVNUM      0

#define    SIMULATOR_UPLOAD_TIME_OUT_S   (2)    //上传数据超时
#define    SIMULATOR_RESET_TIME_OUT_S    (60)   //解绑超时
#define    SIMULATOR_LOGIN_TIME_OUT_S    (60)   //登陆超时
#define    SIMULATOR_LOGOUT_TIME_OUT_S   (10)   //登出超时

#define    SIMULATOR_REG_START  (0) //启动连接状态
#define    SIMULATOR_REG_OK     (1) //GServer 连接OK状态

#define    SIMULATOR_RESET_START (0)
#define    SIMULATOR_RESET_OK    (1)


#define SIMULATOR_MQTT_LOGIN_TOPIC      "cli2ser_req"
#define SIMULATOR_MQTT_LOGIN_CMD        0x0213
#define SIMULATOR_MQTT_LOGIN_CMD_ACK    0x0214
#define SIMULATOR_MQTT_LOGOUT_CMD       0X0215
#define SIMULATOR_MQTT_LOGOUT_CMD_ACK   0X0216

enum SIMULATOR_CONN
{
    SIMULATOR_CONN_TIMEOUT = -22,
    SIMULATOR_CONN_ERROR = -21,//其他错误
    SIMULATOR_CONN_SERVER_ERROR = -20,//服务器错误
    SIMULATOR_CONN_ALREADY  = -19,     //连接异常，改设备之前已经登录过了
    SIMULATOR_CONN_M2M_NO_SUBDEV = -18, //m2m返回登陆失败 该设备未绑定到该中控下
    SIMULATOR_CONN_GS_FAIL =-17,//GServer 连接错误
    SIMULATOR_CONN_GS_9001,
    SIMULATOR_CONN_GS_9002,
    SIMULATOR_CONN_GS_9014,
    SIMULATOR_CONN_GS_9023,
    SIMULATOR_CONN_GS_9046,
    SIMULATOR_CONN_GS_9048,
    SIMULATOR_CONN_GS_ERROR,//其他错误
    SIMULATOR_CONN_GS_IP_NULL,//无GServver ip
    SIMULATOR_CONN_ROUTER_FAIL,
    SIMULATOR_CONN_ARG_INVALID,
    SIMULATOR_CONN_M2M_NO,//设备还没连接上m2m
    SIMULATOR_CONN_GAGENT_NO_DID,//入参是GAgent信息，但是GAgent还没did
    SIMULATOR_CONN_GAGENT_M2M_NO,//入参是GAgent信息，且GAgent有did,但还没连接上m2m
    SIMULATOR_CONN_M2M_RET_FAIL,//m2m返回登陆错误
    SIMULATOR_CONN_MAX,//子设备登陆个数到达上限
    SIMULATOR_CONN_DEV_ERROR,   //硬件底层错误
    SIMULATOR_CONN_OK= 0
};
enum SIMULATOR_RESET
{
    SIMULATOR_RESET_TIMEOUT = -12,
    SIMULATOR_RESET_M2M_NO = -11,
    SIMULATOR_RESET_GS_FAIL,//GServer 连接错误
    SIMULATOR_RESET_GS_9001,
    SIMULATOR_RESET_GS_9002,
    SIMULATOR_RESET_GS_9014,
    SIMULATOR_RESET_GS_9023,
    SIMULATOR_RESET_GS_9046,
    SIMULATOR_RESET_GS_9048,
    SIMULATOR_RESET_GS_ERROR,      //其他错误
    SIMULATOR_RESET_ARG_INVALID,   //入参非法
    SIMULATOR_RESET_DEV_ERROR,     //硬件底层错误
    SIMULATOR_RESET_SUCCESS=0
};
enum SIMULATOR_DISCON
{
    SIMULATOR_DISCON_DEV_ERROR    = -6,         //底层数据出错
    SIMULATOR_DISCON_NOLOGIN      = -5,         //子设备之前没有登陆过
    SIMULATOR_DISCON_TIMEOUT      = -4,         //断开超时
    SIMULATOR_DISCON_M2M_ERROR    = -3,         //m2m返回断开错误
    SIMULATOR_DISCON_GAGENT_CLOUD_NO,           //当前设备没有连接到云端，不需要断开
    SIMULATOR_DISCON_ARG_INVALID,               //入参错误
    SIMULATOR_DISCON_OK           = 0           //断开成功
};
enum SIMULATOR_UPLOAD
{
    SIMULATOR_UPLOAD_TIMEOUT       = -5,        //上传数据超时
    SIMULATOR_UPLOAD_DEV_ERROR     = -4,        //底层出错
    SIMULATOR_UPLOAD_CLIENT_NO,                 //没有客户端连接到GAgent
    SIMULATOR_UPLOAD_GAGENT_WIIF_NO,            //设备还没连接上WiFi
    SIMULATOR_UPLOAD_ARG_INVALID,               //入参错误
    SIMULATOR_UPLOAD_OK            = 0          //上传数据成功
};

typedef void (*gagentConnectCb)(int32 result, uint8* pszDID, uint8 *szPasscode, void *arg);
typedef void (*gagentResetSimulatorCb)(int32 result, uint8* szMac, void *arg);
typedef void (*gagentUploadDataCb)( int32 result,void *arg,uint8* pszDID);
typedef void (*gagentDisconnectCb)(int32 result, uint8* pszDID, void *arg);


typedef struct _devConn_t
{
    uint8 szPk[32+1];
    uint8 szPks[32+1];
    uint8 szMac[32+1];
    uint8 szDID[DID_LEN+1];
    uint8 szPasscode[PASSCODE_MAXLEN+1];
    int16 devStatus;
    void *conArg;//连接函数的参数
    // struct _esp_tcp GServer_tcp;
    // struct espconn GServer_tcp_conn;
    gatTimer_st conTimer;
    gagentConnectCb conFunCb;
}devConn_t;

typedef struct _devReset_t
{
    uint8 szPk[32+1];
    uint8 szPks[32+1];
    uint8 szMac[32+1];
    uint8 szDID[DID_LEN+1];
    uint8 szPasscode[PASSCODE_MAXLEN+1];
    int16 devStatus;
    void *resetArg;//解除绑定参数
    // struct _esp_tcp GServer_tcp;
    // struct espconn GServer_tcp_conn;
    gatTimer_st resetTimer;
    gagentResetSimulatorCb resetFunCb;
}devReset_t;
typedef struct _devDisCon_t
{
    uint8 szDID[DID_LEN+1];
    void *disconArg;//登出云端参数
    gatTimer_st disConTimer;
    gagentDisconnectCb disConFunCb;
}devDisCon_t;
typedef struct  _devUpload_t
{
    uint8 szDID[DID_LEN+1];
    void *uploadArg;//上传数据参数
    gatTimer_st upLoadTimer;
    gagentUploadDataCb uploadFunCb;
}devUpload_t;
#ifdef SIMULATOR_ENABLE
void onSimulatorLoginCb( cliId_t *cli, int8 *data, int32 len,void *arg );
void onSimulatorSignOutCb( cliId_t *cli, int8 *data, int32 len,void *arg );
#else
#define  onSimulatorLoginCb         NULL
#define  onSimulatorSignOutCb       NULL
// #define  gagentResetSimulator
#endif
int32 gagentconnect2M2M(uint8 *szMac, uint8 *szPk, uint8 *szPks,uint8 *szDID, uint8 *szPasscode, void *arg,gagentConnectCb fun );
// void gagentDataReceived( cliId_t *cli,uint16 cmd,int8 *data, int32 len );
#endif
