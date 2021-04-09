/********************************************************
   版权所有 (C), 2001-2100, 四川新健康成生物股份有限公司
  -------------------------------------------------------

		嵌入式开发平台——TD300反应盘功能定义

  -------------------------------------------------------
   文 件 名   : Func_TD300_Reaction_Disk.c
   版 本 号   : V1.0.0.0
   作    者   : ryc
   生成日期   : 2019年10月14日
   功能描述   : TD300反应盘功能定义
   依赖于     : 
 ********************************************************/
 
#include "Func_TD300_Reaction_Disk.h"

#define DSK_NUMBER_MAX 45				/*盘的最大位号*/

static DRV_MCC_TYPE* _gp_DSK = NULL;	/*盘组件*/
extern DRV_MCC_TYPE* _gp_VRT;	/*垂直运动组件*/
//static DRV_AT24C32_TYPE* _gp_EEP = NULL;

uint16_t DskNumberNow = 0;		/*当前位号，即当前盘逻辑1号位置对应的物理位置*/

static MCC_PARAMETER1_TYPE _para1_DSK = {0x11, 16, 1.8*10, 70*100, 360,
										  MCC_RESET_POSITIVE,MCC_SHELTER_YES,MCC_SHELTER_YES, MCC_SHELTER_NO,5,
										  200,	1300,	16, 100,	40,
										  200,	1000,	16, 50,		30};
static MCC_PARAMETER2_TYPE _para2_DSK[2] = {{0x11, DISENABLE_CLOSE_CYCLE, 0.0*100, -1.0*100, 1.0*100, 1.0*100, 1.0*100},
											{0x11, ENABLE_CLOSE_CYCLE, 4.23*100, 3.00*100, 5.5*100, 0.60*100, 1.26*100}
											};
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
	Func_TD300ReactionDiskInit();
	
	m[1] = _gp_DSK;
	
	/*指令任务注册*/
	Cmd_AddCmdNode(dlist, CMD_COM_HELLO,			Func_Cmd_Com_Hello);				/*握手*/
	Cmd_AddCmdNode(dlist, CMD_READ_MCC_POSITION,	Func_Cmd_Com_Read_Mcc_Position);	/*读取组件位置*/
	Cmd_AddCmdNode(dlist, CMD_COM_MCC_RESET,		Func_Cmd_Com_Mcc_Reset);			/*组件复位*/
	Cmd_AddCmdNode(dlist, CMD_COM_MCC_MOVE,			Func_Cmd_Com_Mcc_Move);				/*组件运动控制*/
//	Cmd_AddCmdNode(dlist, CMD_COM_VALVE_CONTROL,	Func_Cmd_Com_Valve_Control);		/*电磁阀/电磁铁控制*/
	Cmd_AddCmdNode(dlist, CMD_COM_READPIN,			Func_Cmd_Com_ReadPin);				/*读取信号*/
//	Cmd_AddCmdNode(dlist, CMD_COM_PUMP_CONTROL,		Func_Cmd_Com_Pump_Control);			/*泵/直流电机控制*/
	Cmd_AddCmdNode(dlist, CMD_COM_RW_PARAMETER,		Func_Cmd_Com_RW_Parameter);			/*单个参数读写*/
	Cmd_AddCmdNode(dlist, CMD_COM_RW_PARAMETER_PAGE, Func_Cmd_Com_RW_Page_Parameter);   /*整页参数读写*/
	
	Cmd_AddCmdNode(dlist, CMD_DSK_RESET,			Func_CmdDskReset);					/*盘复位*/
	Cmd_AddCmdNode(dlist, CMD_DSK_TURN,				Func_CmdDskTurn);					/*盘旋转*/
	return true;
}

/* 初始化----------------------------------------------------*/

/* 初始化 Func_TD300ReactionDiskInit
  ------------------------------
  初始化运动组件
  返回值：true，成功；false，失败
*/
bool Func_TD300ReactionDiskInit(void)
{
	uint8_t temp = 0;
	temp = At24c32ReadByte(_gp_EEP,4*32);
	
	if(temp != 0x11)
	{
		At24c32WritePage(_gp_EEP,4,(uint8_t*)(&_para1_DSK));
		At24c32WritePage(_gp_EEP,41,(uint8_t*)(&(_para2_DSK[0])));
		At24c32WritePage(_gp_EEP,42,(uint8_t*)(&(_para2_DSK[1])));
	}
	else
	{
		At24c32ReadPage(_gp_EEP,4,(uint8_t*)(&_para1_DSK));
	    At24c32ReadPage(_gp_EEP,41,(uint8_t*)(&(_para2_DSK[0])));
	    At24c32ReadPage(_gp_EEP,42,(uint8_t*)(&(_para2_DSK[1])));
	}
	/*盘运动----------------------------------------------------*/
	_gp_DSK = MCC_Init(MCC_2,
						_para1_DSK.subdivision, _para1_DSK.step_angle_multiply10/10.0, _para1_DSK.ratio_multiply100/100.0, _para1_DSK.max_step,	/*细分、步距角、电机转一圈的行程、最大行程*/
						MCC_F, _para1_DSK.reset_dir, _para1_DSK.reset_status,	/*有复位信号和工位信号、撞击信号的直线机构、复位方向、复位光耦状态*/
						_para1_DSK.d_code_status, _para1_DSK.strike_status,	/*码齿遮挡、撞击未遮挡*/
						_PD_, _P3_,			/*复位信号*/
						_PD_, _P4_,				/*码齿信号*/
						_PORT_NC_, _PIN_NC_,
						_PE_,      _P12_,		/*EN*/
						_PE_,      _P13_,		/*DIR*/
						_PE_,      _P11_);		/*STEP*/
	if(_gp_DSK == NULL)	/*模块初始化失败*/
	{
		return false;
	}
	MCC_SetCompensation(_gp_DSK, _para1_DSK.reset_compensation);	/*复位补偿：整步*/
	MCC_SetPresetLocation(_gp_DSK, 0, _para2_DSK[0].en_close_cycle, _para2_DSK[0].data_multiply100/100.0, _para2_DSK[0].forward_pre_deceleration_data_multiply100/100.0, _para2_DSK[0].reverse_pre_deceleration_data_multiply100/100.0, _para2_DSK[0].forward_compensation_multiply100/100.0,_para2_DSK[0].reverse_compensation_multiply100/100.0);		/*预设位置1：复位位置坐标，正向预减速坐标，反向预减速坐标，正向补偿，反向补偿*//*0.0, -1.0, 1.0, 1.0, 1.0 TD300*/
	MCC_SetPresetLocation(_gp_DSK, 1, _para2_DSK[1].en_close_cycle, _para2_DSK[1].data_multiply100/100.0, _para2_DSK[1].forward_pre_deceleration_data_multiply100/100.0, _para2_DSK[1].reverse_pre_deceleration_data_multiply100/100.0, _para2_DSK[1].forward_compensation_multiply100/100.0,_para2_DSK[1].reverse_compensation_multiply100/100.0);		/*预设位置2：第一码齿坐标，正向预减速坐标，反向预减速坐标，正向补偿，反向补偿*//*9.45, 7.0, 11.55, 2.45, 2.1 TD300*/
	MCC_SetStempMotorLine(_gp_DSK, _para1_DSK.start_fre_h, _para1_DSK.end_fre_h, _para1_DSK.ech_lader_step_h, _para1_DSK.up_max_lader_h, _para1_DSK.s_par_h_divide100*100, 0);	/*高速曲线*//*300,	500,	1, 200,	10000, 0 TD300*//*100,	1500,	3, 200,	20000, 0 XC2000*/
	MCC_SetStempMotorLine(_gp_DSK, _para1_DSK.start_fre_l, _para1_DSK.end_fre_l, _para1_DSK.ech_lader_step_l, _para1_DSK.up_max_lader_l, _para1_DSK.s_par_l_divide100*100, 1);	/*低速曲线*//*300,	500,	1, 200,	10000, 1 TD300*//*500,	1500,	3, 200,	20000, 1 XC2000*/
	
	return true;
}

/* 私有常驻任务接口----------------------------------------------------*/


/* 私有常驻任务----------------------------------------------------*/

/*无*/

/* 私有指令任务----------------------------------------------------*/

/* 盘复位
  ------------------------------
  返回值：true，成功；false，失败
*/
bool Func_CmdDskReset(void* p_buffer)
{
	DRV_CAN_RX_BUFFER_TYPE*	p_buf  = (DRV_CAN_RX_BUFFER_TYPE *)p_buffer;		/*获取缓冲区数据*/
	CMD_COMMON_HEAD_TYPE*	p_msg  = (CMD_COMMON_HEAD_TYPE*)(p_buf->buffer);	/*获取消息内容*/
	CMD_DSK_RESET_TYPE*		p_data = (CMD_DSK_RESET_TYPE*)(&(p_msg->data));		/*获取数据域*/
	
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
	
	if((_gp_VRT->reset_status) != BSP_ReadPin(_gp_VRT->reset_port_number, _gp_VRT->reset_pin_number))	/*如果自动清洗复位光耦没有在复位状态，则报错*/
	{
		Drv_CANProtocol_SendErrorToBuffer(can_id, serial_number, cmd, frame_id, 0xFF, 0xFF, 0x0A);		/*报错，指令执行初始状态错误*/
		return false;
	}
	
	err = MCC_Reset(_gp_DSK, 10000);	/*盘电机复位*/
	if(err != OS_ERR_NONE)	/*执行失败*/
	{
		Drv_CANProtocol_SendErrorToBuffer(can_id, serial_number, cmd, frame_id, 0xFF, 0xFF, 0x09);		/*报错，指令执行错误*/
		return false;
	}
	DskNumberNow = 0;	/*当前位号归零*/
	
	Drv_CANProtocol_SendDoneToBuffer(can_id, serial_number, cmd, frame_id);	/*直接回复执行成功*/
	return true;
}

/* 盘转动
  ------------------------------
  返回值：true，成功；false，失败
*/
bool Func_CmdDskTurn(void* p_buffer)
{
	DRV_CAN_RX_BUFFER_TYPE*	p_buf  = (DRV_CAN_RX_BUFFER_TYPE *)p_buffer;		/*获取缓冲区数据*/
	CMD_COMMON_HEAD_TYPE*	p_msg  = (CMD_COMMON_HEAD_TYPE*)(p_buf->buffer);	/*获取消息内容*/
	CMD_DSK_TURN_TYPE*		p_data = (CMD_DSK_TURN_TYPE*)(&(p_msg->data));		/*获取数据域*/
	
	uint8_t  size          = p_buf->size;			/*获取指令长度*/
	uint8_t  can_id        = p_buf->can_id;			/*获取CAN ID*/
	uint8_t  serial_number = p_msg->serial_number;	/*获取流水号*/
	uint16_t cmd           = p_msg->cmd;			/*获取指令*/
	uint32_t frame_id      = p_msg->frame_id;		/*获取报文ID*/
	
	uint8_t  err           = OS_ERR_NONE;
	uint16_t count         = 0;	/*计算需要转的格数*/
	
	if(size != 3)			/*指令长度判断*/
	{
		Drv_CANProtocol_SendErrorToBuffer(can_id, serial_number, cmd, frame_id, 0xFF, 0xFF, 0x05);		/*报错，指令长度错误*/
		return false;
	}
	
	if((_gp_VRT->reset_status) != BSP_ReadPin(_gp_VRT->reset_port_number, _gp_VRT->reset_pin_number))	/*如果自动清洗复位光耦没有在复位状态，则报错*/
	{
		Drv_CANProtocol_SendErrorToBuffer(can_id, serial_number, cmd, frame_id, 0xFF, 0xFF, 0x0A);		/*报错，指令执行初始状态错误*/
		return false;
	}
	
	switch(p_data->type)
	{
		case 0:	/*按位号走*/
			count = abs((int16_t)(p_data->cup_number) - (int16_t)(p_data->place));
			/*先判断运动方向*/
			if(count <= 42)	/*间隔小于半圈的，逆时针转*/
			{
				err = MCC_Move_DCodeCount(_gp_DSK,
											_gp_DSK->reset_dir,
											_gp_DSK->preset_location + 1,
											5000,
											MCC_MOVE_D_CODE,
											count,
											MCC_LINE_AUTO);
				DskNumberNow = (DskNumberNow + DSK_NUMBER_MAX - count) % DSK_NUMBER_MAX;
			}
			else			/*顺时针转*/
			{
				err = MCC_Move_DCodeCount(_gp_DSK,
											!_gp_DSK->reset_dir,
											_gp_DSK->preset_location + 1,
											5000,
											MCC_MOVE_D_CODE,
											count,
											MCC_LINE_AUTO);
				DskNumberNow = (DskNumberNow + count) % DSK_NUMBER_MAX;
			}
			break;
			
		case 1:	/*顺时针按间隔走*/
			err = MCC_Move_DCodeCount(_gp_DSK,
										!_gp_DSK->reset_dir,
										_gp_DSK->preset_location + 1,
										5000,
										MCC_MOVE_D_CODE,
										p_data->cup_number,
										MCC_LINE_AUTO);
			DskNumberNow = (DskNumberNow + p_data->cup_number) % DSK_NUMBER_MAX;
			break;
		
		case 2:	/*逆时针按间隔走*/
			err = MCC_Move_DCodeCount(_gp_DSK,
										_gp_DSK->reset_dir,
										_gp_DSK->preset_location + 1,
										5000,
										MCC_MOVE_D_CODE,
										p_data->cup_number,
										MCC_LINE_AUTO);
			DskNumberNow = (DskNumberNow + DSK_NUMBER_MAX - p_data->cup_number) % DSK_NUMBER_MAX;
			break;
	}
	if(err != OS_ERR_NONE)	/*执行失败*/
	{
		Drv_CANProtocol_SendErrorToBuffer(can_id, serial_number, cmd, frame_id, 0xFF, 0xFF, 0x09);		/*报错，指令执行错误*/
		return false;
	}
	OSTimeDlyHMSM(0, 0, 0, 100);	/*等其稳定*/
	Drv_CANProtocol_SendDoneToBuffer(can_id, serial_number, cmd, frame_id);	/*直接回复执行成功*/
	return true;
}
