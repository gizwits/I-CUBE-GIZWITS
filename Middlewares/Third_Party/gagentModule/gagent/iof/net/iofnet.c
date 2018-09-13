#include "iofnet.h"
#include "iofipc.h"
// #include "user_interface.h"
// #include "task/task.h"
#include "iofstring.h"
#include "gatnet.h"
#include "gagent.h"

extern osSemaphoreId wifi_lock_sem;

static APList_t  gS_APList;

//NULL function
//no need
//static void GAT_FUN_ATTR_ROM gatNetEventCb( void *evt )
//{
//
//}
//
////no need
//uint8* GAT_FUN_ATTR_ROM gatNetLocalIp( )
//{
//    
//}
uint8* GAT_FUN_ATTR_ROM gatNetCacheSSIDGet( void )
{
  return CACHE_SSID;
}
uint8* GAT_FUN_ATTR_ROM gatNetCachePwdGet( void )
{
  return CACHE_PWD;
}
uint8* GAT_FUN_ATTR_ROM gatNetCacheBSSIDGet( void )
{
  return CACHE_SSID;
}

uint8* GAT_FUN_ATTR_ROM gatNetBackupSSIDGet( void )
{
//  if( iofStrlen(gatNetConnParaGet()->ssid2)>0 )
//  {
//    return gatNetConnParaGet()->ssid2;
//  }
//  else
//  {
//    return NULL;
//  }
  return CACHE_SSID;
}
uint8* GAT_FUN_ATTR_ROM gatNetBackupPwdGet( void )
{
//  if( iofStrlen(gatNetConnParaGet()->key2)>0 )
//  {
//    return gatNetConnParaGet()->key2;
//  }
//  else
//  {
//    return NULL;
//  }
  return CACHE_PWD;
}
uint8* GAT_FUN_ATTR_ROM gatNetBackupBSSIDGet( void )
{
//  if( iofStrlen(gatNetConnParaGet()->bssid2)>0 )
//  {
//    return gatNetConnParaGet()->bssid2;
//  }
//  else
//  {
//    return NULL;
//  }
  return CACHE_SSID;
}

uint8 GAT_FUN_ATTR_ROM gatNetCacheSSIDSet( uint8 *szSSID )
{
  NetCon_t* pNetInfo = gatNetConnParaGet();
  if( szSSID!=NULL )
  {
    iofStrcpy( pNetInfo->ssid1,szSSID );
    return GAT_OK;
  }
  else
  {
    return GAT_ERR_FAIL;
  }
}
uint8 GAT_FUN_ATTR_ROM gatNetCachePwdSet( uint8 *szPwd )
{
  NetCon_t* pNetInfo = gatNetConnParaGet();
  if( szPwd!=NULL )
  {
    iofStrcpy( pNetInfo->key1,szPwd );
    return GAT_OK;
  }
  else
  {
    return GAT_ERR_FAIL;
  }
}
uint8 GAT_FUN_ATTR_ROM gatNetCacheBSSIDSet( uint8 *szBSSID )
{
  NetCon_t* pNetInfo = gatNetConnParaGet();
  if( szBSSID!=NULL )
  {
    iofStrcpy( pNetInfo->bssid1,szBSSID );
    return GAT_OK;
  }
  else
  {
    return GAT_ERR_FAIL;
  }
}

uint8 GAT_FUN_ATTR_ROM gatNetBackupSSIDSet( uint8 *szSSID )
{
  NetCon_t* pNetInfo = gatNetConnParaGet();
  if( szSSID!=NULL )
  {
    iofStrcpy( pNetInfo->ssid2,szSSID );
    return GAT_OK;
  }
  else
  {
    return GAT_ERR_FAIL;
  }
}
uint8 GAT_FUN_ATTR_ROM gatNetBackupPwdSet( uint8 *szPwd )
{
  NetCon_t* pNetInfo = gatNetConnParaGet();
  if( szPwd!=NULL )
  {
    iofStrcpy( pNetInfo->key2,szPwd );
    return GAT_OK;
  }
  else
  {
    return GAT_ERR_FAIL;
  }
}
uint8 GAT_FUN_ATTR_ROM gatNetBackupBSSIDSet( uint8 *szBSSID )
{
  NetCon_t* pNetInfo = gatNetConnParaGet();
  if( szBSSID!=NULL )
  {
    iofStrcpy( pNetInfo->bssid2,szBSSID );
    return GAT_OK;
  }
  else
  {
    return GAT_ERR_FAIL;
  }
}

int32 GAT_FUN_ATTR_ROM gatNetStationStart( uint8 *szSSID,uint8 *szPwd,uint8 *szBSSID )
{
  gatSetNwkEvent( NWK_WIFI_AP_DOWN );
  gatSetNwkEvent( NWK_WIFI_STA_UP );
  gatSetNwkEvent( NWK_WIFI_STA_CONNECTED );
  return GAT_OK;
}

int32 GAT_FUN_ATTR_ROM gatNetStationStop( void )
{
  return GAT_OK;
}
int32 GAT_FUN_ATTR_ROM gatNetSoftApStart( uint8 *szSSID,uint8 *szPwd,uint8 *szIP,uint8 *szMask )
{
  gatSetNwkEvent( NWK_WIFI_AP_UP );
  gatSetNwkEvent( NWK_WIFI_STA_DOWN );
  gatSetNwkEvent( NWK_WIFI_STA_DISCONNECTED );
  netPrintf( GAT_NOTIC,"%s SSID:%s Key:%s\n",__FUNCTION__,szSSID,szPwd );
  return GAT_OK;
}
int32 GAT_FUN_ATTR_ROM gatNetSoftApStop( void )
{
  return GAT_OK;
}

uint8 StrMac[17];

uint8* GAT_FUN_ATTR_ROM gatNetMACGet( void )
{
  uint8_t macAddress[6];
  uint8_t i;
  
  //depend on your platform to get mac
  WIFI_GetMAC_Address((uint8_t*)macAddress);
  
  for(i=0; i<6; i++)
  {
    sprintf((char*)&StrMac[2*i], "%02x", macAddress[i]);
  }
  
  for(i=12; i<17; i++)
  {
    StrMac[i] = '\0';
  }
  
  for(i=0; i<17; i++)
  {
    if(StrMac[i]>='a'&&StrMac[i]<='z')
      StrMac[i]-=32;
  }
  
  return StrMac;
}

int32 GAT_FUN_ATTR_ROM gatIofNetInit( void )
{
  int32 ret = GAT_OK;
  netPrintf( GAT_NOTIC,"%s ...... ok.\n",__FUNCTION__ );
  
#if 1
  if( GAT_REBOOT_INSOFTAPCONFIG==gatRunTimeDataGet()->rebootReason )
  {
    //iofSig_t sig;
    netPrintf( GAT_NOTIC,"gagent Reboot to start softap ...\r\n");
    netPrintf( GAT_NOTIC,"Please Wait Wi-Fi SoftAp Active ...\r\n");
    gatSaveRebootReason( GAT_REBOOT_INSOFTAPCONFIG );
    //netPrintf( GAT_NOTIC," wifi active ok .\r\n" );
    //gatSetNwkEvent( NWK_WIFI_AP_UP );
    //gatSetNwkEvent( NWK_WIFI_STA_DOWN );
    //gatSetNwkEvent( NWK_WIFI_ONBOARDING_UP );
    //sig.cmd = SIG_CMD_NETUP;
    //iofSendSig(gatMsgId(), &sig);
    ret = GAT_ERR_FAIL;
  }
  else if( GAT_REBOOT_TESTMODECONFIG==gatRunTimeDataGet()->rebootReason )
  {
    netPrintf( GAT_NOTIC,"gagent is in TEST MODE Start.\n" );
    gatStatusSet( WIFI_MODE_TEST,1 );
    ret = GAT_ERR_FAIL;
  }
  else if( GAT_REBOOT_BAKROUTER==gatRunTimeDataGet()->rebootReason )
  {
    netPrintf( GAT_NOTIC,"gagent is start to connect backup Router.\n" );
    ret = 1;
  }
  else if( GAT_REBOOT_EXCEP!=gatRunTimeDataGet()->rebootReason )
  {
    netPrintf( GAT_NOTIC,"%s %d ...\n",__FUNCTION__ ,__LINE__);
    gatSaveRebootReason( GAT_REBOOT_EXCEP );
  }
#endif
  return ret;
}
int8 GAT_FUN_ATTR_ROM gatIofNetGetRssi(void)
{
  return SIGNAL_LEVEL_6;
}

int8 GAT_FUN_ATTR_ROM gatNetGetSignalLevel( int8 rssi )
{
  if( rssi<=SIGNAL_LEVEL_0)
    return 0;
  else if( (rssi>SIGNAL_LEVEL_0)&&(rssi<SIGNAL_LEVEL_1))
    return 1;
  else if( (rssi>=SIGNAL_LEVEL_1)&&(rssi<SIGNAL_LEVEL_2))
    return 2;
  else if( (rssi>=SIGNAL_LEVEL_2)&&(rssi<SIGNAL_LEVEL_3))
    return 3;
  else if( (rssi>=SIGNAL_LEVEL_3)&&(rssi<SIGNAL_LEVEL_4))
    return 4;
  else if( (rssi>=SIGNAL_LEVEL_4)&&(rssi<SIGNAL_LEVEL_5))
    return 5;
  else if( (rssi>=SIGNAL_LEVEL_5)&&(rssi<SIGNAL_LEVEL_6))
    return 6;
  else if( rssi>=SIGNAL_LEVEL_6 )
    return 7;
  else
    return 0;
  
}
int8 GAT_FUN_ATTR_ROM gatOpenAirlink( void (* fun)(uint8 *szSSID,uint8 *szKey,uint8 *szBSSID) )
{
  fun( CACHE_SSID,CACHE_PWD,NULL );
  
  return GAT_OK;
}

int32 GAT_FUN_ATTR_ROM gatCloseAirlink( void )
{
  return GAT_OK;
}

APList_t* GAT_FUN_ATTR_ROM gatWiFiScanResult( void )
{
  return &gS_APList;
}

void GAT_FUN_ATTR_ROM iofWifiScan(APList_t* gAplist)
{
  netPrintf( GAT_NOTIC,"%s ...... ok.\n",__FUNCTION__ );
  gAplist->ApNum = 1;
  iofMemcpy(gAplist->ApList[0].ssid,GAT_TEST_AP1,iofStrlen(GAT_TEST_AP1));
  gAplist->ApList[0].ApPower =  100;
}

int32 GAT_FUN_ATTR_ROM gatNetStartScan( void )
{
  netPrintf( GAT_NOTIC,"Wi-Fi Scan Start ......\r\n" );
  if( !gS_APList.ApList )
  {
    gS_APList.ApList = iofMalloc( APSCAN_NUM_MAX*sizeof(ApAttr_t) );
    if( NULL==gS_APList.ApList )
    {
      netPrintf( GAT_ERROR,"in ap mode malloc %d fail .\r\n", APSCAN_NUM_MAX*sizeof(ApAttr_t) );
      return GAT_ERR_FAIL;
    }
  }
  else
  {
    netPrintf( GAT_NOTIC,"gS_APList is already malloc .\r\n" );
  }
  iofWifiScan(&gS_APList);//  ≈‰∆ΩÃ®ssid…®√ËAPI
  netPrintf( GAT_NOTIC,"%s ...... ok.\n",__FUNCTION__ );
  return GAT_OK;
}
uint32 GAT_FUN_ATTR_ROM iofGetHostByName(const int8 *domain, uint8 *strIp)
{
  osSemaphoreWait(wifi_lock_sem, osWaitForever);
  if (WIFI_GetHostAddress((char*) domain, strIp)!= 0)
  {
    osSemaphoreRelease(wifi_lock_sem);
    return -1;
  }
  osSemaphoreRelease(wifi_lock_sem);
  return 0;
}
int32 GAT_FUN_ATTR_ROM iofGatGetHostByName( const uint8 *szName,void (dnsResultCb)(const uint8 *szName, const uint8 *szIP, void *arg) )
{
  int32 ret =0;
  uint8 szTempIp[17]={0};
  if( NULL==szName || NULL==dnsResultCb )
  {
    netPrintf( GAT_NOTIC,"%s ,parma invaild.\r\n",__FUNCTION__ );
    return GAT_ERR_PARAM;
  }
  ret = iofGetHostByName( szName, szTempIp );
  if( ret != 0 )
  {
    dnsResultCb( szName,NULL,NULL );
    return GAT_ERR_FAIL;
  }
  else
  {
    //iofSprintf( szTempIp,"%d.%d.%d.%d",*((uint8 *)&ipaddr.addr),*((uint8 *)&ipaddr.addr+1),*((uint8 *)&ipaddr.addr+2),*((uint8 *)&ipaddr.addr+3) );
    dnsResultCb( szName,szTempIp,NULL );
    return GAT_OK;
  }
}