/******************************************************************************

                  版权所有 (C), 2013-2023, 四川新成生物科技有限公司

 ******************************************************************************
  文 件 名   : pid.h
  版 本 号   : 初稿
  作    者   : Gao.J
  生成日期   : 2014年4月11日
  最近修改   :
  功能描述   : pid控制头文件
  函数列表   :
  修改历史   :
  1.日    期   : 2014年4月11日
    作    者   : Gao.J
    修改内容   : 创建文件

******************************************************************************/
#ifndef  __PID_H__
#define  __PID_H__

/*----------------------------------------------*
 * 包含头文件                                   *
 *----------------------------------------------*/

#include "stdbool.h"
#include <stdio.h>
#include <stdint.h>
#include "switch.h"
//#include <stdlib.h>
//#include <math.h>

/*----------------------------------------------*
 * 内部函数原型说明                             *
 *----------------------------------------------*/

/*----------------------------------------------*
 * 模块级变量                                   *
 *----------------------------------------------*/

/*----------------------------------------------*
 * 常量定义                                     *
 *----------------------------------------------*/

/*----------------------------------------------*
 * 宏定义                                       *
 *----------------------------------------------*/
#define 	DEFAULTPERIOD 		 1
#define		FULLDUTY   			 200


//判断输入温度范围偏差
#define TC_REACTEMP_OFFSET 10
#define TC_WASHTEMP_OFFSET 2

//圆周率
#define PI 3.14159
extern uint8_t heat_ch1_duty,heat_ch2_duty,heat_ch3_duty,heat_ch4_duty;


typedef enum 
{
    PRE_PROCESS=0,
    HIGH1,
    LOW1,
    HIGH2,
    LOW2,

    END
}TUNING_STAT;

typedef struct
{
    float setpoint; // 设定目标Desired value
    float kp; 		// 比例常数Proportional Const
    float ki; 		// 积分常数Integral Const
    float kd; 		// 微分常数Derivative Const
    float lasterr; 	// Error[-1]
    float preverr; 	// Error[-2]
    float sumerr; 	// Sums of Errors
}pid;

typedef struct
{
	//pid控制结构体
    pid pid;
    //一个pid控制周期时长
    float period;
    //占空比
    float duty;
	//用于自整定
    TUNING_STAT status;
    float tuning_max;
    float tuning_min;
	  unsigned int tuning_period;
	  //防止抖动
	  unsigned int jitter;
	  //相关调用函数
	
	//相关调用函数
    void (*delay)(float);
    void (*pulsehigh)(heat_channel);
    void (*pulselow)(heat_channel);
}pidtuningpara;
extern pidtuningpara reac_tunning;
//extern pidtuningpara reac_tuning, wash_tuning ,wash_tuning1, wash_tuning2, wash_tuning3,wash_tuning4; 

/*----------------------------------------------*
 * 数据类型定义                                       *
 *----------------------------------------------*/
extern void pid_output(pidtuningpara *tuning);
extern void pid_init(pid* Control, float dsttemp, float p, float i, float d);
extern void pid_inittuningstru(pidtuningpara *tuning);
extern void pid_solo(pidtuningpara *tuning, float curtemp);
extern TUNING_STAT pid_tuning(pidtuningpara *tuning, float curtemp);
extern bool avoidjitter(pidtuningpara *tuning );

#endif

