/********************************************************
   版权所有 (C), 2001-2100, 四川新健康成生物股份有限公司
  -------------------------------------------------------

			嵌入式开发平台——TD300搅拌功能定义

  -------------------------------------------------------
   文 件 名   : Func_TD300_Stirring.c
   版 本 号   : V1.0.0.0
   作    者   : ryc
   生成日期   : 2019年10月14日
   功能描述   : TD300搅拌功能定义
   依赖于     : 
 ********************************************************/
 
#include "Func_TD300_Stirring.h"

static DRV_MCC_TYPE* _gp_VRT = NULL;	/*垂直运动组件*/
static DRV_MCC_TYPE* _gp_RTT = NULL;	/*水平运动组件*/
static DRV_VALVE_TYPE* _gp_CleaningValve1 = NULL;	/*清洗阀1*/
static DRV_VALVE_TYPE* _gp_CleaningValve2 = NULL;	/*清洗阀2*/
static DRV_PUMP_TYPE* _gp_DcMootr1 = NULL;	/*搅拌电机1*/
static DRV_PUMP_TYPE* _gp_DcMootr2 = NULL;	/*搅拌电机2*/

//static DRV_AT24C32_TYPE* _gp_EEP = NULL;

uint16_t RttNumberNow = 0;		/*当前位号，即当前针旋转位置*/
uint8_t DcMootrCleaning = 0;	/*记录已清洗的搅拌杆编号*/

static MCC_PARAMETER1_TYPE _para1_VRT =  {0x11, 16, 1.8*10, 42*100, 250,
										  MCC_RESET_POSITIVE,MCC_SHELTER_YES,MCC_SHELTER_YES, MCC_SHELTER_NO, 5,
										  100, 1300,  8,  40, 20,
										  100, 800,   8,  40, 20};
static MCC_PARAMETER2_TYPE _para2_VRT[2] = {{0x11, DISENABLE_CLOSE_CYCLE, 0.0*100, -1.0*100, 1.0*100, 1.0*100, 1.0*100},
											{0x11, DISENABLE_CLOSE_CYCLE, 47.67*100, 44.00*100, 48.67*100, 1.0*100, 1.0*100}
											};
static MCC_PARAMETER1_TYPE _para1_RTT = {0x11, 16, 1.8*10, 72*100, 360,
										  MCC_RESET_POSITIVE,MCC_SHELTER_YES,MCC_SHELTER_NO,MCC_SHELTER_YES, 5,
										  100,	1200,	16, 40,	20,
                                          100,	800,	16, 20,	10};
static MCC_PARAMETER2_TYPE _para2_RTT[4] = {{0x11, DISENABLE_CLOSE_CYCLE, -19.44*100, -20.44*100, -18.44*100, 1.0*100, 1.0*100},
											 {0x11,DISENABLE_CLOSE_CYCLE, 0.0*100, -1.0*100, 1.0*100, 1.0*100, 1.0*100},
											 {0x11,DISENABLE_CLOSE_CYCLE, 161.28*100, 159.10*100, 162.92*100, 1.0*100, 1.0*100},
											 {0x11,DISENABLE_CLOSE_CYCLE, 180.36*100, 179.36*100, 181.64*100, 1.0*100, 1.0*100}
											 };
STIRRING_PARAMETER_TYPE _para_STIRRING = {0x11, 600, 400};




/* 初始化接口----------------------------------------------------*/

/* 模块1私有初始化（由各模块Func文件实现）
  ------------------------------
  返回值：true，成功；false，失败
*/
bool Func_PrivateInit1(DList* dlist,
						DRV_MCC_TYPE** m,
						DRV_VALVE_TYPE** v,
						DRV_PUMP_TYPE** p)
{
	_gp_EEP = At24c32Init(AT24C32_1, \
						_PB_, _P6_, \
						_PB_, _P7_);
	
	/*硬件初始化*/
	Func_TD300StirringInit();
	
	m[0] = _gp_VRT;
	m[1] = _gp_RTT;
	p[0] = _gp_DcMootr1;
	p[1] = _gp_DcMootr2;
	v[2] = _gp_CleaningValve1;
	v[3] = _gp_CleaningValve2;
	
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
	
	Cmd_AddCmdNode(dlist, CMD_STIR_RESET,			Func_CmdStirReset);					/*搅拌复位*/
	Cmd_AddCmdNode(dlist, CMD_STIR_UPDOWN,			Func_CmdStirUpDown);				/*搅拌升降*/
	Cmd_AddCmdNode(dlist, CMD_STIR_TURN,			Func_CmdStirTurn);					/*搅拌转动*/
	Cmd_AddCmdNode(dlist, CMD_STIR_WASH,			Func_CmdStirWash);					/*搅拌清洗*/
	Cmd_AddCmdNode(dlist, CMD_STIR,					Func_CmdStir);						/*搅拌*/
	Cmd_AddCmdNode(dlist, CMD_STIR_PVCONTROL,		Func_CmdStirPvControl);				/*搅拌泵阀控制*/
	
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
	return true;	/*TD300搅拌模块独占一块板卡*/
}

/* 初始化----------------------------------------------------*/

/* 初始化 Func_TD300StirringInit
  ------------------------------
  初始化运动组件和清洗阀
  返回值：true，成功；false，失败
*/
bool Func_TD300StirringInit(void)
{
	uint8_t temp = 0;
	temp = At24c32ReadByte(_gp_EEP,1*32);
	
	if(temp != 0x11)
	{
		At24c32WritePage(_gp_EEP,1, (uint8_t*)(&_para1_VRT));
		At24c32WritePage(_gp_EEP,2, (uint8_t*)(&_para1_RTT));
		At24c32WritePage(_gp_EEP,104,(uint8_t*)(&_para_STIRRING));
		At24c32WritePage(_gp_EEP,11,(uint8_t*)(&(_para2_VRT[0])));
		At24c32WritePage(_gp_EEP,12,(uint8_t*)(&(_para2_VRT[1])));
		
		At24c32WritePage(_gp_EEP,21,(uint8_t*)(&(_para2_RTT[0])));
		At24c32WritePage(_gp_EEP,22,(uint8_t*)(&(_para2_RTT[1])));
		At24c32WritePage(_gp_EEP,23,(uint8_t*)(&(_para2_RTT[2])));
		At24c32WritePage(_gp_EEP,24,(uint8_t*)(&(_para2_RTT[3])));
	
	}
	else
	{
		At24c32ReadPage(_gp_EEP,1, (uint8_t*)(&_para1_VRT));
		At24c32ReadPage(_gp_EEP,2, (uint8_t*)(&_para1_RTT));
		At24c32ReadPage(_gp_EEP,104,(uint8_t*)(&_para_STIRRING));
		At24c32ReadPage(_gp_EEP,11,(uint8_t*)(&(_para2_VRT[0])));
		At24c32ReadPage(_gp_EEP,12,(uint8_t*)(&(_para2_VRT[1])));
		At24c32ReadPage(_gp_EEP,2, (uint8_t*)(&_para1_RTT));
		At24c32ReadPage(_gp_EEP,21,(uint8_t*)(&(_para2_RTT[0])));
		At24c32ReadPage(_gp_EEP,22,(uint8_t*)(&(_para2_RTT[1])));
		At24c32ReadPage(_gp_EEP,23,(uint8_t*)(&(_para2_RTT[2])));
		At24c32ReadPage(_gp_EEP,24,(uint8_t*)(&(_para2_RTT[3])));
	
	}
	/*垂直运动----------------------------------------------------*/
	_gp_VRT = MCC_Init(MCC_1,
						_para1_VRT.subdivision, _para1_VRT.step_angle_multiply10/10.0, _para1_VRT.ratio_multiply100/100.0, _para1_VRT.max_step,	/*细分、步距角、电机转一圈的行程、最大行程*/
						MCC_E, _para1_VRT.reset_dir, _para1_VRT.reset_status,	/*有复位信号和工位信号、撞击信号的直线机构、复位方向、复位光耦状态*/
						_para1_VRT.d_code_status, _para1_VRT.strike_status,	/*码齿遮挡、撞击未遮挡*/
						_PD_, _P15_,				/*复位信号*/
						_PC_, _P5_,				/*码齿信号*/
						_PC_, _P4_,				/*撞击信号*/
						_PC_,      _P11_,		/*EN*/
						_PC_,      _P12_,		/*DIR*/
						_PC_,      _P10_);		/*STEP*/
	if(_gp_VRT == NULL)	/*模块初始化失败*/
	{
		return false;
	}
	MCC_SetCompensation(_gp_VRT, _para1_VRT.reset_compensation);	/*复位补偿：整步*/
	MCC_SetPresetLocation(_gp_VRT, 0, _para2_VRT[0].en_close_cycle, _para2_VRT[0].data_multiply100/100.0, _para2_VRT[0].forward_pre_deceleration_data_multiply100/100.0, _para2_VRT[0].reverse_pre_deceleration_data_multiply100/100.0, _para2_VRT[0].forward_compensation_multiply100/100.0,_para2_VRT[0].reverse_compensation_multiply100/100.0);		/*预设位置1：复位位置坐标，正向预减速坐标，反向预减速坐标，正向补偿，反向补偿*/
	MCC_SetPresetLocation(_gp_VRT, 1, _para2_VRT[1].en_close_cycle, _para2_VRT[1].data_multiply100/100.0, _para2_VRT[1].forward_pre_deceleration_data_multiply100/100.0, _para2_VRT[1].reverse_pre_deceleration_data_multiply100/100.0, _para2_VRT[1].forward_compensation_multiply100/100.0,_para2_VRT[1].reverse_compensation_multiply100/100.0);	    /*预设位置2：搅拌/清洗位置坐标，正向预减速坐标（46.67），反向预减速坐标，正向补偿，反向补偿*/
	MCC_SetStempMotorLine(_gp_VRT, _para1_VRT.start_fre_h, _para1_VRT.end_fre_h, _para1_VRT.ech_lader_step_h, _para1_VRT.up_max_lader_h, _para1_VRT.s_par_h_divide100*100, 0);	/*MCC_SetStempMotorLine(_gp_VRT, 100,	1200,	32, 100,	15000, 0);	高速曲线*/
	MCC_SetStempMotorLine(_gp_VRT, _para1_VRT.start_fre_l, _para1_VRT.end_fre_l, _para1_VRT.ech_lader_step_l, _para1_VRT.up_max_lader_l, _para1_VRT.s_par_l_divide100*100, 1);	/*MCC_SetStempMotorLine(_gp_VRT, 100,	1200,	32, 100,	15000, 1);	低速曲线*/
	
	/*水平运动----------------------------------------------------*/
	_gp_RTT = MCC_Init(MCC_2,
						_para1_RTT.subdivision, _para1_RTT.step_angle_multiply10/10.0, _para1_RTT.ratio_multiply100/100.0, _para1_RTT.max_step,	/*细分、步距角、电机转一圈的行程、最大行程*/
						MCC_D, _para1_RTT.reset_dir, _para1_RTT.reset_status,	/*有复位信号和工位信号、撞击信号的直线机构、复位方向、复位光耦状态*/
						_para1_RTT.d_code_status, _para1_RTT.strike_status,	/*码齿遮挡、撞击未遮挡*/
						_PD_, _P14_,				/*复位信号*/
						_PC_, _P6_,				/*码齿信号*/
						_PORT_NC_, _PIN_NC_,
						_PD_,      _P5_,		/*EN*/
						_PD_,      _P6_,		/*DIR*/
						_PD_,      _P4_);		/*STEP*/
	if(_gp_RTT == NULL)	/*模块初始化失败*/
	{
		return false;
	}
	MCC_SetCompensation(_gp_RTT, _para1_RTT.reset_compensation);	/*复位补偿：整步*/
	MCC_SetPresetLocation(_gp_RTT, 0, _para2_RTT[0].en_close_cycle, _para2_RTT[0].data_multiply100/100.0, _para2_RTT[0].forward_pre_deceleration_data_multiply100/100.0, _para2_RTT[0].reverse_pre_deceleration_data_multiply100/100.0, _para2_RTT[0].forward_compensation_multiply100/100.0,_para2_RTT[0].reverse_compensation_multiply100/100.0);	/*预设位置1：搅拌1位置（2号搅R）坐标，正向预减速坐标，反向预减速坐标，正向补偿，反向补偿*/
	MCC_SetPresetLocation(_gp_RTT, 1, _para2_RTT[1].en_close_cycle, _para2_RTT[1].data_multiply100/100.0, _para2_RTT[1].forward_pre_deceleration_data_multiply100/100.0, _para2_RTT[1].reverse_pre_deceleration_data_multiply100/100.0, _para2_RTT[1].forward_compensation_multiply100/100.0,_para2_RTT[1].reverse_compensation_multiply100/100.0);/*预设位置2：搅拌2/复位位置（2号搅S）坐标，正向预减速坐标，反向预减速坐标，正向补偿，反向补偿*/
	MCC_SetPresetLocation(_gp_RTT, 2, _para2_RTT[2].en_close_cycle, _para2_RTT[2].data_multiply100/100.0, _para2_RTT[2].forward_pre_deceleration_data_multiply100/100.0, _para2_RTT[2].reverse_pre_deceleration_data_multiply100/100.0, _para2_RTT[2].forward_compensation_multiply100/100.0,_para2_RTT[2].reverse_compensation_multiply100/100.0);	/*预设位置3：搅拌3位置（1号搅R）坐标，正向预减速坐标，反向预减速坐标，正向补偿，反向补偿*/
	MCC_SetPresetLocation(_gp_RTT, 3, _para2_RTT[3].en_close_cycle, _para2_RTT[3].data_multiply100/100.0, _para2_RTT[3].forward_pre_deceleration_data_multiply100/100.0, _para2_RTT[3].reverse_pre_deceleration_data_multiply100/100.0, _para2_RTT[3].forward_compensation_multiply100/100.0,_para2_RTT[3].reverse_compensation_multiply100/100.0);	/*预设位置4：搅拌4位置（1号搅S）坐标，正向预减速坐标，反向预减速坐标，正向补偿，反向补偿*/
	MCC_SetStempMotorLine(_gp_RTT, _para1_RTT.start_fre_h, _para1_RTT.end_fre_h, _para1_RTT.ech_lader_step_h, _para1_RTT.up_max_lader_h, _para1_RTT.s_par_h_divide100*100, 0);	/*高速曲线*/				//	MCC_SetStempMotorLine(_gp_RTT, 200,	1000,	16, 90,	7000, 0);	/*高速曲线*/
	MCC_SetStempMotorLine(_gp_RTT, _para1_RTT.start_fre_l, _para1_RTT.end_fre_l, _para1_RTT.ech_lader_step_l, _para1_RTT.up_max_lader_l, _para1_RTT.s_par_l_divide100*100, 1);	/*低速曲线*/				//	MCC_SetStempMotorLine(_gp_RTT, 200,	1000,	16, 90,	7000, 1);	/*低速曲线*/

	/*清洗阀1----------------------------------------------------*/
	_gp_CleaningValve1 = Valve_Init(VALVE_3, 
									VALVE_NORMAL_CLOSE, VALVE_POSITIVE_LOGIC,	/*常闭阀、正逻辑控制*/
									_PA_, _P6_);
	if(_gp_CleaningValve1 == NULL)	/*模块初始化失败*/
	{
		return false;
	}
	
	/*清洗阀2----------------------------------------------------*/
	_gp_CleaningValve2 = Valve_Init(VALVE_4, 
									VALVE_NORMAL_CLOSE, VALVE_POSITIVE_LOGIC,	/*常闭阀、正逻辑控制*/
									_PA_, _P7_);
	if(_gp_CleaningValve2 == NULL)	/*模块初始化失败*/
	{
		return false;
	}
	
	/*搅拌1----------------------------------------------------*/
	_gp_DcMootr1 = PumpInit(PUMP_1, 
							PUMP_UNADJUSTABLE_SPEED, 
							PUMP_POSITIVE_LOGIC,
							1000,
							50,
							_T6_,
							_PA_, _P4_,
							_PORT_BUTT_, _PIN_BUTT_);
	if(_gp_DcMootr1 == NULL)	/*模块初始化失败*/
	{
		return false;
	}
	
	/*搅拌2----------------------------------------------------*/
	_gp_DcMootr2 = PumpInit(PUMP_2, 
							PUMP_UNADJUSTABLE_SPEED, 
							PUMP_POSITIVE_LOGIC,
							1000,
							50,
							_T6_,
							_PA_, _P5_,
							_PORT_BUTT_, _PIN_BUTT_);
	if(_gp_DcMootr2 == NULL)	/*模块初始化失败*/
	{
		return false;
	}
	
	return true;
}

/* 私有常驻任务接口----------------------------------------------------*/

/* 私有常驻线程1
  -----------------------------------
  由Func层实现
*/
void TaskResident1(void* p_arg)
{
		while(1)
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
	while(1)
	{
		OSTimeDlyHMSM(0, 0, 0, 500);	/*TD300搅拌模块独占一块板卡*/
	}
}

/* 私有常驻任务----------------------------------------------------*/

/*无*/

/* 私有指令任务----------------------------------------------------*/

/* 搅拌复位
  ------------------------------
  返回值：true，成功；false，失败
*/
bool Func_CmdStirReset(void* p_buffer)
{
	DRV_CAN_RX_BUFFER_TYPE*	p_buf  = (DRV_CAN_RX_BUFFER_TYPE *)p_buffer;		/*获取缓冲区数据*/
	CMD_COMMON_HEAD_TYPE*	p_msg  = (CMD_COMMON_HEAD_TYPE*)(p_buf->buffer);	/*获取消息内容*/
	CMD_STIR_RESET_TYPE*	p_data = (CMD_STIR_RESET_TYPE*)(&(p_msg->data));	/*获取数据域*/
	
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
	
	err = MCC_Reset(_gp_VRT, 5000);	/*垂直电机复位*/
	if(err != OS_ERR_NONE)	/*执行失败*/
	{
		Drv_CANProtocol_SendErrorToBuffer(can_id, serial_number, cmd, frame_id, 0xFF, 0xFF, 0x09);		/*报错，指令执行错误*/
		return false;
	}
	
	err = MCC_Reset(_gp_RTT, 5000);	/*水平电机复位*/
	if(err != OS_ERR_NONE)	/*执行失败*/
	{
		Drv_CANProtocol_SendErrorToBuffer(can_id, serial_number, cmd, frame_id, 0xFF, 0xFF, 0x09);		/*报错，指令执行错误*/
		return false;
	}
	RttNumberNow = 1;	/*水平复位位置*/
	
	if(p_data->dis_wash == 0)	/*要清洗*/
	{
		/*降到清洗位*/
		err = MCC_Move_PresetLocation(_gp_VRT,
										_gp_VRT->preset_location + 1,
										5000,
										MCC_MOVE_DISTANCE,
										0,
										MCC_LINE_AUTO);
		if(err != OS_ERR_NONE)	/*执行失败*/
		{
			Drv_CANProtocol_SendErrorToBuffer(can_id, serial_number, cmd, frame_id, 0xFF, 0xFF, 0x09);		/*报错，指令执行错误*/
			return false;
		}
		
		Valve_Open(_gp_CleaningValve1);	/*开清洗阀1*/
		PumpStart(_gp_DcMootr1);		/*开搅拌*/
		if(p_data->time != 0)	/*按设置延时*/
		{
			OSTimeDlyHMSM(0, 0, 0, p_data->time);	/*延时*/
		}
		else
		{
			OSTimeDlyHMSM(0, 0, 0, _para_STIRRING.reset_wash_time);			/*默认延时0.8s*/
		}
		Valve_Close(_gp_CleaningValve1);	/*关清洗阀1*/
		PumpStop(_gp_DcMootr1);		/*开搅拌*/
		OSTimeDlyHMSM(0, 0, 0, 10);
		
		err = MCC_Reset(_gp_VRT, 5000);	/*垂直复位*/
		if(err != OS_ERR_NONE)	/*执行成功*/
		{
			Drv_CANProtocol_SendErrorToBuffer(can_id, serial_number, cmd, frame_id, 0xFF, 0xFF, 0x09);		/*报错，指令执行错误*/
			return false;
		}
		
		DcMootrCleaning = 0;	/*1号搅拌杆已清洗*/
	}
	
	Drv_CANProtocol_SendDoneToBuffer(can_id, serial_number, cmd, frame_id);	/*直接回复执行成功*/
	return true;
}

/* 搅拌升降
  ------------------------------
  返回值：true，成功；false，失败
*/
bool Func_CmdStirUpDown(void* p_buffer)
{
	DRV_CAN_RX_BUFFER_TYPE*	p_buf  = (DRV_CAN_RX_BUFFER_TYPE *)p_buffer;		/*获取缓冲区数据*/
	CMD_COMMON_HEAD_TYPE*	p_msg  = (CMD_COMMON_HEAD_TYPE*)(p_buf->buffer);	/*获取消息内容*/
	CMD_STIR_UPDOWN_TYPE*	p_data = (CMD_STIR_UPDOWN_TYPE*)(&(p_msg->data));	/*获取数据域*/
	
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
		case 0:	/*上升*/
			err = MCC_Reset(_gp_VRT, 5000);
			break;
		
		case 1:	/*下降*/
			
			if((_gp_VRT->reset_status) != BSP_ReadPin(_gp_VRT->reset_port_number, _gp_VRT->reset_pin_number))	/*如果光耦没有在复位状态，则报错*/
			{
				Drv_CANProtocol_SendErrorToBuffer(can_id, serial_number, cmd, frame_id, 0xFF, 0xFF, 0x0A);		/*报错，初始状态错误*/
				return false;
			}
			
			err = MCC_Move_PresetLocation(_gp_VRT,
											_gp_VRT->preset_location + 1,
											5000,
											MCC_MOVE_DISTANCE,
											0,
											MCC_LINE_AUTO);
			break;
		
		default:
			Drv_CANProtocol_SendErrorToBuffer(can_id, serial_number, cmd, frame_id, 0xFF, 0xFF, 0x08);		/*报错，指令参数错误*/
			return false;			
	}
	if(err != OS_ERR_NONE)	/*执行失败*/
	{
		Drv_CANProtocol_SendErrorToBuffer(can_id, serial_number, cmd, frame_id, 0xFF, 0xFF, 0x09);		/*报错，指令执行错误*/
		return false;
	}
	
	Drv_CANProtocol_SendDoneToBuffer(can_id, serial_number, cmd, frame_id);	/*直接回复执行成功*/
	return true;
}

/* 搅拌转动
  ------------------------------
  返回值：true，成功；false，失败
*/
bool Func_CmdStirTurn(void* p_buffer)
{
	DRV_CAN_RX_BUFFER_TYPE*	p_buf  = (DRV_CAN_RX_BUFFER_TYPE *)p_buffer;		/*获取缓冲区数据*/
	CMD_COMMON_HEAD_TYPE*	p_msg  = (CMD_COMMON_HEAD_TYPE*)(p_buf->buffer);	/*获取消息内容*/
	CMD_STIR_TURN_TYPE*		p_data = (CMD_STIR_TURN_TYPE*)(&(p_msg->data));		/*获取数据域*/
	
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
		Drv_CANProtocol_SendErrorToBuffer(can_id, serial_number, cmd, frame_id, 0xFF, 0xFF, 0x0A);		/*报错，初始状态错误*/
		return false;
	}
	
	switch(p_data->place)
	{
		case 0:		/*搅拌S*/
			if(DcMootrCleaning == 0)	/*1号已经洗了*/
			{
				/*1号搅S*/
				err = MCC_Move_PresetLocation(_gp_RTT,
												_gp_RTT->preset_location + 3,	/*预设1号搅S位置*/
												5000,
												MCC_MOVE_DISTANCE,
												0,
												MCC_LINE_AUTO);
				RttNumberNow = 3;	/*水平复位位置*/
			}
			else
			{
				/*2号搅S*/
//				err = MCC_Move_PresetLocation(_gp_RTT,
//												_gp_RTT->preset_location + 1,	/*预设2号搅S位置*/
//												5000,
//												MCC_MOVE_DISTANCE,
//												0,
//												MCC_LINE_AUTO);
				err = MCC_Reset(_gp_RTT, 5000);	/*水平电机复位*/
				RttNumberNow = 1;	/*水平复位位置*/
			}
			break;
		
		case 1:		/*搅拌R*/
			if(DcMootrCleaning == 0)	/*1号已经洗了*/
			{
				/*1号搅R*/
				err = MCC_Move_PresetLocation(_gp_RTT,
												_gp_RTT->preset_location + 2,	/*预设1号搅R位置*/
												5000,
												MCC_MOVE_DISTANCE,
												0,
												MCC_LINE_AUTO);
				RttNumberNow = 2;	/*水平复位位置*/
			}
			else
			{
				/*2号搅R*/
				err = MCC_Move_PresetLocation(_gp_RTT,
												_gp_RTT->preset_location,	/*预设2号搅R位置*/
												5000,
												MCC_MOVE_DISTANCE,
												0,
												MCC_LINE_AUTO);
				RttNumberNow = 0;	/*水平复位位置*/
			}
			break;
		
		default:
			Drv_CANProtocol_SendErrorToBuffer(can_id, serial_number, cmd, frame_id, 0xFF, 0xFF, 0x08);		/*报错，指令参数错误*/
			return false;			
	}
	if(err != OS_ERR_NONE)	/*执行成功*/
	{
		Drv_CANProtocol_SendErrorToBuffer(can_id, serial_number, cmd, frame_id, 0xFF, 0xFF, 0x09);		/*报错，指令执行错误*/
		return false;
	}
	
	Drv_CANProtocol_SendDoneToBuffer(can_id, serial_number, cmd, frame_id);	/*直接回复执行成功*/
	return true;
}


/* 搅拌清洗
  ------------------------------
  返回值：true，成功；false，失败
*/
bool Func_CmdStirWash(void* p_buffer)
{
	DRV_CAN_RX_BUFFER_TYPE*	p_buf  = (DRV_CAN_RX_BUFFER_TYPE *)p_buffer;		/*获取缓冲区数据*/
	CMD_COMMON_HEAD_TYPE*	p_msg  = (CMD_COMMON_HEAD_TYPE*)(p_buf->buffer);	/*获取消息内容*/
	CMD_STIR_WASH_TYPE*		p_data = (CMD_STIR_WASH_TYPE*)(&(p_msg->data));		/*获取数据域*/
	
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
	
	/*开搅拌*/
	PumpStart(_gp_DcMootr1);
	PumpStart(_gp_DcMootr2);
	
	if(p_data->dis_wash == 0)	/*使能清洗*/
	{
		/*开清洗*/
		if(RttNumberNow == 0)	/*2号搅R -> 2号洗1号*/
		{
			Valve_Open(_gp_CleaningValve2);
			DcMootrCleaning = 0;
		}
		else if(RttNumberNow == 2)	/*1号搅R -> 2号洗2号*/
		{
			Valve_Open(_gp_CleaningValve2);
			DcMootrCleaning = 1;
		}
		else if(RttNumberNow == 1)	/*2号搅S -> 1号洗1号*/
		{
			Valve_Open(_gp_CleaningValve1);
			DcMootrCleaning = 0;
		}
		else /*if(RttNumberNow == 3)	1号搅S -> 1号洗2号*/
		{
			Valve_Open(_gp_CleaningValve1);
			DcMootrCleaning = 1;
		}
	}
	
	if(p_data->time != 0)	/*按设置延时*/
	{
		OSTimeDlyHMSM(0, 0, 0, p_data->time);	/*延时*/
	}
	else
	{
		OSTimeDlyHMSM(0, 0, 1, 0);			/*默认延时1.2s*/
	}
	PumpStop(_gp_DcMootr1);				/*关搅拌1*/
	PumpStop(_gp_DcMootr2);				/*关搅拌2*/
	
	if(p_data->time != 0)	/*按设置延时*/
	{
		OSTimeDlyHMSM(0, 0, 0, p_data->time);	/*延时*/
	}
	else
	{
		OSTimeDlyHMSM(0, 0, 0, _para_STIRRING.default_wash_time);			/*默认延时0.5s*/
	}
	
	Valve_Close(_gp_CleaningValve1);	/*关清洗阀1*/
	Valve_Close(_gp_CleaningValve2);	/*关清洗阀2*/
	OSTimeDlyHMSM(0, 0, 0, 100);
	
	Drv_CANProtocol_SendDoneToBuffer(can_id, serial_number, cmd, frame_id);	/*直接回复执行成功*/
	return true;
}

/* 搅拌
  ------------------------------
  返回值：true，成功；false，失败
*/
bool Func_CmdStir(void* p_buffer)
{
	return Func_CmdStirWash(p_buffer);
}

/* 搅拌泵阀控制
  ------------------------------
  返回值：true，成功；false，失败
*/
bool Func_CmdStirPvControl(void* p_buffer)
{
	DRV_CAN_RX_BUFFER_TYPE*		p_buf  = (DRV_CAN_RX_BUFFER_TYPE *)p_buffer;			/*获取缓冲区数据*/
	CMD_COMMON_HEAD_TYPE*		p_msg  = (CMD_COMMON_HEAD_TYPE*)(p_buf->buffer);		/*获取消息内容*/
	CMD_STIR_PVCONTROL_TYPE*	p_data = (CMD_STIR_PVCONTROL_TYPE*)(&(p_msg->data));	/*获取数据域*/
	
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
		case 1:	/*1号清洗阀*/
			if(p_data->status == 1)
				Valve_Open(_gp_CleaningValve1);
			else
				Valve_Close(_gp_CleaningValve1);
			break;
		case 2:	/*2号清洗阀*/
			if(p_data->status == 1)
				Valve_Open(_gp_CleaningValve2);
			else
				Valve_Close(_gp_CleaningValve2);
			break;
		default:
			Drv_CANProtocol_SendErrorToBuffer(can_id, serial_number, cmd, frame_id, 0xFF, 0xFF, 0x08);		/*报错，指令参数错误*/
			return false;			
	}
	OSTimeDlyHMSM(0, 0, 0, 10);
	
	Drv_CANProtocol_SendDoneToBuffer(can_id, serial_number, cmd, frame_id);	/*直接回复执行成功*/
	return true;
}
