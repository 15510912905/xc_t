#include "stm32f10x.h"
#include "led.h"

//初始化PA8和PD2为输出口.并使能这两个口的时钟		    
//状态指示LED IO初始化
void LED_Init(void)
{
    GPIO_InitTypeDef  GPIO_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	 //使能PC端口时钟 PA端口
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;	 //LED->pc13->pc1 端口配置
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
	
    GPIO_ResetBits(GPIOB, GPIO_Pin_5);
}