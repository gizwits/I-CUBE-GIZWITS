/**
  ******************************************************************************
  * @file    iot_flash_config.h
  * @author  MCD Application Team
  * @version V1.0.1
  * @date    12-April-2017
  * @brief   Header for configuration
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __iot_flash_config_H
#define __iot_flash_config_H
#ifdef __cplusplus
 extern "C" {
#endif

#define USER_CONF_WIFI_SSID_MAX_LENGTH  32
#define USER_CONF_WIFI_PSK_MAX_LENGTH   64  
#define USER_CONF_DEVICE_NAME_LENGTH    32
#define USER_CONF_SERVER_NAME_LENGTH    128
#define USER_CONF_TLS_OBJECT_MAX_SIZE   2048
#define USER_CONF_MAGIC                 0x0123456789ABCDEFuLL
   
#define USER_CONF_GAGENT_DATA_LENGTH    2048   
   
typedef struct {
  uint64_t magic;                             /**< The USER_CONF_MAGIC magic word signals that the structure was once written to FLASH. */
  char ssid[USER_CONF_WIFI_SSID_MAX_LENGTH];  /**< Wifi network SSID. */
  char psk[USER_CONF_WIFI_PSK_MAX_LENGTH];    /**< Wifi network PSK. */
  uint8_t security_mode;                      /**< Wifi network security mode. See @ref wifi_network_security_t definition. */
} wifi_config_t;

typedef struct {
  uint64_t magic;                             /**< The USER_CONF_MAGIC magic word signals that the structure was once written to FLASH. */
  char device_name[USER_CONF_DEVICE_NAME_LENGTH];
  char server_name[USER_CONF_SERVER_NAME_LENGTH];
} iot_config_t;
 
/** Static user configuration data which must survive reboot and firmware update. */
/* Do not chnage field order , due to firewall constraint the tls_device_key size must be place 64 bit boundary */
/* its size sould also be multiple of 64 bits.                                                                  */
typedef struct {
//  char tls_root_ca_cert[USER_CONF_TLS_OBJECT_MAX_SIZE];
//  char tls_device_cert[USER_CONF_TLS_OBJECT_MAX_SIZE];
//  // Above structure member must be aligned on 256 bye  boundary 
//   // to match firewall constraint , same for the size.
//  char tls_device_key[USER_CONF_TLS_OBJECT_MAX_SIZE];
  char gagent_data[USER_CONF_GAGENT_DATA_LENGTH];
  wifi_config_t wifi_config;  
//  iot_config_t iot_config;
//  uint64_t tls_magic;                          /**< The USER_CONF_MAGIC magic word signals that the TLS strings above was once written to FLASH. */

} user_config_t;


   
int checkWiFiCredentials(const char ** const ssid, const char ** const psk, uint8_t * const security_mode);
int updateWiFiCredentials(const char ** const ssid, const char ** const psk, uint8_t * const security_mode);
int updateTLSKeys(void);
int getTLSKeys(const char ** const ca_cert, const char ** const device_cert, const char ** const private_key);
int checkTLSCredentials(void);
int updateServerDeviceConfig(void);
int getServerAddress(const char ** const address);
int getDeviceName(const char ** const name);
int checkServerDevice(void);
int updateFirmwareVersion(void);

#ifdef __cplusplus
}
#endif
#endif /* __iot_flash_config_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
