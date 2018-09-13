/**
  ******************************************************************************
  * @file    rfu.h
  * @author  MCD Application Team
  * @version V1.0.1
  * @date    12-April-2017
  * @brief   Remote firmare upate over TCP/IP.
  *          Header for rfu.c file
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
#ifndef __rfu_H
#define __rfu_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>

#define RFU_OK         0
#define RFU_ERR       -1
#define RFU_ERR_HTTP  -2  /**< HTTP error */
#define RFU_ERR_FF    -3  /**< IAR simple file format error */
#define RFU_ERR_FLASH -4  /**< FLASH erase or programming error */
#define RFU_ERR_OB    -5  /**< Option bytes programming error */

#define FWVERSION_DATE_SIZE   16
#define FWVERSION_TIME_SIZE   9
   
typedef void * http_rfu_handle_t;
typedef void * http_rfu_sock_handle_t;

/** HTTP progressive download progress status */
typedef struct {
  int first_byte;           /**< File position of the first byte of the last downloaded chunk. Starting at 0. */
  int last_byte;            /**< File position of the last byte of the last downloaded chunk. Starting at 0. */
  int resource_size;        /**< File/resource length (in bytes). */
  bool connection_is_open;  /**< HTTP keep-alive connection status. If false, the http_rfu_handle must be closed and re-opened. */
} http_range_status_t;


/** Firmware version information.
 * @note  For user information.
 */
typedef struct {
  uint8_t major;
  uint8_t minor;
  uint8_t patch;
  char build_date[FWVERSION_DATE_SIZE];   /**< __DATE__ */
  char build_time[FWVERSION_TIME_SIZE];   /**< __TIME__ */
} firmware_version_t;


int rfu_getVersion(firmware_version_t const ** const version, bool alt);

int http_open(http_rfu_handle_t * const pHnd, const char *url);
int http_close(const http_rfu_handle_t hnd);
int http_read(uint8_t * const readbuffer, http_range_status_t * const status, const int offset, const int size, const http_rfu_handle_t hnd);
bool http_is_open(const http_rfu_handle_t hnd);

int http_sock_open(http_rfu_sock_handle_t * const http_sock_hnd, const char *url);
int http_sock_close(const http_rfu_sock_handle_t http_sock_hnd);
int http_sock_rcv(uint8_t *const readbuffer, const int size, const http_rfu_sock_handle_t http_sock_hnd);


int rfu_update(const char * const url);

#ifdef __cplusplus
}
#endif

#endif /* __rfu_H */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
