#ifndef GATBASE64_H_
#define GATBASE64_H_
#include "gattypes.h"
void gatBase64Encode( int32 dataLen, const uint8 *pData, int8 **ppBase64Str );
#endif