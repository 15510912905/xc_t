#include "task.h"	 
#include "led.h" 
//#include "key.h"
#include "cmd.h"
#include "cmd_funs.h"
#include "product.h"
#include "eeprom_config.h"
#include "delay.h"
#include "application.h"
int i=0;
//闪灯
//void Ledbleek()
//{
//        LED_Init();
//        FLUORESCENCE_SCAN_LED = !FLUORESCENCE_SCAN_LED;
//			  LED_state=!LED_state;
//        delay_ms(1000);
//}

////垂直电机动
//void vertical_motor_move(){
//	while(1){
//  MVRT_EN=0;
//	MVRT_Step=!MVRT_Step;
//	MVRT_Dir=!MVRT_Dir;
//	i++;
//	MVRT_CT=0;
//	delay_us(22);
//	}

//}

////水平电机动
//void  horizontal_motor_move(){
//	
//   MRTT_EN=0;
//	 MRTT_Step=! MRTT_Step;
//	 MRTT_Dir=!MRTT_Dir;
//	 MRTT_CT=0;
//	delay_us(22);

//}

//void  inject_motor_move(){
//	
//   MINJ_EN=0;
//	 MINJ_Step=! MINJ_Step;
//	 MINJ_Dir= MINJ_Dir;
//	 MINJ_CT=0;
//	delay_us(22);

//}


