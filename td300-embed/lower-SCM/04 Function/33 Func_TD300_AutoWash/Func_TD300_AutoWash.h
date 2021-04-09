/********************************************************
   版权所有 (C), 2001-2100, 四川新健康成生物股份有限公司
  -------------------------------------------------------

		嵌入式开发平台——TD300自动清洗功能定义

  -------------------------------------------------------
   文 件 名   : Func_TD300_AutoWash.h
   版 本 号   : V1.0.0.0
   作    者   : ryc
   生成日期   : 2019年10月14日
   功能描述   : TD300自动清洗功能定义
   依赖于     : 
 ********************************************************/

#ifndef _FUNC_TD300_AUTOWASH_H_
#define _FUNC_TD300_AUTOWASH_H_

#include "stdint.h"
#include "Drv_CANProtocol.h"
#include "Drv_MotionControlComponents.h"
#include "Drv_Valve.h"
#include "Drv_Pump.h"
#include "Func_Common.h"
#include "Prt_CmdRegedit.h"
#include "Drv_At24c32.h"

#pragma pack(1)	/*按1字节对齐*/

/*自动清洗复位*/
typedef struct {
	uint16_t cmd;				/*命令字*/
} CMD_AUTOWASH_RESET_TYPE;

/*自动清洗升降*/
typedef struct {
	uint16_t cmd;				/*命令字*/
	uint16_t place;				/*位置编号*/
} CMD_AUTOWASH_MOVE_TYPE;

/*自动清洗*/
typedef struct {
	uint16_t cmd;				/*命令字*/
	uint16_t list;				/*位号表*/
} CMD_AUTOWASH_TYPE;

/*自动清洗单个泵阀控制*/
typedef struct {
	uint16_t cmd;				/*命令字*/
	uint8_t number;				/*泵阀编号*/
	uint8_t status;				/*控制状态*/
} CMD_AUTOWASH_PVCONTROL_TYPE;

#pragma pack()

/* 私有命令字定义
  ---------------------------------------*/
  
typedef enum {
	CMD_AUTOWASH_RESET	= 0x0081,		/*自动清洗复位*/
	CMD_AUTOWASH_MOVE	= 0x0082,		/*自动清洗升降*/
	CMD_AUTOWASH		= 0x0083,		/*自动清洗*/
	CMD_AUTOWASH_PVCONTROL = 0x0084,	/*单个泵阀控制*/
} CMD_AUTOWASH_ENUM;

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
//} MCC_PARAMETER1_TYPE;   /*一个组件参数 32Byte*/ 

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

/* 初始化----------------------------------------------------*/

/* 初始化 Func_TD300AutoWash
  ------------------------------
  初始化运动组件和泵阀
  返回值：true，成功；false，失败
*/
extern bool Func_TD300AutoWashInit(void);

/* 私有常驻任务----------------------------------------------------*/

/*无*/

/* 私有指令任务----------------------------------------------------*/

/* 自动清洗复位
  ------------------------------
  返回值：true，成功；false，失败
*/
extern bool Func_CmdAutoWashReset(void* p_buffer);

/* 自动清洗升降
  ------------------------------
  返回值：true，成功；false，失败
*/
extern bool Func_CmdAutoWashMove(void* p_buffer);

/* 自动清洗
  ------------------------------
  返回值：true，成功；false，失败
*/
extern bool Func_CmdAutoWash(void* p_buffer);

/* 自动清洗单个泵阀控制
  ------------------------------
  返回值：true，成功；false，失败
*/
extern bool Func_CmdAutoWashPvControl(void* p_buffer);

#endif
