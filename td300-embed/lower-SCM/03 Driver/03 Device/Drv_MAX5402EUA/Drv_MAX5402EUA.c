/********************************************************
   版权所有 (C), 2001-2100, 四川新健康成生物股份有限公司
  -------------------------------------------------------

			嵌入式开发平台——MAX5402EUA数字电位器模块

  -------------------------------------------------------
   文 件 名   : Drv_MAX5402EUA.c
   版 本 号   : V1.0.0.0
   作    者   : ryc
   生成日期   : 2019年10月14日
   功能描述   : 定义MAX5402EUA数字电位器的驱动。
   使用方法   ：上层模块通过调用MAX5402EUA_Init获得一个MAX5402EUA
				的指针，通过调用MAX5402EUASet设置增益值。
   依赖于     : Drv_Software_Spi V1.0.0.0
   注         ：该模块不依赖于硬件电路。 
 ********************************************************/

#include "Drv_MAX5402EUA.h"

DRV_MAX5402EUA_TYPE* _gp_Max5402euaUnits[MAX5402EUA_BUTT] = {0};	/*MAX5402EUA数字电位器模块的注册表*/

/* 接口函数
 ********************************************************/

/* 初始化MAX5402EUA模块
  -----------------------------
  入口：MAX5402EUA模块的ID号，电阻值
  返回值：初始化好的MAX5402EUA模块结构体
*/
DRV_MAX5402EUA_TYPE* Max5402euaInit(DRV_MAX5402EUA_ENUM id, \
									uint8_t gain, \
									BSP_PORT_ENUM sdi_port_number, BSP_PIN_ENUM sdi_pin_number, \
									BSP_PORT_ENUM sdo_port_number, BSP_PIN_ENUM sdo_pin_number, \
									BSP_PORT_ENUM sck_port_number, BSP_PIN_ENUM sck_pin_number, \
									BSP_PORT_ENUM cs_port_number, BSP_PIN_ENUM cs_pin_number)
{	
	DRV_MAX5402EUA_TYPE* p_unit = (DRV_MAX5402EUA_TYPE*)calloc(1, sizeof(DRV_MAX5402EUA_TYPE));	/*申请内存*/

	/*分配失败返回NULL*/
	if(p_unit == NULL)
	{
		return NULL;
	}
		
	/*id错误返回NULL*/
	if(id >= SOFTWARE_SPI_BUTT)
	{
		free(p_unit); 
		return NULL;
	}
	
	/*如果当前模块存在，取消分配新存储区*/
	if(_gp_Max5402euaUnits[id] != NULL)
	{
		free(p_unit);
		p_unit = _gp_Max5402euaUnits[id];
	}
	else
	{
		_gp_Max5402euaUnits[id] = p_unit;
	}
	
	/*初始化*/
	
	/*-------------------------------常量*/
	p_unit->id          = id;						/*模块id*/
	p_unit->spi_port    = SoftwareSpiInit((DRV_SOFTWARE_SPI_ENUM)id, 7, \
											sdi_port_number, sdi_pin_number, \
											sdo_port_number, sdo_pin_number, \
											sck_port_number, sck_pin_number, \
											cs_port_number, cs_pin_number);
	
	/*-------------------------------变量*/
	p_unit->status = MAX5402EUA_UNDEFINED;
	if(gain < 85)
		gain = 85;
	p_unit->gain = gain;
	
	SoftwareSpiWriteByte(p_unit->spi_port, gain);
	
	return p_unit;
}

/* 设置MAX5402EUA阻值
  -----------------------------
  入口：MAX5402EUA模块的ID号，电阻档数
  返回值：true 成功，false 失败
*/
bool Max5402euaSet(DRV_MAX5402EUA_TYPE* p_unit, uint8_t gain)
{
	if(gain < 10)
		gain = 10;
	SoftwareSpiWriteByte(p_unit->spi_port, gain);
	return true;
}
