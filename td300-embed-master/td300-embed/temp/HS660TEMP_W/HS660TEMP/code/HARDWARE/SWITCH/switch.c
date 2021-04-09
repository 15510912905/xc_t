#include "stm32f10x.h"
#include "sys.h" 
#include "delay.h"
#include "switch.h"
#include "pid.h"
//AD转换开关  PA11
void switch_Init(void) //IO初始化   BP7
{
 	GPIO_InitTypeDef  GPIO_InitStructure;
	EXTI_InitTypeDef EXTI_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	 //使能PA端口时钟 

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;	 //加热端端口
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
		GPIO_ResetBits(GPIOB, GPIO_Pin_7);
	
	//外部中断接收数据
//	EXTI_ClearITPendingBit(EXTI_Line11);
//	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource11);//注射泵的光耦外部中断配置
//	EXTI_InitStructure.EXTI_Line = EXTI_Line11;
//	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt; //小泵遮挡为高 大泵遮挡为低
//	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;// ;   //中断方式为上升与下降沿  
//	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
//	EXTI_Init(&EXTI_InitStructure);
}

void heat_Chx_on()
{
	SwitchHeat=0;	//OPEN
	heat_ch1_duty=0;//?
}
/*****************************************************************************
 函 数 名  : heat_Chx_off
 功能描述  : 一路加热器停止工作
 输入参数  : heat_channel chx
 输出参数  : 无
 返 回 值  : void
 
 修改历史      :
  1.日    期   : 2014年5月27日
    作    者   : Gao.J

    修改内容   : 新生成函数

*****************************************************************************/
void heat_Chx_off(void)
{
	  SwitchHeat=1;	//高为关闭
//	heat_ch1_duty=0;
}

void heat_delay1s( float seconds )
{
    OSTimeDlyHMSM(0, 0, 1, 000);
}

