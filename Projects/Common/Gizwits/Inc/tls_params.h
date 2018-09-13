/**
  ******************************************************************************
  * @file    tls_params.h
  * @author  MCD Application Team
  * @version V1.0.1
  * @date    12-April-2017
  * @brief   header for TLS parameters.
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
#ifndef __TLS_PARAMS_H
#define __TLS_PARAMS_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
typedef struct
{
  uint8_t * pRootCa;
  uint8_t * pClientCert;
  uint8_t * pClientPrivateKey;
} tls_params_t;

/* Exported constants --------------------------------------------------------*/
/* Exported macros -----------------------------------------------------------*/
/* Private macros ------------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/

/**
  * @brief  This function retrieves the Root CA certificate to be loaded.
  * @retval SUCCESS - pointer to Root CA certificate
  *         FAIL - NULL
  */
const char * tls_GetRootCaCertificate(void);

/**
  * @brief  This function retrieves the Client certificate to be loaded.
  * @retval SUCCESS - pointer to Client certificate
  *         FAIL - NULL
  */
const char * tls_GetClientCertificate(void);

/**
  * @brief  This function retrieves the Client private key to be loaded.
  * @retval SUCCESS - pointer to Client private key
  *         FAIL - NULL
  */
const char * tls_GetClientPrivateKey(void);

/**
  * @brief  This function loads the Root CA certificate, client certificate,
            and client private key.
  * @retval SUCCESS - 0
  *         FAIL - -1
  */
int tls_LoadCertificates(void);

int tls_SetCertificates(tls_params_t const * const tlsParams);

/**
  * @brief  This function searches for a match of the Root CA certificate,
  *         client certificate, and client private key already loaded onto the
  *         module.
  * @retval SUCCESS - 0
  *         FAIL - -1
  */
int tls_FindCertificateMatch(tls_params_t const * const tlsParams);

int tls_CheckRootCaCertificateMatch(int8_t const certificateSet,
                                    tls_params_t const * const tlsParams);
int tls_CheckClientCertificateMatch(int8_t const certificateSet,
                                    tls_params_t const * const tlsParams);
int tls_CheckClientPrivateKeyMatch(int8_t const certificateSet,
                                   tls_params_t const * const tlsParams);



#ifdef __cplusplus
}
#endif

#endif /* __TLS_PARAMS_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
