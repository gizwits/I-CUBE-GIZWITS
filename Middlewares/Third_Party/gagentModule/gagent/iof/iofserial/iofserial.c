#include "iofserial.h"
#include "stm32l4xx_hal.h"
#include "cmsis_os.h"

extern UART_HandleTypeDef huart4;
uint8 uart4_rx_char;
uint8 uart4_it_buf[100];
uint16 uart4_rx_count = 0;
uint32 uart4_it_tick;

osSemaphoreId uart4_lock_sem;
osSemaphoreDef(uart4_lock_sem);

/****************************************************************
Function        :   serial_write
Description     :   write data to serial fd
buf             :   data need to serial pointer.
buflen          :   data want to write.
return          :   >0 the number of bytes written is returned
                    other error.
Author          :   Alex.Lin
Time            :   2017-07-11
****************************************************************/
static int32 GAT_FUN_ATTR_ROM serial_write( int32 serial_fd,uint8 *buf,int32 buflen )
{
  osSemaphoreWait(uart4_lock_sem, osWaitForever);
  while (HAL_OK != HAL_UART_Transmit(&huart4, buf, (uint16_t)buflen, SERIAL_WRITE_TIMEOUT))
  {
    ;
  }
  osSemaphoreRelease(uart4_lock_sem);

  return buflen;
}

/****************************************************************
Function        :   serial_read
Description     :   read data form serial fd
buf             :   data form serial pointer.
buflen          :   data want to read.
return          :   >0 the realy data length form serial
Author          :   Alex.Lin
Time            :   2017-07-11
****************************************************************/
static int32 GAT_FUN_ATTR_ROM serial_read( int32 serial_fd, uint8 *buf,int32 buflen )
{
  uint16 count = uart4_rx_count;
  uint16 i = 0;
  
  uart4_rx_count = 0;
  
  for(i=0;i<count;i++)
  {
    buf[i] = uart4_it_buf[i];
  }
  
  return count;
}

static int32 GAT_FUN_ATTR_ROM serialMaxFd( void )
{
  return 0;
}
/****************************************************************
Function        :   iofSerialOpen
Description     :   open serial
comport         :   serial comport number
bandrate        :   serial bandrate
nBits           :   serial data Bit
return          :   >= 0 the serial fd
                    other fail.
Author          :   Alex.Lin
Time            :   2017-07-11
****************************************************************/
int32 GAT_FUN_ATTR_ROM iofSerialOpen( uint8 *szSerianNmae, int32 bandrate,int8 nBits,uint8 nEvent,int32 nStop )
{
  int Tmpfd = SERIAL_FD;
  gatPrintf( GAT_NOTIC,"open serial :%s\r\n",szSerianNmae );
  
  HAL_UART_Receive_IT(&huart4, &uart4_rx_char, 1);
  return Tmpfd;
}

int32 GAT_FUN_ATTR_ROM iofSerialInit()
{
  gatPrintf(GAT_OK, "%s ...... ok.\n", __FUNCTION__);
  uart4_lock_sem = osSemaphoreCreate(osSemaphore(uart4_lock_sem) , 1 );
  
  return GAT_OK;
}

int32 GAT_FUN_ATTR_ROM iofSerialSelect( void )
{
  uint32_t tickend = HAL_GetTick();
  uint32_t tickcount;
  
  if(tickend >= uart4_it_tick)
  {
    tickcount = tickend - uart4_it_tick;
  }
  else
  {
    tickcount = 0xffffffff - uart4_it_tick + tickend;
  }
  
  if((tickcount >= 10) && uart4_rx_count)
  {
    return SERIAL_FD;
  }
  else
  {
    //gatPrintf( GAT_ERROR,"%s err,ret=%d errno=%d\n",__FUNCTION__,ret,errno );
    return -1;
  }

}

int32 GAT_FUN_ATTR_ROM iofSerialSend(int32 fd, uint8 *pData, int32 datalen, iofSerialSendCb cb)
{
    if(fd < 0)
  {
    return GAT_ERR_FAIL;
  }
  dumpBuf(GAT_DUMP, pData, datalen);
  int32 ret = serial_write(fd, pData, datalen);
  gatPrintf(GAT_NOTIC, "%s fd:%d ret=%d datalen=%d\n", __FUNCTION__, fd,ret,datalen);
  if (cb)
  {
    cb(fd, ret);
  }
  return ret;
}

int32 GAT_FUN_ATTR_ROM iofSerialRead(int32 fd, uint8 *pData, int32 datalen)
{
  if(fd < 0)
  {
    return GAT_ERR_FAIL;
  }
  int32 ret = 0;
  ret = serial_read( fd,pData,datalen );
  return ret;
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
  if (huart == &huart4)
  {
    uart4_it_buf[uart4_rx_count] = uart4_rx_char;
    uart4_rx_count++;
    uart4_it_tick = HAL_GetTick();
    HAL_UART_Receive_IT(&huart4, &uart4_rx_char, 1);
  }
}

