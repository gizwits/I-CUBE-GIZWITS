#ifndef FILE_FILEPUSH_H_
#define FILE_FILEPUSH_H_
#include "gagent.h"
#include "gattypes.h"
#include "httpgat.h"
#include "gagent_md5.h"
#include "gatbase64.h"
#include "ioffile.h"

#ifndef FILEPUSH_ENABLE
#define gatFilePushUploadRequest(a,b)   NULL
#define gatFilePushUploadData(a,b,c)    NULL
#define gatFilePushUploadStop(a,b)      NULL
#define gatFilePushUploadBegin(a,b)     NULL
#define gatFilePushStatusGet()          GAT_IDLE
#else
#define USER_FILE_NAME      "userFile.bin"

#define PUSH_URL_INIT    "/gwfile/v1/files/upload/init"
#define PUSH_URL_SLICE   "/gwfile/v1/files/upload/slice"
#define PUSH_URL_FINISH  "/gwfile/v1/files/upload/finish"

#define GIZWITS_PORT 80
#define UPLOAD_BUF_SIZE             (512)
#define GAT_USERFILE_CACHE_SIZE    (50 *1024 )
#define PIECE_SIZE                  512
#define HTTP_BUF_LEN 1024

#define HTTP_OK              (0)
#define HTTP_ERR_FAIL        (-1)       
#define HTTP_ERR_TIMEOUT     (-2)       

#define FILE_TRANSFER_TIMEOUT                    (15 * ONE_SECOND)  

typedef struct _PushFile_t
{
    int8 status;
    uint8 szSession[32+1];
    uint8 szFileMd5[32+1];
    uint8 szAuthkey[32+1];
    uint8 szIP[17];
    uint8 fileHost[DOMAIN_LEN+1];
    uint8 *pUploadBuf;
    uint32 port;
    int32 fd;
    int32 timestamp;
    uint32 lastRecvTS; /* the last time recieve data */
    uint32 filesize; /* the file whole size */
    uint32 fileRec;  /* filed had received from user */
    uint32 fileOffset; /* filed receive offset had save */
    uint32 offset;   /* the file of offset which had uploaded to server */
    int32 lastUploadLen;/* the last packet len upload to server */
}PushFile_t;

void gatFilePushStart( uint32 filesize, uint8 *filename,uint8 *szFileMD5,uint8 *szIp,uint8 *szHost,uint32 port );
int32 gatFilePushSlice( uint32 contextLen,uint8 *pContext,int32 timeout,httpRes_t cb );
int32 gatFilePushFinish( httpRes_t cb, void *arg );
int32 gatFileReceiveInit( uint32 filesize,uint8 *szFileMD5 );
void gatFileReceiveDataStart( uint32 filesize,uint8 *szMd5,uint32 offset,uint32 pieceSize );
int32 gatFileReceiveDataSave( uint8 *pData,uint32 dataLen );
int8 gatFilePushStatusGet( );
int8 gatFilePushStatusSet( int8 status );
void  gatPushFileUpdataRecTime( uint32 recTS );
void  gatStopFilePush( );
#endif
#endif