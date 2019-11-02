#ifndef __IAPUTIL_H
#define __IAPUTIL_H

#include "Public.h"

#define DEVTYPE		"数据记录仪"

typedef enum{
	IAPINIT,
	IAPLINK,
	JUMPTOAPP,
}_IapStatus;

typedef struct{
	_IapStatus Status;

	u8 erased;
	u8 linked;
	u8 updatePro;

	u64 iapTout;
	u64 curTick;
	u64 iapTick;

	u8 headRecved;
	u16 frameLen;
	u16 rxCnt;
	u16 txCnt;
	u8 rxDat[1024];
	u8 txDat[100];

	void(*SendIapMsg)(u8 *msg,u16 len);
	void(*OnIapMsgRecved)(u8 *msg,u16 len);
}_IapUtil;

extern _IapUtil IapUtil;

extern void IapTask(void);

#endif
