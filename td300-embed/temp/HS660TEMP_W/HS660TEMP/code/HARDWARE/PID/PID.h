/******************************************************************************

                  ��Ȩ���� (C), 2013-2023, �Ĵ��³�����Ƽ����޹�˾

 ******************************************************************************
  �� �� ��   : pid.h
  �� �� ��   : ����
  ��    ��   : Gao.J
  ��������   : 2014��4��11��
  ����޸�   :
  ��������   : pid����ͷ�ļ�
  �����б�   :
  �޸���ʷ   :
  1.��    ��   : 2014��4��11��
    ��    ��   : Gao.J
    �޸�����   : �����ļ�

******************************************************************************/
#ifndef  __PID_H__
#define  __PID_H__

/*----------------------------------------------*
 * ����ͷ�ļ�                                   *
 *----------------------------------------------*/

#include "stdbool.h"
#include <stdio.h>
#include <stdint.h>
#include "switch.h"
//#include <stdlib.h>
//#include <math.h>

/*----------------------------------------------*
 * �ڲ�����ԭ��˵��                             *
 *----------------------------------------------*/

/*----------------------------------------------*
 * ģ�鼶����                                   *
 *----------------------------------------------*/

/*----------------------------------------------*
 * ��������                                     *
 *----------------------------------------------*/

/*----------------------------------------------*
 * �궨��                                       *
 *----------------------------------------------*/
#define 	DEFAULTPERIOD 		 1
#define		FULLDUTY   			 200


//�ж������¶ȷ�Χƫ��
#define TC_REACTEMP_OFFSET 10
#define TC_WASHTEMP_OFFSET 2

//Բ����
#define PI 3.14159
extern uint8_t heat_ch1_duty,heat_ch2_duty,heat_ch3_duty,heat_ch4_duty;


typedef enum 
{
    PRE_PROCESS=0,
    HIGH1,
    LOW1,
    HIGH2,
    LOW2,

    END
}TUNING_STAT;

typedef struct
{
    float setpoint; // �趨Ŀ��Desired value
    float kp; 		// ��������Proportional Const
    float ki; 		// ���ֳ���Integral Const
    float kd; 		// ΢�ֳ���Derivative Const
    float lasterr; 	// Error[-1]
    float preverr; 	// Error[-2]
    float sumerr; 	// Sums of Errors
}pid;

typedef struct
{
	//pid���ƽṹ��
    pid pid;
    //һ��pid��������ʱ��
    float period;
    //ռ�ձ�
    float duty;
	//����������
    TUNING_STAT status;
    float tuning_max;
    float tuning_min;
	  unsigned int tuning_period;
	  //��ֹ����
	  unsigned int jitter;
	  //��ص��ú���
	
	//��ص��ú���
    void (*delay)(float);
    void (*pulsehigh)(heat_channel);
    void (*pulselow)(heat_channel);
}pidtuningpara;
extern pidtuningpara reac_tunning;
//extern pidtuningpara reac_tuning, wash_tuning ,wash_tuning1, wash_tuning2, wash_tuning3,wash_tuning4; 

/*----------------------------------------------*
 * �������Ͷ���                                       *
 *----------------------------------------------*/
extern void pid_output(pidtuningpara *tuning);
extern void pid_init(pid* Control, float dsttemp, float p, float i, float d);
extern void pid_inittuningstru(pidtuningpara *tuning);
extern void pid_solo(pidtuningpara *tuning, float curtemp);
extern TUNING_STAT pid_tuning(pidtuningpara *tuning, float curtemp);
extern bool avoidjitter(pidtuningpara *tuning );

#endif

