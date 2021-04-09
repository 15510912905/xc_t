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

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	/*����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�	   */

	//��ʱ���͹����ж�ͬ�����ȼ��������ж�Ƕ��

	//usart3�Ľ����ж�
	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);


	//usart2�Ľ����ж�
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

extern OS_EVENT *g_CanMutex;//����can����


//��ʱ��
void _timer_init()
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;

	TIM_DeInit(TIM2);

	/* TIM3 Configuration 36MHz/36000=1ms  */
	TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
	TIM_TimeBaseStructure.TIM_Period = 1000;//ʱ������
	TIM_TimeBaseStructure.TIM_Prescaler = 9 - 1u;//Ԥ������   72M/9=8M
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
//	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
//	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);
//	TIM_TimeBaseInit(TIM5, &TIM_TimeBaseStructure);

	TIM_ARRPreloadConfig(TIM2, DISABLE);//����Ԥװ�أ�ֱ���޸ĵ�Ӱ�ӼĴ���
	/* TIM3 TRGO selection */
	TIM_SelectOutputTrigger(TIM2, TIM_TRGOSource_Update);

	TIM_ClearFlag(TIM2, TIM_FLAG_Update);//�����ʱ���������־λ

	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);//ʹ�ܶ�ʱ������ж�
	TIM_Cmd(TIM2,ENABLE);
}

//usart2 �¶ȿ��ƴ���
void _usart2_init()
{
	USART_InitTypeDef USART_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	/*����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�	   */

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
	//AD�ⲿ�����ж�ΪPA6
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

//	EXTI_ClearITPendingBit(EXTI_Line11);
//	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource11);//ע��õĹ����ⲿ�ж�����
//	EXTI_InitStructure.EXTI_Line = EXTI_Line11;
//	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt; //С���ڵ�Ϊ�� ����ڵ�Ϊ��
//	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;// ;   //�жϷ�ʽΪ�������½���  
//	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
//	EXTI_Init(&EXTI_InitStructure);
}


void ProductInit()
{
	_periphClock_init();
	_NVIC_init();
	_timer_init();
	_usart2_init();//����ɨ�贮��
	switch_Init();
	Adc_Init();
	TempComposeReadAndUse();//����������
	init_diff();
//	
}


