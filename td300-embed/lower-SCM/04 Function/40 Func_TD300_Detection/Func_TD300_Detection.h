/********************************************************
   版权所有 (C), 2001-2100, 四川新健康成生物股份有限公司
  -------------------------------------------------------

		嵌入式开发平台——TD300 检测功能定义

  -------------------------------------------------------
   文 件 名   : Func_TD300_Detection.h
   版 本 号   : V1.0.0.0
   作    者   : ryc
   生成日期   : 2019年10月14日
   功能描述   : TD300 检测功能定义
   依赖于     : 
 ********************************************************/

#ifndef _FUNC_TD300_DETECTION_H_
#define _FUNC_TD300_DETECTION_H_

#include "stdint.h"
#include "Drv_CANProtocol.h"
#include "Drv_MAX5402EUA.h"
#include "Drv_MAX4634EUB.h"
#include "Drv_AD7685.h"
#include "Drv_SwitchingSensor.h"
#include "Func_Common.h"
#include "Prt_CmdRegedit.h"

/* 私有命令字定义
  ---------------------------------------*/
  
typedef enum {
	CMD_READ_ONE_AD		= 0x00B5,		/*读一个AD值*/
	CMD_READ_AD			= 0x00B6,		/*检测流程*/
	CMD_SET_GAIN		= 0x00B7,		/*设置增益*/
	CMD_READ_ALL_AD     = 0x00B8,		/*读平坦区流程*/
} CMD_AD_ENUM;

/* 私有命令数据域结构定义
  ---------------------------------------*/

#pragma pack(1)	/*按1字节对齐*/
/*平台区AD值-光耦状态结构定义
-----------------------------------------*/
typedef struct{
uint16_t op;
uint16_t ad;
} AD_OP_TYPE;

/*读平坦区*/
typedef struct {
uint16_t cmd;	/*命令字*/
} CMD_READ_ALL_AD_TYPE;

/*读一个AD值*/
typedef struct {
	uint16_t cmd;				/*命令字*/
	uint8_t channel;			/*通道号*/
} CMD_READ_ONE_AD_TYPE;
typedef struct {
	uint16_t cmd;				/*命令字*/
} CMD_READ_ONE_AD_RETURN_TYPE;

/*检测流程*/
typedef struct {
	uint16_t cmd;				/*命令字*/
	uint16_t count;				/*检测个数*/
} CMD_READ_AD_TYPE;
typedef struct {
	uint32_t count;				/*个数*/
	uint32_t datas[150][4];		/*数据*/
} CMD_READ_AD_RETURN_TYPE;

/*设置增益*/
typedef struct {
	uint16_t cmd;				/*命令字*/
	uint8_t channel;			/*通道号*/
	uint16_t gain;				/*电阻值*/
} CMD_SET_GAIN_TYPE;
typedef struct {
	uint16_t cmd;				/*命令字*/
} CMD_SET_GAIN_RETURN_TYPE;

#pragma pack()

/* 初始化 Func_TD300DetectionInit
  ------------------------------
  返回值：true，成功；false，失败
*/
extern bool Func_TD300DetectionInit(void);

/* 读一个AD值
  ------------------------------
  返回值：true，成功；false，失败
*/
extern bool Func_Cmd_ReadOneAd(void* p_buffer);

/* 检测流程
  ------------------------------
  返回值：true，成功；false，失败
*/
extern bool Func_Cmd_ReadAd(void* p_buffer);

/* 设置增益
  ------------------------------
  返回值：true，成功；false，失败
*/
extern bool Func_Cmd_SetGain(void* p_buffer);

/*--------------------------------------------------------------*/

/* 计算平均值
  ------------------------------
  返回值：均值
*/
static uint32_t _Func_Average(uint32_t* datas, uint8_t count);

/* 按照4个波长从小到大排序
  ------------------------------
  返回值：true，成功；false，失败
*/
static bool _Func_Sort4(uint32_t* datas);

/* 按照4个波长编号与通道编号转换
  ------------------------------
  返回值：通道编号
*/
static uint8_t _Func_Mapping4(uint8_t number);

/*读平台区流程
返回值：true 成功； false， 失败
*/
static bool Func_Cmd_ReadAllAd(void* p_buffer);


#endif
