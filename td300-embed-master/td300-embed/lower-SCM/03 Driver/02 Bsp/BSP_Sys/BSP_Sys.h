/********************************************************
   版权所有 (C), 2001-2100, 四川新健康成生物股份有限公司
  -------------------------------------------------------

				嵌入式开发平台——BSP层 系统封装

  -------------------------------------------------------
   文 件 名   : BSP_Sys.c
   版 本 号   : V1.0.0.0
   作    者   : ryc
   生成日期   : 2019年07月17日
   功能描述   : 定义一种基于STM32的板级支持包。
				该层依赖于硬件电路。
   依赖于     : STM32F10x_StdPeriph_Lib_V3.5.0
 ********************************************************/

#ifndef _BSP_SYS_H_
#define _BSP_SYS_H_

#include "stdint.h"
#include "stdbool.h"
#include "stm32f10x.h"

/* 初始化
  ----------------------------------------------------------------------*/

/* 初始化BSP
  -----------------------------------------
  返回值：成功true，失败false
*/
extern bool BSP_Init(void);

/* 初始化SysTick
  -----------------------------------------
  入口参数：滴答时钟频率、每秒滴答次数
  返回值：成功true，失败false
*/
extern bool BSP_InitSysTick(uint8_t clk, uint16_t ticks_per_sec);

/* 软复位
  -----------------------------------------
  返回值：无
*/
extern void BSP_Reboot(void);

#endif