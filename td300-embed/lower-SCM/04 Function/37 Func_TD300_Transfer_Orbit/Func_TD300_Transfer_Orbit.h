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

#ifndef _FUNC_TD300_TRANSFER_ORBIT_H_
#define _FUNC_TD300_TRANSFER_ORBIT_H_

#include "stdint.h"
#include "Drv_CANProtocol.h"
#include "Drv_MotionControlComponents.h"
#include "Drv_SwitchingSensor.h"
#include "Func_Common.h"
#include "Prt_CmdRegedit.h"
#include "Drv_At24c32.h"
#include "Drv_EM1399.h"


/* 私有命令数据域结构定义
  ---------------------------------------*/
#pragma pack(1)	/*按1字节对齐*/

/*复位*/
typedef struct {
	uint16_t cmd;				/*命令字*/
} CMD_ORBITRESET_TYPE;

/*进架*/
typedef struct {
	uint16_t cmd;				/*命令字*/
} CMD_PUSHIN_TYPE;

/*进样*/
typedef struct {
	uint16_t cmd;				/*命令字*/
	uint8_t number;				/*要进样的位号*/
} CMD_TRANSFER_TYPE;

/*退架*/
typedef struct {
	uint16_t cmd;				/*命令字*/
} CMD_PUSHOUT_TYPE;

/*释放*/
typedef struct {
	uint16_t cmd;				/*命令字*/
} CMD_TRANSFERFREE_TYPE;

/*进样N位*/
typedef struct {
	uint16_t cmd;				/*命令字*/
	uint8_t number;				/*要进样的次数*/
} CMD_TRANSFER_STEP_TYPE;

/*样本管条码信息*/
//typedef struct 
//{ 
//   uint16_t cupno;       /*杯编号*/
//   uint16_t len;         /*长度*/
//   char aucbarcode[1];   /*条码编号 !!!下位机务必保证4字节对齐,并同时更新len值!!! */
//}STRes_ShelfInfo; 

///*样本架信息*/
//typedef struct 
//{ 
//   uint16_t shelfNum;  /*样本架条码*/
//   uint16_t smpCount;  /*样本管个数*/
//   STRes_ShelfInfo stShelfInfo[1];/*TODO先只传架号*/
//}STRes_ShelfScan;
//样本架信息
typedef struct
{
//    uint16_t cmd;           //0xff04
//    uint16_t usstatustype;  //状态类型     46//参数上报
    uint16_t shelfNum;      //样本架条码
    uint8_t cupno;         //杯编号
    uint8_t len;           //条码长度
    char aucbarcode[100];     //条码编号 !!!下位机务必保证4字节对齐,并同时更新len值!!!
}STRes_ShelfScan;

#pragma pack()

/* 私有命令字定义
  ---------------------------------------*/
  
typedef enum {
	CMD_ORBITRESET	= 0x0040,		/*模块复位*/
	CMD_PUSHIN		= 0x0041,		/*进架*/
	CMD_TRANSFER	= 0x0042,		/*进样*/
	CMD_PUSHOUT		= 0x0043,		/*退架*/
	CMD_TRANSFERFREE = 0x0044,		/*进样释放*/
	CMD_TRANSFER_STEP = 0x0045,		/*进样N位*/
//	CMD_SCAN = 0x0046,				/*扫描*/
} CMD_TRANSFER_ORBIT_ENUM;

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

//typedef struct {
//	uint8_t count;				/*个数*/
//	uint8_t datas[100];		/*数据*/
//} CMD_BARCODE_RETURN_TYPE;

/* 初始化----------------------------------------------------*/

/* 初始化 Func_TD300TransferOrbitInit
  ------------------------------
  初始化运动组件
  返回值：true，成功；false，失败
*/
extern bool Func_TD300TransferOrbitInit(void);

/* 私有常驻任务----------------------------------------------------*/

/*无*/

/* 私有指令任务----------------------------------------------------*/

/* 模块复位
  ------------------------------ 
  返回值：true，成功；false，失败
*/
extern bool Func_Cmd_OrbitReset(void* p_buffer);

/* 进架
  ------------------------------ 
  返回值：true，成功；false，失败
*/
extern bool Func_Cmd_PushIn(void* p_buffer);

/* 进样
  ------------------------------ 
  返回值：true，成功；false，失败
*/
extern bool Func_Cmd_Transfer(void* p_buffer);

/* 退架
  ------------------------------ 
  返回值：true，成功；false，失败
*/
extern bool Func_Cmd_PushOut(void* p_buffer);

/* 进样释放
  ------------------------------ 
  返回值：true，成功；false，失败
*/
extern bool Func_Cmd_TransferFree(void* p_buffer);

/* 进样N位
  ------------------------------ 
  返回值：true，成功；false，失败
*/
extern bool Func_Cmd_TransferStep(void* p_buffer);

/* 扫描
  ------------------------------
  返回值：true，成功；false，失败
 */
 extern bool Func_Cmd_Scan(void* p_buffer); 
 
 extern uint16_t Char2Int(uint8_t* data_str,uint8_t len);
#endif
