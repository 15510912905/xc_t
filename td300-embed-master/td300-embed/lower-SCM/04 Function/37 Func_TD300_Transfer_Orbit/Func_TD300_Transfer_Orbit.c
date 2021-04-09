/********************************************************
   版权所有 (C), 2001-2100, 四川新健康成生物股份有限公司
  -------------------------------------------------------

			嵌入式开发平台——TD300轨道功能定义

  -------------------------------------------------------
   文 件 名   : Func_TD300_Transfer_Orbit.c
   版 本 号   : V1.0.0.0
   作    者   : ryc
   生成日期   : 2019年10月17日
   功能描述   : TD300轨道功能定义
   依赖于     : 
 ********************************************************/
 
#include "Func_TD300_Transfer_Orbit.h"

static DRV_MCC_TYPE* _gp_PushIn   = NULL;	/*推入运动组件*/
static DRV_MCC_TYPE* _gp_Transfer = NULL;	/*水平运动组件*/
static DRV_MCC_TYPE* _gp_PushOut  = NULL;	/*推出运动组件*/
static DRV_SWITCHINGSENSOR_TYPE* _gp_PushInSen   = NULL;	/*推入到位检测*/
static DRV_SWITCHINGSENSOR_TYPE* _gp_TransferSen = NULL;	/*水平运动到位检测*/
static DRV_SWITCHINGSENSOR_TYPE* _gp_PushOutSen  = NULL;	/*推出到位检测*/
extern RX_BUF EM1399_RX;
STRes_ShelfScan* _gp_Data;
static int8_t _g_PushInNumber = 0;		/*进样通道上架子数*/
static int8_t _g_Transfer1Number = 0;	/*进样通道上第1架的位号*/
static int8_t _g_Transfer2Number = 0;	/*进样通道上第2架的位号*/
static int8_t _g_TubeNumber = 0;        /*进样通道上对应扫描的杯号*/

//static DRV_AT24C32_TYPE* _gp_EEP = NULL;
static DRV_EM1399_TYPE* _gp_Scanner = NULL;

static MCC_PARAMETER1_TYPE _para1_PushIn = {0x11, 16, 1.8*10, 42*100, 200,
											MCC_RESET_NEGATIVE,MCC_SHELTER_NO,MCC_SHELTER_YES, MCC_SHELTER_NO,20,
											100,	300,	10, 50,	25,
											100,	300,	10, 50,	25};
static MCC_PARAMETER2_TYPE _para2_PushIn[7] = { {0x11, DISENABLE_CLOSE_CYCLE, 0.0*100, -1.0*100, 1.0*100, 1.0*100, 1.0*100},
												{0x11, DISENABLE_CLOSE_CYCLE, 3.99*100, 2.99*100, 4.99*100, 1.0*100, 1.0*100},
												{0x11, DISENABLE_CLOSE_CYCLE, ((110.88 - 3.99) / 5.0       + 3.99)*100, ((110.88 - 3.99) / 5.0       + 2.99)*100, ((110.88 - 3.99) / 5.0       + 4.99)*100, 1.0*100, 1.0*100},
												{0x11, DISENABLE_CLOSE_CYCLE, (((110.88 - 3.99) / 5.0 ) * 2.0     + 3.99)*100, (((110.88 - 3.99) / 5.0 ) * 2.0   + 2.99)*100, (((110.88 - 3.99) / 5.0 ) * 2.0    + 4.99)*100, 1.0*100, 1.0*100},
												{0x11, DISENABLE_CLOSE_CYCLE, (((110.88 - 3.99) / 5.0 ) * 3.0     + 3.99)*100, (((110.88 - 3.99) / 5.0 ) * 3.0   + 2.99)*100, (((110.88 - 3.99) / 5.0 ) * 3.0    + 4.99)*100, 1.0*100, 1.0*100},
												{0x11, DISENABLE_CLOSE_CYCLE, (((110.88 - 3.99) / 5.0 ) * 4.0     + 3.99)*100, (((110.88 - 3.99) / 5.0 ) * 4.0   + 2.99)*100, (((110.88 - 3.99) / 5.0 ) * 4.0    + 4.99)*100, 1.0*100, 1.0*100},
												{0x11, DISENABLE_CLOSE_CYCLE, (((110.88 - 3.99) / 5.0 ) * 5.0     + 3.99)*100, (((110.88 - 3.99) / 5.0 ) * 5.0   + 2.99)*100, (((110.88 - 3.99) / 5.0 ) * 5.0    + 4.99)*100, 1.0*100, 1.0*100},
											  };
static MCC_PARAMETER1_TYPE _para1_Transfer = {0x11, 16, 1.8*10, 42*100, 50,
										  MCC_RESET_NEGATIVE,MCC_SHELTER_NO,MCC_SHELTER_NO,MCC_SHELTER_YES,10,
										  100,	400,	10, 50,	25,
                                          100,	400,	10, 50,	25};

static MCC_PARAMETER2_TYPE _para2_Transfer[3] = {{0x11, DISENABLE_CLOSE_CYCLE, 0.0*100, -1.0*100, 1.0*100, 1.0*100, 1.0*100},
												{0x11, DISENABLE_CLOSE_CYCLE, 39.69*100, 38.69*100, 40.69*100, 1.0*100, 1.0*100},
												{0x11, DISENABLE_CLOSE_CYCLE, 30.55*100, 28.65*100, 31.85*100, 1.0*100, 1.0*100}
											  };

static MCC_PARAMETER1_TYPE _para1_PushOut = {0x11, 16, 1.8*10, 42*100, 200,
											 MCC_RESET_NEGATIVE,MCC_SHELTER_NO,MCC_SHELTER_YES,MCC_SHELTER_YES,10,
											 100,	100,	10, 50,	25,
											 100,	100,	10, 50,	25};

static MCC_PARAMETER2_TYPE _para2_PushOut[3] = {{0x11,DISENABLE_CLOSE_CYCLE,0.0*100, -1.0*100,  1.0*100, 1.0*100, 1.0*100},
												{0x11,DISENABLE_CLOSE_CYCLE,3.99*100, 2.99*100, 4.99*100, 1.0*100, 1.0*100},
												{0x11,DISENABLE_CLOSE_CYCLE,28.77*100, 27.77*100, 29.77*100, 1.0*100, 1.0*100}	
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
	_gp_Scanner = EM1399Init(EM1399_1,9600,_USART2_,_PA_,_P2_,_PA_,_P3_);
	_gp_Data = (STRes_ShelfScan*)calloc(1, sizeof(STRes_ShelfScan));
	/*硬件初始化*/
	Func_TD300TransferOrbitInit();
	
	m[0] = _gp_PushIn;
	m[1] = _gp_Transfer;
	m[2] = _gp_PushOut;
	
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
	
	Cmd_AddCmdNode(dlist, CMD_ORBITRESET,	Func_Cmd_OrbitReset);		/*模块复位*/
	Cmd_AddCmdNode(dlist, CMD_PUSHIN,		Func_Cmd_PushIn);			/*进架*/
	Cmd_AddCmdNode(dlist, CMD_TRANSFER,		Func_Cmd_Transfer);			/*进样*/
	Cmd_AddCmdNode(dlist, CMD_PUSHOUT,		Func_Cmd_PushOut);			/*退架*/
	Cmd_AddCmdNode(dlist, CMD_TRANSFERFREE,	Func_Cmd_TransferFree);		/*释放*/
	Cmd_AddCmdNode(dlist, CMD_TRANSFER_STEP,Func_Cmd_TransferStep);		/*进样N次*/
//	Cmd_AddCmdNode(dlist, CMD_SCAN,         Func_Cmd_Scan);             /*扫描*/
	
	
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
	return true;/*TD300轨道模块独占一个板卡*/
}

/* 初始化----------------------------------------------------*/

/* 初始化 Func_TD300TransferOrbitInit
  ------------------------------
  初始化运动组件
  返回值：true，成功；false，失败
*/
bool Func_TD300TransferOrbitInit(void)
{
	uint8_t temp = 0;
	temp = At24c32ReadByte(_gp_EEP,7*32);
	
	if(temp != 0x11)
	{
		At24c32WritePage(_gp_EEP,7,(uint8_t*)(&_para1_PushIn));
		At24c32WritePage(_gp_EEP,8,(uint8_t*)(&_para1_Transfer));
		At24c32WritePage(_gp_EEP,9,(uint8_t*)(&_para1_PushOut));
		At24c32WritePage(_gp_EEP,71,(uint8_t*)(&(_para2_PushIn[0])));
		At24c32WritePage(_gp_EEP,72,(uint8_t*)(&(_para2_PushIn[1])));
		At24c32WritePage(_gp_EEP,73,(uint8_t*)(&(_para2_PushIn[2])));
		At24c32WritePage(_gp_EEP,74,(uint8_t*)(&(_para2_PushIn[3])));
		At24c32WritePage(_gp_EEP,75,(uint8_t*)(&(_para2_PushIn[4])));
		At24c32WritePage(_gp_EEP,76,(uint8_t*)(&(_para2_PushIn[5])));
		At24c32WritePage(_gp_EEP,77,(uint8_t*)(&(_para2_PushIn[6])));
		
		At24c32WritePage(_gp_EEP,81,(uint8_t*)(&(_para2_Transfer[0])));
		At24c32WritePage(_gp_EEP,82,(uint8_t*)(&(_para2_Transfer[1])));
		At24c32WritePage(_gp_EEP,83,(uint8_t*)(&(_para2_Transfer[2])));
		
		At24c32WritePage(_gp_EEP,91,(uint8_t*)(&(_para2_PushOut[0])));
		At24c32WritePage(_gp_EEP,92,(uint8_t*)(&(_para2_PushOut[1])));
		At24c32WritePage(_gp_EEP,93,(uint8_t*)(&(_para2_PushOut[2])));
	}
	else
	{
		At24c32ReadPage(_gp_EEP,7,(uint8_t*)(&_para1_PushIn));
		At24c32ReadPage(_gp_EEP,8,(uint8_t*)(&_para1_Transfer));
		At24c32ReadPage(_gp_EEP,9,(uint8_t*)(&_para1_PushOut));
		At24c32ReadPage(_gp_EEP,71,(uint8_t*)(&(_para2_PushIn[0])));
		At24c32ReadPage(_gp_EEP,72,(uint8_t*)(&(_para2_PushIn[1])));
		At24c32ReadPage(_gp_EEP,73,(uint8_t*)(&(_para2_PushIn[2])));
		At24c32ReadPage(_gp_EEP,74,(uint8_t*)(&(_para2_PushIn[3])));
		At24c32ReadPage(_gp_EEP,75,(uint8_t*)(&(_para2_PushIn[4])));
		At24c32ReadPage(_gp_EEP,76,(uint8_t*)(&(_para2_PushIn[5])));
		At24c32ReadPage(_gp_EEP,77,(uint8_t*)(&(_para2_PushIn[6])));
		
		At24c32ReadPage(_gp_EEP,81,(uint8_t*)(&(_para2_Transfer[0])));
		At24c32ReadPage(_gp_EEP,82,(uint8_t*)(&(_para2_Transfer[1])));
		At24c32ReadPage(_gp_EEP,83,(uint8_t*)(&(_para2_Transfer[2])));
		
		At24c32ReadPage(_gp_EEP,91,(uint8_t*)(&(_para2_PushOut[0])));
		At24c32ReadPage(_gp_EEP,92,(uint8_t*)(&(_para2_PushOut[1])));
		At24c32ReadPage(_gp_EEP,93,(uint8_t*)(&(_para2_PushOut[2])));
	}
	/*推入----------------------------------------------------*/
	_gp_PushIn = MCC_Init(MCC_1,
						  _para1_PushIn.subdivision, _para1_PushIn.step_angle_multiply10/10.0, _para1_PushIn.ratio_multiply100/100.0, _para1_PushIn.max_step,	/*细分、步距角、电机转一圈的行程、最大行程*/
						  MCC_A, _para1_PushIn.reset_dir, _para1_PushIn.reset_status,	/*有复位信号和工位信号、撞击信号的直线机构、复位方向、复位光耦状态*/
						 _para1_PushIn.d_code_status, _para1_PushIn.strike_status,	/*码齿遮挡、撞击未遮挡*/
						  _PD_, _P14_,			/*复位信号*/
						  _PORT_NC_, _PIN_NC_,
						  _PC_, _P7_,
						  _PC_,      _P11_,		/*EN*/
						  _PC_,      _P12_,		/*DIR*/
						  _PC_,      _P10_);	/*STEP*/
	if(_gp_PushIn == NULL)	/*模块初始化失败*/
	{
		return false;
	}
	MCC_SetCompensation(_gp_PushIn, _para1_PushIn.reset_compensation);	/*复位补偿：整步*/
	MCC_SetPresetLocation(_gp_PushIn, 0,  _para2_PushIn[0].en_close_cycle, _para2_PushIn[0].data_multiply100/100.0, _para2_PushIn[0].forward_pre_deceleration_data_multiply100/100.0, _para2_PushIn[0].reverse_pre_deceleration_data_multiply100/100.0, _para2_PushIn[0].forward_compensation_multiply100/100.0,_para2_PushIn[0].reverse_compensation_multiply100/100.0);/*预设位置1：复位位置坐标，正向预减速坐标，反向预减速坐标，正向补偿，反向补偿*/
	MCC_SetPresetLocation(_gp_PushIn, 1,  _para2_PushIn[1].en_close_cycle, _para2_PushIn[1].data_multiply100/100.0, _para2_PushIn[1].forward_pre_deceleration_data_multiply100/100.0, _para2_PushIn[1].reverse_pre_deceleration_data_multiply100/100.0, _para2_PushIn[1].forward_compensation_multiply100/100.0,_para2_PushIn[1].reverse_compensation_multiply100/100.0);/*预设位置2：预备位置坐标，正向预减速坐标，反向预减速坐标，正向补偿，反向补偿*/
	MCC_SetPresetLocation(_gp_PushIn, 2,  _para2_PushIn[2].en_close_cycle, _para2_PushIn[2].data_multiply100/100.0, _para2_PushIn[2].forward_pre_deceleration_data_multiply100/100.0, _para2_PushIn[2].reverse_pre_deceleration_data_multiply100/100.0, _para2_PushIn[2].forward_compensation_multiply100/100.0,_para2_PushIn[2].reverse_compensation_multiply100/100.0);/*预设位置3：推出1架位置坐标，正向预减速坐标，反向预减速坐标，正向补偿，反向补偿*/
	MCC_SetPresetLocation(_gp_PushIn, 3,  _para2_PushIn[3].en_close_cycle, _para2_PushIn[3].data_multiply100/100.0, _para2_PushIn[3].forward_pre_deceleration_data_multiply100/100.0, _para2_PushIn[3].reverse_pre_deceleration_data_multiply100/100.0, _para2_PushIn[3].forward_compensation_multiply100/100.0,_para2_PushIn[3].reverse_compensation_multiply100/100.0);/*预设位置4：推出2架位置坐标，正向预减速坐标，反向预减速坐标，正向补偿，反向补偿*/
	MCC_SetPresetLocation(_gp_PushIn, 4,  _para2_PushIn[4].en_close_cycle, _para2_PushIn[4].data_multiply100/100.0, _para2_PushIn[4].forward_pre_deceleration_data_multiply100/100.0, _para2_PushIn[4].reverse_pre_deceleration_data_multiply100/100.0, _para2_PushIn[4].forward_compensation_multiply100/100.0,_para2_PushIn[4].reverse_compensation_multiply100/100.0);/*预设位置5：推出3架位置坐标，正向预减速坐标，反向预减速坐标，正向补偿，反向补偿*/
	MCC_SetPresetLocation(_gp_PushIn, 5,  _para2_PushIn[5].en_close_cycle, _para2_PushIn[5].data_multiply100/100.0, _para2_PushIn[5].forward_pre_deceleration_data_multiply100/100.0, _para2_PushIn[5].reverse_pre_deceleration_data_multiply100/100.0, _para2_PushIn[5].forward_compensation_multiply100/100.0,_para2_PushIn[5].reverse_compensation_multiply100/100.0);/*预设位置6：推出4架位置坐标，正向预减速坐标，反向预减速坐标，正向补偿，反向补偿*/
	MCC_SetPresetLocation(_gp_PushIn, 6,  _para2_PushIn[6].en_close_cycle, _para2_PushIn[6].data_multiply100/100.0, _para2_PushIn[6].forward_pre_deceleration_data_multiply100/100.0, _para2_PushIn[6].reverse_pre_deceleration_data_multiply100/100.0, _para2_PushIn[6].forward_compensation_multiply100/100.0,_para2_PushIn[6].reverse_compensation_multiply100/100.0);/*预设位置7：推出5架位置坐标，正向预减速坐标，反向预减速坐标，正向补偿，反向补偿*/
	MCC_SetStempMotorLine(_gp_PushIn,_para1_PushIn.start_fre_h, _para1_PushIn.end_fre_h, _para1_PushIn.ech_lader_step_h, _para1_PushIn.up_max_lader_h, _para1_PushIn.s_par_h_divide100*100, 0);	/*高速曲线*/	/*总行程300步*/	/*100,	800,	16, 70,	8000, 0*/
	MCC_SetStempMotorLine(_gp_PushIn,_para1_PushIn.start_fre_l, _para1_PushIn.end_fre_l, _para1_PushIn.ech_lader_step_l, _para1_PushIn.up_max_lader_l, _para1_PushIn.s_par_l_divide100*100, 1);	/*低速曲线*/					/*100,	800,	16, 70,	8000, 0*/

	/*进样----------------------------------------------------*/
	_gp_Transfer = MCC_Init(MCC_2,
						 _para1_Transfer.subdivision, _para1_Transfer.step_angle_multiply10/10.0, _para1_Transfer.ratio_multiply100/100.0, _para1_Transfer.max_step,	/*细分、步距角、电机转一圈的行程、最大行程*/
						  MCC_A, _para1_Transfer.reset_dir, _para1_Transfer.reset_status,	/*有复位信号和工位信号、撞击信号的直线机构、复位方向、复位光耦状态*/
						 _para1_Transfer.d_code_status, _para1_Transfer.strike_status,	/*码齿遮挡、撞击未遮挡*/
						_PC_, _P6_,			/*复位信号*//*PD15*/
						_PORT_NC_, _PIN_NC_,
						_PC_, _P9_,
						_PD_,      _P5_,		/*EN*/
						_PD_,      _P6_,		/*DIR*/
						_PD_,      _P4_);		/*STEP*/
	if(_gp_Transfer == NULL)	/*模块初始化失败*/
	{
		return false;
	}
	MCC_SetCompensation(_gp_Transfer, _para1_Transfer.reset_compensation);	/*复位补偿：整步*/
	MCC_SetPresetLocation(_gp_Transfer, 0, _para2_Transfer[0].en_close_cycle, _para2_Transfer[0].data_multiply100/100.0, _para2_Transfer[0].forward_pre_deceleration_data_multiply100/100.0, _para2_Transfer[0].reverse_pre_deceleration_data_multiply100/100.0, _para2_Transfer[0].forward_compensation_multiply100/100.0,_para2_Transfer[0].reverse_compensation_multiply100/100.0);	/*预设位置1：复位位置坐标，正向预减速坐标，反向预减速坐标，正向补偿，反向补偿*/
	MCC_SetPresetLocation(_gp_Transfer, 1, _para2_Transfer[1].en_close_cycle, _para2_Transfer[1].data_multiply100/100.0, _para2_Transfer[1].forward_pre_deceleration_data_multiply100/100.0, _para2_Transfer[1].reverse_pre_deceleration_data_multiply100/100.0, _para2_Transfer[1].forward_compensation_multiply100/100.0,_para2_Transfer[1].reverse_compensation_multiply100/100.0);	/*预设位置2：拉取下一位位置坐标，正向预减速坐标，反向预减速坐标，正向补偿，反向补偿*/
	MCC_SetPresetLocation(_gp_Transfer, 2, _para2_Transfer[2].en_close_cycle, _para2_Transfer[2].data_multiply100/100.0, _para2_Transfer[2].forward_pre_deceleration_data_multiply100/100.0, _para2_Transfer[2].reverse_pre_deceleration_data_multiply100/100.0, _para2_Transfer[2].forward_compensation_multiply100/100.0,_para2_Transfer[2].reverse_compensation_multiply100/100.0);	/*预设位置2：拉取下一位位置坐标，正向预减速坐标，反向预减速坐标，正向补偿，反向补偿*/
	MCC_SetStempMotorLine(_gp_Transfer, _para1_Transfer.start_fre_h, _para1_Transfer.end_fre_h, _para1_Transfer.ech_lader_step_h, _para1_Transfer.up_max_lader_h, _para1_Transfer.s_par_h_divide100*100, 0);	/*高速曲线*/	/*总行程150步*/   /*100,	800,	16, 70,	8000, 0*/
	MCC_SetStempMotorLine(_gp_Transfer, _para1_Transfer.start_fre_l, _para1_Transfer.end_fre_l, _para1_Transfer.ech_lader_step_l, _para1_Transfer.up_max_lader_l, _para1_Transfer.s_par_l_divide100*100, 1);	/*低速曲线*/                      /*100,	800,	16, 70,	8000, 0*/

	/*推出----------------------------------------------------*/
	_gp_PushOut = MCC_Init(MCC_3,
						  _para1_PushOut.subdivision, _para1_PushOut.step_angle_multiply10/10.0, _para1_PushOut.ratio_multiply100/100.0, _para1_PushOut.max_step,	/*细分、步距角、电机转一圈的行程、最大行程*/
						  MCC_A, _para1_PushOut.reset_dir, _para1_PushOut.reset_status,	/*有复位信号和工位信号、撞击信号的直线机构、复位方向、复位光耦状态*/
						  _para1_PushOut.d_code_status, _para1_PushOut.strike_status,	/*码齿遮挡、撞击未遮挡*/
						_PD_, _P15_,				/*复位信号*//*PC6*/
						_PORT_NC_, _PIN_NC_,
						_PC_, _P8_,
						_PE_,      _P0_,		/*EN*/
						_PE_,      _P1_,		/*DIR*/
						_PB_,      _P9_);		/*STEP*/
	if(_gp_PushOut == NULL)	/*模块初始化失败*/
	{
		return false;
	}
	MCC_SetCompensation(_gp_PushOut, _para1_PushOut.reset_compensation);	/*复位补偿：整步*/
	MCC_SetPresetLocation(_gp_PushOut, 0, _para2_PushOut[0].en_close_cycle, _para2_PushOut[0].data_multiply100/100.0, _para2_PushOut[0].forward_pre_deceleration_data_multiply100/100.0, _para2_PushOut[0].reverse_pre_deceleration_data_multiply100/100.0, _para2_PushOut[0].forward_compensation_multiply100/100.0,_para2_PushOut[0].reverse_compensation_multiply100/100.0);		/*预设位置1：复位位置坐标，正向预减速坐标，反向预减速坐标，正向补偿，反向补偿*/
	MCC_SetPresetLocation(_gp_PushOut, 1, _para2_PushOut[1].en_close_cycle, _para2_PushOut[1].data_multiply100/100.0, _para2_PushOut[1].forward_pre_deceleration_data_multiply100/100.0, _para2_PushOut[1].reverse_pre_deceleration_data_multiply100/100.0, _para2_PushOut[1].forward_compensation_multiply100/100.0,_para2_PushOut[1].reverse_compensation_multiply100/100.0);		/*预设位置2：预备位置坐标，正向预减速坐标，反向预减速坐标，正向补偿，反向补偿*/
	MCC_SetPresetLocation(_gp_PushOut, 2, _para2_PushOut[2].en_close_cycle, _para2_PushOut[2].data_multiply100/100.0, _para2_PushOut[2].forward_pre_deceleration_data_multiply100/100.0, _para2_PushOut[2].reverse_pre_deceleration_data_multiply100/100.0, _para2_PushOut[2].forward_compensation_multiply100/100.0,_para2_PushOut[2].reverse_compensation_multiply100/100.0);		/*预设位置3：推出1架位置坐标，正向预减速坐标，反向预减速坐标，正向补偿，反向补偿*/
	MCC_SetStempMotorLine(_gp_PushOut,  _para1_PushOut.start_fre_h, _para1_PushOut.end_fre_h, _para1_PushOut.ech_lader_step_h, _para1_PushOut.up_max_lader_h, _para1_PushOut.s_par_h_divide100*100, 0);	/*高速曲线*/		/*100,	800,	16, 70,	8000, 0*/
	MCC_SetStempMotorLine(_gp_PushOut,  _para1_PushOut.start_fre_l, _para1_PushOut.end_fre_l, _para1_PushOut.ech_lader_step_l, _para1_PushOut.up_max_lader_l, _para1_PushOut.s_par_l_divide100*100, 1);	/*低速曲线*/        /*100,	800,	16, 70,	8000, 0*/

	/*推入光耦----------------------------------------------------*/
	_gp_PushInSen = Drv_SwitchingSensorInit(SWITCHINGSENSOR_1, RISING, 50, _PC_, _P7_);    /*20191210 修改防抖时间为50ms by hyc*/
	if(_gp_PushInSen == NULL)
	{
		return false;
	}

	/*进样光耦----------------------------------------------------*/
	_gp_TransferSen = Drv_SwitchingSensorInit(SWITCHINGSENSOR_2, RISING, 10, _PC_, _P9_);
	if(_gp_TransferSen == NULL)
	{
		return false;
	}

	/*推出光耦----------------------------------------------------*/
	_gp_PushOutSen = Drv_SwitchingSensorInit(SWITCHINGSENSOR_3, RISING, 10, _PC_, _P8_);
	if(_gp_PushOutSen == NULL)
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
	Drv_AllSwitchingSensorDebounce_Loop();	/*传感器消抖*/
}

/* 私有常驻线程2
  -----------------------------------
  由Func层实现
*/
void TaskResident2(void* p_arg)
{
	while(1)
	{
		OSTimeDlyHMSM(0, 0, 0, 500);	/*TD300轨道模块独占一个板卡*/
	}
}

/* 私有常驻任务----------------------------------------------------*/

/*无*/

/* 私有指令任务----------------------------------------------------*/

/* 模块复位
  ------------------------------ 
  返回值：true，成功；false，失败
*/
bool Func_Cmd_OrbitReset(void* p_buffer)
{
	DRV_CAN_RX_BUFFER_TYPE*	p_buf  = (DRV_CAN_RX_BUFFER_TYPE *)p_buffer;		/*获取缓冲区数据*/
	CMD_COMMON_HEAD_TYPE*	p_msg  = (CMD_COMMON_HEAD_TYPE*)(p_buf->buffer);	/*获取消息内容*/
	CMD_ORBITRESET_TYPE*	p_data = (CMD_ORBITRESET_TYPE*)(&(p_msg->data));		/*获取数据域*/
	
	uint8_t  size          = p_buf->size;			/*获取指令长度*/
	uint8_t  can_id        = p_buf->can_id;			/*获取CAN ID*/
	uint8_t  serial_number = p_msg->serial_number;	/*获取流水号*/
	uint16_t cmd           = p_msg->cmd;			/*获取指令*/
	uint32_t frame_id      = p_msg->frame_id;		/*获取报文ID*/
	
	uint8_t  err           = OS_ERR_NONE;
	uint8_t  count         = 0;
	
	if(size != 3)
	{
		Drv_CANProtocol_SendErrorToBuffer(can_id, serial_number, cmd, frame_id, 0xFF, 0xFF, 0x05);		/*报错，指令长度错误*/
		return false;
	}
	
	err = MCC_Reset(_gp_PushIn, 15000);
	if(err != OS_ERR_NONE)	/*执行成功*/
	{
		Drv_CANProtocol_SendErrorToBuffer(can_id, serial_number, cmd, frame_id, 0xFF, 0xFF, 0x09);		/*报错，指令执行错误*/
		return false;
	}
	
	err = MCC_Reset(_gp_Transfer, 15000);
	if(err != OS_ERR_NONE)	/*执行成功*/
	{
		Drv_CANProtocol_SendErrorToBuffer(can_id, serial_number, cmd, frame_id, 0xFF, 0xFF, 0x09);		/*报错，指令执行错误*/
		return false;
	}
	
	err = MCC_Reset(_gp_PushOut, 15000);
	if(err != OS_ERR_NONE)	/*执行成功*/
	{
		Drv_CANProtocol_SendErrorToBuffer(can_id, serial_number, cmd, frame_id, 0xFF, 0xFF, 0x09);		/*报错，指令执行错误*/
		return false;
	}
	
	_g_PushInNumber = 0;		/*进样通道上架子数*/
	_g_Transfer1Number = 0;		/*进样通道上第1架的位号*/
	_g_Transfer2Number = 0;		/*进样通道上第2架的位号*/
	
	Drv_CANProtocol_SendDoneToBuffer(can_id, serial_number, cmd, frame_id);	/*直接回复执行成功*/	
	
	return true;
}

/* 进架
  ------------------------------ 
  返回值：true，成功；false，失败
*/
bool Func_Cmd_PushIn(void* p_buffer)
{
	DRV_CAN_RX_BUFFER_TYPE*	p_buf  = (DRV_CAN_RX_BUFFER_TYPE *)p_buffer;		/*获取缓冲区数据*/
	CMD_COMMON_HEAD_TYPE*	p_msg  = (CMD_COMMON_HEAD_TYPE*)(p_buf->buffer);	/*获取消息内容*/
	CMD_PUSHIN_TYPE*		p_data = (CMD_PUSHIN_TYPE*)(&(p_msg->data));		/*获取数据域*/
	
	uint8_t  size          = p_buf->size;			/*获取指令长度*/
	uint8_t  can_id        = p_buf->can_id;			/*获取CAN ID*/
	uint8_t  serial_number = p_msg->serial_number;	/*获取流水号*/
	uint16_t cmd           = p_msg->cmd;			/*获取指令*/
	uint32_t frame_id      = p_msg->frame_id;		/*获取报文ID*/
	
	uint8_t  err           = OS_ERR_NONE;
	uint8_t  count,count_d = 0;						/*记录推出次数*/
	uint8_t  status        = 0;						/*记录推入后状态，0没到位，1到位*/
//	STRes_ShelfScan info   = {0};					/*记录条码信息*/
	
	if(size != 3)
	{
		Drv_CANProtocol_SendErrorToBuffer(can_id, serial_number, cmd, frame_id, 0xFF, 0xFF, 0x05);		/*报错，指令长度错误*/
		return false;
	}
	
	/*先判断能否进架*/
	if(Drv_SwitchingSensorRead(_gp_PushInSen) == 0)/*能进架*/
	{
		count = 5;	/*依次推5次，直到推到位*/
		do{
			/*推一次*/
			err = MCC_Move_PresetLocation(_gp_PushIn, \
											_gp_PushIn->preset_location + 7 - count, \
											5000, \
											MCC_MOVE_DISTANCE, \
											0, \
											MCC_LINE_AUTO);	/*到预设位置1*/
			if(err != OS_ERR_NONE)	/*执行失败*/
			{
				Drv_CANProtocol_SendErrorToBuffer(can_id, serial_number, cmd, frame_id, 0xFF, 0xFF, 0x09);		/*报错，指令执行错误*/
				return false;
			}
			count--;
			OSTimeDlyHMSM(0, 0, 0, 100);	/*延时0.1s*/
		}while((Drv_SwitchingSensorRead(_gp_PushInSen) == 0) && (count > 0));	/*没到位继续推，直到推满行程*/
		
		OSTimeDlyHMSM(0, 0, 0, 100);			/*延时0.1s*/
		if((count != 0) || (Drv_SwitchingSensorRead(_gp_PushInSen) == 1))	/*记录推到位状态*/
		{
			status = 1;
			if(_g_PushInNumber == 0)	/*当前0个*/
			{
				_g_PushInNumber = 1;	/*推入了1个*/
				_g_Transfer1Number = -9;	/*记录1号样本架位置*/
				count = -_g_Transfer1Number;
				
			}
			else if(_g_PushInNumber == 1)	/*当前1个*/
			{
				_g_PushInNumber = 2;	/*推入了2个*/
				_g_Transfer2Number = -9;	/*记录2号样本架位置*/
				count = -_g_Transfer2Number;
			}
			_g_TubeNumber = 1;   /*记录杯号*/
		}
		Func_Cmd_Scan(p_buffer);    //扫描第一个样本管并保存条码
		err = MCC_Reset(_gp_PushIn, 5000);	/*复位*/
		
		
			if(count > 0)	/*向前进样*/
			{	
			do{
				/*判断能否进样*/
				OSTimeDlyHMSM(0, 0, 0, 10);			/*延时0.01s*/
				if(Drv_SwitchingSensorRead(_gp_TransferSen) == 0)	/*能进样*/
				{
					status = 0;
					
					/*往前推一次*/
					if(_g_TubeNumber == 1||_g_TubeNumber == 12)
					{
						err = MCC_Move_PresetLocation(_gp_Transfer, \
													_gp_Transfer->preset_location + 2, \
													5000, \
													MCC_MOVE_DISTANCE, \
													0, \
													MCC_LINE_AUTO);	/*到预设位置2，扫描试管架条码*/
						if(err != OS_ERR_NONE)	/*执行失败*/
						{
						Drv_CANProtocol_SendErrorToBuffer(can_id, serial_number, cmd, frame_id, 0xFF, 0xFF, 0x09);		/*报错，指令执行错误*/
						return false;
						}
						if(_g_TubeNumber == 1)
							_g_TubeNumber = 11;
						else
						{
							_g_TubeNumber = 2;
							if(_g_PushInNumber == 1)	/*有一个样本架*/
							{
								_g_Transfer1Number++;	/*记录1号样本架位置*/
							}
							else if(_g_PushInNumber == 2)	/*有两个样本架*/
							{
								_g_Transfer1Number++;	/*记录1号样本架位置*/
								_g_Transfer2Number++;	/*记录2号样本架位置*/
							}
							count--;
						}
						Func_Cmd_Scan(p_buffer);
						
					}
					else
					{
						err = MCC_Move_PresetLocation(_gp_Transfer, \
													_gp_Transfer->preset_location + 1, \
													5000, \
													MCC_MOVE_DISTANCE, \
													0, \
													MCC_LINE_AUTO);	/*到预设位置1，勾取样本架*/
						if(err != OS_ERR_NONE)	/*执行失败*/
						{
							Drv_CANProtocol_SendErrorToBuffer(can_id, serial_number, cmd, frame_id, 0xFF, 0xFF, 0x09);		/*报错，指令执行错误*/
							return false;
						}
						_g_TubeNumber+=1;
						if(_g_TubeNumber < 11)
							Func_Cmd_Scan(p_buffer);
						else
							_g_TubeNumber = 0;
						count--;
						if(_g_PushInNumber == 1)	/*有一个样本架*/
						{
						_g_Transfer1Number++;	/*记录1号样本架位置*/
						}
						else if(_g_PushInNumber == 2)	/*有两个样本架*/
						{
							_g_Transfer1Number++;	/*记录1号样本架位置*/
							_g_Transfer2Number++;	/*记录2号样本架位置*/
						}
//						Func_Cmd_Scan(p_buffer);
					}
					
					err = MCC_Reset(_gp_Transfer, 5000);	/*复位位置*/
					if(err != OS_ERR_NONE)	/*执行失败*/
					{
						Drv_CANProtocol_SendErrorToBuffer(can_id, serial_number, cmd, frame_id, 0xFF, 0xFF, 0x09);		/*报错，指令执行错误*/
						return false;
					}			
				}
//				else
//				{
//					status = 1;
//					
//					/*判断能否退架*/
//					if(Drv_SwitchingSensorRead(_gp_PushOutSen) == 0)/*能退架*/
//					{
//						/*向外推一次*/
//						err = MCC_Move_PresetLocation(_gp_PushOut, \
//														_gp_PushOut->preset_location + 2, \
//														5000, \
//														MCC_MOVE_DISTANCE, \
//														0, \
//														MCC_LINE_AUTO);	/*到预设位置1*/
//						if(err != OS_ERR_NONE)	/*执行失败*/
//						{
//							Drv_CANProtocol_SendErrorToBuffer(can_id, serial_number, cmd, frame_id, 0xFF, 0xFF, 0x09);		/*报错，指令执行错误*/
//							return false;
//						}
//							
//						status = 0;	/*恢复进样*/
//						
//						err = MCC_Reset(_gp_PushOut, 10000);	/*复位位置*/
//						if(err != OS_ERR_NONE)	/*执行失败*/
//						{
//							Drv_CANProtocol_SendErrorToBuffer(can_id, serial_number, cmd, frame_id, 0xFF, 0xFF, 0x09);		/*报错，指令执行错误*/
//							return false;
//						}				
//					}
//				}
//				if((_g_Transfer1Number > 0)&&(Drv_SwitchingSensorRead(_gp_PushInSen) == 0))/*能进架*/
//				{
//					count_d = 5;	/*依次推5次，直到推到位*/
//					do{
//						/*推一次*/
//						err = MCC_Move_PresetLocation(_gp_PushIn, \
//														_gp_PushIn->preset_location + 7 - count_d, \
//														5000, \
//														MCC_MOVE_DISTANCE, \
//														0, \
//														MCC_LINE_AUTO);	/*到预设位置1*/
//						if(err != OS_ERR_NONE)	/*执行失败*/
//						{
//							Drv_CANProtocol_SendErrorToBuffer(can_id, serial_number, cmd, frame_id, 0xFF, 0xFF, 0x09);		/*报错，指令执行错误*/
//							return false;
//						}
//						count_d--;
//						OSTimeDlyHMSM(0, 0, 0, 50);	/*延时0.05s*/
//					}while((Drv_SwitchingSensorRead(_gp_PushInSen) == 0) && (count_d > 0));	/*没到位继续推，直到推满行程*/
//					
//					OSTimeDlyHMSM(0, 0, 0, 100);			/*延时0.1s*/
//					if((count_d != 0) || (Drv_SwitchingSensorRead(_gp_PushInSen) == 1))	/*记录推到位状态*/
//					{
//						if(_g_PushInNumber == 0)	/*当前0个*/
//						{
//							_g_PushInNumber = 1;	/*推入了1个*/
//							_g_Transfer1Number = -9;	/*记录1号样本架位置*/
//						}
//						else if(_g_PushInNumber == 1)	/*当前1个*/
//						{
//							_g_PushInNumber = 2;	/*推入了2个*/
//							_g_Transfer2Number = -9;	/*记录2号样本架位置*/
//						}
//					}
//					_g_TubeNumber = 1;
//					Func_Cmd_Scan(p_buffer);
//					err = MCC_Reset(_gp_PushIn, 10000);	/*复位*/
//					if(err != OS_ERR_NONE)	/*执行失败*/
//					{
//						Drv_CANProtocol_SendErrorToBuffer(can_id, serial_number, cmd, frame_id, 0xFF, 0xFF, 0x09);		/*报错，指令执行错误*/
//						return false;
//					}
//					
//				}
			} while((status == 0) && (count > 0));	/*进样未结束*/
				
			if(count > 0)	/*未推到位*/
			{
				Drv_CANProtocol_SendErrorToBuffer(can_id, serial_number, cmd, frame_id, 0xFF, 0xFF, 0x09);		/*报错，指令执行错误*/
				return false;
			}
		}
		
		
		if(err != OS_ERR_NONE)	/*执行失败*/
		{
			Drv_CANProtocol_SendErrorToBuffer(can_id, serial_number, cmd, frame_id, 0xFF, 0xFF, 0x09);		/*报错，指令执行错误*/
			return false;
		}
		Drv_CANProtocol_SendDoneToBuffer(can_id, serial_number, cmd, frame_id);	/*回复执行成功*/	
	}
	return true;
}

/* 进样
  ------------------------------待完善
  返回值：true，成功；false，失败
*/
bool Func_Cmd_Transfer(void* p_buffer)
{
	DRV_CAN_RX_BUFFER_TYPE*	p_buf  = (DRV_CAN_RX_BUFFER_TYPE *)p_buffer;		/*获取缓冲区数据*/
	CMD_COMMON_HEAD_TYPE*	p_msg  = (CMD_COMMON_HEAD_TYPE*)(p_buf->buffer);	/*获取消息内容*/
	CMD_TRANSFER_TYPE*		p_data = (CMD_TRANSFER_TYPE*)(&(p_msg->data));		/*获取数据域*/
	
	uint8_t  size          = p_buf->size;			/*获取指令长度*/
	uint8_t  can_id        = p_buf->can_id;			/*获取CAN ID*/
	uint8_t  serial_number = p_msg->serial_number;	/*获取流水号*/
	uint16_t cmd           = p_msg->cmd;			/*获取指令*/
	uint32_t frame_id      = p_msg->frame_id;		/*获取报文ID*/
	
	uint8_t  err           = OS_ERR_NONE;
	int8_t  count         = 0;				/*进样计数*/
	int8_t  count_d       = 0;  			/*进架计数*/
	uint8_t status         = 0;				/*记录到位情况*/
		
	if(size != 3)
	{
		Drv_CANProtocol_SendErrorToBuffer(can_id, serial_number, cmd, frame_id, 0xFF, 0xFF, 0x05);		/*报错，指令长度错误*/
		return false;
	}
	
	if(_g_PushInNumber != 0)	/*有样本架才推，没有样本架直接反馈完成*/
	{
		count = -_g_Transfer1Number + p_data->number - 1;	/*计算进样次数*/
		
		if(count > 0)	/*向前进样*/
		{
			do{
				/*判断能否进样*/
				OSTimeDlyHMSM(0, 0, 0, 10);			/*延时0.01s*/
				if(Drv_SwitchingSensorRead(_gp_TransferSen) == 0)	/*能进样*/
				{
					status = 0;
					
					/*往前推一次*/
					if(_g_TubeNumber == 1||_g_TubeNumber == 12)
					{
						err = MCC_Move_PresetLocation(_gp_Transfer, \
													_gp_Transfer->preset_location + 2, \
													5000, \
													MCC_MOVE_DISTANCE, \
													0, \
													MCC_LINE_AUTO);	/*到预设位置2，扫描试管架条码*/
						if(err != OS_ERR_NONE)	/*执行失败*/
						{
						Drv_CANProtocol_SendErrorToBuffer(can_id, serial_number, cmd, frame_id, 0xFF, 0xFF, 0x09);		/*报错，指令执行错误*/
						return false;
						}
						if(_g_TubeNumber == 1)
							_g_TubeNumber = 11;
						else
						{
							_g_TubeNumber = 2;
							if(_g_PushInNumber == 1)	/*有一个样本架*/
							{
								_g_Transfer1Number++;	/*记录1号样本架位置*/
							}
							else if(_g_PushInNumber == 2)	/*有两个样本架*/
							{
								_g_Transfer1Number++;	/*记录1号样本架位置*/
								_g_Transfer2Number++;	/*记录2号样本架位置*/
							}
							count--;
						}
						Func_Cmd_Scan(p_buffer);
						
					}
					else
					{
						err = MCC_Move_PresetLocation(_gp_Transfer, \
													_gp_Transfer->preset_location + 1, \
													5000, \
													MCC_MOVE_DISTANCE, \
													0, \
													MCC_LINE_AUTO);	/*到预设位置1，勾取样本架*/
						if(err != OS_ERR_NONE)	/*执行失败*/
						{
							Drv_CANProtocol_SendErrorToBuffer(can_id, serial_number, cmd, frame_id, 0xFF, 0xFF, 0x09);		/*报错，指令执行错误*/
							return false;
						}
						_g_TubeNumber+=1;
						if(_g_TubeNumber < 11)
							Func_Cmd_Scan(p_buffer);
						else
							_g_TubeNumber = 0;
						count--;
						if(_g_PushInNumber == 1)	/*有一个样本架*/
						{
						_g_Transfer1Number++;	/*记录1号样本架位置*/
						}
						else if(_g_PushInNumber == 2)	/*有两个样本架*/
						{
							_g_Transfer1Number++;	/*记录1号样本架位置*/
							_g_Transfer2Number++;	/*记录2号样本架位置*/
						}
						Func_Cmd_Scan(p_buffer);
						}
					
					err = MCC_Reset(_gp_Transfer, 5000);	/*复位位置*/
					if(err != OS_ERR_NONE)	/*执行失败*/
					{
						Drv_CANProtocol_SendErrorToBuffer(can_id, serial_number, cmd, frame_id, 0xFF, 0xFF, 0x09);		/*报错，指令执行错误*/
						return false;
					}			
				}
				else
				{
					status = 1;
					
					/*判断能否退架*/
					if(Drv_SwitchingSensorRead(_gp_PushOutSen) == 0)/*能退架*/
					{
						/*向外推一次*/
						err = MCC_Move_PresetLocation(_gp_PushOut, \
														_gp_PushOut->preset_location + 2, \
														5000, \
														MCC_MOVE_DISTANCE, \
														0, \
														MCC_LINE_AUTO);	/*到预设位置1*/
						if(err != OS_ERR_NONE)	/*执行失败*/
						{
							Drv_CANProtocol_SendErrorToBuffer(can_id, serial_number, cmd, frame_id, 0xFF, 0xFF, 0x09);		/*报错，指令执行错误*/
							return false;
						}
							
						status = 0;	/*恢复进样*/
						
						err = MCC_Reset(_gp_PushOut, 10000);	/*复位位置*/
						if(err != OS_ERR_NONE)	/*执行失败*/
						{
							Drv_CANProtocol_SendErrorToBuffer(can_id, serial_number, cmd, frame_id, 0xFF, 0xFF, 0x09);		/*报错，指令执行错误*/
							return false;
						}				
					}
				}
				if((_g_Transfer1Number > 0)&&(Drv_SwitchingSensorRead(_gp_PushInSen) == 0))/*能进架*/
				{
					count_d = 5;	/*依次推5次，直到推到位*/
					do{
						/*推一次*/
						err = MCC_Move_PresetLocation(_gp_PushIn, \
														_gp_PushIn->preset_location + 7 - count_d, \
														5000, \
														MCC_MOVE_DISTANCE, \
														0, \
														MCC_LINE_AUTO);	/*到预设位置1*/
						if(err != OS_ERR_NONE)	/*执行失败*/
						{
							Drv_CANProtocol_SendErrorToBuffer(can_id, serial_number, cmd, frame_id, 0xFF, 0xFF, 0x09);		/*报错，指令执行错误*/
							return false;
						}
						count_d--;
						OSTimeDlyHMSM(0, 0, 0, 50);	/*延时0.05s*/
					}while((Drv_SwitchingSensorRead(_gp_PushInSen) == 0) && (count_d > 0));	/*没到位继续推，直到推满行程*/
					
					OSTimeDlyHMSM(0, 0, 0, 100);			/*延时0.1s*/
					if((count_d != 0) || (Drv_SwitchingSensorRead(_gp_PushInSen) == 1))	/*记录推到位状态*/
					{
						if(_g_PushInNumber == 0)	/*当前0个*/
						{
							_g_PushInNumber = 1;	/*推入了1个*/
							_g_Transfer1Number = -9;	/*记录1号样本架位置*/
						}
						else if(_g_PushInNumber == 1)	/*当前1个*/
						{
							_g_PushInNumber = 2;	/*推入了2个*/
							_g_Transfer2Number = -9;	/*记录2号样本架位置*/
						}
					}
					_g_TubeNumber = 1;
					Func_Cmd_Scan(p_buffer);
					err = MCC_Reset(_gp_PushIn, 10000);	/*复位*/
					if(err != OS_ERR_NONE)	/*执行失败*/
					{
						Drv_CANProtocol_SendErrorToBuffer(can_id, serial_number, cmd, frame_id, 0xFF, 0xFF, 0x09);		/*报错，指令执行错误*/
						return false;
					}
					
				}
			} while((status == 0) && (count > 0));	/*进样未结束*/
				
			if(count > 0)	/*未推到位*/
			{
				Drv_CANProtocol_SendErrorToBuffer(can_id, serial_number, cmd, frame_id, 0xFF, 0xFF, 0x09);		/*报错，指令执行错误*/
				return false;
			}
		}
		else if(count == 0)
		{
			;
		}
		else	/*不允许向反方向运动*/
		{
			Drv_CANProtocol_SendErrorToBuffer(can_id, serial_number, cmd, frame_id, 0xFF, 0xFF, 0x08);		/*报错，指令参数错误*/
			return false;
		}
	}
	
	/*反馈到位情况*/	
	Drv_CANProtocol_SendDataToBuffer(can_id, serial_number, cmd, frame_id, Drv_SwitchingSensorRead(_gp_TransferSen), 54);
	
	return true;
}

/* 退架
  ------------------------------ 
  返回值：true，成功；false，失败
*/
bool Func_Cmd_PushOut(void* p_buffer)
{
	DRV_CAN_RX_BUFFER_TYPE*	p_buf  = (DRV_CAN_RX_BUFFER_TYPE *)p_buffer;		/*获取缓冲区数据*/
	CMD_COMMON_HEAD_TYPE*	p_msg  = (CMD_COMMON_HEAD_TYPE*)(p_buf->buffer);	/*获取消息内容*/
	CMD_PUSHOUT_TYPE*		p_data = (CMD_PUSHOUT_TYPE*)(&(p_msg->data));		/*获取数据域*/
	
	uint8_t  size          = p_buf->size;			/*获取指令长度*/
	uint8_t  can_id        = p_buf->can_id;			/*获取CAN ID*/
	uint8_t  serial_number = p_msg->serial_number;	/*获取流水号*/
	uint16_t cmd           = p_msg->cmd;			/*获取指令*/
	uint32_t frame_id      = p_msg->frame_id;		/*获取报文ID*/
	
	uint8_t  err           = OS_ERR_NONE;
	uint8_t  status        = 0;	/*记录传感器状态*/
		
	if(size != 3)
	{
		Drv_CANProtocol_SendErrorToBuffer(can_id, serial_number, cmd, frame_id, 0xFF, 0xFF, 0x05);		/*报错，指令长度错误*/
		return false;
	}
	
	/*先判断能否退架*/
	OSTimeDlyHMSM(0, 0, 0, 10);			/*延时0.01s*/
	if((Drv_SwitchingSensorRead(_gp_PushOutSen) == 0) && (Drv_SwitchingSensorRead(_gp_TransferSen) == 1))/*能退架*/
	{
		status = 0;
		
		/*向外推一次*/
		err = MCC_Move_PresetLocation(_gp_PushOut, \
										_gp_PushOut->preset_location + 2, \
										5000, \
										MCC_MOVE_DISTANCE, \
										0, \
										MCC_LINE_AUTO);	/*到预设位置2*/
		if(err != OS_ERR_NONE)	/*执行失败*/
		{
			Drv_CANProtocol_SendErrorToBuffer(can_id, serial_number, cmd, frame_id, 0xFF, 0xFF, 0x09);		/*报错，指令执行错误*/
			return false;
		}
		
		if(Drv_SwitchingSensorRead(_gp_PushOutSen) != 0)/*到位否*/
		{
			status = 1;
		}
			
		err = MCC_Reset(_gp_PushOut, 10000);	/*复位位置*/
		if(err != OS_ERR_NONE)	/*执行失败*/
		{
			Drv_CANProtocol_SendErrorToBuffer(can_id, serial_number, cmd, frame_id, 0xFF, 0xFF, 0x09);		/*报错，指令执行错误*/
			return false;
		}			
	}
	
	/*反馈到位情况*/	
	Drv_CANProtocol_SendDataToBuffer(can_id, serial_number, cmd, frame_id, status, 55);
	
	return true;
}

/* 进样释放
  ------------------------------ 
  返回值：true，成功；false，失败
*/
bool Func_Cmd_TransferFree(void* p_buffer)
{
	DRV_CAN_RX_BUFFER_TYPE*	p_buf  = (DRV_CAN_RX_BUFFER_TYPE *)p_buffer;		/*获取缓冲区数据*/
	CMD_COMMON_HEAD_TYPE*	p_msg  = (CMD_COMMON_HEAD_TYPE*)(p_buf->buffer);	/*获取消息内容*/
	CMD_TRANSFERFREE_TYPE*	p_data = (CMD_TRANSFERFREE_TYPE*)(&(p_msg->data));	/*获取数据域*/
	
	uint8_t  size          = p_buf->size;			/*获取指令长度*/
	uint8_t  can_id        = p_buf->can_id;			/*获取CAN ID*/
	uint8_t  serial_number = p_msg->serial_number;	/*获取流水号*/
	uint16_t cmd           = p_msg->cmd;			/*获取指令*/
	uint32_t frame_id      = p_msg->frame_id;		/*获取报文ID*/
	uint8_t  err           = OS_ERR_NONE;
	
	uint8_t count          = 0;                     /*进样计数*/
	uint8_t status         = 0;						/*退架状态*/
	
	if(size != 3)
	{
		Drv_CANProtocol_SendErrorToBuffer(can_id, serial_number, cmd, frame_id, 0xFF, 0xFF, 0x05);		/*报错，指令长度错误*/
		return false;
	}
	
	if(_g_PushInNumber == 1)	/*当前1个*/
	{
		count = 10 - _g_Transfer1Number;											/*最大进样坐标为10*/
			do{
				if(Drv_SwitchingSensorRead(_gp_TransferSen) == 0)	/*能进样*/
				{
					/*往前推一次*/
					err = MCC_Move_PresetLocation(_gp_Transfer, \
													_gp_Transfer->preset_location + 1, \
													5000, \
													MCC_MOVE_DISTANCE, \
													0, \
													MCC_LINE_AUTO);	/*到预设位置1，勾取样本架*/
					if(err != OS_ERR_NONE)	/*执行失败*/
					{
						Drv_CANProtocol_SendErrorToBuffer(can_id, serial_number, cmd, frame_id, 0xFF, 0xFF, 0x09);		/*报错，指令执行错误*/
						return false;
					}
					err = MCC_Reset(_gp_Transfer, 5000);	/*复位位置*/
					if(err != OS_ERR_NONE)	/*执行失败*/
					{
						Drv_CANProtocol_SendErrorToBuffer(can_id, serial_number, cmd, frame_id, 0xFF, 0xFF, 0x09);		/*报错，指令执行错误*/
						return false;
					}	
					count--;
					OSTimeDlyHMSM(0, 0, 0, 100);		/*100ms延时防抖*/		
				}
					/*判断能否退架*/
				if((Drv_SwitchingSensorRead(_gp_PushOutSen) == 0) && (Drv_SwitchingSensorRead(_gp_TransferSen) == 1))/*能退架*/
				{
					/*向外推一次*/
					err = MCC_Move_PresetLocation(_gp_PushOut, \
													_gp_PushOut->preset_location + 2, \
													5000, \
													MCC_MOVE_DISTANCE, \
													0, \
													MCC_LINE_AUTO);	/*到预设位置1*/
					if(err != OS_ERR_NONE)	/*执行失败*/
					{
						Drv_CANProtocol_SendErrorToBuffer(can_id, serial_number, cmd, frame_id, 0xFF, 0xFF, 0x09);		/*报错，指令执行错误*/
						return false;
					}
					err = MCC_Reset(_gp_PushOut, 10000);	/*复位位置*/
					if(err != OS_ERR_NONE)	/*执行失败*/
					{
						Drv_CANProtocol_SendErrorToBuffer(can_id, serial_number, cmd, frame_id, 0xFF, 0xFF, 0x09);		/*报错，指令执行错误*/
						return false;
					}
					status = 1;																						/*退架完成*/
				}
			} while((count > 0)&&(status == 0));		
		_g_PushInNumber = 0;	/*推入了0个*/
		_g_Transfer1Number = 0;	/*清零1号样本架位置*/
	}
	else if(_g_PushInNumber == 2)	/*当前2个*/
	{
		_g_PushInNumber = 1;	/*推入了1个*/
		_g_Transfer1Number = _g_Transfer2Number;	/*写入1号样本架位置*/
		_g_Transfer2Number = 0;	/*清零2号样本架位置*/
						
		}
	Drv_CANProtocol_SendDoneToBuffer(can_id, serial_number, cmd, frame_id);	/*直接回复执行成功*/	
	
	return true;
}

/* 进样N位
  ------------------------------ 
  返回值：true，成功；false，失败
*/
bool Func_Cmd_TransferStep(void* p_buffer)
{
	DRV_CAN_RX_BUFFER_TYPE*	p_buf  = (DRV_CAN_RX_BUFFER_TYPE *)p_buffer;		/*获取缓冲区数据*/
	CMD_COMMON_HEAD_TYPE*	p_msg  = (CMD_COMMON_HEAD_TYPE*)(p_buf->buffer);	/*获取消息内容*/
	CMD_TRANSFER_STEP_TYPE*		p_data = (CMD_TRANSFER_STEP_TYPE*)(&(p_msg->data));		/*获取数据域*/
	
	uint8_t  size          = p_buf->size;			/*获取指令长度*/
	uint8_t  can_id        = p_buf->can_id;			/*获取CAN ID*/
	uint8_t  serial_number = p_msg->serial_number;	/*获取流水号*/
	uint16_t cmd           = p_msg->cmd;			/*获取指令*/
	uint32_t frame_id      = p_msg->frame_id;		/*获取报文ID*/
	
	uint8_t  err           = OS_ERR_NONE;
	int8_t  count         = 0;				/*进样计数*/
	uint8_t status         = 0;				/*记录到位情况*/
		
	if(size != 3)
	{
		Drv_CANProtocol_SendErrorToBuffer(can_id, serial_number, cmd, frame_id, 0xFF, 0xFF, 0x05);		/*报错，指令长度错误*/
		return false;
	}
	
	if(_g_PushInNumber != 0)	/*有样本架才推，没有样本架直接反馈完成*/
	{
		count = p_data->number;	/*计算进样次数*/
		
		do{
			/*判断能否进样*/
			OSTimeDlyHMSM(0, 0, 0, 10);			/*延时0.01s*/
			if(Drv_SwitchingSensorRead(_gp_TransferSen) == 0)	/*能进样*/
			{
				status = 0;
				
				/*往前推一次*/
				err = MCC_Move_PresetLocation(_gp_Transfer, \
												_gp_Transfer->preset_location + 1, \
												5000, \
												MCC_MOVE_DISTANCE, \
												0, \
												MCC_LINE_AUTO);	/*到预设位置1，勾取样本架*/
				if(err != OS_ERR_NONE)	/*执行失败*/
				{
					Drv_CANProtocol_SendErrorToBuffer(can_id, serial_number, cmd, frame_id, 0xFF, 0xFF, 0x09);		/*报错，指令执行错误*/
					return false;
				}
				
				count--;
			
				if(_g_PushInNumber == 1)	/*有一个样本架*/
				{
					_g_Transfer1Number++;	/*记录1号样本架位置*/
				}
				else if(_g_PushInNumber == 2)	/*有两个样本架*/
				{
					_g_Transfer1Number++;	/*记录1号样本架位置*/
					_g_Transfer2Number++;	/*记录2号样本架位置*/
				}
				
				err = MCC_Reset(_gp_Transfer, 5000);	/*复位位置*/
				if(err != OS_ERR_NONE)	/*执行失败*/
				{
					Drv_CANProtocol_SendErrorToBuffer(can_id, serial_number, cmd, frame_id, 0xFF, 0xFF, 0x09);		/*报错，指令执行错误*/
					return false;
				}			
			}
			else
			{
				status = 1;
				
				/*判断能否退架*/
				if(Drv_SwitchingSensorRead(_gp_PushOutSen) == 0)/*能退架*/
				{
					/*向外推一次*/
					err = MCC_Move_PresetLocation(_gp_PushOut, \
													_gp_PushOut->preset_location + 2, \
													5000, \
													MCC_MOVE_DISTANCE, \
													0, \
													MCC_LINE_AUTO);	/*到预设位置1*/
					if(err != OS_ERR_NONE)	/*执行失败*/
					{
						Drv_CANProtocol_SendErrorToBuffer(can_id, serial_number, cmd, frame_id, 0xFF, 0xFF, 0x09);		/*报错，指令执行错误*/
						return false;
					}
						
					status = 0;	/*恢复进样*/
					
					err = MCC_Reset(_gp_PushOut, 5000);	/*复位位置*/
					if(err != OS_ERR_NONE)	/*执行失败*/
					{
						Drv_CANProtocol_SendErrorToBuffer(can_id, serial_number, cmd, frame_id, 0xFF, 0xFF, 0x09);		/*报错，指令执行错误*/
						return false;
					}				
				}
			}
		} while((status == 0) && (count > 0));	/*进样未结束*/
			
		if(count > 0)	/*未推到位*/
		{
			Drv_CANProtocol_SendErrorToBuffer(can_id, serial_number, cmd, frame_id, 0xFF, 0xFF, 0x09);		/*报错，指令执行错误*/
			return false;
		}
	}
	
	/*反馈到位情况*/	
	Drv_CANProtocol_SendDataToBuffer(can_id, serial_number, cmd, frame_id, Drv_SwitchingSensorRead(_gp_TransferSen), 54);
	
	return true;
}
bool Func_Cmd_Scan(void* p_buffer)
{
	DRV_CAN_RX_BUFFER_TYPE*	p_buf  = (DRV_CAN_RX_BUFFER_TYPE *)p_buffer;		/*获取缓冲区数据*/
	CMD_COMMON_HEAD_TYPE*	p_msg  = (CMD_COMMON_HEAD_TYPE*)(p_buf->buffer);	/*获取消息内容*/
	CMD_TRANSFER_STEP_TYPE*		p_data = (CMD_TRANSFER_STEP_TYPE*)(&(p_msg->data));		/*获取数据域*/
	
	uint8_t  size          = p_buf->size;			/*获取指令长度*/
	uint8_t  can_id        = p_buf->can_id;			/*获取CAN ID*/
	uint8_t  serial_number = p_msg->serial_number;	/*获取流水号*/
	uint16_t cmd           = 0xff04;			/*获取指令*/
	uint32_t frame_id      = p_msg->frame_id;		/*获取报文ID*/
	uint8_t  err           = OS_ERR_NONE;
	uint8_t i;
	bool scan_err;

	EM1399_Enable(_gp_Scanner);
	scan_err = EM1399_Receive(_gp_Scanner, &EM1399_RX);

	if(_g_TubeNumber > 1)
	{	
		if(_g_TubeNumber == 11)
		{
			_g_TubeNumber = 12;
			_gp_Data->shelfNum = Char2Int(EM1399_RX.RX_Ptr,EM1399_RX.RX_LEN);
			if(_gp_Data->shelfNum == 0)
				_gp_Data->shelfNum = 0xFFFF;
//			_gp_Data->cupno = 1;
		}
		else
		{
			_gp_Data->cupno = _g_TubeNumber;
			_gp_Data->len = EM1399_RX.RX_LEN;
			for(i = 0;i < _gp_Data->len;i++)
			{	
				_gp_Data->aucbarcode[i] = EM1399_RX.RX_Ptr[i];
			}
		}
	}
	else
	{
		_gp_Data->cupno = _g_TubeNumber;
		_gp_Data->len = EM1399_RX.RX_LEN;
		for(i = 0;i < _gp_Data->len;i++)
		{	
			_gp_Data->aucbarcode[i] = EM1399_RX.RX_Ptr[i];
		}
		return true;
	}
	
	OSTimeDlyHMSM(0, 0, 0, 10);											/*待其稳定*/
	Drv_CANProtocol_Report(can_id, serial_number, cmd, frame_id,(uint8_t*)_gp_Data,_gp_Data->len + 4,46);	/*直接回复执行成功*/
	return true;
}
uint16_t Char2Int(uint8_t* data_str,uint8_t len)
{
	uint16_t temp = 0;
	uint8_t i = 0;
	if(len<1)
		return 0;
	if(len>5)
		return 0;
	for(i = 0; i < len; i++)
	{
		temp = data_str[i] - '0' + temp;
		if(i == len - 1)
			return temp;
		temp *= 10;
	}
	return temp;	
}