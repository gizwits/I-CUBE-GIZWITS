#ifndef GDNS_H_
#define GDNS_H_

#include "gattypes.h"
#include "log.h"

/*小循环日志打印接口*/
#define dnsPrintf(level,format, args...) \
{\
    if(GAT_LOGLEVEL>=level) \
    {\
        if(GAT_LOGLEVEL>=GAT_MEMLEVEL )\
        {\
         Log("[%06d]",dev_free_heap_size() );\
        }\
        Log("[dns] "format, ##args);\
    }\
}
//最大允许同时进行的DNS 条目
#define FR_DNS_REQ_MAX                      6
#define DNS_DELAY_TIME                      (6*ONE_SECOND)
#define FR_DNS_TIMEOUT                      0
#define FR_DNS_SUCCESS                      1
typedef void (*dnsResult_Cb)(int8 reCode,  int8* ipAddr);

typedef struct _dnsReq_st
{
    int8* domain;
    uint8 ip[IP_LEN + 1];
    int16 reReqTime;
    int32 timeout;
    int32 lastReqTime;
    dnsResult_Cb dnsResultCb;
}dnsReq_st;


int32 GAT_FUN_ATTR_ROM gatDnsTask();
int32 GAT_FUN_ATTR_ROM gatGetHostByName(int8 *domain, int32 timeout, dnsResult_Cb dnsResultCb);
#endif