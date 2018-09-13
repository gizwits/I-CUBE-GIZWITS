#include "ioftime.h"
#include "stm32l4xx_hal.h"  
#include "iofpthread.h"
#include "gagent.h"
#include "es_wifi.h"
#include "iot_flash_config.h"
//#include <sys/time.h>

void milliseconds_sleep(unsigned long mSec);
uint32 Sensor_Upload_Tick;
int8 WIFI_IsConnected = 1;

extern osThreadId SensorThreadHandler;
extern int16 cloud_connect;
extern ES_WIFIObject_t EsWifiObj;
extern osSemaphoreId wifi_lock_sem;
extern uint32 Sensor_Upload_Tick;

void timerCb( int sig )
{
    //static int timernum=0;
    // printf("time=%d\n", ++timernum );
    gatTimerTick( 1 );
    //alarm( 1 );             //we contimue set the timer
    gatTimerTraversal();
    //printf("%s %d \n",__FUNCTION__,__LINE__ );
}

char *getWallTime(void);
void milliseconds_sleep(unsigned long mSec)
{
   HAL_Delay((uint32_t)mSec);
}
void GAT_FUN_ATTR_ROM iofThreadDelay( int32 cnt )
{
   osDelay(cnt);
}

uint32 getDevTime_MS()
{
  return HAL_GetTick();
}
uint32 GetDevTime_S()
{
  return (HAL_GetTick()/1000);
}

void iofTimerInit( void )
{
  osThreadDef(Timer, TimerThread, osPriorityRealtime, 0, 1000);
  osThreadCreate (osThread(Timer), NULL);
  
  osThreadDef(APDetect, APDetectThread, osPriorityBelowNormal, 0, 1000);
  osThreadCreate (osThread(APDetect), NULL);
  
  printf( "Time:%s\n",iofCurTime() );
}

char wt[32];
//only used once in iofTimerInit for printing the current time. Currently leave 
//it alone.
char GAT_FUN_ATTR_ROM *iofCurTime()
{
  return wt;
}

void TimerThread(void const * argument)
{
  static uint32_t timernum=0;
  /* Infinite loop */
  for(;;)
  {
    timernum++;
    //printf("time=%d\n", timernum );
    timerCb(0);
    timerEventHandler();
    osDelay(1000);
  }
}

void timerEventHandler(void)
{
  uint32 current_tick = HAL_GetTick();
  uint32 tick_count;
  if(current_tick >= Sensor_Upload_Tick)
  {
    tick_count = current_tick - Sensor_Upload_Tick;
  }
  else
  {
    tick_count = 0xffffffff - Sensor_Upload_Tick + current_tick;
  }
  
  if(tick_count >= 10000)
  {
    if(cloud_connect&&WIFI_IsConnected)
    {
      if(osThreadIsSuspended(SensorThreadHandler) == osOK)
      {
        osThreadResume(SensorThreadHandler);
      }
    }
  }
}

void LocaltimerHandler( void const* arg )
{
  int32 temp = (int32)pvTimerGetTimerID((TimerHandle_t)arg);
  gatLocalRecTimerOutHandle((void* )temp);
}

osTimerDef(LocalTimer, LocaltimerHandler);

int32 GAT_FUN_ATTR_ROM iofLocalTimerMSStart(void *Timer,localSendNode_t *sendNode)
{
  osStatus ret;
  osTimerId *timer = (osTimerId*)Timer;
  *timer = osTimerCreate(osTimer(LocalTimer), osTimerOnce, (void *)sendNode->fd);
  
  ret = osTimerStart(*timer, sendNode->timeout_ms);
  
  if( ret != 0)
  {
    timerPrintf( GAT_ERROR,"%s timer_settime fail\n",__FUNCTION__);
    return GAT_ERR_FAIL;
  }
  
  timerPrintf( GAT_DEBUG,"%s serial fd[%d] ok\n",__FUNCTION__,sendNode->fd );
  return GAT_OK;
}

int32 GAT_FUN_ATTR_ROM iofLocalTimerMSStop( void *sendTimeoutTimer )
{
  int32 ret = 0;
  osTimerId *timer = (osTimerId*)sendTimeoutTimer;
  
  timerPrintf( GAT_DEBUG,"%s stop send timer\n",__FUNCTION__);
  
  ret = osTimerDelete(*timer);
  if(0 !=ret )
  {
    timerPrintf(GAT_ERROR,"%s delete timer fail\r\n",__FUNCTION__);
  }
  return ret;
}


void APDetectThread(void const * argument)
{
  //static uint32_t timernum=0;
  int8 ret;
  const char *ssid;
  const char  *psk;
  ES_WIFI_SecurityType_t security_mode;
  
  while (checkWiFiCredentials(&ssid, &psk, (uint8_t *) &security_mode) != HAL_OK)
  {
    printf("Your WIFI parameters need to be entered to proceed\n");
    updateWiFiCredentials(&ssid, &psk, (uint8_t *) &security_mode);
  }
  
  /* Infinite loop */
  for(;;)
  {
    if(WIFI_IsConnected)
    {
      printf("---------------------------Detect if WIFI connection is OK.---------------------------\n");
      osSemaphoreWait(wifi_lock_sem, osWaitForever);
      ret = ES_WIFI_IsConnected(&EsWifiObj);
      osSemaphoreRelease(wifi_lock_sem);
      
      if(!ret)
      {
        printf("---------------------------WIFI connection is broken.---------------------------\n");
        WIFI_IsConnected = 0;
        gatSetNwkEvent(NWK_WIFI_STA_DISCONNECTED);
        
        printf("---------------------------Try to reconnect to WIFI.---------------------------\n");
        osSemaphoreWait(wifi_lock_sem, osWaitForever);
        ret = ES_WIFI_Connect(&EsWifiObj, ssid, psk, security_mode);
        osSemaphoreRelease(wifi_lock_sem);
        
        if(ret == ES_WIFI_STATUS_OK)
        {
          printf("---------------------------Reconnection to WIFI is OK now.---------------------------\n");
          WIFI_IsConnected = 1;
          Sensor_Upload_Tick = HAL_GetTick();
          gatSetNwkEvent(NWK_WIFI_STA_CONNECTED);
        }
        else
        {
          printf("---------------------------Reconnection to WIFI is failed.---------------------------\n");
        }
      }
    }
    else
    {
      printf("---------------------------Try to reconnect to WIFI.---------------------------\n");
      osSemaphoreWait(wifi_lock_sem, osWaitForever);
      ret = ES_WIFI_Connect(&EsWifiObj, ssid, psk, security_mode);
      osSemaphoreRelease(wifi_lock_sem);
      
      if(ret == ES_WIFI_STATUS_OK)
      {
        printf("---------------------------Reconnection to WIFI is OK now.---------------------------\n");
        WIFI_IsConnected = 1;
        Sensor_Upload_Tick = HAL_GetTick();
        gatSetNwkEvent(NWK_WIFI_STA_CONNECTED);
      }
      else
      {
        printf("---------------------------Reconnection to WIFI is failed.---------------------------\n");
      }
    }
    
    osDelay(5000);
  }
}
