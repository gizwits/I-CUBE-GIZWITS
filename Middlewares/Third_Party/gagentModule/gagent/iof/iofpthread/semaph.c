//#include <sys/ipc.h>
//#include <sys/sem.h>
#include <errno.h>
#include "semaph.h"

///****************************************************************************
// * Create a semaphore with a specified initial value.
// * If the semaphore already exists, we don't initialize it (of course).
// * We return the semaphore ID if all OK, else -1.
// */
////no need
//int semCreate(key_t key, int initval)
//{
//
//}
//
///****************************************************************************
// * Open a semaphore that must already exist.
// * This function should be used, instead of semCreate(), if the caller
// * knows that the semaphore must already exist.  For example a client
// * from a client-server pair would use this, if its the server's
// * responsibility to create the semaphore.
// * We return the semaphore ID if all OK, else -1.
// */
////no need
//int semOpen(key_t key)
//{
//
//}

/****************************************************************************
 * Remove a semaphore.
 * This call is intended to be called by a server, for example,
 * when it is being shut down, as we do an IPC_RMID on the semaphore,
 * regardless whether other processes may be using it or not.
 * Most other processes should use semClose() below.
 */
//no need
void semRm(int id)
{

}

/****************************************************************************
 * Close a semaphore.
 * Unlike the remove function above, this function is for a process
 * to call before it exits, when it is done with the semaphore.
 * We "decrement" the counter of processes using the semaphore, and
 * if this was the last one, we can remove the semaphore.
 */
//no need
void semClose(int id)
{

}

/****************************************************************************
 * General semaphore operation.  Increment or decrement by a user-specified
 * amount (positive or negative; amount can't be zero).
 */
int semOp(lock_t id, int value)
{
   if(value == 1)
     xSemaphoreGive( (osSemaphoreId)id );
   else if(value == -1)
   {
     if( xSemaphoreTake( (osSemaphoreId)id, osWaitForever ) != pdTRUE )
       return -1;
   }
   else
     return -1;
   
   return 0;
}


/****************************************************************************
 * Wait until a semaphore's value is greater than 0, then decrement
 * it by 1 and return.
 * Dijkstra's P operation.  Tanenbaum's DOWN operation.
 */
void semP(lock_t id)
{
  semOp(id, -1);
}

/****************************************************************************
* Increment a semaphore by 1.
* Dijkstra's V operation.  Tanenbaum's UP operation.
*/
void semV(lock_t id)
{
  semOp(id, 1);
}

/* 每次添加数据时调用该函数 */
void localTrigSend( lock_t localSemId )
{
    semPrintf( GAT_DEBUG,"localSemId:%d\r\n", localSemId);
    if( localSemId >=0 )
    {
        semV( localSemId );
        semPrintf(GAT_DEBUG, "sem v\r\n");
    }
    else
    {
        semPrintf(GAT_ERROR, "%s sem v error\r\n",__FUNCTION__ );
    }
}

uint16 tcpTrigcount = 0;

void tcpTrigSend( lock_t SemId )
{
  tcpTrigcount++;
  semPrintf( GAT_ERROR,"tcpSemId:%d\r\n", SemId);
  if(SemId >=0 )
  {
    semV(SemId);
  }
  else
  {
    semPrintf(GAT_ERROR, "%s sem v error\r\n",__FUNCTION__ );
  }  
}

//no need
void udpTrigSend( lock_t udpSemId )
{
    semPrintf( GAT_DEBUG,"udpSemId:%d\r\n", udpSemId);
    if(udpSemId >=0 )
    {
        semV(udpSemId);
    }
    else
    {
        semPrintf(GAT_ERROR, "%s sem v error\r\n",__FUNCTION__ );
    }
}
