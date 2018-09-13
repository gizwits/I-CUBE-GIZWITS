#include "ioffile.h"
#include "iot_flash_config.h"
#include "flash.h"
#include "stm32l4xx_hal.h"
#include "stm32l475xx.h"

#define OTA_BASE          (FLASH_BASE + FLASH_BANK_SIZE)

extern const user_config_t lUserConfig;

//use FLASH_update to replace the write function, and directly use lUserConfig to replace the read function. 
//int FLASH_update(uint32_t dst_addr, const void *data, uint32_t size);
//ret = FLASH_update((uint32_t)&lUserConfig.wifi_config, &wifi_config, sizeof(wifi_config_t));  

int32 GAT_FUN_ATTR_ROM iofFileOpen( int8 *szName,int32 Oflag )
{
  if(strcmp((char const*)szName, GAGENT_CONFIG_FILE) == 0)
  {
    return GAGENT_CONFIG_FD;
  }
  else if(strcmp((char const*)szName, OTA_FILE_PATH) == 0)
  {
    return OTA_FILE_FD;
  }
  else
  {
    return -1;
  }
}
int32 GAT_FUN_ATTR_ROM iofFileClose( int32 fd )
{
    return fd;
}
int32 GAT_FUN_ATTR_ROM iofFileRead( int32 fd,uint32 offset, uint8 *pData,uint32 len )
{
  if(fd == GAGENT_CONFIG_FD)
  {
    memcpy(pData, (void*)((uint32_t)lUserConfig.gagent_data + offset), len);
  }
  else if(fd == OTA_FILE_FD)
  {
    memcpy(pData, (void*)((uint32_t)OTA_BASE + offset), len);
  }
  else
  {
    return -1;
  }

  return len;
}
int32 GAT_FUN_ATTR_ROM iofFileWrite( int32 fd,uint32 offset, uint8 *pData,uint32 len )
{
  int ret = 0;
  static uint8 ota_full_erase = 0; 
  
  if(fd == GAGENT_CONFIG_FD)
  {
    ret = FLASH_update(((uint32_t)lUserConfig.gagent_data + offset), pData, len);  
  }
  else if(fd == OTA_FILE_FD)
  {
    if(!ota_full_erase)
    {
      printf("Erase the target flash bank first.\n");
      ret = iofFileErase(fd);
      if(ret < 0)
        return -1;
      ota_full_erase = 1;
    }
    ret = FLASH_update(((uint32_t)OTA_BASE + offset), pData, len);  
  }
  else
  {
    return -1;
  }
  
  if (ret < 0)
  {
    fsPrintf(GAT_ERROR, "Failed updating the IOF configuration in FLASH.\n");
    return -1;
  }
  
  return 1;
}
int32 GAT_FUN_ATTR_ROM iofFileErase( int32 fd )
{
  int16 ret;
  if(fd == GAGENT_CONFIG_FD)
  {
    ret = FLASH_unlock_erase((uint32_t)lUserConfig.gagent_data, USER_CONF_GAGENT_DATA_LENGTH);
  }
  else if(fd == OTA_FILE_FD)
  {
    ret = FLASH_unlock_erase((uint32_t)OTA_BASE, FLASH_BANK_SIZE);
  }
  else
  {
    return -1;
  }
    
  return ret;
}

void FirmwareDownloadCb()
{
  uint8 *buf = NULL;
  buf = (uint8*)iofMalloc( sizeof(user_config_t) );
  
  memcpy(buf, (void*)((uint32_t)&lUserConfig), sizeof(user_config_t));
  
  FLASH_update(((uint32_t)FLASH_BANK_SIZE + (uint32_t)&lUserConfig), buf, sizeof(user_config_t));  
  
  iofFree(buf);
  
  if (0 == FLASH_set_boot_bank(FLASH_BANK_BOTH))
  {
    Log("Befault boot bank switched successfully-----------------------------------------\n");
  }
  else
  {
    Log("Error: failed changing the boot configuration------------------------------------------\n");
  }
  
  HAL_NVIC_SystemReset();  
}
