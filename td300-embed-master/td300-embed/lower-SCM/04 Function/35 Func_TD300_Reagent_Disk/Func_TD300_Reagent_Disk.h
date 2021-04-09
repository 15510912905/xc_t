/********************************************************
   版权所有 (C), 2001-2100, 四川新健康成生物股份有限公司
  -------------------------------------------------------

			嵌入式开发平台——TD300试剂盘功能定义

  -------------------------------------------------------
   文 件 名   : Func_TD300_Reagent_Disk.h
   版 本 号   : V1.0.0.0
   作    者   : ryc
   生成日期   : 2019年10月17日
   功能描述   : TD300试剂盘功能定义
   依赖于     : 
 ********************************************************/

#ifndef _FUNC_TD300_REAGENT_DISK_H_
#define _FUNC_TD300_REAGENT_DISK_H_

#include "stdint.h"
#include "Drv_CANProtocol.h"
#include "Drv_MotionControlComponents.h"
#include "Drv_Valve.h"
#include "Func_Common.h"
#include "Prt_CmdRegedit.h"
#include "Drv_AT24C32.h"
#include "DRV_FS680.h"


#pragma pack(1)	/*按1字节对齐*/

/*盘复位*/
typedef struct {
	uint16_t cmd;				/*命令字*/
} CMD_DSK_RESET_TYPE;

/*盘转动*/
typedef struct {
	uint16_t cmd;				/*命令字*/
	uint16_t type;				/*运动类型*/
	uint16_t cup_number;		/*位号或间隔*/
	uint16_t place;				/*位置*/
} CMD_DSK_TURN_TYPE;

typedef struct {
	uint16_t cmd;
	uint8_t cup_number;
	uint8_t place;
} CMD_Reagent_SCAN_TYPE;
#pragma pack()
typedef struct {
	uint16_t traytype;		//Disk Type
	uint16_t tlvnum;        //tlv number
	char trayno;            //tray number
	char cupno;             //cup number
	uint16_t len;             //length
	char aucbarcode[100];   //barcode 
} CMD_BARCODE_RETURN_TYPE;

/* 私有命令字定义
  ---------------------------------------*/
  
typedef enum {
	
	CMD_DSK_RESET	= 0x0053,		/*盘复位*/
	CMD_DSK_TURN	= 0x0054,		/*盘旋转*/
	CMD_Reagent_SCAN= 0x0055,       /*试剂扫描*/
	
} CMD_DSK_ENUM;

//typedef struct { 
//   uint8_t write_status;        /*写标志位*/
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
//   uint8_t write_status;
//   uint8_t en_close_cycle;         /*闭环控制可用标识，1可用，0不可用，2表示未使用*/ 
//   int32_t data_multiply100;      /*坐标值乘以10000，单位°/mm/uL*/ 
//   int32_t forward_pre_deceleration_data_multiply100;      /*正向预减速坐标值乘以10000，单位°/mm/uL*/
//    int32_t reverse_pre_deceleration_data_multiply100;      /*反向预减速坐标值乘以10000，单位°/mm/uL*/
//    int32_t forward_compensation_multiply100;            /*正向补偿值乘以10000，单位整步*/ 
//   int32_t reverse_compensation_multiply100;            /*反向补偿值乘以10000，单位整步*/ 
//   uint8_t reserve[10];                           /*预留*/ 
//} MCC_PARAMETER2_TYPE;   /*一个位置参数 32Byte*/


/* 初始化----------------------------------------------------*/

/* 初始化 Func_TD300ReagentDiskInit
  ------------------------------
  初始化运动组件
  返回值：true，成功；false，失败
*/
extern bool Func_TD300ReagentDiskInit(void);

/* 私有常驻任务----------------------------------------------------*/

/*无*/

/* 私有指令任务----------------------------------------------------*/

/* 盘复位
  ------------------------------
  返回值：true，成功；false，失败
*/
extern bool Func_CmdDskReset(void* p_buffer);

/* 盘转动
  ------------------------------
  返回值：true，成功；false，失败
*/
extern bool Func_CmdDskTurn(void* p_buffer);

extern bool Func_CmdReagentScan(void* p_buffer);
#endif
