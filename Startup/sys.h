#ifndef __SYS_H
#define __SYS_H

#include "stm32f103xe.h"

typedef __uint8_t u8;
typedef volatile __uint8_t vu8;
typedef __uint16_t u16;
typedef volatile __uint16_t vu16;
typedef __int16_t s16;
typedef __uint32_t u32;
typedef __int32_t  s32;
typedef volatile __uint32_t vu32;
typedef __uint64_t u64;

#define BIT0            (u32)0x00000001
#define BIT1            (u32)0x00000002
#define BIT2            (u32)0x00000004
#define BIT3            (u32)0x00000008
#define BIT4            (u32)0x00000010
#define BIT5            (u32)0x00000020
#define BIT6            (u32)0x00000040
#define BIT7            (u32)0x00000080
#define BIT8            (u32)0x00000100
#define BIT9            (u32)0x00000200
#define BIT10           (u32)0x00000400
#define BIT11           (u32)0x00000800
#define BIT12           (u32)0x00001000
#define BIT13           (u32)0x00002000
#define BIT14           (u32)0x00004000
#define BIT15           (u32)0x00008000
#define BIT16           (u32)0x00010000
#define BIT17           (u32)0x00020000
#define BIT18           (u32)0x00040000
#define BIT19           (u32)0x00080000
#define BIT20           (u32)0x00100000
#define BIT21           (u32)0x00200000
#define BIT22           (u32)0x00400000
#define BIT23           (u32)0x00800000
#define BIT24           (u32)0x01000000
#define BIT25           (u32)0x02000000
#define BIT26           (u32)0x04000000
#define BIT27           (u32)0x08000000
#define BIT28           (u32)0x10000000
#define BIT29           (u32)0x20000000
#define BIT30           (u32)0x40000000
#define BIT31           (u32)0x80000000

//Ex_NVIC_Config专用定义
#define GPIO_A 0
#define GPIO_B 1
#define GPIO_C 2
#define GPIO_D 3
#define GPIO_E 4
#define GPIO_F 5
#define GPIO_G 6
#define FTIR   1  //下降沿触发
#define RTIR   2  //上升沿触发


//JTAG模式设置定义
#define JTAG_SWD_DISABLE   0X02
#define SWD_ENABLE         0X01
#define JTAG_SWD_ENABLE    0X00

/////////////////////////////////////////////////////////////////
void Stm32_Clock_Init(u8 PLL);  //时钟初始化
void Stm32_Hsi_Init(u8 pll);
void Sys_Soft_Reset(void);      //系统软复位
void Sys_Standby(void);         //待机模式
void MY_NVIC_SetVectorTable(u32 NVIC_VectTab, u32 Offset);//设置偏移地址
void MY_NVIC_PriorityGroupConfig(u8 NVIC_Group);//设置NVIC分组
void MY_NVIC_Init(u8 NVIC_PreemptionPriority, u8 NVIC_SubPriority, u8 NVIC_Channel, u8 NVIC_Group);//设置中断
void Ex_NVIC_Config(u8 GPIOx, u8 BITx, u8 TRIM);//外部中断配置函数(只对GPIOA~G)
void JTAG_Set(u8 mode);

//////////////////////////////////////////////////////////////////////////////
//以下为汇编函数
void WFI_SET(void);                //执行WFI指令
void INTX_DISABLE(void);//关闭所有中断
void INTX_ENABLE(void);        //开启所有中断
void MSR_MSP(u32 addr);        //设置堆栈地址

u32 GetTimeRemain(void);
void TimeOutInit(u16 nms);
void delay_ms(u16 nms);
void delay_init(u8 SYSCLK);
void EnFlashReadProtect(void);

#endif











