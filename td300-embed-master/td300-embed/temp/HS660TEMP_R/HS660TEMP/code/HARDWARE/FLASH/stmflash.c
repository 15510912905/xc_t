#include "stmflash.h"
#include "delay.h"


 
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
 
//读取指定地址的半字(16位数据)
//faddr:读地址(此地址必须为2的倍数!!)
//返回值:对应数据.
u16 STMFLASH_ReadHalfWord(u32 faddr)
{
	return *(vu16*)faddr; 
}
#if STM32_FLASH_WREN	//如果使能了写   
//不检查的写入
//WriteAddr:起始地址
//pBuffer:数据指针
//NumToWrite:半字(16位)数   
void STMFLASH_Write_NoCheck(u32 WriteAddr,u16 *pBuffer,u16 NumToWrite)   
{ 			 		 
	u16 i;
	for(i=0;i<NumToWrite;i++)
	{
		FLASH_ProgramHalfWord(WriteAddr,pBuffer[i]);
	    WriteAddr+=2;//地址增加2.
	}  
} 
//从指定地址开始写入指定长度的数据
//WriteAddr:起始地址(此地址必须为2的倍数!!)
//pBuffer:数据指针
//NumToWrite:半字(16位)数(就是要写入的16位数据的个数.)
#if STM32_FLASH_SIZE<256
#define STM_SECTOR_SIZE 1024 //字节
#else 
#define STM_SECTOR_SIZE	2048
#endif		 
u16 STMFLASH_BUF[STM_SECTOR_SIZE/2];//最多是2K字节
void STMFLASH_Write(u32 WriteAddr,u16 *pBuffer,u16 NumToWrite)	
{
	u32 secpos;	   //扇区地址
	u16 secoff;	   //扇区内偏移地址(16位字计算)
	u16 secremain; //扇区内剩余地址(16位字计算)	   
 	u16 i;    
	u32 offaddr;   //去掉0X08000000后的地址
	if(WriteAddr<STM32_FLASH_BASE||(WriteAddr>=(STM32_FLASH_BASE+1024*STM32_FLASH_SIZE)))return;//非法地址
	FLASH_Unlock();						//解锁
	offaddr=WriteAddr-STM32_FLASH_BASE;		//实际偏移地址.
	secpos=offaddr/STM_SECTOR_SIZE;			//扇区地址  0~127 for STM32F103RBT6
	secoff=(offaddr%STM_SECTOR_SIZE)/2;		//在扇区内的偏移(2个字节为基本单位.)
	secremain=STM_SECTOR_SIZE/2-secoff;		//扇区剩余空间大小   
	if(NumToWrite<=secremain)secremain=NumToWrite;//不大于该扇区范围
	while(1) 
	{	
		STMFLASH_Read(secpos*STM_SECTOR_SIZE+STM32_FLASH_BASE,STMFLASH_BUF,STM_SECTOR_SIZE/2);//读出整个扇区的内容
		for(i=0;i<secremain;i++)//校验数据
		{
			if(STMFLASH_BUF[secoff+i]!=0XFFFF)break;//需要擦除  	  
		}
		if(i<secremain)//需要擦除
		{
			FLASH_ErasePage(secpos*STM_SECTOR_SIZE+STM32_FLASH_BASE);//擦除这个扇区
			for(i=0;i<secremain;i++)//复制
			{
				STMFLASH_BUF[i+secoff]=pBuffer[i];	  
			}
			STMFLASH_Write_NoCheck(secpos*STM_SECTOR_SIZE+STM32_FLASH_BASE,STMFLASH_BUF,STM_SECTOR_SIZE/2);//写入整个扇区  
		}else STMFLASH_Write_NoCheck(WriteAddr,pBuffer,secremain);//写已经擦除了的,直接写入扇区剩余区间. 				   
		if(NumToWrite==secremain)break;//写入结束了
		else//写入未结束
		{
			secpos++;				//扇区地址增1
			secoff=0;				//偏移位置为0 	 
		   	pBuffer+=secremain;  	//指针偏移
			WriteAddr+=secremain;	//写地址偏移	   
		   	NumToWrite-=secremain;	//字节(16位)数递减
			if(NumToWrite>(STM_SECTOR_SIZE/2))secremain=STM_SECTOR_SIZE/2;//下一个扇区还是写不完
			else secremain=NumToWrite;//下一个扇区可以写完了
		}	 
	};	
	FLASH_Lock();//上锁
}
#endif

//从指定地址开始读出指定长度的数据
//ReadAddr:起始地址
//pBuffer:数据指针
//NumToWrite:半字(16位)数
void STMFLASH_Read(u32 ReadAddr,u16 *pBuffer,u16 NumToRead)   	
{
	u16 i;
	for(i=0;i<NumToRead;i++)
	{
		pBuffer[i]=STMFLASH_ReadHalfWord(ReadAddr);//读取2个字节.
		ReadAddr+=2;//偏移2个字节.	
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
//WriteAddr:起始地址
//WriteData:要写入的数据
void Test_Write(u32 WriteAddr,u16 WriteData)   	
{
	STMFLASH_Write(WriteAddr,&WriteData,1);//写入一个字 
}


//从flash中读取数据
 void read_data_from_flash(u32 ReadAddr,void *ptr,u16 Num)
 {
   STMFLASH_Read(ReadAddr,ptr,Num);
 }
//write data to flash 
 
 void write_data_to_flaash(u32 WriteAddr,void *ptr,u16 Num)
 {
    STMFLASH_Write(WriteAddr,ptr,Num);
 }

 
 
// //横坐标的最大值
// void write_H_max_resetStep(u16 step){
// STMFLASH_Write(H_MAXSTEP_ADDR,&step,1);
// }
// u16 read_H_max_resetStep(void){
// return STMFLASH_ReadHalfWord(H_MAXSTEP_ADDR);
// }
// 
// //卡入的步距
// void write_H_card_in_Step(u16 step){
// STMFLASH_Write(H_CARDIN_ADDR,&step,1);
// }
// u16 read_H_card_in_Step(void){
// return STMFLASH_ReadHalfWord(H_CARDIN_ADDR);
// }
// 
// //TIp的步距
// void write_h_tip_step(u16 step){
// STMFLASH_Write(H_TIP_ADDR,&step,1);
// }
// u16 read_h_tip_step(void){
// return STMFLASH_ReadHalfWord(H_TIP_ADDR);
// }
// //r1的步距
// void write_h_r1_step(u16 step){
// STMFLASH_Write(H_R1_ADDR,&step,1);
// }
// u16 read_H_r1_step(void){
// return STMFLASH_ReadHalfWord(H_R1_ADDR);
// }
// 
// //r2的步距
// void write_H_r2_step(u16 step){
// STMFLASH_Write(H_R2_ADDR,&step,1);
// }
// u16 read_H_r2_step(void){
// return STMFLASH_ReadHalfWord(H_R2_ADDR);
// }
// 
// //H 戳破R1的步距 
//  void write_H_r1_break_step(u16 step){
// STMFLASH_Write(H_R1_BREAK_STEP_ADDR,&step,1);
// }
// u16 read_H_r1_break_step(void){
// return STMFLASH_ReadHalfWord(H_R1_BREAK_STEP_ADDR);
// }
// 
// //H 戳破R2的步距
// 
//  void write_H_r2_break_step(u16 step){
// STMFLASH_Write(H_R2_BREAK_STEP_ADDR,&step,1);
// }
// u16 read_H_r2_break_step(void){
// return STMFLASH_ReadHalfWord(H_R2_BREAK_STEP_ADDR);
// }
// 
// 
// //sample的步距
// void write_H_sample_step(u16 step){
//// STMFLASH_Write(H_SAMPLE_ADDR,&step,1);
// }
// u16 read_H_sample_step(void){
//// return STMFLASH_ReadHalfWord(H_SAMPLE_ADDR);
// }
// 
//  //扫描的起始位置
// void write_H_scan_start_step(u16 step){
// STMFLASH_Write(H_SCAN_START_ADDR,&step,1);
// }
// u16 read_H_scan_start_step(void){
// return STMFLASH_ReadHalfWord(H_SCAN_START_ADDR);
// }
// 
//  //扫描的终止位置
// void write_H_scan_end_step(u16 step){
// STMFLASH_Write(H_SCAN_END_ADDR,&step,1);
// }
// u16 read_H_scan_end_step(void){
// return STMFLASH_ReadHalfWord(H_SCAN_END_ADDR);
// }
// 
 
 
 
// motor_compensation motor_compensationa_para={
//	 0,2995,0,0,250,750,
//	 1300,2100,450,1420,950,1500,//850,1400,
//	 0,1700,1600,760,760,0,30,760,450,450,500,500,
//	 180,
//	 1,
//	 
////	 0,3520,0,394,0,831,
////	 1347,2170,450,1420,
////	 0,2200,2100,870,870,600,350,700,
////	 180
// };
// void motor_compen_init()
// {
//	 u16 temp;
//   if(0xAA != STMFLASH_ReadHalfWord(IS_NEVER_HAS_PARM)){//第一次写数据
//	  motor_compen.H_cardin_step			=			motor_compensationa_para.H_cardin_step;
//		 temp=motor_compensationa_para.H_cardin_step;
//		 STMFLASH_Write(H_CARDIN_ADDR,&temp,1);
//		motor_compen.H_QR_step					=			motor_compensationa_para.H_QR_step;
//		  temp=motor_compensationa_para.H_QR_step;
//		  STMFLASH_Write(H_QRSCAN_ADDR,&temp,1);
//		motor_compen.H_R1_step					=			motor_compensationa_para.H_R1_step;
//		  temp=motor_compensationa_para.H_R1_step;
//		  STMFLASH_Write(H_R1_ADDR,&temp,1);
//		 motor_compen.H_R2_step					=			motor_compensationa_para.H_R2_step;
//		 temp=motor_compensationa_para.H_R2_step;
//		  STMFLASH_Write(H_R2_ADDR,&temp,1);
//		 motor_compen.H_reset_step			=			motor_compensationa_para.H_reset_step;
//		 temp=motor_compensationa_para.H_reset_step;
//		  STMFLASH_Write(H_MAXSTEP_ADDR,&temp,1);
//		 
//		 motor_compen.H_sample_in_step	=			motor_compensationa_para.H_sample_in_step;
//		 temp=motor_compensationa_para.H_sample_in_step;
//		  STMFLASH_Write(H_SAMPLE_IN_ADDR,&temp,1);
//		 
//		 motor_compen.H_sample_out_step	=			motor_compensationa_para.H_sample_out_step;
//		  temp=motor_compensationa_para.H_sample_out_step;
//		  STMFLASH_Write(H_SAMPLE_OUT_ADDR,&temp,1);
//		 
//		 motor_compen.H_scan_start_step =			motor_compensationa_para.H_scan_start_step;
//		   temp=motor_compensationa_para.H_scan_start_step;
//		  STMFLASH_Write(H_SCAN_START_ADDR,&temp,1);
//		 
//		 motor_compen.H_scan_end_step		=			motor_compensationa_para.H_scan_end_step;
//		  temp=motor_compensationa_para.H_scan_end_step;
//		  STMFLASH_Write(H_SCAN_END_ADDR,&temp,1);
//			
//		 motor_compen.H_tip_step				=			motor_compensationa_para.H_tip_step;
//		 temp=motor_compensationa_para.H_tip_step;
//		  STMFLASH_Write(H_TIP_ADDR,&temp,1);
//			
//			//H break R1
//			motor_compen.H_R1_break_step				=			motor_compensationa_para.H_R1_break_step;
//		 temp=motor_compensationa_para.H_R1_break_step;
//		  STMFLASH_Write(H_R1_BREAK_STEP_ADDR,&temp,1);
//			
//			// h break r2
//			motor_compen.H_R2_break_step				=			motor_compensationa_para.H_R2_break_step;
//		 temp=motor_compensationa_para.H_R2_break_step;
//		  STMFLASH_Write(H_R2_BREAK_STEP_ADDR,&temp,1);
//			
//		 
//		 motor_compen.INJ_perML_step 		=			motor_compensationa_para.INJ_perML_step;
//		 temp=motor_compensationa_para.INJ_perML_step;
//		  STMFLASH_Write(INJ_PERML_STEP,&temp,1);
//		 
//		 
//		 motor_compen.V_reset_setp=motor_compensationa_para.V_reset_setp;
//		 temp=motor_compensationa_para.V_reset_setp;
//		 STMFLASH_Write(V_RESET_STEP_ADDR,&temp,1);
//			
//		 motor_compen.V_sample_in_step=motor_compensationa_para.V_sample_in_step;
//		 temp=motor_compensationa_para.V_sample_in_step;
//		 STMFLASH_Write(V_SORB_SAMPLE_ADDR,&temp,1);
//		 
//		 motor_compen.V_sample_out=motor_compensationa_para.V_sample_out;
//		  temp=motor_compensationa_para.V_sample_out;
//		 STMFLASH_Write(V_STOCK_SAMPLE_ADDR,&temp,1);
//		 
//		 motor_compen.V_sorb_air_step=motor_compensationa_para.V_sorb_air_step;
//		 temp=motor_compensationa_para.V_sorb_air_step;
//		 STMFLASH_Write(V_SORB_AIR_ADDR,&temp,1);
//		 
//		 motor_compen.V_sorb_R1_step=motor_compensationa_para.V_sorb_R1_step;
//		 temp=motor_compensationa_para.V_sorb_R1_step;
//		 STMFLASH_Write(V_SORB_R1_ADDR,&temp,1);
//		 
//		 motor_compen.V_sorb_R2_step=motor_compensationa_para.V_sorb_R2_step;
//		 temp=motor_compensationa_para.V_sorb_R2_step;
//		 STMFLASH_Write(V_SORB_R2_ADDR,&temp,1);
//		 
//		 motor_compen.V_tip_in_step=motor_compensationa_para.V_tip_in_step;
//		  temp=motor_compensationa_para.V_tip_in_step;
//		 STMFLASH_Write(V_TIP_IN_ADDR,&temp,1);
//		 
//		 motor_compen.V_tip_out_step=motor_compensationa_para.V_tip_out_step;
//		 temp=motor_compensationa_para.V_tip_out_step;
//		 STMFLASH_Write(V_TIP_OUT_ADDR,&temp,1);
//		 
//		 //V break R1 
//		 motor_compen.V_R1_break_step=motor_compensationa_para.V_R1_break_step;
//		  temp=motor_compensationa_para.V_R1_break_step;
//		 STMFLASH_Write(V_R1_BREAK_STEP_ADDR,&temp,1);
//		 
//		 //v break R2
//		 motor_compen.V_R2_break_step=motor_compensationa_para.V_R2_break_step;
//		  temp=motor_compensationa_para.V_R2_break_step;
//		 STMFLASH_Write(V_R2_BREAK_STEP_ADDR,&temp,1);
//		 
//		 //排R1液体的位置
//		  motor_compen.V_drain_R1_step=motor_compensationa_para.V_drain_R1_step;
//		  temp=motor_compensationa_para.V_drain_R1_step;
//		 STMFLASH_Write(V_DRAIN_R1_STEP_ADDR,&temp,1);
//		 
//		  //排R2液体的位置
//		  motor_compen.V_drain_R2_step=motor_compensationa_para.V_drain_R2_step;
//		  temp=motor_compensationa_para.V_drain_R2_step;
//		 STMFLASH_Write(V_DRAIN_R2_STEP_ADDR,&temp,1);
//		 
//		 //放大倍数
//		 motor_compen.AD_gain=motor_compensationa_para.AD_gain;
//		 temp=motor_compensationa_para.AD_gain;
//		 STMFLASH_Write(GAIN_ADDR,&temp,1);
//		  setAMP((u16)temp);
//			setAMP((u16)temp);
//		
//			temp=0xaa;
//		  STMFLASH_Write(IS_NEVER_HAS_PARM,&temp,1);//标记为已写参数
//	 }else{
//		 motor_compen.H_reset_step=STMFLASH_ReadHalfWord(H_MAXSTEP_ADDR);
//	   motor_compen.H_cardin_step=STMFLASH_ReadHalfWord(H_CARDIN_ADDR);
//		 motor_compen.H_QR_step=STMFLASH_ReadHalfWord(H_QRSCAN_ADDR);
//		 motor_compen.H_tip_step=STMFLASH_ReadHalfWord(H_TIP_ADDR);
//		 motor_compen.H_sample_in_step=STMFLASH_ReadHalfWord(H_SAMPLE_IN_ADDR);
//		 
//		 motor_compen.H_R1_step=STMFLASH_ReadHalfWord(H_R1_ADDR);
//		 motor_compen.H_R2_step=STMFLASH_ReadHalfWord(H_R2_ADDR);
//		 motor_compen.H_sample_out_step=STMFLASH_ReadHalfWord(H_SAMPLE_OUT_ADDR);
//		 motor_compen.H_scan_start_step=STMFLASH_ReadHalfWord(H_SCAN_START_ADDR);
//		 motor_compen.H_scan_end_step=STMFLASH_ReadHalfWord(H_SCAN_END_ADDR);
//		 motor_compen.H_R1_break_step=STMFLASH_ReadHalfWord(H_R1_BREAK_STEP_ADDR);
//		 motor_compen.H_R2_break_step=STMFLASH_ReadHalfWord(H_R2_BREAK_STEP_ADDR);
//		 
//		 
//		 
//		 motor_compen.V_reset_setp=STMFLASH_ReadHalfWord(V_RESET_STEP_ADDR);
//		 motor_compen.V_tip_in_step=STMFLASH_ReadHalfWord(V_TIP_IN_ADDR);
//		 motor_compen.V_sample_in_step=STMFLASH_ReadHalfWord(V_SORB_SAMPLE_ADDR);
//		 motor_compen.V_sorb_R1_step=STMFLASH_ReadHalfWord(V_SORB_R1_ADDR);
//		 motor_compen.V_sorb_R2_step=STMFLASH_ReadHalfWord(V_SORB_R2_ADDR);
//		 motor_compen.V_sample_out=STMFLASH_ReadHalfWord(V_STOCK_SAMPLE_ADDR);
//		 motor_compen.V_sorb_air_step=STMFLASH_ReadHalfWord(V_SORB_AIR_ADDR);
//		 motor_compen.V_tip_out_step=STMFLASH_ReadHalfWord(V_TIP_OUT_ADDR);
//		 motor_compen.INJ_perML_step=STMFLASH_ReadHalfWord(INJ_PERML_STEP);
//		 
//		 motor_compen.V_R1_break_step=STMFLASH_ReadHalfWord(V_R1_BREAK_STEP_ADDR);
//		 motor_compen.V_R2_break_step=STMFLASH_ReadHalfWord(V_R2_BREAK_STEP_ADDR);
//		 
//		 motor_compen.V_drain_R1_step=STMFLASH_ReadHalfWord(V_DRAIN_R1_STEP_ADDR);
//		 motor_compen.V_drain_R2_step=STMFLASH_ReadHalfWord(V_DRAIN_R2_STEP_ADDR);
//		 motor_compen.AD_gain=STMFLASH_ReadHalfWord(GAIN_ADDR);
//		  
////		 setAMP((u16)motor_compen.AD_gain);
//		 temp=motor_compen.AD_gain;
//		 setAMP(temp);
//		 setAMP(temp);
//		 
		 
//	 }
// }
// 
 













