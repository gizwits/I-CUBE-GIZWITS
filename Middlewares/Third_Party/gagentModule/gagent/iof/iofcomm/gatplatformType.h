#ifndef GATPLATFORMTYPE_H_
#define GATPLATFORMTYPE_H_
#include "gatplatform.h"
#include "iofstring.h"
//平台相关数据类型定义

typedef struct {
  int a;
}pthread_attr_t;

typedef struct {
  int a;
}pthread_t;

typedef struct {
  int a;
}fd_set_n;

//typedef struct {
//  int a;
//}key_t;    //鍙笌淇″彿閲忔湁鍏�

//typedef int  key_t;

typedef fd_set_n              gat_fd_set;
typedef struct sockaddr     gatSockAddr;
typedef pthread_t           gatpthread_t;
typedef pthread_attr_t      gatpthread_attr_t;
typedef osTimerId           gatDevTimer_t;

//#define GAT_FD_ZERO     FD_ZERO
//#define GAT_FD_SET      FD_SET
//#define GAT_FD_ISSET    FD_ISSET
//#define GAT_FD_CLR      FD_CLR
#define GAT_FD_ZERO     
#define GAT_FD_SET      
#define GAT_FD_ISSET(fd, ptr)    (socket_check[fd].socket_read > 0)
#define GAT_FD_CLR      
#endif
