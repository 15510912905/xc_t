#include "24cxx.h" 
#include "delay.h"
#include "cmd_downmachine.h"
#include "eeprom_config.h"


//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEKս��STM32������
//24CXX���� ����(�ʺ�24C01~24C16,24C32~256δ��������!�д���֤!)		   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2012/9/9
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2009-2019
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


//��ʼ��IIC�ӿ�
void AT24CXX_Init(void)
{
	u16 temp;
	if (0X55 != AT24CXX_ReadOneByte(SAVE_MOTOR_COMPENSATION_FLAG_ADDR)) //����ǲ��Ǵ洢��
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
//��AT24CXXָ����ַ����һ������
//ReadAddr:��ʼ�����ĵ�ַ  
//����ֵ  :����������
u8 AT24CXX_ReadOneByte(u16 ReadAddr)
{
	u8 temp = 0;
	IIC_Start();
	if (EE_TYPE > AT24C16)
	{
		IIC_Send_Byte(0XA0);	   //����д����
		IIC_Wait_Ack();
		IIC_Send_Byte(ReadAddr >> 8);//���͸ߵ�ַ
		//IIC_Wait_Ack();		 
	}
	else IIC_Send_Byte(0XA0 + ((ReadAddr / 256) << 1));   //����������ַ0XA0,д���� 	 

	IIC_Wait_Ack();
	IIC_Send_Byte(ReadAddr % 256);   //���͵͵�ַ
	IIC_Wait_Ack();
	IIC_Start();
	IIC_Send_Byte(0XA1);           //�������ģʽ			   
	IIC_Wait_Ack();
	temp = IIC_Read_Byte(0);
	IIC_Stop();//����һ��ֹͣ����	    
	return temp;
}
//��AT24CXXָ����ַд��һ������
//WriteAddr  :д�����ݵ�Ŀ�ĵ�ַ    
//DataToWrite:Ҫд�������
void AT24CXX_WriteOneByte(u16 WriteAddr, u8 DataToWrite)
{
	IIC_Start();
	if (EE_TYPE > AT24C16)
	{
		IIC_Send_Byte(0XA0);	    //����д����
		IIC_Wait_Ack();
		IIC_Send_Byte(WriteAddr >> 8);//���͸ߵ�ַ
	}
	else
	{
		IIC_Send_Byte(0XA0 + ((WriteAddr / 256) << 1));   //����������ַ0XA0,д���� 
	}
	IIC_Wait_Ack();
	IIC_Send_Byte(WriteAddr % 256);   //���͵͵�ַ
	IIC_Wait_Ack();
	IIC_Send_Byte(DataToWrite);     //�����ֽ�							   
	IIC_Wait_Ack();
	IIC_Stop();//����һ��ֹͣ����
	iic_delay(1000);
	//  delay_ms(100)	;
	//	OSTimeDlyHMSM(0, 0, 0, 10);
}
//��AT24CXX�����ָ����ַ��ʼд�볤��ΪLen������
//�ú�������д��16bit����32bit������.
//WriteAddr  :��ʼд��ĵ�ַ  
//DataToWrite:���������׵�ַ
//Len        :Ҫд�����ݵĳ���2,4
void AT24CXX_WriteLenByte(u16 WriteAddr, u32 DataToWrite, u8 Len)
{
	u8 t;
	for (t = 0; t < Len; t++)
	{
		AT24CXX_WriteOneByte(WriteAddr + t, (DataToWrite >> (8 * t)) & 0xff);
	}
}

//��AT24CXX�����ָ����ַ��ʼ��������ΪLen������
//�ú������ڶ���16bit����32bit������.
//ReadAddr   :��ʼ�����ĵ�ַ 
//����ֵ     :����
//Len        :Ҫ�������ݵĳ���2,4
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
//���AT24CXX�Ƿ�����
//��������24XX�����һ����ַ(255)���洢��־��.
//���������24Cϵ��,�����ַҪ�޸�
//����1:���ʧ��
//����0:���ɹ�
u8 AT24CXX_Check(void)
{
	u8 temp;
	temp = AT24CXX_ReadOneByte(255);//����ÿ�ο�����дAT24CXX			   
	if (temp == 0X55)return 0;
	else//�ų���һ�γ�ʼ�������
	{
		AT24CXX_WriteOneByte(255, 0X55);
		temp = AT24CXX_ReadOneByte(255);
		if (temp == 0X55)return 0;
	}
	return 1;
}

//��AT24CXX�����ָ����ַ��ʼ����ָ������������
//ReadAddr :��ʼ�����ĵ�ַ ��24c02Ϊ0~255
//pBuffer  :���������׵�ַ
//NumToRead:Ҫ�������ݵĸ���
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
//��AT24CXX�����ָ����ַ��ʼд��ָ������������
//WriteAddr :��ʼд��ĵ�ַ ��24c02Ϊ0~255
//pBuffer   :���������׵�ַ
//NumToWrite:Ҫд�����ݵĸ���
void AT24CXX_Write(u16 WriteAddr, u8 *pBuffer, u16 NumToWrite)
{
	while (NumToWrite--)
	{
		AT24CXX_WriteOneByte(WriteAddr, *pBuffer);
		WriteAddr++;
		pBuffer++;
	}
}

//��AT24CXX�����ָ����ַ��ʼд��ָ������������
//WriteAddr :��ʼд��ĵ�ַ ��24c02Ϊ0~255
//pBuffer   :Ҫд��Ľṹ������
//NumToWrite:Ҫд�����ݵĸ���
void AT24CXX_Write1(u16 WriteAddr, u16 *pBuffer, u16 NumToWrite)
{
	while (NumToWrite--)
	{
		AT24CXX_WriteLenByte(WriteAddr, *pBuffer, 2);
		WriteAddr++;
		pBuffer++;
	}
}

//�洢�ṹ������(�������)
//WriteAddr:��ʼд��ĵ�ַ 
//Structor��Ҫд��Ľṹ������ָ��
//StructSize���ṹ�����ݵĴ�С
void AT24C32WriteSturctureData(u16 WriteAddr, motor_compensation* data, u16 StructSize)
{
	AT24CXX_Write(WriteAddr, (u8*)data, StructSize);
}

//��ȡ�ṹ�����ݣ������ȡ��
//ReadAddr����ȡ���׵�ַ
//Structor: ���ݴ�ŵĽṹ��ָ��
//NumToRead���ṹ�����ݵĴ�С
void AT24C32ReadStructureData(u16 ReadAddr, motor_compensation* data, u16 NumToRead)
{
	AT24CXX_Read(0, (u8*)data, NumToRead);
}

//��ȡ�ṹ���е������ݵ�ֵ
//ReadAddr�ṹ��洢����ʼ��ַ
//StructSaveAddr ���ṹ��洢��λ��
//data ����ȡ�����ݵķ���ֵ
u16 AT24C32ReadSingleStructMember(u16 StructSaveAddr,u16 ReadAddr, motor_compensation* data)
{
	u16 temp;
	temp = (u16)AT24CXX_ReadLenByte(StructSaveAddr + ((u16)ReadAddr - (u16)(data)), 2);
	return temp;
}
//д�ṹ�嵥�����ݵ�ֵ

void AT24C32WriteSingleStructMember(u16 StructSaveAddr, u16 WriteAddr, u16 writedata, motor_compensation* data)
{
	AT24CXX_WriteLenByte(StructSaveAddr + ((u16)WriteAddr - (u16)(data)), (u32)writedata, 2);
}















