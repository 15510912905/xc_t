#pragma once

#include "property.h"
#include "includes.h"

#define MOTOR_MAX_STEP		  (0xffffu)  //1000
#define FPAG_CLK             (60000000)

//void MConfig_INJ(uint32_t pulse, uint32_t fre, uint8_t dir);
//void MConfig_RTT(uint32_t pulse, uint32_t fre, uint8_t dir);
//void MConfig_VRT(uint32_t pulse, uint32_t fre, uint8_t dir);

//void WASH_stop(void);
//void WASH_outside(void);
//void WASH_start(void);

//void CleanMStep_IRQ(void);
//void CleanOPT_IRQ(void);
//void ReadOPT_IRQ(uint8_t *optoirt);
//void ReadMStep_IRQ(uint8_t * motorirt);
//void ReadOPTstate(uint8_t *optotype);

//void MDisable(uint8_t motortype);
//void MEnable(uint8_t motortype);

//void MEnableFun(void * pstThis);
//void MDisableFun(void *pstThis);


//enum MR_enum  //寄存器定义
//{
//    MR_mStep1 = 0x01FC,	//垂直、水平脉冲数
//    MR_mStep2 = 0x01FD,
//    MR_mStep3 = 0x01FE,
//    MR_mStep4 = 0x01FF,

//    MR_injStep1 = 0x0130,	//泵脉冲数
//    MR_injStep2 = 0x0131,
//    MR_injStep3 = 0x0132,
//    MR_injStep4 = 0x0133,

//    MR_mFre1 = 0x00BC,	//垂直、水平电机频率
//    MR_mFre2 = 0x00BD,
//    MR_mFre3 = 0x00BE,
//    MR_mFre4 = 0x00BF,

//    MR_injFre1 = 0x00DC,	//泵电机频率
//    MR_injFre2 = 0x00DD,
//    MR_injFre3 = 0x00DE,
//    MR_injFre4 = 0x00DF,

//    MR_mChoose = 0x01F8,		//垂直水平选择:1、垂直；0、水平	

//    MR_vrtEnable = 0x00B8,	//垂直使能位 1使能，0禁止
//    MR_rttEnable = 0x00B8,	//水平使能
//    MR_injEnable = 0x00D8,	//泵使能

//    MR_vrtDir = 0x00C4,	//垂直方向 0、向上，  1、向下
//    MR_rttDir = 0x00B4,	//旋转方向 0、顺时针，1、逆时针
//    MR_injDir = 0x00D4,	//注射器方向 0、向上，1、向下

//    MR_vrtOPTstate = 0x0158,	//垂直初始位光耦状态
//    MR_rttOPTDstate = 0x0154,	//水平位置码盘光耦状态
//    MR_rttOPTIstate = 0x0150,	//水平初始位光耦状态
//    MR_injOPTstate = 0x015C,	//泵光耦状态	

//    MR_mStep_IRQ = 0x00AC,		//外部中断2(电机中断)

//    MR_vrtOPT_IRQ = 0x020c,	//垂直光耦中断
//    MR_rttOPTD_IRQ = 0x020a,	//水平码盘光耦中断
//    MR_rttOPTI_IRQ = 0x0208,	//水平初始光耦中断
//    MR_injOPTD_IRQ = 0x020e,	//泵光耦中断

//    MR_vrtStepCount_ll = 0x0240,//垂直电机步数计数寄存器
//    MR_vrtStepCount_lh = 0x0241,
//    MR_vrtStepCount_hl = 0x0242,
//    MR_vrtStepCount_hh = 0x0243,
//    
//    MR_rttIrqReg = 0x022E,  //水平旋转光耦触发方式寄存器

//    MR_iWash = 0x01C4,	//内壁清洗阀		1打开，0关闭
//    MR_bWash = 0x01F4,	//内壁清洗回流阀
//    MR_oWash = 0x0168,	//外壁清洗阀		1打开，0关闭

//};

//extern uint32_t total_vrt_step;

