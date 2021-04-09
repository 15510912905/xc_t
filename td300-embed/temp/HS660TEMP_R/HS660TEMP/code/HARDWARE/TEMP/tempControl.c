/******************************************************************************
  �� �� ��   : tempcontrol.c
  �� �� ��   : ����
  ��������   : �¶ȿ���c�ļ�
******************************************************************************/
#include "tempcontrol.h"
#include "stm32f10x.h"
#include "pid.h"
#include "TempCollecte.h"
//#include "ADC.h"
//#include "tc_heat.h"
#include "TEMP_Sample.h"
#include "switch.h"
#include "usart.h"
float curtemp=0;


//Ŀ���¶�
float g_TempOfMach = 40.5;	//5.0c   40.5 ��Ӧ��  45.0 ��ϴˮ 


float wash_temp0 = 0.0;
float wash_temp1 = 0.0;
float wash_temp2 = 0.0;
float wash_temp3 = 0.0;
float display_temp = 0.0;

uint8_t ht_duty_count = 0 ;
uint16_t samplecount = 0;



/*****************************************************************************
 �� �� ��  : init_diff
 ��������  : ��ʼ������
 �������  : ��
 �������  : ��
 �� �� ֵ  : void
*****************************************************************************/
void init_diff( void )
{
    uint32_t i = 0;
    uint32_t j = 0;
		pid_inittuningstru(&reac_tunning);
		pid_init(&(reac_tunning.pid), g_TempOfMach + tempUpperCtrlPara.TempcCompensate, 1074, 42, 6849);	//�¶ȣ�P��I��D /*g_TempOfMach+tempUpperCtrlPara.TempcCompensate*/

		return;
}
/*****************************************************************************
 �� �� ��  : temp_control
 ��������  : �¶ȿ��ƺ���
 �������  : channelx    x can be :1 2 3
 �������  : ��
 �� �� ֵ  : void
*****************************************************************************/
void temp_control(void)
{   
	
//	  float curtemp = 0;
	  char buffer[10];
	  char Buffer[10];
		uint16_t currentAD;
	//��ȡ��ǰ��ADֵ
	currentAD=Get_Adc_Average(6,8);
	curtemp = wash_sampling_adtotemp(currentAD);//ADֵת��Ϊ�¶�
//	printf("��ǰ�¶�: %f \n",curtemp);
	pid_solo(&reac_tunning,curtemp);
	//����ռ�ձ� �����͵�ǰ�¶�
}


void temp_controlTurning()
{
	 char buffer[10];
	 char Buffer[10];
		uint16_t currentAD;
//	while (reac_tunning.status!=END)
//	{
	if(reac_tunning.status!=END)
	{
		TIM_Cmd(TIM2,DISABLE);
	}
	else
	{
		TIM_Cmd(TIM2,ENABLE);
	}
	//��ȡ��ǰ��ADֵ
	currentAD=Get_Adc_Average(6,8);//PA3
//	printf("��ǰ��õ�AD	ֵ: %d \n",currentAD);
	curtemp = wash_sampling_adtotemp(currentAD);//ADֵת��Ϊ�¶�
//	printf("��ǰ�¶��������׶�:  \n");
//	PrintFloat(curtemp);
	
	pid_tuning(&reac_tunning, curtemp);	/*������*/

}

//PID������
void pidSelfTuning(void)
{
	uint16_t currentAD;
	//��ȡ��ǰ��ADֵ
	currentAD=Get_Adc_Average(6,8);
	curtemp = wash_sampling_adtotemp(currentAD);//ADֵת��Ϊ�¶�
//	printf("��ǰ�¶�: %f \n",curtemp);
	
	
}


void TIM2_IRQHandler()
{
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)
	{
	 TIM_ClearITPendingBit(TIM2 , TIM_FLAG_Update); 
			ht_duty_count++;      
	    samplecount++;
	    if(ht_duty_count > heat_ch1_duty)  
			{
				//
				SwitchHeat=1;//�ر�
			}
			if(ht_duty_count > FULLDUTY)          //200msһ������
			{
				if(heat_ch1_duty)	
				{
				
					SwitchHeat=0;//�򿪼��ȵ�
				}
				ht_duty_count = 0;
			}	
	}	
}



