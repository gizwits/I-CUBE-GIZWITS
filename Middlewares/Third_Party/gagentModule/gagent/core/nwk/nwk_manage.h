#ifndef NWK_MANAGE_H_
#define NWK_MANAGE_H_
#include "gattypes.h"

#define NWK_EVENT_CB_MAX                10

#define HAL_STATUS_AP                   (1<<0)
#define HAL_STATUS_STATION              (1<<1)
#define HAL_STATUS_ONBOARDING           (1<<2)
#define HAL_STATUS_BINDING              (1<<3)
#define HAL_STATUS_STA_CONN             (1<<4)
#define HAL_STATUS_CLOUD_CONN           (1<<5)
#define HAL_STATUS_TEST                 (1<<6)
#define HAL_STATUS_CLIENT               (1<<7)
#define HAL_STATUS_MASK                 0x00FF



typedef struct _nwkInfo_st
{
    uint32 event;
    int32 param;
}nwkInfo_st;

typedef void (*nwkEvent_Cb)(nwkInfo_st NwkInfo);

typedef struct _nwkEventCb_st
{
    void (*eventCb)( uint32 event,int32 param );
    uint32 event;
}nwkEventCb_st;



void  GAT_FUN_ATTR_ROM gatNwkInit( void );
uint16 GAT_FUN_ATTR_ROM nwkHalStatusGet( void );
int32 GAT_FUN_ATTR_ROM gatNwkevtCbRegister( void (*func_Cb)( uint32 event,int32 param ), uint32 event  );
int32 GAT_FUN_ATTR_ROM gatSetNwkEvent( uint32 event );
int32 GAT_FUN_ATTR_ROM gatNwkEventHandle( uint32 event,int32 param );
#endif