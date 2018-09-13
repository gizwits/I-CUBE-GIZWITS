#ifndef IOF_TIMER_H_
#define IOF_TIMER_H_
#include "gatplatform.h"
#include "timer_subsystem.h"
#include "localio.h"
void iofTimerInit( void );
uint32 getDevTime_MS();
char *iofCurTime();
void timerCb( int sig );
int32 iofLocalTimerMSStart(void *Timer,localSendNode_t *sendNode);
int32 iofLocalTimerMSStop( void *sendTimeoutTimer );
void TimerThread(void const * arg);
void APDetectThread(void const * arg);
void timerEventHandler(void);
#endif
