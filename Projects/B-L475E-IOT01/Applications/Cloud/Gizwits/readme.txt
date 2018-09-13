/**
  @page Gizwits Cloud application

  @verbatim
  ******************************************************************************
  * @file    readme.txt 
  * @author  GCSAK IoT Compentence Center
  * @version V1.0.4
  * @date    3-March-2017
  * @brief   Description of Gizwits Cloud application.
  * @IDE     IAR 8.20
  ******************************************************************************
  *
  * Copyright (c) 2017 STMicroelectronics International N.V. All rights reserved.
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
  @endverbatim

@par Application Description 

This application implements an Gizwits Cloud IoT client for B-L475E-IOT01 board 
using on-board Inventek ISM43362 Wifi module connectivity, and FreeRTOS as the 
operating system.

The application connects to Gizwits IoT Cloud with the keys provided by the user. 
When the User sends a command through the app installed on Mobile phone, the 
command will firstly arrive at the cloud server and the server will send the client 
a message including the mobile phone command, which will be finally received by the 
B-L475E-IOT01 board for further processing, via MQTT and HTTP protocol. In addition, 
sensor data collected by the client node can be uploaded to server or smartphone app 
for user processing.

This application supports 1 possible configuration:
- B-L475E-IOT01         : Commands can be sent to the client IoT node, using the cloud 
					      server as a bridge. The client will decode the command and 
                          possibly finish some concrete missions. Besides, sensor data, 
						  which is collected by the IoT node and will be transformed to 
						  meet the cloud server required can be uploaded to server or 
						  smartphone app for user processing.
@par Directory contents

   - Cloud/Gizwits/Src/main.c                             Main application file
   - Cloud/Gizwits/Src/stm32l4xx_it.c                     STM32 interrupt handlers 
   - Cloud/Gizwits/Src/stm32l4xx_hal_msp.c                specific initializations
   - Cloud/Gizwits/Src/stm32l4xx_hal_timebase_tim.c       timebase configurations
   - Cloud/Gizwits/Src/system_stm32l4xx.c                 system initialization
   - Cloud/Gizwits/Src/flash_l4.c                         flash programing interface for L4.
   - Cloud/Gizwits/Inc/main.h                             Header containing config parameters for the application.
   - Cloud/Gizwits/Inc/stm32l4xx_it.h                     STM32 interrupt handlers header file
   - Cloud/Gizwits/Inc/stm32l4xx_hal_conf.h               HAL configuration file
   - Cloud/Gizwits/Inc/es_wifi_conf.h                     ES Wifi configuration file
   - Cloud/Gizwits/Inc/FreeRTOSConfig.h                   FreeRTOS configuration file
	
@par Hardware and Software environment

  - This example runs on B-L475E-IOT01 board (MB1297 rev C01).
  
  - A WiFi access point is required.

  - A Gizwits account is required. Once registered you will be provided Product key and Product Secret for Gizwits 
    IoT service using.

    see https://accounts.gizwits.com/en/register/

@par How to use it ? 

In order to make the program work, you must do the following:

 - WARNING: before opening the project with any toolchain be sure your folder
   installation path is not too in-depth since the toolchain may report errors
   after building.

 - Open IAR toolchain and compile the project (see the release note for detailed 
   information about the version). 

 - Program the firmware on the STM32 board: you can copy (or drag and drop) the 
   binary file under Projects\B-L475E-IOT01\Applications\Cloud\Gizwits\Binary
   to the USB mass storage location created when you plug the STM32 
   board to your PC. 

   Alternatively, you can program the STM32 board directly through one of the 
   supported development toolchains.
  
 - configure the required settings (to be done only once): 
   - when the board is connected to a PC with USB (ST-LINK USB port), 
     open a serial terminal emulator, find the board's COM port and configure it with:
      - 8N1, 115200 bauds, no HW flow control
      - set the line endings to LF or CR-LF
   - at first boot, enter the required parameters:
     - Type Wifi network configuration (SSID, security mode, password) via serial console
     - Enter the Product Key and Product Secret at the specified position in local.c which 
	   is located at Middlewares\Third_Party\gagentModule\gagent\local\soclocal\. Details 
	   about this can be refered to the user manual.

   - after the parameters are configured, it is possible to change wifi network configuration 
     by restarting the board and pressing User button (blue button) just after boot.

@par Connectivity test

  - By default the Gizwits project tries to automatically connect to Gizwits IoT cloud 
    (using parameters set above). 
	
  - Install the app on one mobile phone and the phone will automatically send the command to the 
    client board via Gizwits cloud server. The board will receive the command and print it out 
	through serial port for observing. Besides, the board will by default collect the sensor data, 
	transform it and send to the cloud server and smartphone end for user processing. 


 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
