/******************************************************************************

                  版权所有 (C), 2013-2023, 四川新成生物科技有限公司

 ******************************************************************************
  文 件 名   : tempcontrol.h
  版 本 号   : 初稿
  作    者   : Gao.J
  生成日期   : 2014年5月26日
  最近修改   :
  功能描述   : tempcontrol.c 的头文件
  函数列表   :
******************************************************************************/
#ifndef  __TEMPCONTROL_H__
#define  __TEMPCONTROL_H__
#include <stdint.h>

//标准温度值
#define		WASHMACHTEMP_STD   			 (37.0)
#define		WASHCUPTEMP_STD   			 (37.0)
#define		ABLCUPTEMP_STD   			 (37.0)
extern float curtemp;
extern float g_TempOfMach;
extern uint8_t ht_duty_count  ;
extern uint16_t samplecount ;

void init_diff( void );
void temp_control(void);
void temp_controlTurning();



#endif   /*__TEMPCONTROL_H__*/

