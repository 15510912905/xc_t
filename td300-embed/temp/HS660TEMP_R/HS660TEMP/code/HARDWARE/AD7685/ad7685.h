#ifndef __AD7685_H__
#define __AD7685_H__

#include "includes.h"

#define CNV_HIGH    GPIO_SetBits(GPIOA, GPIO_Pin_4) //PB6->PA15->PA4
#define CNV_LOW     GPIO_ResetBits(GPIOA, GPIO_Pin_4) //pb6->pa15->PA4

#define DUMMY   0xFFFF

void AD7685_GPIO_Config(void);
void LIGHT_LED_Config(void);
void AD7685_SPI_Config(void);

uint16_t AD7685_oneAcq(uint16_t tData);

extern uint16_t read_color(void);

#endif
