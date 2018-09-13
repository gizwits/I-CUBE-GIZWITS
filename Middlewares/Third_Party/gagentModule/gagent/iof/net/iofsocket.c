#include "gatplatform.h"
#include "iofsocket.h"
#include "iofipc.h"
// #include "http.h"
// #include "lanTcp.h"
#include "network.h"
#include "wifi.h"
#include "tcp.h"

struct _udpAddr_t_ gatUdpAddr;

extern osSemaphoreId wifi_lock_sem;
extern osSemaphoreId tcp_recv_lock_sem;

uint32 SPI_Send_Count = 0;
Socket_Check socket_check[MAX_SOCKET_NUM];

osThreadId RecvCheckThreadHandler;
osSemaphoreId recv_check_lock_sem;
osSemaphoreDef(recv_check_lock_sem);

void recv_check_thread(void const * par);


void GAT_FUN_ATTR_ROM gatIofSocketClose(int32 fd)
{
  static int8 socket_close_count = 0;
  socket_close_count++;
  osSemaphoreWait(wifi_lock_sem, osWaitForever);
  WIFI_CloseClientConnection(fd);
  osSemaphoreRelease(wifi_lock_sem);
  socket_check[fd].active_socket = 0;
  socket_check[fd].socket_connected = 0;
}

int32 GAT_FUN_ATTR_ROM gatIofSocketOpen()
{
  int8 count = 0;
  static int8 recv_check_thread_init = 0;
  static int8 socket_count = 0;
  socket_count++;
  
  if(!recv_check_thread_init)
  {
    osThreadDef(RecvCheck, recv_check_thread, osPriorityNormal, 0, 1000);
    RecvCheckThreadHandler = osThreadCreate (osThread(RecvCheck), NULL);
   
    recv_check_lock_sem = osSemaphoreCreate(osSemaphore(recv_check_lock_sem) , 1 );
    
    recv_check_thread_init = 1;
  }
  
  while(socket_check[count].active_socket)
  {
    count++;
    if(count >= MAX_SOCKET_NUM)
      break;
  }
  
  if(count < MAX_SOCKET_NUM)
  {
    socket_check[count].active_socket = 1;
    return count;
  }
  else
  {
    return -1;
  }
}

int32 GAT_FUN_ATTR_ROM gatIofSocketConnect(int8 *szIp, int32 port, gatTcpConnCb cb)
{
    //struct sockaddr_in addr;
    int32 iRet;
//    int8 msg[32];
//    int32 msgLen;
    iofSig_t sig;
    int32 port_convert;
    static int8 tcp_failed_count = 0;
    
    int fd = gatIofSocketOpen();
    
    if(fd < 0)
    {
      sktPrintf(GAT_ERROR,"%s create socket fail\r\n",__FUNCTION__);
    }
    else
    {
//      struct sockaddr_in addr;
//      addr.sin_family = AF_INET;
//      addr.sin_port= htons(port);
//      addr.sin_addr.s_addr = inet_addr(szIp);
      port_convert = port;
      
      osSemaphoreWait(wifi_lock_sem, osWaitForever);
      iRet = WIFI_OpenClientConnection(fd, WIFI_TCP_PROTOCOL, "", (uint8_t*)szIp, port_convert, 0 );  
      osSemaphoreRelease(wifi_lock_sem);
//        osSemaphoreWait(wifi_lock_sem, osWaitForever);
//        iRet = WIFI_OpenClientConnection(fd, WIFI_TCP_PROTOCOL, "", (uint8_t*)szIp, port_convert, 0 );  
//        osSemaphoreRelease(wifi_lock_sem);
      if(iRet)
      {
        gatIofSocketClose(fd);
        fd = -1;
        printf("+++++++++++++++++++++++ Failed connection with iRet = %d, fd = %d +++++++++++++++++++++++\r\n", iRet, fd);
        
        tcp_failed_count++;
        if(tcp_failed_count == 10)
        {
          while(iRet)
          {
            printf("Reset WIFI module until resetting succeeds.\n");
            osSemaphoreWait(wifi_lock_sem, osWaitForever);
            iRet = SPI_WIFI_ResetModule();
            osSemaphoreRelease(wifi_lock_sem);
          }
          printf("WIFI module has been reset successfully.\n");
          tcp_failed_count = 0;
        }
      }
      else
      {
        printf("+++++++++++++++++++++++ Successful connection with iRet = %d, fd = %d +++++++++++++++++++++++\r\n", iRet, fd);
      }
    }

    if(NULL != cb)
    {
        if(fd < 0)
        {
            sig.val.conSigVal.result = GAT_ERR_FAIL;
        }
        else
        {
            sig.val.conSigVal.result = GAT_OK;
            socket_check[fd].socket_connected = 1;
        }
        sig.cmd = SIG_CMD_SKTCON;
        sig.val.conSigVal.fd = fd;
        sig.val.conSigVal.cb = cb;
        sktPrintf(GAT_NOTIC,"send conn cb:%p,fd:%d\r\n", cb, fd);
        iofSendSig(gatMsgId(), &sig);
    }
 
    return fd;
}

int32 GAT_FUN_ATTR_ROM gatIofTcpSend(int32 fd, int8 *data, int32 dataLen ,void *arg)
{
  uint16_t sent=0;
  int32 result;
  SPI_Send_Count++;

  osSemaphoreWait(wifi_lock_sem, osWaitForever);
  if ( WIFI_SendData(fd, (uint8_t *)data, dataLen, &sent, GAGENT_WIFI_WRITE_TIMEOUT)!= WIFI_STATUS_OK)
  {
    result = TCP_ERR_FAIL;
  }
  else
  {
    result = TCP_OK;
  }
  osSemaphoreRelease(wifi_lock_sem);
  
  gatTcpSendResult( fd, result, arg );
  
  return (int32)sent;
}

////no need
//int32 gatIofUdpSend(int32 fd, uint8 *data, int32 len, struct _udpAddr_t_ *to)
//{
//
//}

int32 GAT_FUN_ATTR_ROM gatIofSocketTcpRecv( int32 sockfd, void *buf, int32 len, int32 flags )
{
  uint16_t r = 0;
  WIFI_Status_t status = WIFI_STATUS_OK;
  
  osSemaphoreWait(wifi_lock_sem, osWaitForever);
  
  if (len > GAGENT_WIFI_PAYLOAD_SIZE) len = GAGENT_WIFI_PAYLOAD_SIZE;
  
  if(len > socket_check[sockfd].socket_read)
  {
    memcpy(buf, socket_check[sockfd].recv_buf, socket_check[sockfd].socket_read);
    len -= socket_check[sockfd].socket_read;
    
    status = WIFI_ReceiveData( sockfd, (uint8_t*)((int32)buf + socket_check[sockfd].socket_read), (uint16_t) len, &r, 200);
    
    if (status != WIFI_STATUS_OK)
    {
      printf("network_socket_recv(): status %d in WIFI_ReceiveData() - socket=%d requestedLen=%d received=%d\n",
             sockfd, sockfd, len, r);
      return -1;
    }
    
    r += socket_check[sockfd].socket_read;
    socket_check[sockfd].socket_read = 0;
    
    osSemaphoreRelease(wifi_lock_sem);
      
    return (int32)r;
  }
  else
  {
    memcpy(buf, socket_check[sockfd].recv_buf, len);
    osSemaphoreRelease(wifi_lock_sem);
    return len;
  }
}

//no need
//int32 GAT_FUN_ATTR_ROM gatIofSocketUdpRecv( int32 sockfd, void *buf, int32 len, int32 flags,
//                        gatSockAddr *src_addr, int32 *addrlen )
//{
//
//}

int32 GAT_FUN_ATTR_ROM gatIofSelect(int32 nfds, gat_fd_set *readfds, gat_fd_set *writefds,
                  gat_fd_set *exceptfds,int32 timeout_sec,int32 timeout_usec )
{
  osSemaphoreWait(tcp_recv_lock_sem, osWaitForever);
  return 1;
}

////no need
//int32 GAT_FUN_ATTR_ROM gatIofTcpCreateServer(uint16 port)
//{
//
//}
//
////no need
///*监听tcp server看是否有新客户端连接,返回新客户端的fd */
//int32 GAT_FUN_ATTR_ROM gatIofTcpServerListen(int32 fd)
//{
//
//}
//
////no need
//int32 GAT_FUN_ATTR_ROM gatCreateUDPServer( uint16 udp_port )
//{
//
//}
//
////no need
//int32 GAT_FUN_ATTR_ROM gatIofUdpCreateBCServer( uint32 port,udpAddr_t **socketAddr )
//{
//
//}

void GAT_FUN_ATTR_ROM gatTcpSendResult( int32 fd, int32 result, void *arg )
{
    iofSig_t sig;

    sig.cmd = SIG_CMD_TCPSEND_CB;
    sig.val.sktTxSigVal.fd = fd;//node->fd;
    // sig.val.sktTxSigVal.cb = node->sendCB;
    sig.val.sktTxSigVal.arg = arg;
    sig.val.sktTxSigVal.result = result;

    iofSendSig(gatMsgId(), &sig);
}

//no need
void GAT_FUN_ATTR_ROM gatIofSocketInit()
{

}

//no need
void GAT_FUN_ATTR_ROM gatIofTcpSocketInfoGet( )
{

}

////no need
//uint8* GAT_FUN_ATTR_ROM gatIofUdpSocketInfoGet( void *socketAddr,int32* port )
//{
//
//}
uint16 recv_check_count = 0;

void recv_check_thread(void const * par)
{
  int8 i, count;
  uint16 r;
  for(;;)
  {
    recv_check_count++;
    count = 0;
    osSemaphoreWait(recv_check_lock_sem, osWaitForever);
    for(i=0;i<MAX_SOCKET_NUM;i++)
    {
      osSemaphoreWait(wifi_lock_sem, osWaitForever);
      if(socket_check[i].active_socket)
      {
        if(socket_check[i].socket_connected)
        {
          WIFI_ReceiveData( (uint8)i, socket_check[i].recv_buf, 1024, &r, 1);
          if(r > 0)
          {
            socket_check[i].socket_read = r;
            count++;
          }
          else
          {
            socket_check[i].socket_read = 0;
          }
        }
      }
      osSemaphoreRelease(wifi_lock_sem);
    }
    
    if(count == 0)
    {
      osSemaphoreRelease(recv_check_lock_sem);
    }
    else
    {
      osSemaphoreRelease(tcp_recv_lock_sem);
    }

    osDelay(50);
  }
}
