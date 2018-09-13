#ifndef _GAGENT_TIMER_LIST_
#define _GAGENT_TIMER_LIST_
#include "gatlist.h"
#include "gattypes.h"
#include "iofipc.h"

#define SYS_MUTEXLOCK_SUPPORT
#define TIMER_HANDLE_MODE              TIMER_HANDLE_MULT
//系统是否支持多线程，若支持，资源采用线程锁
#ifdef  SYS_MUTEXLOCK_SUPPORT
#define TIMER_MUTEX_LOCK(x)      iofLock(&x)
#define TIMER_MUTEX_UNLOCK(x)    iofUnlock(&x)
#else
#define TIMER_MUTEX_LOCK(x)  ((x == TIMER_LOCK) ? -1 : (x = TIMER_LOCK))
#define TIMER_MUTEX_UNLOCK(x)  (x = TIMER_UNLOCK)
#endif

#define TIMER_UNLOCK                   0
#define TIMER_LOCK                     1
#define TIMER_HANDLE_SINGLE            0
#define TIMER_HANDLE_MULT              1

#define TIMER_EXPIRE_MAX               0xFFFFFFFF
#define TIMER_EXPOSE_MAX               0X7FFFFFFF

typedef struct _gagent_Timer_Core
{
    struct gatListHead timerList;
    uint32 count;               /* 创建了多少个timer，用于统计信息使用 */
    uint32 expose;              /* 定时器本次运行计时，到达一定值后清零 */
    lock_t lock;
}gagentTimerCore_st, *pGagentTimerCore_st;

typedef struct _gagent_timer_st
{
   struct gatListHead entry;
   gatTimer_st *appTimer;        /* 应用层定义的定时器成员 */

   // int32 (*timerCB)(struct _gagent_timer_st *timer); /* 回调函数返回值表明该回调被重新激活的时间 */
   uint32 expire;              /* 期望触发的时间，相对定时器系统时间 Core->expose 的相对时间 */
   // uint32 period;              /* 期望循环触发的周期, 绝对时间*/
   uint8 _linked;              /* 该节点是否已链入链表 */
   // char* szName;               /* 定时器名称 */
   // void* param;                /* 用作回调函数的参数或参数指针 */
}gatHalTimer_st;

typedef int32 (*timerCB)(gatTimer_st *pTimer);

/* 初始化定时器系统 */
void GAT_FUN_ATTR_ROM gatTimerCoreInit();

/* 添加定时器，将timer节点添加到定时器子系统timer链表中 */
int32 GAT_FUN_ATTR_ROM gatTimerAdd(gatTimer_st *pTimer,
                        char* szName,
                        uint32 expire,
                        timerCB functionCB,
                        void* param);

/* 将节点从链表中摘除 */
int32 GAT_FUN_ATTR_ROM gatTimerDel(gatTimer_st* pTimer);
/* 打印定时器表的信息*/
int32 GAT_FUN_ATTR_ROM gatTimerTraversal(void);

/* 将timer的触发时间推后*/

// /* 改变循环任务的周期 */
// int32 GAT_FUN_ATTR_ROM gagentTimerChangePeriodHp(gatTimer_st* pChangeTimer, uint32 cPeriod);
// int32 GAT_FUN_ATTR_ROM gagentTimerChangePeriodLp(gatTimer_st* pChangeTimer, uint32 cPeriod);

/* 定时器心跳 */
int32 GAT_FUN_ATTR_ROM gatTimerTick(uint32 dTime);

/* 定时器任务处理 */
int32 GAT_FUN_ATTR_ROM gatTimerHandle( uint8 flag, iofSig_t *pSig );

#endif
