/******************************************************************************
  文 件 名   : tempcontrol.c
  版 本 号   : 初稿
  功能描述   : 温度控制c文件
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


//目标温度
float g_TempOfMach = 40.5;	//5.0c   40.5 反应盘  45.0 清洗水 


float wash_temp0 = 0.0;
float wash_temp1 = 0.0;
float wash_temp2 = 0.0;
float wash_temp3 = 0.0;
float display_temp = 0.0;

uint8_t ht_duty_count = 0 ;
uint16_t samplecount = 0;



/*****************************************************************************
 函 数 名  : init_diff
 功能描述  : 初始化函数
 输入参数  : 无
 输出参数  : 无
 返 回 值  : void
*****************************************************************************/
void init_diff( void )
{
    uint32_t i = 0;
    uint32_t j = 0;
		pid_inittuningstru(&reac_tunning);
		pid_init(&(reac_tunning.pid), g_TempOfMach + tempUpperCtrlPara.TempcCompensate, 1074, 42, 6849);	//温度，P，I，D /*g_TempOfMach+tempUpperCtrlPara.TempcCompensate*/

		return;
}
/*****************************************************************************
 函 数 名  : temp_control
 功能描述  : 温度控制函数
 输入参数  : channelx    x can be :1 2 3
 输出参数  : 无
 返 回 值  : void
*****************************************************************************/
void temp_control(void)
{   
	
//	  float curtemp = 0;
	  char buffer[10];
	  char Buffer[10];
		uint16_t currentAD;
	//读取当前的AD值
	currentAD=Get_Adc_Average(6,8);
	curtemp = wash_sampling_adtotemp(currentAD);//AD值转换为温度
//	printf("当前温度: %f \n",curtemp);
	pid_solo(&reac_tunning,curtemp);
	//发送占空比 ，发送当前温度
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
	//读取当前的AD值
	currentAD=Get_Adc_Average(6,8);//PA3
//	printf("当前测得的AD	值: %d \n",currentAD);
	curtemp = wash_sampling_adtotemp(currentAD);//AD值转换为温度
//	printf("当前温度自整定阶段:  \n");
//	PrintFloat(curtemp);
	
	pid_tuning(&reac_tunning, curtemp);	/*自整定*/

}

//PID自整定
void pidSelfTuning(void)
{
	uint16_t currentAD;
	//读取当前的AD值
	currentAD=Get_Adc_Average(6,8);
	curtemp = wash_sampling_adtotemp(currentAD);//AD值转换为温度
//	printf("当前温度: %f \n",curtemp);
	
	
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
				SwitchHeat=1;//关闭
			}
			if(ht_duty_count > FULLDUTY)          //200ms一个周期
			{
				if(heat_ch1_duty)	
				{
				
					SwitchHeat=0;//打开加热的
				}
				ht_duty_count = 0;
			}	
	}	
}



