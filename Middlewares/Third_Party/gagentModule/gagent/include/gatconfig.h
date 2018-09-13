#ifndef GAT_CONFIG_H_
#define GAT_CONFIG_H_

#define LOCAL_STATUS_SYNC_INTERVAL_S        (10*ONE_MINUTE)
#define LOCAL_HEARTBEAT_INTERVAL_S          (55)
#define LOCAL_SIGNALCHECK_INTERVAL_S		(60*3)
#define LOCAL_ACK_TIME_MS                   (200)

#define CLOUT_TIME_REREQ_MAX                (600)/* 10分钟 */
#define GSERVER_EXCEP_TIMEOUT               (60*ONE_MINUTE)
#define GSERVER_RECONNECT_INTERVAL          (10*ONE_SECOND)
#define PROVISION_REGET_INTERVAL            (60*ONE_MINUTE)
#define GSERVER_RECEVICE_TIMEOUT            (5*ONE_SECOND)
/* GAgent GServer 重连次数 10次 */
#define GSERVER_RECONNECT_NUM               (10)
#define GAGENT_TEST_TIME_S                  (180*ONE_SECOND)
/* 无app连接超过2分钟重启。默认2分钟 */
#define NET_UNREACH_REBOOT_NOCLIENT_TIME    (2*ONE_MINUTE)


/*For GAgent Defined SoftAP*/
#define AP_NAME             "XPG-GAgent-"
#define AP_PASSWORD         "123456789"
#define AP_LOCAL_IP         "10.10.100.254"
#define AP_NET_MASK         "255.255.255.0"
#define ADDRESS_POOL_START  "10.10.100.240"
#define ADDRESS_POOL_END    "10.10.100.255"
#define GAT_TEST_AP1        "GIZWITS_TEST_1"
#define GAT_TEST_AP_PASS    "GIZWITS_TEST_PASS"
/* head(4) | len(Min 1) | flag(1) | cmd(2) */
#define HI_PACKETLEN_MIN            8
#define GAGENT_PROTOCOL_VERSION     (0x00000003)

/* Macro of Cloud Server */
#define GAGENT_DEFAULT_GSERVER       "api.gizwits.com"
#define GAGENT_CNAPI_GSERVER         "api.gizwits.com"
#define GAGENT_USAPI_GSERVER         "usapi.gizwits.com"
#define GAGENT_EUAPI_GSERVER         "euapi.gizwits.com"

#define GAGENT_WECHAT_DEVICE_TYPE   "gh_35dd1e10ab57"

#define GAGENT_DEFAULT_PORT         80

/* Macro of module LAN */
#define HIPROVER                        "4.1.4"
#define GAGENT_TCP_SERVER_PORT          (12416)
#define LAN_UDP_SERVER_PORT             (12414)
#define LAN_UDP_BROADCAST_SERVER_PORT   (2415)
#define SEND_UDP_TIMES                  (30)

/*For data buf defined */
#define SIMULATOR_UPLOAD_SIZE      (1024)

/* For Dev Defined */
#define NET_ETH               "ens33"
#define GAT_SERIAL            "/dev/ttyUSB0"
//#define GAT_BPS                9600
#endif
