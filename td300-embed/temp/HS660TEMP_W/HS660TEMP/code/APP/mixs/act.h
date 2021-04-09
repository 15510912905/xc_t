#ifndef __ACT_H__
#define __ACT_H__

#include "includes.h"
typedef enum
{
MsgReplay =01,
}ADUpMsgKind;

typedef enum
{
    TempRead = 0x1000,
    TempComposeWriteOver = 0x1001,
    TempComposeReadOver = 0x1002,
    ADCmpRecieveErr = 0xeeee,//数据传输错误
}ADUpMsgType;


void CleanSem(void);
extern void ReadOneTemp(USART_TypeDef* USARTx);
extern void ReadTempCompse(USART_TypeDef* USARTx);
extern void WriteTempCompse(USART_TypeDef* USARTx,u16 data);
extern void TempComposeReadAndUse(void);

extern void FuncGetVersion(USART_TypeDef * USARTx);//获取版本信息

#endif
