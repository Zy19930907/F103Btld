#include "CrcUtil.h"

#define CRCGENP   0xA001

static void SetCrc(u8 *dat,u16 len);
static u8 CheckCrc(u8 *dat,u16 len);

_CrcUtil CrcUtil = {
	.SetCrc = SetCrc,
	.CheckCrc = CheckCrc,
};
//计算数据CRC校验值
static u16 CalCrc16(u8 *read_buf,u16 len)
{
	u16 i,Crc16 = 0xFFFF;
	u8 j,k ;
	for(i=0;i<len;i++)
	{
		k = 0;
		Crc16 ^= *(read_buf + i);
		for(j=0;j<8;j++)
		{
			k = (Crc16 & 0x0001);
			Crc16 >>= 1;
			Crc16 &= 0x7FFF;
			if(k)
				Crc16 ^= CRCGENP;
		}
	}
	return Crc16;
}

static void SetCrc(u8 *dat,u16 len)
{
	u16 Crc16 = CalCrc16(dat,len-2);
	*(u16*)&dat[len-2] =  Crc16;
}

static u8 CheckCrc(u8 *dat,u16 len)
{
	u16 Crc16 = CalCrc16(dat,len-2);
	if(Crc16 == *(u16*)&dat[len-2])
		return 1;
	else
		return 0;
}
