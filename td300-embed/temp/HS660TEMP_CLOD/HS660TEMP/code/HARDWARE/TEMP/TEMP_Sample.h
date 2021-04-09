/******************************************************************************

                  版权所有 (C), 2013-2023, 四川新成生物科技有限公司

 ******************************************************************************
  文 件 名   : TEMP_Sample.h
  版 本 号   : 初稿
  功能描述   : TEMP_Sample.c 的头文件
******************************************************************************/
#ifndef __TEMP_SAMPLE_H__
#define __TEMP_SAMPLE_H__

#include "stdint.h"	

#define MAX_ADVAL				4095.0
#define VREF_VOL				3.3          
#define DIV_RES         4.7        //上分压电阻 4.7K

#define adtovoltage(advalue)  (float)((advalue*VREF_VOL)/MAX_ADVAL);
#define voltagetores(voltage) (((voltage)*DIV_RES)/(VREF_VOL-(voltage)));

extern void PrintFloat(float value);
float wash_sampling_linearcalc( float R, float tem1, float res1, float tem2, float res2 );
void  wash_sampling_RtoTconvert( float Res, float* temperature);
float wash_sampling_adtotemp(uint16_t adval);

#endif /*__TEMP_SAMPLE_H__*/
