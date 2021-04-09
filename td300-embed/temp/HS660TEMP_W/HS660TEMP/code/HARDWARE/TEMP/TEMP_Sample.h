/******************************************************************************

                  ��Ȩ���� (C), 2013-2023, �Ĵ��³�����Ƽ����޹�˾

 ******************************************************************************
  �� �� ��   : TEMP_Sample.h
  �� �� ��   : ����
  ��������   : TEMP_Sample.c ��ͷ�ļ�
******************************************************************************/
#ifndef __TEMP_SAMPLE_H__
#define __TEMP_SAMPLE_H__

#include "stdint.h"	

#define MAX_ADVAL				4095.0
#define VREF_VOL				3.3          
#define DIV_RES         4.7        //�Ϸ�ѹ���� 4.7K

#define adtovoltage(advalue)  (float)((advalue*VREF_VOL)/MAX_ADVAL);
#define voltagetores(voltage) (((voltage)*DIV_RES)/(VREF_VOL-(voltage)));

extern void PrintFloat(float value);
float wash_sampling_linearcalc( float R, float tem1, float res1, float tem2, float res2 );
void  wash_sampling_RtoTconvert( float Res, float* temperature);
float wash_sampling_adtotemp(uint16_t adval);

#endif /*__TEMP_SAMPLE_H__*/
