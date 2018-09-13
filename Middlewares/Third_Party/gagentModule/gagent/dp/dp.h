#ifndef _DP_H_
#define _DP_H_
#include "gattypes.h"
// #include "iofipc.h"
typedef enum
{
    PROT_LAN = 0,
    PROT_WAN = 1,
    PROT_DEV = 2,
    PROT_MAX,
}prot_t;

typedef enum
{
    PROTOCOL_IDLE,
    PROTOCOL_TCP,
    PROTOCOL_UDP,
}lanProtocolType;
typedef struct
{
    struct _udpAddr_t_ addr; //udp client addr
    lanProtocolType type;
    uint8   did[DID_LEN+1];   //there had a did if flag= 0x01
    uint16  cmd;
    int32   fd;
    int32   sn;
}lanCli_t;

typedef struct
{
    uint8   cliId[CLIENTID_MAXLEN+1];
    uint8   did[DID_LEN+1];
    uint16  cmd;
    int32   fd;
    int32   sn;
}wanCli_t;

typedef struct
{
    int32 fd;
    uint8 did[DID_LEN+1];
    uint16 sn;
}devCli_t;

typedef struct
{
    prot_t prot;
    union
    {
        lanCli_t lan;
        wanCli_t wan;
        devCli_t dev;
    }cli;
}cliId_t;

typedef struct
{
    cliId_t src;
    cliId_t dst;
}tunnel_t;

typedef void (*cmdHandle_t)(cliId_t *pCli, int8 *data, int32 len,void* arg );

/* 条目:一条命令所需的信息 */
typedef struct
{
    uint16 cmd;
    cmdHandle_t func;
    uint32 mode;
}cmdEntry_t;

/* 组:同一协议下所有条目构成一个组.
   最后需要用一个func为NULL的条目来表示组的结束
 */
typedef struct
{
    prot_t prot;    /* 协议 */
    cmdEntry_t *entry;  /* 指向首条条目的指针 */
}protCG_t;

extern protCG_t protCmdTab[3];

void dpTrans(cliId_t *pCli, int8 *data, int32 len,void *arg );
cmdEntry_t *getDpEntry(cliId_t *pCli, uint16 cmd);
int32 getCurMode(void);
int32 isInCurMode(int32 mode);

#endif
