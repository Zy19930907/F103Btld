//
// Created by zy on 2019/10/31.
//

#include "Public.h"

int main(void)
{
	Stm32_Clock_Init(9);
	my_mem_init();
	delay_init(72);
	Timer3Init(7999,8);
	LedInit();
	TasksInit();

	ExecTask();
}
