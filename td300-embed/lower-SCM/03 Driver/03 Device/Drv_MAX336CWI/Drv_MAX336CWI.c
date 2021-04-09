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

#include "Drv_MAX336CWI.h"

DRV_MAX336CWI_TYPE* _gp_Max336cwiUnits[MAX336CWI_BUTT] = {0};	/*MAX336CWI多路选择器模块的注册表*/

/* 接口函数
 ********************************************************/

/* 初始化MAX336CWI模块
  -----------------------------
  入口：MAX336CWI模块的ID号，引脚端口
  返回值：初始化好的MAX336CWI模块结构体
*/
DRV_MAX336CWI_TYPE* Max336cwiInit(DRV_MAX336CWI_ENUM id, \
								BSP_PORT_ENUM a0_port_number, BSP_PIN_ENUM a0_pin_number, \
								BSP_PORT_ENUM a1_port_number, BSP_PIN_ENUM a1_pin_number, \
								BSP_PORT_ENUM a2_port_number, BSP_PIN_ENUM a2_pin_number, \
								BSP_PORT_ENUM a3_port_number, BSP_PIN_ENUM a3_pin_number)
{	
	DRV_MAX336CWI_TYPE* p_unit = (DRV_MAX336CWI_TYPE*)calloc(1, sizeof(DRV_MAX336CWI_TYPE));	/*申请内存*/

	/*分配失败返回NULL*/
	if(p_unit == NULL)
	{
		return NULL;
	}
		
	/*id错误返回NULL*/
	if(id >= MAX336CWI_BUTT)
	{
		free(p_unit); 
		return NULL;
	}
	
	/*如果当前模块存在，取消分配新存储区*/
	if(_gp_Max336cwiUnits[id] != NULL)
	{
		free(p_unit);
		p_unit = _gp_Max336cwiUnits[id];
	}
	else
	{
		_gp_Max336cwiUnits[id] = p_unit;
	}
	
	/*初始化*/
	
	/*-------------------------------常量*/
	p_unit->id             = id;				/*模块id*/
	p_unit->a0_port_number = a0_port_number;	/*引脚端口号*/
	p_unit->a0_pin_number  = a0_pin_number;		/*引脚序号*/
	BSP_Init_Pin(a0_port_number, a0_pin_number, _OUT_PP_);	/*初始化硬件*/
	p_unit->a1_port_number = a1_port_number;	/*引脚端口号*/
	p_unit->a1_pin_number  = a1_pin_number;		/*引脚序号*/
	BSP_Init_Pin(a1_port_number, a1_pin_number, _OUT_PP_);		/*初始化硬件*/
	p_unit->a2_port_number = a2_port_number;	/*引脚端口号*/
	p_unit->a2_pin_number  = a2_pin_number;		/*引脚序号*/
	BSP_Init_Pin(a2_port_number, a2_pin_number, _OUT_PP_);		/*初始化硬件*/
	p_unit->a3_port_number = a3_port_number;	/*引脚端口号*/
	p_unit->a3_pin_number  = a3_pin_number;		/*引脚序号*/
	BSP_Init_Pin(a3_port_number, a3_pin_number, _OUT_PP_);		/*初始化硬件*/
	
	/*-------------------------------变量*/
	p_unit->status = MAX336CWI_READY;
	p_unit->a0_pin_buffer = 0;
	p_unit->a1_pin_buffer = 0;
	p_unit->a2_pin_buffer = 0;
	p_unit->a3_pin_buffer = 0;
	p_unit->choose = 0;
	
	Max336cwiChoose(p_unit, 1);	/*初始化选择通道1*/
	
	return p_unit;
}

/* MAX336CWI模块选择通道
  -----------------------------
  入口：MAX336CWI模块的ID号，通道号
  返回值：true 成功，flase 失败
*/
bool Max336cwiChoose(DRV_MAX336CWI_TYPE* p_unit, uint8_t choose)
{
	if((choose > 16) || (choose < 1))
		return false;
	
	p_unit->choose = choose;
	switch(choose)
	{
		case 1:
			p_unit->a0_pin_buffer = 0;
			p_unit->a1_pin_buffer = 0;
			p_unit->a2_pin_buffer = 0;
			p_unit->a3_pin_buffer = 0;
			break;
		case 2:
			p_unit->a0_pin_buffer = 1;
			p_unit->a1_pin_buffer = 0;
			p_unit->a2_pin_buffer = 0;
			p_unit->a3_pin_buffer = 0;
			break;
		case 3:
			p_unit->a0_pin_buffer = 0;
			p_unit->a1_pin_buffer = 1;
			p_unit->a2_pin_buffer = 0;
			p_unit->a3_pin_buffer = 0;
			break;
		case 4:
			p_unit->a0_pin_buffer = 1;
			p_unit->a1_pin_buffer = 1;
			p_unit->a2_pin_buffer = 0;
			p_unit->a3_pin_buffer = 0;
			break;
		case 5:
			p_unit->a0_pin_buffer = 0;
			p_unit->a1_pin_buffer = 0;
			p_unit->a2_pin_buffer = 1;
			p_unit->a3_pin_buffer = 0;
			break;
		case 6:
			p_unit->a0_pin_buffer = 1;
			p_unit->a1_pin_buffer = 0;
			p_unit->a2_pin_buffer = 1;
			p_unit->a3_pin_buffer = 0;
			break;
		case 7:
			p_unit->a0_pin_buffer = 0;
			p_unit->a1_pin_buffer = 1;
			p_unit->a2_pin_buffer = 1;
			p_unit->a3_pin_buffer = 0;
			break;
		case 8:
			p_unit->a0_pin_buffer = 1;
			p_unit->a1_pin_buffer = 1;
			p_unit->a2_pin_buffer = 1;
			p_unit->a3_pin_buffer = 0;
			break;
		case 9:
			p_unit->a0_pin_buffer = 0;
			p_unit->a1_pin_buffer = 0;
			p_unit->a2_pin_buffer = 0;
			p_unit->a3_pin_buffer = 1;
			break;
		case 10:
			p_unit->a0_pin_buffer = 1;
			p_unit->a1_pin_buffer = 0;
			p_unit->a2_pin_buffer = 0;
			p_unit->a3_pin_buffer = 1;
			break;
		case 11:
			p_unit->a0_pin_buffer = 0;
			p_unit->a1_pin_buffer = 1;
			p_unit->a2_pin_buffer = 0;
			p_unit->a3_pin_buffer = 1;
			break;
		case 12:
			p_unit->a0_pin_buffer = 1;
			p_unit->a1_pin_buffer = 1;
			p_unit->a2_pin_buffer = 0;
			p_unit->a3_pin_buffer = 1;
			break;
		case 13:
			p_unit->a0_pin_buffer = 0;
			p_unit->a1_pin_buffer = 0;
			p_unit->a2_pin_buffer = 1;
			p_unit->a3_pin_buffer = 1;
			break;
		case 14:
			p_unit->a0_pin_buffer = 1;
			p_unit->a1_pin_buffer = 0;
			p_unit->a2_pin_buffer = 1;
			p_unit->a3_pin_buffer = 1;
			break;
		case 15:
			p_unit->a0_pin_buffer = 0;
			p_unit->a1_pin_buffer = 1;
			p_unit->a2_pin_buffer = 1;
			p_unit->a3_pin_buffer = 1;
			break;
		case 16:
			p_unit->a0_pin_buffer = 1;
			p_unit->a1_pin_buffer = 1;
			p_unit->a2_pin_buffer = 1;
			p_unit->a3_pin_buffer = 1;
			break;
	}
	
	BSP_WritePin(p_unit->a0_port_number, p_unit->a0_pin_number, p_unit->a0_pin_buffer);
	BSP_WritePin(p_unit->a1_port_number, p_unit->a1_pin_number, p_unit->a1_pin_buffer);
	BSP_WritePin(p_unit->a2_port_number, p_unit->a2_pin_number, p_unit->a2_pin_buffer);
	BSP_WritePin(p_unit->a3_port_number, p_unit->a3_pin_number, p_unit->a3_pin_buffer);
	
	return true;
}
