/********************************************************
   版权所有 (C), 2001-2100, 四川新健康成生物股份有限公司
  -------------------------------------------------------

		嵌入式开发平台——TD300加样臂功能定义

  -------------------------------------------------------
   文 件 名   : Func_TD300_Sample_Arm.c
   版 本 号   : V1.0.0.0
   作    者   : ryc
   生成日期   : 2019年10月14日
   功能描述   : TD300加样臂功能定义
   依赖于     : 
 ********************************************************/
 
#include "Func_TD300_Sample_Arm.h"

static DRV_MCC_TYPE* _gp_VRT = NULL;	/*垂直运动组件*/
static DRV_MCC_TYPE* _gp_RTT = NULL;	/*水平运动组件*/
static DRV_MCC_TYPE* _gp_INJ = NULL;	/*柱塞泵组件*/
static DRV_VALVE_TYPE* _gp_InnerWallCleaningValve = NULL;	/*内壁清洗阀*/
static DRV_VALVE_TYPE* _gp_OuterWallCleaningValve = NULL;	/*外壁清洗阀*/

static OS_EVENT* _gp_UpInhaleSemIn = NULL;		/*上升同步吸气输入信号量*/
static OS_EVENT* _gp_UpInhaleSemOut = NULL;		/*上升同步吸气输出信号量*/
static OS_EVENT* _gp_TurnInhaleSemIn = NULL;	/*旋转同步吸气输入信号量*/
static OS_EVENT* _gp_TurnInhaleSemOut = NULL;	/*旋转同步吸气输出信号量*/

uint16_t RttNumberNow = 0;		/*当前位号，即当前针旋转位置*/
double SampleLeve[18] = {0};	/*记录样本（盘）液面下降高度*/
static uint8_t _gp_InjStatus = 0;	/*吸样标志位，0未吸样，1已吸样*/
//static DRV_AT24C32_TYPE* _gp_EEP = NULL; 

static MCC_PARAMETER1_TYPE _para1_VRT = {0x11, 16, 1.8*10, 42*100, 250,
										  MCC_RESET_POSITIVE,MCC_SHELTER_YES,MCC_SHELTER_YES, MCC_SHELTER_NO,8,
										  100, 1300,  4,  40, 20,
										  100, 1300,  4,  40, 20};
static MCC_PARAMETER2_TYPE _para2_VRT[6] = {{0x11, DISENABLE_CLOSE_CYCLE, 0.0*100, -1.0*100, 1.0*100, 1.0*100, 1.0*100},
											{0x11, DISENABLE_CLOSE_CYCLE, 21.0*100, 20.0*100, 22.0*100, 1.0*100, 1.0*100},
											{0x11, ENABLE_CLOSE_CYCLE, 112.32*100, 110.55*100, 114.55*100, 0.0*100, 1.0*100},
                                            {0x11, ENABLE_CLOSE_CYCLE, 47.72*100, 46.72*100, 48.72*100, 1.0*100, 1.0*100},
										    {0x11, DISENABLE_CLOSE_CYCLE, 112.32*100, 110.55*100, 114.55*100, 1.0*100, 1.0*100},
											{0x11, DISENABLE_CLOSE_CYCLE, 112.32*100, 110.55*100, 114.55*100, 1.0*100, 1.0*100}
											};
static MCC_PARAMETER1_TYPE _para1_RTT = {0x11, 16, 1.8*10, 72*100, 180,
										  MCC_RESET_POSITIVE,MCC_SHELTER_YES,MCC_SHELTER_NO,MCC_SHELTER_YES,10,
										  100,	1500,	16, 70, 20,
                                          100,	1000,	16, 40,	15};
static MCC_PARAMETER2_TYPE _para2_RTT[7] = {{0x11, DISENABLE_CLOSE_CYCLE, -42.12*100, -44.92*100, -40.12*100, 1.0*100, 1.0*100},
											 {0x11,DISENABLE_CLOSE_CYCLE, 0.0*100, -1.0*100, 1.0*100, 1.0*100, 1.0*100},
											 {0x11,DISENABLE_CLOSE_CYCLE, 34.92*100, 32.84*100, 35.84*100, 1.0*100, 1.0*100},
											 {0x11,DISENABLE_CLOSE_CYCLE, 42.48*100, 39.24*100, 43.96*100, 1.0*100, 1.0*100},
											 {0x11,DISENABLE_CLOSE_CYCLE, 86.76*100, 85.76*100, 87.76*100,1.0*100,1.0*100},
											 {0x11,DISENABLE_CLOSE_CYCLE, 124.20*100, 123.04*100, 125.94*100,1.0*100,1.0*100},
											 {0x11,DISENABLE_CLOSE_CYCLE, 162.91*100, 161.5*100, 164.0*100,1.0*100,1.0*100}
											 };

static MCC_PARAMETER1_TYPE _para1_INJ = {0x11, 16, 1.8*10, 10*100, 100,
										 MCC_RESET_NEGATIVE,MCC_SHELTER_YES,MCC_SHELTER_YES,MCC_SHELTER_YES,10,
										 300,	1500,	16, 70, 40,
                                         300,   1000,	16, 30,	30 };
static MCC_PARAMETER2_TYPE _para2_INJ = {0x11,DISENABLE_CLOSE_CYCLE,0.0*100, -1.0*100, 1.0*100, 1.0*100, 1.0*100};
static SAMPLE_NEEDLE_PARAMETER_TYPE _para_NEEDLE = {0x11, 10.0*100, 10.0*100, 500, 500, 2.0*100, 0.15*100, 1.0*100};



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
	Func_TD300SampleArmInit();
	
	m[0] = _gp_VRT;
	m[1] = _gp_RTT;
	m[2] = _gp_INJ;
	v[2] = _gp_InnerWallCleaningValve;
	v[3] = _gp_OuterWallCleaningValve;
	
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
	
	Cmd_AddCmdNode(dlist, CMD_NDL_RESET,			Func_CmdNdlReset);					/*针复位*/
	Cmd_AddCmdNode(dlist, CMD_NDL_UP,				Func_CmdNdlUp);						/*针上升*/
	Cmd_AddCmdNode(dlist, CMD_NDL_DOWN,				Func_CmdNdlDown);					/*针下降*/
	Cmd_AddCmdNode(dlist, CMD_NDL_TURN,				Func_CmdNdlTurn);					/*针旋转*/
	Cmd_AddCmdNode(dlist, CMD_NDL_SAMPLE,			Func_CmdNdlSample);					/*针采样*/
	Cmd_AddCmdNode(dlist, CMD_NDL_BLOWOFF,			Func_CmdNdlBlowoff);				/*针排样*/
	Cmd_AddCmdNode(dlist, CMD_NDL_WASH,				Func_CmdNdlWash);					/*针清洗*/
	Cmd_AddCmdNode(dlist, CMD_NDL_WATER_UPTAKE,		Func_CmdNdlWaterUptake);			/*针吸水*/
	Cmd_AddCmdNode(dlist, CMD_NDL_WASH_VALVE_CONTROL,	Func_CmdNdlWashValveControl);	/*针清洗阀控制*/
	Cmd_AddCmdNode(dlist, CMD_NDL_AIR_OUT,			Func_CmdNdlAirOut);					/*针排气*/
	
	return true;
}

/* 初始化----------------------------------------------------*/

/* 初始化 Func_TD300SampleArmInit
  ------------------------------
  初始化运动组件和清洗阀
  返回值：true，成功；false，失败
*/
bool Func_TD300SampleArmInit(void)
{	
	
	uint8_t temp = 0;
	temp = At24c32ReadByte(_gp_EEP,1*32);

	if(temp != 0x11)
	{
		At24c32WritePage(_gp_EEP,1, (uint8_t*)(&_para1_VRT));
		At24c32WritePage(_gp_EEP,11,(uint8_t*)(&(_para2_VRT[0])));
		At24c32WritePage(_gp_EEP,12,(uint8_t*)(&(_para2_VRT[1])));
		At24c32WritePage(_gp_EEP,13,(uint8_t*)(&(_para2_VRT[2])));
		At24c32WritePage(_gp_EEP,14,(uint8_t*)(&(_para2_VRT[3])));
		At24c32WritePage(_gp_EEP,15,(uint8_t*)(&(_para2_VRT[4])));
		At24c32WritePage(_gp_EEP,16,(uint8_t*)(&(_para2_VRT[5])));
		At24c32WritePage(_gp_EEP,2, (uint8_t*)(&_para1_RTT));
		At24c32WritePage(_gp_EEP,21,(uint8_t*)(&(_para2_RTT[0])));
		At24c32WritePage(_gp_EEP,22,(uint8_t*)(&(_para2_RTT[1])));
		At24c32WritePage(_gp_EEP,23,(uint8_t*)(&(_para2_RTT[2])));
		At24c32WritePage(_gp_EEP,24,(uint8_t*)(&(_para2_RTT[3])));
		At24c32WritePage(_gp_EEP,25,(uint8_t*)(&(_para2_RTT[4])));
		At24c32WritePage(_gp_EEP,26,(uint8_t*)(&(_para2_RTT[5])));
		At24c32WritePage(_gp_EEP,27,(uint8_t*)(&(_para2_RTT[6])));
	    At24c32WritePage(_gp_EEP,3, (uint8_t*)(&_para1_INJ));
	    At24c32WritePage(_gp_EEP,31,(uint8_t*)(&_para2_INJ));
		At24c32WritePage(_gp_EEP,100, (uint8_t*)(&_para_NEEDLE));
		
	}
	else
	{
		At24c32ReadPage(_gp_EEP,1, (uint8_t*)(&_para1_VRT));
		At24c32ReadPage(_gp_EEP,11,(uint8_t*)(&(_para2_VRT[0])));
		At24c32ReadPage(_gp_EEP,12,(uint8_t*)(&(_para2_VRT[1])));
		At24c32ReadPage(_gp_EEP,13,(uint8_t*)(&(_para2_VRT[2])));
		At24c32ReadPage(_gp_EEP,14,(uint8_t*)(&(_para2_VRT[3])));
		At24c32ReadPage(_gp_EEP,15,(uint8_t*)(&(_para2_VRT[4])));
		At24c32ReadPage(_gp_EEP,16,(uint8_t*)(&(_para2_VRT[5])));
		At24c32ReadPage(_gp_EEP,2, (uint8_t*)(&_para1_RTT));
		At24c32ReadPage(_gp_EEP,21,(uint8_t*)(&(_para2_RTT[0])));
		At24c32ReadPage(_gp_EEP,22,(uint8_t*)(&(_para2_RTT[1])));
		At24c32ReadPage(_gp_EEP,23,(uint8_t*)(&(_para2_RTT[2])));
		At24c32ReadPage(_gp_EEP,24,(uint8_t*)(&(_para2_RTT[3])));
		At24c32ReadPage(_gp_EEP,25,(uint8_t*)(&(_para2_RTT[4])));
		At24c32ReadPage(_gp_EEP,26,(uint8_t*)(&(_para2_RTT[5])));
		At24c32ReadPage(_gp_EEP,27,(uint8_t*)(&(_para2_RTT[6])));
	    At24c32ReadPage(_gp_EEP,3, (uint8_t*)(&_para1_INJ));
	    At24c32ReadPage(_gp_EEP,31,(uint8_t*)(&_para2_INJ));
		At24c32ReadPage(_gp_EEP,100, (uint8_t*)(&_para_NEEDLE));		
	}
	/*垂直运动----------------------------------------------------*/
	_gp_VRT = MCC_Init(MCC_1,
						_para1_VRT.subdivision, _para1_VRT.step_angle_multiply10/10.0, _para1_VRT.ratio_multiply100/100.0, _para1_VRT.max_step,	/*细分、步距角、电机转一圈的行程、最大行程*/
						MCC_E, _para1_VRT.reset_dir, _para1_VRT.reset_status,	/*有复位信号和工位信号、撞击信号的直线机构、复位方向、复位光耦状态*/
						_para1_VRT.d_code_status, _para1_VRT.strike_status,	/*码齿遮挡、撞击未遮挡*/
						_PC_, _P6_,				/*复位信号*/
						_PC_, _P4_,				/*码齿信号*/
						_PC_, _P5_,				/*撞击信号*/
						_PC_,      _P11_,		/*EN*/
						_PC_,      _P12_,		/*DIR*/
						_PC_,      _P10_);		/*STEP*/
	if(_gp_VRT == NULL)	/*模块初始化失败*/
	{
		return false;
	}
	MCC_SetCompensation(_gp_VRT, _para1_VRT.reset_compensation);						/*复位补偿：整步 15(0227)*/
	MCC_SetPresetLocation(_gp_VRT, 0,  _para2_VRT[0].en_close_cycle, _para2_VRT[0].data_multiply100/100.0, _para2_VRT[0].forward_pre_deceleration_data_multiply100/100.0, _para2_VRT[0].reverse_pre_deceleration_data_multiply100/100.0, _para2_VRT[0].forward_compensation_multiply100/100.0,_para2_VRT[0].reverse_compensation_multiply100/100.0);	/*预设位置1ok：复位位置坐标，正向预减速坐标，反向预减速坐标，正向补偿，反向补偿*/
	MCC_SetPresetLocation(_gp_VRT, 1,  _para2_VRT[1].en_close_cycle, _para2_VRT[1].data_multiply100/100.0, _para2_VRT[1].forward_pre_deceleration_data_multiply100/100.0, _para2_VRT[1].reverse_pre_deceleration_data_multiply100/100.0, _para2_VRT[1].forward_compensation_multiply100/100.0,_para2_VRT[1].reverse_compensation_multiply100/100.0);	/*预设位置2ok：洗针位置坐标，正向预减速坐标，反向预减速坐标，正向补偿，反向补偿*/
	MCC_SetPresetLocation(_gp_VRT, 2,  _para2_VRT[2].en_close_cycle, _para2_VRT[2].data_multiply100/100.0, _para2_VRT[2].forward_pre_deceleration_data_multiply100/100.0, _para2_VRT[2].reverse_pre_deceleration_data_multiply100/100.0, _para2_VRT[2].forward_compensation_multiply100/100.0,_para2_VRT[2].reverse_compensation_multiply100/100.0);	/*预设位置3ok：吸样1（盘）垂直凌空位置坐标，正向预减速坐标，反向预减速坐标，正向补偿，反向补偿*//*15.75, 14.75, 116.55, 1.0, 1.0*//*115.55, 114.55,*/
	MCC_SetPresetLocation(_gp_VRT, 3,  _para2_VRT[3].en_close_cycle, _para2_VRT[3].data_multiply100/100.0, _para2_VRT[3].forward_pre_deceleration_data_multiply100/100.0, _para2_VRT[3].reverse_pre_deceleration_data_multiply100/100.0, _para2_VRT[3].forward_compensation_multiply100/100.0,_para2_VRT[3].reverse_compensation_multiply100/100.0);	/*预设位置4ok：排样/稀释垂直凌空位置坐标，正向预减速坐标，反向预减速坐标，正向补偿，反向补偿*//*16.8, 15.8, 48.72, 1.0, 1.0*//*16.8, 15.8*/
	MCC_SetPresetLocation(_gp_VRT, 4,  _para2_VRT[4].en_close_cycle, _para2_VRT[4].data_multiply100/100.0, _para2_VRT[4].forward_pre_deceleration_data_multiply100/100.0, _para2_VRT[4].reverse_pre_deceleration_data_multiply100/100.0, _para2_VRT[4].forward_compensation_multiply100/100.0,_para2_VRT[4].reverse_compensation_multiply100/100.0);	/*预设位置5：吸样2（轨道）垂直凌空位置坐标，正向预减速坐标，反向预减速坐标，正向补偿，反向补偿*//*31.5, 30.5, 32.5, 1.0, 1.0*/
	MCC_SetPresetLocation(_gp_VRT, 5,  _para2_VRT[5].en_close_cycle, _para2_VRT[5].data_multiply100/100.0, _para2_VRT[5].forward_pre_deceleration_data_multiply100/100.0, _para2_VRT[5].reverse_pre_deceleration_data_multiply100/100.0, _para2_VRT[5].forward_compensation_multiply100/100.0,_para2_VRT[5].reverse_compensation_multiply100/100.0);	/*预设位置6：吸样3（急诊）垂直凌空位置坐标，正向预减速坐标，反向预减速坐标，正向补偿，反向补偿*//*32.5, 31.5, 33.5, 1.0, 1.0*/
	MCC_SetStempMotorLine(_gp_VRT, _para1_VRT.start_fre_h, _para1_VRT.end_fre_h, _para1_VRT.ech_lader_step_h, _para1_VRT.up_max_lader_h, _para1_VRT.s_par_h_divide100*100, 0);	/*高速曲线*//*800,	1200,	1, 35,	10000, 0*//*总行程550步，瓶上部50，中部300*//*400,	1100,	1, 150,	10000, 0*//*200,	1200,	32, 30,		1500, 0*//*200,	1300,	7, 50,		1500, 0*/
	MCC_SetStempMotorLine(_gp_VRT, _para1_VRT.start_fre_l, _para1_VRT.end_fre_l, _para1_VRT.ech_lader_step_l, _para1_VRT.up_max_lader_l, _para1_VRT.s_par_l_divide100*100, 1);	/*低速曲线*//*200,	1300,	4,  40,		3000, 1*//*400,	500,	1, 25,	8000, 1*//*250,	1000,	16, 5,		1500, 1*//*200,	1300,	4,  40,		2000, 0*/
	
	
	/*水平旋转----------------------------------------------------*/
	_gp_RTT = MCC_Init(MCC_2,
						_para1_RTT.subdivision, _para1_RTT.step_angle_multiply10/10.0, _para1_RTT.ratio_multiply100/100.0, _para1_RTT.max_step,	/*细分、步距角、电机转一圈的行程、最大行程*/
						MCC_D, _para1_RTT.reset_dir, _para1_RTT.reset_status,	/*有复位信号和工位信号、撞击信号的直线机构、复位方向、复位光耦状态*/
						_para1_RTT.d_code_status, _para1_RTT.strike_status,	/*码齿遮挡、撞击未遮挡*/
						_PD_, _P14_,				/*复位信号*/
						_PD_, _P15_,				/*码齿信号*/
						_PORT_NC_, _PIN_NC_,
						_PD_,      _P5_,		/*EN*/
						_PD_,      _P6_,		/*DIR*/
						_PD_,      _P4_);		/*STEP*/
	if(_gp_RTT == NULL)	/*模块初始化失败*/
	{
		return false;
	}
	MCC_SetCompensation(_gp_RTT, _para1_RTT.reset_compensation);	/*复位补偿：整步*/
	MCC_SetPresetLocation(_gp_RTT, 0, _para2_RTT[0].en_close_cycle, _para2_RTT[0].data_multiply100/100.0, _para2_RTT[0].forward_pre_deceleration_data_multiply100/100.0, _para2_RTT[0].reverse_pre_deceleration_data_multiply100/100.0, _para2_RTT[0].forward_compensation_multiply100/100.0,_para2_RTT[0].reverse_compensation_multiply100/100.0);	/*预设位置1：吸样位置2（内圈R1）坐标，正向预减速坐标，反向预减速坐标，正向补偿，反向补偿	-44.14, -43.14, -42.64, 1.0, 1.0*/
	MCC_SetPresetLocation(_gp_RTT, 1, _para2_RTT[1].en_close_cycle, _para2_RTT[1].data_multiply100/100.0, _para2_RTT[1].forward_pre_deceleration_data_multiply100/100.0, _para2_RTT[1].reverse_pre_deceleration_data_multiply100/100.0, _para2_RTT[1].forward_compensation_multiply100/100.0,_para2_RTT[1].reverse_compensation_multiply100/100.0);	/*预设位置2：吸样位置1（外圈R2）坐标，正向预减速坐标，反向预减速坐标，正向补偿，反向补偿	-23.62, -22.12, -23.12, 1.0, 1.0*/	
	MCC_SetPresetLocation(_gp_RTT, 2, _para2_RTT[2].en_close_cycle, _para2_RTT[2].data_multiply100/100.0, _para2_RTT[2].forward_pre_deceleration_data_multiply100/100.0, _para2_RTT[2].reverse_pre_deceleration_data_multiply100/100.0, _para2_RTT[2].forward_compensation_multiply100/100.0,_para2_RTT[2].reverse_compensation_multiply100/100.0);	/*预设位置3：复位位置坐标，正向预减速坐标，反向预减速坐标，正向补偿，反向补偿*/
	MCC_SetPresetLocation(_gp_RTT, 3, _para2_RTT[3].en_close_cycle, _para2_RTT[3].data_multiply100/100.0, _para2_RTT[3].forward_pre_deceleration_data_multiply100/100.0, _para2_RTT[3].reverse_pre_deceleration_data_multiply100/100.0, _para2_RTT[3].forward_compensation_multiply100/100.0,_para2_RTT[3].reverse_compensation_multiply100/100.0);	/*预设位置4：排样位置1（R1）坐标，正向预减速坐标，反向预减速坐标，正向补偿，反向补偿        28.80, 27.80, 29.80, 1.0, 1.0*/
	MCC_SetPresetLocation(_gp_RTT, 4, _para2_RTT[4].en_close_cycle, _para2_RTT[4].data_multiply100/100.0, _para2_RTT[4].forward_pre_deceleration_data_multiply100/100.0, _para2_RTT[4].reverse_pre_deceleration_data_multiply100/100.0, _para2_RTT[4].forward_compensation_multiply100/100.0,_para2_RTT[4].reverse_compensation_multiply100/100.0);	/*预设位置5：排样位置2（R2）坐标，正向预减速坐标，反向预减速坐标，正向补偿，反向补偿*/
	MCC_SetPresetLocation(_gp_RTT, 5, _para2_RTT[5].en_close_cycle, _para2_RTT[5].data_multiply100/100.0, _para2_RTT[5].forward_pre_deceleration_data_multiply100/100.0, _para2_RTT[5].reverse_pre_deceleration_data_multiply100/100.0, _para2_RTT[5].forward_compensation_multiply100/100.0,_para2_RTT[5].reverse_compensation_multiply100/100.0);	/*预设位置6：吸样位置2（轨道）坐标，正向预减速坐标，反向预减速坐标，正向补偿，反向补偿*/
	MCC_SetPresetLocation(_gp_RTT, 6, _para2_RTT[6].en_close_cycle, _para2_RTT[6].data_multiply100/100.0, _para2_RTT[6].forward_pre_deceleration_data_multiply100/100.0, _para2_RTT[6].reverse_pre_deceleration_data_multiply100/100.0, _para2_RTT[6].forward_compensation_multiply100/100.0,_para2_RTT[6].reverse_compensation_multiply100/100.0);	/*预设位置7：吸样位置3（急诊）坐标，正向预减速坐标，反向预减速坐标，正向补偿，反向补偿*/
	MCC_SetStempMotorLine(_gp_RTT, _para1_RTT.start_fre_h, _para1_RTT.end_fre_h, _para1_RTT.ech_lader_step_h, _para1_RTT.up_max_lader_h, _para1_RTT.s_par_h_divide100*100, 0);	/*高速曲线*/
	MCC_SetStempMotorLine(_gp_RTT, _para1_RTT.start_fre_l, _para1_RTT.end_fre_l, _para1_RTT.ech_lader_step_l, _para1_RTT.up_max_lader_l, _para1_RTT.s_par_l_divide100*100, 1);	/*低速曲线*/

	/*柱塞泵----------------------------------------------------*/
	_gp_INJ = MCC_Init(MCC_3,
						_para1_INJ.subdivision, _para1_INJ.step_angle_multiply10/10.0, _para1_INJ.ratio_multiply100/100.0, _para1_RTT.max_step,	/*细分、步距角、电机转一圈的行程、最大行程*/
						MCC_A, _para1_INJ.reset_dir, _para1_INJ.reset_status,	/*有复位信号和工位信号、撞击信号的直线机构、复位方向、复位光耦状态*/
						_para1_INJ.d_code_status, _para1_INJ.strike_status,	/*码齿遮挡、撞击未遮挡*/
						_PC_, _P7_,				/*复位信号*/
						_PORT_NC_, _PIN_NC_,
						_PORT_NC_, _PIN_NC_,
						_PE_,      _P0_,		/*EN*/
						_PE_,      _P1_,		/*DIR*/
						_PB_,      _P9_);		/*STEP*/
	if(_gp_INJ == NULL)	/*模块初始化失败*/
	{
		return false;
	}
	MCC_SetCompensation(_gp_INJ, _para1_INJ.reset_compensation);	/*复位补偿：默认10整步*/
	MCC_SetPresetLocation(_gp_INJ, 0, _para2_INJ.en_close_cycle, _para2_INJ.data_multiply100/100.0, _para2_INJ.forward_pre_deceleration_data_multiply100/100.0, _para2_INJ.reverse_pre_deceleration_data_multiply100/100.0, _para2_INJ.forward_compensation_multiply100/100.0,_para2_INJ.reverse_compensation_multiply100/100.0);	/*预设位置1：复位位置坐标，正向预减速坐标，反向预减速坐标，正向补偿，反向补偿*/
	MCC_SetStempMotorLine(_gp_INJ, _para1_INJ.start_fre_h, _para1_INJ.end_fre_h, _para1_INJ.ech_lader_step_h, _para1_INJ.up_max_lader_h, _para1_INJ.s_par_h_divide100*100, 0);	/*高速曲线*/
	MCC_SetStempMotorLine(_gp_INJ, _para1_INJ.start_fre_l, _para1_INJ.end_fre_l, _para1_INJ.ech_lader_step_l, _para1_INJ.up_max_lader_l, _para1_INJ.s_par_l_divide100*100, 1);	/*低速曲线*/
	
	
	/*内壁清洗阀----------------------------------------------------*/
	_gp_InnerWallCleaningValve = Valve_Init(VALVE_1, 
											VALVE_NORMAL_CLOSE, VALVE_POSITIVE_LOGIC,		/*常闭阀、正逻辑控制*/
											_PA_, _P6_);
	if(_gp_InnerWallCleaningValve == NULL)	/*模块初始化失败*/
	{
		return false;
	}
	
	/*外壁清洗阀----------------------------------------------------*/
	_gp_OuterWallCleaningValve = Valve_Init(VALVE_2, 
											VALVE_NORMAL_CLOSE, VALVE_POSITIVE_LOGIC,		/*常闭阀、正逻辑控制*/
											_PA_, _P7_);
	if(_gp_OuterWallCleaningValve == NULL)	/*模块初始化失败*/
	{
		return false;
	}
	
	/*---------------------------------------------------------*/
	_gp_UpInhaleSemIn = OSSemCreate(0);		/*上升同步吸气输入信号量*/
	_gp_UpInhaleSemOut = OSSemCreate(0);	/*上升同步吸气输出信号量*/
	_gp_TurnInhaleSemIn = OSSemCreate(0);	/*旋转同步吸气输入信号量*/
	_gp_TurnInhaleSemOut = OSSemCreate(0);	/*旋转同步吸气输出信号量*/
	
	return true;
}

/* 私有常驻任务接口----------------------------------------------------*/

/* 私有常驻线程1
  -----------------------------------
  由Func层实现，上升吸空气
*/
void TaskResident1(void* p_arg)
{
	uint8_t err = OS_ERR_NONE;
	
	while(1)
	{
		OSSemPend(_gp_UpInhaleSemIn, 0, &err);
		
		OSTimeDlyHMSM(0, 0, 0, 100);	/*延时100ms*/
		
		err = MCC_Move_Distance(_gp_INJ,
								!_gp_INJ->reset_dir,
								(double)_para_NEEDLE.up_inhaled_air_multiply100/100.0,				/*单位uL，吸10uL空气*/
								5000,
								MCC_MOVE_DISTANCE,
								0,
								MCC_LINE_AUTO);
								
		OSSemPost(_gp_UpInhaleSemOut);
	}
}

/* 私有常驻线程2
  -----------------------------------
  由Func层实现，旋转吸空气
*/
void TaskResident2(void* p_arg)
{
	uint8_t err = OS_ERR_NONE;
	
	while(1)
	{
		OSSemPend(_gp_TurnInhaleSemIn, 0, &err);
				
		err = MCC_Move_Distance(_gp_INJ,
								!_gp_INJ->reset_dir,
								(double)_para_NEEDLE.rotate_inhaled_air_multiply100/100.0,				/*单位uL，吸10uL空气*/
								5000,
								MCC_MOVE_DISTANCE,
								0,
								MCC_LINE_AUTO);
								
		OSSemPost(_gp_TurnInhaleSemOut);
	}
}

/* 私有常驻任务----------------------------------------------------*/

/*无*/

/* 私有指令任务----------------------------------------------------*/

/* 针复位
  ------------------------------
  返回值：true，成功；false，失败
*/
bool Func_CmdNdlReset(void* p_buffer)
{
	DRV_CAN_RX_BUFFER_TYPE*	p_buf  = (DRV_CAN_RX_BUFFER_TYPE *)p_buffer;		/*获取缓冲区数据*/
	CMD_COMMON_HEAD_TYPE*	p_msg  = (CMD_COMMON_HEAD_TYPE*)(p_buf->buffer);	/*获取消息内容*/
	CMD_NDL_RESET_TYPE*		p_data = (CMD_NDL_RESET_TYPE*)(&(p_msg->data));		/*获取数据域*/
	
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
	
	err = MCC_Reset(_gp_VRT, 10000);	/*垂直电机复位*/
	if(err != OS_ERR_NONE)	/*执行失败*/
	{
		Drv_CANProtocol_SendErrorToBuffer(can_id, serial_number, cmd, frame_id, 0xFF, 0xFF, 0x09);		/*报错，指令执行错误*/
		return false;
	}
	
	err = MCC_Reset(_gp_RTT, 10000);	/*水平电机复位*/
	if(err != OS_ERR_NONE)	/*执行失败*/
	{
		Drv_CANProtocol_SendErrorToBuffer(can_id, serial_number, cmd, frame_id, 0xFF, 0xFF, 0x09);		/*报错，指令执行错误*/
		return false;
	}
	RttNumberNow = 1;	/*针在复位位置*/
	
	err = MCC_Reset(_gp_INJ, 10000);	/*柱塞泵电机复位*/
	if(err != OS_ERR_NONE)	/*执行失败*/
	{
		Drv_CANProtocol_SendErrorToBuffer(can_id, serial_number, cmd, frame_id, 0xFF, 0xFF, 0x09);		/*报错，指令执行错误*/
		return false;
	}
	
	if(p_data->dis_wash == 0)	/*要清洗*/
	{
//		err = MCC_Move_PresetLocation(_gp_VRT,
//										_gp_VRT->preset_location + 1,	/*按预设位置下降到清洗位*/
//										5000,
//										MCC_MOVE_DISTANCE,
//										0,
//										MCC_LINE_AUTO);
//		if(err != OS_ERR_NONE)	/*执行失败*/
//		{
//			Drv_CANProtocol_SendErrorToBuffer(can_id, serial_number, cmd, frame_id, 0xFF, 0xFF, 0x09);		/*报错，指令执行错误*/
//			return false;
//		}
		
		Valve_Open(_gp_InnerWallCleaningValve);	/*开内壁清洗阀*/
		Valve_Open(_gp_OuterWallCleaningValve);	/*开外壁清洗阀*/
		if(p_data->time != 0)	/*按设置延时*/
		{
			OSTimeDlyHMSM(0, 0, 0, p_data->time);	/*延时*/
		}
		else
		{
			OSTimeDlyHMSM(0, 0, 0, _para_NEEDLE.reset_wash_time);			/*默认延时0.5s*/
		}
		Valve_Close(_gp_InnerWallCleaningValve);	/*关内壁清洗阀*/
		Valve_Close(_gp_OuterWallCleaningValve);	/*关外壁清洗阀*/
		OSTimeDlyHMSM(0, 0, 0, 10);
		
//		err = MCC_Reset(_gp_VRT, 5000);	/*垂直电机复位*/
//		if(err != OS_ERR_NONE)	/*执行失败*/
//		{
//			Drv_CANProtocol_SendErrorToBuffer(can_id, serial_number, cmd, frame_id, 0xFF, 0xFF, 0x09);		/*报错，指令执行错误*/
//			return false;
//		}
	}
	
	Drv_CANProtocol_SendDoneToBuffer(can_id, serial_number, cmd, frame_id);	/*直接回复执行成功*/
	return true;
}

/* 针上升
  ------------------------------
  返回值：true，成功；false，失败
*/
bool Func_CmdNdlUp(void* p_buffer)
{
	DRV_CAN_RX_BUFFER_TYPE*	p_buf  = (DRV_CAN_RX_BUFFER_TYPE *)p_buffer;		/*获取缓冲区数据*/
	CMD_COMMON_HEAD_TYPE*	p_msg  = (CMD_COMMON_HEAD_TYPE*)(p_buf->buffer);	/*获取消息内容*/
	CMD_NDL_UP_TYPE*		p_data = (CMD_NDL_UP_TYPE*)(&(p_msg->data));		/*获取数据域*/
	
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
	
	switch(RttNumberNow)	/*排样后上升要吸空气*/
	{
		case 0: /*吸样位（盘）*/
		case 2:	/*稀释位*/
		case 3:	/*排样位*/
		case 4: /*吸样位（轨道）*/
		case 5: /*吸样位（急诊）*/
			if(_gp_InjStatus == 0)	/*已经排样*/
				OSSemPost(_gp_UpInhaleSemIn);	/*启动同步吸空气*/
		break;
	}
	
	err = MCC_Move_SynchronizingStep(_gp_VRT, \
									_gp_VRT->reset_dir, \
									10, \
									5000, \
									MCC_MOVE_DISTANCE, \
									0, \
									MCC_LINE_SLOW);	/*匀速脱离*/
	if(err != OS_ERR_NONE)	/*执行失败*/
	{
		Drv_CANProtocol_SendErrorToBuffer(can_id, serial_number, cmd, frame_id, 0xFF, 0xFF, 0x09);		/*报错，指令执行错误*/
		return false;
	}
	err = MCC_Reset(_gp_VRT, 10000);	/*垂直电机复位*/
	if(err != OS_ERR_NONE)	/*执行失败*/
	{
		Drv_CANProtocol_SendErrorToBuffer(can_id, serial_number, cmd, frame_id, 0xFF, 0xFF, 0x09);		/*报错，指令执行错误*/
		return false;
	}
	
	
	switch(RttNumberNow)	/*排样后上升要吸空气*/
	{
		case 0: /*吸样位（盘）*/
		case 2:	/*稀释位*/
		case 3:	/*排样位*/
		case 4: /*吸样位（轨道）*/
		case 5: /*吸样位（急诊）*/
			if(_gp_InjStatus == 0)	/*已经排样*/
			{
				OSSemPend(_gp_UpInhaleSemOut, 5000, &err);	/*等待同步吸空气完成*/
				if(err != OS_ERR_NONE)	/*执行失败*/
				{
					Drv_CANProtocol_SendErrorToBuffer(can_id, serial_number, cmd, frame_id, 0xFF, 0xFF, 0x09);		/*报错，指令执行错误*/
					return false;
				}
			}
		break;
	}
		
	Drv_CANProtocol_SendDoneToBuffer(can_id, serial_number, cmd, frame_id);	/*直接回复执行成功*/
	return true;
}

/* 针下降
  ------------------------------ 待完善
  返回值：true，成功；false，失败
*/
bool Func_CmdNdlDown(void* p_buffer)
{
	DRV_CAN_RX_BUFFER_TYPE*	p_buf  = (DRV_CAN_RX_BUFFER_TYPE *)p_buffer;		/*获取缓冲区数据*/
	CMD_COMMON_HEAD_TYPE*	p_msg  = (CMD_COMMON_HEAD_TYPE*)(p_buf->buffer);	/*获取消息内容*/
	CMD_NDL_DOWN_TYPE*		p_data = (CMD_NDL_DOWN_TYPE*)(&(p_msg->data));		/*获取数据域*/
	
	uint8_t  size          = p_buf->size;			/*获取指令长度*/
	uint8_t  can_id        = p_buf->can_id;			/*获取CAN ID*/
	uint8_t  serial_number = p_msg->serial_number;	/*获取流水号*/
	uint16_t cmd           = p_msg->cmd;			/*获取指令*/
	uint32_t frame_id      = p_msg->frame_id;		/*获取报文ID*/
	
	uint8_t  err           = OS_ERR_NONE;
	uint32_t plase         = 0;		/*计算当前位置*/
	uint8_t  datas[11]	   = {0};	/*打包回传数据*/
	
	if((size != 4) && (size != 3))			/*指令长度判断*/
	{
		Drv_CANProtocol_SendErrorToBuffer(can_id, serial_number, cmd, frame_id, 0xFF, 0xFF, 0x05);		/*报错，指令长度错误*/
		return false;
	}
	
	if((_gp_VRT->reset_status) != BSP_ReadPin(_gp_VRT->reset_port_number, _gp_VRT->reset_pin_number))	/*如果光耦没有在复位状态，则报错*/
	{
		Drv_CANProtocol_SendErrorToBuffer(can_id, serial_number, cmd, frame_id, 0xFF, 0xFF, 0x0A);		/*报错，初始状态错误*/
		return false;
	}
	
	if(p_data->en_liquid_level_measuring == 1)	/*要进行液面检测*/
	{
		if(RttNumberNow == 0)	/*针在吸样位置（盘）*/
		{
			err = MCC_Move_PresetLocation(_gp_VRT,
											_gp_VRT->preset_location + 2,	/*按预设吸样位置下降*/
											5000,
											MCC_MOVE_D_CODE,
											MCC_GetDCodeInterval(_gp_VRT, MCC_CalcSubdivisionStep(_gp_VRT, (_gp_VRT->preset_location + 1)->data)),
											MCC_LINE_AUTO);
		}
		else if((RttNumberNow == 2) || (RttNumberNow == 3))	/*针在排样位置*/
		{
			err = MCC_Move_PresetLocation(_gp_VRT,
											_gp_VRT->preset_location + 3,	/*按预设排样位置下降*/
											5000,
											MCC_MOVE_D_CODE,
											MCC_GetDCodeInterval(_gp_VRT, MCC_CalcSubdivisionStep(_gp_VRT, (_gp_VRT->preset_location + 1)->data)),
											MCC_LINE_AUTO);
		}
		else if(RttNumberNow == 4)	/*针在吸样位置（轨道）*/
		{
			err = MCC_Move_PresetLocation(_gp_VRT,
											_gp_VRT->preset_location + 4,	/*按预设排样位置下降*/
											5000,
											MCC_MOVE_D_CODE,
											MCC_GetDCodeInterval(_gp_VRT, MCC_CalcSubdivisionStep(_gp_VRT, (_gp_VRT->preset_location + 1)->data)),
											MCC_LINE_AUTO);
		}
		else if(RttNumberNow == 5)	/*针在吸样位置（急诊）*/
		{
			err = MCC_Move_PresetLocation(_gp_VRT,
											_gp_VRT->preset_location + 5,	/*按预设排样位置下降*/
											5000,
											MCC_MOVE_D_CODE,
											MCC_GetDCodeInterval(_gp_VRT, MCC_CalcSubdivisionStep(_gp_VRT, (_gp_VRT->preset_location + 1)->data)),
											MCC_LINE_AUTO);
		}
		else
		{
			Drv_CANProtocol_SendErrorToBuffer(can_id, serial_number, cmd, frame_id, 0xFF, 0xFF, 0x08);		/*报错，指令参数错误*/
			return false;
		}
	}
	else	/*不进行液面检测*/
	{
		if(RttNumberNow == 0)	/*针在吸样位置（盘），强制液面检测*/
		{
			err = MCC_Move_PresetLocation(_gp_VRT,
											_gp_VRT->preset_location + 2,	/*按预设吸样位置下降*/
											5000,
											MCC_MOVE_D_CODE,
											MCC_GetDCodeInterval(_gp_VRT, MCC_CalcSubdivisionStep(_gp_VRT, (_gp_VRT->preset_location + 1)->data)),
											MCC_LINE_AUTO);
		}
		else if((RttNumberNow == 2) || (RttNumberNow == 3))	/*针在需要随量跟踪的位置*/
		{
			
			double liquid_h = 0.0;	/*液面高度*/
			double tempf = 0, tempd = 0;	/*零时保存*/
			
			liquid_h = (double)(p_data->total_volume) / 250.0;	/*计算液面高度，单位mm*/
			
			tempf = (_gp_VRT->preset_location + 3)->forward_pre_deceleration_data;	/*记录当前预减速坐标*/
			tempd = (_gp_VRT->preset_location + 3)->data;							/*记录当前目标坐标*/
			
			if((liquid_h > 2.5) && (liquid_h < (_gp_VRT->preset_location + 3)->forward_pre_deceleration_data))	/*如果液面高度大于2.5mm，需要探入液面2mm*/
			{
				(_gp_VRT->preset_location + 3)->forward_pre_deceleration_data -= (liquid_h - 4);	/*-4为下降补偿*/
				(_gp_VRT->preset_location + 3)->data -= (liquid_h - 4);
				
				err = MCC_Move_PresetLocation(_gp_VRT,
												_gp_VRT->preset_location + 3,	/*按预设排样位置下降*/
												5000,
												MCC_MOVE_DISTANCE,
												0,
												MCC_LINE_AUTO);
				
				(_gp_VRT->preset_location + 3)->forward_pre_deceleration_data = tempf;
				(_gp_VRT->preset_location + 3)->data = tempd;
			}
			else if((liquid_h <= 2.5) && (liquid_h >= 0))	/*液量不够下探*/
			{
				(_gp_VRT->preset_location + 3)->forward_pre_deceleration_data -= liquid_h;
				(_gp_VRT->preset_location + 3)->data -= liquid_h;
				
				err = MCC_Move_PresetLocation(_gp_VRT,
												_gp_VRT->preset_location + 3,	/*按预设排样位置下降*/
												5000,
												MCC_MOVE_DISTANCE,
												0,
												MCC_LINE_AUTO);
				
				(_gp_VRT->preset_location + 3)->forward_pre_deceleration_data = tempf;
				(_gp_VRT->preset_location + 3)->data = tempd;
			}
			else
			{
				Drv_CANProtocol_SendErrorToBuffer(can_id, serial_number, cmd, frame_id, 0xFF, 0xFF, 0x08);		/*报错，指令参数错误*/
				return false;
			}
//			err = MCC_Move_PresetLocation(_gp_VRT,
//											_gp_VRT->preset_location + 3,	/*按预设排样位置下降*/
//											5000,
//											MCC_MOVE_DISTANCE,
//											MCC_GetDCodeInterval(_gp_VRT, MCC_CalcSubdivisionStep(_gp_VRT, (_gp_VRT->preset_location + 1)->data)),
//											MCC_LINE_AUTO);
		}
		else if(RttNumberNow == 4)	/*针在吸样位置（轨道），强制液面检测*/
		{
			err = MCC_Move_PresetLocation(_gp_VRT,
											_gp_VRT->preset_location + 4,	/*按预设排样位置下降*/
											5000,
											MCC_MOVE_D_CODE,
											MCC_GetDCodeInterval(_gp_VRT, MCC_CalcSubdivisionStep(_gp_VRT, (_gp_VRT->preset_location + 1)->data)),
											MCC_LINE_AUTO);
		}
		else if(RttNumberNow == 5)	/*针在吸样位置（急诊），强制液面检测*/
		{
			err = MCC_Move_PresetLocation(_gp_VRT,
											_gp_VRT->preset_location + 5,	/*按预设排样位置下降*/
											5000,
											MCC_MOVE_D_CODE,
											MCC_GetDCodeInterval(_gp_VRT, MCC_CalcSubdivisionStep(_gp_VRT, (_gp_VRT->preset_location + 1)->data)),
											MCC_LINE_AUTO);
		}
		else
		{
			Drv_CANProtocol_SendErrorToBuffer(can_id, serial_number, cmd, frame_id, 0xFF, 0xFF, 0x08);		/*报错，指令参数错误*/
			return false;
		}
	}
	if(err != OS_ERR_NONE)	/*执行失败*/
	{
		Drv_CANProtocol_SendErrorToBuffer(can_id, serial_number, cmd, frame_id, 0xFF, 0xFF, 0x09);		/*报错，指令执行错误*/
		return false;
	}
	if(p_data->en_reporting_margin == 1)	/*使能余量上报*/
	{
		plase = (int32_t)(((double)(_gp_VRT->position) / (double)(_gp_VRT->subdivision)) * ((_gp_VRT->step_angle) / (double)360) * (_gp_VRT->ratio) * 100);	/*计算当前位置，整形，含两位小数*/
		/*计算液面高度*/
		if(RttNumberNow == 0)	/*针在吸样位置*/
			plase = (uint32_t)((_gp_VRT->preset_location + 2)->reverse_pre_deceleration_data * 100) - plase;
		else if((RttNumberNow == 2) || (RttNumberNow == 3))	/*针在排样位置*/
			plase = (uint32_t)((_gp_VRT->preset_location + 3)->reverse_pre_deceleration_data * 100) - plase;
		else if(RttNumberNow == 4)
			plase = (uint32_t)((_gp_VRT->preset_location + 4)->reverse_pre_deceleration_data * 100) - plase;
		else if(RttNumberNow == 5)
			plase = (uint32_t)((_gp_VRT->preset_location + 5)->reverse_pre_deceleration_data * 100) - plase;
		else
			plase = 0;
		datas[0] = 1;
		datas[1] = 0;
		datas[2] = can_id;
		datas[3] = plase;
		datas[4] = plase >> 8;
		datas[5] = p_data->ring_number;
		datas[6] = (p_data->ring_number) >> 8;
		datas[7] = p_data->place_number;
		datas[8] = (p_data->place_number) >> 8;
		datas[9] = p_data->cup_number;
		datas[10] = (p_data->cup_number) >> 8;
		Drv_CANProtocol_SendDatasToBuffer(can_id, serial_number, cmd, frame_id, datas, 11, 1);		/*上报余量*/
	}
	else	/*不上报余量*/
	{
		Drv_CANProtocol_SendDoneToBuffer(can_id, serial_number, cmd, frame_id);	/*直接回复执行成功*/
	}
	return true;
}

/* 针转动
  ------------------------------
  返回值：true，成功；false，失败
*/
bool Func_CmdNdlTurn(void* p_buffer)
{
	DRV_CAN_RX_BUFFER_TYPE*	p_buf  = (DRV_CAN_RX_BUFFER_TYPE *)p_buffer;		/*获取缓冲区数据*/
	CMD_COMMON_HEAD_TYPE*	p_msg  = (CMD_COMMON_HEAD_TYPE*)(p_buf->buffer);	/*获取消息内容*/
	CMD_NDL_TURN_TYPE*		p_data = (CMD_NDL_TURN_TYPE*)(&(p_msg->data));		/*获取数据域*/
	
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
		case 0:		/*吸样位置1（盘）*/
			if(RttNumberNow != 0)
			{
				OSSemPost(_gp_TurnInhaleSemIn);	/*启动旋转吸10uL空气*/
				err = MCC_Move_PresetLocation(_gp_RTT,
												_gp_RTT->preset_location,	/*预设吸样位置1（盘）*/
												5000,
												MCC_MOVE_DISTANCE,
												0,
												MCC_LINE_AUTO);
				if(err != OS_ERR_NONE)	/*执行失败*/
				{
					Drv_CANProtocol_SendErrorToBuffer(can_id, serial_number, cmd, frame_id, 0xFF, 0xFF, 0x09);		/*报错，指令执行错误*/
					return false;
				}
				OSSemPend(_gp_TurnInhaleSemOut, 5000, &err);	/*等待吸空气完成*/
				if(err != OS_ERR_NONE)	/*执行失败*/
				{
					Drv_CANProtocol_SendErrorToBuffer(can_id, serial_number, cmd, frame_id, 0xFF, 0xFF, 0x09);		/*报错，指令执行错误*/
					return false;
				}
				RttNumberNow = 0;
			}
			break;
		
		case 1:		/*复位位置*/
			if(RttNumberNow != 1)
			{
//				err = MCC_Move_PresetLocation(_gp_RTT,
//												_gp_RTT->preset_location + 1,	/*预设复位位置*/
//												5000,
//												MCC_MOVE_DISTANCE,
//												0,
//												MCC_LINE_AUTO);
				err = MCC_Reset(_gp_RTT, 10000);	/*水平电机复位*/
				if(err != OS_ERR_NONE)	/*执行失败*/
				{
					Drv_CANProtocol_SendErrorToBuffer(can_id, serial_number, cmd, frame_id, 0xFF, 0xFF, 0x09);		/*报错，指令执行错误*/
					return false;
				}
				RttNumberNow = 1;
			}
			break;
		
		case 2:		/*稀释位置*/
			if(RttNumberNow != 2)
			{
				OSSemPost(_gp_TurnInhaleSemIn);	/*启动旋转吸10uL空气*/
				err = MCC_Move_PresetLocation(_gp_RTT,
												_gp_RTT->preset_location + 2,	/*预设排样位置*/
												5000,
												MCC_MOVE_DISTANCE,
												0,
												MCC_LINE_AUTO);
				
				if(err != OS_ERR_NONE)	/*执行失败*/
				{
					Drv_CANProtocol_SendErrorToBuffer(can_id, serial_number, cmd, frame_id, 0xFF, 0xFF, 0x09);		/*报错，指令执行错误*/
					return false;
				}
				OSSemPend(_gp_TurnInhaleSemOut, 5000, &err);	/*等待吸空气完成*/
				if(err != OS_ERR_NONE)	/*执行失败*/
				{
					Drv_CANProtocol_SendErrorToBuffer(can_id, serial_number, cmd, frame_id, 0xFF, 0xFF, 0x09);		/*报错，指令执行错误*/
					return false;
				}
				RttNumberNow = 2;
			}
			break;
		
		case 3:		/*排样位置*/
			if(RttNumberNow != 3)
			{
				err = MCC_Move_PresetLocation(_gp_RTT,
												_gp_RTT->preset_location + 3,	/*预设排样位置*/
												5000,
												MCC_MOVE_DISTANCE,
												0,
												MCC_LINE_AUTO);
				if(err != OS_ERR_NONE)	/*执行失败*/
				{
					Drv_CANProtocol_SendErrorToBuffer(can_id, serial_number, cmd, frame_id, 0xFF, 0xFF, 0x09);		/*报错，指令执行错误*/
					return false;
				}
				RttNumberNow = 3;
			}
			break;
		
		case 4:		/*吸样位置2（轨道）*/			
			if(RttNumberNow != 4)
			{
				err = MCC_Move_PresetLocation(_gp_RTT,
												_gp_RTT->preset_location + 4,	/*预设吸样位置2（轨道）*/
												5000,
												MCC_MOVE_DISTANCE,
												0,
												MCC_LINE_AUTO);
				if(err != OS_ERR_NONE)	/*执行失败*/
				{
					Drv_CANProtocol_SendErrorToBuffer(can_id, serial_number, cmd, frame_id, 0xFF, 0xFF, 0x09);		/*报错，指令执行错误*/
					return false;
				}
				RttNumberNow = 4;
			}
			break;
		
		case 5:		/*吸样位置2（急诊）*/
			if(RttNumberNow != 5)
			{
				err = MCC_Move_PresetLocation(_gp_RTT,
												_gp_RTT->preset_location + 5,	/*预设吸样位置3（急诊）*/
												5000,
												MCC_MOVE_DISTANCE,
												0,
												MCC_LINE_AUTO);
				if(err != OS_ERR_NONE)	/*执行失败*/
				{
					Drv_CANProtocol_SendErrorToBuffer(can_id, serial_number, cmd, frame_id, 0xFF, 0xFF, 0x09);		/*报错，指令执行错误*/
					return false;
				}
				RttNumberNow = 5;
			}
			break;
	}
	
	Drv_CANProtocol_SendDoneToBuffer(can_id, serial_number, cmd, frame_id);	/*直接回复执行成功*/
	return true;
}

/* 针吸样
  ------------------------------
  返回值：true，成功；false，失败
*/
bool Func_CmdNdlSample(void* p_buffer)
{
	DRV_CAN_RX_BUFFER_TYPE*	p_buf  = (DRV_CAN_RX_BUFFER_TYPE *)p_buffer;		/*获取缓冲区数据*/
	CMD_COMMON_HEAD_TYPE*	p_msg  = (CMD_COMMON_HEAD_TYPE*)(p_buf->buffer);	/*获取消息内容*/
	CMD_NDL_SAMPLE_TYPE*	p_data = (CMD_NDL_SAMPLE_TYPE*)(&(p_msg->data));	/*获取数据域*/
	
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
	
	err = MCC_Move_Distance(_gp_INJ,
							!_gp_INJ->reset_dir,	/*复位反方向*/
							(double)p_data->size / 10.0 + _para_NEEDLE.multi_suction_multiply100/100.0,	/*单位uL，多吸2uL*/
							5000,
							MCC_MOVE_DISTANCE,
							0,
							MCC_LINE_AUTO);
	if(err != OS_ERR_NONE)	/*执行成功*/
	{
		Drv_CANProtocol_SendErrorToBuffer(can_id, serial_number, cmd, frame_id, 0xFF, 0xFF, 0x0C);		/*报错，指令执行错误*/
		return false;
	}
	
	err = MCC_Move_Distance(_gp_INJ,
							_gp_INJ->reset_dir,	/*复位方向*/
							_para_NEEDLE.push_back_quantity_multiply100/100.0,	/*单位uL，回推1uL*/
							5000,
							MCC_MOVE_DISTANCE,
							0,
							MCC_LINE_AUTO);
	if(err != OS_ERR_NONE)	/*执行成功*/
	{
		Drv_CANProtocol_SendErrorToBuffer(can_id, serial_number, cmd, frame_id, 0xFF, 0xFF, 0x0D);		/*报错，指令执行错误*/
		return false;
	}
	
	_gp_InjStatus = 1;	/*已经吸样*/
	
	Drv_CANProtocol_SendDoneToBuffer(can_id, serial_number, cmd, frame_id);	/*直接回复执行成功*/
	return true;
}

/* 针排样
  ------------------------------
  返回值：true，成功；false，失败
*/
bool Func_CmdNdlBlowoff(void* p_buffer)
{
	DRV_CAN_RX_BUFFER_TYPE*	p_buf  = (DRV_CAN_RX_BUFFER_TYPE *)p_buffer;					/*获取缓冲区数据*/
	CMD_COMMON_HEAD_TYPE*	p_msg  = (CMD_COMMON_HEAD_TYPE*)(p_buf->buffer);				/*获取消息内容*/
	CMD_NDL_BLOWOFF_TYPE*	p_data = (CMD_NDL_BLOWOFF_TYPE*)(&(p_msg->data));				/*获取数据域*/
	
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
	
	err = MCC_Move_Distance(_gp_INJ,
							_gp_INJ->reset_dir,		/*复位方向*/
							((double)p_data->size / 10.0 ) + (((double)p_data->size / 10.0)/ 35) * 0.5 + _para_NEEDLE.multi_displacement_multiply100/100.0,/*单位uL，多排0uL		((double)p_data->size / 10.0) + 0.3*/
							5000,
							MCC_MOVE_DISTANCE,
							0,
							MCC_LINE_AUTO);
	if(err != OS_ERR_NONE)	/*执行成功*/
	{
		Drv_CANProtocol_SendErrorToBuffer(can_id, serial_number, cmd, frame_id, 0xFF, 0xFF, 0x09);		/*报错，指令执行错误*/
		return false;
	}
	
	_gp_InjStatus = 0;	/*已经排样*/
	
	Drv_CANProtocol_SendDoneToBuffer(can_id, serial_number, cmd, frame_id);	/*直接回复执行成功*/
	return true;
}

/* 针清洗
  ------------------------------待完善
  返回值：true，成功；false，失败
*/
bool Func_CmdNdlWash(void* p_buffer)
{
	DRV_CAN_RX_BUFFER_TYPE*	p_buf  = (DRV_CAN_RX_BUFFER_TYPE *)p_buffer;		/*获取缓冲区数据*/
	CMD_COMMON_HEAD_TYPE*	p_msg  = (CMD_COMMON_HEAD_TYPE*)(p_buf->buffer);	/*获取消息内容*/
	CMD_NDL_WASH_TYPE*		p_data = (CMD_NDL_WASH_TYPE*)(&(p_msg->data));		/*获取数据域*/
	
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
	switch(p_data->type)
	{
		case 0:	/*常规清洗*/
			err = MCC_Reset(_gp_INJ, 10000);	/*柱塞泵电机复位*/
			if(err != OS_ERR_NONE)	/*执行失败*/
			{
				Drv_CANProtocol_SendErrorToBuffer(can_id, serial_number, cmd, frame_id, 0xFF, 0xFF, 0x09);		/*报错，指令执行错误*/
				return false;
			}
			
			Valve_Open(_gp_InnerWallCleaningValve);		/*开内壁清洗阀*/
			Valve_Open(_gp_OuterWallCleaningValve);		/*开外壁清洗阀*/
			if(p_data->time != 0)	/*按设置延时*/
			{
				OSTimeDlyHMSM(0, 0, 0, p_data->time);	/*延时*/
			}
			else
			{
				OSTimeDlyHMSM(0, 0, 0, _para_NEEDLE.default_wash_time);			/*默认延时0.5s*/
			}
			Valve_Close(_gp_InnerWallCleaningValve);	/*关内壁清洗阀*/
			Valve_Close(_gp_OuterWallCleaningValve);	/*关外壁清洗阀*/
			OSTimeDlyHMSM(0, 0, 0, 200);
			break;
			
		case 1:	/*长周期清洗*/
			err = MCC_Reset(_gp_INJ, 10000);	/*柱塞泵电机复位*/
			if(err != OS_ERR_NONE)	/*执行失败*/
			{
				Drv_CANProtocol_SendErrorToBuffer(can_id, serial_number, cmd, frame_id, 0xFF, 0xFF, 0x09);		/*报错，指令执行错误*/
				return false;
			}
			
			Valve_Open(_gp_InnerWallCleaningValve);		/*开内壁清洗阀*/
			Valve_Open(_gp_OuterWallCleaningValve);		/*开外壁清洗阀*/
			if(p_data->time != 0)	/*按设置延时*/
			{
				OSTimeDlyHMSM(0, 0, 0, p_data->time);	/*延时*/
			}
			else
			{
				OSTimeDlyHMSM(0, 0, 0, _para_NEEDLE.default_wash_time);			/*默认延时0.5s*/
			}
			Valve_Close(_gp_InnerWallCleaningValve);	/*关内壁清洗阀*/
			Valve_Close(_gp_OuterWallCleaningValve);	/*关外壁清洗阀*/
			OSTimeDlyHMSM(0, 0, 0, 200);
			break;
		
		case 2:	/*强化清洗*/
			err = MCC_Reset(_gp_INJ, 10000);	/*柱塞泵电机复位*/
			if(err != OS_ERR_NONE)	/*执行失败*/
			{
				Drv_CANProtocol_SendErrorToBuffer(can_id, serial_number, cmd, frame_id, 0xFF, 0xFF, 0x09);		/*报错，指令执行错误*/
				return false;
			}
			
			Valve_Open(_gp_InnerWallCleaningValve);		/*开内壁清洗阀*/
			Valve_Open(_gp_OuterWallCleaningValve);		/*开外壁清洗阀*/
			if(p_data->time != 0)	/*按设置延时*/
			{
				OSTimeDlyHMSM(0, 0, 0, p_data->time);	/*延时*/
			}
			else
			{
				OSTimeDlyHMSM(0, 0, 0, _para_NEEDLE.default_wash_time);			/*默认延时0.5s*/
			}
			Valve_Close(_gp_InnerWallCleaningValve);	/*关内壁清洗阀*/
			Valve_Close(_gp_OuterWallCleaningValve);	/*关外壁清洗阀*/
			OSTimeDlyHMSM(0, 0, 0, 200);
			break;
		
		default:
			break;
	}
	
	Drv_CANProtocol_SendDoneToBuffer(can_id, serial_number, cmd, frame_id);	/*直接回复执行成功*/
	return true;
}

/* 针吸水
  ------------------------------
  返回值：true，成功；false，失败
*/
bool Func_CmdNdlWaterUptake(void* p_buffer)
{
	DRV_CAN_RX_BUFFER_TYPE*          p_buf  = (DRV_CAN_RX_BUFFER_TYPE *)p_buffer;					/*获取缓冲区数据*/
	CMD_COMMON_HEAD_TYPE*            p_msg  = (CMD_COMMON_HEAD_TYPE*)(p_buf->buffer);				/*获取消息内容*/
	CMD_NDL_WATER_UPTAKE_TYPE*       p_data = (CMD_NDL_WATER_UPTAKE_TYPE*)(&(p_msg->data));			/*获取数据域*/
	
	uint8_t  size          = p_buf->size;			/*获取指令长度*/
	uint8_t  can_id        = p_buf->can_id;			/*获取CAN ID*/
	uint8_t  serial_number = p_msg->serial_number;	/*获取流水号*/
	uint16_t cmd           = p_msg->cmd;			/*获取指令*/
	uint32_t frame_id      = p_msg->frame_id;		/*获取报文ID*/
	
	uint8_t  err           = OS_ERR_NONE;
	
	if(size != 4)			/*指令长度判断*/
	{
		Drv_CANProtocol_SendErrorToBuffer(can_id, serial_number, cmd, frame_id, 0xFF, 0xFF, 0x05);		/*报错，指令长度错误*/
		return false;
	}
	
	Valve_Open(_gp_OuterWallCleaningValve);	/*开外壁清洗阀*/
	OSTimeDlyHMSM(0, 0, 0, 10);				/*延时10ms*/
	
	err = MCC_Move_Distance(_gp_INJ,
							!_gp_INJ->reset_dir,	/*复位反方向*/
							(double)p_data->size,	/*单位uL*/
							5000,
							MCC_MOVE_DISTANCE,
							0,
							MCC_LINE_AUTO);
	if(err != OS_ERR_NONE)	/*执行失败*/
	{
		Valve_Close(_gp_OuterWallCleaningValve);	/*关外壁清洗阀*/
		Drv_CANProtocol_SendErrorToBuffer(can_id, serial_number, cmd, frame_id, 0xFF, 0xFF, 0x09);		/*报错，指令执行错误*/
		return false;
	}
	
	Valve_Close(_gp_OuterWallCleaningValve);	/*关外壁清洗阀*/
	OSTimeDlyHMSM(0, 0, 0, 10);
	
	Drv_CANProtocol_SendDoneToBuffer(can_id, serial_number, cmd, frame_id);	/*直接回复执行成功*/
	return true;
}

/* 针清洗阀控制
  ------------------------------
  返回值：true，成功；false，失败
*/
bool Func_CmdNdlWashValveControl(void* p_buffer)
{
	DRV_CAN_RX_BUFFER_TYPE*          p_buf  = (DRV_CAN_RX_BUFFER_TYPE *)p_buffer;					/*获取缓冲区数据*/
	CMD_COMMON_HEAD_TYPE*            p_msg  = (CMD_COMMON_HEAD_TYPE*)(p_buf->buffer);				/*获取消息内容*/
	CMD_NDL_WASH_VALVE_CONTROL_TYPE* p_data = (CMD_NDL_WASH_VALVE_CONTROL_TYPE*)(&(p_msg->data));	/*获取数据域*/
	
	uint8_t  size          = p_buf->size;			/*获取指令长度*/
	uint8_t  can_id        = p_buf->can_id;			/*获取CAN ID*/
	uint8_t  serial_number = p_msg->serial_number;	/*获取流水号*/
	uint16_t cmd           = p_msg->cmd;			/*获取指令*/
	uint32_t frame_id      = p_msg->frame_id;		/*获取报文ID*/
	
	uint8_t  err           = OS_ERR_NONE;
	
	if(size != 4)			/*指令长度判断*/
	{
		Drv_CANProtocol_SendErrorToBuffer(can_id, serial_number, cmd, frame_id, 0xFF, 0xFF, 0x05);		/*报错，指令长度错误*/
		return false;
	}
	
	switch(p_data->number)
	{
		case 1:	/*外壁清洗泵*/
			if(p_data->status == 0)	/*开启*/
				Valve_Open(_gp_OuterWallCleaningValve);		/*开外壁清洗阀*/
			else					/*关闭*/
				Valve_Close(_gp_OuterWallCleaningValve);	/*关外壁清洗阀*/
			break;
		case 2:	/*内壁清洗阀*/
			if(p_data->status == 0)	/*开启*/
				Valve_Open(_gp_InnerWallCleaningValve);		/*开内壁清洗阀*/
			else					/*关闭*/
				Valve_Close(_gp_InnerWallCleaningValve);	/*关内壁清洗阀*/
			break;
	}
	OSTimeDlyHMSM(0, 0, 0, 10);		/*延时10ms，等待开启完全*/
	
	Drv_CANProtocol_SendDoneToBuffer(can_id, serial_number, cmd, frame_id);	/*直接回复执行成功*/
	return true;
}

/* 针排气
  ------------------------------
  返回值：true，成功；false，失败
*/
bool Func_CmdNdlAirOut(void* p_buffer)
{
	DRV_CAN_RX_BUFFER_TYPE*          p_buf  = (DRV_CAN_RX_BUFFER_TYPE *)p_buffer;					/*获取缓冲区数据*/
	CMD_COMMON_HEAD_TYPE*            p_msg  = (CMD_COMMON_HEAD_TYPE*)(p_buf->buffer);				/*获取消息内容*/
	CMD_NDL_AIR_OUT_TYPE*            p_data = (CMD_NDL_AIR_OUT_TYPE*)(&(p_msg->data));				/*获取数据域*/
	
	uint8_t  size          = p_buf->size;			/*获取指令长度*/
	uint8_t  can_id        = p_buf->can_id;			/*获取CAN ID*/
	uint8_t  serial_number = p_msg->serial_number;	/*获取流水号*/
	uint16_t cmd           = p_msg->cmd;			/*获取指令*/
	uint32_t frame_id      = p_msg->frame_id;		/*获取报文ID*/
	
	uint8_t  err           = OS_ERR_NONE;
	uint8_t  i             = 0;
	uint8_t  j             = 0;
	
	if(size != 3)			/*指令长度判断*/
	{
		Drv_CANProtocol_SendErrorToBuffer(can_id, serial_number, cmd, frame_id, 0xFF, 0xFF, 0x05);		/*报错，指令长度错误*/
		return false;
	}
	
	Valve_Open(_gp_InnerWallCleaningValve);		/*开内壁清洗阀*/
	Valve_Open(_gp_OuterWallCleaningValve);		/*开外壁清洗阀*/
	OSTimeDlyHMSM(0, 0, 0, 200);				/*洗200ms*/
	Valve_Close(_gp_OuterWallCleaningValve);	/*关外壁清洗阀*/
	
	for(i = 0; i < p_data->number; i++)
	{
		/*吸95uL*/
		err = MCC_Move_Distance(_gp_INJ,
								!_gp_INJ->reset_dir,	/*复位反方向*/
								95.0,					/*单位uL*/
								5000,
								MCC_MOVE_DISTANCE,
								0,
								MCC_LINE_AUTO);
		if(err != OS_ERR_NONE)	/*执行失败*/
		{
			Valve_Close(_gp_InnerWallCleaningValve);	/*关内壁清洗阀*/
			OSTimeDlyHMSM(0, 0, 0, 10);
			Drv_CANProtocol_SendErrorToBuffer(can_id, serial_number, cmd, frame_id, 0xFF, 0xFF, 0x09);		/*报错，指令执行错误*/
			return false;
		}
		/*排4次20uL*/
		for(j = 0; j < 4; j++)
		{
			err = MCC_Move_Distance(_gp_INJ,
									_gp_INJ->reset_dir,	/*复位反方向*/
									20.0,					/*单位uL*/
									5000,
									MCC_MOVE_DISTANCE,
									0,
									MCC_LINE_AUTO);
			if(err != OS_ERR_NONE)	/*执行失败*/
			{
				Valve_Close(_gp_InnerWallCleaningValve);	/*关内壁清洗阀*/
				OSTimeDlyHMSM(0, 0, 0, 10);
				Drv_CANProtocol_SendErrorToBuffer(can_id, serial_number, cmd, frame_id, 0xFF, 0xFF, 0x09);		/*报错，指令执行错误*/
				return false;
			}
		}
		/*复位*/
		MCC_Reset(_gp_INJ, 5000);
	}
	Valve_Open(_gp_OuterWallCleaningValve);		/*开外壁清洗阀*/
	OSTimeDlyHMSM(0, 0, 0, 200);				/*洗200ms*/
	Valve_Close(_gp_InnerWallCleaningValve);	/*关内壁清洗阀*/
	Valve_Close(_gp_OuterWallCleaningValve);	/*关外壁清洗阀*/
	OSTimeDlyHMSM(0, 0, 0, 10);
	
	Drv_CANProtocol_SendDoneToBuffer(can_id, serial_number, cmd, frame_id);	/*直接回复执行成功*/
	return true;
}
