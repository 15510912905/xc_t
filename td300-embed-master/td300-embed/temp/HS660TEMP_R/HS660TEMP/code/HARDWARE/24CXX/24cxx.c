#include "24cxx.h" 
#include "delay.h"
#include "cmd_downmachine.h"
#include "eeprom_config.h"


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

motor_compensation motor_compensationa_para1 = {
	0, 2995, 0, 0, 250, 750,
	1300, 2100, 450, 1420, 950, 1500,//850,1400,
	0, 1700, 1600, 760, 760, 0, 30, 760, 450, 450, 500, 500,
	180,
	1,

	//	 0,3520,0,394,0,831,
	//	 1347,2170,450,1420,
	//	 0,2200,2100,870,870,600,350,700,
	//	 180
};
motor_compensation motor_compensation2;


//初始化IIC接口
void AT24CXX_Init(void)
{
	u16 temp;
	if (0X55 != AT24CXX_ReadOneByte(SAVE_MOTOR_COMPENSATION_FLAG_ADDR)) //检查是不是存储过
	{
		AT24C32WriteSturctureData(SAVE_MOTOR_COMPENSATION_ADDR, &motor_compensationa_para1, sizeof(motor_compensationa_para1));
		AT24CXX_WriteOneByte(SAVE_MOTOR_COMPENSATION_FLAG_ADDR, 0x55);
		AT24C32ReadStructureData(SAVE_MOTOR_COMPENSATION_ADDR, &motor_compen, 50);
		AT24C32WriteSingleStructMember(SAVE_MOTOR_COMPENSATION_ADDR, (u16)&motor_compen.AD_gain, 66, &motor_compen);
		temp = AT24C32ReadSingleStructMember(SAVE_MOTOR_COMPENSATION_ADDR, (u16)&motor_compen.AD_gain, &motor_compen);
temp=temp;
	}
	else{
		AT24C32ReadStructureData(SAVE_MOTOR_COMPENSATION_ADDR, &motor_compen, 50);
		AT24CXX_WriteOneByte(SAVE_MOTOR_COMPENSATION_FLAG_ADDR, 0x00);
	}
	//u16 temp;
	//u8* temp1 = (u8*)&motor_compensationa_para1;
	//u16 temp2[100];
	//	IIC_Init();
	//	AT24CXX_WriteOneByte(4095,0X55);
	//	temp =AT24CXX_ReadOneByte(100);
	//	temp =AT24CXX_ReadOneByte(100);
	//	temp=temp+1;




	//	if (0x55 != AT24CXX_ReadOneByte(4095 - 2)){

	//		AT24CXX_Write(0, (u8*)&motor_compensationa_para1, 50);
	//		AT24CXX_WriteOneByte(4095 - 2, 0X55);
	//		AT24CXX_Read(0, (u8*)&motor_compensation2, 50);
	//		//		AT24CXX_WriteLenByte(2,5000,2);
	//		temp2[0]=(u16)(&motor_compensation2.H_cardin_step);
	//		temp2[1]=(u16)(&motor_compensation2.H_reset_step);
	//		temp2[2]=(u16)(&motor_compensation2.H_QR_step);
	//		temp = (u16)AT24CXX_ReadLenByte(((u16)(&motor_compensation2.H_cardin_step) - (u16)(&motor_compensation2.H_reset_step)), 2);
	//		temp = (u16)AT24CXX_ReadLenByte((u16)((&motor_compensation2.H_R2_break_step) - (u16)(&motor_compensation2.H_reset_step)), 2);
	//		temp = (u16)AT24CXX_ReadLenByte(26, 2);
	//		temp = (u16)AT24CXX_ReadLenByte(28, 2);
	//		temp = (u16)AT24CXX_ReadLenByte(30, 2);



	//	}
	//	else{
	//		AT24CXX_WriteOneByte(4095 - 2, 0Xff);
	//		AT24CXX_Read(0, (u8*)(&motor_compensation2), sizeof(motor_compensationa_para1));

	//	}



}
//在AT24CXX指定地址读出一个数据
//ReadAddr:开始读数的地址  
//返回值  :读到的数据
u8 AT24CXX_ReadOneByte(u16 ReadAddr)
{
	u8 temp = 0;
	IIC_Start();
	if (EE_TYPE > AT24C16)
	{
		IIC_Send_Byte(0XA0);	   //发送写命令
		IIC_Wait_Ack();
		IIC_Send_Byte(ReadAddr >> 8);//发送高地址
		//IIC_Wait_Ack();		 
	}
	else IIC_Send_Byte(0XA0 + ((ReadAddr / 256) << 1));   //发送器件地址0XA0,写数据 	 

	IIC_Wait_Ack();
	IIC_Send_Byte(ReadAddr % 256);   //发送低地址
	IIC_Wait_Ack();
	IIC_Start();
	IIC_Send_Byte(0XA1);           //进入接收模式			   
	IIC_Wait_Ack();
	temp = IIC_Read_Byte(0);
	IIC_Stop();//产生一个停止条件	    
	return temp;
}
//在AT24CXX指定地址写入一个数据
//WriteAddr  :写入数据的目的地址    
//DataToWrite:要写入的数据
void AT24CXX_WriteOneByte(u16 WriteAddr, u8 DataToWrite)
{
	IIC_Start();
	if (EE_TYPE > AT24C16)
	{
		IIC_Send_Byte(0XA0);	    //发送写命令
		IIC_Wait_Ack();
		IIC_Send_Byte(WriteAddr >> 8);//发送高地址
	}
	else
	{
		IIC_Send_Byte(0XA0 + ((WriteAddr / 256) << 1));   //发送器件地址0XA0,写数据 
	}
	IIC_Wait_Ack();
	IIC_Send_Byte(WriteAddr % 256);   //发送低地址
	IIC_Wait_Ack();
	IIC_Send_Byte(DataToWrite);     //发送字节							   
	IIC_Wait_Ack();
	IIC_Stop();//产生一个停止条件
	iic_delay(1000);
	//  delay_ms(100)	;
	//	OSTimeDlyHMSM(0, 0, 0, 10);
}
//在AT24CXX里面的指定地址开始写入长度为Len的数据
//该函数用于写入16bit或者32bit的数据.
//WriteAddr  :开始写入的地址  
//DataToWrite:数据数组首地址
//Len        :要写入数据的长度2,4
void AT24CXX_WriteLenByte(u16 WriteAddr, u32 DataToWrite, u8 Len)
{
	u8 t;
	for (t = 0; t < Len; t++)
	{
		AT24CXX_WriteOneByte(WriteAddr + t, (DataToWrite >> (8 * t)) & 0xff);
	}
}

//在AT24CXX里面的指定地址开始读出长度为Len的数据
//该函数用于读出16bit或者32bit的数据.
//ReadAddr   :开始读出的地址 
//返回值     :数据
//Len        :要读出数据的长度2,4
u32 AT24CXX_ReadLenByte(u16 ReadAddr, u8 Len)
{
	u8 t;
	u32 temp = 0;
	for (t = 0; t < Len; t++)
	{
		temp <<= 8;
		temp += AT24CXX_ReadOneByte(ReadAddr + Len - t - 1);
	}
	return temp;
}
//检查AT24CXX是否正常
//这里用了24XX的最后一个地址(255)来存储标志字.
//如果用其他24C系列,这个地址要修改
//返回1:检测失败
//返回0:检测成功
u8 AT24CXX_Check(void)
{
	u8 temp;
	temp = AT24CXX_ReadOneByte(255);//避免每次开机都写AT24CXX			   
	if (temp == 0X55)return 0;
	else//排除第一次初始化的情况
	{
		AT24CXX_WriteOneByte(255, 0X55);
		temp = AT24CXX_ReadOneByte(255);
		if (temp == 0X55)return 0;
	}
	return 1;
}

//在AT24CXX里面的指定地址开始读出指定个数的数据
//ReadAddr :开始读出的地址 对24c02为0~255
//pBuffer  :数据数组首地址
//NumToRead:要读出数据的个数
void AT24CXX_Read(u16 ReadAddr, u8 *pBuffer, u16 NumToRead)
{
	while (NumToRead)
	{
		*pBuffer++ = AT24CXX_ReadOneByte(ReadAddr++);
		NumToRead--;
	}
}
void AT24CXX_Read1(u16 ReadAddr, u16 *pBuffer, u16 NumToRead)
{
	while (NumToRead)
	{
		*pBuffer++ = AT24CXX_ReadLenByte(ReadAddr++, 2);
		NumToRead--;
	}
}
//在AT24CXX里面的指定地址开始写入指定个数的数据
//WriteAddr :开始写入的地址 对24c02为0~255
//pBuffer   :数据数组首地址
//NumToWrite:要写入数据的个数
void AT24CXX_Write(u16 WriteAddr, u8 *pBuffer, u16 NumToWrite)
{
	while (NumToWrite--)
	{
		AT24CXX_WriteOneByte(WriteAddr, *pBuffer);
		WriteAddr++;
		pBuffer++;
	}
}

//在AT24CXX里面的指定地址开始写入指定个数的数据
//WriteAddr :开始写入的地址 对24c02为0~255
//pBuffer   :要写入的结构体数据
//NumToWrite:要写入数据的个数
void AT24CXX_Write1(u16 WriteAddr, u16 *pBuffer, u16 NumToWrite)
{
	while (NumToWrite--)
	{
		AT24CXX_WriteLenByte(WriteAddr, *pBuffer, 2);
		WriteAddr++;
		pBuffer++;
	}
}

//存储结构体数据(整体存入)
//WriteAddr:开始写入的地址 
//Structor：要写入的结构体数据指针
//StructSize：结构体数据的大小
void AT24C32WriteSturctureData(u16 WriteAddr, motor_compensation* data, u16 StructSize)
{
	AT24CXX_Write(WriteAddr, (u8*)data, StructSize);
}

//读取结构体数据（整体读取）
//ReadAddr：读取的首地址
//Structor: 数据存放的结构体指针
//NumToRead：结构体数据的大小
void AT24C32ReadStructureData(u16 ReadAddr, motor_compensation* data, u16 NumToRead)
{
	AT24CXX_Read(0, (u8*)data, NumToRead);
}

//读取结构体中单个数据的值
//ReadAddr结构体存储的起始地址
//StructSaveAddr ：结构体存储的位置
//data ：读取的数据的返回值
u16 AT24C32ReadSingleStructMember(u16 StructSaveAddr,u16 ReadAddr, motor_compensation* data)
{
	u16 temp;
	temp = (u16)AT24CXX_ReadLenByte(StructSaveAddr + ((u16)ReadAddr - (u16)(data)), 2);
	return temp;
}
//写结构体单个数据的值

void AT24C32WriteSingleStructMember(u16 StructSaveAddr, u16 WriteAddr, u16 writedata, motor_compensation* data)
{
	AT24CXX_WriteLenByte(StructSaveAddr + ((u16)WriteAddr - (u16)(data)), (u32)writedata, 2);
}















