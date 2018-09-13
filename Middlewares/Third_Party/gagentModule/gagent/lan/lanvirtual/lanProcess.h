#ifndef LANCOMM_H_
#define LANCOMM_H_
#include "gattypes.h"
#include "log.h"
#include "dp.h"

#define gatTcpServerThread         NULL
#define lanDisDeal                 NULL
#define gatUdpScanSubDevAck        NULL
#define lanBindDeal                NULL
#define lanLoginDeal               NULL
#define lanHtDeal                  NULL
#define gatDealUserPayload         NULL
#define gatSetGServerDoMain        NULL
#define gatUdpScanSubDev           NULL
#define gatCtrlSubDev              NULL
#define gatExitTestDoMain      	   NULL
#define lanOnboardingDeal          NULL
#define lanNoticExitTestDeal	   NULL
#define lanGetDevInfo			   NULL
#define lanAPListDeal			   NULL
#define lanReqOtaCheck             NULL
#define gatSubDevStatusNotify	   {}
#define gatTcpClientDelete(x)      {}
#define deleteLoginClient(x)       {}
#define gatUdpInit()               {}
#define gatLanInit()               {}
#define gatInterActionInit()       {}
#define udpRxHandle(x)             {}
#define gatUdpSendHandle()         {}
#define gatUdpReceiveHandle()      {}
#define lanOtaStatusGet()          GAT_IDLE

int32 gatLanSend( cliId_t *cli,uint16 cmd,uint8 *pData,int32 dataLen,uint8 flag,uint32 sn, gatTcpSendCb cb,void *arg );
int8 isLoginClient( int32 fd );

#endif