#include "Drv_EM1399.h"

DRV_EM1399_TYPE* _gp_EM1399[EM1399_BUTT] = {0}; /*EM1399模块的注册表*/

static OS_EVENT* _gp_ScanSemIn = NULL;


RX_BUF EM1399_RX = {0};

CMD_EM1399_TYPE scan_cmd_mode = {0x7e,0x00,0x08,0x01,0x00,0x00,0xf5,0xcb,0x23};		 		 //0xf5,0xcb,0x23		0xf9,0x0a,0xaf			//MX 0x1b,0x41,0x30,0x0d
CMD_EM1399_TYPE scan_cmd = {0x7e,0x00,0x08,0x01,0x00,0x02,0x01,0x02,0xda};			//EM1399
CMD_EM1399_TYPE scan_cmd_mode_stop = {0x7e,0x00,0x08,0x01,0x00,0x02,0x02,0x32,0xb9}; 	//EM1399				//MX 0x1b,0x41,0x31,0x0d    	
CMD_EM1399_TYPE tail_newline = {0x7e,0x00,0x08,0x01,0x00,0x31,0x30,0x74,0x6e};
/*接口函数
***************************************************/


/*初始化EM1399模块
***************************************************/

DRV_EM1399_TYPE* EM1399Init(DRV_EM1399_ENUM id, uint32_t baud_rate, BSP_USART_ENUM usart_number,
									BSP_PORT_ENUM tx_port_number, BSP_PIN_ENUM tx_pin_number,
									BSP_PORT_ENUM rx_port_number, BSP_PIN_ENUM rx_pin_number
)
{
	DRV_EM1399_TYPE* p_unit = (DRV_EM1399_TYPE*)calloc(1,sizeof(DRV_EM1399_TYPE));  /*申请内存*/
	if(p_unit == NULL) return NULL;
	if(id >= EM1399_BUTT)
	{
		free(p_unit);
		return NULL;
	}
	if(_gp_EM1399[id] != NULL)
	{
		free(p_unit);
		p_unit = _gp_EM1399[id];
	}
	else
	{
		_gp_EM1399[id] = p_unit;
	}
	/*初始化*/
	p_unit->id = id;
	p_unit->Uart_port = usart_number;
	BSP_Init_UsartInterrupt(usart_number,baud_rate,tx_port_number,tx_pin_number,rx_port_number,rx_pin_number);
	p_unit->status = EM1399_READY;
	p_unit->read_buffer = 0;
	p_unit->write_buffer = 0;
	EM1399_Write(p_unit,&scan_cmd_mode);
//	EM1399_Write(p_unit,&tail_newline);
	_gp_ScanSemIn = OSSemCreate(0);
	return p_unit;
}
bool EM1399_Write(DRV_EM1399_TYPE* p_unit,CMD_EM1399_TYPE* cmd_unit)
{

	BSP_UsartSendByte(p_unit->Uart_port,cmd_unit->prefix_MSB);
	BSP_UsartSendByte(p_unit->Uart_port,cmd_unit->prefix_LSB);
	BSP_UsartSendByte(p_unit->Uart_port,cmd_unit->type);
	BSP_UsartSendByte(p_unit->Uart_port,cmd_unit->lens);
	BSP_UsartSendByte(p_unit->Uart_port,cmd_unit->addr_MSB);
	BSP_UsartSendByte(p_unit->Uart_port,cmd_unit->addr_LSB);
	BSP_UsartSendByte(p_unit->Uart_port,cmd_unit->data);
	BSP_UsartSendByte(p_unit->Uart_port,cmd_unit->FCS_MSB);
	BSP_UsartSendByte(p_unit->Uart_port,cmd_unit->FCS_LSB);
	return true;

}
bool EM1399_Receive(DRV_EM1399_TYPE* p_unit,RX_BUF* EM1399_RX)
{	

	uint8_t i;
	uint16_t UART_RX_STA;
	
	uint8_t err = OS_ERR_NONE;
	OSSemPend(_gp_ScanSemIn,1000,&err);
	switch(p_unit->Uart_port)
	{
		case _USART1_:
			UART_RX_STA = USART1_RX_STA;
			EM1399_RX->RX_LEN = (UART_RX_STA&0xff);
			for(i = 0;i<(UART_RX_STA&0xff);i++)
				EM1399_RX->RX_Ptr[i] = USART1_RX_BUF[i];
			USART1_RX_STA = 0;
			break;
		case _USART2_:
			UART_RX_STA = USART2_RX_STA;
			EM1399_RX->RX_LEN = (UART_RX_STA&0xff);
			for(i = 0;i<(UART_RX_STA&0xff);i++)
				EM1399_RX->RX_Ptr[i] = USART2_RX_BUF[i];
			USART2_RX_STA = 0;
			break;
		case _USART3_:
			UART_RX_STA = USART3_RX_STA;
			EM1399_RX->RX_LEN = (UART_RX_STA&0xff);
			for(i = 0;i<(UART_RX_STA&0xff);i++)
				EM1399_RX->RX_Ptr[i] = USART3_RX_BUF[i];
			USART3_RX_STA = 0;
			break;
		case _UART4_:
			UART_RX_STA = USART4_RX_STA;
			EM1399_RX->RX_LEN = (UART_RX_STA&0xff);
			for(i = 0;i<(UART_RX_STA&0xff);i++)
				EM1399_RX->RX_Ptr[i] = USART4_RX_BUF[i];
			USART4_RX_STA = 0;
			break;
		default:
			break;
	}
	if(EM1399_RX->RX_LEN == 0)
	return false;
	else
	return true;
	
	
}
void EM1399_Enable(DRV_EM1399_TYPE* p_unit)
{
							 
	EM1399_Write(p_unit,&scan_cmd);

}
void EM1399_DISABLE(DRV_EM1399_TYPE* p_unit)
{

	EM1399_Write(p_unit,&scan_cmd_mode_stop);

}
uint16_t crc_cal_by_bit(uint8_t* ptr, uint8_t len)
{
	unsigned int crc = 0;
	while(len-- != 0)
	{
		for(unsigned char i = 0x80; i != 0; i /= 2)
	{
		crc *= 2;
		if((crc&0x10000) !=0) 
		crc ^= 0x11021;
		if((*ptr&i) != 0) 
		crc ^= 0x1021;
	}
	ptr++;
	}
	return crc;
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

