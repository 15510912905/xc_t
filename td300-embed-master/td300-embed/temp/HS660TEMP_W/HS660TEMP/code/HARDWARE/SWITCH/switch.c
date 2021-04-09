#include "stm32f10x.h"
#include "sys.h" 
#include "delay.h"
#include "switch.h"
#include "pid.h"
//ADת������  PA11
void switch_Init(void) //IO��ʼ��   BP7
{
 	GPIO_InitTypeDef  GPIO_InitStructure;
	EXTI_InitTypeDef EXTI_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	 //ʹ��PA�˿�ʱ�� 

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;	 //���ȶ˶˿�
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
		GPIO_ResetBits(GPIOB, GPIO_Pin_7);
	
	//�ⲿ�жϽ�������
//	EXTI_ClearITPendingBit(EXTI_Line11);
//	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource11);//ע��õĹ����ⲿ�ж�����
//	EXTI_InitStructure.EXTI_Line = EXTI_Line11;
//	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt; //С���ڵ�Ϊ�� ����ڵ�Ϊ��
//	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;// ;   //�жϷ�ʽΪ�������½���  
//	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
//	EXTI_Init(&EXTI_InitStructure);
}

void heat_Chx_on()
{
	SwitchHeat=0;	//OPEN
	heat_ch1_duty=0;//?
}
/*****************************************************************************
 �� �� ��  : heat_Chx_off
 ��������  : һ·������ֹͣ����
 �������  : heat_channel chx
 �������  : ��
 �� �� ֵ  : void
 
 �޸���ʷ      :
  1.��    ��   : 2014��5��27��
    ��    ��   : Gao.J

    �޸�����   : �����ɺ���

*****************************************************************************/
void heat_Chx_off(void)
{
	  SwitchHeat=1;	//��Ϊ�ر�
//	heat_ch1_duty=0;
}

void heat_delay1s( float seconds )
{
    OSTimeDlyHMSM(0, 0, 1, 000);
}

