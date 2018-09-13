#ifndef _GAGENT_TCP_H_
#define _GAGENT_TCP_H_
#include "gattypes.h"
#include "gatlist.h"

/*tcpServer */
#define LAN_CLICNT_MAX          8       /* 最多允许建立连接的tcp client数量 */

#define TCP_OK              (0)

#define TCP_ERR_FAIL        (-1)       /* 执行失败 */
#define TCP_ERR_TIMEOUT     (-2)        /* 超时 */
#define TCP_ERR_NOMEM       (-3)        /* 无资源 */
#define TCP_ERR_PARAM       (-4)       /* 参数错误 */

#define TCP_NR_MAX          20

#define SKT_RECBUF_LEN      1024        /* 接收缓冲区大小 */

/* all */
/* sockt 层相关打印 */
#define tcpPrintf(level,format, args...)\
{\
        if(GAT_LOGLEVEL>=GAT_MEMLEVEL )\
        {\
         Log("[%06d]",dev_free_heap_size() );\
        }\
        Log( "[tcp] "format, ##args);\
}


typedef struct _tcpChListNr_
{
    lock_t lock;
    struct gatListHead head;
    gat_fd_set rfds;
    int32 fdMax;
}tcpChListNr_t;

typedef struct _tcpChListNode_
{
    struct gatListHead list;
    int32 fd;
    gatTcpRecCb cb;
    void *arg;
}tcpChListNode_t;


/* tcpDataRec */
/*****************************************************************
* @struct 定义tcp数据接收链表的节点
******************************************************************/
typedef struct _tcpDataListNode_
{
    struct gatListHead list;
    int32 fd;
    uint8* data;
    int32 len;
    gatTcpRecCb cb;
}tcpDataListNode_t;

typedef struct _tcpDataListNodeNr_
{
    lock_t lock;
    struct gatListHead head;
    int32 nr;         /* 当前节点数 */
    int32 nrMax;      /* 允许最大的节点数 */
}tcpDataListNodeNr_t;

void gatTcpDataRecThread(void *arg);
int32 gatTcpRecv(int32 fd, uint8* recData, int32 len);

/*tcpDataSend */
#define TCP_SEND_BUFFER_SUPPORT 0  /*平台底层tcp 发送有buffer */


/******************************************************************
* @function 声明平台tcp发送回调函数，通道建立时指定,不需要时指定
* 为NULL
* @param[in] fd- socket通道文件描述符
* @param[in] GAT_OK-发送 成功
******************************************************************/
typedef void (*gatIofTcpSendCb)(int32 fd, int32 result);

/*****************************************************************
* @struct 定义tcp数据发送链表的节点
******************************************************************/
typedef struct _tcpSendListNode_
{
    struct gatListHead list;
    int32 fd;
    gatTcpSendCb sendCB;
    uint8* data;
    int32 len;
    uint8* arg;
}tcpSendListNode_t;

typedef struct _tcpSendListNr_
{
    lock_t sendLock;    /* 链表锁,保护下面资源 */
    struct gatListHead tcpSendList;       /* 发送链表 */
    struct gatListHead tcpSendingList;     /* 正在发送中链表 */
    int32 sendNr;          /* 当前节点数 */
    int32 sendNrMax;      /* 允许最大的节点数 */
}tcpSendListNr;

int32 gatTcpSend(int32 fd, uint8 *data, int32 dataLen, int32 timeoutSec, gatTcpSendCb cb, void *arg);
void gatTcpDataSendThread(void *arg);

typedef struct
{
    lock_t lock;
    struct gatListHead head;
}dl_t;  /* data list */

#endif /* _GAGENT_TCP_H_ */

