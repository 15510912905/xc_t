#ifndef __CARDCHECK_H
#define __CARDCHECK_H	 
#include "stm32f10x.h"
#include "sys.h"

#define LED_state PCout(1) //PA1
void CardCheck_Init(void);

#endif