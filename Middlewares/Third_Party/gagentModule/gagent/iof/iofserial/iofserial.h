#ifndef IOF_SERIAL_H_
#define IOF_SERIAL_H_

#include "gatplatform.h"
#include "gatlist.h"
#include "gattypes.h"
// #include "localio.h"
#define SERIAL_FD             2
#define SERIAL_WRITE_TIMEOUT  30000
#define SERIAL_READ_TIMEOUT   30000
#define SERIAL_REC_LEN  1024

typedef void (*iofSerialSendCb)( int32 fd,int8 result );

int32 iofSerialSelect( void );
int32 iofSerialInit();
int32 GAT_FUN_ATTR_ROM iofSerialSend(int32 fd, uint8 *pData, int32 datalen, iofSerialSendCb cb);
int32 GAT_FUN_ATTR_ROM iofSerialRead(int32 fd, uint8 *pData, int32 datalen);
#endif
