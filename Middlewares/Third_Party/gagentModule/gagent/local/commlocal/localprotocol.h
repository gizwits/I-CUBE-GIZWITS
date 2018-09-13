#ifndef LOCALPROTOCOL_H_
#define LOCALPROTOCOL_H_
#include "gattypes.h"
#include "localio.h"
#define HAL_BUF_SIZE            1024
#define HAL_BUF_MASK            (HAL_BUF_SIZE - 1)

#define MCU_SYNC_HEAD_LEN      2
#define MCU_LEN_POS            2
#define MCU_CMD_POS            4
#define MCU_SN_POS             5
#define MCU_FLAG_POS           6
#define MCU_ERROR_POS          8
#define MCU_HDR_LEN            8
#define MCU_LEN_NO_PAYLOAD     9
#define MCU_HDR_FF             0xFF
#define MCU_P0_POS             8
#define MCU_BYTES_NO_SUM        3
#define LOCAL_GAGENTSTATUS_MASK 0x1FFF

int32 GAT_FUN_ATTR_ROM localWriteP0(uint8 *plocalSn, uint8 *data, int32 len,uint8 cmd,uint16 flag,uint32 timeout_ms,void (* fun)(void* arg ) );
int8 GAT_FUN_ATTR_ROM localExtractPacket(uint8 *buf,uint32 dataLen,uint32 *offset,localRecvCache_t *recvCache);

#endif