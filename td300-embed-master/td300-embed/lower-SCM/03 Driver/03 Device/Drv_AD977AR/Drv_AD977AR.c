/********************************************************
   版权所有 (C), 2001-2100, 四川新健康成生物股份有限公司
  -------------------------------------------------------

			嵌入式开发平台——AD977AR模数转换模块

  -------------------------------------------------------
   文 件 名   : Drv_AD977AR.c
   版 本 号   : V1.0.0.0
   作    者   : ryc
   生成日期   : 2019年11月29日
   功能描述   : 定义AD977AR模数转换器的驱动。
   使用方法   ：上层模块通过调用Ad977arInit获得一个AD977AR
				的指针。
   依赖于     : 
   注         ：该模块不依赖于硬件电路。 
 ********************************************************/

#include "Drv_AD977AR.h"

DRV_AD977AR_TYPE* _gp_Ad977arUnits[AD977AR_BUTT] = {0};	/*AD977AR模数转换模块的注册表*/

/* 接口函数
 ********************************************************/

/* 初始化AD977AR模块
  -----------------------------
  入口：AD977AR模块的ID号，SPI端口编号，引脚端口号
  返回值：初始化好的AD977AR模块结构体
*/
DRV_AD977AR_TYPE* Drv_Ad977arInit(DRV_AD977AR_ENUM id, BSP_SPI_ENUM spi_number, \
								BSP_PORT_ENUM nss_port_number, BSP_PIN_ENUM nss_pin_number, \
								BSP_PORT_ENUM sck_port_number, BSP_PIN_ENUM sck_pin_number, \
								BSP_PORT_ENUM miso_port_number, BSP_PIN_ENUM miso_pin_number, \
								BSP_PORT_ENUM mosi_port_number, BSP_PIN_ENUM mosi_pin_number, \
								BSP_PORT_ENUM rc_port_number, BSP_PIN_ENUM rc_pin_number, \
								BSP_PORT_ENUM busy_port_number, BSP_PIN_ENUM busy_pin_number)
{	
	DRV_AD977AR_TYPE* p_unit = (DRV_AD977AR_TYPE*)calloc(1, sizeof(DRV_AD977AR_TYPE));	/*申请内存*/

	/*分配失败返回NULL*/
	if(p_unit == NULL)
	{
		return NULL;
	}
		
	/*id错误返回NULL*/
	if(id >= AD977AR_BUTT)
	{
		free(p_unit); 
		return NULL;
	}
	
	/*如果当前模块存在，取消分配新存储区*/
	if(_gp_Ad977arUnits[id] != NULL)
	{
		free(p_unit);
		p_unit = _gp_Ad977arUnits[id];
	}
	else
	{
		_gp_Ad977arUnits[id] = p_unit;
	}
	
	/*初始化*/
	
	/*-------------------------------常量*/
	p_unit->id         = id;				/*模块id*/
	p_unit->spi_number = spi_number;		/*spi端口编号*/
	p_unit->nss_port_number = nss_port_number;
	p_unit->nss_pin_number = nss_pin_number;
	p_unit->sck_port_number = sck_port_number;
	p_unit->sck_pin_number = sck_pin_number;
	p_unit->miso_port_number = miso_port_number;
	p_unit->miso_pin_number = miso_pin_number;
	p_unit->mosi_port_number = mosi_port_number;
	p_unit->mosi_pin_number = mosi_pin_number;
	BSP_InitSpi(spi_number, \
				nss_port_number, nss_pin_number, \
				sck_port_number, sck_pin_number, \
				miso_port_number, miso_pin_number, \
				mosi_port_number, mosi_pin_number);
	p_unit->rc_port_number = rc_port_number;
	p_unit->rc_pin_number = rc_pin_number;
	BSP_Init_Pin(rc_port_number, rc_pin_number, _OUT_PP_);		/*初始化硬件*/	
	p_unit->busy_port_number = busy_port_number;
	p_unit->busy_pin_number = busy_pin_number;
	BSP_Init_Pin(busy_port_number, busy_pin_number, _IN_FLOATING_);		/*初始化硬件*/

	/*-------------------------------变量*/
	p_unit->status     = AD977AR_READY;
	p_unit->rx_buffer  = 0;
	p_unit->rc_buffer  = 0;
	p_unit->busy_buffer  = 0;
		
	return p_unit;
}

/* AD977AR模块完成一次采样
  -----------------------------
  入口：AD977AR模块结构体指针
  返回值：true 成功，false 失败
*/
uint16_t Ad977arOneAcq(DRV_AD977AR_TYPE* p_unit)
{    
    if(_Ad977arStart(p_unit) == false)
		return 0;
    if(_Ad977arWait(p_unit) == false)
		return 0;
    p_unit->rx_buffer = _Ad977arReadData(p_unit) ;
	
    return p_unit->rx_buffer;
}

/* 内部函数
 ********************************************************/

/* AD977AR启动
  -----------------------------
  入口：AD977AR模块结构体指针
  返回值：true 成功，false 失败
*/
static bool _Ad977arStart(DRV_AD977AR_TYPE* p_unit)
{
	p_unit->rc_buffer = 0;
	BSP_WritePin(p_unit->rc_port_number, p_unit->rc_pin_number, p_unit->rc_buffer);	/*RC拉低*/
	_Ad977arDelayUs(p_unit, 1);
	p_unit->rc_buffer = 1;
	BSP_WritePin(p_unit->rc_port_number, p_unit->rc_pin_number, p_unit->rc_buffer);	/*RC拉高*/
	_Ad977arDelayUs(p_unit, 1);
	p_unit->busy_buffer = BSP_ReadPin(p_unit->busy_port_number, p_unit->busy_pin_number);
	if(p_unit->busy_buffer == 1)	/*转换未开始*/
	{
		return false;	/*返回失败*/
	}
	return true;
}

/* AD977AR等待转换
  -----------------------------
  入口：AD977AR模块结构体指针
  返回值：true 成功，false 失败
*/
static bool _Ad977arWait(DRV_AD977AR_TYPE* p_unit)
{
	_Ad977arDelayUs(p_unit, 5);	/*等待5us*/
	p_unit->busy_buffer = BSP_ReadPin(p_unit->busy_port_number, p_unit->busy_pin_number);
	if(p_unit->busy_buffer == 0)	/*转换未完成*/
	{
		_Ad977arDelayUs(p_unit, 5);	/*再等待5us*/
		p_unit->busy_buffer = BSP_ReadPin(p_unit->busy_port_number, p_unit->busy_pin_number);
		if(p_unit->busy_buffer == 0)	/*转换未完成*/
		{
			return false;	/*返回失败*/
		}
		else
		{
			return true;
		}
	}
	
	return true;
} 

/*  AD977AR模块延时n微秒
  -----------------------------
  入口：延时时间
  返回值：true 成功，false 失败
*/
static bool _Ad977arDelayUs(DRV_AD977AR_TYPE* p_unit, uint32_t time)
{    
   unsigned short i = 0; 
   
   while(time--)
   {
      i = p_unit->theta;
      while(i--);    
   }
   
   return true;
}

/*  AD977AR模块读取AD值
  -----------------------------
  入口：命令
  返回值：true 成功，false 失败
*/
static uint16_t _Ad977arReadData(DRV_AD977AR_TYPE* p_unit)
{
	uint16_t data_out = 0;
	
	data_out = BSP_SpiGetData(p_unit->spi_number);
	
	return data_out;
}
