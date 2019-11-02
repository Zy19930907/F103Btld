#ifndef __LED_H
#define __LED_H

#include "Public.h"

#define LED1ON      GPIOA->BRR  = BIT12
#define LED1OFF     GPIOA->BSRR = BIT12
#define LED1TOGGLE  GPIOA->ODR ^= BIT12

#define  RUNLEDON			GPIOB->BRR = BIT12
#define  RUNLEDTOGGLE		GPIOB->ODR ^= BIT12

#define LED2ON      GPIOA->BRR  = BIT11
#define LED2OFF     GPIOA->BSRR = BIT11
#define LED2TOGGLE  GPIOA->ODR ^= BIT11

extern void LedInit(void);
extern void RunLedTask(void);

#endif
