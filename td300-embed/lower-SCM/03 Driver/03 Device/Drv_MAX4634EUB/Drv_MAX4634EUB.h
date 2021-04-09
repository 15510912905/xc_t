/********************************************************
   版权所有 (C), 2001-2100, 四川新健康成生物股份有限公司
  -------------------------------------------------------

			嵌入式开发平台——MAX4634EUB多路开关模块

  -------------------------------------------------------
   文 件 名   : Drv_MAX4634EUB.h
   版 本 号   : V1.0.0.0
   作    者   : ryc
   生成日期   : 2019年10月14日
   功能描述   : 定义MAX4634EUB多路开关的驱动。
   使用方法   ：上层模块通过调用Max4634eubInit获得一个MAX4634EUB
				的指针，通过调用Max4634eubSet设置选通哪一路。
   依赖于     : Drv_Software_Spi V1.0.0.0
   注         ：该模块不依赖于硬件电路。 
 ********************************************************/
 
#ifndef _DRV_MAX4634EUB_H_
#define _DRV_MAX4634EUB_H_ 

#include "stdint.h"
#include "stdlib.h"
#include "stdbool.h"
#include "BSP_Gpio.h"

/*MAX4634EUB模块枚举*/
typedef enum{
	MAX4634EUB_1 = 0,
	MAX4634EUB_BUTT
} DRV_MAX4634EUB_ENUM;

/*MAX4634EUB模块状态枚举*/
typedef enum{
	MAX4634EUB_UNDEFINED = 0,				/*未初始化*/
	MAX4634EUB_READY  = 1					/*可用*/
} DRV_MAX4634EUB_STATUS_ENUM;

/*MAX4634EUB模块结构体*/
typedef struct{
	/*------------------------------变量*/
	DRV_MAX4634EUB_STATUS_ENUM status;		/*该MAX4634EUB模块的状态*/
	bool a0_pin_buffer;						/*a0输出引脚缓存*/
	bool a1_pin_buffer;						/*a1输出引脚缓存*/
	uint8_t choose;							/*通道选择*/
	/*------------------------------变常*/
	/*------------------------------常量*/
	DRV_MAX4634EUB_ENUM id;					/*MAX5402EUA模块id*/
	BSP_PORT_ENUM a0_port_number; 			/*a0引脚端口号*/
	BSP_PIN_ENUM a0_pin_number;				/*a0引脚编号*/
	BSP_PORT_ENUM a1_port_number; 			/*a1引脚端口号*/
	BSP_PIN_ENUM a1_pin_number;				/*a1引脚编号*/
	
} DRV_MAX4634EUB_TYPE;

/* 接口函数
 ********************************************************/

/* 初始化MAX4634EUB模块
  -----------------------------
  入口：MAX4634EUB模块的ID号，引脚端口
  返回值：初始化好的MAX4634EUB模块结构体
*/
extern DRV_MAX4634EUB_TYPE* Max4634eubInit(DRV_MAX4634EUB_ENUM id, \
											BSP_PORT_ENUM a0_port_number, BSP_PIN_ENUM a0_pin_number, \
											BSP_PORT_ENUM a1_port_number, BSP_PIN_ENUM a1_pin_number);

/* MAX4634EUB模块选择通道
  -----------------------------
  入口：MAX4634EUB模块的ID号，通道号
  返回值：true 成功，flase 失败
*/
extern bool Max4634eubChoose(DRV_MAX4634EUB_TYPE* p_unit, uint8_t choose);

#endif
