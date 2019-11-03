#include "Led.h"

//初始化LED IO
void LedInit(void)
{
	RCC->APB2ENR |= BIT3;           //使能GPIOA时钟
	RCC->APB2ENR |= BIT3;           //使能GPIOA时钟

	GPIOB->CRH &= ~0x000F0000;
	GPIOB->CRH |= 0x00030000;
}

void RunLedTask(void)
{
	RUNLEDTOGGLE;
}
