/*
**************************************************************************************************************	 
 
*支持适应不同频率下的串口波特率设置.
*加入了对printf的支持
*增加了串口接收命令功能.
*修正了printf第一个字符丢失的bug

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
uint8_t HandShake;//命令握手
uint8_t RepeatHandShake;//反复握手
uint8_t RecieveEnd;//接收结束
uint8_t MessageCheck;//消息校验
uint16_t RecieveDataNum;//接收的数据个数
uint8_t RecieveDataSAdd;//校验和
uint8_t RecieveOK;
uint8_t UartBuffer[24];//接收数据队列
}USARTInfo;

typedef struct
{
     uint8_t  RecieveDataStatus ; //接收数据状态机
     uint16_t RecieveCRC;//uint8_t RecieveCRC ;    //接收到的crc
     uint16_t DataCRCalculate ;    //通过接收到的数据计算出的crc值
     uint8_t  LenoffSet; //帧长度偏移
     uint16_t Datalen ;    //帧长度(包括ID和CRC及报文)
     uint8_t SendId ;//消息发起ID
     uint8_t RecieveId ;//目标ID
     uint32_t Cmdlistindex;    //命令队列索引
     uint16_t CmdType ;//消息类型
     uint16_t CmdFormate ;//消息格式
}HS660CmdInfo;

//温控结构体
typedef struct
{
 float TempcCompensate;     //温控的补偿
 int InitiativeUploadFlag;//是否主动上传
 int UpLoadInTerval;      //上传的时间间隔 S
}TempUpperCtrlStruc;

//AD命令接收结构体
typedef struct
{
	uint16_t msglen;//消息总长度
	uint8_t SendId;//消息来源
	uint8_t CmdType;
	uint8_t RSVState;//消息接收状态
	uint8_t  data[20];//消息接收队列

}HS660TemperatureRsvCmd;

typedef struct
{
	//消息头550f
	uint16_t MSGlen;//消息帧长度
	uint8_t  ID;//消息发来的ID号
	uint16_t crc;//校验
	uint8_t MsgKind;//消息类型 接收消息反馈？
	uint32_t MsgIndex;//消息帧号
	uint8_t MsgType;//消息类型 接收消息反馈？
	uint8_t ExcuteResult;
	uint8_t* data;
}HS660TemperatureMsgReturn;


//全自动温控用全局变量
extern HS660CmdInfo PCCmdMsg;//来自上位机发来的命令
extern HS660TemperatureRsvCmd TempRecieveCmd;//温控板接收的命令
extern HS660TemperatureMsgReturn TempMsgReturn;//温控板消息返回
extern TempUpperCtrlStruc tempUpperCtrlPara;


extern OS_EVENT *OSAndroid1ResponSem;



extern USARTInfo USARTForTemp;      //温度控制用的串口
//extern uint8_t USART_RX_BUF[64];                                /*接收缓冲,最大63个字节.末字节为换行符 	    */
//extern uint8_t USART_RX_STA; 
extern uint8_t USART2_Buffer[24] ;
extern uint8_t RQRecieve_num ;

/*********************************************************************************************************
 定义常用的变量
 *********************************************************************************************************/
/*接收状态标记							   	                                 */
//extern uint8_t SPIOK ;                        //串口接收到有效数据
extern uint8_t Uart_Buff[10] ;                  //串口数据BUFF
extern uint8_t Uart_Buff_USART3[24];                  //串口3数据BUFF

extern void uart_init(uint32_t bound);

//extern int parse_cmd(void);//UART4 命令解析

extern int parse_cmd_USART3(void);//USART3 命令解析

extern void  FuncTxtOneResponseMsg(USART_TypeDef* USARTx,uint8_t msgKind,uint16_t MSGType,uint8_t ExcuteState,uint8_t *data,uint16_t msgLen);
extern void Func_GETVERSION(uint8_t* Uart_Buff);//获取版本号

#endif
