#ifndef _TASK_H_
#define _TASK_H_

#include "includes.h"	 
#include "unitinfo.h"

#define START_STK_SIZE   512

//设置任务堆栈大小
#define LED_STK_SIZE     64  
//#define UART4_STK_SIZE     64
#define APP_TASK_CMD_STK_SIZE   512
#define APP_TASK_PARES_CMD_STK_SIZE 256

//设置任务优先级
#define START_TASK_Prio     20

#define LED_TASK_Prio       19

#define  APP_MVERT_PRIO     18

#define  APP_AD_PRIO        17 //ad
//申请3个任务优先级
#define  APP_PRASE_CMD_PRIO     16
#define  APP_TASK2_PRIO     15
#define  APP_TASK3_PRIO     14
#define UART4_TASK_Prio     13

#define  APP_TASK_Mix_PRIO  3
#define  APP_TASK_Smp_PRIO  4

//任务申明
void TaskStart(void *pdata);
void TaskLed(void *pdata);
//void TaskLed1(void *pdata);
//void TaskKey(void *pdata);
//void TaskCmdMix(void  *parg);

//void  TaskMVert_move(void *pdata);

extern OS_STK  TASK_START_STK[START_STK_SIZE];

#endif
