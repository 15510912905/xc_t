#ifndef __LED_H
#define __LED_H	 
#include "stm32f10x.h"
#include "sys.h"

//LED端口定义
#define FLUORESCENCE_SCAN_LED PCout(6)////LED1 PCout(14)// PC9->pc1->pc14 //AD采集的灯 
#define LED2 PCout(2)//   pc 2PC0
#define LED3 PCout(3)//   pc 2PC0

#define LED_state PBout(5) //PC1 //PC13

// #define LED2 PCout(8)// PC8	
// #define LED3 PCout(7)// PC7	
// #define LED4 PCout(6)// PC6	

void LED_Init(void);//初始化

		 				    
#endif
