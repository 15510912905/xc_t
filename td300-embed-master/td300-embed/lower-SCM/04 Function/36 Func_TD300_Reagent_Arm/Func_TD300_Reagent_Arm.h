/********************************************************
   版权所有 (C), 2001-2100, 四川新健康成生物股份有限公司
  -------------------------------------------------------

			嵌入式开发平台——TD300试剂臂功能定义

  -------------------------------------------------------
   文 件 名   : Func_TD300_Reagent_Arm.h
   版 本 号   : V1.0.0.0
   作    者   : ryc
   生成日期   : 2019年10月17日
   功能描述   : TD300试剂臂功能定义
   依赖于     : 
 ********************************************************/

#ifndef _FUNC_TD300_REAGENT_ARM_H_
#define _FUNC_TD300_REAGENT_ARM_H_


#include "stdint.h"
#include "Drv_CANProtocol.h"
#include "Drv_MotionControlComponents.h"
#include "Drv_Valve.h"
#include "Drv_Pump.h"
#include "Func_Common.h"
#include "Prt_CmdRegedit.h"
#include "Drv_AT24C32.h"

#pragma pack(1)	/*按1字节对齐*/

#define DSK_NUMBER_MAX 60				/*盘的最大位号*/
/*针复位*/
typedef struct {
	uint16_t cmd;				/*命令字*/
	uint16_t dis_wash;			/*不清洗标志位*/
	uint32_t time;				/*清洗时间*/
} CMD_NDL_RESET_TYPE;

/*针上升*/
typedef struct {
	uint16_t cmd;				/*命令字*/
} CMD_NDL_UP_TYPE;

/*针下降*/
typedef struct {
	uint16_t cmd;				/*命令字*/
	uint8_t en_liquid_level_measuring;	/*使能液面检测标志位*/
	uint8_t en_reporting_margin;	/*使能余量上报标志位*/
	uint16_t total_volume;	/*总体积*/
	uint16_t ring_number;	/*圈号*/
	uint16_t place_number;	/*位号*/
	uint16_t cup_number;	/*反应杯号*/
} CMD_NDL_DOWN_TYPE;

/*针转动*/
typedef struct {
	uint16_t cmd;				/*命令字*/
	uint8_t  place;				/*目标位置*/
} CMD_NDL_TURN_TYPE;

/*针吸样*/
typedef struct {
	uint16_t cmd;				/*命令字*/
	uint16_t size;				/*吸样量0.1uL*/
} CMD_NDL_SAMPLE_TYPE;

/*针排样*/
typedef struct {
	uint16_t cmd;				/*命令字*/
	uint16_t size;				/*排样量0.1uL*/
} CMD_NDL_BLOWOFF_TYPE;

/*针清洗*/
typedef struct {
	uint16_t cmd;				/*命令字*/
	uint16_t type;				/*清洗方式*/
	uint32_t time;				/*清洗时间*/
} CMD_NDL_WASH_TYPE;

/*针吸水*/
typedef struct {
	uint16_t cmd;				/*命令字*/
	uint16_t size;				/*吸水量*/
} CMD_NDL_WATER_UPTAKE_TYPE;

/*针清洗阀控制*/
typedef struct {
	uint16_t cmd;				/*命令字*/
	uint8_t number;				/*阀编号*/
	uint8_t status;				/*状态*/
} CMD_NDL_WASH_VALVE_CONTROL_TYPE;

/*针排气*/
typedef struct {
	uint16_t cmd;				/*命令字*/
	uint16_t number;			/*次数*/
} CMD_NDL_AIR_OUT_TYPE;

#pragma pack()

/* 私有命令字定义
  ---------------------------------------*/
  
typedef enum {
	CMD_NDL_RESET	= 0x0011,		/*针复位*/
	CMD_NDL_UP		= 0x0012,		/*针上升*/
	CMD_NDL_DOWN	= 0x0013,		/*针下降*/
	CMD_NDL_TURN	= 0x0014,		/*针旋转*/
	CMD_NDL_SAMPLE	= 0x0015,		/*针采样*/
	CMD_NDL_BLOWOFF	= 0x0016,		/*针排样*/
	CMD_NDL_WASH	= 0x0017,		/*针清洗*/
	CMD_NDL_WATER_UPTAKE = 0x0018,	/*针吸水*/
	CMD_NDL_WASH_VALVE_CONTROL = 0x0019,	/*针清洗阀控制*/
	CMD_NDL_AIR_OUT	= 0x001a,		/*针排气*/
} CMD_NDL_ENUM;

//typedef struct { 
//   uint8_t write_flag;        /*写标志位*/
//   uint8_t subdivision;         /*细分*/ 
//   uint8_t step_angle_multiply10;   /*步距角乘以10*/ 
//   uint16_t ratio_multiply100;      /*导程乘以100*/ 
//   uint16_t max_step;            /*最大行程*/ 
//   uint8_t reset_dir;            /*复位方向*/ 
//   uint8_t reset_status;         /*复位状态*/ 
//   uint8_t d_code_status;         /*码齿状态*/ 
//   uint8_t strike_status;         /*撞击状态*/ 
//   uint8_t reset_compensation;   /*复位补偿*/ 
//   uint16_t start_fre_h;         /*高速启动频率*/ 
//   uint16_t end_fre_h;            /*高速最高频率*/ 
//   uint16_t ech_lader_step_h;      /*高速每台阶微步*/ 
//   uint16_t up_max_lader_h;      /*高速最大加速台阶*/ 
//   uint16_t s_par_h_divide100;      /*高速S型曲线的形状参数除100*/ 
//   uint16_t start_fre_l;         /*低速启动频率*/ 
//   uint16_t end_fre_l;            /*低速最高频率*/ 
//   uint16_t ech_lader_step_l;      /*低速每台阶微步*/ 
//   uint16_t up_max_lader_l;      /*低速最大加速台阶*/ 
//   uint16_t s_par_l_divide100;      /*低速S型曲线的形状参数除100*/ 
//} MCC_PARAMETER1_TYPE;   /*一个电机运动组件参数 32Byte*/ 

//typedef struct {    
//   uint8_t write_flag;
//   uint8_t en_close_cycle;         /*闭环控制可用标识，1可用，0不可用，2表示未使用*/ 
//   int32_t data_multiply100;      /*坐标值乘以10000，单位°/mm/uL*/ 
//   int32_t forward_pre_deceleration_data_multiply100;      /*正向预减速坐标值乘以10000，单位°/mm/uL*/
//    int32_t reverse_pre_deceleration_data_multiply100;      /*反向预减速坐标值乘以10000，单位°/mm/uL*/
//    int32_t forward_compensation_multiply100;            /*正向补偿值乘以10000，单位整步*/ 
//   int32_t reverse_compensation_multiply100;            /*反向补偿值乘以10000，单位整步*/ 
//   uint8_t reserve[10];                           /*预留*/ 
//} MCC_PARAMETER2_TYPE;   /*一个位置参数 32Byte*/


/* 样本针参数结构定义 
 ---------------------------------------*/ 
//typedef struct { 
//   uint8_t flag;               /*已写标志0x11*/ 
//   uint16_t up_inhaled_air_1_multiply100;      /*吸样上升吸空气体积(uL)乘以100*/
//   uint16_t up_inhaled_air_2_multiply100;      /*排样上升吸空气体积(ul)乘以100*/
//   uint32_t rotate_inhaled_air_multiply100;   /*旋转吸空气体积(uL)乘以100*/ 
//   uint32_t reset_wash_time;               /*复位洗针时间(ms)*/ 
//   uint32_t default_wash_time;               /*默认洗针时间(ms)*/ 
//   uint32_t multi_suction_multiply100;         /*多吸量(uL)*/ 
//   uint32_t multi_displacement_multiply100;   /*多排量(uL)*/ 
//   uint32_t push_back_quantity_multiply100;   /*回推量(uL)*/ 
//   uint8_t reserve[3];         /*预留*/ 
//} NEEDLE_PARAMETER_TYPE;   /*一个样本针参数 32Byte*/

/* 初始化----------------------------------------------------*/

/* 初始化 Func_TD300ReagentArmInit
  ------------------------------
  初始化运动组件和清洗阀
  返回值：true，成功；false，失败
*/
bool Func_TD300ReagentArmInit(void);

/* 私有常驻任务----------------------------------------------------*/


/* 私有指令任务----------------------------------------------------*/

/* 针复位
  ------------------------------
  返回值：true，成功；false，失败
*/
bool Func_CmdNdlReset(void* p_buffer);

/* 针上升
  ------------------------------
  返回值：true，成功；false，失败
*/
bool Func_CmdNdlUp(void* p_buffer);

/* 针下降
  ------------------------------
  返回值：true，成功；false，失败
*/
bool Func_CmdNdlDown(void* p_buffer);

/* 针转动
  ------------------------------
  返回值：true，成功；false，失败
*/
bool Func_CmdNdlTurn(void* p_buffer);

/* 针吸样
  ------------------------------
  返回值：true，成功；false，失败
*/
bool Func_CmdNdlSample(void* p_buffer);

/* 针排样
  ------------------------------
  返回值：true，成功；false，失败
*/
bool Func_CmdNdlBlowoff(void* p_buffer);

/* 针清洗
  ------------------------------
  返回值：true，成功；false，失败
*/
bool Func_CmdNdlWash(void* p_buffer);

/* 针吸水
  ------------------------------
  返回值：true，成功；false，失败
*/
bool Func_CmdNdlWaterUptake(void* p_buffer);

/* 针清洗阀控制
  ------------------------------
  返回值：true，成功；false，失败
*/
bool Func_CmdNdlWashValveControl(void* p_buffer);

/* 针排气
  ------------------------------
  返回值：true，成功；false，失败
*/
bool Func_CmdNdlAirOut(void* p_buffer);

#endif
