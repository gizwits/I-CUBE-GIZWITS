#ifndef _GAGENT_HTTP_H
#define _GAGENT_HTTP_H
#include "gattypes.h"
#include "gatlist.h"


#define IP_LEN_MAX      16
#define DOMAIN_LEN_MAX      128
#define GSERVER_DEFAULT_PORT             80

/* GAgent 掉线重连间隔 10s */
#define HTTP_RECONNECT_INTERVAL          (10*ONE_SECOND)

#define HTTP_OK              (0)
#define HTTP_ERR_FAIL        (-1)       /* 执行失败 */
#define HTTP_ERR_TIMEOUT     (-2)       /* 超时 */
#define HTTP_ERR_NOMEM       (-3)       /* 无资源 */
#define HTTP_ERR_PARAM       (-4)       /* 参数错误 */
#define HTTP_ERR_UNPACK      (-5)       /* 解析出错 */
#define HTTP_ERR_BLOCKLEN_ERR      (-6)       /* 解析出错 */


#define HTTP_SEND_NR_MAX         3         /* http链表允许的最大节点数 */
#define HTTP_REC_NR_MAX         10         /* 从http协议层接收数据包的最大个数 */

#define HTTP_POST_HEAD      "POST %s HTTP/1.1\r\n"\
            "Host: %s\r\n"\
            "Content-Length: %d\r\n"\
            "Content-Type: application/x-www-form-urlencoded\r\n\r\n"\
            "%s"
#define HTTP_GET_HEAD       "GET %s HTTP/1.1\r\n"\
            "Host: %s\r\n"\
            "Cache-Control: no-cache\r\n"\
            "Content-Type: application/x-www-form-urlencoded\r\n\r\n"
//带参数的HTTP_GET头部
#define HTTP_GET_HEAD_CACHE       \
            "GET %s HTTP/1.1\r\n"\
            "Host: %s\r\n"\
            "Content-Type: application/text\r\n"\
            "%s\r\n\r\n"
#define HTTP_DELETE_HEAD    "DELETE %s HTTP/1.1\r\n"\
            "Host: %s\r\n"\
            "Content-Length: %d\r\n"\
            "Content-Type: application/x-www-form-urlencoded\r\n\r\n"\
            "%s"
#define HTTPVersionFlag     "HTTP/1.1"
#define kCRLFNewLine     "\r\n"
#define kCRLFLineEnding     "\r\n\r\n"
#define HTTPTransferFlag    "Transfer-Encoding:"
#define HTTPContentLengthFlag       "Content-Length:"
#define TransEncodingEnding           "\r\n0\r\n\r\n"

#define HTTP_RECBUF_LEN         1024
#define HTTP_CONTENT_LENGTH_TYPE 1
#define HTTP_BLOCK_LEN_MAX 13 //\r\n [blk_len] \r\n, max =iofStrlen(\r\n) + blk_len_max(9) + iofStrlen(\r\n) = 14


typedef enum
{
    HTTP_TRANS_CHUNK,
    HTTP_TRANS_LENGTH,
}httpTranType_t;

typedef enum
{
    HTTP_POST,
    HTTP_GET,
    HTTP_DELETE,
    HTTP_GET_CACHE,
}httpMethod_t;

/******************************************************************************
 * @function http响应报文处理程序。该函数执行时间应尽可能短，
             否则会影响http发送效率
 * @param[in] result- 0接收到http响应报文;-1--请求报文发送失败;
                  -2--接收http响应超时;-3--获取发送资源超时;
 * @param[in] resCode- 仅当result为0时有效.http响应报文的状态代码
 * @param[in] respond- 仅当result为0时有效.http响应报文中的正文内容
 * @param[in] respondLen- 仅当result为0时有效.本次接收http响应报文中正文内容的长度
 * @param[in] beFinished- 用于标识响应报文是否已接收完毕.当响应正文长度超过接收缓冲区大小时,
              回调函数会分多次被调用,直到接收完毕,或超时.若未接收完,需要保存数据。
              1-接收完;0-未接收完成
 * @param[in] arg- 调用gagentHttpReq时传入的用户数据
 *****************************************************************************/

/*http发送节点 */
typedef struct _gatHttpSendNode
{
    struct gatListHead list;
    uint8 szHttpDomain[DOMAIN_LEN_MAX];
    uint8 szIp[IP_LEN_MAX];
    uint32 port;
    int32 fd;
    gatChSta_t sta;
    uint8 *sendBuf;
    uint32 sendLen;
    httpRes_t usrCb;        /* 用户指定的回调函数 */
    uint32 recvTimeout;  /*http接收Timeout */
    int32 reconnTimes;     /*断开重连的次数 */
    gatTimer_st httpReConnectTimer; /*http断开重连的定时器 */
    gatTimer_st httpRecvTimer; /*http接收定时器 */
    void *arg;              /* 用户数据 */
}gatHttpSendNode_t;

typedef struct _gatHttpSendNodeNr
{
    lock_t sendLock;
    struct gatListHead sendHead;
    int32 sendNr;                 /*sendHead链表的 当前节点数 */
    int32 sendNrMax;              /*sendHead链表的 运行最大节点数 */
}gatHttpSendNodeNr_t;

/* http接收节点*/
typedef struct _gatHttpRecNode
{
    struct gatListHead list;
    int32 fd;
    httpRes_t usrCb;
    void* arg;
    uint8 *pHeader;  /*http header接收缓存 */
    uint8 *cacheBuf;  /*http body接收缓存 */
    httpTranType_t type;    /* http传输类型。目前支持Content-Length/chunked两种方式 */
    int32 isRecHead;         /*解析http header状态标识 */
    uint8 cacheBlkLen[HTTP_BLOCK_LEN_MAX];   /*用于解析block len的缓存 */
    int32 toRecLen;         /* http为Content-Length时为总正文长度，chunk时为当前block的长度 */
    uint32 recLen;            /* http为Content-Length时已接收正文长度,chunk时为当前block已接收长度*/
    uint32 cacheLen;           /*cacheBuf 已缓存的数据长度 */
    uint8 isFinished;    /* 判断是否解析完整接收的http数据包，1:解析完整,0:非完整  */
}gatHttpRecNode_t;

typedef struct _gatHttpRecNodeNr
{
    lock_t cbLock;
    struct gatListHead cbHead;
    int32 cbNr;                  /*cbNr链表的 当前节点数 */
    int32 cbNrMax;               /*cbNrMax链表的 运行最大节点数 */
}gatHttpRecNodeNr_t;

/******************************************************************************
 * @function 发送http请求报文。可重入。
 * @param[in] timeoutSec-- 超时时间
 * @param[in] host- URL,比如api.gizwits.com:80,或者10.10.10.154:80,
               未指定端口号时，默认是80
 * @param[in] method- http 请求类型,目前支持GET/POST/DELETE
 * @param[in] content- http请求内容
 * @param[in] cb- 回调函数,当接收到http响应报文或者超时未接收到报文时,该函数被调用
                  不需要时传入NULL
 * @param[in] arg- 用于回调函数被调用时的入参,不需要时传入NULL
 * @return    0- 成功,表示发送中;-1- 失败,无资源
 *****************************************************************************/
int32 gatHttpReq(uint32 timeoutSec, uint8 *host, uint8 *szHostIp,int32 port, uint8 *absPath, httpMethod_t method,
                uint8 *content, httpRes_t cb, void *arg);


/******************************************************************************
 * @function tcp接收到数据时，该函数会被调用.
 * @param[in] fd- socket通道文件描述符
 * @param[in] data- 发送数据
 * @param[in] dataLen- 发送数据长度
 *****************************************************************************/
void httpSendCb(int32 fd, int32 result, void *arg);
void httpConnCb(int32 fd, int32 result);
void httpRecCb(int32 fd, uint8 *data, int32 dataLen);
void gatHttpInit(void);
void httpDelSendNode(gatHttpSendNode_t *node);
void httpDelRecNode(gatHttpRecNode_t *node);
int32 httpParseRescode(int8 *data);
int32 httpPostContextAdapter( uint8* src,int32 srcLen, uint8 **dest );

#endif /* _GAGENT_HTTP_H */

