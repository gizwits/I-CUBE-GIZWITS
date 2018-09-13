#include "gatplatformType.h"
#include "gattypes.h"
#include "iofstring.h"

int8 *strnstr(const int8 *p, const int8 *strObj, int32 n)
{
    int32 len = strlen(strObj);
    int32 i = 0;
    int32 j = 0;

    if(len <= 0)
    {
        return (int8 *)p;
    }\
    if(len > n)
    {
        return NULL;
    }

    for(i = 0; i < n; i++)
    {
        if(p[i] == 0)
        {
            return NULL;
        }

        for(j = 0; j < len; j++)
        {
            if(p[i + j] != strObj[j])
                break;
        }
        if(j == len)
        {
            /*  */
            return (int8 *)(p + i);
        }
    }

    return NULL;
}

char *devMalloc( int32 size,const char *pName,const int line )
{
    char *pst=NULL;
    pst = malloc( size );
    Log("[%06d][M.m %p] %s[%d] len=%d \r\n",dev_free_heap_size(),pst,pName,line,size);
    if( pst )
    {
        return pst;
    }
    return NULL;
}