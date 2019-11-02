#include "Timer.h"

//初始化定时器3
//arr自动重装值
//psc预分频值
void Timer3Init(u16 arr, u16 psc)
{
	RCC->APB1ENR |= 1 << 1;                //TIM3时钟使能
	TIM3->ARR = arr;                //设定计数器自动重装值
	TIM3->PSC = psc;                //预分频器

	TIM3->DIER |= 0x01;        //允许更新中断
	TIM3->DIER |= 1 << 6;        //允许触发中断
	TIM3->CR1 |= 0x01;
	MY_NVIC_Init(0, 3, TIM3_IRQn, 0);
}

void TIM3_IRQHandler(void)
{
	if (TIM3->SR & 0x0001)//溢出中断
	{
		IapUtil.iapTick++;
		TaskManger.tick++;
	}
	TIM3->SR &= 0xFE;//清除中断标志位
}
