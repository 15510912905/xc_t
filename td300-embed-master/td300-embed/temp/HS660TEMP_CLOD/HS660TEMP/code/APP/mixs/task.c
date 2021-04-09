#include "task.h"	 
#include "led.h" 
#include "delay.h"
//#include "key.h"
#include "cmd.h"
//#include "cmd_funs.h"
#include "product.h"
#include "eeprom_config.h"

#include  "act.h"
//#include "act_smp.h"
//#include "ad7685.h"
#include "usart.h"
#include "TempCollecte.h"
#include "TempControl.h"




//任务堆栈
OS_STK  TASK_LED_STK[LED_STK_SIZE];
OS_STK  TASK_START_STK[START_STK_SIZE];
OS_STK  TASK_AD_STK[APP_TASK_CMD_STK_SIZE];
OS_STK  TASK_PARES_CMD_STK[APP_TASK_PARES_CMD_STK_SIZE];

OS_EVENT *CMD_Sem[UnitCount] = { NULL };

//测试用的
char *str;//

//任务1
//控制DS0的亮灭.
void TaskLed(void *pdata)
{		
	LED_Init();
	while (1)
	{
		LED_state = !LED_state;
		OSTimeDlyHMSM(0, 0, 1, 0);
		
//		temp=(curtemp-tempUpperCtrlPara.TempcCompensate)*100;//测试用
//		data[0]=0x03;
//		data[1]=0xfc;
//		data[3]=temp/256;
//		data[2]=temp%256;
//		data[4]=0xfc;
//		data[5]=0x03;		
//	    USART_SendData(USARTTEMP, data[0]);//printf("%02X",0x1A);                   //发送帧头
//	    while (USART_GetFlagStatus(USARTTEMP, USART_FLAG_TXE) == RESET){};
//	    USART_SendData(USARTTEMP, data[1]);//printf("%02X",0x1A);                   //发送帧头
//	    while (USART_GetFlagStatus(USARTTEMP, USART_FLAG_TXE) == RESET){};	
//	    USART_SendData(USARTTEMP, data[2]);//printf("%02X",0x1A);                   //发送帧头
//	    while (USART_GetFlagStatus(USARTTEMP, USART_FLAG_TXE) == RESET){};	
//	    USART_SendData(USARTTEMP, data[3]);//printf("%02X",0x1A);                   //发送帧头
//	    while (USART_GetFlagStatus(USARTTEMP, USART_FLAG_TXE) == RESET){};	
//	    USART_SendData(USARTTEMP, data[4]);//printf("%02X",0x1A);                   //发送帧头
//	    while (USART_GetFlagStatus(USARTTEMP, USART_FLAG_TXE) == RESET){};	
//	    USART_SendData(USARTTEMP, data[5]);//printf("%02X",0x1A);                   //发送帧头
//	    while (USART_GetFlagStatus(USARTTEMP, USART_FLAG_TXE) == RESET){};				
//		printf("%d \n",temp)	;;            
	} 
}


void TaskUART4(void *pdata)
{
	//    LED_Init();
	//TIM_Cmd(TIM3, ENABLE);
	while (1)
	{
		//			 LED1 = !LED1;
		//        LED_state = !LED_state;
//		parse_cmd();
		OSTimeDlyHMSM(0, 0, 1, 000);       
	}
}



//命令处理任务
//void TaskCmdMix(void  *parg)
//{
//	INT8U err = 0;

//	while (1)
//	{
//		OSSemPend(CMD_Sem[Unit_Mix], 0, &err);
////		DoCmd(Unit_Mix);
//	}
//}

//命令处理任务
//void TaskCmdSmp(void  *parg)
//{
//	INT8U err = 0;

//	while (1)
//	{
//		OSSemPend(CMD_Sem[Unit_Smp], 0, &err);
//		DoCmd(Unit_Smp);
//	}
//}

////读AD的任务
void read_AD(void * parg)
{
	u16 temp;
	while (1)
	{
		temp_controlTurning();
		temp = curtemp * 100;
		USART_SendData(USARTTEMP, 0x00);
		while (USART_GetFlagStatus(USARTTEMP, USART_FLAG_TXE) == RESET){};
		USART_SendData(USARTTEMP, 0x00);
		while (USART_GetFlagStatus(USARTTEMP, USART_FLAG_TXE) == RESET){};
		USART_SendData(USARTTEMP, temp/256);
		while (USART_GetFlagStatus(USARTTEMP, USART_FLAG_TXE) == RESET){};
		USART_SendData(USARTTEMP, temp%256);
		while (USART_GetFlagStatus(USARTTEMP, USART_FLAG_TXE) == RESET){};
		OSTimeDlyHMSM(0, 0, 1, 000);
	}
}

void responseCmdFromUpper()
{
	OSAndroid1ResponSem = OSSemCreate(0);
	while(1)
	{
	 parse_cmd_USART3();
	 OSTimeDlyHMSM(0, 0, 1, 000);
	}	
}
//void _cmdTaskInit()
//{
//	int i = 0;
//	for (; i < UnitCount; i++)
//	{
//		CMD_Sem[i] = OSSemCreate(0);

//		if (NULL == CMD_Sem[i])
//		{
//			return;
//		}
//	}
//	//			VRT_Sem1 = OSSemCreate(0);
//	//			VRT_Sem2 = OSSemCreate(0);
//	//			VRT_Sem3 = OSSemCreate(0);

////	OSTaskCreate(TaskCmdMix, (void *)0, (OS_STK *)&App_TaskCmdStk_Mix[APP_TASK_CMD_STK_SIZE - 1], APP_TASK_Mix_PRIO);
////	OSTaskCreate(TaskCmdSmp, (void *)0, (OS_STK *)&App_TaskCmdStk_Smp[APP_TASK_CMD_STK_SIZE - 1], APP_TASK_Smp_PRIO);
//}


//开始任务
void TaskStart(void * pdata)
{
#if OS_CRITICAL_METHOD == 3          /* Allocate storage for CPU status register */
	OS_CPU_SR  cpu_sr;
#endif
	pdata = pdata;
	OS_ENTER_CRITICAL();
//	_cmdTaskInit();
	OSTaskCreate(TaskLed, (void *)0, (OS_STK *)&TASK_LED_STK[LED_STK_SIZE - 1], LED_TASK_Prio);
	OSTaskCreate(read_AD, (void *)0, (OS_STK *)&TASK_AD_STK[APP_TASK_CMD_STK_SIZE - 1], APP_AD_PRIO);
	OSTaskCreate(responseCmdFromUpper, (void *)0, (OS_STK *)&TASK_PARES_CMD_STK[APP_TASK_PARES_CMD_STK_SIZE - 1], APP_PRASE_CMD_PRIO);
	OSTaskSuspend(START_TASK_Prio);	//suspend but not delete
	OS_EXIT_CRITICAL();
}
