#include "IapHwOpts.h"
#include "IapUtil.h"

typedef  void (*pFunction)(void);
pFunction Jump_To_Application;

static void WriteAppFlag(void)
{
	vu16 *fAddr = (vu16*)0x0802F000;
	UnlockFlash;
	FLASH->CR |= FLASH_PROGRAM;
	while(FLASH->SR & 0x01);//等待falsh空闲
	*fAddr++ = 0x5533;
	while(FLASH->SR & 0x01);//等待falsh空闲
	*fAddr = 0x5533;
	while(FLASH->SR & 0x01);//等待falsh空闲
	FLASH->CR &= ~FLASH_PROGRAM;
	LockFlash;
}

static u8 AppExist(void)
{
	if(*(u32*)0x0802F000 == 0x55335533)
		return 1;
	else
		return 0;
}
u32 test;
static void EraseApp(void)
{
	u8 EarseCnt,i;

	UnlockFlash;
	EarseCnt = (APPENDADDR-APPSTARTADDR) >> 11;//计算需要擦除的扇区个数
	FLASH->CR |= EARSEFLASH_PAGE;
	while(FLASH->SR & 0x01);//等待falsh空闲
	for(i = 0;i<EarseCnt;i++)
	{
		while(FLASH->SR & 0x01);//等待falsh空闲
		FLASH->AR = APPSTARTADDR+(i<<11);
		FLASH->CR |= FLASHEARSESTART;
	}
	FLASH->CR &= ~EARSEFLASH_PAGE;
	LockFlash;
}

static void WriteAppData(u32 saddr,u8 *dat,u16 len)
{
	u16 i=(len >> 1),j;
	saddr+=APPSTARTADDR;
	UnlockFlash;
	FLASH->CR |= FLASH_PROGRAM;
	while(FLASH->SR & 0x01);//等待falsh空闲
	for(j=0;j<i;j++)
	{
		*(vu16*)(saddr+(j<<1)) = *(u16*)(dat+(j<<1));
		while(FLASH->SR & 0x01);//等待falsh空闲
	}
	FLASH->CR &= ~FLASH_PROGRAM;//退出FLASH编程模式
	LockFlash;
}

static void JumpToApp(void)
{
	u32 JumpAddress;
	TIM3->CR1&=~0x01;
	JumpAddress = *(vu32*)(APPSTARTADDR+4);
	Jump_To_Application = (pFunction) JumpAddress;
	__set_MSP(*(vu32*) APPSTARTADDR);
	Jump_To_Application();
}

static void JumpToBoot(void)
{
	Sys_Soft_Reset();
}
static void SendIapMsg(u8 *msg,u16 len)
{
	Com2SendData(msg,len);
}

static void InitIapDataline(void)
{
	Com2Init(72,460800,1024,IapUtil.OnIapMsgRecved);
}

//static void CloseIapDataline(void)
//{
//	//CloseSocket(&socket);
//}

_IapHwOpts IapHwOpts = {
	.InitIapDataline = InitIapDataline,
	.EraseApp = EraseApp,
	.WriteAppData = WriteAppData,
	.JumpToApp = JumpToApp,
	.JumpToBoot = JumpToBoot,
	.WriteAppFlag = WriteAppFlag,
	.AppExist = AppExist,
	.SendIapMsg = SendIapMsg,
};