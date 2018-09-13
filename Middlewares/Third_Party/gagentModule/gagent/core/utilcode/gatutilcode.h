#ifndef GAT_UTIL_H_
#define GAT_UTIL_H_
#include "gattypes.h"
#include "log.h"
#define DELIMITER     '&'
#define CR            '\r'
#define NEWLINE       '\n'
#define utilPrintf(level,format, args...)\
{\
    if(GAT_LOGLEVEL>=GAT_MEMLEVEL )\
    {\
     Log("[%06d]",dev_free_heap_size() );\
    }\
    if(GAT_LOGLEVEL>=level) \
        Log( "[Util] "format, ##args);\
}

#define DAY_SEC                     (24*60*60)
#define Eastern8th                  (8*60*60)
#define NTP_MIN                     ((2010 - 1970)*365*24*ONE_HOUR)
#define NTP_LOCAL_TICK              (30*ONE_MINUTE)      /* 本地校准的周期，防止tick溢出 */
#define NTP_NET_PERIOD              (60*ONE_MINUTE)      /* 网络校准ntp的周期 */
#define NTP_NET_PERIOD_CNT          (NTP_NET_PERIOD/NTP_LOCAL_TICK)
#define TIMEZONE_DEFAULT            (8)                  /* 默认东8区,北京时间 */
#define TZSEC_VAL_DEFAULT           (TIMEZONE_DEFAULT*ONE_HOUR)

#define TZ_LEN_MAX                  (3)    /* 字符串-12,最长3个字节 */
#define TZ_VAL_MAX                  (12)
#define TZ_VAL_MIN                  (-12)
#define TZSEC_LEN_MAX               (6) /* -12*3600=-43200，字符串"-43200"长度为6个字节 */
#define TZSEC_VAL_MAX               (TZ_VAL_MAX*ONE_HOUR)
#define TZSEC_VAL_MIN               (TZ_VAL_MIN*ONE_HOUR)

#define set_bit(m,n)       m|=(1<<n)
#define reset_bit(m,n)     m &= ~( 1<<n )
#define get_bit( m,n )     ( (m &( 1<<n )) >> n )

int32 getKeyValueCustom(int8 *pBuf,const int8 *pKey,int8 **pValueStartAddr,int8 delimiter);
/**********************************************************
* @function getKeyValue
* @brief    根据关键字key在pBuf里查找对应的value
* @param[in]  pBuf -待查找的buf
* @param[in]  pKey -待查找的关键字
* @param[out] pValueStartAddr -存放查找到的value
* @return other -  value的长度
          -1    - 在pBuf中没有查找到key对应的value
* @see      在返回-1时，value值为NULL
**********************************************************/
int32 getKeyValue(int8 *pBuf,const int8 *pKey,int8 **pValueStartAddr);
int32 strToHex(int8 *dest, int8 *src);
void hexToStr( uint8 *dest,uint8 *src,int32 srcLen,int8 flag );
uint8* gatUpLoadDataCombine( uint16 cmd,uint8 *pszDID,uint8 flag,uint32 sn,uint8 *data,uint32 len,int32 *outLen );
int32 gatIofInet_addr( int8 *strIp, uint8* arrayIp4b );
_tm gatNtp2Wt( uint32 ntp,int32 tzSec );
char * encryptDisplay( uint8* buf,uint32 len );

#define gatDataLegalityCheck( Str ) (NULL==Str? (uint8*)"NULL":(uint8*)Str)

#endif
