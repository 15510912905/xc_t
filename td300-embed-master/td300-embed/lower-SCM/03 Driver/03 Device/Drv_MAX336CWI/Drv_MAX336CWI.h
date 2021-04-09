/********************************************************
   版权所有 (C), 2001-2100, 四川新健康成生物股份有限公司
  -------------------------------------------------------

			嵌入式开发平台——MAX336CWI多路开关模块

  -------------------------------------------------------
   文 件 名   : Drv_MAX336CWI.c
   版 本 号   : V1.0.0.0
   作    者   : ryc
   生成日期   : 2019年11月29日
   功能描述   : 定义MAX336CWI多路开关的驱动。
   使用方法   ：上层模块通过调用Max336cwiInit获得一个MAX336CWI
				的指针，通过调用Max336cwiSet设置选通哪一路。
   依赖于     : 
   注         ：该模块不依赖于硬件电路。 
 ********************************************************/
 
#ifndef _DRV_MAX336CWI_H_
#define _DRV_MAX336CWI_H_ 

#include "stdint.h"
#include "stdlib.h"
#include "stdbool.h"
#include "BSP_Gpio.h"

/*MAX336CWI模块枚举*/
typedef enum{
	MAX336CWI_1 = 0,
	MAX336CWI_2,
	MAX336CWI_BUTT
} DRV_MAX336CWI_ENUM;

/*MAX336CWI模块状态枚举*/
typedef enum{
	MAX336CWI_UNDEFINED = 0,				/*未初始化*/
	MAX336CWI_READY  = 1					/*可用*/
} DRV_MAX336CWI_STATUS_ENUM;

/*MAX336CWI模块结构体*/
typedef struct{
	/*------------------------------变量*/
	DRV_MAX336CWI_STATUS_ENUM status;		/*该MAX336CWI模块的状态*/
	bool a0_pin_buffer;						/*a0输出引脚缓存*/
	bool a1_pin_buffer;						/*a1输出引脚缓存*/
	bool a2_pin_buffer;						/*a2输出引脚缓存*/
	bool a3_pin_buffer;						/*a3输出引脚缓存*/
	uint8_t choose;							/*通道选择*/
	/*------------------------------变常*/
	/*------------------------------常量*/
	DRV_MAX336CWI_ENUM id;					/*MAX336CWI模块id*/
	BSP_PORT_ENUM a0_port_number; 			/*a0引脚端口号*/
	BSP_PIN_ENUM a0_pin_number;				/*a0引脚编号*/
	BSP_PORT_ENUM a1_port_number; 			/*a1引脚端口号*/
	BSP_PIN_ENUM a1_pin_number;				/*a1引脚编号*/
	BSP_PORT_ENUM a2_port_number; 			/*a2引脚端口号*/
	BSP_PIN_ENUM a2_pin_number;				/*a2引脚编号*/
	BSP_PORT_ENUM a3_port_number; 			/*a3引脚端口号*/
	BSP_PIN_ENUM a3_pin_number;				/*a3引脚编号*/
	
} DRV_MAX336CWI_TYPE;

/* 接口函数
 ********************************************************/

/* 初始化MAX336CWI模块
  -----------------------------
  入口：MAX336CWI模块的ID号，引脚端口
  返回值：初始化好的MAX336CWI模块结构体
*/
extern DRV_MAX336CWI_TYPE* Max336cwiInit(DRV_MAX336CWI_ENUM id, \
											BSP_PORT_ENUM a0_port_number, BSP_PIN_ENUM a0_pin_number, \
											BSP_PORT_ENUM a1_port_number, BSP_PIN_ENUM a1_pin_number, \
											BSP_PORT_ENUM a2_port_number, BSP_PIN_ENUM a2_pin_number, \
											BSP_PORT_ENUM a3_port_number, BSP_PIN_ENUM a3_pin_number);

/* MAX336CWI模块选择通道
  -----------------------------
  入口：MAX336CWI模块的ID号，通道号
  返回值：true 成功，flase 失败
*/
extern bool Max336cwiChoose(DRV_MAX336CWI_TYPE* p_unit, uint8_t choose);

#endif
