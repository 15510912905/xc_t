/********************************************************
   版权所有 (C), 2001-2100, 四川新健康成生物股份有限公司
  -------------------------------------------------------

			嵌入式开发平台——AD7685模数转换模块

  -------------------------------------------------------
   文 件 名   : Drv_AD7685.h
   版 本 号   : V1.0.0.0
   作    者   : ryc
   生成日期   : 2019年10月15日
   功能描述   : 定义AD7685模数转换器的驱动。
   使用方法   ：上层模块通过调用Ad7685Init获得一个AD7685
				的指针。
   依赖于     : 
   注         ：该模块不依赖于硬件电路。 
 ********************************************************/
 
#ifndef _DRV_AD7685_H_
#define _DRV_AD7685_H_ 

#include "stdint.h"
#include "stdlib.h"
#include "stdbool.h"
#include "BSP_Spi.h"

#define DUMMY   0xFFFF

/*AD7685模块枚举*/
typedef enum{
	AD7685_1 = 0,
	AD7685_BUTT
} DRV_AD7685_ENUM;

/*AD7685模块状态枚举*/
typedef enum{
	AD7685_UNDEFINED = 0,				/*未初始化*/
	AD7685_READY  = 1					/*可用*/
} DRV_AD7685_STATUS_ENUM;

/*AD7685模块结构体*/
typedef struct{
	/*------------------------------变量*/
	DRV_AD7685_STATUS_ENUM status;			/*该AD7685模块的状态*/
	uint16_t tx_buffer;						/*发送缓存*/
	uint16_t rx_buffer;						/*接收缓存*/
	/*------------------------------变常*/
	/*------------------------------常量*/
	DRV_AD7685_ENUM id;						/*AD7685模块id*/
	uint32_t theta;							/*时钟速度参数*/
	BSP_SPI_ENUM spi_number;				/*spi接口编号*/
	BSP_PORT_ENUM nss_port_number; 
	BSP_PIN_ENUM nss_pin_number;
	BSP_PORT_ENUM sck_port_number;
	BSP_PIN_ENUM sck_pin_number;
	BSP_PORT_ENUM miso_port_number;
	BSP_PIN_ENUM miso_pin_number;
	BSP_PORT_ENUM mosi_port_number;
	BSP_PIN_ENUM mosi_pin_number;
	
} DRV_AD7685_TYPE;

/* 接口函数
 ********************************************************/

/* 初始化AD7685模块
  -----------------------------
  入口：AD7685模块的ID号，SPI端口编号，引脚端口号
  返回值：初始化好的AD7685模块结构体
*/
extern DRV_AD7685_TYPE* Drv_Ad7685Init(DRV_AD7685_ENUM id, BSP_SPI_ENUM spi_number, \
										BSP_PORT_ENUM nss_port_number, BSP_PIN_ENUM nss_pin_number, \
										BSP_PORT_ENUM sck_port_number, BSP_PIN_ENUM sck_pin_number, \
										BSP_PORT_ENUM miso_port_number, BSP_PIN_ENUM miso_pin_number, \
										BSP_PORT_ENUM mosi_port_number, BSP_PIN_ENUM mosi_pin_number);

/*  AD7685模块完成一次采样
  -----------------------------
  入口：AD7685模块结构体指针
  返回值：true 成功，false 失败
*/
extern uint16_t Ad7685OneAcq(DRV_AD7685_TYPE* p_unit);

/* 内部函数
 ********************************************************/

/* AD7685启动
  -----------------------------
  入口：AD7685模块结构体指针
  返回值：true 成功，false 失败
*/
static bool _Ad7685Start(DRV_AD7685_TYPE* p_unit);

/* AD7685等待转换
  -----------------------------
  入口：AD7685模块结构体指针
  返回值：true 成功，false 失败
*/
static bool _Ad7685Wait(DRV_AD7685_TYPE* p_unit);

/*  AD7685模块延时n微秒
  -----------------------------
  入口：延时时间
  返回值：true 成功，false 失败
*/
static bool _Ad7685DelayUs(DRV_AD7685_TYPE* p_unit, uint32_t time);

/*  AD7685模块读取AD值
  -----------------------------
  入口：命令
  返回值：true 成功，false 失败
*/
static uint16_t _Ad7685ReadData(DRV_AD7685_TYPE* p_unit);

#endif
