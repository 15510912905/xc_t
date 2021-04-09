/********************************************************
   版权所有 (C), 2001-2100, 四川新健康成生物股份有限公司
  -------------------------------------------------------

				嵌入式开发平台——BSP层 UART封装

  -------------------------------------------------------
   文 件 名   : BSP_Uart.c
   版 本 号   : V1.0.0.0
   作    者   : ryc
   生成日期   : 2019年11月14日
   功能描述   : 定义一种基于STM32的板级支持包。
				该层依赖于硬件电路。
   依赖于     : STM32F10x_StdPeriph_Lib_V3.5.0
 ********************************************************/
 
#include "BSP_Uart.h"

uint16_t USART1_RX_STA;
uint16_t USART2_RX_STA;
uint16_t USART3_RX_STA;
uint16_t USART4_RX_STA;

u8 USART1_RX_BUF[100];
u8 USART2_RX_BUF[100];
u8 USART3_RX_BUF[100];
u8 USART4_RX_BUF[100];



/* 初始化
  ----------------------------------------------------------------------*/

/* 初始化UART及接收中断
  -----------------------------------------
  入口：串口编号、波特率，TX、RX端口
  返回值：成功true，失败false
*/
bool BSP_Init_UsartInterrupt(BSP_USART_ENUM usart_number, uint32_t baud_rate, \
							 BSP_PORT_ENUM tx_port_number, BSP_PIN_ENUM tx_pin_number, \
							 BSP_PORT_ENUM rx_port_number, BSP_PIN_ENUM rx_pin_number)
{
	GPIO_InitTypeDef    GPIO_InitStructure;  
	USART_InitTypeDef   USART_InitStructure; 
	NVIC_InitTypeDef    NVIC_InitStructure;
	
	uint8_t irq;

	/*初始化串口时钟*/
	switch(usart_number)
	{
		case _USART1_:
			RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
			break;
			
		case _USART2_:
			RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
			break;
			
		case _USART3_:
			RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
			break;
			
		case _UART4_:
			RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4, ENABLE);
			break;
			
		default:
			return false;
	}
	/*引脚时钟初始化*/
	RCC_APB2PeriphClockCmd((RCC_APB2Periph_GPIOA << tx_port_number) | (RCC_APB2Periph_GPIOA << rx_port_number) | RCC_APB2Periph_AFIO, ENABLE);
	
	/*使能串口重映射*/
	switch(usart_number)
	{
		case _USART1_:
//			GPIO_PinRemapConfig(GPIO_Remap_USART1, ENABLE);
			break;
			
		case _USART2_:
//			GPIO_PinRemapConfig(GPIO_Remap_USART2, ENABLE);
			break;
			
		case _USART3_:
//			GPIO_PinRemapConfig(GPIO_FullRemap_USART3, ENABLE);
			break;
			
		case _UART4_:
			break;
			
		default:
			return false;
	}
		
	/*管脚配置*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 << tx_pin_number;			
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;			
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;   
	GPIO_Init(PORT(tx_port_number), &GPIO_InitStructure);			/* TXIO */
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 << rx_pin_number;			 	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(PORT(rx_port_number), &GPIO_InitStructure); 			/* RXIO */
	
	/*串口工作模式配置*/
	USART_InitStructure.USART_BaudRate = baud_rate;						        	    /*设置波特率*/	
	USART_InitStructure.USART_WordLength = USART_WordLength_8b; 		                /*8位数据位*/		 	
	USART_InitStructure.USART_StopBits = USART_StopBits_1;					            /*1位停止位*/	
	USART_InitStructure.USART_Parity = USART_Parity_No; 						        /*无校验*/
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;     /*硬件流控 None*/
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;        				/*设置为收发模式*/
	
	switch(usart_number)
	{
		case _USART1_:
			USART_Init(USART1, &USART_InitStructure);			                   		/*初始化UART*/
			USART_Cmd(USART1, ENABLE);	                                           		/*开启串口*/
			irq = USART1_IRQn;
			break;
			
		case _USART2_:
			USART_Init(USART2, &USART_InitStructure);			                   		/*初始化UART*/
			USART_Cmd(USART2, ENABLE);	                                           		/*开启串口*/
			irq = USART2_IRQn;
			break;
			
		case _USART3_:
			USART_Init(USART3, &USART_InitStructure);			                   		/*初始化UART*/
			USART_Cmd(USART3, ENABLE);	                                           		/*开启串口*/
			irq = USART3_IRQn;
			break;
			
		case _UART4_:
			USART_Init(UART4, &USART_InitStructure);			                   		/*初始化UART*/
			USART_Cmd(UART4, ENABLE);	                                           		/*开启串口*/
			irq = UART4_IRQn;
			break;
			
		default:
			return false;
	}                             

	/*中断设置*/
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);		
	NVIC_InitStructure.NVIC_IRQChannel = irq;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;       					/*低优先级别的中断*/
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;			    					/*响应中断等级为0*/
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);  
	
	switch(usart_number)
	{
		case _USART1_:
			USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);	                 					/*Enable UART IRQ*/
			break;
			
		case _USART2_:
			USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);	                 					/*Enable UART IRQ*/
			break;
			
		case _USART3_:
			USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);	                 					/*Enable UART IRQ*/
			break;
			
		case _UART4_:
			USART_ITConfig(UART4, USART_IT_RXNE, ENABLE);	                 					/*Enable UART IRQ*/
			break;
			
		default:
			return false;
	}
	
	return true;
}

/* 操作
  ----------------------------------------------------------------------*/

/* 串口发送一个字节
  -----------------------------
  入口：串口编号，发送内容
  返回值：成功true，失败false
*/
bool BSP_UsartSendByte(BSP_USART_ENUM usart_number, uint8_t data)
{
	
	switch(usart_number)
	{
		case _USART1_: 
			USART_SendData(USART1, data); 			/*发送数据*/
			while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
			break;
		case _USART2_:
			USART_SendData(USART2, data); 			/*发送数据*/
			while (USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);
			break;
		case _USART3_:
			USART_SendData(USART3, data); 			/*发送数据*/
			while (USART_GetFlagStatus(USART3, USART_FLAG_TXE) == RESET);
			break;
		case _UART4_:
			USART_SendData(UART4, data); 			/*发送数据*/
			while (USART_GetFlagStatus(UART4, USART_FLAG_TXE) == RESET);
			break;
		default: 
			return false;
	}
	
	return true;
}

/* 串口接收一个字节
-----------------------------ok
入口：串口编号
返回值：串口接收到的数据
*/
uint8_t BSP_UsartGetByte(BSP_USART_ENUM usart_number)
{
	switch(usart_number)
	{
		case _USART1_:
			return USART_ReceiveData(USART1);
		case _USART2_:
			return USART_ReceiveData(USART2);
		case _USART3_:
			return USART_ReceiveData(USART3);
		case _UART4_:
			return USART_ReceiveData(UART4);
		default:
			return 0;
	}
}

/* 中断
  ----------------------------------------------------------------------*/
 
/* 串口1中断
  -------------------------------
*/
void USART1_IRQHandler(void)
{
	
   if(USART_GetITStatus(USART1, USART_IT_RXNE)!=RESET)
   {
		BSP_Usart1_IRQHandler();
		USART_ClearITPendingBit(USART1 , USART_IT_RXNE); 
   }
}

/* 串口2中断
  -------------------------------
*/
void USART2_IRQHandler(void)
{
   if(USART_GetITStatus(USART2,USART_IT_RXNE)!=RESET)
   {	
		BSP_Usart2_IRQHandler();
   }
   USART_ClearITPendingBit(USART2 , USART_IT_RXNE);
}

/* 串口3中断
  -------------------------------
*/
void USART3_IRQHandler(void)
{
	uint8_t Res;
   if(USART_GetITStatus(USART3,USART_IT_RXNE)!=RESET)
   {	   
		Res =USART_ReceiveData(USART3);
		if((USART3_RX_STA&0x8000)==0)//½ÓÊÕÎ´Íê³É
			{
			if(USART3_RX_STA&0x4000)//½ÓÊÕµ½ÁË0x0d
				{
				if(Res!=0x0a)USART3_RX_STA=0;//½ÓÊÕ´íÎó,ÖØÐÂ¿ªÊ¼
				else USART3_RX_STA|=0x8000;	//½ÓÊÕÍê³ÉÁË 
				}
			else //»¹Ã»ÊÕµ½0X0D
				{	
				if(Res==0x0d)USART3_RX_STA|=0x4000;
				else
					{
					USART3_RX_BUF[USART3_RX_STA&0X3FFF]=Res ;
					USART3_RX_STA++;
					if(USART3_RX_STA>(100))USART3_RX_STA=0;//½ÓÊÕÊý¾Ý´íÎó,ÖØÐÂ¿ªÊ¼½ÓÊÕ	  
					}		 
				}
			} 		
   USART_ClearITPendingBit(USART3 , USART_IT_RXNE); 
   }
}

/* 串口4中断
  -------------------------------
*/
void UART4_IRQHandler(void)
{
   if(USART_GetITStatus(UART4,USART_IT_RXNE)!=RESET)
   {	   
		USART_ClearITPendingBit(UART4 , USART_IT_RXNE); 
//		BSP_Uart4_IRQHandler();		
   }
}
