#ifndef _LOCAL_H_
#define _LOCAL_H_
#include "gattypes.h"
#include "iofipc.h"
extern gatpthread_t gatSerialSendTid;
extern gatpthread_t gatSerialRecTid;
typedef void (*localSendCb_t)( void* arg );


typedef struct _localInfo_st
{
    int32 fd;
    lock_t localSemId;
}localInfo_st;

#define SERIAL_SEND_NODE_MAX    20
#define SERIAL_RECV_NODE_MAX    20

typedef struct
{
    uint8 cmd;
    uint8 sn;
    uint8 crc;
    uint32 len;
}prevPkgAttr_t;
typedef struct
{
    uint8 *buf;
    uint16 offset;
    uint16 bufLen;
    uint8 lastData;
}localRecvCache_t;

typedef struct
{
    struct gatListHead head;
    int32 fd;
    uint8 *pData;
    uint32 dataLen;
    uint32 timeout_ms;
    localSendCb_t funCb;
}localSendNode_t;

typedef struct
{
    struct gatListHead head;
    int32 fd;
    uint8 *pData;
    uint32 dataLen;
}localRecvNode_t;

typedef struct
{
    struct gatListHead head;
    int32 Nr;
    int32 NMax;
    lock_t lock;
}localSendRecvList_t;

typedef struct
{
    struct gatListHead head;
    int32 fd;
    lock_t lock;
    gatDevTimer_t sendTimeoutTimer;
    localRecvCache_t recvCache;
    gatLocalRecCb cb;
    uint8 sendingCount;
    localSendNode_t *sendingNode;
    prevPkgAttr_t prevPkgAttr;
}localChannelOpen_t;


#define LOCAL_ACK_OK        0
#define LOCAL_ACK_TIMEOUT   (-1)
#define LOCAL_BUF_FULLOVER  (-2)
#define LOCAL_NORES         (-3)


int32 GAT_FUN_ATTR_ROM localFdGet( void );
void gatLocalioInit( );
int32 gatLocalSend( int32 fd,uint8 *pData,uint32 dataLen,uint32 timeout_ms,void (* fun)(void* arg ) );
void *gatSerialRecThread( void *arg );
void *gatSerialSendThread( void *arg );
void gatLocalRecTimerOutHandle(void *arg );
localSendRecvList_t *localRecvListGet(void);
#endif