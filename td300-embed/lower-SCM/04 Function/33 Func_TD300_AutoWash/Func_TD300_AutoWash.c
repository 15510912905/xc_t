/********************************************************
   版权所有 (C), 2001-2100, 四川新健康成生物股份有限公司
  -------------------------------------------------------

		嵌入式开发平台——TD300自动清洗功能定义

  -------------------------------------------------------
   文 件 名   : Func_TD300_AutoWash.c
   版 本 号   : V1.0.0.0
   作    者   : ryc
   生成日期   : 2019年10月14日
   功能描述   : TD300自动清洗功能定义
   依赖于     : 
 ********************************************************/
 
#include "Func_TD300_AutoWash.h"

DRV_MCC_TYPE* _gp_VRT = NULL;	/*垂直运动组件*/
static DRV_PUMP_TYPE* _gp_LiquidPump3 = NULL;	/*液泵3, 光源灯散热*/
static DRV_PUMP_TYPE* _gp_LiquidPump1 = NULL;	/*液泵1，加稀释清洗液*/
static DRV_PUMP_TYPE* _gp_LiquidPump2 = NULL;	/*液泵2，加纯化水*/
static DRV_PUMP_TYPE* _gp_AirPump1    = NULL;	/*气泵1，排针1*/
static DRV_PUMP_TYPE* _gp_AirPump2    = NULL;	/*气泵2，排针2*/
static DRV_PUMP_TYPE* _gp_AirPump3    = NULL;	/*气泵3，排针3*/
static DRV_PUMP_TYPE* _gp_AirPump4    = NULL;	/*气泵4，排针4*/
static DRV_PUMP_TYPE* _gp_CleaningWaterPump = NULL;	/*清洗水泵，针的内壁清洗水泵*/
static DRV_VALVE_TYPE* _gp_LiquidValve1 = NULL;	/*液泵1后端阀*/
static DRV_VALVE_TYPE* _gp_LiquidValve2 = NULL;	/*液泵2针2后端阀*/
static DRV_VALVE_TYPE* _gp_LiquidValve3 = NULL;	/*液泵2针3后端阀*/

static OS_EVENT* _gp_AutoWashSemIn = NULL;	/*自动清洗液路输入信号量*/
static OS_EVENT* _gp_AutoWashSemOut = NULL;	/*自动清洗液路输出信号量*/
static uint16_t _g_List = 0;	/*自动清洗执行表*/
//static DRV_AT24C32_TYPE* _gp_EEP = NULL;

static MCC_PARAMETER1_TYPE _para1_VRT = {0x11, 16, 1.8*10, 12.7*100, 100,
										  MCC_RESET_POSITIVE,MCC_SHELTER_YES,MCC_SHELTER_YES, MCC_SHELTER_NO,10,
										  500,	1900,	16, 60,	30,
										  300,	1500,	16, 40,	25};
static MCC_PARAMETER2_TYPE _para2_VRT[3] = {{0x11, DISENABLE_CLOSE_CYCLE, 0.0*100, -1.0*100, 1.0*100, 1.0*100, 1.0*100},
											{0x11, DISENABLE_CLOSE_CYCLE, 21.84*100, 20.09*100, 23.09*100, 1.0*100, 1.0*100},
                                            {0x11, ENABLE_CLOSE_CYCLE, 40.06*100, 38.74*100, 41.60*100, 4.09*100, 1.0*100}
											};



/* 初始化接口----------------------------------------------------*/

/* 模块2私有初始化（由各模块Func文件实现）
  ------------------------------
  返回值：true，成功；false，失败
*/
bool Func_PrivateInit2(DList* dlist,
						DRV_MCC_TYPE** m,
						DRV_VALVE_TYPE** v,
						DRV_PUMP_TYPE** p)
{
	_gp_EEP = At24c32Init(AT24C32_1, \
				_PB_, _P6_, \
				_PB_, _P7_);
	/*硬件初始化*/
	Func_TD300AutoWashInit();
	
	m[0] = _gp_VRT;
	p[0] = _gp_LiquidPump1;
	p[1] = _gp_LiquidPump2;
	p[2] = _gp_AirPump1;
	p[3] = _gp_AirPump2;
	p[4] = _gp_AirPump3;
	p[5] = _gp_AirPump4;
	p[6] = _gp_CleaningWaterPump;
	p[7] = _gp_LiquidPump3;
	v[7] = _gp_LiquidValve1;
	v[8] = _gp_LiquidValve2;
	v[9] = _gp_LiquidValve3;
	
	/*指令任务注册*/
	Cmd_AddCmdNode(dlist, CMD_COM_HELLO,			Func_Cmd_Com_Hello);				/*握手*/
	Cmd_AddCmdNode(dlist, CMD_READ_MCC_POSITION,	Func_Cmd_Com_Read_Mcc_Position);	/*读取组件位置*/
	Cmd_AddCmdNode(dlist, CMD_COM_MCC_RESET,		Func_Cmd_Com_Mcc_Reset);			/*组件复位*/
	Cmd_AddCmdNode(dlist, CMD_COM_MCC_MOVE,			Func_Cmd_Com_Mcc_Move);				/*组件运动控制*/
	Cmd_AddCmdNode(dlist, CMD_COM_VALVE_CONTROL,	Func_Cmd_Com_Valve_Control);		/*电磁阀/电磁铁控制*/
	Cmd_AddCmdNode(dlist, CMD_COM_READPIN,			Func_Cmd_Com_ReadPin);				/*读取信号*/
	Cmd_AddCmdNode(dlist, CMD_COM_PUMP_CONTROL,		Func_Cmd_Com_Pump_Control);			/*泵/直流电机控制*/
	Cmd_AddCmdNode(dlist, CMD_COM_RW_PARAMETER,		Func_Cmd_Com_RW_Parameter);			/*单个参数读写*/
	Cmd_AddCmdNode(dlist, CMD_COM_RW_PARAMETER_PAGE, Func_Cmd_Com_RW_Page_Parameter);   /*整页参数读写*/
	
	Cmd_AddCmdNode(dlist, CMD_AUTOWASH_RESET,		Func_CmdAutoWashReset);				/*自动清洗复位*/
	Cmd_AddCmdNode(dlist, CMD_AUTOWASH_MOVE,		Func_CmdAutoWashMove);				/*自动清洗升降*/
	Cmd_AddCmdNode(dlist, CMD_AUTOWASH,				Func_CmdAutoWash);					/*自动清洗*/
	Cmd_AddCmdNode(dlist, CMD_AUTOWASH_PVCONTROL,	Func_CmdAutoWashPvControl);			/*自动清洗单个泵阀控制*/
	
	return true;
}

/* 初始化----------------------------------------------------*/

/* 初始化 Func_TD300AutoWash
  ------------------------------
  初始化运动组件和泵阀
  返回值：true，成功；false，失败
*/
bool Func_TD300AutoWashInit(void)
{
	uint8_t temp = 0;
	temp = At24c32ReadByte(_gp_EEP,1*32);
	
	if(temp != 0x11)
	{
		At24c32WritePage(_gp_EEP,1,(uint8_t*)(&_para1_VRT));
		At24c32WritePage(_gp_EEP,11,(uint8_t*)(&(_para2_VRT[0])));
		At24c32WritePage(_gp_EEP,12,(uint8_t*)(&(_para2_VRT[1])));
		At24c32WritePage(_gp_EEP,13,(uint8_t*)(&(_para2_VRT[2])));
	}
	else
	{	
		At24c32ReadPage(_gp_EEP,1,(uint8_t*)(&_para1_VRT));
		At24c32ReadPage(_gp_EEP,11,(uint8_t*)(&(_para2_VRT[0])));
		At24c32ReadPage(_gp_EEP,12,(uint8_t*)(&(_para2_VRT[1])));
		At24c32ReadPage(_gp_EEP,13,(uint8_t*)(&(_para2_VRT[2])));
	}
	_gp_VRT = MCC_Init(MCC_1,
						_para1_VRT.subdivision, _para1_VRT.step_angle_multiply10/10.0, _para1_VRT.ratio_multiply100/100.0, _para1_VRT.max_step,	/*细分、步距角、电机转一圈的行程、最大行程*/
						MCC_B, _para1_VRT.reset_dir, _para1_VRT.reset_status,	/*有复位信号和工位信号、撞击信号的直线机构、复位方向、复位光耦状态*/
						_para1_VRT.d_code_status, _para1_VRT.strike_status,	/*码齿遮挡、撞击未遮挡*/
						_PD_, _P5_,			/*复位信号*/
						_PD_, _P6_,			/*码齿信号*/
						_PORT_NC_, _PIN_NC_,
						_PB_,      _P13_,		/*EN*/
						_PB_,      _P14_,		/*DIR*/
						_PB_,      _P12_);		/*STEP*/
	if(_gp_VRT == NULL)	/*模块初始化失败*/
	{
		return false;
	}
	MCC_SetCompensation(_gp_VRT, _para1_VRT.reset_compensation);	/*复位补偿：整步*/
	MCC_SetPresetLocation(_gp_VRT, 0,  _para2_VRT[0].en_close_cycle, _para2_VRT[0].data_multiply100/100.0, _para2_VRT[0].forward_pre_deceleration_data_multiply100/100.0, _para2_VRT[0].reverse_pre_deceleration_data_multiply100/100.0, _para2_VRT[0].forward_compensation_multiply100/100.0,_para2_VRT[0].reverse_compensation_multiply100/100.0);	/*预设位置1：复位位置坐标，正向预减速坐标，反向预减速坐标，正向补偿，反向补偿*/
	MCC_SetPresetLocation(_gp_VRT, 1,  _para2_VRT[1].en_close_cycle, _para2_VRT[1].data_multiply100/100.0, _para2_VRT[1].forward_pre_deceleration_data_multiply100/100.0, _para2_VRT[1].reverse_pre_deceleration_data_multiply100/100.0, _para2_VRT[1].forward_compensation_multiply100/100.0,_para2_VRT[1].reverse_compensation_multiply100/100.0);	/*预设位置2：清洗高位坐标，正向预减速坐标，反向预减速坐标，正向补偿，反向补偿 24.35 */
	MCC_SetPresetLocation(_gp_VRT, 2,  _para2_VRT[2].en_close_cycle, _para2_VRT[2].data_multiply100/100.0, _para2_VRT[2].forward_pre_deceleration_data_multiply100/100.0, _para2_VRT[2].reverse_pre_deceleration_data_multiply100/100.0, _para2_VRT[2].forward_compensation_multiply100/100.0,_para2_VRT[2].reverse_compensation_multiply100/100.0);	/*预设位置3：清洗低位坐标，正向预减速坐标，反向预减速坐标，正向补偿，反向补偿*/
	MCC_SetStempMotorLine(_gp_VRT, _para1_VRT.start_fre_h, _para1_VRT.end_fre_h, _para1_VRT.ech_lader_step_h, _para1_VRT.up_max_lader_h, _para1_VRT.s_par_h_divide100*100, 0);	/*高速曲线*//*2700达不到*//*300,	1000,	3, 160,	12000, 0*/ /* 300,	2000,	16, 70,	7000, 0 */
	MCC_SetStempMotorLine(_gp_VRT, _para1_VRT.start_fre_l, _para1_VRT.end_fre_l, _para1_VRT.ech_lader_step_l, _para1_VRT.up_max_lader_l, _para1_VRT.s_par_l_divide100*100, 1);	/*低速曲线*//*1650达不到*//*300,	800,	2, 120,	10000, 1*/ /* 300,	1700,	16, 40,	7000, 1 */
	
	/*---------------------------------------------------------*/
	_gp_LiquidPump1 = PumpInit(PUMP_1,
								PUMP_ADJUSTABLE_SPEED_WITH_SIGNAL,
								PUMP_POSITIVE_LOGIC,
								60000,    //此处频率为需求频率*50
								20,
								_T6_,
								_PE_,_P9_,
								_PA_, _P3_);
	if(_gp_LiquidPump1 == NULL)	/*模块初始化失败*/
	{
		return false;
	}
	/*---------------------------------------------------------*/
	_gp_LiquidPump3 = PumpInit(PUMP_8,
								PUMP_UNADJUSTABLE_SPEED,
								PUMP_POSITIVE_LOGIC,
								1000,
								50,
								_T6_,
								_PE_,_P8_,
								_PORT_BUTT_, _PIN_BUTT_);
	if(_gp_LiquidPump3 == NULL)
	{
		return false;
	}
	/*---------------------------------------------------------*/
	_gp_LiquidPump2 = PumpInit(PUMP_2,
								PUMP_ADJUSTABLE_SPEED_WITH_SIGNAL,
								PUMP_POSITIVE_LOGIC,
								60000,
								50,
								_T6_,
								_PC_,_P7_,
								_PA_, _P0_);
	if(_gp_LiquidPump2 == NULL)	/*模块初始化失败*/
	{
		return false;
	}
	
	/*---------------------------------------------------------*/
	_gp_AirPump1 = PumpInit(PUMP_3, 
								PUMP_UNADJUSTABLE_SPEED, 
								PUMP_POSITIVE_LOGIC,
								1000,
								50,
								_T6_,
								_PB_, _P0_,
								_PORT_BUTT_, _PIN_BUTT_);
	if(_gp_AirPump1 == NULL)	/*模块初始化失败*/
	{
		return false;
	}
	
	/*---------------------------------------------------------*/
	_gp_AirPump2 = PumpInit(PUMP_4, 
								PUMP_UNADJUSTABLE_SPEED, 
								PUMP_POSITIVE_LOGIC,
								1000,
								50,
								_T6_,
								_PC_, _P5_,
								_PORT_BUTT_, _PIN_BUTT_);
	if(_gp_AirPump2 == NULL)	/*模块初始化失败*/
	{
		return false;
	}
	
	/*---------------------------------------------------------*/
	_gp_AirPump3 = PumpInit(PUMP_5, 
								PUMP_UNADJUSTABLE_SPEED, 
								PUMP_POSITIVE_LOGIC,
								1000,
								50,
								_T6_,
								_PB_, _P1_,
								_PORT_BUTT_, _PIN_BUTT_);
	if(_gp_AirPump3 == NULL)	/*模块初始化失败*/
	{
		return false;
	}
	
	/*---------------------------------------------------------*/
	_gp_AirPump4 = PumpInit(PUMP_6, 
								PUMP_UNADJUSTABLE_SPEED, 
								PUMP_POSITIVE_LOGIC,
								1000,
								50,
								_T6_,
								_PE_, _P7_,
								_PORT_BUTT_, _PIN_BUTT_);
	if(_gp_AirPump4 == NULL)	/*模块初始化失败*/
	{
		return false;
	}
	
	/*---------------------------------------------------------*/
	_gp_CleaningWaterPump = PumpInit(PUMP_7, 
								PUMP_UNADJUSTABLE_SPEED, 
								PUMP_POSITIVE_LOGIC,
								1000,
								50,
								_T6_,
								_PE_, _P8_,
								_PORT_BUTT_, _PIN_BUTT_);
	if(_gp_CleaningWaterPump == NULL)	/*模块初始化失败*/
	{
		return false;
	}
	
	/*---------------------------------------------------------*/
	_gp_LiquidValve1 = Valve_Init(VALVE_8, 
								VALVE_NORMAL_CLOSE, 
								VALVE_POSITIVE_LOGIC,
								_PC_, _P13_);
	if(_gp_LiquidValve1 == NULL)	/*模块初始化失败*/
	{
		return false;
	}
	
	/*---------------------------------------------------------*/
	_gp_LiquidValve2 = Valve_Init(VALVE_9, 
								VALVE_NORMAL_CLOSE, 
								VALVE_POSITIVE_LOGIC,
								_PE_, _P5_);
	if(_gp_LiquidValve2 == NULL)	/*模块初始化失败*/
	{
		return false;
	}
	
	/*---------------------------------------------------------*/
	_gp_LiquidValve3 = Valve_Init(VALVE_10, 
								VALVE_NORMAL_CLOSE, 
								VALVE_POSITIVE_LOGIC,
								_PE_, _P3_);
	if(_gp_LiquidValve3 == NULL)	/*模块初始化失败*/
	{
		return false;
	}
	
	/*---------------------------------------------------------*/
	_gp_AutoWashSemIn  = OSSemCreate(0);	/*自动清洗液路输入信号量*/
	_gp_AutoWashSemOut = OSSemCreate(0);	/*自动清洗液路输出信号量*/
	
	return true;
}

/* 私有常驻任务接口----------------------------------------------------*/

/* 私有常驻线程1
  -----------------------------------
  由Func层实现
*/
void TaskResident1(void* p_arg)
{
	PumpStart(_gp_LiquidPump3);   /*液泵2常开*/
	while(1)	/*实现浮子状态监控*/
	{
		OSTimeDlyHMSM(0, 0, 0, 500);
	}
}

/* 私有常驻线程2
  -----------------------------------
  由Func层实现
*/
void TaskResident2(void* p_arg)
{
	uint8_t  err           = OS_ERR_NONE;
	
	while(1)	/*实现自动清洗电机运动中同时开关泵阀*/
	{
		OSSemPend(_gp_AutoWashSemIn, 0,& err);	/*等待启动自动清洗液路系统*/
		
		OSTimeDlyHMSM(0, 0, 0, 100);	/*延时0.3s*//*0.1*/
		
		/*开气泵*/
		/*全开*/
		PumpStart(_gp_AirPump1);
		PumpStart(_gp_AirPump2);
		PumpStart(_gp_AirPump3);
		PumpStart(_gp_AirPump4);
		
		OSTimeDlyHMSM(0, 0, 1, 200);			/*延时1s*//*0.2*/
		
		/*开液泵后端阀*/
		if(_g_List & 0x01)	/*第一阶要洗*/
		{
			Valve_Open(_gp_LiquidValve1);
		}
		if(_g_List & 0x02)	/*第二阶要洗*/
		{
			Valve_Open(_gp_LiquidValve2);
		}
		if(_g_List & 0x04)	/*第三阶要洗*/
		{
			Valve_Open(_gp_LiquidValve3);
		}
		
		OSTimeDlyHMSM(0, 0, 0, 200);			/*延时0.2s*/
		
		/*开液泵*/
		if(_g_List & 0x01)	/*第一阶要洗*/
		{
			PumpStart(_gp_LiquidPump1);
		}
		if((_g_List & 0x02) || (_g_List & 0x04))	/*第二阶要洗或者第三阶要洗*/
		{
			if((_g_List & 0x02)&(_g_List & 0x04) )
				_gp_LiquidPump2->duty_cycle = 50;
			else
				_gp_LiquidPump2->duty_cycle = 20;
			PumpStart(_gp_LiquidPump2);
		}
				
		OSTimeDlyHMSM(0, 0, 0, 900);			/*延时0.9s*/
		
		/*关液泵*/
		PumpStop(_gp_LiquidPump1);
		PumpStop(_gp_LiquidPump2);
		
		OSTimeDlyHMSM(0, 0, 0, 100);			/*延时0.1s*/
		
		/*关液泵后端阀*/
		Valve_Close(_gp_LiquidValve1);
		Valve_Close(_gp_LiquidValve2);
		Valve_Close(_gp_LiquidValve3);
				
		/*关气泵*/
		PumpStop(_gp_AirPump1);
		PumpStop(_gp_AirPump2);
		PumpStop(_gp_AirPump3);
		PumpStop(_gp_AirPump4);
		
		OSSemPost(_gp_AutoWashSemOut);	/*液路停止*/
	}
}

/* 私有常驻任务----------------------------------------------------*/

/*无*/

/* 私有指令任务----------------------------------------------------*/

/* 自动清洗复位
  ------------------------------
  返回值：true，成功；false，失败
*/
bool Func_CmdAutoWashReset(void* p_buffer)
{
	DRV_CAN_RX_BUFFER_TYPE*		p_buf  = (DRV_CAN_RX_BUFFER_TYPE *)p_buffer;			/*获取缓冲区数据*/
	CMD_COMMON_HEAD_TYPE*		p_msg  = (CMD_COMMON_HEAD_TYPE*)(p_buf->buffer);		/*获取消息内容*/
	CMD_AUTOWASH_RESET_TYPE*	p_data = (CMD_AUTOWASH_RESET_TYPE*)(&(p_msg->data));	/*获取数据域*/
	
	uint8_t  size          = p_buf->size;			/*获取指令长度*/
	uint8_t  can_id        = p_buf->can_id;			/*获取CAN ID*/
	uint8_t  serial_number = p_msg->serial_number;	/*获取流水号*/
	uint16_t cmd           = p_msg->cmd;			/*获取指令*/
	uint32_t frame_id      = p_msg->frame_id;		/*获取报文ID*/
	
	uint8_t  err           = OS_ERR_NONE;
	
	if(size != 3)			/*指令长度判断*/
	{
		Drv_CANProtocol_SendErrorToBuffer(can_id, serial_number, cmd, frame_id, 0xFF, 0xFF, 0x05);		/*报错，指令长度错误*/
		return false;
	}
	
	err = MCC_Reset(_gp_VRT, 10000);	/*升降电机复位*/
	if(err != OS_ERR_NONE)	/*执行失败*/
	{
		Drv_CANProtocol_SendErrorToBuffer(can_id, serial_number, cmd, frame_id, 0xFF, 0xFF, 0x09);		/*报错，指令执行错误*/
		return false;
	}
	
	Drv_CANProtocol_SendDoneToBuffer(can_id, serial_number, cmd, frame_id);	/*直接回复执行成功*/
	return true;
}

/* 自动清洗升降
  ------------------------------
  返回值：true，成功；false，失败
*/
bool Func_CmdAutoWashMove(void* p_buffer)
{
	DRV_CAN_RX_BUFFER_TYPE*		p_buf  = (DRV_CAN_RX_BUFFER_TYPE *)p_buffer;			/*获取缓冲区数据*/
	CMD_COMMON_HEAD_TYPE*		p_msg  = (CMD_COMMON_HEAD_TYPE*)(p_buf->buffer);		/*获取消息内容*/
	CMD_AUTOWASH_MOVE_TYPE*		p_data = (CMD_AUTOWASH_MOVE_TYPE*)(&(p_msg->data));		/*获取数据域*/
	
	uint8_t  size          = p_buf->size;			/*获取指令长度*/
	uint8_t  can_id        = p_buf->can_id;			/*获取CAN ID*/
	uint8_t  serial_number = p_msg->serial_number;	/*获取流水号*/
	uint16_t cmd           = p_msg->cmd;			/*获取指令*/
	uint32_t frame_id      = p_msg->frame_id;		/*获取报文ID*/
	
	uint8_t  err           = OS_ERR_NONE;
	
	if(size != 3)			/*指令长度判断*/
	{
		Drv_CANProtocol_SendErrorToBuffer(can_id, serial_number, cmd, frame_id, 0xFF, 0xFF, 0x05);		/*报错，指令长度错误*/
		return false;
	}
	
	switch(p_data->place)
	{
		case 0:	/*复位位置*/
			err = MCC_Move_PresetLocation(_gp_VRT,
										_gp_VRT->preset_location,
										5000,
										MCC_MOVE_DISTANCE,
										0,
										MCC_LINE_AUTO);
			break;
		case 1:	/*中间位置*/
			err = MCC_Move_PresetLocation(_gp_VRT,
										_gp_VRT->preset_location + 1,	/*清洗高位*/
										5000,
										MCC_MOVE_DISTANCE,
										0,
										MCC_LINE_AUTO);
			break;
		case 2:	/*清洗位置*/
			err = MCC_Move_PresetLocation(_gp_VRT,
										_gp_VRT->preset_location + 2,	/*清洗低位*/
										5000,
										((_gp_VRT->preset_location + 2)->en_close_cycle == ENABLE_CLOSE_CYCLE) ? MCC_MOVE_D_CODE: MCC_MOVE_DISTANCE,
										MCC_GetDCodeInterval(_gp_VRT, MCC_CalcSubdivisionStep(_gp_VRT, (_gp_VRT->preset_location + 2)->data)),
										MCC_LINE_AUTO);
			break;
	}
	if(err != OS_ERR_NONE)	/*执行失败*/
	{
		Drv_CANProtocol_SendErrorToBuffer(can_id, serial_number, cmd, frame_id, 0xFF, 0xFF, 0x09);		/*报错，指令执行错误*/
		return false;
	}
	
	Drv_CANProtocol_SendDoneToBuffer(can_id, serial_number, cmd, frame_id);	/*直接回复执行成功*/
	return true;
}

/* 自动清洗
  ------------------------------
  返回值：true，成功；false，失败
*/
bool Func_CmdAutoWash(void* p_buffer)
{
	DRV_CAN_RX_BUFFER_TYPE*	p_buf  = (DRV_CAN_RX_BUFFER_TYPE *)p_buffer;		/*获取缓冲区数据*/
	CMD_COMMON_HEAD_TYPE*	p_msg  = (CMD_COMMON_HEAD_TYPE*)(p_buf->buffer);	/*获取消息内容*/
	CMD_AUTOWASH_TYPE*		p_data = (CMD_AUTOWASH_TYPE*)(&(p_msg->data));		/*获取数据域*/
	
	uint8_t  size          = p_buf->size;			/*获取指令长度*/
	uint8_t  can_id        = p_buf->can_id;			/*获取CAN ID*/
	uint8_t  serial_number = p_msg->serial_number;	/*获取流水号*/
	uint16_t cmd           = p_msg->cmd;			/*获取指令*/
	uint32_t frame_id      = p_msg->frame_id;		/*获取报文ID*/
	
	uint8_t  err           = OS_ERR_NONE;
	
	if(size != 3)			/*指令长度判断*/
	{
		Drv_CANProtocol_SendErrorToBuffer(can_id, serial_number, cmd, frame_id, 0xFF, 0xFF, 0x05);		/*报错，指令长度错误*/
		return false;
	}
	
	if((_gp_VRT->reset_status) != BSP_ReadPin(_gp_VRT->reset_port_number, _gp_VRT->reset_pin_number))	/*如果光耦没有在复位状态，则报错*/
	{
		Drv_CANProtocol_SendErrorToBuffer(can_id, serial_number, cmd, frame_id, 0xFF, 0xFF, 0x0A);		/*报错，指令执行初始状态错误*/
		return false;
	}
	
	_g_List = p_data->list;	/*获取液路执行表*/
	OSSemPost(_gp_AutoWashSemIn);	/*启动液路*/
	
	/*降到清洗位置*/
	err = MCC_Move_PresetLocation(_gp_VRT,
								_gp_VRT->preset_location + 2,	/*清洗低位*/
								5000,
								((_gp_VRT->preset_location + 2)->en_close_cycle == ENABLE_CLOSE_CYCLE) ? MCC_MOVE_D_CODE: MCC_MOVE_DISTANCE,
								MCC_GetDCodeInterval(_gp_VRT, MCC_CalcSubdivisionStep(_gp_VRT, (_gp_VRT->preset_location + 2)->data)),
								MCC_LINE_AUTO);
	if(err != OS_ERR_NONE)	/*执行失败*/
	{
		Drv_CANProtocol_SendErrorToBuffer(can_id, serial_number, cmd, frame_id, 0xFF, 0xFF, 0x09);		/*报错，指令执行错误*/
		return false;
	}
	
	OSTimeDlyHMSM(0, 0, 1, 100);			/*延时1.1s*/
	
	/*升到清洗高位*/
	err = MCC_Move_PresetLocation(_gp_VRT,
								_gp_VRT->preset_location + 1,	/*清洗高位*/
								5000,
								((_gp_VRT->preset_location + 1)->en_close_cycle == ENABLE_CLOSE_CYCLE) ? MCC_MOVE_D_CODE: MCC_MOVE_DISTANCE,
								MCC_GetDCodeInterval(_gp_VRT, MCC_CalcSubdivisionStep(_gp_VRT, (_gp_VRT->preset_location + 1)->data)),
								MCC_LINE_AUTO);
	if(err != OS_ERR_NONE)	/*执行失败*/
	{
		Drv_CANProtocol_SendErrorToBuffer(can_id, serial_number, cmd, frame_id, 0xFF, 0xFF, 0x09);		/*报错，指令执行错误*/
		return false;
	}
	
	OSTimeDlyHMSM(0, 0, 0, 400);			/*延时0.4s*/
	
	OSSemPend(_gp_AutoWashSemOut, 5000, &err);	/*等待液路执行完成*/	/*20191114修改，在自动清洗提升时先同步*/
	if(err != OS_ERR_NONE)	/*执行失败*/
	{
		PumpStop(_gp_AirPump1);
		PumpStop(_gp_AirPump2);
		PumpStop(_gp_AirPump3);
		PumpStop(_gp_AirPump4);
		PumpStop(_gp_LiquidPump1);
		PumpStop(_gp_LiquidPump2);
		Drv_CANProtocol_SendErrorToBuffer(can_id, serial_number, cmd, frame_id, 0xFF, 0xFF, 0x09);		/*报错，指令执行错误*/
		return false;
	}
	
	/*复位*/
	err = MCC_Reset(_gp_VRT, 10000);	/*升降电机复位*/
	if(err != OS_ERR_NONE)	/*执行失败*/
	{
		Drv_CANProtocol_SendErrorToBuffer(can_id, serial_number, cmd, frame_id, 0xFF, 0xFF, 0x09);		/*报错，指令执行错误*/
		return false;
	}	
	
	Drv_CANProtocol_SendDoneToBuffer(can_id, serial_number, cmd, frame_id);	/*直接回复执行成功*/
	return true;
}

/* 自动清洗单个泵阀控制
  ------------------------------
  返回值：true，成功；false，失败
*/
bool Func_CmdAutoWashPvControl(void* p_buffer)
{
	DRV_CAN_RX_BUFFER_TYPE*			p_buf  = (DRV_CAN_RX_BUFFER_TYPE *)p_buffer;				/*获取缓冲区数据*/
	CMD_COMMON_HEAD_TYPE*			p_msg  = (CMD_COMMON_HEAD_TYPE*)(p_buf->buffer);			/*获取消息内容*/
	CMD_AUTOWASH_PVCONTROL_TYPE*	p_data = (CMD_AUTOWASH_PVCONTROL_TYPE*)(&(p_msg->data));	/*获取数据域*/
	
	uint8_t  size          = p_buf->size;			/*获取指令长度*/
	uint8_t  can_id        = p_buf->can_id;			/*获取CAN ID*/
	uint8_t  serial_number = p_msg->serial_number;	/*获取流水号*/
	uint16_t cmd           = p_msg->cmd;			/*获取指令*/
	uint32_t frame_id      = p_msg->frame_id;		/*获取报文ID*/
	
	uint8_t  err           = OS_ERR_NONE;
	
	if(size != 3)			/*指令长度判断*/
	{
		Drv_CANProtocol_SendErrorToBuffer(can_id, serial_number, cmd, frame_id, 0xFF, 0xFF, 0x05);		/*报错，指令长度错误*/
		return false;
	}
	
	switch(p_data->number)
	{
		case 1:
			if(p_data->status == 0)
				PumpStop(_gp_LiquidPump1);
			else
				PumpStart(_gp_LiquidPump1);
			break;
		case 2:
			if(p_data->status == 0)
				PumpStop(_gp_LiquidPump2);
			else
				PumpStart(_gp_LiquidPump2);
			break;
		case 3:
			if(p_data->status == 0)
				PumpStop(_gp_AirPump1);
			else
				PumpStart(_gp_AirPump1);
			break;
		case 4:
			if(p_data->status == 0)
				PumpStop(_gp_AirPump2);
			else
				PumpStart(_gp_AirPump2);
			break;
		case 5:
			if(p_data->status == 0)
				PumpStop(_gp_AirPump3);
			else
				PumpStart(_gp_AirPump3);
			break;
		case 6:
			if(p_data->status == 0)
				PumpStop(_gp_AirPump4);
			else
				PumpStart(_gp_AirPump4);
			break;
		case 7:
			if(p_data->status == 0)
				PumpStop(_gp_CleaningWaterPump);
			else
				PumpStart(_gp_CleaningWaterPump);
			break;
		default:
			Drv_CANProtocol_SendErrorToBuffer(can_id, serial_number, cmd, frame_id, 0xFF, 0xFF, 0x08);		/*报错，指令参数错误*/
			return false;
	}
	
	OSTimeDlyHMSM(0, 0, 0, 10);			/*延时*/
	
	Drv_CANProtocol_SendDoneToBuffer(can_id, serial_number, cmd, frame_id);	/*直接回复执行成功*/
	return true;
}
