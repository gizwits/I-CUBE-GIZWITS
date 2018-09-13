#ifndef IOF_PTHREAD_H_
#define IOF_PTHREAD_H_
#include "gatplatformType.h"
#include "gattypes.h"
#include "local.h"
#include "lanProcess.h"

#define INTERFACE_THREAD_STACK_SIZE            ( 1000 )
typedef struct _gatThread_st
{
    int8 startFlag;
    void*(*threadFun)(void *arg);
    gatpthread_t *threadId;
    void *arg;
}gatThread_st;
void gatTcpServerHandle( void  );
void TcpDataSendThread(void const * arg);
void TcpDataRecThread(void const * arg);
void SerialSendThread(void const * arg);
void SerialRecThread(void const * arg);

int32 iofStartAllThread( void );
void iofThreadEnable( int32 threadNum,int8 flag );
int32 iofPthreadCreate();
int32 iofPthreadNameSet( int8 *szPthreadName );
#endif
