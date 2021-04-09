#ifndef __STMFLASH_H__
#define __STMFLASH_H__
#include "sys.h"  
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK战舰STM32开发板
//STM32 FLASH 驱动代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//修改日期:2012/9/13
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2009-2019
//All rights reserved									  
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////////////////
//用户根据自己的需要设置
#define STM32_FLASH_SIZE 64 	 		//所选STM32的FLASH容量大小(单位为K)
#define STM32_FLASH_WREN 1              //使能FLASH写入(0，不是能;1，使能)
//////////////////////////////////////////////////////////////////////////////////////////////////////

//FLASH起始地址
#define STM32_FLASH_BASE 0x08000000 	//STM32 FLASH的起始地址


#define H_TEMPCOMPSE_ADDR  	0X0800FFF6-2 //横坐标的最大值
//#define H_CARDIN_ADDR   	H_MAXSTEP_ADDR-2//卡入的最大值
//#define H_QRSCAN_ADDR     H_CARDIN_ADDR-2 //二维码扫描位
//#define H_TIP_ADDR      	H_QRSCAN_ADDR-2 //TIP位置
//#define H_SAMPLE_IN_ADDR   	H_TIP_ADDR-2 		//排样位
//#define H_R1_ADDR       	H_SAMPLE_IN_ADDR-2		// R1的位置
//#define H_R2_ADDR       	H_R1_ADDR-2 		//R2的位置
//#define H_SAMPLE_OUT_ADDR   	H_R2_ADDR-2 		//排样位
//#define H_SCAN_START_ADDR H_SAMPLE_OUT_ADDR-2 //scan 起始位置
//#define H_SCAN_END_ADDR		H_SCAN_START_ADDR-2	//scan 结束位置  暂时没用

//#define V_RESET_STEP_ADDR   	H_SCAN_END_ADDR-2 //垂直复位最大步数
//#define V_TIP_IN_ADDR   			V_RESET_STEP_ADDR-2 //tip头插入的步数
//#define V_SORB_SAMPLE_ADDR  	V_TIP_IN_ADDR-2  //吸样本的位置
//#define V_SORB_R1_ADDR  			V_SORB_SAMPLE_ADDR-2 //R1吸样本的步数
//#define V_SORB_R2_ADDR   			V_SORB_R1_ADDR-2 //R2样本吸入的步数
//#define V_STOCK_SAMPLE_ADDR  	V_SORB_R2_ADDR-2  //排样的步数
//#define V_SORB_AIR_ADDR      	V_STOCK_SAMPLE_ADDR-2 //吸空气的坐标
//#define V_TIP_OUT_ADDR      	V_SORB_AIR_ADDR-2 //退TIp头的位置

////泵的补偿
//#define INJ_RESET_STEP  V_TIP_OUT_ADDR-2//泵的复位
//#define INJ_PERML_STEP   INJ_RESET_STEP-2//每毫升的步距

//#define IS_NEVER_HAS_PARM  INJ_PERML_STEP-2 //是否写入过参数，初始化参数


////添加的4个位位置 
//#define H_R1_BREAK_STEP_ADDR  IS_NEVER_HAS_PARM-2 //H——R1戳破薄膜的横坐标位置
//#define H_R2_BREAK_STEP_ADDR  H_R1_BREAK_STEP_ADDR-2 //H--R2戳破薄膜的横坐标位置
//#define V_R1_BREAK_STEP_ADDR  H_R2_BREAK_STEP_ADDR-2 //v----R1戳破薄膜的纵坐标位置
//#define V_R2_BREAK_STEP_ADDR  V_R1_BREAK_STEP_ADDR-2 //v---R2戳破薄膜的纵坐标位置
//#define V_DRAIN_R1_STEP_ADDR  V_R2_BREAK_STEP_ADDR-2 //v---R1的排液位置
//#define V_DRAIN_R2_STEP_ADDR  V_DRAIN_R1_STEP_ADDR-2 //v---R2的排液位置
//#define GAIN_ADDR           V_DRAIN_R2_STEP_ADDR-2 //AD的放大倍数





//FLASH解锁键值
u16 STMFLASH_ReadHalfWord(u32 faddr);		  //读出半字  
void STMFLASH_WriteLenByte(u32 WriteAddr,u32 DataToWrite,u16 Len);	//指定地址开始写入指定长度的数据
u32 STMFLASH_ReadLenByte(u32 ReadAddr,u16 Len);						//指定地址开始读取指定长度数据
void STMFLASH_Write(u32 WriteAddr,u16 *pBuffer,u16 NumToWrite);		//从指定地址开始写入指定长度的数据
void STMFLASH_Read(u32 ReadAddr,u16 *pBuffer,u16 NumToRead);   		//从指定地址开始读出指定长度的数据
extern void read_data_from_flash(u32 ReadAddr,void *ptr,u16 Num);//读数据
extern void write_data_to_flaash(u32 WriteAddr,void *ptr,u16 Num);//写数据
//测试写入
extern void Test_Write(u32 WriteAddr,u16 WriteData);	
extern void motor_compen_init(void);
#endif

















