#ifndef __SWITCH_H
#define __SWITCH_H	
 #include "stm32f10x_gpio.h"
#define SwitchHeat PBout(7)   //PA4 ->
typedef enum{
//	wash_heat_ch1	=	HEAT1_SWITCH,
//	wash_heat_ch2	=	HEAT2_SWITCH,
//	wash_heat_ch3	=	HEAT3_SWITCH,
	
	  reac_heat_ch1	=	GPIO_Pin_7,
//	reac_heat_ch2	=	HEAT5_SWITCH,
//	reac_heat_ch3	=	HEAT6_SWITCH,
//	
//	reac_heat_ch_butt
}heat_channel;

//加热器开关

void switch_Init(void) ;
extern void heat_Chx_on(void);
extern void heat_Chx_off(void);
extern void heat_delay1s( float seconds );

#endif
