#ifndef __IAPHWOPTS_H
#define __IAPHWOPTS_H

#include "Public.h"

#define FLASH_BASEADDR   0x08000000
#define FLASH_MAXADDR	0x0803FFFF

#define FLASH_BUSY		0x00000001
#define EARSEFLASH_PAGE	0x00000002	//页擦除模式
#define FLASH_PROGRAM	0x00000001	//Flash编程模式
#define FLASHEARSESTART	0x00000040	//开始擦除

#define LockFlash			FLASH->CR = 0x00000080//锁Flash
#define UnlockFlash		{FLASH->KEYR = 0x45670123;FLASH->KEYR = 0xCDEF89AB;}

#define APPSTARTADDR  		(u32)0x08005000	//App起始地址
#define APPENDADDR			(u32)0x0803FFFF	//APP结束地址

typedef struct{
	void(*InitIapDataline)(void);
	void(*EraseApp)(void);
	void(*WriteAppData)(u32 saddr,u8 *dat,u16 len);
	void(*JumpToApp)(void);
	void(*JumpToBoot)(void);
	void(*WriteAppFlag)(void);
	u8(*AppExist)(void);
	void(*SendIapMsg)(u8 *msg,u16 len);
}_IapHwOpts;
extern _IapHwOpts IapHwOpts;

#endif

