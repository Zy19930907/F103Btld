#include "Uart.h"

_UartBuffer *uartBuffers[3] = {0,0,0};

void(*NoticeUart1Recved)(u8 *dat,u16 len);
void(*NoticeUart2Recved)(u8 *dat,u16 len);
void(*NoticeUart3Recved)(u8 *dat,u16 len);

static inline void Com1DmaInit(u16 len)
{
	RCC->AHBENR |= 0x01;
	uartBuffers[0] = mymalloc(sizeof(_UartBuffer));
	uartBuffers[0]->txBuf = mymalloc(100);
	uartBuffers[0]->buf1 = mymalloc(len);
	uartBuffers[0]->buf2 = mymalloc(len);
	uartBuffers[0]->len = len;

	//DMA_RX
	COM1RXCH->CPAR = (u32)&(USART1->DR); 											//外设地址
	COM1RXCH->CMAR = (u32)uartBuffers[0]->buf1;   							//存储器地址
	COM1RXCH->CNDTR = uartBuffers[0]->len;
	COM1RXCH->CCR = 0x00000000; 														//复位
	COM1RXCH->CCR |= BIT5;        															//执行循环操作
	COM1RXCH->CCR |= BIT7;       															//存储器地址递增
	COM1RXCH->CCR |= BIT12;   																//中等优先级
	MY_NVIC_Init(0, 2, DMA1_Channel5_IRQn, 0);
	COM1RXCH->CCR |= 0x03; 																//传输完成中断、通道使能

	//DMA_TX
	COM1TXCH->CPAR = (u32)&(USART1->DR); 									//外设地址
	COM1TXCH->CMAR = (u32)uartBuffers[0]->txBuf;           						//存储器地址
	COM1TXCH->CCR = 0x00000000;                           						//复位
	COM1TXCH->CCR |= BIT4;                           										//传输方向，从存储器读数据输出到外设
	COM1TXCH->CCR |= BIT7;                           										//存储器地址递增
	COM1TXCH->CCR |= BIT12;                           									//中等优先级
}

static inline void Com2DmaInit(u16 len)
{
	RCC->AHBENR |= 0x01;
	uartBuffers[1] = mymalloc(sizeof(_UartBuffer));
	uartBuffers[1]->txBuf = mymalloc(len);
	uartBuffers[1]->buf1 = mymalloc(len);
	uartBuffers[1]->len = len;
	//DMA_RX
	COM2RXCH->CPAR = (u32)&(USART2->DR); //外设地址
	COM2RXCH->CMAR = (u32)uartBuffers[1]->buf1;   //存储器地址
	COM2RXCH->CNDTR = uartBuffers[1]->len;
	COM2RXCH->CCR = 0x00000000; 	//复位
	COM2RXCH->CCR |= BIT5;        //执行循环操作
	COM2RXCH->CCR |= BIT7;        //存储器地址递增
	COM2RXCH->CCR |= BIT12;   	//中等优先级
//	MY_NVIC_Init(0, 2, DMA1_Channel6_IRQn, 0);
	COM2RXCH->CCR |= 0x01; //传输过半中断、传输完成中断、通道使能

	//DMA_TX
	COM2TXCH->CPAR = (u32) &(USART2->DR); //外设地址
	COM2TXCH->CMAR = (u32)uartBuffers[1]->txBuf;           //存储器地址
	COM2TXCH->CCR = 0x00000000;                           //复位
	COM2TXCH->CCR |= BIT4;                           //传输方向，从存储器读数据输出到外设
	COM2TXCH->CCR |= BIT7;                           //存储器地址递增
	COM2TXCH->CCR |= BIT12;                           //中等优先级
}

static inline void Com3DmaInit(u16 len)
{
	RCC->AHBENR |= 0x01;
	uartBuffers[2] = mymalloc(sizeof(_UartBuffer));
	uartBuffers[2]->txBuf = mymalloc(100);
	uartBuffers[2]->buf1 = mymalloc(len);
	uartBuffers[2]->buf2 = mymalloc(len);
	uartBuffers[2]->len = len;
	//DMA_RX
	COM3RXCH->CPAR = (u32)&(USART3->DR); //外设地址
	COM3RXCH->CMAR = (u32)uartBuffers[2]->buf1;   //存储器地址
	COM3RXCH->CNDTR = uartBuffers[2]->len;
	COM3RXCH->CCR = 0x00000000; 	//复位
	COM3RXCH->CCR |= BIT5;        //执行循环操作
	COM3RXCH->CCR |= BIT7;        //存储器地址递增
	COM3RXCH->CCR |= BIT12;   	//中等优先级
	MY_NVIC_Init(0, 2, DMA1_Channel3_IRQn, 0);
	COM3RXCH->CCR |= 0x03; //传输过半中断、传输完成中断、通道使能

	//DMA_TX
	COM3TXCH->CPAR = (u32) &(USART2->DR); //外设地址
	COM3TXCH->CMAR = (u32)uartBuffers[2]->txBuf;           //存储器地址
	COM3TXCH->CCR = 0x00000000;                  //复位
	COM3TXCH->CCR |= BIT4;                           //传输方向，从存储器读数据输出到外设
	COM3TXCH->CCR |= BIT7;                           //存储器地址递增
	COM3TXCH->CCR |= BIT12;                           //中等优先级
}

void Com1Init(u32 pclk2, u32 bound,u16 bufferLen,void(*OnRecved)(u8 *dat,u16 len))
{
	float temp;
	u16 mantissa;
	u16 fraction;

	NoticeUart1Recved = OnRecved;
	Com1DmaInit(bufferLen);

	temp = ((float) (pclk2 * 1000000)) / (bound<<4); //得到USARTDIV
	mantissa = temp;                                                                //得到整数部分
	fraction = (temp - mantissa) * 16;                                //得到小数部分
	mantissa <<= 4;
	mantissa += fraction;

	RCC->APB2ENR |= BIT14; //使能串口时钟
	RCC->APB2ENR |= BIT3;  //使能PB时钟
	RCC->APB2ENR |= BIT0;	//AFIO时钟
	AFIO->MAPR |= BIT2;		//串口1重映射到PB6、PB7
	GPIOB->CRL &= 0x00FFFFFF;
	GPIOB->CRL |= 0x4B000000;

	USART1->BRR = mantissa;   // 波特率设置

	USART1->CR3 |= 0xC0;
	USART1->CR1 |= 0x000C; //1位停止,无校验位.
	//使能接收中断
	USART1->CR1 |= BIT4;                                //idle中断
	MY_NVIC_Init(0, 1, USART1_IRQn, 0); //组2，最低优先级
	USART1->CR1 |= 0x2000;
}

//串口2初始化
void Com2Init(u32 pclk2, u32 bound,u16 bufferLen,void(*OnRecved)(u8 *dat,u16 len))
{
	float temp;
	u16 mantissa;
	u16 fraction;

	NoticeUart2Recved = OnRecved;
	Com2DmaInit(bufferLen);

	temp = ((float) ((pclk2 * 1000000)>>1)) / (bound * 16);      //得到USARTDIV
	mantissa = temp;                                                                                 //得到整数部分
	fraction = (temp - mantissa) * 16;                                                 //得到小数部分
	mantissa <<= 4;
	mantissa += fraction;
	RCC->APB2ENR |= (1 << 2);  //使能PORTA口时钟
	RCC->APB1ENR |= (1 << 17); //使能串口时钟
	GPIOA->CRL &= 0xFFFF00FF;
	GPIOA->CRL |= 0x00004B00; //IO状态设置

	RCC->APB1RSTR |= (1 << 17);  //复位串口2
	RCC->APB1RSTR &= ~(1 << 17); //停止复位
	//波特率设置
	USART2->BRR = mantissa;  // 波特率设置

	USART2->CR3 |= 0xC0;
	USART2->CR1 |= 0X200C; //1位停止,无校验位.

	//使能空闲中断
	USART2->CR1 |= (1 << 4);                                //idle中断
	MY_NVIC_Init(0, 4, USART2_IRQn, 0); //组2，最低优先级
}

//串口2初始化
void Com3Init(u32 pclk2, u32 bound,u16 bufferLen,void(*OnRecved)(u8 *dat,u16 len))
{
	float temp;
	u16 mantissa;
	u16 fraction;

	NoticeUart3Recved = OnRecved;
	Com3DmaInit(bufferLen);

	temp = ((float) ((pclk2 * 1000000)>>1)) / (bound * 16);      				//得到USARTDIV
	mantissa = temp;                                                                     //得到整数部分
	fraction = (temp - mantissa) * 16;                                                 //得到小数部分
	mantissa <<= 4;
	mantissa += fraction;
	RCC->APB2ENR |= (1 << 3);  //使能PORTB口时钟
	RCC->APB1ENR |= (1 << 18); //使能串口时钟
	GPIOB->CRH &= 0xFFFF00FF;
	GPIOB->CRH |= 0x00004B00; //IO状态设置

	RCC->APB1RSTR |= (1 << 18);  //复位串口3
	RCC->APB1RSTR &= ~(1 << 18); //停止复位
	//波特率设置
	USART3->BRR = mantissa;  // 波特率设置

	USART3->CR3 |= 0xC0;
	USART3->CR1 |= 0X200C; 													//1位停止,无校验位.

	//使能空闲中断
	USART3->CR1 |= (1 << 4);                                //idle中断
	MY_NVIC_Init(0, 4, USART3_IRQn, 0); //组2，最低优先级
}

// 往PC传输,将串口收到的数据搬移到缓冲区中
void DMA1_Channel5_IRQHandler(void)
{
	_UartBuffer *buffer = uartBuffers[0];
	if ((DMA1->ISR) & BIT17) //缓冲区全满
	{
		COM1RXCH->CCR &= ~0x01;
		if(buffer->curBuf==0)	//缓冲区1接收满，切换到缓冲区2继续接收数据
		{
			buffer->curBuf = 1;
			COM1RXCH->CMAR = (u32)buffer->buf2;
			COM1RXCH->CNDTR = buffer->len;
			COM1RXCH->CCR |= 0x01;	//数据拷贝前开始DMA传输，否则会漏数据
			NoticeUart1Recved(buffer->buf1,buffer->len);
		}else//缓冲区2接收满，切换到缓冲区1继续接收数据
		{
			buffer->curBuf = 0;
			COM1RXCH->CMAR = (u32)buffer->buf1;
			COM1RXCH->CNDTR = MAXDMALEN;
			COM1RXCH->CCR |= 0x01;	//数据拷贝前开始DMA传输，否则会漏数据
			NoticeUart1Recved(buffer->buf2,buffer->len);
		}
	}
	DMA1->IFCR = 0x000F0000;
}

// 往PC传输,将串口收到的数据搬移到缓冲区中
void DMA1_Channel6_IRQHandler(void)
{
	_UartBuffer *buffer = uartBuffers[1];
	if ((DMA1->ISR) & BIT21) //缓冲区全满
	{
		COM2RXCH->CCR &= ~0x01;
		if(buffer->curBuf==0)	//缓冲区1接收满，切换到缓冲区2继续接收数据
		{
			buffer->curBuf = 1;
			COM2RXCH->CMAR = (u32)buffer->buf2;
			COM2RXCH->CNDTR = buffer->len;
			COM2RXCH->CCR |= 0x01;	//数据拷贝前开始DMA传输，否则会漏数据
//			bufferUtil.PushBuffer(1,buffer->buf1,buffer->len);
		}else//缓冲区2接收满，切换到缓冲区1继续接收数据
		{
			buffer->curBuf = 0;
			COM2RXCH->CMAR = (u32)buffer->buf1;
			COM2RXCH->CNDTR = buffer->len;
			COM2RXCH->CCR |= 0x01;	//数据拷贝前开始DMA传输，否则会漏数据
//			bufferUtil.PushBuffer(1,buffer->buf1,buffer->len);
		}
	}
	DMA1->IFCR = 0x00F00000;
}

// 往PC传输,将串口收到的数据搬移到缓冲区中
void DMA1_Channel3_IRQHandler(void)
{
	_UartBuffer *buffer = uartBuffers[2];
	if ((DMA1->ISR) & BIT9) //缓冲区全满
	{
		COM3RXCH->CCR &= ~0x01;
		if(buffer->curBuf==0)	//缓冲区1接收满，切换到缓冲区2继续接收数据
		{
			buffer->curBuf = 1;
			COM3RXCH->CMAR = (u32)buffer->buf2;
			COM3RXCH->CNDTR = buffer->len;
			COM3RXCH->CCR |= 0x01;	//数据拷贝前开始DMA传输，否则会漏数据
			NoticeUart3Recved(buffer->buf1,buffer->len);
		}else//缓冲区2接收满，切换到缓冲区1继续接收数据
		{
			buffer->curBuf = 0;
			COM3RXCH->CMAR = (u32)buffer->buf1;
			COM3RXCH->CNDTR = buffer->len;
			COM3RXCH->CCR |= 0x01;	//数据拷贝前开始DMA传输，否则会漏数据
			NoticeUart3Recved(buffer->buf1,buffer->len);
		}
	}
	DMA1->IFCR = 0x00000F00;
}

void USART1_IRQHandler(void)
{
	u16 temp;
	_UartBuffer *buffer = uartBuffers[0];
	if (USART1->SR & BIT4) //空闲中断
	{
		COM1RXCH->CCR &= ~0x01;
		temp = USART1->SR;
		temp = USART1->DR;
		temp = buffer->len - COM1RXCH->CNDTR;
		if(buffer->curBuf == 0)
		{
			buffer->curBuf = 1;
			COM1RXCH->CMAR = (u32)buffer->buf2;
			COM1RXCH->CNDTR = buffer->len;
			COM1RXCH->CCR |= 0x01;
			NoticeUart1Recved(buffer->buf1,temp);
		}else
		{
			buffer->curBuf = 0;
			COM1RXCH->CMAR = (u32)buffer->buf1;
			COM1RXCH->CNDTR = buffer->len;
			COM1RXCH->CCR |= 0x01;
			NoticeUart1Recved(buffer->buf2,temp);
		}
	}
}

void USART2_IRQHandler(void)
{
	u16 temp;
	_UartBuffer *buffer = uartBuffers[1];
	if (USART2->SR & BIT4) //空闲中断
	{
		COM2RXCH->CCR &= ~0x01;
		temp = USART2->SR;
		temp = USART2->DR;
		temp = buffer->len - COM2RXCH->CNDTR;
		NoticeUart2Recved(buffer->buf1,temp);
		COM2RXCH->CMAR = (u32)buffer->buf1;
		COM2RXCH->CNDTR = buffer->len;
		COM2RXCH->CCR |= 0x01;
	}
}

void USART3_IRQHandler(void)
{
	u16 temp;
	_UartBuffer *buffer = uartBuffers[2];
	if (USART3->SR & BIT4) //空闲中断
	{
		COM3RXCH->CCR &= ~0x01;
		temp = USART3->SR;
		temp = USART3->DR;
		temp = buffer->len - COM3RXCH->CNDTR;
		if(buffer->curBuf == 0)
		{
			buffer->curBuf = 1;
			COM3RXCH->CMAR = (u32)buffer->buf2;
			COM3RXCH->CNDTR = buffer->len;
			COM3RXCH->CCR |= 0x01;
			NoticeUart3Recved(buffer->buf1,temp);
		}else
		{
			buffer->curBuf = 0;
			COM3RXCH->CMAR = (u32)buffer->buf1;
			COM3RXCH->CNDTR = buffer->len;
			COM3RXCH->CCR |= 0x01;
			NoticeUart3Recved(buffer->buf2,temp);
		}
	}
}

void Com1SendData(u8 *buf, u16 len)
{
	_UartBuffer *buffer = uartBuffers[0];
	DMA1->IFCR = (0x0F << 12);
	memcpy(buffer->txBuf, buf, len);
	COM1TXCH->CCR &= ~0x01;
	COM1TXCH->CMAR = (u32)buffer->txBuf;
	COM1TXCH->CNDTR = len;
	COM1TXCH->CCR |= 0x01; //开启数据传输
}

void Com2SendData(u8 *buf, u16 len)
{
	_UartBuffer *buffer = uartBuffers[1];
	DMA1->IFCR = (0x0F << 24);
	memcpy(buffer->txBuf, buf, len);
	COM2TXCH->CCR &= ~0x01;
	COM2TXCH->CMAR = (u32)buffer->txBuf;
	COM2TXCH->CNDTR = len;
	COM2TXCH->CCR |= 0x01; //开启数据传输
}
