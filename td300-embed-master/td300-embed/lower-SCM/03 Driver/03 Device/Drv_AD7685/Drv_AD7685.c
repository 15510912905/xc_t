/********************************************************
   版权所有 (C), 2001-2100, 四川新健康成生物股份有限公司
  -------------------------------------------------------

			嵌入式开发平台——AD7685模数转换模块

  -------------------------------------------------------
   文 件 名   : Drv_AD7685.c
   版 本 号   : V1.0.0.0
   作    者   : ryc
   生成日期   : 2019年10月15日
   功能描述   : 定义AD7685模数转换器的驱动。
   使用方法   ：上层模块通过调用Ad7685Init获得一个AD7685
				的指针。
   依赖于     : 
   注         ：该模块不依赖于硬件电路。 
 ********************************************************/

#include "Drv_AD7685.h"

DRV_AD7685_TYPE* _gp_Ad7685Units[AD7685_BUTT] = {0};	/*AD7685模数转换模块的注册表*/

/* 接口函数
 ********************************************************/

/* 初始化AD7685模块
  -----------------------------
  入口：AD7685模块的ID号，SPI端口编号，引脚端口号
  返回值：初始化好的AD7685模块结构体
*/
DRV_AD7685_TYPE* Drv_Ad7685Init(DRV_AD7685_ENUM id, BSP_SPI_ENUM spi_number, \
								BSP_PORT_ENUM nss_port_number, BSP_PIN_ENUM nss_pin_number, \
								BSP_PORT_ENUM sck_port_number, BSP_PIN_ENUM sck_pin_number, \
								BSP_PORT_ENUM miso_port_number, BSP_PIN_ENUM miso_pin_number, \
								BSP_PORT_ENUM mosi_port_number, BSP_PIN_ENUM mosi_pin_number)
{	
	DRV_AD7685_TYPE* p_unit = (DRV_AD7685_TYPE*)calloc(1, sizeof(DRV_AD7685_TYPE));	/*申请内存*/

	/*分配失败返回NULL*/
	if(p_unit == NULL)
	{
		return NULL;
	}
		
	/*id错误返回NULL*/
	if(id >= AD7685_BUTT)
	{
		free(p_unit); 
		return NULL;
	}
	
	/*如果当前模块存在，取消分配新存储区*/
	if(_gp_Ad7685Units[id] != NULL)
	{
		free(p_unit);
		p_unit = _gp_Ad7685Units[id];
	}
	else
	{
		_gp_Ad7685Units[id] = p_unit;
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

	/*-------------------------------变量*/
	p_unit->status     = AD7685_READY;
	p_unit->tx_buffer  = 0;
	p_unit->rx_buffer  = 0;
		
	return p_unit;
}

/*  AD7685模块完成一次采样
  -----------------------------
  入口：AD7685模块结构体指针
  返回值：true 成功，false 失败
*/
uint16_t Ad7685OneAcq(DRV_AD7685_TYPE* p_unit)
{
    uint16_t data_out = 0;
    
    _Ad7685Start(p_unit);
    _Ad7685Wait(p_unit);
    p_unit->rx_buffer = _Ad7685ReadData(p_unit) ;
	
    return p_unit->rx_buffer;
}

/* 内部函数
 ********************************************************/

/* AD7685启动
  -----------------------------
  入口：AD7685模块结构体指针
  返回值：true 成功，false 失败
*/
static bool _Ad7685Start(DRV_AD7685_TYPE* p_unit)
{
	BSP_SpiNssSet(p_unit->nss_port_number, p_unit->nss_pin_number, NSS_LOW);	/*CNV拉低*/
	_Ad7685DelayUs(p_unit, 1);
	BSP_SpiNssSet(p_unit->nss_port_number, p_unit->nss_pin_number, NSS_HIGH);	/*CNV拉高*/
	_Ad7685DelayUs(p_unit, 1);
	BSP_SpiNssSet(p_unit->nss_port_number, p_unit->nss_pin_number, NSS_LOW);	/*CNV拉低*/  
	return true;
}

/* AD7685等待转换
  -----------------------------
  入口：AD7685模块结构体指针
  返回值：true 成功，false 失败
*/
static bool _Ad7685Wait(DRV_AD7685_TYPE* p_unit)
{
	_Ad7685DelayUs(p_unit, 20);	/*等待20us*/
	return true;
} 

/*  AD7685模块延时n微秒
  -----------------------------
  入口：延时时间
  返回值：true 成功，false 失败
*/
static bool _Ad7685DelayUs(DRV_AD7685_TYPE* p_unit, uint32_t time)
{    
   unsigned short i = 0; 
   
   while(time--)
   {
      i = p_unit->theta;
      while(i--);    
   }
    
   return true;
}

/*  AD7685模块读取AD值
  -----------------------------
  入口：命令
  返回值：true 成功，false 失败
*/
static uint16_t _Ad7685ReadData(DRV_AD7685_TYPE* p_unit)
{
	uint16_t data_out = 0;
	
	data_out = BSP_SpiGetData(p_unit->spi_number);
	
	return data_out;
}
