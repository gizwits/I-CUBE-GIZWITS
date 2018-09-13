/**
  ******************************************************************************
  * @file    sensors_data.c
  * @author  MCD Application Team
  * @version V1.0.1
  * @date    12-April-2017
  * @brief   Manage sensors of STM32L475 IoT board.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2017 STMicroelectronics International N.V.
  * All rights reserved.</center></h2>
  *
  * Redistribution and use in source and binary forms, with or without 
  * modification, are permitted, provided that the following conditions are met:
  *
  * 1. Redistribution of source code must retain the above copyright notice, 
  *    this list of conditions and the following disclaimer.
  * 2. Redistributions in binary form must reproduce the above copyright notice,
  *    this list of conditions and the following disclaimer in the documentation
  *    and/or other materials provided with the distribution.
  * 3. Neither the name of STMicroelectronics nor the names of other 
  *    contributors to this software may be used to endorse or promote products 
  *    derived from this software without specific written permission.
  * 4. This software, including modifications and/or derivative works of this 
  *    software, must execute solely and exclusively on microcontroller or
  *    microprocessor devices manufactured by or for STMicroelectronics.
  * 5. Redistribution and use of this software other than as permitted under 
  *    this license is void and will automatically terminate your rights under 
  *    this license. 
  *
  * THIS SOFTWARE IS PROVIDED BY STMICROELECTRONICS AND CONTRIBUTORS "AS IS" 
  * AND ANY EXPRESS, IMPLIED OR STATUTORY WARRANTIES, INCLUDING, BUT NOT 
  * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A 
  * PARTICULAR PURPOSE AND NON-INFRINGEMENT OF THIRD PARTY INTELLECTUAL PROPERTY
  * RIGHTS ARE DISCLAIMED TO THE FULLEST EXTENT PERMITTED BY LAW. IN NO EVENT 
  * SHALL STMICROELECTRONICS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
  * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
  * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, 
  * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF 
  * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING 
  * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
  * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */

#include <string.h>
#include <stdlib.h>
#include <stdio.h>


#include "stm32l4xx_hal.h"
#include "stm32l475e_iot01.h"
#include "stm32l475e_iot01_tsensor.h"
#include "stm32l475e_iot01_hsensor.h"
#include "stm32l475e_iot01_psensor.h"
#include "stm32l475e_iot01_magneto.h"
#include "stm32l475e_iot01_gyro.h"
#include "stm32l475e_iot01_accelero.h"
#include "vl53l0x_proximity.h"
#include "cmsis_os.h"
#include "gagent.h"
#include "gagent_soc.h"

void sensor_thread(void const * par);
volatile uint8_t LED_Status = 1;
volatile uint8_t LED_Last_Status = 1;
extern uint32 Sensor_Upload_Tick;
extern int16 cloud_connect;
extern int8 WIFI_IsConnected;
extern int8 Device_Passive_Report;

/* Private typedef -----------------------------------------------------------*/
/* Private defines -----------------------------------------------------------*/
/* Private macros ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Global variables ----------------------------------------------------------*/

static int16_t  ACC_Value[3];
static uint8_t  TEMPERATURE_Value;
static uint8_t  HUMIDITY_Value;
uint8_t cPayload[10];
osThreadId SensorThreadHandler;

/* Private function prototypes -----------------------------------------------*/
/* Functions Definition ------------------------------------------------------*/

/**
  * @brief  init_sensors
  * @param  none
  * @retval 0 in case of success
  *         -1 in case of failure
  */
int init_sensors(void)
{
  int ret = 0;
  
  if (ACCELERO_OK != BSP_ACCELERO_Init()) 
  {  
    printf("BSP_ACCELERO_Init() returns %d\n", ret);
    ret = -1;
  }
  
  if (HSENSOR_OK != BSP_HSENSOR_Init()) 
  {
    printf("BSP_HSENSOR_Init() returns %d\n", ret);
    ret = -1;
  }
  
  if (TSENSOR_OK != BSP_TSENSOR_Init())
  {  
    printf("BSP_TSENSOR_Init() returns %d\n", ret);
    ret = -1;
  }
  
  osThreadDef(Sensor, sensor_thread, osPriorityAboveNormal, 0, 1000);
  SensorThreadHandler = osThreadCreate (osThread(Sensor), NULL);
    
  return ret;
}

/**
  * @brief  fill the payload with the sensor values
  * @param  none
  * @param PayloadBuffer is the char pointer for the Payload buffer to be filled
  * @param PayloadSize size of the above buffer
  * @retval 0 in case of success
  *         -1 in case of failure
  */
int PrepareMqttPayload(unsigned char * PayloadBuffer, int PayloadSize)
{  
  BSP_ACCELERO_AccGetXYZ(ACC_Value);
  TEMPERATURE_Value = (uint8_t)BSP_TSENSOR_ReadTemp();
  HUMIDITY_Value = (uint8_t)BSP_HSENSOR_ReadHumidity();
  
  if(Device_Passive_Report)
  {
    PayloadBuffer[0] = 0x03;
    Device_Passive_Report = 0;
  }
  else
  {
    PayloadBuffer[0] = 0x04;
  }

  PayloadBuffer[1] = LED_Status;
  PayloadBuffer[2] = TEMPERATURE_Value;
  PayloadBuffer[3] = HUMIDITY_Value;
  PayloadBuffer[4] = (uint8_t)((ACC_Value[0] + 4000) >> 8);
  PayloadBuffer[5] = (uint8_t)((ACC_Value[0] + 4000) & 0xFF);
  PayloadBuffer[6] = (uint8_t)((ACC_Value[1] + 4000) >> 8);
  PayloadBuffer[7] = (uint8_t)((ACC_Value[1] + 4000) & 0xFF);
  PayloadBuffer[8] = (uint8_t)((ACC_Value[2] + 4000) >> 8);
  PayloadBuffer[9] = (uint8_t)((ACC_Value[2] + 4000) & 0xFF);
  
  return 0;
}

void SensorUploadDataCb( int32 result,void *arg,uint8* pszDID)
{
  printf("Sensor data uploading sent.\n");
}


void sensor_thread(void const * par)
{
  static uint32_t sensor_num=0;
  /* Infinite loop */
  for(;;)
  {
    Sensor_Upload_Tick = HAL_GetTick();
    sensor_num++;
    if(cloud_connect&&WIFI_IsConnected)
    {
      PrepareMqttPayload(cPayload, sizeof(cPayload));  
      gagentUploadData( NULL, cPayload, sizeof(cPayload) ,1, "HELLO", SensorUploadDataCb);
    }
    osThreadSuspend(NULL);
  }
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
