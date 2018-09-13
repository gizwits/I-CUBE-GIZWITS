#ifndef _GAT_ENV_H
#define _GAT_ENV_H

#define ENVINFO_ADDR            0
#define ENVINFO_SIZE            1024

/*********************** env key Word ***********************/

// #define KW_PK           "pk"               /* product key */
// #define KW_PS           "ps"               /* product secret */
// #define KW_PASSCODE     "pc"               /* passcode */
// #define KW_PD           "pd"               /* product did */
// #define kW_GSERVER      "gs"               /* GServer */
// #define KW_GSERVER_IP   "gi"               /* GServer ip */
// #define kW_GSERVER_PORT "gp"               /* GServer port */
// #define KW_OLD_PD       "opd"              /* old product did */
// #define KW_OLD_PASSCODE "opc"              /* old product passcode */
// #define KW_TZSEC        "tzs"              /* time zone secone */
// #define KW_WIFI_SSID1   "ws1"              /* wifi ssid 1 */
// #define KW_WIFI_BSSID1  "bsd1"             /* wifi bssid 1 */
// #define KW_WIFI_KEY1    "wk1"              /* wifi key 1 */
// #define KW_WIFI_SSID2   "ws2"              /* wifi ssid 2 */
// #define KW_WIFI_BSSID2  "bsd2"             /* wifi bssid 2 */
// #define KW_WIFI_KEY2    "wk2"              /* wifi key 2 */

//m2m相关环境变量的读写api
int8 *gatGetM2MDomain( void );
int8  gatSetM2MDomain( uint8 *szDoMain );
int32 gatGetM2MPort( void );
int8  gatSetM2MPort( int32 port );

int32 gatEnvInit();
int8 gatEnvWrite( uint8*pHead,uint32 headLen,uint8 *srcData,uint32 dataLen );
uint8 gatEnvRead( uint8*pHead,uint32 headLen,uint8 *destData,uint32 dataLen );
#endif /* _GAT_ENV_H */
