#ifndef __24CXX_H
#define __24CXX_H
#include "myiic.h"  
#include "cmd_downmachine.h"
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK战舰STM32开发板
//24CXX驱动 代码(适合24C01~24C16,24C32~256未经过测试!有待验证!)		   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//修改日期:2012/9/9
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2009-2019
//All rights reserved									  
//////////////////////////////////////////////////////////////////////////////////

#define AT24C01		(127)
#define AT24C02		(255)
#define AT24C04		(511)
#define AT24C08		(1023)
#define AT24C16		(2047)
#define AT24C32		(4095)
#define AT24C64		(8191)
#define AT24C128	(16383)
#define AT24C256	(32767)  
//Mini STM32开发板使用的是24c02，所以定义EE_TYPE为AT24C02
#define EE_TYPE   AT24C32//AT24C02

#define EE_DATA_ADDRESS_OF_ISWRITE  (4095-2)
#define EE_DATA_ADDRESS_OF_START    (0)

#define  SAVE_MOTOR_COMPENSATION_ADDR (0) //电机参数补偿存储地址
#define  SAVE_MOTOR_COMPENSATION_FLAG_ADDR (4095-2) //电机参数补偿标准位存储的地址


extern motor_compensation motor_compensation2;

u8 AT24CXX_ReadOneByte(u16 ReadAddr);							//指定地址读取一个字节
void AT24CXX_WriteOneByte(u16 WriteAddr, u8 DataToWrite);		//指定地址写入一个字节
void AT24CXX_WriteLenByte(u16 WriteAddr, u32 DataToWrite, u8 Len);//指定地址开始写入指定长度的数据
u32 AT24CXX_ReadLenByte(u16 ReadAddr, u8 Len);					//指定地址开始读取指定长度数据
void AT24CXX_Write(u16 WriteAddr, u8 *pBuffer, u16 NumToWrite);	//从指定地址开始写入指定长度的数据
void AT24CXX_Read(u16 ReadAddr, u8 *pBuffer, u16 NumToRead);   	//从指定地址开始读出指定长度的数据

u8 AT24CXX_Check(void);  //检查器件
void AT24CXX_Init(void); //初始化IIC
void AT24CXX_Write1(u16 WriteAddr, u16 *pBuffer, u16 NumToWrite);
void AT24CXX_Read1(u16 ReadAddr, u16 *pBuffer, u16 NumToRead);

void AT24C32WriteSturctureData(u16 WriteAddr, motor_compensation* data, u16 StructSize);
void AT24C32ReadStructureData(u16 ReadAddr, motor_compensation* data, u16 NumToRead);
u16 AT24C32ReadSingleStructMember(u16 StructSaveAddr, u16 ReadAddr, motor_compensation* data);
void AT24C32WriteSingleStructMember(u16 StructSaveAddr, u16 WriteAddr, u16 writedata, motor_compensation* data);
#endif
















