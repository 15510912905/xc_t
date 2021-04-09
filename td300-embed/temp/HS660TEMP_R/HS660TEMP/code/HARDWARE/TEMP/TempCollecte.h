#ifndef __TEMPCOLLECTE_H
#define __TEMPCOLLECTE_H	 
#include "sys.h"
#include "delay.h"
#include "stm32f10x_adc.h"
#include "stm32f10x.h"

//#define MAX_ADVAL				4095
//#define VREF_VOL				3.3          
//#define DIV_RES                 10        //�Ϸ�ѹ���� 10K

//#define adtovoltage(advalue)  ((advalue)*VREF_VOL/MAX_ADVAL); //��ADֵת��Ϊ��ѹ
//#define voltagetores(voltage) (((voltage)*DIV_RES)/(VREF_VOL-(voltage)));//��ADֵת��λ����ֵ

void Adc_Init(void);
u16  Get_Adc(u8 ch); 
u16 Get_Adc_Average(u8 ch,u8 times); 
extern uint8_t ISCardExist(void);

		 				    
#endif
