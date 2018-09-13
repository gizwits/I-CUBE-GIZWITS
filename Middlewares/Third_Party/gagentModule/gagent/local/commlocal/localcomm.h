#ifndef _LOCAL_H
#define _LOCAL_H
#include "dp.h"
#include "gattypes.h"
#include "local.h"
#define MCU_P0_LEN              2
#define MCU_CMD_LEN             2
#define MCU_ATTR_USED_BIT       8
typedef void (*localUserRecDataCb)( cliId_t *pCli, char *data, int len,void *arg );
typedef int32 (*localWriteAdapter_st) (uint8 *plocalSn, uint8 *data, int32 len,uint8 cmd,uint16 flag,uint32 timeout_ms,void (* fun)(void* arg ) );

typedef struct _mcuInfo_st
{
    uint8 uartProtoVer[MCU_PROTOCOLVER_LEN+1];
    uint8 businessProtoVer[MCU_P0VER_LEN+1];
    uint8 mcuHardVer[MCU_HARDVER_LEN+1];
    uint8 mcuSoftVer[MCU_SOFTVER_LEN+1];
    uint8 pk[PK_LEN+1];
    uint16 bindTimeout;
    uint8 enableBindFlag;//允许绑定Flag
    uint8 mcuAttr[MCUATTR_LEN];
    uint8 pks[PKS_LEN+1];
}mcuInfo_st;

/*V4 CMD of P0*/
#define MCU_INFO_CMD                    0X01
#define MCU_INFO_CMD_ACK                0X02

#define MCU_CTRL_CMD                    0X03
#define MCU_CTRL_CMD_ACK                0X04

#define MCU_REPORT                      0X05
#define MCU_REPORT_ACK                  0X06

#define WIFI_PING2MCU                   0X07
#define WIFI_PING2MCU_ACK               0X08

#define MCU_CONFIG_WIFI                 0X09
#define MCU_CONFIG_WIFI_ACK             0X0A

#define MCU_RESET_WIFI                  0X0B
#define MCU_RESET_WIFI_ACK              0X0C

#define WIFI_STATUS2MCU                 0X0D
#define WIFI_STATUS2MCU_ACK             0X0E

#define MCU_DATA_ILLEGAL                0x11
#define MCU_REPLY_GAGENT_DATA_ILLEGAL   0x12

#define WIFI_TEST                       0x13
#define WIFI_TEST_ACK                   0x14

#define MCU_ENABLE_BIND                 0x15
#define MCU_ENABLE_BIND_ACK             0x16

#define MCU_REQ_GSERVER_TIME            0x17
#define MCU_REQ_GSERVER_TIME_ACK        0x18

#define MCU_NEED_UPGRADE                0x19
#define MCU_NEED_UPGRADE_ACK            0x1A

#define MCU_READY_RECV_FIRMWARE         0x1B
#define MCU_READY_RECV_FIRMWARE_ACK     0x1C

#define GAGENT_SEND_BIGDATA             0x1D
#define GAGENT_SEND_BIGDATA_ACK         0x1E

#define GAGENT_STOP_SEND                0x1F
#define GAGENT_STOP_SEND_ACK            0x20

#define MCU_QUERY_WIFI_INFO             0x21
#define MCU_QUERY_WIFI_INFO_ACK         0x22

#define MCU_TRANSCTION_REQUEST          0x23
#define MCU_TRANSCTION_REQUEST_ACK      0x24

#define MCU_TRANSCTION_RESULT           0x25
#define MCU_TRANSCTION_RESULT_ACK       0x26

#define MCU_STOP_RECV_BIGDATA           0x27
#define MCU_STOP_RECV_BIGDATA_ACK       0x28

#define MCU_RESTART_GAGENT              0x29
#define MCU_RESTART_GAGENT_ACK          0x2A

//mcu模拟器登陆云端
#define MCU2GAGENT_LOGIN_CLOUD                  0X2D
#define MCU2GAGENT_LOGIN_CLOUD_ACK              0X2E
//mcu模拟器登陆云端结果
#define GAGENT2MCU_CONN_LOGIN_RESULT           0X2F
#define GAGENT2MCU_CONN_LOGIN_RESULT_ACK       0X30
//mcu模拟器上传数据到客户端
#define MCU2GAGENT_UPLOAD_DATA                 0x31
#define MCU2GAGENT_UPLOAD_DATA_ACK             0x32
//mcu模拟器上传数据到客户端结果
#define GAGENT2MCU_UPLOAD_DATA_RESUT           0X33
#define GAGENT2MCU_UPLOAD_DATA_RESUT_ACK       0X34
//mcu模拟器登出云端
#define MCU2GAGENT_LONGOUT_CLOUD               0X35
#define MCU2GAGENT_LONGOUT_CLOUD_ACK           0X36
//mcu模拟器登出云端结果
#define GAGENT2MCU_LONGOUT_CLOUD_RESULT        0X37
#define GAGENT2MCU_LONGOUT_CLOUD_RESULT_ACK    0X38
//mcu模拟器解除用户与设备的绑定
#define MCU2GAGENT_UNBIND                      0X39
#define MCU2GAGENT_UNBIND_ACK                  0X3A
//mcu模拟器解除用户与设备的绑定的结果
#define GAGENT2MCU_UNBIND_RESULT               0X3B
#define GAGENT2MCU_UNBIND_RESULT_ACK           0X3C

/***** 设备互动指令 *****/
//mcu 通知gagent 进入子设备扫描
#define MCU2GAGENT_SUBDEV_SCAN                 0X3D
#define MCU2GAGENT_SUBDEV_SCAN_ACK             0X3E
//GAgent 通知MCU 获取到的子设备信息
#define GAGENT2MCU_SUBDEV_INFO                 0X3F
#define GAGENT2MCU_SUBDEV_INFO_ACK             0X40
//GAgent 通知MCU 子设备状态
#define GAGENT2MCU_SUBDEV_STATUS               0x41
#define GAGENT2MCU_SUBDEV_STATUS_ACK           0x42
//MCU通知GAgent 控制子设备
#define MCU2GAGENT_SUBDEV_CTRL                 0X43
#define MCU2GAGENT_SUBDEV_CTRL_ACK             0X44

//GAgent 通知MCU 需要预授权
#define GAGENT2MCU_AUTHKEY_REQ                 0X45
#define GAGENT2MCU_AUTHKEY_REQ_ACK             0X46
//MCU 通知GAgent 写授权秘钥
#define MCU2GAGENT_AUTHKEY                     0X47
#define MCU2GAGENT_AUTHKEY_ACK                 0X48

#define MCU_GAGENT_GPS_REQUEST                 0X4B
#define MCU_GAGENT_GPS_REQUEST_ACK             0X4C
#define MCU_GAGENT_MODULE_CHECK                0X4D
#define MCU_GAGENT_MODULE_CHECK_ACK            0X4E
#define GAGENT_LOCAL_CMD_MAX                   0X50



#define GAGENT_MCU_CHECKSUM_ERROR           0x01
#define GAGENT_MCU_CMD_ERROR                0x02
#define GAGENT_MCU_OTHER_ERROR              0x03
#define GAGENT_MCU_FILETPYE_ERROR           0x04


extern localWriteAdapter_st localWriteAdapter;

void gatLocalInit();
mcuInfo_st *gatGetMcuInfo(void);
void gatAuthCbFun( int32 result );
void gatFactoryMode( cliId_t *pCli, int8 *data, int32 len,void *arg );
void gatEnableBindStart( cliId_t *pCli, int8 *data, int32 len,void *arg );
void gatEnterTest( cliId_t *pCli, int8 *data, int32 len,void *arg );
void gatFactoryMode( cliId_t *pCli, int8 *data, int32 len,void *arg );
void gatLocalModuleCheck( cliId_t *pCli, int8 *data, int32 len,void *arg );
void gatLocalGAgentRestart( cliId_t *pCli, int8 *data, int32 len,void *arg );
int32 GAT_FUN_ATTR_ROM gatLocalSignalCheckTick( gatTimer_st *pTimer );
void GAT_FUN_ATTR_ROM gatLocalSignalTickStart( void );
void GAT_FUN_ATTR_ROM gatLocalSignalTickClose( void );
#endif  //_LOCAL_H
