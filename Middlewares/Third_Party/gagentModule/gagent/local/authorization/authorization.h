#ifndef AUTHORIZATION_H_
#define AUTHORIZATION_H_
#include "gagent.h"

#include "local.h"

#define AUTH_TIME_OUT_S      10
/*GAgent应用层未定义模块信息打印接口*/
#define authPrintf(level,format,args...)\
{\
    if(GAT_LOGLEVEL>=level) \
    {\
        if(GAT_LOGLEVEL>=GAT_MEMLEVEL )\
        {\
         Log("[%06d]",dev_free_heap_size() );\
        }\
        Log("[Auth] "format, ##args);\
    }\
}

void iofAuthKeyRead(uint8 *apiAuthKey);
uint8 *gatAuthkeyGet( void );
void gatRecAuthKey( cliId_t *pCli, int8 *data,int32 len,void *arg );
void gatGetAuthReqAck( cliId_t *pCli, int8 *data,int32 len,void *arg );
#endif