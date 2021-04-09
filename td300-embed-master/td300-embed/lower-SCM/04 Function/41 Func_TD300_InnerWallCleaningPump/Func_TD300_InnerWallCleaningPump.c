/********************************************************
   版权所有 (C), 2001-2100, 四川新健康成生物股份有限公司
  -------------------------------------------------------

		嵌入式开发平台——TD300内壁清洗泵功能定义

  -------------------------------------------------------
   文 件 名   : Func_TD300_InnerWallCleaningPump.c
   版 本 号   : V1.0.0.0
   作    者   : ryc
   生成日期   : 2019年12月5日
   功能描述   : TD300内壁清洗泵功能定义
   依赖于     : 
 ********************************************************/
 
#include "Func_TD300_InnerWallCleaningPump.h"

static DRV_PUMP_TYPE* _gp_InnerWallCleaningPump = NULL;	/*内壁清洗泵*/

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
	/*硬件初始化*/
	Func_TD300InnerWallCleaningPumpInit();
	
	p[6] = _gp_InnerWallCleaningPump;
	
	/*指令任务注册*/
	Cmd_AddCmdNode(dlist, CMD_COM_HELLO,			Func_Cmd_Com_Hello);				/*握手*/
//	Cmd_AddCmdNode(dlist, CMD_READ_MCC_POSITION,	Func_Cmd_Com_Read_Mcc_Position);	/*读取组件位置*/
//	Cmd_AddCmdNode(dlist, CMD_COM_MCC_RESET,		Func_Cmd_Com_Mcc_Reset);			/*组件复位*/
//	Cmd_AddCmdNode(dlist, CMD_COM_MCC_MOVE,			Func_Cmd_Com_Mcc_Move);				/*组件运动控制*/
	Cmd_AddCmdNode(dlist, CMD_COM_VALVE_CONTROL,	Func_Cmd_Com_Valve_Control);		/*电磁阀/电磁铁控制*/
//	Cmd_AddCmdNode(dlist, CMD_COM_READPIN,			Func_Cmd_Com_ReadPin);				/*读取信号*/
	Cmd_AddCmdNode(dlist, CMD_COM_PUMP_CONTROL,		Func_Cmd_Com_Pump_Control);			/*泵/直流电机控制*/
	Cmd_AddCmdNode(dlist, CMD_COM_RW_PARAMETER,		Func_Cmd_Com_RW_Parameter);			/*单个参数读写*/
	
	Cmd_AddCmdNode(dlist, CMD_AUTOWASH_PVCONTROL,	Func_CmdAutoWashPvControl);			/*自动清洗单个泵阀控制*/
	
	return true;
}

/* 初始化----------------------------------------------------*/

/* 初始化 Func_TD300InnerWallCleaningPumpInit
  ------------------------------
  初始化硬件
  返回值：true，成功；false，失败
*/
bool Func_TD300InnerWallCleaningPumpInit(void)
{	
	/*内壁清洗泵----------------------------------------------------*/
	_gp_InnerWallCleaningPump = PumpInit(PUMP_7, 
										PUMP_UNADJUSTABLE_SPEED, 
										PUMP_POSITIVE_LOGIC,		/*不调速、正逻辑控制*/
										1000, 50, _T6_,
										_PA_, _P4_,
										_PORT_BUTT_, _PIN_BUTT_);
	if(_gp_InnerWallCleaningPump == NULL)	/*模块初始化失败*/
	{
		return false;
	}
	
	return true;
}

/* 私有常驻任务接口----------------------------------------------------*/

/* 私有常驻任务----------------------------------------------------*/

/*无*/

/* 私有指令任务----------------------------------------------------*/

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
		case 7:
			if(p_data->status == 0)
				PumpStop(_gp_InnerWallCleaningPump);
			else
				PumpStart(_gp_InnerWallCleaningPump);
			break;
		default:
			Drv_CANProtocol_SendErrorToBuffer(can_id, serial_number, cmd, frame_id, 0xFF, 0xFF, 0x08);		/*报错，指令参数错误*/
			return false;
	}
	
	OSTimeDlyHMSM(0, 0, 0, 10);			/*延时*/
	
	Drv_CANProtocol_SendDoneToBuffer(can_id, serial_number, cmd, frame_id);	/*直接回复执行成功*/
	return true;
}
