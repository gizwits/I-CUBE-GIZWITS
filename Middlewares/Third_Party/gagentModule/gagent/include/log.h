#ifndef LOG_H_
#define LOG_H_

#define GAT_CRITICAL    0X00
#define GAT_ERROR       0x01
#define GAT_WARNING     0X02
#define GAT_NOTIC       0X03
#define GAT_DEBUG       0x04
#define GAT_DUMP        0x05

/* GAgent 日志等级 通过该宏设置*/
#define GAT_LOGLEVEL GAT_DUMP
#define GAT_MEMLEVEL 0x06

/*这个函数是纯粹的打印,不受等级影响*/
#define Log(format, args...)  printf( format, ##args ) /*这个地方替换成平台相关的打印函数*/


#define assert()    Log("[func:%s][line:%d]param is illegal.\r\n", __FUNCTION__, __LINE__)

/* 打印日志,不添加额外的信息，等级为dump级别 */
#define dumpPrintf(format, args...)  \
{\
    if(GAT_LOGLEVEL==GAT_DUMP ) \
        Log( format, ##args ); \
}

#define dumpBuf( level,buf,len )\
{\
    if(GAT_LOGLEVEL>=level)\
    {   int i,j=1;\
        if( len>0 )\
        {\
            if(GAT_LOGLEVEL>=GAT_MEMLEVEL )\
            {\
             Log("[%06d]",dev_free_heap_size() );\
            }\
        Log("*********************** %04d **********************\n",(int)len );\
        for( i=0;i<len;i++ )\
        {\
         if( i!=0&&0==i%20 ) \
         {\
            Log("\r\n");\
         }\
         Log("%02x ",*((unsigned char*)((int32)buf+i)) );\
        }\
        Log("\r\n\r\n");\
        }\
    }\
}
/*m2m日志打印接口*/
#define m2mPrintf(level,format, args...) \
{\
    if(GAT_LOGLEVEL>=level) \
    {\
        if(GAT_LOGLEVEL>=GAT_MEMLEVEL )\
        {\
         Log("[%06d]",dev_free_heap_size() );\
        }\
        Log( "[M2M] "format, ##args);\
    }\
}
/*GServer日志打印接口*/
#define gServerPrintf(level,format, args...) \
{\
    if(GAT_LOGLEVEL>=level) \
    {\
        if(GAT_LOGLEVEL>=GAT_MEMLEVEL )\
        {\
         Log("[%06d]",dev_free_heap_size() );\
        }\
        Log("[GServer] "format, ##args);\
    }\
}
/*logServer日志打印接口*/
#define logServerPrintf(level,format, args...) \
{\
    if(GAT_LOGLEVEL>=level) \
    {\
        if(GAT_LOGLEVEL>=GAT_MEMLEVEL )\
        {\
         Log("[%06d]",dev_free_heap_size() );\
        }\
        Log("[LogServer] "format, ##args);\
    }\
}
/*小循环日志打印接口*/
#define lanPrintf(level,format, args...) \
{\
    if(GAT_LOGLEVEL>=level) \
    {\
        if(GAT_LOGLEVEL>=GAT_MEMLEVEL )\
        {\
         Log("[%06d]",dev_free_heap_size() );\
        }\
        Log("[Lan] "format, ##args);\
    }\
}
/*local日志打印接口 */
#define localPrintf(level,format, args...) \
{\
    if(GAT_LOGLEVEL>=level)\
    {\
        if(GAT_LOGLEVEL>=GAT_MEMLEVEL )\
        {\
         Log("[%06d]",dev_free_heap_size() );\
        }\
     Log("[Local] "format, ##args);\
    }\
}
/*wifi事件打印接口*/
#define wifiPrintf(level,format, args...) \
{\
    if(GAT_LOGLEVEL>=level)\
    {\
        if(GAT_LOGLEVEL>=GAT_MEMLEVEL )\
        {\
         Log("[%06d]",dev_free_heap_size() );\
        }\
         Log("[WiFiEvent] "format, ##args);\
    }\
}

/*定时器系统打印接口*/
#define timerPrintf(level,format, args...) \
{\
    if(GAT_LOGLEVEL>=level)\
    {\
        if(GAT_LOGLEVEL>=GAT_MEMLEVEL )\
        {\
         Log("[%06d]",dev_free_heap_size() );\
        }\
     Log("[Timer] "format, ##args);\
    }\
}
/*FS打印接口*/
#define fsPrintf(level,format, args...) \
{\
    if(GAT_LOGLEVEL>=level)\
    {\
        if(GAT_LOGLEVEL>=GAT_MEMLEVEL )\
        {\
         Log("[%06d]",dev_free_heap_size() );\
        }\
     Log("[FS] "format, ##args);\
    }\
}
/*平台底层相关信息打印接口*/
#define devPrintf(level,format,args...)\
{\
    if(GAT_LOGLEVEL>=level)\
    {\
        if(GAT_LOGLEVEL>=GAT_MEMLEVEL )\
        {\
         Log("[%06d]",dev_free_heap_size() );\
        }\
        Log("[Dev] "format,##args);\
    }\
}
/*GAgent应用层未定义模块信息打印接口*/
#define gatPrintf(level,format,args...)\
{\
    if(GAT_LOGLEVEL>=level)\
    {\
        if(GAT_LOGLEVEL>=GAT_MEMLEVEL )\
        {\
         Log("[%06d]",dev_free_heap_size() );\
        }\
        Log("[GAgent] "format,##args); \
    }\
}
/* 网络管理模块信息打印接口 */
#define nwkPrintf(level,format,args...)\
{\
    if(GAT_LOGLEVEL>=level)\
    {\
        if(GAT_LOGLEVEL>=GAT_MEMLEVEL )\
        {\
         Log("[%06d]",dev_free_heap_size() );\
        }\
        Log("[NWK] "format,##args); \
    }\
}

/* 故障管理模块信息打印接口 */
// #define frPrintf(level,format,args...)\
// {\
//     if(GAT_LOGLEVEL>=level)\
//     {\
//         if(GAT_LOGLEVEL>=GAT_MEMLEVEL )\
//         {\
//          Log("[%06d]",dev_free_heap_size() );\
//         }\
//         Log("[FR] "format,##args); \
//     }\
// }
/* OTA 模块信息打印接口 */
#define otaPrintf(level,format,args...)\
{\
    if(GAT_LOGLEVEL>=level)\
    {\
        if(GAT_LOGLEVEL>=GAT_MEMLEVEL )\
        {\
         Log("[%06d]",dev_free_heap_size() );\
        }\
        Log("[OTA] "format,##args); \
    }\
}
#define lanOtaPrintf(level,format,args...)\
{\
    if(GAT_LOGLEVEL>=level)\
    {\
        if(GAT_LOGLEVEL>=GAT_MEMLEVEL )\
        {\
         Log("[%06d]",dev_free_heap_size() );\
        }\
        Log("[LanOTA] "format,##args); \
    }\
}

#define sktPrintf(level,format,args...)\
{\
    if(GAT_LOGLEVEL>=level)\
    {\
        if(GAT_LOGLEVEL>=GAT_MEMLEVEL )\
        {\
         Log("[%06d]",dev_free_heap_size() );\
        }\
        Log("[Socket] "format,##args); \
    }\
}
/* sockt 层相关打印 */
#define httpPrintf(level,format, args...)\
{\
    if(GAT_LOGLEVEL>=level)\
    {\
        if(GAT_LOGLEVEL>=GAT_MEMLEVEL )\
        {\
         Log("[%06d]",dev_free_heap_size() );\
        }\
        Log("[http] "format,##args); \
    }\
}
/* mqtt 协议层打印 */
#define mqttPrintf(level,format, args...)\
{\
    if(GAT_LOGLEVEL>=level) \
    {\
        if(GAT_LOGLEVEL>=GAT_MEMLEVEL )\
        {\
         Log("[%06d]",dev_free_heap_size() );\
        }\
        Log( "[mqtt] "format, ##args);\
    }\
}
#define netPrintf(level,format, args...)\
{\
    if(GAT_LOGLEVEL>=level) \
    {\
        if(GAT_LOGLEVEL>=GAT_MEMLEVEL )\
        {\
         Log("[%06d]",dev_free_heap_size() );\
        }\
        Log( "[net] "format, ##args);\
    }\
}
#define PushPrintf(level,format, args...) \
{\
    if(GAT_LOGLEVEL>=level) \
    {\
        if(GAT_LOGLEVEL>=GAT_MEMLEVEL )\
        {\
         Log("[%06d]",dev_free_heap_size() );\
        }\
        Log( "[Push] "format, ##args);\
    }\
}
#define PullPrintf(level,format, args...) \
{\
    if(GAT_LOGLEVEL>=level) \
    {\
        if(GAT_LOGLEVEL>=GAT_MEMLEVEL )\
        {\
         Log("[%06d]",dev_free_heap_size() );\
        }\
        Log( "[Push] "format, ##args);\
    }\
}
/*平台底层相关信息打印接口*/
#define pthreadPrintf(level,format,args...)\
{\
    if(GAT_LOGLEVEL>=level)\
    {\
        if(GAT_LOGLEVEL>=GAT_MEMLEVEL )\
        {\
         Log("[%06d]",dev_free_heap_size() );\
        }\
        Log("[pthread] "format,##args);\
    }\
}
#endif
