#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "main.h"
#include "network.h"
#include "timedate.h"
#include "heap.h"
#include "iot_flash_config.h"
#include "gagent_iot_test_basic_connectivity.h"
#include "msg.h"
#include "gagent_soc.h"

#define STR_PK         "plese replace your product PK" //find it on your Gizwits Platform Project
#define STR_PS         "plese replace your product PS" //find it on your Gizwits Platform Project
#define GIZ_VER        "01"//is your project version,you can change it.

volatile extern uint8_t LED_Status;
volatile extern uint8_t LED_Last_Status;
extern IWDG_HandleTypeDef hiwdg;
extern uint8_t cPayload[10];
extern osThreadId SensorThreadHandler;

osSemaphoreId wifi_lock_sem;
osSemaphoreDef(wifi_lock_sem);
osSemaphoreId tcp_recv_lock_sem;
osSemaphoreDef(tcp_recv_lock_sem);

static void MX_IWDG_Init(void);
static void IWDG_Refresh_Count_Test();

int16 cloud_connect = 0;
int8 Device_Passive_Report = 0;


/* IWDG init function */
static void MX_IWDG_Init(void)
{

  hiwdg.Instance = IWDG;
  hiwdg.Init.Prescaler = IWDG_PRESCALER_256;    //time(s)*32000 = Prescaler*(Window+1)
  hiwdg.Init.Window = 3749;                     //30s timeout
  hiwdg.Init.Reload = 3749;
  if (HAL_IWDG_Init(&hiwdg) != HAL_OK)
  {
    Error_Handler();
  }

}


void userInit()
{
    struct devAttrs attrs;
    unsigned short time=0;
    iofMemset(&attrs,0,sizeof(attrs));

    attrs.mBindEnableTime = time;
    attrs.mDevAttr[0] = 0x00;
    attrs.mDevAttr[1] = 0x00;
    attrs.mDevAttr[2] = 0x00;
    attrs.mDevAttr[3] = 0x00;
    attrs.mDevAttr[4] = 0x00;
    attrs.mDevAttr[5] = 0x00;
    attrs.mDevAttr[6] = 0x00;
    attrs.mDevAttr[7] = 0x00;

    iofMemcpy(attrs.mstrDevHV, "00000001", iofStrlen("00000001"));
    iofMemcpy(attrs.mstrDevSV, "00000001", iofStrlen("00000001"));
    iofMemcpy(attrs.mstrP0Ver, "00000001", iofStrlen("00000001"));

    iofMemcpy(attrs.mstrProductKey, STR_PK, iofStrlen(STR_PK));
    iofMemcpy(attrs.mstrPKSecret, STR_PS, iofStrlen(STR_PS));

    iofMemcpy(attrs.mstrProtocolVer, "00000001", iofStrlen("00000001"));
    iofMemcpy(attrs.mstrSdkVerLow, GIZ_VER, iofStrlen(GIZ_VER));

    gagentInit(attrs);
    GAgentEnableBind();
}

void gizWiFiStatus(unsigned short status)
{
//    _tm cTime;
//    unsigned short gagentStatus = ntohs(status);
//    printf("%s %d status :%04x \r\n",__FUNCTION__,__LINE__,gagentStatus );
//    //bit5 =1,m2m up!
//    if( (gagentStatus &( 1<<5 )) >> 5 )
//    {
//        cloud_connect = 1;
//        gagentGetNTP( &cTime );
//        printf(" year   : %d \r\n",cTime.year );
//        printf(" month  : %d \r\n",cTime.month );
//        printf(" day    : %d \r\n",cTime.day );
//        printf(" hour   : %d \r\n",cTime.hour );
//        printf(" minute : %d \r\n",cTime.minute );
//        printf(" second : %d \r\n",cTime.second );
//        printf(" ntp    : %d \r\n",cTime.ntp );
//        gagentUploadData( NULL,"1234567890", 10,1, &(simulator1.arg),simulatorTestUploadDataCb );
//    }
  unsigned short gagentStatus = ntohs(status);
  printf("%s %d status :%04x \r\n",__FUNCTION__,__LINE__,gagentStatus );
  if( (gagentStatus &( 1<<5 )) >> 5 )
  {
    cloud_connect = 1;
  }
}

int8 GAT_FUN_ATTR_ROM gizIssuedProcess( uint8 *didPtr, uint8 *inData, uint32 inLen, uint8 *outData, int32 *outLen )
{
    uint8_t i = 0;

    if((NULL == inData) || (NULL == outData) || (NULL == outLen))
    {
        printf("!!! IssuedProcess Error \n");
        return -1;
    }

    if(NULL == didPtr)
    {
        printf("~~~gizIssuedProcess: did is NULL .\n");
    }
    else
    {
        printf("%s did:%s.\n",__FUNCTION__,didPtr );
    }

    if(NULL == didPtr)
    {
        switch(inData[0])
        {
            case 0x01:
                if(inData[1]&0x01)
                {
                    LED_Status = inData[2] & 0x01;

                    if(LED_Status)
                    {
                        BSP_LED_On(LED_GREEN);
                    }
                    else
                    {
                        BSP_LED_Off(LED_GREEN);
                    }
                }
                if(LED_Status != LED_Last_Status)
                {
                    if(osThreadIsSuspended(SensorThreadHandler) == osOK)
                    {
                        osThreadResume(SensorThreadHandler);
                    }
                    else
                    {
                        cPayload[1] = LED_Status;
                    }
                    LED_Last_Status = LED_Status;
                }
                break;

            case 0x02:
                if(osThreadIsSuspended(SensorThreadHandler) == osOK)
                {
                    Device_Passive_Report = 1;
                    osThreadResume(SensorThreadHandler);
                }
                break;
            default:

                break;
        }
    }
    else
    {
        printf(" Error : didPtr  \n");
    }

    return 0;
}

//extern osMessageQId  MsgBox;

typedef enum
{
  CLOUD_DEMO_WIFI_INITIALIZATION_ERROR      = -2,
  CLOUD_DEMO_MAC_ADDRESS_ERROR              = -3,
  CLOUD_DEMO_WIFI_CONNECTION_ERROR          = -4,
  CLOUD_DEMO_IP_ADDRESS_ERROR               = -5,
  CLOUD_DEMO_AWS_CONNECTION_ERROR           = -6
} CLOUD_DEMO_Error_t;

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
void CLOUD_Error_Handler(int errorCode)
{
  switch (errorCode)
  {
    case (CLOUD_DEMO_WIFI_INITIALIZATION_ERROR):
    {
      msg_info("Error initializing the module!\n");

      break;
    }
    case (CLOUD_DEMO_MAC_ADDRESS_ERROR):
    {
      msg_info("Error detecting module!\n");

      break;
    }
    case (CLOUD_DEMO_WIFI_CONNECTION_ERROR):
    {
      msg_info("Error connecting to AP!\n");

      break;
    }
    case (CLOUD_DEMO_IP_ADDRESS_ERROR):
    {
      msg_info("Error retrieving IP address!\n");

      break;
    }
    case (CLOUD_DEMO_AWS_CONNECTION_ERROR):
    {
      msg_info("Error connecting to Cloud!\n");

      break;
    }
    default:
    {
      break;
    }
  }
}

void gagent_test(void const * par)
{
  uint8_t ipAddress[4];

  int32 iRet = -1;
  iofSig_t sig;
  memset(&sig, 0, sizeof(iofSig_t));

  msg_info("\n------------------------Program in Flash Bank %d------------------------\n", FLASH_get_bank(FLASH_BASE));

  /* Possibility to update the parameters if the user button is pushed */
  msg_info("Press the User button (Blue) within the next 5 seconds if you want to change the configuration\n"
           "(network configuration)\n\n");

  if ( Button_WaitForPush(5000) )
  {
    network_credential();
  }

  do
  {
    if (network_init()!= 0)
    {
      CLOUD_Error_Handler(CLOUD_DEMO_IP_ADDRESS_ERROR);
    }

    /* Slight delay since the module seems to take some time prior to being able
    to retrieve its IP address after a connection. */
    HAL_Delay(500);

    msg_info("Retrieving the IP address.\n");

    if (network_get_ip_address((uint8_t*) ipAddress))
    {
      CLOUD_Error_Handler(CLOUD_DEMO_IP_ADDRESS_ERROR);
    }
    else
    {
      msg_info("IP address: %d.%d.%d.%d\n",
               ipAddress[0], ipAddress[1], ipAddress[2], ipAddress[3]);
      break;
    }
  }
  while(1);
  /* End of WIFI initialisation */

  msg_info("Setting the RTC from the network time.\n");
  if (0 != setRTCTimeDateFromNetwork())
  {
    msg_error("Could not set the RTC from the network time.\n");
  }

  wifi_lock_sem = osSemaphoreCreate(osSemaphore(wifi_lock_sem) , 1 );
  tcp_recv_lock_sem = osSemaphoreCreate(osSemaphore(tcp_recv_lock_sem) , 1 );
  osSemaphoreWait(tcp_recv_lock_sem, osWaitForever);
  userInit();
  MX_IWDG_Init();

  static uint32_t count_main = 0;
  while(1)
  {
    count_main++;
    iRet = iofRecSig(gatMsgId(), &sig);

    if(iRet < 0 )
    {
      osDelay(1);
    }
    else
    {
      gatMainLoop( &sig );
      HAL_IWDG_Refresh(&hiwdg);
      IWDG_Refresh_Count_Test();
      osDelay(1);
    }
  }
}

void IWDG_Refresh_Count_Test()
{
  static uint32 max_IWDG_interval = 0;
  static uint32 pre_IWDG_count = 0;
  static uint8 IWDG_count_init = 0;

  if(!IWDG_count_init)
  {
    pre_IWDG_count = HAL_GetTick();
    IWDG_count_init = 1;
  }

  uint32 current_count = HAL_GetTick();
  uint32 current_interval = current_count - pre_IWDG_count;
  pre_IWDG_count = current_count;

  if(current_interval > max_IWDG_interval)
  {
    max_IWDG_interval = current_interval;
  }
}