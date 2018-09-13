/**
  ******************************************************************************
  * @file    network.c
  * @author  MCD Application Team
  * @version V1.0.1
  * @date    12-April-2017
  * @brief   Network abstraction for Wifi boards.
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

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "network.h"
#include "wifi.h"
#include "iot_flash_config.h"

#define  WIFI_CONNECT_MAX_ATTEMPT_COUNT  3
#define  WIFI_WRITE_TIMEOUT   200
#define  WIFI_READ_TIMEOUT    200

#ifdef ES_WIFI_MAX_SSID_NAME_SIZE
#define WIFI_PRODUCT_INFO_SIZE                      ES_WIFI_MAX_SSID_NAME_SIZE
#define WIFI_PAYLOAD_SIZE                           ES_WIFI_PAYLOAD_SIZE
#endif


int network_init(void)
{
  const char *ssid;
  const char  *psk;
  WIFI_Ecn_t security_mode;
  char moduleinfo[WIFI_PRODUCT_INFO_SIZE];
  WIFI_Status_t wifiRes;
  uint8_t macAddress[6];
  int wifiConnectCounter = 0;  

  printf("\n*** WIFI connection ***\n\n");
    
  while (checkWiFiCredentials(&ssid, &psk, (uint8_t *) &security_mode) != HAL_OK)
  {
    printf("Your WIFI parameters need to be entered to proceed\n");
    updateWiFiCredentials(&ssid, &psk, (uint8_t *) &security_mode);
  }
  
  /*  Wifi Module initialization */
  printf("Initializing the WiFi module\n");
  
  wifiRes = WIFI_Init();
  if (WIFI_STATUS_OK != wifiRes )
  {
    printf("Failed to initialize WIFI module\n");
    return -1;
  }
  
  
  /* Retrieve the WiFi module mac address to confirm that it is detected and communicating. */
  WIFI_GetModuleName(moduleinfo);
  printf("Module initialized successfully: %s",moduleinfo);
  
  WIFI_GetModuleID(moduleinfo);
  printf(" %s",moduleinfo);
  
  WIFI_GetModuleFwRevision(moduleinfo);
  printf(" %s\n",moduleinfo);
  
  printf("Retrieving the WiFi module MAC address:");
  wifiRes = WIFI_GetMAC_Address( (uint8_t*)macAddress);
  if ( WIFI_STATUS_OK == wifiRes)
  {
    printf(" %02x:%02x:%02x:%02x:%02x:%02x\n",
         macAddress[0], macAddress[1], macAddress[2],
         macAddress[3], macAddress[4], macAddress[5]);
  }
  else
  {
       printf("Failed to get MAC address\n");
  }
  /* Connect to the specified SSID. */

  printf("\n");
  do 
  {
    printf("\rConnecting to AP: %s  Attempt %d/%d ...",ssid, ++wifiConnectCounter,WIFI_CONNECT_MAX_ATTEMPT_COUNT);
    wifiRes = WIFI_Connect(ssid, psk, security_mode);
    if (wifiRes == WIFI_STATUS_OK) break;
  } 
  while(1);
  
  if (wifiRes == WIFI_STATUS_OK)
  {
    printf("\nConnected to AP %s\n",ssid);
  }
  else
  {
    printf("\nFailed to connect to AP %s\n",ssid);
  }
  
  return (wifiRes == WIFI_STATUS_OK)?0:-1;
}


void network_credential(void)
{
  const char *ssid;
  const char  *psk;
  WIFI_Ecn_t security_mode;
  char console_yn;
  do
  {
    printf("\rDo you want to configure the Wifi credentials ? (y/n)  \b");
    console_yn= getchar();
  }
  while((console_yn != 'y') && (console_yn != 'n') && (console_yn != '\n'));
  if (console_yn != '\n') msg_info("\n");

  if (console_yn == 'y')
  {
    updateWiFiCredentials(&ssid, &psk, (uint8_t *) &security_mode);
  }
}

int  network_get_ip_address(uint8_t  *ipAddress)
{
  if ( WIFI_STATUS_OK != WIFI_GetIP_Address(ipAddress))
  {
    return -1;
  }
  return 0;
}


int network_socket_recv(void *ctx, unsigned char *buf, size_t len)
{
  uint16_t r = 0;
  WIFI_Status_t status = WIFI_STATUS_OK;
  int socket = (int) ctx;
  
  if (len > WIFI_PAYLOAD_SIZE) len=WIFI_PAYLOAD_SIZE;
  
  status = WIFI_ReceiveData( socket, (uint8_t *) buf, (uint16_t) len, &r, WIFI_READ_TIMEOUT);
  
  if (status != WIFI_STATUS_OK)
  {
    printf("network_socket_recv(): error %d in WIFI_ReceiveData() - socket=%d requestedLen=%d received=%d\n",
           socket, status, len, r);
    return -1;
  }
  
  if (r == 0)
  {
    return  -0x6900;
  }
 
  return r;
}




int network_connect( void *ctx, const char *hostname, int dstport )
{
   int          Socket=0;
   uint8_t      ip_addr[4];
   * (int*)     ctx =Socket;
   
   if (WIFI_GetHostAddress((char*) hostname, ip_addr)!= WIFI_STATUS_OK)
   {
     return -1;
   }

   if(WIFI_OpenClientConnection(Socket,WIFI_TCP_PROTOCOL, 
                                      "", 
                                      ip_addr, 
                                      dstport, 
                                      0 ) == WIFI_STATUS_OK)
    {
      return 0;
    }
    
  
  return -1;  
 }
 

int network_socket_send(void *ctx, const unsigned char *buf, size_t len)
{
  int Socket = (int) ctx;
  uint16_t sent=0;
  if ( WIFI_SendData(Socket, (uint8_t *)buf, len, &sent,WIFI_WRITE_TIMEOUT)!= WIFI_STATUS_OK)
  {
    return -1;
  }
  
  return sent;
}


int network_socket_close(uint32_t Socket)
{
  return  WIFI_CloseClientConnection(Socket);
}



/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
