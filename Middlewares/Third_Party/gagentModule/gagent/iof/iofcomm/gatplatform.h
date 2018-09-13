#ifndef GAT_PLATFORM_H_
#define GAT_PLATFORM_H_

//#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
//#include <sys/types.h>          /* See NOTES */
//#include <sys/stat.h>
//#include <sys/select.h>
//#include <fcntl.h>
//#include <sys/socket.h>
#include <inttypes.h>
//#include <netinet/in.h>
//#include <sys/un.h>
//#include <sys/time.h>
#include <time.h>
#include <math.h>
//#include <netdb.h>
#include <errno.h>
//#include <net/if.h>
//#include <sys/ioctl.h>
//#include <termios.h>
#include <signal.h>
//#include <sys/socket.h>
//#include <arpa/inet.h>
//#include <memory.h>
//#include <pthread.h>
#include <string.h>
#include <ctype.h>
//#include <sys/prctl.h>
//#include <sys/msg.h>
#include "iofdev.h"
#include "cmsis_os.h"
//鍑芥暟鏀惧湪ram 涓繍琛�
#define GAT_FUN_ATTR_RAM
//鍑芥暟鏀惧湪rom 涓繍琛�
#define GAT_FUN_ATTR_ROM

#define GAGENT_PATH_NAME    "./"


/********time define***********/
#define ONE_SECOND                      (1)

#define SDK_HARD_VERSION  "LinuxUbuntu_X86"
#define SDK_SOFT_VERSION  "1.0.23"

#define GAGENT_SOFTWARE_VER_SOC     "040301**"
#define GAGENT_SOFTWARE_VER         "04030001"
#define GAGENT_HARDWARE_VER         "00BL01A2"

#endif
