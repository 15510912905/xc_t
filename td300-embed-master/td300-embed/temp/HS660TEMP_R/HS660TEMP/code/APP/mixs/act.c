#include "act.h"
#include "usart.h"
#include "TempCollecte.h"	
#include "tempcontrol.h"
#include "stmflash.h"
#include "pid.h"



////扫描窗长度 


void ReadOneTemp(USART_TypeDef* USARTx){
	uint16_t temp;
	uint8_t  m;
	uint8_t data[2]={0};
	temp=(curtemp-tempUpperCtrlPara.TempcCompensate)*100;
	data[0]=temp/256;
	data[1]=temp%256;
	FuncTxtOneResponseMsg(USARTx, MsgReplay, TempRead, 0,(uint8_t*)&data, 2);
	
}

//读温控补偿
void TempComposeReadAndUse(void)
{
	uint16_t temp;
	int16_t tempcompose;
//    temp=STMFLASH_ReadHalfWord(H_TEMPCOMPSE_ADDR);
//	tempcompose=(int16_t)temp;
	tempUpperCtrlPara.TempcCompensate = 0;//tempcompose/100.0;
}

//写入温度的补偿值
void WriteTempCompse(USART_TypeDef* USARTx,u16 data){
	uint16_t temp;
	int16_t temp1;
	uint8_t data1[2]={0};
	STMFLASH_Write(H_TEMPCOMPSE_ADDR,&data,1);
	temp=STMFLASH_ReadHalfWord(H_TEMPCOMPSE_ADDR);
	temp1=(int16_t)temp;
	tempUpperCtrlPara.TempcCompensate=((temp1)/100.0)*1.00;
	data1[0]=(temp)/256;
	data1[1]=(temp)%256;
	FuncTxtOneResponseMsg(USARTx, MsgReplay, TempComposeWriteOver, 0,(uint8_t*)&data1, 2);
//	init_diff();
	reac_tunning.pid.setpoint= g_TempOfMach+tempUpperCtrlPara.TempcCompensate;
		
}
//读取温度的补偿值
void ReadTempCompse(USART_TypeDef* USARTx){
	uint16_t temp;
	int16_t temp1;
	uint8_t data1[2]={0};
	uint8_t Buff[24]={0};
	uint8_t  m;

	memset(Buff,0,sizeof(Buff));
	Buff[0]=1;
	Buff[1]=0;
	Buff[2]=0;
	Buff[3]=2;
	Buff[4]=0;

	temp=STMFLASH_ReadHalfWord(H_TEMPCOMPSE_ADDR);
	temp1=(int16_t)temp;
	data1[0]=(temp)/256;
	data1[1]=(temp)%256;
	tempUpperCtrlPara.TempcCompensate=(temp1)/100.0;
	FuncTxtOneResponseMsg(USARTx, MsgReplay, TempComposeWriteOver, 0,(uint8_t*)&data1, 2);
}

void FuncGetVersion(USART_TypeDef * USARTx)//获取版本信息
{
    uint8_t i;
    uint8_t buffer0[10] = {0};
		uint16_t crc_date = 0;
	
    Func_GETVERSION(buffer0);
//		for(i= 0;i<10;i++)
//		{
//			USART_SendData(USARTx, buffer0[i]);
//			while (USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET){}
//		}
		
		  USART_SendData(USARTx, 0x55);// printf("%02X",Buffer[m]); 
		  while (USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET){}   
		  USART_SendData(USARTx, 0x0f);// printf("%02X",Buffer[m]); 
		  while (USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET){}
			
		  USART_SendData(USARTx, 0x00);// printf("%02X",Buffer[m]); 		//数据长度
		  while (USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET){}			  
		  USART_SendData(USARTx, 0x0C);// printf("%02X",Buffer[m]); 
		  while (USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET){}	

			for(i= 0;i<10;i++)//数据
			{
				crc_date = crc_date + buffer0[i];
				USART_SendData(USARTx, buffer0[i]);
				while (USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET){}
			}

		  USART_SendData(USARTx, ((0x55+0x0f+0x00+0x0C+crc_date)>>8)&0xff);// printf("%02X",Buffer[m]);         //crc_h 
		  while (USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET){}
		  USART_SendData(USARTx, (0x55+0x0f+0x00+0x0C+crc_date)&0xff);// printf("%02X",Buffer[m]);         //crc_l 
		  while (USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET){}	
//      memset(buffer0, 0, sizeof(buffer0));//清理数据结构				
}