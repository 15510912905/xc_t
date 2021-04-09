#ifndef __STMFLASH_H__
#define __STMFLASH_H__
#include "sys.h"  
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

//////////////////////////////////////////////////////////////////////////////////////////////////////
//�û������Լ�����Ҫ����
#define STM32_FLASH_SIZE 64 	 		//��ѡSTM32��FLASH������С(��λΪK)
#define STM32_FLASH_WREN 1              //ʹ��FLASHд��(0��������;1��ʹ��)
//////////////////////////////////////////////////////////////////////////////////////////////////////

//FLASH��ʼ��ַ
#define STM32_FLASH_BASE 0x08000000 	//STM32 FLASH����ʼ��ַ


#define H_TEMPCOMPSE_ADDR  	0X0800FFF6-2 //����������ֵ
//#define H_CARDIN_ADDR   	H_MAXSTEP_ADDR-2//��������ֵ
//#define H_QRSCAN_ADDR     H_CARDIN_ADDR-2 //��ά��ɨ��λ
//#define H_TIP_ADDR      	H_QRSCAN_ADDR-2 //TIPλ��
//#define H_SAMPLE_IN_ADDR   	H_TIP_ADDR-2 		//����λ
//#define H_R1_ADDR       	H_SAMPLE_IN_ADDR-2		// R1��λ��
//#define H_R2_ADDR       	H_R1_ADDR-2 		//R2��λ��
//#define H_SAMPLE_OUT_ADDR   	H_R2_ADDR-2 		//����λ
//#define H_SCAN_START_ADDR H_SAMPLE_OUT_ADDR-2 //scan ��ʼλ��
//#define H_SCAN_END_ADDR		H_SCAN_START_ADDR-2	//scan ����λ��  ��ʱû��

//#define V_RESET_STEP_ADDR   	H_SCAN_END_ADDR-2 //��ֱ��λ�����
//#define V_TIP_IN_ADDR   			V_RESET_STEP_ADDR-2 //tipͷ����Ĳ���
//#define V_SORB_SAMPLE_ADDR  	V_TIP_IN_ADDR-2  //��������λ��
//#define V_SORB_R1_ADDR  			V_SORB_SAMPLE_ADDR-2 //R1�������Ĳ���
//#define V_SORB_R2_ADDR   			V_SORB_R1_ADDR-2 //R2��������Ĳ���
//#define V_STOCK_SAMPLE_ADDR  	V_SORB_R2_ADDR-2  //�����Ĳ���
//#define V_SORB_AIR_ADDR      	V_STOCK_SAMPLE_ADDR-2 //������������
//#define V_TIP_OUT_ADDR      	V_SORB_AIR_ADDR-2 //��TIpͷ��λ��

////�õĲ���
//#define INJ_RESET_STEP  V_TIP_OUT_ADDR-2//�õĸ�λ
//#define INJ_PERML_STEP   INJ_RESET_STEP-2//ÿ�����Ĳ���

//#define IS_NEVER_HAS_PARM  INJ_PERML_STEP-2 //�Ƿ�д�����������ʼ������


////��ӵ�4��λλ�� 
//#define H_R1_BREAK_STEP_ADDR  IS_NEVER_HAS_PARM-2 //H����R1���Ʊ�Ĥ�ĺ�����λ��
//#define H_R2_BREAK_STEP_ADDR  H_R1_BREAK_STEP_ADDR-2 //H--R2���Ʊ�Ĥ�ĺ�����λ��
//#define V_R1_BREAK_STEP_ADDR  H_R2_BREAK_STEP_ADDR-2 //v----R1���Ʊ�Ĥ��������λ��
//#define V_R2_BREAK_STEP_ADDR  V_R1_BREAK_STEP_ADDR-2 //v---R2���Ʊ�Ĥ��������λ��
//#define V_DRAIN_R1_STEP_ADDR  V_R2_BREAK_STEP_ADDR-2 //v---R1����Һλ��
//#define V_DRAIN_R2_STEP_ADDR  V_DRAIN_R1_STEP_ADDR-2 //v---R2����Һλ��
//#define GAIN_ADDR           V_DRAIN_R2_STEP_ADDR-2 //AD�ķŴ���





//FLASH������ֵ
u16 STMFLASH_ReadHalfWord(u32 faddr);		  //��������  
void STMFLASH_WriteLenByte(u32 WriteAddr,u32 DataToWrite,u16 Len);	//ָ����ַ��ʼд��ָ�����ȵ�����
u32 STMFLASH_ReadLenByte(u32 ReadAddr,u16 Len);						//ָ����ַ��ʼ��ȡָ����������
void STMFLASH_Write(u32 WriteAddr,u16 *pBuffer,u16 NumToWrite);		//��ָ����ַ��ʼд��ָ�����ȵ�����
void STMFLASH_Read(u32 ReadAddr,u16 *pBuffer,u16 NumToRead);   		//��ָ����ַ��ʼ����ָ�����ȵ�����
extern void read_data_from_flash(u32 ReadAddr,void *ptr,u16 Num);//������
extern void write_data_to_flaash(u32 WriteAddr,void *ptr,u16 Num);//д����
//����д��
extern void Test_Write(u32 WriteAddr,u16 WriteData);	
extern void motor_compen_init(void);
#endif

















