#ifndef FILE_PULL_H_
#define FILE_PULL_H_
#include "gagent.h"
#include "gattypes.h"
#include "ioffile.h"

#ifndef FILEPULL_ENABLE
#define gatFilePullReady(a)	        NULL
#define gatFilePullAck()            NULL
#define gatFilePullStop()           NULL
#define gatFilePushToMcu(a,b,c)     NULL
#define gatFilePullStatusGet()      GAT_IDLE

#else
#define HEX_FILE_MAXLEN 263
#define BIN_FILE_DEFLEN 256
#define FILE_PULL_TIMEOUT                    (15 * ONE_SECOND)  

typedef struct _FilePull_t
{
	int8 status;	
    int8 fileType;                          //保存的文件类型：bin/hex
    int8 sendType;                          //发送给MCU的文件格式：bin/hex
    uint8 szMd5[33];                        //保存在flash中的文件MD5
    int32 fileSize;                          //保存在flash中的文件长度
    gatTimer_st gatPullTimer;				//分片超时定时器			
}FilePull_t;

int32 gatFileCheckValid(uint8* localRxbuf,uint16 *piecelen );
void  gatFilePushToMcu( int8 filetype,uint8 *MD5,uint32 filesize);
int32 gatFileSendByBin( uint16 piecelen, uint8 isfirst );
int32 gatFileSendByHex( uint8 isfirst );
int32 gatReadPullFile( uint32 offset,uint32 fileSize,int8* buf,uint32 len,int8* szName );
int32 gatFilePullReady( uint8* data );
void gatFilePullAck( );
void  gatFilePullTimerStart( );
void  gatFilePullTimerStop( );
int8  gatFilePullStatusGet( );
#endif

#endif