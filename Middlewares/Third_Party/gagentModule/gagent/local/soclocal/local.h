#ifndef SOC_LOCAL_H_
#define SOC_LOCAL_H_
#include "localcomm.h"
#include "filepush.h"
#include "filepull.h"

#define GAGENT_MCU_HARDVER
#define GAGENT_MCU_SOFTVER


#define gatLocalShakeHandAck            NULL
#define gatAppData2LocalAck             NULL
#define gatLocalHeartBeatAck            NULL
#define gatLocalConfigGAgent            NULL
#define gatLocalNTPGet                  NULL
#define gatLocalGAgentInfoGet           NULL
#define gatDealLocalReport              NULL
#define gatLocalUploadStop              NULL
#define gatLocalUploadData              NULL
#define gatLocalUploadRequest           NULL
#define gatLocalUploadBegin             NULL
#define gatLocalSendReady              	NULL
#define gatLocalSendAck              	NULL
#define gatLocalSendStop              	NULL
#define gatLocalHeartBeatDelay          NULL


#define simulator_mcuLoginCloud         NULL
#define simulator_mcuUploadData         NULL
#define simulator_mcuLogOutCloud        NULL
#define simulator_mcuDeviceUnbind       NULL
#define gatLocalGpsRequest              NULL
#define gatLocalGpsResponse             NULL
// #define localWriteP0( a,b,c,d,e,f,g )   {}
// #define gatLocalHandle( a )             {}
#define gatLocalUpdataSnCorre( a )      {}

#define gatAppData2Local                gagentDataReceived
// #define gatSerialSendThread             NULL
// #define gatSerialRecThread              NULL
#endif