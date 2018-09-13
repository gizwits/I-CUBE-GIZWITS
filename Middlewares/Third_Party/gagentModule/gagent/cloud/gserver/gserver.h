#ifndef _GSERVER_H
#define _GSERVER_H


// #define CLOUT_TIME_REREQ_MAX                (600)/* 10分钟 */
// #define GSERVER_RECONNECT_INTERVAL          (10*ONE_SECOND)
// #define GSERVER_RECEVICE_TIMEOUT            (5*ONE_SECOND)
/* GAgent GServer 重连次数 10次 */
// #define GSERVER_RECONNECT_NUM               (10)

#define TCP_COMM_TX 0
#define TCP_COMM_RX 1
int32 gatRegsiter( int8 *szMac, int8 *szPwd,int8 *szPK,int8 *szPKS,int8 *szMCUAttr,uint8 *szType,uint8 *parent_did,uint8 *szAuth_key,int8 is_reset, int8 *szDomain,uint8 *szDomainIp, uint32 port,int32 timeout,httpRes_t cb, void *arg );
int32 Http_Decrypt_RES( uint8 *Http_recevieBuf,uint8 *pPKSec );
void gatNormalDeviceDisable( void );
int32 gatProvision(void);
void gatGServerStart( void );
void gatGServerInit( void );
#endif /* _GSERVER_H */

