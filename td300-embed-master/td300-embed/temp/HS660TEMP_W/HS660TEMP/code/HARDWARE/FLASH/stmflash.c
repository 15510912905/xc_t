#include "stmflash.h"
#include "delay.h"


 
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEKս��STM32������
//STM32 FLASH ��������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2012/9/13
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2009-2019
//All rights reserved									  
//////////////////////////////////////////////////////////////////////////////////
 
//��ȡָ����ַ�İ���(16λ����)
//faddr:����ַ(�˵�ַ����Ϊ2�ı���!!)
//����ֵ:��Ӧ����.
u16 STMFLASH_ReadHalfWord(u32 faddr)
{
	return *(vu16*)faddr; 
}
#if STM32_FLASH_WREN	//���ʹ����д   
//������д��
//WriteAddr:��ʼ��ַ
//pBuffer:����ָ��
//NumToWrite:����(16λ)��   
void STMFLASH_Write_NoCheck(u32 WriteAddr,u16 *pBuffer,u16 NumToWrite)   
{ 			 		 
	u16 i;
	for(i=0;i<NumToWrite;i++)
	{
		FLASH_ProgramHalfWord(WriteAddr,pBuffer[i]);
	    WriteAddr+=2;//��ַ����2.
	}  
} 
//��ָ����ַ��ʼд��ָ�����ȵ�����
//WriteAddr:��ʼ��ַ(�˵�ַ����Ϊ2�ı���!!)
//pBuffer:����ָ��
//NumToWrite:����(16λ)��(����Ҫд���16λ���ݵĸ���.)
#if STM32_FLASH_SIZE<256
#define STM_SECTOR_SIZE 1024 //�ֽ�
#else 
#define STM_SECTOR_SIZE	2048
#endif		 
u16 STMFLASH_BUF[STM_SECTOR_SIZE/2];//�����2K�ֽ�
void STMFLASH_Write(u32 WriteAddr,u16 *pBuffer,u16 NumToWrite)	
{
	u32 secpos;	   //������ַ
	u16 secoff;	   //������ƫ�Ƶ�ַ(16λ�ּ���)
	u16 secremain; //������ʣ���ַ(16λ�ּ���)	   
 	u16 i;    
	u32 offaddr;   //ȥ��0X08000000��ĵ�ַ
	if(WriteAddr<STM32_FLASH_BASE||(WriteAddr>=(STM32_FLASH_BASE+1024*STM32_FLASH_SIZE)))return;//�Ƿ���ַ
	FLASH_Unlock();						//����
	offaddr=WriteAddr-STM32_FLASH_BASE;		//ʵ��ƫ�Ƶ�ַ.
	secpos=offaddr/STM_SECTOR_SIZE;			//������ַ  0~127 for STM32F103RBT6
	secoff=(offaddr%STM_SECTOR_SIZE)/2;		//�������ڵ�ƫ��(2���ֽ�Ϊ������λ.)
	secremain=STM_SECTOR_SIZE/2-secoff;		//����ʣ��ռ��С   
	if(NumToWrite<=secremain)secremain=NumToWrite;//�����ڸ�������Χ
	while(1) 
	{	
		STMFLASH_Read(secpos*STM_SECTOR_SIZE+STM32_FLASH_BASE,STMFLASH_BUF,STM_SECTOR_SIZE/2);//������������������
		for(i=0;i<secremain;i++)//У������
		{
			if(STMFLASH_BUF[secoff+i]!=0XFFFF)break;//��Ҫ����  	  
		}
		if(i<secremain)//��Ҫ����
		{
			FLASH_ErasePage(secpos*STM_SECTOR_SIZE+STM32_FLASH_BASE);//�����������
			for(i=0;i<secremain;i++)//����
			{
				STMFLASH_BUF[i+secoff]=pBuffer[i];	  
			}
			STMFLASH_Write_NoCheck(secpos*STM_SECTOR_SIZE+STM32_FLASH_BASE,STMFLASH_BUF,STM_SECTOR_SIZE/2);//д����������  
		}else STMFLASH_Write_NoCheck(WriteAddr,pBuffer,secremain);//д�Ѿ������˵�,ֱ��д������ʣ������. 				   
		if(NumToWrite==secremain)break;//д�������
		else//д��δ����
		{
			secpos++;				//������ַ��1
			secoff=0;				//ƫ��λ��Ϊ0 	 
		   	pBuffer+=secremain;  	//ָ��ƫ��
			WriteAddr+=secremain;	//д��ַƫ��	   
		   	NumToWrite-=secremain;	//�ֽ�(16λ)���ݼ�
			if(NumToWrite>(STM_SECTOR_SIZE/2))secremain=STM_SECTOR_SIZE/2;//��һ����������д����
			else secremain=NumToWrite;//��һ����������д����
		}	 
	};	
	FLASH_Lock();//����
}
#endif

//��ָ����ַ��ʼ����ָ�����ȵ�����
//ReadAddr:��ʼ��ַ
//pBuffer:����ָ��
//NumToWrite:����(16λ)��
void STMFLASH_Read(u32 ReadAddr,u16 *pBuffer,u16 NumToRead)   	
{
	u16 i;
	for(i=0;i<NumToRead;i++)
	{
		pBuffer[i]=STMFLASH_ReadHalfWord(ReadAddr);//��ȡ2���ֽ�.
		ReadAddr+=2;//ƫ��2���ֽ�.	
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
//WriteAddr:��ʼ��ַ
//WriteData:Ҫд�������
void Test_Write(u32 WriteAddr,u16 WriteData)   	
{
	STMFLASH_Write(WriteAddr,&WriteData,1);//д��һ���� 
}


//��flash�ж�ȡ����
 void read_data_from_flash(u32 ReadAddr,void *ptr,u16 Num)
 {
   STMFLASH_Read(ReadAddr,ptr,Num);
 }
//write data to flash 
 
 void write_data_to_flaash(u32 WriteAddr,void *ptr,u16 Num)
 {
    STMFLASH_Write(WriteAddr,ptr,Num);
 }

 
 
// //����������ֵ
// void write_H_max_resetStep(u16 step){
// STMFLASH_Write(H_MAXSTEP_ADDR,&step,1);
// }
// u16 read_H_max_resetStep(void){
// return STMFLASH_ReadHalfWord(H_MAXSTEP_ADDR);
// }
// 
// //����Ĳ���
// void write_H_card_in_Step(u16 step){
// STMFLASH_Write(H_CARDIN_ADDR,&step,1);
// }
// u16 read_H_card_in_Step(void){
// return STMFLASH_ReadHalfWord(H_CARDIN_ADDR);
// }
// 
// //TIp�Ĳ���
// void write_h_tip_step(u16 step){
// STMFLASH_Write(H_TIP_ADDR,&step,1);
// }
// u16 read_h_tip_step(void){
// return STMFLASH_ReadHalfWord(H_TIP_ADDR);
// }
// //r1�Ĳ���
// void write_h_r1_step(u16 step){
// STMFLASH_Write(H_R1_ADDR,&step,1);
// }
// u16 read_H_r1_step(void){
// return STMFLASH_ReadHalfWord(H_R1_ADDR);
// }
// 
// //r2�Ĳ���
// void write_H_r2_step(u16 step){
// STMFLASH_Write(H_R2_ADDR,&step,1);
// }
// u16 read_H_r2_step(void){
// return STMFLASH_ReadHalfWord(H_R2_ADDR);
// }
// 
// //H ����R1�Ĳ��� 
//  void write_H_r1_break_step(u16 step){
// STMFLASH_Write(H_R1_BREAK_STEP_ADDR,&step,1);
// }
// u16 read_H_r1_break_step(void){
// return STMFLASH_ReadHalfWord(H_R1_BREAK_STEP_ADDR);
// }
// 
// //H ����R2�Ĳ���
// 
//  void write_H_r2_break_step(u16 step){
// STMFLASH_Write(H_R2_BREAK_STEP_ADDR,&step,1);
// }
// u16 read_H_r2_break_step(void){
// return STMFLASH_ReadHalfWord(H_R2_BREAK_STEP_ADDR);
// }
// 
// 
// //sample�Ĳ���
// void write_H_sample_step(u16 step){
//// STMFLASH_Write(H_SAMPLE_ADDR,&step,1);
// }
// u16 read_H_sample_step(void){
//// return STMFLASH_ReadHalfWord(H_SAMPLE_ADDR);
// }
// 
//  //ɨ�����ʼλ��
// void write_H_scan_start_step(u16 step){
// STMFLASH_Write(H_SCAN_START_ADDR,&step,1);
// }
// u16 read_H_scan_start_step(void){
// return STMFLASH_ReadHalfWord(H_SCAN_START_ADDR);
// }
// 
//  //ɨ�����ֹλ��
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
//   if(0xAA != STMFLASH_ReadHalfWord(IS_NEVER_HAS_PARM)){//��һ��д����
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
//		 //��R1Һ���λ��
//		  motor_compen.V_drain_R1_step=motor_compensationa_para.V_drain_R1_step;
//		  temp=motor_compensationa_para.V_drain_R1_step;
//		 STMFLASH_Write(V_DRAIN_R1_STEP_ADDR,&temp,1);
//		 
//		  //��R2Һ���λ��
//		  motor_compen.V_drain_R2_step=motor_compensationa_para.V_drain_R2_step;
//		  temp=motor_compensationa_para.V_drain_R2_step;
//		 STMFLASH_Write(V_DRAIN_R2_STEP_ADDR,&temp,1);
//		 
//		 //�Ŵ���
//		 motor_compen.AD_gain=motor_compensationa_para.AD_gain;
//		 temp=motor_compensationa_para.AD_gain;
//		 STMFLASH_Write(GAIN_ADDR,&temp,1);
//		  setAMP((u16)temp);
//			setAMP((u16)temp);
//		
//			temp=0xaa;
//		  STMFLASH_Write(IS_NEVER_HAS_PARM,&temp,1);//���Ϊ��д����
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
 













