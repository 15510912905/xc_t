#ifndef _PRODUCT_H__
#define _PRODUCT_H__

#include "sys.h" 

//定时器
#define Timer_VRT TIM2
#define Timer_RTT TIM3
#define Timer_INJ TIM4

//低电平使能
//#define p_MEnable   0
//#define p_MDisable  1
//#define p_MHoled    1
//#define p_MDisHoled 0
void ProductInit(void);

#endif
