#ifndef _IOF_TCP_H
#define _IOF_TCP_H
#include "gattypes.h"

typedef struct
{
  volatile uint16 socket_read;
  volatile uint8 active_socket;
  volatile uint8 socket_connected;
  uint8 recv_buf[1200];
}Socket_Check;

#define MAX_SOCKET_NUM          4
//#define sockaddr_t sockaddr_in

#define TCP_SKTBUF_SIZE         4096    /* TCP 底层缓存区大小设置 */
#define TCP_LISTEN_MAX          8

#define  GAGENT_WIFI_WRITE_TIMEOUT   1000
#define  GAGENT_WIFI_READ_TIMEOUT    1000

#define  GAGENT_WIFI_PAYLOAD_SIZE    1200

typedef enum SOCKETTYPE_T
{
    TCP_SERVER=0,
    TCP_CLIENT_LOCAL, //tcp 客户端连接到GAgent类型
    TCP_CLIENT_REMOTE, //tcp 客户端连接到远程服务器类型
    UDP_SERVER,
    UDP_CLIENT,
}sktType_t;

extern Socket_Check socket_check[4];

void gatIofSocketClose(int32 fd);
int32 gatIofSocketConnect(int8 *szIp, int32 port, gatTcpConnCb cb);
int32 gatIofSelect(int32 nfds, gat_fd_set *readfds, gat_fd_set *writefds,gat_fd_set *exceptfds,int32 timeout_sec,int32 timeout_usec );
int32 gatIofTcpSend(int32 fd, int8 *data, int32 dataLen, void *arg);
int32 gatIofSocketUdpRecv( int32 sockfd, void *buf, int32 len, int32 flags,gatSockAddr *src_addr, int32 *addrlen );
int32 gatIofSocketTcpRecv( int32 sockfd, void *buf, int32 len, int32 flags );
int32 gatIofUdpSend(int32 fd, uint8 *data, int32 len, struct _udpAddr_t_ *to);
uint8* gatIofUdpSocketInfoGet( void *socketAddr,int32* port );
int32 gatIofTcpCreateServer(uint16 port);
int32 gatIofTcpServerListen(int32 fd);
int32 gatIofUdpCreateBCServer( uint32 port,udpAddr_t **socketAddr );
void gatTcpSendResult( int32 fd, int32 result, void *arg );
#endif /* _IOF_TCP_H */
