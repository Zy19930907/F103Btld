#include "IapUtil.h"
#include "IapHwOpts.h"

static void OnIapMsgRecved(u8 *msg,u16 len);

_IapUtil IapUtil = {
	.iapTout = 5000,
	.linked = 0,
	.erased = 0,
	.OnIapMsgRecved = OnIapMsgRecved,
};

static void FrameReset(void)
{
	IapUtil.headRecved = 0;
	IapUtil.rxCnt = 0;
	IapUtil.frameLen = 0;
	memset(IapUtil.rxDat,'\0',1024);
}

void SendIapMsg(u8 ack,u8 *msg,u16 len)
{
	u16 i;
	*(u32*)IapUtil.txDat = 0x55335533;
	*(u16*)&IapUtil.txDat[4] = 9+len;
	IapUtil.txDat[6] = ack;
	for(i=0;i<len;i++)
		IapUtil.txDat[7+i] = *(msg+i);
	CrcUtil.SetCrc(IapUtil.txDat,9+len);
	IapHwOpts.SendIapMsg(IapUtil.txDat,9+len);
}

static void IapFrameDeal(u8 type,u8 *frame,u16 len)
{
	switch(type)
	{
		case 0x01://服务器对设备注册响应
			IapUtil.iapTout = 60000;
			IapUtil.linked = 1;		//标记服务器成功连接
			break;
		case 0x02://开始更新指令
			SendIapMsg(0x02,(u8*)"",0);
			IapHwOpts.EraseApp();
			IapUtil.iapTout = 5000;
			delay_ms(10);
			SendIapMsg(0x03,(u8*)"",0);
			break;
		case 0x03://接收到程序数据
			IapHwOpts.WriteAppData(*(u32*)&frame[1],&frame[7],*(u16*)&frame[5]);
			IapUtil.updatePro = frame[0];
			SendIapMsg(0x03,(u8*)"",0);
			break;
		case 0x04://上位机发送程序数据完成
			IapHwOpts.WriteAppFlag();
			IapUtil.Status = JUMPTOAPP;		//不能在中断中进行程序跳转
			break;
	}
	IapUtil.curTick = IapUtil.iapTick;
}

void OnIapMsgRecved(u8 *msg,u16 len)
{
	u16 i;
	IapUtil.curTick = IapUtil.iapTick;
	if(!IapUtil.headRecved && (*(u32*)msg != 0x55335533))//未接收到帧头
	{
		SendIapMsg(0xFE,(u8*)"",0);//数据错误，请求服务器重发
		FrameReset();
		return;
	}

	if(*(u32*)msg == 0x55335533)
	{
		IapUtil.headRecved = 1;
		IapUtil.rxCnt = 0;
	}

	if(IapUtil.headRecved)
	{
		for(i=0;i<len;i++)
			IapUtil.rxDat[IapUtil.rxCnt++] = msg[i];
		if(IapUtil.rxCnt >= 6)
			IapUtil.frameLen = *(u16*)&IapUtil.rxDat[4];
		if(IapUtil.rxCnt == IapUtil.frameLen)//收到一包完整数据帧
		{
			if(CrcUtil.CheckCrc(IapUtil.rxDat,IapUtil.rxCnt))
			{
				IapFrameDeal(IapUtil.rxDat[6],&IapUtil.rxDat[7],IapUtil.frameLen - 9);
				FrameReset();
			}
			else
			{
				SendIapMsg(0xFE,(u8*)"",0);//CRC校验错误，请求服务器重发
				FrameReset();
			}
		}
	}
}

//计算任务执行时钟
static inline u64 IapTickDiff(u64 tick)
{
	unsigned long systick = IapUtil.iapTick;
	if(tick > systick)
		return 0xFFFFFFFFFFFFFFFF - tick + systick;
	else
		return systick - tick;
}

void IapTask(void)
{
	switch(IapUtil.Status)
	{
		case IAPINIT:
			IapHwOpts.InitIapDataline();
			IapUtil.curTick = IapUtil.iapTick;
			IapUtil.Status = IAPLINK;
			break;

		case IAPLINK:
			if(!IapUtil.linked)
				SendIapMsg(0x01,(u8*)DEVTYPE,strlen(DEVTYPE));//上报设备类型
			if(IapTickDiff(IapUtil.curTick) >= IapUtil.iapTout)
			{
				if(IapHwOpts.AppExist())
					IapHwOpts.JumpToApp();
				else
					IapHwOpts.JumpToBoot();
			}
			break;

		case JUMPTOAPP:
			SendIapMsg(0x04,(u8*)"",0);
			delay_ms(100);
			IapHwOpts.JumpToApp();
			break;
	}
}
