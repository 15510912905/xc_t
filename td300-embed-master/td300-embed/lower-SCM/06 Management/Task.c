/********************************************************
   版权所有 (C), 2001-2100, 四川新健康成生物股份有限公司
  -------------------------------------------------------

				嵌入式开发平台——任务定义

  -------------------------------------------------------
   文 件 名   : Tesk.c
   版 本 号   : V1.0.0.0
   作    者   : ryc
   生成日期   : 2019年08月01日
   功能描述   : 定义并行任务。
   依赖于     : uC/OS-II V2.92.07
   注         ：该文件不依赖于硬件电路。 
 ********************************************************/
 
#include "Task.h"

/*----------------------------------------------------定义任务堆栈*/
OS_STK g_TaskStart_Stk[TASK_START_STK_SIZE];							/*初始任务 堆栈*/

static OS_STK g_TaskCmdTransceiver_Stk[TASK_CMD_TRANSCEIVER_STK_SIZE];	/*指令收发器任务 堆栈*/
static OS_STK g_TaskCmdScheduler1_Stk[TASK_CMD_SCHEDULER1_STK_SIZE];	/*指令调度器1任务 堆栈*/
static OS_STK g_TaskCmdScheduler2_Stk[TASK_CMD_SCHEDULER2_STK_SIZE];	/*指令调度器2任务 堆栈*/

static OS_STK g_TaskLed_Stk[TASK_LED_STK_SIZE];							/*系统指示灯任务 堆栈*/
static OS_STK g_TaskResident1_Stk[TASK_RESIDENT_1_STK_SIZE];			/*私有常驻任务1 堆栈*/
static OS_STK g_TaskResident2_Stk[TASK_RESIDENT_2_STK_SIZE];			/*私有常驻任务2 堆栈*/

/*----------------------------------------------------定义全局变量*/

static DRV_CAN_TYPE* _gp_can = NULL;									/*指向can收发器*/

/*----------------------------------------------------定义任务*/

/* 初始任务
  -----------------------------------ok
*/
void TaskStart(void* p_arg)
{
#if OS_CRITICAL_METHOD == 3			/* Allocate storage for CPU status register */
    OS_CPU_SR  cpu_sr;
#endif
    OS_ENTER_CRITICAL();

	/*创建协议层任务*/
    OSTaskCreate(TaskCmdTransceiver, NULL, \
				(OS_STK*)&g_TaskCmdTransceiver_Stk[TASK_CMD_TRANSCEIVER_STK_SIZE - 1], \
				TASK_CMD_TRANSCEIVER_PRIO);	/*指令收发器*/
				
	OSTaskCreate(TaskCmdScheduler1,  NULL, \
				(OS_STK*)&g_TaskCmdScheduler1_Stk[TASK_CMD_SCHEDULER1_STK_SIZE - 1], \
				TASK_CMD_SCHEDULER1_PRIO);	/*指令调度器1*/
				
	OSTaskCreate(TaskCmdScheduler2,  NULL, \
				(OS_STK*)&g_TaskCmdScheduler2_Stk[TASK_CMD_SCHEDULER2_STK_SIZE - 1], \
				TASK_CMD_SCHEDULER2_PRIO);	/*指令调度器2*/

	/*创建功能层任务*/
    OSTaskCreate(TaskLed,            NULL, \
				(OS_STK*)&g_TaskLed_Stk[TASK_LED_STK_SIZE - 1], \
				TASK_LED_PRIO);				/*系统指示灯*/
    OSTaskCreate(TaskResident1,      NULL, \
				(OS_STK*)&g_TaskResident1_Stk[TASK_RESIDENT_1_STK_SIZE - 1], \
				TASK_RESIDENT_1_PRIO);		/*私有常驻任务1*/
    OSTaskCreate(TaskResident2,      NULL, \
				(OS_STK*)&g_TaskResident2_Stk[TASK_RESIDENT_2_STK_SIZE - 1], \
				TASK_RESIDENT_2_PRIO);		/*私有常驻任务2*/
	
    OSTaskSuspend(TASK_START_PRIO);	/* suspend but not delete */
    OS_EXIT_CRITICAL();
}

/* 指令收发器任务
  -----------------------------------ok
*/
static void TaskCmdTransceiver(void* p_arg)
{	
	_gp_can = Drv_CANProtocol_Init(_CAN1_, CONFIG_CAN_BAUD_RATE, \
									CONFIG_CAN_ID_1, CONFIG_CAN_ID_2, 0, 0, \
									CONFIG_CAN_TX_PORT, CONFIG_CAN_TX_PIN, \
									CONFIG_CAN_RX_PORT, CONFIG_CAN_RX_PIN);	/*初始化CAN收发器*/
	
	if(_gp_can == NULL)	/*CAN通信模块初始化失败*/
	{
		while(1)
		{
			OSTimeDlyHMSM(0, 0, 0, 500);
		}
	}
	else	/*初始化成功，处理can发送任务*/
	{
		while(1)
		{			
			//OSSemPend(_gp_can->tx_sem, 1, &err);	/*等待有数据包待发送的信号量*/	改为查询方式不使用信号量
			OSTimeDlyHMSM(0, 0, 0, 1);
			Drv_CANProtocol_RX_TimeOutCheck(0, &(_gp_can->rx_buffer[0][0]));
			Drv_CANProtocol_RX_TimeOutCheck(1, &(_gp_can->rx_buffer[1][0]));
			Drv_CANProtocol_PutFrame();				/*查找并发送缓冲区数据*/
		}
	}
}

/* 指令调度器1任务
  -----------------------------------
*/
static void TaskCmdScheduler1(void* p_arg)
{
	uint8_t err = OS_ERR_NONE;
	DRV_CAN_RX_BUFFER_TYPE* p_msg = NULL;
	
	CMD_REGEDIT_TYPE* p_cmd_regedit = Cmd_Regedit_Init(CMD_REGEDIT_1, CONFIG_CAN_ID_1);	/*初始化指令注册表1*/
	uint8_t count = 0;	/*用于记录指令条数*/
	uint8_t cmd_h = 0, cmd_l = 0;	/*用于记录命令字*/
	uint8_t len_h = 0, len_l = 0;	/*用于记录数据长度*/
	uint16_t i = 0;
	
	if(p_cmd_regedit == (CMD_REGEDIT_TYPE*)NULL)	/*指令注册表初始化失败*/
	{
		while(1)
		{
			OSTimeDlyHMSM(0, 0, 0, 500);
		}
	}
	else						/*指令注册表初始化成功*/
	{	
		while(1)
		{
			if(_gp_can != NULL)	/*CAN收发器初始化成功*/
			{
				/*等待*/
				OSSemPend(_gp_can->rx_sem[0], 0, &err);	/*等待接收到完整数据包的信号量*/
				
				/*接收*/
				p_msg = Drv_CANProtocol_GetPacket(0);	/*获取数据包的地址*/
				
				if(p_msg != NULL)	/*获取到数据包*/
				{
					/*发送ACK*/
//					Drv_CANProtocol_SendAckToBuffer(p_msg->can_id, \
//													p_msg->buffer[0][0], \
//													((uint16_t)p_msg->buffer[0][2] << 8) | p_msg->buffer[0][1], \
//													((uint32_t)p_msg->buffer[0][12] << 24) | ((uint32_t)p_msg->buffer[0][11] << 16) | ((uint32_t)p_msg->buffer[0][10] << 8) | (uint32_t)p_msg->buffer[0][9]);
					/*处理组合指令*/
					if(((uint8_t *)(p_msg->buffer))[1] == 0x0c)	/*判断是组合指令*/
					{
						/*获取组合指令条数*/
						count = ((uint8_t *)(p_msg->buffer))[15];
						/*数据前移，冲掉指令条数*/
						for(i = 15; i < (CAN_RX_CMD_MAX_SIZE * 8 - 2); i++)	
						{
							((uint8_t *)(p_msg->buffer))[i] = ((uint8_t *)(p_msg->buffer))[i + 2];
						}
						while(count > 0)
						{
							/*获取新的数据长度和命令字*/
							len_l = ((uint8_t *)(p_msg->buffer))[15];
							len_h = ((uint8_t *)(p_msg->buffer))[16];
							cmd_l = ((uint8_t *)(p_msg->buffer))[19];
							cmd_h = ((uint8_t *)(p_msg->buffer))[20];
							/*新数据长度和命令字组装*/
							((uint8_t *)(p_msg->buffer))[1] = cmd_l;
							((uint8_t *)(p_msg->buffer))[2] = cmd_h;
							((uint8_t *)(p_msg->buffer))[7] = len_l;
							((uint8_t *)(p_msg->buffer))[8] = len_h;
							/*数据整体前移，冲掉命令字和数据长度*/
							for(i = 13; i < (CAN_RX_CMD_MAX_SIZE * 8 - 6); i++)	
							{
								((uint8_t *)(p_msg->buffer))[i] = ((uint8_t *)(p_msg->buffer))[i + 6];
							}
							p_msg->size = ((len_l + len_h * 256) - 2) / 7 + ((((len_l + len_h * 256) - 2) % 7) == 0 ? 0 : 1) + 2;	/*通过数据长度反算帧数*/
							/*调度*/
							if(false == Prt_CmdScheduler(p_msg, p_cmd_regedit))
							{
								Drv_CANProtocol_SendErrorToBuffer(p_msg->can_id, p_msg->buffer[0][0], 
																((uint16_t)(p_msg->buffer[0][2]) << 8 | (uint16_t)(p_msg->buffer[0][1])), 
																((uint32_t)(p_msg->buffer[0][12]) << 24 | (uint32_t)(p_msg->buffer[0][11]) << 16 |(uint32_t)(p_msg->buffer[0][10]) << 8 |(uint32_t)(p_msg->buffer[0][9])), 
																0xFF, 0xFF, 0x06);		/*报错，不支持该指令*/
							}
							/*数据整体前移，冲掉数据域*/
							for(i = 15; i < (CAN_RX_CMD_MAX_SIZE * 8 - (len_l + len_h * 256 - 2)); i++)
							{
								((uint8_t *)(p_msg->buffer))[i] = ((uint8_t *)(p_msg->buffer))[i + (len_l + len_h * 256 - 2)];
							}
							count--;
						}
					}
					else
					{
						/*调度*/
						if(false == Prt_CmdScheduler(p_msg, p_cmd_regedit))
						{
							Drv_CANProtocol_SendErrorToBuffer(p_msg->can_id, p_msg->buffer[0][0], 
																((uint16_t)(p_msg->buffer[0][2]) << 8 | (uint16_t)(p_msg->buffer[0][1])), 
																((uint32_t)(p_msg->buffer[0][12]) << 24 | (uint32_t)(p_msg->buffer[0][11]) << 16 |(uint32_t)(p_msg->buffer[0][10]) << 8 |(uint32_t)(p_msg->buffer[0][9])), 
																0xFF, 0xFF, 0x06);		/*报错，不支持该指令*/
						}
					}
					
					/*释放*/
					Drv_CANProtocol_RXBufferFree(0, p_msg);		/*释放该接收缓冲区*/
					p_msg = NULL;
				}
			}
		}
	}
}

/* 指令调度器2任务
  -----------------------------------
*/
static void TaskCmdScheduler2(void* p_arg)
{
	uint8_t err = OS_ERR_NONE;
	DRV_CAN_RX_BUFFER_TYPE* p_msg = NULL;
	
	CMD_REGEDIT_TYPE* p_cmd_regedit = Cmd_Regedit_Init(CMD_REGEDIT_2, CONFIG_CAN_ID_2);	/*初始化指令注册表2*/
	uint8_t count = 0;	/*用于记录指令条数*/
	uint8_t cmd_h = 0, cmd_l = 0;	/*用于记录命令字*/
	uint8_t len_h = 0, len_l = 0;	/*用于记录数据长度*/
	uint16_t i = 0;
	
	if(p_cmd_regedit == (CMD_REGEDIT_TYPE*)NULL)	/*指令注册表初始化失败*/
	{
		while(1)
		{
			OSTimeDlyHMSM(0, 0, 0, 500);
		}
	}
	else						/*指令注册表初始化成功*/
	{	
		while(1)
		{
			if(_gp_can != NULL)	/*CAN收发器初始化成功*/
			{
				/*等待*/
				OSSemPend(_gp_can->rx_sem[1], 0, &err);	/*等待接收到完整数据包的信号量*/
				
				/*接收*/
				p_msg = Drv_CANProtocol_GetPacket(1);	/*获取数据包的地址*/
				
				if(p_msg != NULL)	/*获取到数据包*/
				{
					/*发送ACK*/
//					Drv_CANProtocol_SendAckToBuffer(p_msg->can_id, \
//													p_msg->buffer[0][0], \
//													((uint16_t)p_msg->buffer[0][2] << 8) | p_msg->buffer[0][1], \
//													((uint32_t)p_msg->buffer[0][12] << 24) | ((uint32_t)p_msg->buffer[0][11] << 16) | ((uint32_t)p_msg->buffer[0][10] << 8) | (uint32_t)p_msg->buffer[0][9]);
					/*处理组合指令*/
					if(((uint8_t *)(p_msg->buffer))[1] == 0x0c)	/*判断是组合指令*/
					{
						/*获取组合指令条数*/
						count = ((uint8_t *)(p_msg->buffer))[15];
						/*数据前移，冲掉指令条数*/
						for(i = 15; i < (CAN_RX_CMD_MAX_SIZE * 8 - 1); i++)	
						{
							((uint8_t *)(p_msg->buffer))[i] = ((uint8_t *)(p_msg->buffer))[i + 1];
						}
						while(count > 0)
						{
							/*获取新的数据长度和命令字*/
							len_l = ((uint8_t *)(p_msg->buffer))[15];
							len_h = ((uint8_t *)(p_msg->buffer))[16];
							cmd_l = ((uint8_t *)(p_msg->buffer))[17];
							cmd_h = ((uint8_t *)(p_msg->buffer))[18];
							/*新数据长度和命令字组装*/
							((uint8_t *)(p_msg->buffer))[1] = cmd_l;
							((uint8_t *)(p_msg->buffer))[2] = cmd_h;
							((uint8_t *)(p_msg->buffer))[7] = len_l;
							((uint8_t *)(p_msg->buffer))[8] = len_h;
							/*数据整体前移，冲掉命令字和数据长度*/
							for(i = 13; i < (CAN_RX_CMD_MAX_SIZE * 8 - 4); i++)	
							{
								((uint8_t *)(p_msg->buffer))[i] = ((uint8_t *)(p_msg->buffer))[i + 4];
							}
							p_msg->size = ((len_l + len_h * 256) - 2) / 7 + ((((len_l + len_h * 256) - 2) % 7) == 0 ? 0 : 1) + 2;	/*通过数据长度反算帧数*/
							/*调度*/
							if(false == Prt_CmdScheduler(p_msg, p_cmd_regedit))
							{
								Drv_CANProtocol_SendErrorToBuffer(p_msg->can_id, p_msg->buffer[0][0], 
																	((uint16_t)(p_msg->buffer[0][2]) << 8 | (uint16_t)(p_msg->buffer[0][1])), 
																	((uint32_t)(p_msg->buffer[0][12]) << 24 | (uint32_t)(p_msg->buffer[0][11]) << 16 |(uint32_t)(p_msg->buffer[0][10]) << 8 |(uint32_t)(p_msg->buffer[0][9])), 
																	0xFF, 0xFF, 0x06);		/*报错，不支持该指令*/
							}
							/*数据整体前移，冲掉数据域*/
							for(i = 15; i < (CAN_RX_CMD_MAX_SIZE * 8 - (len_l + len_h * 256 - 2)); i++)
							{
								((uint8_t *)(p_msg->buffer))[i] = ((uint8_t *)(p_msg->buffer))[i + (len_l + len_h * 256 - 2)];
							}
							count--;
						}
					}
					else
					{
						/*调度*/
						if(false == Prt_CmdScheduler(p_msg, p_cmd_regedit))
						{
							Drv_CANProtocol_SendErrorToBuffer(p_msg->can_id, p_msg->buffer[0][0], 
																((uint16_t)(p_msg->buffer[0][2]) << 8 | (uint16_t)(p_msg->buffer[0][1])), 
																((uint32_t)(p_msg->buffer[0][12]) << 24 | (uint32_t)(p_msg->buffer[0][11]) << 16 |(uint32_t)(p_msg->buffer[0][10]) << 8 |(uint32_t)(p_msg->buffer[0][9])), 
																0xFF, 0xFF, 0x06);		/*报错，不支持该指令*/
						}
					}
					
					/*释放*/
					Drv_CANProtocol_RXBufferFree(1, p_msg);		/*释放该接收缓冲区*/
					p_msg = NULL;
				}
			}
		}
	}
}

/* 系统指示灯任务
  -----------------------------------ok
*/
static void TaskLed(void* p_arg)
{
	DRV_LED_TYPE* p_led = Led_Init(LED_1, CONFIG_LED_LOGIC, CONFIG_LED_PORT, CONFIG_LED_PIN);
	
	if(p_led == NULL)
	{
		while(1)
		{
			OSTimeDlyHMSM(0, 0, 0, 500);	//出错需要处理
		}
	}
	else
	{
		while(1)
		{
			Led_On(p_led);
			OSTimeDlyHMSM(0, 0, 0, 500);
			Led_Off(p_led);
			OSTimeDlyHMSM(0, 0, 0, 500);
		}
	}
}
