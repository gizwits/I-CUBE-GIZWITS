#ifndef _SEMAPH_H
#define _SEMAPH_H
#include "log.h"
#include "gattypes.h"
#define semPrintf(level,format, args...)\
{\
    if(GAT_LOGLEVEL>=level) \
        Log( "[SEM] "format, ##args);\
}


void semP(lock_t id);
void semV(lock_t id);
int semOp(lock_t id, int value);
//int semCreate(key_t, int);
//int semOpen(key_t);
void semRm(int);
void semClose(int);

void localTrigSend( lock_t localSemId );
void tcpTrigSend( lock_t SemId );
void udpTrigSend( lock_t udpSemId );

#endif /* _SEMAPH_H */
