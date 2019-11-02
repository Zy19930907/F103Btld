#ifndef __CRCUTIL_H
#define __CRCUTIL_H

#include "Public.h"

typedef struct
{
	void(*SetCrc)(u8 *dat,u16 len);
	u8(*CheckCrc)(u8 *dat,u16 len);
}_CrcUtil;
extern _CrcUtil CrcUtil;

#endif
