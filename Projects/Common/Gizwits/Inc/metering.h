/**
  ******************************************************************************
  * @file    metering.h
  * @author  MCD Application Team
  * @version V1.0.1
  * @date    12-April-2017
  * @brief   Control of the measurement sampling and MQTT reporting loop.
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

#ifndef __METERING_H
#define __METERING_H

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
#include <stdbool.h>

/* Private define ------------------------------------------------------------*/

#define STIOT_SENSOR_NAME_MAX_LENGTH        20
#define STIOT_METERING_TOPIC_MAX_SIZE       50
#define STIOT_METERING_MSG_MAX_SIZE         200

#define STIOT_METERING_SENSOR_TOPIC     "/outbox/gpiot"

#define STIOT_SENSOR_HYGROMETER_NAME    "hygro"
#define STIOT_SENSOR_THERMOMETER1_NAME  "thermo1"
#define STIOT_SENSOR_THERMOMETER2_NAME  "thermo2"
#define STIOT_SENSOR_BAROMETER_NAME     "baro"
#define STIOT_SENSOR_MAGNETOMETER_NAME  "magneto"
#define STIOT_SENSOR_GYROMETER_NAME     "gyro"
#define STIOT_SENSOR_ACCELEROMETER_NAME "accelero"
#define STIOT_SENSOR_PROXIMITY_NAME     "proximity"
#define STIOT_SENSOR_SOUNDMETER_NAME    "sound"

/* NB: VECT_SIZE may be used in the future for sampling more often than publishing. 
 * Will size the temporary storage. */
#define STIOT_SENSOR_HYGROMETER_VECT_SIZE     1
#define STIOT_SENSOR_THERMOMETER1_VECT_SIZE   1
#define STIOT_SENSOR_THERMOMETER2_VECT_SIZE   1
#define STIOT_SENSOR_BAROMETER_VECT_SIZE      1
#define STIOT_SENSOR_MAGNETOMETER_VECT_SIZE   3
#define STIOT_SENSOR_GYROMETER_VECT_SIZE      3
#define STIOT_SENSOR_ACCELEROMETER_VECT_SIZE  3
#define STIOT_SENSOR_PROXIMITY_VECT_SIZE      1
#define STIOT_SENSOR_SOUNDMETER_VECT_SIZE     2

typedef enum {
  STIOT_METERING_REPORTING_FREQ_NONE, /**< No reporting. */
  STIOT_METERING_REPORTING_FREQ_LOW,  /**< Low reporting frequency. Set to the frequency of the MQTT yield loop. */
  STIOT_METERING_REPORTING_FREQ_HIGH, /**< High reporting frequency. Set to the timer-based sampling frequency. */
} stiot_metering_reporting_freq_t;

typedef enum {
  STIOT_SENSOR_HYGROMETER_ID = 0,
  STIOT_SENSOR_THERMOMETER1_ID,
  STIOT_SENSOR_THERMOMETER2_ID,
  STIOT_SENSOR_BAROMETER_ID,
  STIOT_SENSOR_MAGNETOMETER_ID,
  STIOT_SENSOR_GYROMETER_ID,
  STIOT_SENSOR_ACCELEROMETER_ID,
  STIOT_SENSOR_PROXIMITY_ID,
  STIOT_SENSOR_SOUNDMETER_ID,
  STIOT_SENSOR_MAX_ID 
} stiot_metering_sensorID_t;

typedef struct {
  void * sensor_hnd;                              /** Handle to the sensor context. */
  stiot_metering_reporting_freq_t reporting_freq; /** How often the sampled values should be published. */
  int threshold;                                  /** Use case support: An action may be triggered when the measurement crosses the trigger. */
  char sensor_name[STIOT_SENSOR_NAME_MAX_LENGTH]; 
  uint8_t sample_size;                            /** Size of the sample vector. For instance, the accelerometer report 3 axes. */
} stiot_metering_sensor_t;

typedef struct {
  uint32_t high_sampling_freq_Hz;                 /** Frequency of the timer used to trig the sampling.
                                                    * 0 means no oversamping compared to STIOT_METERING_REPORTING_FREQ_LOW. */
  stiot_metering_sensor_t aSensors[STIOT_SENSOR_MAX_ID];
  bool deviceInfoNeedsPublish;                    /** Crouton requires that the deviceInfo is published after each MQTT connect
                                                    * and upon reception of a message on the inbox/<device>/deviceInfo topic. */
 } stiot_metering_context_t;


/** The implementation needs the typedef (may redirect to any MQTT client), not the caller.
 *  Keep it opaque.
 */
typedef void* stiot_mqtt_context_t;
// typedef MQTTClient stiot_mqtt_context_t;

/* Private function prototypes -----------------------------------------------*/

stiot_metering_context_t * stiot_metering_init(stiot_mqtt_context_t * mqtt_ctx);
stiot_metering_sensor_t * stiot_metering_sensor_get(stiot_metering_context_t * meter_ctx, stiot_metering_sensorID_t ID);
int stiot_metering_sensor_setReportingFreq(stiot_metering_sensor_t * sensor, stiot_metering_reporting_freq_t freq);
int stiot_metering_sensor_setThreshold(stiot_metering_sensor_t * sensor, int threshold);
int stiot_metering_need_deviceInfoPublish(stiot_metering_context_t * meter_ctx);
int stiot_metering_report(stiot_mqtt_context_t * mqtt_ctx, stiot_metering_context_t * meter_ctx);

#endif /* __METERING_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
