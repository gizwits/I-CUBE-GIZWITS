#include "iofpthread.h"
#include "log.h"
#include "ioftime.h"

extern osSemaphoreId recv_check_lock_sem;

typedef void(*threadFun_t)(void *arg);

#ifndef gatTcpServerThread
#define  gatTcpServerThread  iofTcpServerThread
#else
#define  gatTcpServerHandle()  {}
#endif

#ifndef gatUdpSendThread
#define gatUdpSendThread     iofUdpSendThread
#endif

#ifndef gatUdpRecThread
#define gatUdpRecThread      iofUdpRecThread
#endif

static uint8_t gatSerialSendThread_en;
static uint8_t gatSerialRecThread_en;

int32 GAT_FUN_ATTR_ROM iofPthreadNameSet( int8 *szPthreadName )
{
   return 0;
}

int32 GAT_FUN_ATTR_ROM iofStartAllThread( void )
{
  osThreadDef(TcpSend, TcpDataSendThread, osPriorityAboveNormal, 0, INTERFACE_THREAD_STACK_SIZE);
  osThreadCreate (osThread(TcpSend), NULL);
  
  osThreadDef(TcpRecv, TcpDataRecThread, osPriorityNormal, 0, INTERFACE_THREAD_STACK_SIZE);
  osThreadCreate (osThread(TcpRecv), NULL);
  
  if(gatSerialSendThread_en)
  {
    osThreadDef(SerialSend, SerialSendThread, osPriorityBelowNormal, 0, INTERFACE_THREAD_STACK_SIZE);
    osThreadCreate (osThread(SerialSend), NULL);
  }
  
  if(gatSerialRecThread_en)
  {
    osThreadDef(SerialRecv, SerialRecThread, osPriorityBelowNormal, 0, INTERFACE_THREAD_STACK_SIZE);
    osThreadCreate (osThread(SerialRecv), NULL);
  }
  
  return 0;
}
 
void iofThreadEnable( int32 threadNum,int8 flag )
{
    if(threadNum == 5)
    {
      if(flag)
      {
        gatSerialSendThread_en = 1;
      }
      else
      {
        gatSerialSendThread_en = 0;
      }
    }
    
    if(threadNum == 6)
    {
      if(flag)
      {
        gatSerialRecThread_en = 1;
      }
      else
      {
        gatSerialRecThread_en = 0;
      }
    }
}

void GAT_FUN_ATTR_ROM *iofTcpServerThread(void *arg)
{
    iofPthreadNameSet( "gatTcpServerThread" );
    while(1)
    {
        gatTcpServerHandle();
    }
}


void TcpDataSendThread(void const * arg)
{
    while(1)
    {
        gatTcpDataSendHandle();
        osDelay(100);
    }
}

void TcpDataRecThread(void const * arg)
{
  static int8 flag = 0;
  while(1)
  {
    gatTcpReceiveHandle();
    
//    if(!flag)
//    {
//      flag = 1;
//    }
//    else
//    {
//      
//    }
    osSemaphoreRelease(recv_check_lock_sem);
    osDelay(100);
  }
}
/* 目前x86发送是阻塞的 */
void GAT_FUN_ATTR_ROM *iofUdpSendThread(void *arg)
{
    iofPthreadNameSet( "gatUdpSendThread" );
    while(1)
    {
        gatUdpSendHandle();
    }
}

/******************************************************************
* @function iofUdpServerCreat
* @breif creat udp server
* @param[in] port- server port
* @return fd
******************************************************************/
void GAT_FUN_ATTR_ROM *iofUdpRecThread(void *arg)
{
    iofPthreadNameSet( "gatUdpRecThread" );
    while(1)
    {
        gatUdpReceiveHandle();
    }
}

uint32 serial_send_count = 0, serial_recv_count = 0;

void SerialSendThread(void const * arg)
{
    while(1)
    {
      serial_send_count++;
      gatSerialSendHandle();
      osDelay(100);
    }
}

void SerialRecThread(void const * arg)
{
    while(1)
    {
      serial_recv_count++;
      gatSerialRecHandle();
      osDelay(100);
    }
}


