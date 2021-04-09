/********************************************************
   版权所有 (C), 2001-2100, 四川新健康成生物股份有限公司
  -------------------------------------------------------

				嵌入式开发平台——任务定义

  -------------------------------------------------------
   文 件 名   : Tesk.h
   版 本 号   : V1.0.0.0
   作    者   : ryc
   生成日期   : 2019年08月01日
   功能描述   : 定义并行任务。
   依赖于     : uC/OS-II V2.92.07
				Drv_Led V1.0.0.0
   注         ：该模块不依赖于硬件电路。 
 ********************************************************/

#ifndef _TASK_H_
#define _TASK_H_

#include "stdlib.h"
#include "ucos_ii.h"
#include "Drv_Led.h"
#include "Drv_CANProtocol.h"
#include "Func_Common.h"
#include "Prt_CmdRegedit.h"
#include "Prt_CmdScheduler.h"

/*----------------------------------------------------可配置参数*/
#define CONFIG_CAN_BAUD_RATE				500		/*CAN通信的波特率(k)，注意，单独修改可能并不准确，需要配合修改BS1\BS2*/


/*--------------------------------------------------------------*/
#ifdef TD300_DETECTION												/*TD300 AD检测板*/

	#define CONFIG_CAN_ID_1					10		/*CAN ID 1*/
	#define CONFIG_CAN_ID_2					0		/*CAN ID 2*/

	#define CONFIG_CAN_TX_PORT				_PA_				/*TX端口*/
	#define CONFIG_CAN_TX_PIN				_P12_				/*TX引脚*/
	#define CONFIG_CAN_RX_PORT				_PA_				/*RX端口*/
	#define CONFIG_CAN_RX_PIN				_P11_				/*RX引脚*/

	#define CONFIG_LED_LOGIC				LED_POSITIVE_LOGIC	/*LED控制逻辑*/
	#define CONFIG_LED_PORT					_PC_				/*LED端口*/
	#define CONFIG_LED_PIN					_P12_				/*LED引脚*/

/*--------------------------------------------------------------*/
#elif TD300_REACTION_DISK_AUTOWASH									/*TD300 反应盘自动清洗驱动板*/

	#define CONFIG_CAN_ID_1					5		/*CAN ID 1*/
	#define CONFIG_CAN_ID_2					6		/*CAN ID 2*/

	#define CONFIG_CAN_TX_PORT				_PA_				/*TX端口*/
	#define CONFIG_CAN_TX_PIN				_P12_				/*TX引脚*/
	#define CONFIG_CAN_RX_PORT				_PA_				/*RX端口*/
	#define CONFIG_CAN_RX_PIN				_P11_				/*RX引脚*/

	#define CONFIG_LED_LOGIC				LED_POSITIVE_LOGIC	/*LED控制逻辑*/
	#define CONFIG_LED_PORT					_PC_				/*LED端口*/
	#define CONFIG_LED_PIN					_P1_				/*LED引脚*/

/*--------------------------------------------------------------*/

#elif TD300_SCATTER

	#define CONFIG_CAN_ID_1					13		/*CAN ID 1*/
	#define CONFIG_CAN_ID_2					0		/*CAN ID 2*/
	
	#define CONFIG_CAN_TX_PORT				_PA_				/*TX端口*/
	#define CONFIG_CAN_TX_PIN				_P12_				/*TX引脚*/
	#define CONFIG_CAN_RX_PORT				_PA_				/*RX端口*/
	#define CONFIG_CAN_RX_PIN				_P11_				/*RX引脚*/

	#define CONFIG_LED_LOGIC				LED_POSITIVE_LOGIC	/*LED控制逻辑*/
	#define CONFIG_LED_PORT					_PC_				/*LED端口*/
	#define CONFIG_LED_PIN					_P12_				/*LED引脚*/

#elif XC2000_CDB_V1_0												/*XC2000 大驱动板*/

	#ifdef XC2000_SAMPLE_ARM_1							/*XC2000 第一加样*/

		#define CONFIG_CAN_ID_1					1		/*CAN ID 1*/
		#define CONFIG_CAN_ID_2					0		/*CAN ID 2*/

	#elif XC2000_SAMPLE_ARM_2							/*XC2000 第二加样*/

		#define CONFIG_CAN_ID_1					2		/*CAN ID 1*/
		#define CONFIG_CAN_ID_2					0		/*CAN ID 2*/

	#elif XC2000_SAMPLE_STIRRING_AND_REAGENT_STIRRING	/*XC2000 样本搅拌与试剂搅拌*/

		#define CONFIG_CAN_ID_1					5		/*CAN ID 1*/
		#define CONFIG_CAN_ID_2					6		/*CAN ID 2*/

	#elif XC2000_REAGENT_ARM_1_1_AND_REAGENT_DISK_1		/*XC2000 第一R1试剂臂与R1试剂盘*/

		#define CONFIG_CAN_ID_1					7		/*CAN ID 1*/
		#define CONFIG_CAN_ID_2					8		/*CAN ID 2*/

	#elif XC2000_REAGENT_ARM_1_2						/*XC2000 第二R1试剂臂*/

		#define CONFIG_CAN_ID_1					9		/*CAN ID 1*/
		#define CONFIG_CAN_ID_2					0		/*CAN ID 2*/

	#elif XC2000_REAGENT_ARM_2_1_AND_REAGENT_DISK_2		/*XC2000 第一R2试剂臂与R2试剂盘*/

		#define CONFIG_CAN_ID_1					10		/*CAN ID 1*/
		#define CONFIG_CAN_ID_2					11		/*CAN ID 2*/

	#elif XC2000_REAGENT_ARM_2_2						/*XC2000 第二R2试剂臂*/

		#define CONFIG_CAN_ID_1					12		/*CAN ID 1*/
		#define CONFIG_CAN_ID_2					0		/*CAN ID 2*/

	#elif XC2000_CONVENTIONAL_SAMPLE					/*XC2000 常规通道定位*/

		#define CONFIG_CAN_ID_1					19		/*CAN ID 1*/
		#define CONFIG_CAN_ID_2					0		/*CAN ID 2*/

	#elif XC2000_EMERGENCY_SAMPLE						/*XC2000 急诊通道定位*/

		#define CONFIG_CAN_ID_1					20		/*CAN ID 1*/
		#define CONFIG_CAN_ID_2					0		/*CAN ID 2*/

	#elif XC2000_SAMPLING_AREA							/*XC2000 前端进样区*/

		#define CONFIG_CAN_ID_1					1		/*CAN ID 1*/
		#define CONFIG_CAN_ID_2					0		/*CAN ID 2*/

	#elif XC2000_SWEEP_CODE_CHANNEL						/*XC2000 前端扫描通道*/

		#define CONFIG_CAN_ID_1					2		/*CAN ID 1*/
		#define CONFIG_CAN_ID_2					0		/*CAN ID 2*/

	#elif XC2000_DISPATCHER								/*XC2000 前端调度*/

		#define CONFIG_CAN_ID_1					3		/*CAN ID 1*/
		#define CONFIG_CAN_ID_2					0		/*CAN ID 2*/

	#elif XC2000_TRANSFER_CHANNEL						/*XC2000 前端传输通道*/

		#define CONFIG_CAN_ID_1					4		/*CAN ID 1*/
		#define CONFIG_CAN_ID_2					0		/*CAN ID 2*/

	#elif XC2000_RECOVERY_AREA							/*XC2000 前端回收区*/

		#define CONFIG_CAN_ID_1					5		/*CAN ID 1*/
		#define CONFIG_CAN_ID_2					0		/*CAN ID 2*/

	#elif XC2000_SAMPLING_CHANNEL						/*XC2000 后端常规通道*/

		#define CONFIG_CAN_ID_1					6		/*CAN ID 1*/
		#define CONFIG_CAN_ID_2					0		/*CAN ID 2*/

	#elif XC2000_ORBITAL_TRANSFER						/*XC2000 后端变轨*/

		#define CONFIG_CAN_ID_1					7		/*CAN ID 1*/
		#define CONFIG_CAN_ID_2					0		/*CAN ID 2*/
		
	#elif XC2000_RECYCLING_CHANNEL						/*XC2000 后端回收通道*/

		#define CONFIG_CAN_ID_1					8		/*CAN ID 1*/
		#define CONFIG_CAN_ID_2					0		/*CAN ID 2*/
		
	#endif

	#define CONFIG_CAN_TX_PORT				_PB_				/*TX端口*/
	#define CONFIG_CAN_TX_PIN				_P9_				/*TX引脚*/
	#define CONFIG_CAN_RX_PORT				_PB_				/*RX端口*/
	#define CONFIG_CAN_RX_PIN				_P8_				/*RX引脚*/

	#define CONFIG_LED_LOGIC				LED_POSITIVE_LOGIC	/*LED控制逻辑*/
	#define CONFIG_LED_PORT					_PA_				/*LED端口*/
	#define CONFIG_LED_PIN					_P3_				/*LED引脚*/

/*--------------------------------------------------------------*/
#elif XC2000_AWB_V1_0												/*XC2000 反应盘自动清洗驱动板*/

	#define CONFIG_CAN_ID_1					3		/*CAN ID 1*/
	#define CONFIG_CAN_ID_2					4		/*CAN ID 2*/

	#define CONFIG_CAN_TX_PORT				_PB_				/*TX端口*/
	#define CONFIG_CAN_TX_PIN				_P9_				/*TX引脚*/
	#define CONFIG_CAN_RX_PORT				_PB_				/*RX端口*/
	#define CONFIG_CAN_RX_PIN				_P8_				/*RX引脚*/

	#define CONFIG_LED_LOGIC				LED_POSITIVE_LOGIC	/*LED控制逻辑*/
	#define CONFIG_LED_PORT					_PA_				/*LED端口*/
	#define CONFIG_LED_PIN					_P3_				/*LED引脚*/

/*--------------------------------------------------------------*/
#elif XC2000_DETECTION												/*XC2000 AD转接板*/

	#define CONFIG_CAN_ID_1					16		/*CAN ID 1*/
	#define CONFIG_CAN_ID_2					17		/*CAN ID 2*/

	#define CONFIG_CAN_TX_PORT				_PB_				/*TX端口*/
	#define CONFIG_CAN_TX_PIN				_P9_				/*TX引脚*/
	#define CONFIG_CAN_RX_PORT				_PB_				/*RX端口*/
	#define CONFIG_CAN_RX_PIN				_P8_				/*RX引脚*/

	#define CONFIG_LED_LOGIC				LED_POSITIVE_LOGIC	/*LED控制逻辑*/
	#define CONFIG_LED_PORT					_PORT_BUTT_			/*LED端口*/
	#define CONFIG_LED_PIN					_PIN_BUTT_			/*LED引脚*/

/*--------------------------------------------------------------*/
#else																/*TD300 电机驱动板*/

	/*===================================================TD300*/

	#ifdef TD300_TRANSFER_ORBIT				/*TD300 轨道*/

		#define CONFIG_CAN_ID_1					12		/*CAN ID 1*/
		#define CONFIG_CAN_ID_2					0		/*CAN ID 2*/

	#elif TD300_STRIRRING					/*TD300 搅拌*/

		#define CONFIG_CAN_ID_1					4		/*CAN ID 1*/
		#define CONFIG_CAN_ID_2					0		/*CAN ID 2*/

	#elif TD300_SAMPLE_ARM					/*TD300 加样*/

		#define CONFIG_CAN_ID_1					3		/*CAN ID 1*/
		#define CONFIG_CAN_ID_2					11		/*CAN ID 2*/

	#elif TD300_REAGENT_ARM_REAGENT_DISK	/*TD300 试剂*/

		#define CONFIG_CAN_ID_1					1		/*CAN ID 1*/
		#define CONFIG_CAN_ID_2					2		/*CAN ID 2*/

	/*==================================================XC2000*/

	#elif XC2000_REAGENT_ARM_1_1_REAGENT_DISK_1		/*XC2000 第一试剂R1*//*弃用*/

		#define CONFIG_CAN_ID_1					7		/*CAN ID 1*/
		#define CONFIG_CAN_ID_2					8		/*CAN ID 2*/

	#endif

	/*========================================================*/

	#define CONFIG_CAN_TX_PORT				_PD_				/*TX端口*/
	#define CONFIG_CAN_TX_PIN				_P1_				/*TX引脚*/
	#define CONFIG_CAN_RX_PORT				_PD_				/*RX端口*/
	#define CONFIG_CAN_RX_PIN				_P0_				/*RX引脚*/

	#define CONFIG_LED_LOGIC				LED_POSITIVE_LOGIC	/*LED控制逻辑*/
	#define CONFIG_LED_PORT					_PD_				/*LED端口*/
	#define CONFIG_LED_PIN					_P8_				/*LED引脚*/

#endif
/*--------------------------------------------------------------*/


/*----------------------------------------------------定义任务堆栈深度*/

#define TASK_START_STK_SIZE				512		/*初始任务 堆栈深度*/

#define TASK_CMD_TRANSCEIVER_STK_SIZE	512		/*指令收发器任务 堆栈深度*/
#define TASK_CMD_SCHEDULER1_STK_SIZE	512		/*指令调度器1任务 堆栈深度*/
#define TASK_CMD_SCHEDULER2_STK_SIZE	512		/*指令调度器2任务 堆栈深度*/

#define TASK_LED_STK_SIZE				512		/*系统指示灯任务 堆栈深度*/
#define TASK_RESIDENT_1_STK_SIZE		512		/*私有常驻任务 堆栈深度*/
#define TASK_RESIDENT_2_STK_SIZE		512		/*私有常驻任务 堆栈深度*/

/*----------------------------------------------------定义任务优先级，数值越小优先级越高*/

#define TASK_START_PRIO					20		/*初始任务 优先级*/

#define TASK_CMD_TRANSCEIVER_PRIO		4		/*指令收发器任务 优先级*/
//#define MUTEX_PRIO					5		/*互斥信号量任务提升优先级*/
#define TASK_CMD_SCHEDULER1_PRIO		6		/*指令调度器1任务 优先级*/
#define TASK_CMD_SCHEDULER2_PRIO		7		/*指令调度器2任务 优先级*/
#define TASK_RESIDENT_1_PRIO			8		/*私有常驻任务1 优先级*/
#define TASK_RESIDENT_2_PRIO			9		/*私有常驻任务1 优先级*/

#define TASK_LED_PRIO					19		/*系统指示灯任务 优先级*/

/*----------------------------------------------------变量及函数声明*/

extern OS_STK g_TaskStart_Stk[TASK_START_STK_SIZE];

/* 初始线程
  -----------------------------------
*/
extern void TaskStart(void* p_arg);

/* 指令收发器线程
  -----------------------------------
*/
static void TaskCmdTransceiver(void* p_arg);

/* 指令调度器1线程（私有指令线程1）
  -----------------------------------
*/
static void TaskCmdScheduler1(void* p_arg);

/* 指令调度器2线程（私有指令线程2）
  -----------------------------------
*/
static void TaskCmdScheduler2(void* p_arg);

/* LED指示灯线程
  -----------------------------------
*/
static void TaskLed(void* p_arg);

/* 私有常驻线程1
  -----------------------------------
  由Func层实现
*/
extern void TaskResident1(void* p_arg);

/* 私有常驻线程2
  -----------------------------------
  由Func层实现
*/
extern void TaskResident2(void* p_arg);

#endif
