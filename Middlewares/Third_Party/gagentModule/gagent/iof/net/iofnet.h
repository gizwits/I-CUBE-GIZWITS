#ifndef IOF_NETINFO_H_
#define IOF_NETINFO_H_
#include "gatplatform.h"
#include "gattypes.h"

#define CACHE_SSID      "abc"
#define CACHE_PWD       "12345678"

#define STRMAC          "C47F5101045D"

#define SIGNAL_LEVEL_0                -100
#define SIGNAL_LEVEL_1                -80
#define SIGNAL_LEVEL_2                -75
#define SIGNAL_LEVEL_3                -70
#define SIGNAL_LEVEL_4                -65
#define SIGNAL_LEVEL_5                -60
#define SIGNAL_LEVEL_6                -55

#define APSCAN_NUM_MAX   30

uint8* GAT_FUN_ATTR_ROM gatNetCacheSSIDGet( void );
uint8* GAT_FUN_ATTR_ROM gatNetCachePwdGet( void );
uint8* GAT_FUN_ATTR_ROM gatNetCacheBSSIDGet( void );

uint8* GAT_FUN_ATTR_ROM gatNetBackupSSIDGet( void );
uint8* GAT_FUN_ATTR_ROM gatNetBackupPwdGet( void );
uint8* GAT_FUN_ATTR_ROM gatNetBackupBSSIDGet( void );

uint8 GAT_FUN_ATTR_ROM gatNetCacheSSIDSet( uint8 *szSSID );
uint8 GAT_FUN_ATTR_ROM gatNetCachePwdSet( uint8 *szPwd );
uint8 GAT_FUN_ATTR_ROM gatNetCacheBSSIDSet( uint8 *szBSSID );
uint8 GAT_FUN_ATTR_ROM gatNetBackupSSIDSet( uint8 *szSSID );
uint8 GAT_FUN_ATTR_ROM gatNetBackupPwdSet( uint8 *szPwd );
uint8 GAT_FUN_ATTR_ROM gatNetBackupBSSIDSet( uint8 *szBSSID );

int32 GAT_FUN_ATTR_ROM gatNetStationStart( uint8 *szSSID,uint8 *szPwd,uint8 *szMAC );
int32 GAT_FUN_ATTR_ROM gatNetStationStop( void );
int32 GAT_FUN_ATTR_ROM gatNetSoftApStart( uint8 *szSSID,uint8 *szPwd,uint8 *szIP,uint8 *szMask );
int32 GAT_FUN_ATTR_ROM gatNetSoftApStop( void );
uint8* GAT_FUN_ATTR_ROM gatNetMACGet( void );
int32 GAT_FUN_ATTR_ROM gatIofNetInit( void );
int8 GAT_FUN_ATTR_ROM gatOpenAirlink( void (* fun)(uint8 *szSSID,uint8 *szKey,uint8 *szBSSID) );
int32 GAT_FUN_ATTR_ROM gatCloseAirlink( void );
uint8* GAT_FUN_ATTR_ROM gatNetLocalIp( );
APList_t* GAT_FUN_ATTR_ROM gatWiFiScanResult( void );
int32 GAT_FUN_ATTR_ROM gatNetStartScan( void );
int32 GAT_FUN_ATTR_ROM iofGatGetHostByName( const uint8 *szName,void (dnsResultCb)(const uint8 *szName, const uint8 *szIP, void *arg) );
#endif
