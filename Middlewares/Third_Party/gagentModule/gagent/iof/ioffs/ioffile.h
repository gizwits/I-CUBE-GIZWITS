#ifndef IOF_FILE_H_
#define IOF_FILE_H_
#include "gatplatform.h"
#include "gattypes.h"
#include "log.h"

/* Flag bits to open a file. */
enum{
    GAPI_FS_O_RDONLY_E  = 1,
    GAPI_FS_O_WRONLY_E  = 2,
    GAPI_FS_O_RDWR_E    = 4,
    GAPI_FS_O_CREAT_E   = 8,
};

///* file 层相关打印 */
//#define fsPrintf(level,format, args...)\
//{\
//    if(GAT_LOGLEVEL>=level) \
//        Log( "[File] "format, ##args);\
//}

#define GAGENT_CONFIG_FILE "./gagent_cfg.config"

#define OTA_FILE_PATH  "./wifiota.bin"
#define UPLOAD_FILE    "log1"

#define GAGENT_CONFIG_FD  1
#define OTA_FILE_FD       2

int32 iofFileOpen( int8 *szName,int32 Oflag );
int32 iofFileClose( int32 fd );
int32 iofFileRead( int32 fd,uint32 offset, uint8 *pData,uint32 len );
int32 iofFileWrite( int32 fd,uint32 offset, uint8 *pData,uint32 len );
int32 iofFileErase( int32 fd );
void  FirmwareDownloadCb();
#endif
