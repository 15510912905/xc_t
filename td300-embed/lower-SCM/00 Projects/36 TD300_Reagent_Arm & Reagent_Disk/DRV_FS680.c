#include "Drv_FS680.h"

DRV_FS680_TYPE* _gp_FS680[FS680_BUTT] = {0}; /*EM1399模块的注册表*/

static OS_EVENT* _gp_ScanSemIn = NULL;

RX_BUF FS680_RX = {0};

CMD_FS680_TYPE scan_cmd = {0x03,0x53,0x80,0xff,0x2a};			 						
CMD_FS680_TYPE scan_cmd_mode_stop = {0x03,0x45,0x80,0xff,0x38};                        




/*接口函数
***************************************************/


/*初始化EM1399模块
***************************************************/

DRV_FS680_TYPE* FS680Init(DRV_FS680_ENUM id, uint32_t baud_rate, BSP_USART_ENUM usart_number,
									BSP_PORT_ENUM tx_port_number, BSP_PIN_ENUM tx_pin_number,
									BSP_PORT_ENUM rx_port_number, BSP_PIN_ENUM rx_pin_number
)
{
	DRV_FS680_TYPE* p_unit = (DRV_FS680_TYPE*)calloc(1,sizeof(DRV_FS680_TYPE));  /*申请内存*/
	if(p_unit == NULL) return NULL;
	if(id >= FS680_BUTT)
	{
		free(p_unit);
		return NULL;
	}
	if(_gp_FS680[id] != NULL)
	{
		free(p_unit);
		p_unit = _gp_FS680[id];
	}
	else
	{
		_gp_FS680[id] = p_unit;
	}
	/*初始化*/
	p_unit->id = id;
	p_unit->Uart_port = usart_number;
	BSP_Init_UsartInterrupt(usart_number,baud_rate,tx_port_number,tx_pin_number,rx_port_number,rx_pin_number);
	p_unit->status = FS680_READY;
	_gp_ScanSemIn = OSSemCreate(0);
	return p_unit;
}
bool FS680_Write(DRV_FS680_TYPE* p_unit,CMD_FS680_TYPE* cmd_unit)
{

	BSP_UsartSendByte(p_unit->Uart_port,cmd_unit->len);
	BSP_UsartSendByte(p_unit->Uart_port,cmd_unit->cmd);
	BSP_UsartSendByte(p_unit->Uart_port,cmd_unit->status);
	BSP_UsartSendByte(p_unit->Uart_port,cmd_unit->checksum_MSB);
	BSP_UsartSendByte(p_unit->Uart_port,cmd_unit->checksum_LSB);
	return true;

}
bool FS680_Receive(DRV_FS680_TYPE* p_unit,RX_BUF* FS680_RX)
{	

	uint8_t i;
	uint16_t UART_RX_STA;
	
	uint8_t err = OS_ERR_NONE;
	OSSemPend(_gp_ScanSemIn,1000,&err);
	switch(p_unit->Uart_port)
	{
		case _USART1_:
			UART_RX_STA = USART1_RX_STA;
			FS680_RX->RX_LEN = (UART_RX_STA&0xff);
			for(i = 0;i<(UART_RX_STA&0xff);i++)
				FS680_RX->RX_Ptr[i] = USART1_RX_BUF[i];
			USART1_RX_STA = 0;
			break;
		case _USART2_:
			UART_RX_STA = USART2_RX_STA;
			FS680_RX->RX_LEN = (UART_RX_STA&0xff);
			for(i = 0;i<(UART_RX_STA&0xff);i++)
				FS680_RX->RX_Ptr[i] = USART2_RX_BUF[i];
			USART2_RX_STA = 0;
			break;
		case _USART3_:
			UART_RX_STA = USART3_RX_STA;
			FS680_RX->RX_LEN = (UART_RX_STA&0xff);
			for(i = 0;i<(UART_RX_STA&0xff);i++)
				FS680_RX->RX_Ptr[i] = USART3_RX_BUF[i];
			USART3_RX_STA = 0;
			break;
		case _UART4_:
			UART_RX_STA = USART4_RX_STA;
			FS680_RX->RX_LEN = (UART_RX_STA&0xff);
			for(i = 0;i<(UART_RX_STA&0xff);i++)
				FS680_RX->RX_Ptr[i] = USART4_RX_BUF[i];
			USART4_RX_STA = 0;
			break;
		default:
			break;
	}
	if(FS680_RX->RX_LEN == 0)
	return false;
	else
	return true;
	
	
}
void FS680_Enable(DRV_FS680_TYPE* p_unit)
{
							 
	FS680_Write(p_unit,&scan_cmd);

}
void FS680_DISABLE(DRV_FS680_TYPE* p_unit)
{

	FS680_Write(p_unit,&scan_cmd_mode_stop);

}

void BSP_Usart1_IRQHandler()
{
	uint8_t Res;
	Res = USART_ReceiveData(USART1);
	if((USART1_RX_STA&0x8000)==0)//接收未完成
	{
	if(USART1_RX_STA&0x4000)//接收到了0x0d
		{
		if(Res!=0x0a)USART1_RX_STA=0;//接收错误,重新开始
		else 
		{	
			USART1_RX_STA|=0x8000;	//接收完成了 
			OSSemPost(_gp_ScanSemIn);
		}
		}
	else //还没收到0X0D
		{	
		if(Res==0x0d)USART1_RX_STA|=0x4000;
		else
			{
			USART1_RX_BUF[USART1_RX_STA&0X3FFF]=Res ;
			USART1_RX_STA++;
			if(USART1_RX_STA>100)USART1_RX_STA=0;//接收数据错误,重新开始接收	  
			if((USART1_RX_STA == 5)&(USART1_RX_BUF[4] == 0xA4))   //(USART2_RX_STA == 7)&(USART2_RX_BUF[6] == 0x31)
				USART1_RX_STA = 0;
			}		 
		}
	} 
}
void BSP_Usart2_IRQHandler()
{
	uint8_t Res;
	Res = USART_ReceiveData(USART2);
	if((USART2_RX_STA&0x8000)==0)//接收未完成
	{
	if(USART2_RX_STA&0x4000)//接收到了0x0d
		{
		if(Res!=0x0a)USART2_RX_STA=0;//接收错误,重新开始
		else 
		{	
			USART2_RX_STA|=0x8000;	//接收完成了 
			OSSemPost(_gp_ScanSemIn);
		}
		}
	else //还没收到0X0D
		{	
		if(Res==0x0d)USART2_RX_STA|=0x4000;
		else
			{
			USART2_RX_BUF[USART2_RX_STA&0X3FFF]=Res ;
			USART2_RX_STA++;
			if(USART2_RX_STA>100)USART2_RX_STA=0;//接收数据错误,重新开始接收	  
			if((USART2_RX_STA == 7)&(USART2_RX_BUF[6] == 0x31))   //(USART2_RX_STA == 7)&(USART2_RX_BUF[6] == 0x31)
				USART2_RX_STA = 0;
			}		 
		}
	} 
}