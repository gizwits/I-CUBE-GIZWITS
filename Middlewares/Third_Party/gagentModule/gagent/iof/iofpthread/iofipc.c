
#include "gatplatform.h"
#include "iofipc.h"
#include "semaph.h"

osPoolDef(mpool_ptr, 16, 4);
osPoolId  mpool_ptr;
osPoolDef(mpool_data, 16, iofSig_t);
osPoolId  mpool_data;
 
//osMessageQDef(MsgBox, 16, 4);
//osMessageQId  MsgBox;
osMessageQDef(gatMsgIdValue, 16, 4);

osSemaphoreId msg_send_lock_sem;
osSemaphoreDef(msg_send_lock_sem);

int32 iofMsgCreat(int32 interKeyId)
{
  osMessageQId msgId;
  mpool_ptr = osPoolCreate(osPool(mpool_ptr));
  mpool_data = osPoolCreate(osPool(mpool_data));
  msgId = osMessageCreate(osMessageQ(gatMsgIdValue), NULL);
  msg_send_lock_sem = osSemaphoreCreate(osSemaphore(msg_send_lock_sem) , 1 );
  return (int32)msgId; 
}

int32 msg_send = 0, msg_rev = 0;

int32 iofSendSig(int32 taskId, iofSig_t *sig)
{
  osSemaphoreWait(msg_send_lock_sem, osWaitForever);
  iofSig_t **message; 
  message = (iofSig_t**)osPoolAlloc(mpool_ptr);
  *message = (iofSig_t*)osPoolAlloc(mpool_data);
  memcpy(*message, sig, sizeof(iofSig_t));
  //printf("msg send 1: id:%d \r\n",(uint32_t)(taskId) );
  dumpBuf( GAT_DEBUG, *message, sizeof(iofSig_t) );
  osMessagePut((osMessageQId)taskId, (uint32_t)message, osWaitForever);
  msg_send++;
  osSemaphoreRelease(msg_send_lock_sem);
  //printf("msg send 2: id:%d \r\n",(uint32_t)(taskId) );
  
  return GAT_OK;
}

int32 iofRecSig(int32 taskId, iofSig_t *sig)
{
  osEvent evt = osMessageGet((osMessageQId)taskId, osWaitForever);
  if (evt.status == osEventMessage) 
  {
    //printf("msg recv 1: id:%d \r\n",(uint32_t)(taskId) );
    msg_rev++;
    iofSig_t **message = (iofSig_t**)evt.value.p;
    memcpy(sig, *message, sizeof(iofSig_t));
    dumpBuf( GAT_DEBUG, sig, sizeof(iofSig_t) );
    //printf("msg recv 2: id:%d \r\n",(uint32_t)(taskId) );
    osPoolFree(mpool_data, *message);
    osPoolFree(mpool_ptr, message);
  }
  else
  {
    return -1;
  }
  
  return GAT_OK;
}



/* 信号量初始化 */
//will be called at iofLockInit()
int32 iofSemInit(lock_t *lock, int32 initVal, int32 semSubId)
{
  osSemaphoreId sema;
  
  if(semSubId != GAT_KEYID_TCPSEM_SYNC)
  {
    vSemaphoreCreateBinary(sema);
    if(!initVal)
    {
      if( xSemaphoreTake(sema, osWaitForever) != pdTRUE)
        return -1;
    }
  }
  else
  {
    sema = xSemaphoreCreateCounting(MAX_SEM_COUNT, initVal);
  }
  
  *lock = (lock_t)sema;
  
  return 0;   
}

int32 iofCountingSemInit(lock_t *lock, int32 initVal, int32 semSubId)
{
  osSemaphoreId sema;
  sema = xSemaphoreCreateCounting(MAX_SEM_COUNT, initVal);
  *lock = (lock_t)sema;
  return 0; 
}

int32 iofLockInit(lock_t *lock, int32 subId)
{
  iofSemInit(lock, 1, subId);
  
  return 0;
}

//获取到锁
int32 iofLock(lock_t *lock)
{
  if(NULL != lock)
    semOp(*lock, -1);
  return 1;
}

//释放锁成功
int32 iofUnlock(lock_t *lock)
{
  if(NULL != lock)
    semOp(*lock, 1);
  return 1;
}
