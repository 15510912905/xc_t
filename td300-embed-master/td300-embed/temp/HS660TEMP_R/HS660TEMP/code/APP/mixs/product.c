#include "product.h"
#include "includes.h"
#include "stm32f10x_tim.h"
//#include "ad7685.h"
#include "tempcontrol.h"
#include "switch.h"
#include "TempCollecte.h"




void _periphClock_init()
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB , ENABLE);
//	RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1 , ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2 , ENABLE);

}

void _NVIC_init()
{
	NVIC_InitTypeDef  		NVIC_InitStructure;

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	/*设置NVIC中断分组2:2位抢占优先级，2位响应优先级	   */

	//定时器和光耦中断同等优先级，避免中断嵌套

	//usart3的接收中断
	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);


	//usart2的接收中断
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);



}

extern OS_EVENT *g_CanMutex;//保护can发送


//定时器
void _timer_init()
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;

	TIM_DeInit(TIM2);

	/* TIM3 Configuration 36MHz/36000=1ms  */
	TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
	TIM_TimeBaseStructure.TIM_Period = 1000;//时钟周期
	TIM_TimeBaseStructure.TIM_Prescaler = 9 - 1u;//预分配数   72M/9=8M
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
//	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
//	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);
//	TIM_TimeBaseInit(TIM5, &TIM_TimeBaseStructure);

	TIM_ARRPreloadConfig(TIM2, DISABLE);//不用预装载，直接修改到影子寄存器
	/* TIM3 TRGO selection */
	TIM_SelectOutputTrigger(TIM2, TIM_TRGOSource_Update);

	TIM_ClearFlag(TIM2, TIM_FLAG_Update);//清除定时器的溢出标志位

	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);//使能定时器溢出中断
	TIM_Cmd(TIM2,ENABLE);
}

//usart2 温度控制串口
void _usart2_init()
{
	USART_InitTypeDef USART_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	/*设置NVIC中断分组2:2位抢占优先级，2位响应优先级	   */

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);


	/* Configure USART Tx and Rx as alternate function push-pull */
	//PA2
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	//PA3
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//GPIO_Mode_IN_FLOATING;//GPIO_Mode_IPU;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	/* USARTx configuration ----------------------------------------------------*/
	/* USARTx configured as follow:
	- BaudRate = 57600 baud
	- Word Length = 8 Bits
	- One Stop Bit
	- No parity
	- Hardware flow control disabled (RTS and CTS signals)
	- Receive and transmit enabled
	*/
	USART_InitStructure.USART_BaudRate = 115200;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl =
		USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART2, &USART_InitStructure);
	/* Enable USART */
	USART_Cmd(USART2, ENABLE);
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
}



void AD_EXITinit(){
	EXTI_InitTypeDef EXTI_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	//AD外部启动中断为PA6
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

//	EXTI_ClearITPendingBit(EXTI_Line11);
//	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource11);//注射泵的光耦外部中断配置
//	EXTI_InitStructure.EXTI_Line = EXTI_Line11;
//	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt; //小泵遮挡为高 大泵遮挡为低
//	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;// ;   //中断方式为上升与下降沿  
//	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
//	EXTI_Init(&EXTI_InitStructure);
}


void ProductInit()
{
	_periphClock_init();
	_NVIC_init();
	_timer_init();
	_usart2_init();//条码扫描串口
	switch_Init();
	Adc_Init();
	TempComposeReadAndUse();//读补偿参数
	init_diff();
//	
}


