#include "stm32f10x.h"
#include "key.h"
#include "sys.h" 
#include "delay.h"
 
void KEY_Init(void) //IO初始化
{
 	GPIO_InitTypeDef GPIO_InitStructure;
	//init GPIOD.15,GPIOD.14,GPIOD.13,GPIOD.12  上拉输入
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD,ENABLE);
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_15|GPIO_Pin_14|GPIO_Pin_13|GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;       //GPIO_Mode_IPD->下拉输入

 	GPIO_Init(GPIOD, &GPIO_InitStructure);

}
u8 KEY_Scan(void)
{	 
	static u8 key_up=1;//按键按松开标志	
 
// 	GPIO_PinRemapConfig(GPIO_Remap_SWJ_Disable, ENABLE);
	if(key_up&&(KEY1==0||KEY2==0||KEY3==0||KEY4==0))
	{
		delay_ms(10);//去抖动 
		key_up=0;
		if(KEY1==0)
		{
			return 1;
		}
		else if(KEY2==0)
		{
			return 2;
		}
		else if(KEY3==0)
		{
			return 3;
		}
		else if(KEY4==0)
		{
			return 4;
		}
	}else if(KEY1==1&&KEY2==1&&KEY3==1&&KEY4==1)key_up=1; 	    

	return 0;// 无按键按下
}
