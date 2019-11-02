#ifndef __UART_H
#define __UART_H

#include "Public.h"

#define COM1            	0
#define COM2           	1
#define COM3				2

#define COM1TXCH		DMA1_Channel4
#define COM1RXCH		DMA1_Channel5

#define COM2TXCH		DMA1_Channel7
#define COM2RXCH		DMA1_Channel6

#define COM3TXCH		DMA1_Channel2
#define COM3RXCH		DMA1_Channel3

#define MAXDMALEN		2048

typedef struct{
	u16 len;
	u8 curBuf;
	u8 *buf1;
	u8 *buf2;
	u8 *txBuf;
}_UartBuffer;

extern void Com1Init(u32 pclk2, u32 bound,u16 bufferLen,void(*OnRecved)(u8 *dat,u16 len));
extern void Com1SendData(u8 *buf,u16 len);

extern void Com2Init(u32 pclk2,u32 bound,u16 bufferLen,void(*OnRecved)(u8 *dat,u16 len));
extern void Com2SendData(u8 *buf,u16 len);

extern void Com3Init(u32 pclk2, u32 bound,u16 bufferLen,void(*OnRecved)(u8 *dat,u16 len));

#endif

