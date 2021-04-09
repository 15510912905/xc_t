/********************************************************
   版权所有 (C), 2001-2100, 四川新健康成生物股份有限公司
  -------------------------------------------------------

		嵌入式开发平台——TD300 检测功能定义

  -------------------------------------------------------
   文 件 名   : Func_TD300_Scatter.c
   版 本 号   : V1.0.0.0
   作    者   : ryc
   生成日期   : 2019年12月23日
   功能描述   : TD300 检测功能定义
   依赖于     : 
 ********************************************************/
 
#include "Func_TD300_Scatter.h"
#include "ucos_ii.h"

static DRV_MAX5402EUA_TYPE* _gp_Max5402eua = NULL;	/*定义数字电位器*/
//static DRV_MAX4634EUB_TYPE* _gp_Max4634eub    = NULL;	/*定义多路开关*/
static DRV_AD7685_TYPE*     _gp_Ad7685        = NULL;	/*定义ADC*/
static DRV_SWITCHINGSENSOR_TYPE* _gp_Sen      = NULL;	/*定义采光信号*/

CMD_READ_AD_RETURN_TYPE* gp_Data = NULL;	/*获取AD值*/
AD_OP_TYPE* _gp_AllAD = NULL;

/* 模块1私有初始化（由各模块Func文件实现）
  ------------------------------
  返回值：true，成功；false，失败
*/
bool Func_PrivateInit1(DList* dlist,
						DRV_MCC_TYPE** m,
						DRV_VALVE_TYPE** v,
						DRV_PUMP_TYPE** p)
{
	/*硬件初始化*/
	Func_TD300ScatterInit();
		
	/*指令任务注册*/
	Cmd_AddCmdNode(dlist, CMD_COM_HELLO,			Func_Cmd_Com_Hello);				/*握手*/
//	Cmd_AddCmdNode(dlist, CMD_READ_MCC_POSITION,	Func_Cmd_Com_Read_Mcc_Position);	/*读取组件位置*/
//	Cmd_AddCmdNode(dlist, CMD_COM_MCC_RESET,		Func_Cmd_Com_Mcc_Reset);			/*组件复位*/
//	Cmd_AddCmdNode(dlist, CMD_COM_MCC_MOVE,			Func_Cmd_Com_Mcc_Move);				/*组件运动控制*/
//	Cmd_AddCmdNode(dlist, CMD_COM_VALVE_CONTROL,	Func_Cmd_Com_Valve_Control);		/*电磁阀/电磁铁控制*/
//	Cmd_AddCmdNode(dlist, CMD_COM_READPIN,			Func_Cmd_Com_ReadPin);				/*读取信号*/
//	Cmd_AddCmdNode(dlist, CMD_COM_PUMP_CONTROL,		Func_Cmd_Com_Pump_Control);			/*泵/直流电机控制*/
//	Cmd_AddCmdNode(dlist, CMD_COM_RW_PARAMETER,		Func_Cmd_Com_RW_Parameter);			/*单个参数读写*/
	Cmd_AddCmdNode(dlist, CMD_READ_ONE_AD,			Func_Cmd_ReadOneAd);				/*读一个AD值*/
	Cmd_AddCmdNode(dlist, CMD_READ_AD,				Func_Cmd_ReadAd);					/*检测流程*/
	Cmd_AddCmdNode(dlist, CMD_SET_GAIN,				Func_Cmd_SetGain);					/*设置增益*/
	Cmd_AddCmdNode(dlist, CMD_READ_ALL_AD,			Func_Cmd_ReadAllAd);					/*平坦区*/
	return true;
}

/* 模块2私有初始化（由各模块Func文件实现）
  ------------------------------
  返回值：true，成功；false，失败
*/
bool Func_PrivateInit2(DList* dlist,
						DRV_MCC_TYPE** m,
						DRV_VALVE_TYPE** v,
						DRV_PUMP_TYPE** p)
{
	return true;	/*XC2000检测模块独占一个板卡*/
}

/* 初始化 Func_TD300DetectionInit
  ------------------------------
  初始化运动组件和清洗阀
  返回值：true，成功；false，失败
*/
bool Func_TD300ScatterInit(void)
{
	/*初始化增益*/
	_gp_Max5402eua = Max5402euaInit(MAX5402EUA_1, 85, \
						_PORT_BUTT_, _PIN_BUTT_, \
						_PA_, _P7_, \
						_PA_, _P5_, \
						_PB_, _P10_);
	if(_gp_Max5402eua == NULL)	/*定义数字电位器，编号1，端口PA7、PA5、PB10*/
	{
		return false;
	}
	
//	_gp_Max5402eua[1] = Max5402euaInit(MAX5402EUA_2, 185, \
//						_PORT_BUTT_, _PIN_BUTT_, \
//						_PA_, _P7_, \
//						_PA_, _P5_, \
//						_PB_, _P11_);
//	if(_gp_Max5402eua[1] == NULL)	/*定义数字电位器，编号2，端口PA7、PA5、PB11*/
//	{
//		return false;
//	}
//	
//	_gp_Max5402eua[2] = Max5402euaInit(MAX5402EUA_3, 185, \
//						_PORT_BUTT_, _PIN_BUTT_, \
//						_PA_, _P7_, \
//						_PA_, _P5_, \
//						_PB_, _P1_);	/*定义数字电位器，编号3，端口PA7、PA5、PB1*/
//	if(_gp_Max5402eua[2] == NULL)
//	{
//		return false;
//	}
//	
//	_gp_Max5402eua[3] = Max5402euaInit(MAX5402EUA_4, 185, \
//						_PORT_BUTT_, _PIN_BUTT_, \
//						_PA_, _P7_, \
//						_PA_, _P5_, \
//						_PB_, _P2_);	/*定义数字电位器，编号4，端口PA7、PA5、PB2*/
//	if(_gp_Max5402eua[3] == NULL)
//	{
//		return false;
//	}
//						
//	_gp_Max4634eub = Max4634eubInit(MAX4634EUB_1, _PC_, _P6_, _PC_, _P7_);	/*定义多路选择器，编号1，端口PC6、PC7*/
//	if(_gp_Max4634eub == NULL)
//	{
//		return false;
//	}
	
	_gp_Ad7685 = Drv_Ad7685Init(AD7685_1, _SPI2_, _PB_, _P12_, _PB_, _P13_, _PB_, _P14_, _PORT_BUTT_, _PIN_BUTT_);	/*定义片外AD芯片，编号1，使用SPI2接口，端口PB12、PB13、PB14、PB15*/
	if(_gp_Ad7685 == NULL)
	{
		return false;
	}
	
	_gp_Sen = Drv_SwitchingSensorInit(SWITCHINGSENSOR_1, FALLING, 0, _PA_, _P3_);	/*定义采光光耦，编号1，下降沿响应，消抖延时10ms，端口PA3*/
	if(_gp_Sen == NULL)
	{
		return false;
	}
	
	gp_Data = malloc(sizeof(CMD_READ_AD_RETURN_TYPE));
	_gp_AllAD = malloc(sizeof(AD_OP_TYPE)*2001);
//	
//	Max5402euaSet(_gp_Max5402eua[3], 255);
//	Max5402euaSet(_gp_Max5402eua[2], 255);
//	Max5402euaSet(_gp_Max5402eua[1], 255);
	Max5402euaSet(_gp_Max5402eua, 255); //85
	
	return true;
}


/* 私有常驻线程1
  -----------------------------------
  由Func层实现
*/
void TaskResident1(void* p_arg)
{
	Drv_AllSwitchingSensorDebounce_Loop();
}

/* 私有常驻线程2
  -----------------------------------
  由Func层实现
*/
void TaskResident2(void* p_arg)
{
	while(1)
	{
		OSTimeDlyHMSM(0, 0, 0, 500);	/*XC2000检测模块独占一个板卡*/
	}
}

/* 读一个AD值
  ------------------------------ 
  返回值：true，成功；false，失败
*/
bool Func_Cmd_ReadOneAd(void* p_buffer)
{
	DRV_CAN_RX_BUFFER_TYPE*	p_buf  = (DRV_CAN_RX_BUFFER_TYPE *)p_buffer;		/*获取缓冲区数据*/
	CMD_COMMON_HEAD_TYPE*	p_msg  = (CMD_COMMON_HEAD_TYPE*)(p_buf->buffer);	/*获取消息内容*/
	CMD_READ_ONE_AD_TYPE*	p_data = (CMD_READ_ONE_AD_TYPE*)(&(p_msg->data));		/*获取数据域*/
	
	uint8_t  size          = p_buf->size;			/*获取指令长度*/
	uint8_t  can_id        = p_buf->can_id;			/*获取CAN ID*/
	uint8_t  serial_number = p_msg->serial_number;	/*获取流水号*/
	uint16_t cmd           = p_msg->cmd;			/*获取指令*/
	uint32_t frame_id      = p_msg->frame_id;		/*获取报文ID*/
//	uint32_t datas[4][10];							/*缓存AD数据*/
	uint8_t i = 0;
	uint32_t temp[10] = {0};	/*一个通道读5次，取平均*/
	
	if(size != 3)
	{
		Drv_CANProtocol_SendErrorToBuffer(can_id, serial_number, cmd, frame_id, 0xFF, 0xFF, 0x05);		/*报错，指令长度错误*/
		return false;
	}
	
//	if(p_data->channel > 4)
//	{
//		Drv_CANProtocol_SendErrorToBuffer(can_id, serial_number, cmd, frame_id, 0xFF, 0xFF, 0x08);		/*报错，指令参数错误*/
//		return false;
//	}
	
//		gp_Data->count = 4;
//		for(j = 0; j < 10; j++)
//		{
//			for(i = 0; i < 4; i++)
//			{
//				Max4634eubChoose(_gp_Max4634eub, i + 1);				/*通道选择*/
//			datas[i][j] = Ad7685OneAcq(_gp_Ad7685);
//			for(j = 0; j < 5; j++)								/*修改单通道采集次数为7次*/	
//			{
//				temp[j] = Ad7685OneAcq(_gp_Ad7685);				/*读取AD值*/
//			}
//			}
//		}
		for(i = 0; i < 10; i++)
		{
//			for(j = 0; j < 10; j++)
//			{
			temp[i] = Ad7685OneAcq(_gp_Ad7685);
//			}
//			gp_Data->datas[0] = _Func_Average(temp, 10);		/*均值滤波*/
		}
		gp_Data->datas[0] = _Func_Average(temp, 10);		/*均值滤波*/
		gp_Data->count=1;
//		_Func_Sort4(&(gp_Data->datas[0][0]));					/*排序*/
		Drv_CANProtocol_SendDatasToBuffer(can_id, serial_number, cmd, frame_id, (uint8_t*)(gp_Data), 8, 44);	/*直接回复执行成功*/
//	else
//	{
//		gp_Data->count = 1;
//		Max4634eubChoose(_gp_Max4634eub, _Func_Mapping4(p_data->channel - 1) + 1);	/*通道选择*/					/*通道选择*/
//		for(j = 0; j < 10; j++)
//		{
//			temp[j] = Ad7685OneAcq(_gp_Ad7685);				/*读取AD值*/
//		}
//		gp_Data->datas[0][0] = _Func_Average(temp, 10);		/*均值滤波*/
//		Drv_CANProtocol_SendDatasToBuffer(can_id, serial_number, cmd, frame_id, (uint8_t*)(gp_Data), 8, 44);	/*直接回复执行成功*/
//	}
	
	return true;
}

/* 检测流程
  ------------------------------
  返回值：true，成功；false，失败
*/
bool Func_Cmd_ReadAd(void* p_buffer)
{
	DRV_CAN_RX_BUFFER_TYPE*	p_buf  = (DRV_CAN_RX_BUFFER_TYPE *)p_buffer;		/*获取缓冲区数据*/
	CMD_COMMON_HEAD_TYPE*	p_msg  = (CMD_COMMON_HEAD_TYPE*)(p_buf->buffer);	/*获取消息内容*/
	CMD_READ_AD_TYPE*		p_data = (CMD_READ_AD_TYPE*)(&(p_msg->data));		/*获取数据域*/
	
	uint8_t  size          = p_buf->size;			/*获取指令长度*/
	uint8_t  can_id        = p_buf->can_id;			/*获取CAN ID*/
	uint8_t  serial_number = p_msg->serial_number;	/*获取流水号*/
	uint16_t cmd           = p_msg->cmd;			/*获取指令*/
	uint32_t frame_id      = p_msg->frame_id;		/*获取报文ID*/
	uint32_t datas[4][10];							/*数据缓存*/	
    
	uint8_t i = 0;
	uint16_t j = 0, k = 0;
	int16_t len = 0;		/*记录数据长度*/
//	uint8_t err = OS_ERR_NONE;
	uint32_t temp[10] = {0};
	uint8_t last_sta = 0, sta = 1;	/*存入当前状态*/
	uint32_t timer = 0;	
	if(size != 3)
	{
		Drv_CANProtocol_SendErrorToBuffer(can_id, serial_number, cmd, frame_id, 0xFF, 0xFF, 0x05);		/*报错，指令长度错误*/
		return false;
	}
	
	gp_Data->count = p_data->count;
	Drv_SwitchingSensorSemSet(_gp_Sen, EN_SEM, RISING);	/*开启IO下降沿信号量*/
	
	for(i = 0; i < p_data->count; i++)
	{
		//OSSemPend(_gp_Sen->sem, 1000, &err);/*等待光耦信号,超时时间1s*/
		timer = OSTICK_MS;
		while(1)
		{	
			
			sta = BSP_ReadPin(_gp_Sen->port_number, _gp_Sen->pin_number);	/*读当前状态*/
			if( sta == 1)
			{	
				last_sta = 1;
				while(sta)
				{
					
					sta = BSP_ReadPin(_gp_Sen->port_number, _gp_Sen->pin_number);
					if(OSTICK_MS - timer > 50000)												//超过100次退出
						break;
				}
			}
			if((sta == 0) && (last_sta) )
			{
				last_sta = 0;
				break;
			}
			if(OSTICK_MS - timer > 50000)
				break;
		}
			
//			for(j = 0; j < 10; j++)
//			{
//				for(k = 0; k < 4; k++)
//				{
//					Max4634eubChoose(_gp_Max4634eub, k + 1);				/*通道选择*/
//					datas[k][j] = Ad7685OneAcq(_gp_Ad7685);
//				}
//			}
			timer = OSTICK_MS - timer;
			if(timer > 50000)	/*超时报错*/
			{
				Drv_CANProtocol_SendErrorToBuffer(can_id, serial_number, cmd, frame_id, 0xFF, 0xFF, 0x09);		/*报错，指令执行错误*/
				return false;
			}
			for(j = 0; j < 10; j++)
			{
//				for(j = 0; j < 10; j++)
//				{
				temp[j] = Ad7685OneAcq(_gp_Ad7685);
//				}
//				gp_Data->datas[i] = _Func_Average(temp, 10);		/*均值滤波*/
			}
			
			gp_Data->datas[i] = _Func_Average(temp, 10);		/*均值滤波*/
//		for(j = 0; j < 4; j++)
//		{
//			
//			Max4634eubChoose(_gp_Max4634eub, j + 1);	/*通道选择*/
//			for(k = 0; k < 5; k++)						/*修改单通道采集次数为10次*/			
//			{
//				temp[k] = Ad7685OneAcq(_gp_Ad7685);					/*读取AD值*/
//			}
//			gp_Data->datas[i][j] = _Func_Average(temp, 5);			/*均值滤波*/
//		}
//		_Func_Sort4(&(gp_Data->datas[i][0]));						/*排序*/
		if(i == p_data->count - 1)		/*采完直接退出*/
			break;
		}
	Drv_SwitchingSensorSemSet(_gp_Sen, DIS_SEM, RISING);	/*关闭IO下降沿信号量*/
	len = p_data->count * 4  + 4;	/*计算待发送数据总长度*/
	Drv_CANProtocol_SendDatasToBuffer(can_id, serial_number, cmd, frame_id, (uint8_t*)(gp_Data), len, 44);	/*直接回复执行成功*/
	
	return true;
}

/* 设置增益
  ------------------------------
  返回值：true，成功；false，失败
*/
bool Func_Cmd_SetGain(void* p_buffer)
{
	DRV_CAN_RX_BUFFER_TYPE*	p_buf  = (DRV_CAN_RX_BUFFER_TYPE *)p_buffer;		/*获取缓冲区数据*/
	CMD_COMMON_HEAD_TYPE*	p_msg  = (CMD_COMMON_HEAD_TYPE*)(p_buf->buffer);	/*获取消息内容*/
	CMD_SET_GAIN_TYPE*		p_data = (CMD_SET_GAIN_TYPE*)(&(p_msg->data));		/*获取数据域*/
	
	uint8_t  size          = p_buf->size;			/*获取指令长度*/
	uint8_t  can_id        = p_buf->can_id;			/*获取CAN ID*/
	uint8_t  serial_number = p_msg->serial_number;	/*获取流水号*/
	uint16_t cmd           = p_msg->cmd;			/*获取指令*/
	uint32_t frame_id      = p_msg->frame_id;		/*获取报文ID*/
		
	if(size != 3)
	{
		Drv_CANProtocol_SendErrorToBuffer(can_id, serial_number, cmd, frame_id, 0xFF, 0xFF, 0x05);		/*报错，指令长度错误*/
		return false;
	}
	
//	if((p_data->channel < 1) || (p_data->channel > 4))
//	{
//		Drv_CANProtocol_SendErrorToBuffer(can_id, serial_number, cmd, frame_id, 0xFF, 0xFF, 0x08);		/*报错，指令参数错误*/
//		return false;
//	}
	Max5402euaSet(_gp_Max5402eua, p_data->gain);
	
	Drv_CANProtocol_SendDoneToBuffer(can_id, serial_number, cmd, frame_id);	/*直接回复执行成功*/
	return true;
}

/*--------------------------------------------------------------*/

/* a-b滤波
   均值滤波 ----20191216
   重定义滤波算法，功能为去除五次采集得到的最大和最小值，将剩下的值取平均
------------------------------
  返回值：均值
*/
static uint32_t _Func_Average(uint32_t* datas, uint8_t count)
{
//	uint32_t temp = datas[0];
	uint32_t temp = 0; 
	uint8_t i = 0,j = 0;
	for(j = 0;j < count; j++)			/*先对采集的AD值进行冒泡排序*/
	{
		for(i = 0; i < count; i++)
		{
			if(datas[i] < datas[i+1])
			{	
				temp = datas[i];
				datas[i] = datas[i+1];
				datas[i+1] = temp;
			}
		}
	}
	temp = 0;
	for(i = 2; i < count - 2; i++)
	{
		temp = temp + datas[i];
	}
	temp = temp/(count - 4);           /*求得去除最大和最小值之后剩余值得均值*/
//	for(i = 1; i < count; i++)
//	{
//		temp = temp * 0.5 + datas[i] * 0.5;
//	}
	
	return temp;
}

/* 按照4个波长从小到大排序
  ------------------------------
  返回值：true，成功；false，失败
*/
//static bool _Func_Sort4(uint32_t* datas)
//{
//	uint32_t temp[4] = {0};
//	uint8_t i = 0;
//	
//	if(datas == NULL)
//	{
//		return false;
//	}
//	
//	for(i = 0; i < 4; i++)
//	{
//		temp[i] = datas[i];
//	}
//	
//	for(i = 0; i < 4; i++)
//	{
//		datas[i] = temp[_Func_Mapping4(i)];
//	}
//	
//	return true;
//}

/* 按照4个波长编号与通道编号转换
  ------------------------------
  返回值：通道编号
*/
//static uint8_t _Func_Mapping4(uint8_t number)
//{
//	switch(number)
//	{
//		case 0:		return 2;
//		case 1:		return 1;
//		case 2:		return 3;
//		case 3:		return 0;
//	}
//}
static bool Func_Cmd_ReadAllAd(void* p_buffer)
{
	DRV_CAN_RX_BUFFER_TYPE* p_buf  = (DRV_CAN_RX_BUFFER_TYPE *)p_buffer; /*获取缓冲区数据*/
	CMD_COMMON_HEAD_TYPE* p_msg  = (CMD_COMMON_HEAD_TYPE*)(p_buf->buffer);	/*获取消息内容*/
	CMD_READ_ALL_AD_TYPE* p_data = (CMD_READ_ALL_AD_TYPE*)(&(p_msg->data));	/*获取数据域*/
	
	uint8_t  size          = p_buf->size;	/*获取指令长度*/
	uint8_t  can_id        = p_buf->can_id;	/*获取CAN ID*/
	uint8_t  serial_number = p_msg->serial_number; /*获取流水号*/
	uint16_t cmd           = p_msg->cmd;	/*获取指令*/
	uint32_t frame_id      = p_msg->frame_id;	/*获取报文ID*/
	
	uint8_t err = OS_ERR_NONE;
	uint16_t i = 0;
	uint8_t j = 0, k = 0;
	uint8_t last_op = 0;
	uint32_t temp[10] = {0};
	
	if(size != 3)
	{
	Drv_CANProtocol_SendErrorToBuffer(can_id, serial_number, cmd, frame_id, 0xFF, 0xFF, 0x05);	/*报错，指令长度错误*/
	return false;
	}
	
//	Max4634eubChoose(_gp_Max4634eub,  1);;	/*选择通道1*/
	
	_gp_AllAD[0].op = can_id;
	_gp_AllAD[0].ad = 0;
	
	/*1ms读一次AD值*/
	i = 1;
	last_op = BSP_ReadPin(_gp_Sen->port_number, _gp_Sen->pin_number);	/*读取OP状态*/
	while(i < 2001)
	{
	_gp_AllAD[i].op = BSP_ReadPin(_gp_Sen->port_number, _gp_Sen->pin_number);	/*读取OP状态*/
	
	if((last_op == 1) && (_gp_AllAD[i].op == 0))	/*下降沿*/
	{
		last_op = _gp_AllAD[i].op;
		for(j=0;j<4;j++)
		{
		for(k = 0; k < 10; k++)
		{
			temp[k] = Ad7685OneAcq(_gp_Ad7685);	/*读取AD值*/
		}
		gp_Data->datas[j] = _Func_Average(temp, 10);	/*滤波*/
		}
		for(j = 0; j < 3; j++)
		{
			_gp_AllAD[i].op = 2;
			_gp_AllAD[i++].ad = gp_Data->datas[j];
		}
		_gp_AllAD[i].op = 2;
		_gp_AllAD[i].ad = gp_Data->datas[j];
		
	}
	else
	{
		last_op = _gp_AllAD[i].op;
		_gp_AllAD[i].ad = Ad7685OneAcq(_gp_Ad7685); /*读取AD值*/
	}
	
	i++;
	OSTimeDlyHMSM(0, 0, 0, 1);
	}
	
	Drv_CANProtocol_SendDatasToBuffer(can_id, serial_number, cmd, frame_id, (uint8_t*)(_gp_AllAD), 2001 * sizeof(AD_OP_TYPE), 45);	/*直接回复执行成功*/

	return true;
}
