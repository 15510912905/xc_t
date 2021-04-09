/********************************************************
   版权所有 (C), 2001-2100, 四川新健康成生物股份有限公司
  -------------------------------------------------------

		嵌入式开发平台——TD300内壁清洗泵功能定义

  -------------------------------------------------------
   文 件 名   : Func_TD300_InnerWallCleaningPump.c
   版 本 号   : V1.0.0.0
   作    者   : ryc
   生成日期   : 2019年12月5日
   功能描述   : TD300内壁清洗泵功能定义
   依赖于     : 
 ********************************************************/

#ifndef _FUNC_TD300_INNERWALLCLEANINGPUMP_H_
#define _FUNC_TD300_INNERWALLCLEANINGPUMP_H_

#include "stdint.h"
#include "Drv_CANProtocol.h"
#include "Drv_MotionControlComponents.h"
#include "Drv_Valve.h"
#include "Drv_Pump.h"
#include "Func_Common.h"
#include "Prt_CmdRegedit.h"

#pragma pack(1)	/*按1字节对齐*/

/*自动清洗单个泵阀控制*/
typedef struct {
	uint16_t cmd;				/*命令字*/
	uint8_t number;				/*泵阀编号*/
	uint8_t status;				/*控制状态*/
} CMD_AUTOWASH_PVCONTROL_TYPE;

#pragma pack()

/* 私有命令字定义
  ---------------------------------------*/
  
typedef enum {
	CMD_AUTOWASH_PVCONTROL = 0x0084,	/*单个泵阀控制*/
} CMD_AUTOWASH_ENUM;

/* 初始化----------------------------------------------------*/

/* 初始化 Func_TD300InnerWallCleaningPumpInit
  ------------------------------
  初始化硬件
  返回值：true，成功；false，失败
*/
bool Func_TD300InnerWallCleaningPumpInit(void);

/* 私有常驻任务----------------------------------------------------*/


/* 私有指令任务----------------------------------------------------*/

/* 自动清洗单个泵阀控制
  ------------------------------
  返回值：true，成功；false，失败
*/
extern bool Func_CmdAutoWashPvControl(void* p_buffer);

#endif
