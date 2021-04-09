/******************************************************************************

                  ��Ȩ���� (C), 2013-2023, �Ĵ��³�����Ƽ����޹�˾

 ******************************************************************************
  �� �� ��   : pid.c
  �� �� ��   : ����
  ��    ��   : PengChuan
  ��������   : 2014��4��11��
  ����޸�   :
  ��������   : pid����c�ļ�
  �����б�   :
  �޸���ʷ   :
  1.��    ��   : 2016��9��11��
    ��    ��   : PengChuan
    �޸�����   : �޸��ļ�

******************************************************************************/

/*----------------------------------------------*
 * ����ͷ�ļ�                                   *
 *----------------------------------------------*/
#include "pid.h"
#include "float.h"
#include "switch.h"
//#include <math.h>
//#include "USART.h"
//#include "user_conf.h"
//#include <string.h>
/*----------------------------------------------*
 * �ڲ�����ԭ��˵��                             *
 *----------------------------------------------*/

/*----------------------------------------------*
 * ģ�鼶����                                   *
 *----------------------------------------------*/
// pidtuningpara reac_tuning, wash_tuning, wash_tuning1, wash_tuning2, wash_tuning3,wash_tuning4;
uint8_t heat_ch1_duty;
pidtuningpara reac_tunning;

/*----------------------------------------------*
 * ��������                                     *
 *----------------------------------------------*/
 

/*----------------------------------------------*
 * �궨��                                       *
 *----------------------------------------------*/ 
 
/*****************************************************************************
 �� �� ��  : pid_init
 ��������  : PID�ṹ��ʼ��
 �������  : pid* Control
 				float dsttemp
              	float p
             	float i
             	float d
 �������  : ��
 �� �� ֵ  : void
 
 �޸���ʷ      :
  1.��    ��   : 2014��5��27��
    ��    ��   : Gao.J

    �޸�����   : �����ɺ���

*****************************************************************************/
void pid_init(pid *Control, float dsttemp, float p, float i, float d)
{
    memset(Control, 0, sizeof(pid));
    //����Ŀ���¶�
    Control->setpoint = dsttemp;

    //�趨PID����
    Control->kp = p;
    Control->ki = i;
    Control->kd = d;
	
}
/*****************************************************************************
 �� �� ��  : pid_inittuningstru
 ��������  : ��ʼ��PID�����������ṹ��
 �������  : pidtuningpara (*tuning)
 �������  : ��
 �� �� ֵ  : void
 
 �޸���ʷ      :
  1.��    ��   : 2014��5��27��
    ��    ��   : Gao.J

    �޸�����   : �����ɺ���

*****************************************************************************/
void pid_inittuningstru(pidtuningpara *tuning)
{
    tuning->period = DEFAULTPERIOD;
    tuning->status = PRE_PROCESS;
    tuning->tuning_max = 0;   //�����޸ģ����ڵ�һ�αȽ��ж�
    tuning->tuning_min = 100;   //�����޸ģ����ڵ�һ�αȽ��ж�
	  tuning->jitter = 0;
	
	 tuning->delay = heat_delay1s;
   tuning->pulsehigh = heat_Chx_on;
   tuning->pulselow  = heat_Chx_off;
}

float ArrayDuty[4] = {100, 50, 50, 50};  //�����޷��ٷֱ�
float ArrayErr[4] = {3, 1, 2, 2};  //��������ƫ��
/*****************************************************************************
 �� �� ��  : pid_calculate
 ��������  : ���ݲ���ֵ��PID�����������ֵ
 �������  : pidtuningpara *tuning 
               float currentTemp
 �������  : ��
 �� �� ֵ  : float
 
 �޸���ʷ      :
  1.��    ��   : 2014��09��15��
    ��    ��   : PengChuan

    �޸�����   : �����ɺ���

*****************************************************************************/
float pid_calculate(pidtuningpara *tuning, float currentTemp)
{
    float kp = 0.0;
    float ki = 0.0;
    float kd = 0.0;
    float sum = 0.0;
    float sumerr = 0.0;
    int index = 0;
    
    tuning->pid.preverr = tuning->pid.lasterr;
    tuning->pid.lasterr = tuning->pid.setpoint - currentTemp;
    tuning->pid.sumerr += tuning->pid.lasterr;
    
//    if(&reac_tuning == tuning)
//    {
//        index = 0;
//    }
//    else if(&wash_tuning1 == tuning)
//    {
//        index = 1;
//    } 
//    else if(&wash_tuning2 == tuning)
//    {
//        index = 2;
//    }
//    else if(&wash_tuning3 == tuning)
//    {
//        index = 3;
//    }
    
    if(fabs(tuning->pid.lasterr) > ArrayErr[index]) //��������
    {
        tuning->pid.sumerr = 0;
    }
    
    if(fabs(tuning->pid.ki) > FLT_EPSILON)
    {
        sumerr = (2 * ArrayDuty[index]/tuning->pid.ki);
    }
    else
    {
        sumerr = 0.0;
    }

    if(tuning->pid.sumerr > sumerr) 
    {
        tuning->pid.sumerr = sumerr;
    }
    else if(tuning->pid.sumerr < -sumerr)
    {
        tuning->pid.sumerr = -sumerr;
    }
    
    kp = tuning->pid.kp * tuning->pid.lasterr;
    ki = tuning->pid.ki * tuning->pid.sumerr;
    kd = tuning->pid.kd * (tuning->pid.lasterr - tuning->pid.preverr);

    sum = kp + ki + kd;
    if (sum <= 0)
    {
        return 0;
    }
    else if (sum >= FULLDUTY)
    {
        return FULLDUTY;
    }
    else
    {
        return sum;
    }
}

/*****************************************************************************
 �� �� ��  : pid_para
 ��������  : ����ku��tu����PID���ڲ�����ѡ��Ϊ�򵥵��ٽ�����ȷ�����Tp=0.6*ku��
 			Ti=0.5*tu,Td=0.125*tu������ku=(4*d)/(3.141592*a),dΪ�̵������Է�ֵ��
 			aΪ�̵����ͷ����Ի��������һ��г�������
 �������  : pid *pid
             uint16_t tu
             float ku
 �������  : ��
 �� �� ֵ  : void
 
 �޸���ʷ      :
  1.��    ��   : 2014��4��15��
    ��    ��   : Gao.J

    �޸�����   : �����ɺ���

*****************************************************************************/
static void pid_para(pid *pid, unsigned int tu, float k)
{
    float Ti = 0.0;
    float Td = 0.0;
    float ku = 0.0;
    
//    if(fabs(k) > FLT_EPSILON)
//    {
        ku = 4 * 100 / (PI * k);
//    }
    (*pid).kp = 0.6 * ku;

    
    Ti = tu * 0.5;
    
//    if(fabs(Ti) > FLT_EPSILON)
//    {
        (*pid).ki = (*pid).kp / Ti;
//    }
//    else
//    {
//        (*pid).ki = 0;
//    }
    
    Td = tu * 0.125;
    (*pid).kd=(*pid).kp * Td;
}

/*****************************************************************************
 �� �� ��  : pid_output
 ��������  : PID�������,ָ������ͨ��
 �������  : pidtuningpara *tuning
 			 heat_channel chx
 �������  : ��
 �� �� ֵ  : void
 
 �޸���ʷ      :
  1.��    ��   : 2014��5��27��
    ��    ��   : Gao.J

    �޸�����   : �����ɺ���

*****************************************************************************/
void pid_output(pidtuningpara *tuning)
{	
	
 heat_ch1_duty = (uint8_t) tuning->duty;
//	switch(chx)
//	{
//	  case 1 : heat_ch1_duty = (uint8_t) tuning->duty;
//	        break;
//	  case 2 : heat_ch2_duty = (uint8_t) tuning->duty;
//	        break;
//	  case 3 : heat_ch3_duty = (uint8_t) tuning->duty;
//	        break;
//	  case 4 : heat_ch4_duty = (uint8_t) tuning->duty;
//	        break;
//	}

}







/*****************************************************************************
 �� �� ��  : pid_tuning
 ��������  : pid����������,ָ������ͨ��
 �������  : pidtuningpara *tuning
             float curtemp
             heat_channel chx
 �������  : ��
 �� �� ֵ  : TUNING_STAT
 
 �޸���ʷ      :
  1.��    ��   : 2014��5��27��
    ��    ��   : Gao.J

    �޸�����   : �����ɺ���
  2.��    ��   : 2016��6��28��
    ��    ��   : PengChuan

    �޸�����   : �Ż�
  3.��    ��   : 2019��11��11��
    ��    ��   : ryc

    �޸�����   : ��Ϊ����

*****************************************************************************/
TUNING_STAT pid_tuning(pidtuningpara *tuning, float curtemp)
{
	float setpoint = tuning->pid.setpoint;
	
    if ((tuning->status) != END)
    {
        switch(tuning->status)
        {
            case PRE_PROCESS:   //Ԥ����
            {
                if (avoidjitter(tuning) && (setpoint < curtemp))
                {
                    heat_Chx_on();
                }
                
//                tuning->delay(tuning->period);
                if (avoidjitter(tuning) && (setpoint >= curtemp))	//��ֹ�¶ȶ���
                {
                    tuning->status = HIGH1;
                     heat_Chx_off(); 
                }
                break;
            }
            case HIGH1: //��һ������������
            {
                if (avoidjitter(tuning) && (setpoint >= curtemp))
                {
                    heat_Chx_off();
                }
                
//                tuning->delay(tuning->period);
                if (avoidjitter(tuning) && (setpoint < curtemp))	//��ֹ�¶ȶ���
                {
                    tuning->status = LOW1;
                   heat_Chx_on();
                }
                break;
            }
            case LOW1:
            {
                if (avoidjitter(tuning) && (setpoint < curtemp))
                {
                    heat_Chx_on();
                }
                
//                tuning->delay(tuning->period); 
                if (avoidjitter(tuning) && (setpoint >= curtemp))	//��ֹ�¶ȶ���
                {
                    tuning->status = HIGH2;
                    heat_Chx_off();
                }
                break;
            }
            case HIGH2:
            {
                if (avoidjitter(tuning) && (setpoint >= curtemp))
                {
                    heat_Chx_off();
                }
                
//                tuning->delay(tuning->period);
                (tuning->tuning_period)++;  //��¼���ڸ���
                if ((tuning->tuning_max) > curtemp)
                {
                    (tuning->tuning_max) = curtemp; //ȡ�ý׶���ߵ�
                }	
                if (avoidjitter(tuning) && (setpoint < curtemp))    //��ֹ�¶ȶ���
                {
                    tuning->status = LOW2;
                    heat_Chx_on();
                }
                break;
            }
            case LOW2:
            {
                if (avoidjitter(tuning) && (setpoint < curtemp))
                {
                    heat_Chx_on();
                }
                
//                tuning->delay(tuning->period);
                (tuning->tuning_period)++;  //��¼���ڸ���
                if ((tuning->tuning_min) < curtemp)
                {
                    (tuning->tuning_min) = curtemp; //ȡ�ý׶���͵�
                }
                if (avoidjitter(tuning) && (setpoint >= curtemp))   //��ֹ�¶ȶ���
                {//��
                    tuning->tuning_period = (tuning->period)*(tuning->tuning_period);   //������ʱ��x���ڸ���
                    pid_para(&(tuning->pid), tuning->tuning_period, ((tuning->tuning_max)-(tuning->tuning_min))/2);  //��������������������pid����
                    tuning->status = END;
//									printf("�������������PID������Ϊ%f \n %f \n %f \n ",tuning->pid.kp,tuning->pid.ki,tuning->pid.kd);
                }
                break;
            }
            default:
            {
                break;
            }
        }
    }
    else
    {
        tuning->duty = pid_calculate(tuning, curtemp);
        pid_output(tuning);   //����ʱΪһ��pid��������
    }
    return tuning->status;
}

/*****************************************************************************
 �� �� ��  : pid_solo
 ��������  : ����Ҫpid�����������Ŀ��ƹ���,ָ������ͨ��
 �������  : pidtuningpara *tuning
               float curtemp
			    heat_channel chx
 �������  : ��
 �� �� ֵ  : void
 
 �޸���ʷ      :
  1.��    ��   : 2016��8��26��
    ��    ��   : PengChuan

    �޸�����   : �����ɺ���

*****************************************************************************/
void pid_solo(pidtuningpara *tuning, float curtemp)
{
    //�ӽ�Ŀ��ֵ��ʼpid���ƹ���
    tuning->duty  = pid_calculate(tuning, curtemp);
    pid_output(tuning);
}

/*****************************************************************************
 �� �� ��  : avoidjitter
 ��������  : ������������״̬Ǩ�ƵĶ���
 �������  : pidtuningpara *tuning

 �������  : ��
 �� �� ֵ  : bool
 
 �޸���ʷ      :
  1.��    ��   : 2014��6��30��
    ��    ��   : Gao.J

    �޸�����   : �����ɺ���

*****************************************************************************/
bool avoidjitter(pidtuningpara *tuning)
{
	(tuning->jitter)++;
	if ((tuning->jitter) == 3)
	{
		(tuning->jitter) = 0;
		return true;
	}
	return false;
}
