/*
**************************************************************************************************************

*֧����Ӧ��ͬƵ���µĴ��ڲ���������.
*�����˶�printf��֧��
*�����˴��ڽ��������.
*������printf��һ���ַ���ʧ��bug

**************************************************************************************************************
*/

#include "sys.h"
#include "usart.h"
#include "act.h"
//#include "led.h"
#include "stdio.h"
#include "stdint.h"
//#include "spi.h"
//#include "stmflash.h"
#include "os_cpu.h"
//#include "eeprom_config.h"
TempUpperCtrlStruc tempUpperCtrlPara;
HS660CmdInfo PCCmdMsg;//������λ������������
HS660TemperatureRsvCmd TempRecieveCmd;//�¿ذ���յ�����
HS660TemperatureMsgReturn TempMsgReturn;//�¿ذ���Ϣ����

OS_EVENT *OSAndroid1ResponSem;

typedef enum
{
    idel = 0,
    head1 = 1,
    headConfirm = 2,
    dataRecv = 3,

}CmdStatusEnum;

/*
**************************************************************************************************************

*�������´���,֧��printf����,������Ҫѡ��use MicroLIB

**************************************************************************************************************
*/

#if 1
#pragma import(__use_no_semihosting)             
/*��׼����Ҫ��֧�ֺ���                 		    */
struct __FILE
{
	int handle;

};

FILE __stdout;
USARTInfo USARTForTemp;      //�¶ȿ����õĴ���

/*����_sys_exit()�Ա���ʹ�ð�����ģʽ    	    */
_sys_exit(int x)
{
	x = x;
}
/*�ض���fputc���� 								    */
int fputc(int ch, FILE *f)
{
//	while ((USART3->SR & 0X40) == 0);                        /*ѭ������,ֱ���������   						*/
//	USART3->DR = (u8)ch;
//	return ch;
	
	while(USART_GetFlagStatus(USARTTEMP,USART_FLAG_TC) != SET);
    USART_SendData(USARTTEMP,(unsigned char)ch);    
    while(USART_GetFlagStatus(USARTTEMP,USART_FLAG_TC) != SET);  
    return (ch);  
}

//����3��ӡ
int fputc_3(int ch, FILE *f)
{
	while ((USART3->SR & 0X04) == 0);
	USART1->DR = (u8)ch;
	return ch;
}
#endif 
/*
**************************************************************************************************************

*ʹ��microLib�ķ���

**************************************************************************************************************
*/


//int fputc(int ch, FILE *f)
//{
//	USART_SendData(USART1, (uint8_t) ch);
//
//	while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET) {}	
//   
//    return ch;
//}
//int GetKey (void)  { 
//
//    while (!(USART1->SR & USART_FLAG_RXNE));
//
//    return ((int)(USART1->DR & 0x1FF));
//}


//u8 USART_RX_BUF[64];                                    /*���ջ���,���64���ֽ�.					        */
/*
**************************************************************************************************************

*         ����״̬
*bit7��   ������ɱ�־
*bit6��   ���յ�0x0d
*bit5~0�� ���յ�����Ч�ֽ���Ŀ

**************************************************************************************************************
*/

//u8 USART_RX_STA = 0;                                     /*����״̬���									    */



/*********************************************************************************************************
 ���峣�õı���
 *********************************************************************************************************/
/*����״̬���							   	    */
//uint8_t SPIOK = 0;
uint8_t SPIOK_USART3 = 0;                        //���ڽ��յ���Ч����
uint8_t Uart_Buff[10] = //24
{ 0 };                  //��������BUFF
uint8_t Uart_Buff_USART3[24] =
{ 0 };                  //��������BUFF
void USART4_IRQHandler(void)                	      /*����1�жϷ������								   */
{
  uint8_t Rdata;
	uint8_t num;
	if (USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)
	{
		Uart_Buff[0] = USART_ReceiveData(USART3);
		if(0xaa==USART2_Buffer[0]){//��������
		 USART_SendData(USART3, 0x55);//printf("%02X",0xff);
		while (USART_GetFlagStatus(USART3, USART_FLAG_TXE) == RESET){}
//		calculate_final_AD_line(USART1);
		}
		else if(0xfe == USART2_Buffer[0])
		{
		 memset(Uart_Buff,0,sizeof(Uart_Buff));
		}
		else if(0xbb==USART2_Buffer[0]){//����������
		 ReadOneTemp(USART3);
		}else if(0xcc==USART2_Buffer[0]){//��������
		 memset(Uart_Buff,0,sizeof(Uart_Buff));
		}
		
		RQRecieve_num++;
	}
}
//USART2 recieve interruption 
//uint8_t RQnum = 200;
uint8_t USART2_Buffer[24] = { 0 };

uint8_t RQRecieve_num = 0;

//����3�����ж�
//����3���ڰ�׿�˵Ĵ��ڵ���
//void USART3_IRQHandler(void)
//{

//	uint8_t Rdata;
//	uint8_t i = 0;
//	if (USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)
//	{
//		
//		if (0x00 == USARTForTemp.HandShake)
//		{
//			i=USART_ReceiveData(USARTTEMP);
//			if (0XAA == i)//��������
//			{
//				
//				USART_SendData(USARTTEMP, 0x55);
//				while (USART_GetFlagStatus(USARTTEMP, USART_FLAG_TXE) == RESET){}
//				USARTForTemp.RepeatHandShake=1;
//				USARTForTemp.HandShake=1;
//				USARTForTemp.MessageCheck = 0;
//				USARTForTemp.RecieveDataNum=0;
//				USARTForTemp.RecieveDataSAdd=0;
//				USARTForTemp.RecieveEnd=0;
//				USARTForTemp.RecieveOK=0;
//			}
//			return;
//		}
//		else if (0x01 == USARTForTemp.RepeatHandShake)//Ӧ���Ѿ�����
//		{
//			i = USART_ReceiveData(USARTTEMP);
//			if (i == 0xAA)
//			{
//				USART_SendData(USARTTEMP, 0x55);
//				while (USART_GetFlagStatus(USARTTEMP, USART_FLAG_TXE) == RESET){}
//				USARTForTemp.RepeatHandShake=1;
//				USARTForTemp.HandShake=1;
//				USARTForTemp.MessageCheck = 0;
//				USARTForTemp.RecieveDataNum=0;
//				USARTForTemp.RecieveDataSAdd=0;
//				USARTForTemp.RecieveEnd=0;
//				USARTForTemp.RecieveOK=0;
//				return;
//			}
//			else
//			{
//				USARTForTemp.RepeatHandShake=0;
//				USARTForTemp.UartBuffer[USARTForTemp.RecieveDataNum]=i;
//				USARTForTemp.RecieveDataSAdd=USARTForTemp.RecieveDataSAdd+
//																	USARTForTemp.UartBuffer[USARTForTemp.RecieveDataNum];
//				USARTForTemp.RecieveDataNum++;
//				return;
//			}
//		}
//		if (0x00 == USARTForTemp.RecieveEnd)
//		{
//			USARTForTemp.UartBuffer[USARTForTemp.RecieveDataNum]=USART_ReceiveData(USARTTEMP);
//			USARTForTemp.RecieveDataSAdd=USARTForTemp.RecieveDataSAdd+
//																	USARTForTemp.UartBuffer[USARTForTemp.RecieveDataNum];
//			USARTForTemp.RecieveDataNum++;
//			if (USARTForTemp.RecieveDataNum == 23)
//				USARTForTemp.RecieveEnd = 0x01;
//		}
//		else
//		{
//			if (USARTForTemp.RecieveDataSAdd == USART_ReceiveData(USARTTEMP))
//			{
//				USART_SendData(USARTTEMP, 0xFE);
//				while (USART_GetFlagStatus(USARTTEMP, USART_FLAG_TXE) == RESET){}
//				USARTForTemp.RepeatHandShake=0;
//				USARTForTemp.HandShake=0;
//				USARTForTemp.RecieveDataNum=0;
//				USARTForTemp.RecieveDataSAdd=0;
//				USARTForTemp.RecieveEnd=0;
//				USARTForTemp.MessageCheck = 1;
//				USARTForTemp.RecieveOK=1;
//			}
//			else
//			{
//				USART_SendData(USARTTEMP, 0xFD);
//				while (USART_GetFlagStatus(USARTTEMP, USART_FLAG_TXE) == RESET){}
//					
//				USARTForTemp.RepeatHandShake=0;
//				USARTForTemp.HandShake=0;
//				USARTForTemp.RecieveDataNum=0;
//				USARTForTemp.RecieveDataSAdd=0;
//				USARTForTemp.RecieveEnd=0;
//				USARTForTemp.MessageCheck = 1;
//				USARTForTemp.RecieveOK=0;
//			}
//		}
//	}
//}

void USART2_IRQHandler()
{
    uint8_t Rdata;
    uint8_t i = 0;
    if (USART_GetITStatus(USARTTEMP, USART_IT_RXNE) != RESET)
    {
        Rdata = USART_ReceiveData(USARTTEMP);
        switch (PCCmdMsg.RecieveDataStatus)
        {
        case idel:
            if (Rdata == 0x55)
            {
                PCCmdMsg.RecieveDataStatus = head1;
                PCCmdMsg.DataCRCalculate = PCCmdMsg.DataCRCalculate + Rdata;
            }
            break;
        case head1:
            if (Rdata == 0X0F)
            {
                PCCmdMsg.RecieveDataStatus = headConfirm;
                PCCmdMsg.DataCRCalculate = PCCmdMsg.DataCRCalculate + Rdata;
            }
            else
            {
                PCCmdMsg.RecieveDataStatus = idel;
                PCCmdMsg.DataCRCalculate = 0;
            }
            break;
        case headConfirm:
            if (0 == PCCmdMsg.LenoffSet)
            {
                PCCmdMsg.Datalen = Rdata;
                PCCmdMsg.DataCRCalculate = PCCmdMsg.DataCRCalculate + Rdata;
                PCCmdMsg.LenoffSet++;
            }
            else if (1 == PCCmdMsg.LenoffSet)
            {
                PCCmdMsg.Datalen = (PCCmdMsg.Datalen << 8) + Rdata;
                TempRecieveCmd.RSVState = 0;//׼����������
                TempRecieveCmd.msglen = 0;
                PCCmdMsg.DataCRCalculate = PCCmdMsg.DataCRCalculate + Rdata;
                PCCmdMsg.LenoffSet=0;
				PCCmdMsg.RecieveDataStatus = dataRecv;
            }
            break;
        case dataRecv://��ʼ��������
            if ((PCCmdMsg.LenoffSet < (PCCmdMsg.Datalen )))//��������
            {
                TempRecieveCmd.data[PCCmdMsg.LenoffSet] = Rdata;
				if(PCCmdMsg.LenoffSet < PCCmdMsg.Datalen-2 )
				{
                PCCmdMsg.DataCRCalculate = PCCmdMsg.DataCRCalculate + Rdata;
				}
            }
            PCCmdMsg.LenoffSet++;
            if (PCCmdMsg.LenoffSet == PCCmdMsg.Datalen )
            {
                TempRecieveCmd.msglen = PCCmdMsg.LenoffSet;
				PCCmdMsg.RecieveCRC=TempRecieveCmd.data[PCCmdMsg.LenoffSet-2]*256+TempRecieveCmd.data[PCCmdMsg.LenoffSet-1];
//				PCCmdMsg.DataCRCalculate=PCCmdMsg.DataCRCalculate-PCCmdMsg.RecieveCRC;
                PCCmdMsg.LenoffSet = 0;
                PCCmdMsg.RecieveDataStatus = idel;
                TempRecieveCmd.RSVState = 1;


                if (PCCmdMsg.DataCRCalculate == PCCmdMsg.RecieveCRC)//����У��
                {
									TempRecieveCmd.RSVState =2;					
									OSSemPost(OSAndroid1ResponSem);
                }
                else
                {
									FuncTxtOneResponseMsg(USARTTEMP, MsgReplay, ADCmpRecieveErr, 01, NULL, 0);
                }
                memset(&PCCmdMsg, 0, sizeof(PCCmdMsg));//�������ݽṹ

            }
            break;
        }
    }
}

void  FuncTxtOneResponseMsg(USART_TypeDef* USARTx,uint8_t msgKind,uint16_t MSGType,uint8_t ExcuteState,uint8_t *data,uint16_t msgLen)
{
	uint8_t *j;
	uint16_t i;
	uint32_t CRCCalculate;
	uint8_t msgTotalLen =6;
//ȫ�Զ���ADֵ����
	USART_SendData(USARTx, 0x55);//printf("%02X",0xff);
	while (USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET){};
	USART_SendData(USARTx, 0x0f);//printf("%02X",0x1A);                   //����֡ͷ
	while (USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET){};
	CRCCalculate	=0x55+0x0f;
	USART_SendData(USARTx, 0x00);//printf("%02X",0x1A);                   //����֡ͷ
	while (USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET){};
		CRCCalculate	=CRCCalculate+0;
		USART_SendData(USARTx, msgTotalLen+msgLen);//printf("%02X",0x1A);                   //����֡ͷ
	while (USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET){};
		CRCCalculate	=CRCCalculate+msgTotalLen+msgLen;
		
	USART_SendData(USARTx, msgKind);//printf("%02X",0x1A);                   //��λ����Ϣ����
	while (USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET){};	
		CRCCalculate=CRCCalculate+msgKind;
		USART_SendData(USARTx, MSGType/256);//printf("%02X",0x1A);                   //��λ����Ϣ����
	while (USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET){};
		CRCCalculate=CRCCalculate+MSGType/256;
		USART_SendData(USARTx, MSGType%256);//printf("%02X",0x1A);                   //��λ����Ϣ����
	while (USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET){};
		CRCCalculate=CRCCalculate+MSGType%256;
	USART_SendData(USARTx, ExcuteState);//printf("%02X",0x1A);                   //ִ�����
	while (USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET){};
		CRCCalculate=CRCCalculate+ExcuteState;
		
		j = data;
		if(msgLen>0)
		{
			for(i=0;i<msgLen ;i++)
			{
			USART_SendData(USARTx, *j);//printf("%02X",0x1A);                   //ִ�����
			while (USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET){};
			CRCCalculate=CRCCalculate+*j;
			j++;
			}
		}
		
	USART_SendData(USARTx, (CRCCalculate%65536)/256);//printf("%02X",0x1A);                   //ִ�����
	while (USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET){};
		USART_SendData(USARTx, (CRCCalculate%65536)%256);//printf("%02X",0x1A);                   //ִ�����
	while (USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET){};	

}


typedef struct
{
	INT8U major;
	INT8U minjor;
	INT8U subVersion;
	INT16U revision;
	INT8U build[32];
} DevVersionType;
DevVersionType DEVICE_VERSION =
{ 1,0,2,0, __DATE__""__TIME__ };                       //�汾��__DATE__"#"__TIME__

void Func_GETVERSION(uint8_t* Uart_Buff)
{
    INT8U i = 0;
    INT8U j = 0;
    INT8U Buff[32];
    //INT32U date = 0;
    char s_month[5];
    char s_hms[32];
    int month, day, year;

    static const char month_names[36] = "JanFebMarAprMayJunJulAugSepOctNovDec";

    memset(Buff, 0, 32);
    memset(s_hms, 0, 32);

    sscanf(__DATE__, "%s %d %d", s_month, &day, &year);//���س������ʱ�����ں�ʱ��
    sscanf(__TIME__, "%s", s_hms);

    month = (strstr(month_names, s_month) - month_names) / 3;//strstr(str1,str2) ���������ж��ַ���str2�Ƿ���str1���Ӵ�������ǣ���ú������� str1�ַ����� str2��һ�γ��ֵ�λ�ÿ�ʼ�� str1��β���ַ��������򣬷���NULL��

    memcpy(Buff, s_hms, 32);
    for (i = 0; i < 8; i++)
    {
        if (!((s_hms[i] == 0x20) || (s_hms[i] == 0x3A)))//��Ϊð�Ż��߿ո�
        {
            Buff[j] = s_hms[i] - 0x30;

            j++;
        }
    }

    for (i = 0; i <= 10; i++)
    {
        Uart_Buff[i] = 0x00;
    }
    Uart_Buff[0] = DEVICE_VERSION.major;
    Uart_Buff[1] = DEVICE_VERSION.minjor;
	  Uart_Buff[2] = DEVICE_VERSION.subVersion;
    Uart_Buff[3] = DEVICE_VERSION.revision;
    Uart_Buff[4] = ((year - 2000) > 0) ? (year - 2000) : (year - 1900);
    Uart_Buff[5] = month + 1;
    Uart_Buff[6] = day & 0xff;
    Uart_Buff[7] = (Buff[0]*10 + Buff[1]);//(Buff[0] & 0x0f) << 4 | (Buff[1] & 0x0f)
    Uart_Buff[8] = (Buff[2]*10 + Buff[3]);//(Buff[2] & 0x0f) << 4 | (Buff[3] & 0x0f)
    Uart_Buff[9] = (Buff[4]*10 + Buff[5]);//(Buff[4] & 0x0f) << 4 | (Buff[5] & 0x0f)
}
//Android ������Ӧ
int parse_cmd_USART3(void)
{
	char* str;
	u16 temp;
	uint8_t  err=0;
	
	OSSemPend(OSAndroid1ResponSem,0,&err);
     if (2 == TempRecieveCmd.RSVState)//���ղ�У��ɹ�
     {
		     TempRecieveCmd.RSVState=0;
         switch (TempRecieveCmd.data[0])
         {
						 case 0://���¶�
								 ReadOneTemp(USARTTEMP);//calculate_final_AD_line(USARTAD);
								 break;
						 case 1://д����
								 temp=TempRecieveCmd.data[1]*256+TempRecieveCmd.data[2];//USARTForTemp.UartBuffer[7]*256+USARTForTemp.UartBuffer[8];
								 WriteTempCompse(USARTTEMP,temp);//ReadOnePointAD(USART1);		      
								 break;
						 case 2://������
								 ReadTempCompse(USARTTEMP);
								 break;
						 case 3://�汾�Ų�ѯ
							   FuncGetVersion(USARTTEMP);//��ȡ�汾��Ϣ 
							   break;
						 default:
							   break;
         }
     }
}
