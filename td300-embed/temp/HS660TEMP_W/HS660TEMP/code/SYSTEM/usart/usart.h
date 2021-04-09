/*
**************************************************************************************************************	 
 
*֧����Ӧ��ͬƵ���µĴ��ڲ���������.
*�����˶�printf��֧��
*�����˴��ڽ��������.
*������printf��һ���ַ���ʧ��bug

**************************************************************************************************************	  
*/

#ifndef __USART_H
#define __USART_H
#include "stdio.h"
#include "stdint.h"
#include "ucos_ii.h"
#include "stm32f10x.h"
#define USARTTEMP  USART2

typedef struct {
uint8_t HandShake;//��������
uint8_t RepeatHandShake;//��������
uint8_t RecieveEnd;//���ս���
uint8_t MessageCheck;//��ϢУ��
uint16_t RecieveDataNum;//���յ����ݸ���
uint8_t RecieveDataSAdd;//У���
uint8_t RecieveOK;
uint8_t UartBuffer[24];//�������ݶ���
}USARTInfo;

typedef struct
{
     uint8_t  RecieveDataStatus ; //��������״̬��
     uint16_t RecieveCRC;//uint8_t RecieveCRC ;    //���յ���crc
     uint16_t DataCRCalculate ;    //ͨ�����յ������ݼ������crcֵ
     uint8_t  LenoffSet; //֡����ƫ��
     uint16_t Datalen ;    //֡����(����ID��CRC������)
     uint8_t SendId ;//��Ϣ����ID
     uint8_t RecieveId ;//Ŀ��ID
     uint32_t Cmdlistindex;    //�����������
     uint16_t CmdType ;//��Ϣ����
     uint16_t CmdFormate ;//��Ϣ��ʽ
}HS660CmdInfo;

//�¿ؽṹ��
typedef struct
{
 float TempcCompensate;     //�¿صĲ���
 int InitiativeUploadFlag;//�Ƿ������ϴ�
 int UpLoadInTerval;      //�ϴ���ʱ���� S
}TempUpperCtrlStruc;

//AD������սṹ��
typedef struct
{
	uint16_t msglen;//��Ϣ�ܳ���
	uint8_t SendId;//��Ϣ��Դ
	uint8_t CmdType;
	uint8_t RSVState;//��Ϣ����״̬
	uint8_t  data[20];//��Ϣ���ն���

}HS660TemperatureRsvCmd;

typedef struct
{
	//��Ϣͷ550f
	uint16_t MSGlen;//��Ϣ֡����
	uint8_t  ID;//��Ϣ������ID��
	uint16_t crc;//У��
	uint8_t MsgKind;//��Ϣ���� ������Ϣ������
	uint32_t MsgIndex;//��Ϣ֡��
	uint8_t MsgType;//��Ϣ���� ������Ϣ������
	uint8_t ExcuteResult;
	uint8_t* data;
}HS660TemperatureMsgReturn;


//ȫ�Զ��¿���ȫ�ֱ���
extern HS660CmdInfo PCCmdMsg;//������λ������������
extern HS660TemperatureRsvCmd TempRecieveCmd;//�¿ذ���յ�����
extern HS660TemperatureMsgReturn TempMsgReturn;//�¿ذ���Ϣ����
extern TempUpperCtrlStruc tempUpperCtrlPara;


extern OS_EVENT *OSAndroid1ResponSem;



extern USARTInfo USARTForTemp;      //�¶ȿ����õĴ���
//extern uint8_t USART_RX_BUF[64];                                /*���ջ���,���63���ֽ�.ĩ�ֽ�Ϊ���з� 	    */
//extern uint8_t USART_RX_STA; 
extern uint8_t USART2_Buffer[24] ;
extern uint8_t RQRecieve_num ;

/*********************************************************************************************************
 ���峣�õı���
 *********************************************************************************************************/
/*����״̬���							   	                                 */
//extern uint8_t SPIOK ;                        //���ڽ��յ���Ч����
extern uint8_t Uart_Buff[10] ;                  //��������BUFF
extern uint8_t Uart_Buff_USART3[24];                  //����3����BUFF

extern void uart_init(uint32_t bound);

//extern int parse_cmd(void);//UART4 �������

extern int parse_cmd_USART3(void);//USART3 �������

extern void  FuncTxtOneResponseMsg(USART_TypeDef* USARTx,uint8_t msgKind,uint16_t MSGType,uint8_t ExcuteState,uint8_t *data,uint16_t msgLen);
extern void Func_GETVERSION(uint8_t* Uart_Buff);//��ȡ�汾��

#endif
