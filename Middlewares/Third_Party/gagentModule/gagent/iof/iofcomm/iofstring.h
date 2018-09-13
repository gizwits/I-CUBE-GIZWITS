#ifndef _IOFSTRING_H_
#define _IOFSTRING_H_
#include "gatplatform.h"
#include "log.h"
char *devMalloc( int size,const char *pName,const int line );
//char *mallocP;
//平台字符串适配接口
#define iofFree(addr)                   vPortFree(addr)           //void vPortFree( void *pv )
#define iofMalloc(len)                  pvPortMalloc(len)          //void *pvPortMalloc( size_t xSize )
#define iofMemset(s,ch,n)               memset(s,ch,n)
#define iofMemcpy(s,d,l)                memcpy(s,d,l)
#define iofMemcmp(s,d,l)                memcmp(s,d,l)

#define iofStrcmp(s1,s2)                strcmp(s1,s2)
#define iofStrncmp(s1,s2,n)             strncmp(s1,s2,n)
#define iofStrstr( haystack,needle)     strstr( haystack,needle)
#define iofStrcpy(s1,s2)                strcpy(s1,s2)
#define iofStrncpy(s1,s2,n)             strncpy(s1,s2,n)
#define iofStrlen( s )                  strlen(s)
#define iofStrtoul(nptr, endptr, base)  strtoul(nptr, endptr, base)
#define iofAtoi(str)                    atoi(str)
#define iofSprintf                      sprintf
#define iofSrand                        srand
#endif
