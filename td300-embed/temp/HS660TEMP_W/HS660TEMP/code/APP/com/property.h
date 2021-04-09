#ifndef _PROPERTY_H__
#define _PROPERTY_H__
#include "includes.h"
//#ifndef OPEN
//#define OPEN	(1)
//#endif
//#ifndef CLOSE
//#define CLOSE	(0)
//#endif
//#ifndef NDL_IN
//#define NDL_IN				 (1)
//#endif
//#ifndef NDL_OUT
//#define NDL_OUT			     (0)
//#endif
//#ifndef ANTICLOCKWISE
//#define ANTICLOCKWISE	(1)
//#endif
//#ifndef CLOCKWISE
//#define CLOCKWISE 		(0)
//#endif

//#define DIR_IDLE		(3)	//空闲

//#define STIR_DCMOTOR_CLOCKWISE 		(0)
//#define STIR_DCMOTOR_ANTICLOCKWISE	(1)


////光耦
//#define VRT		(0)	//垂直
//#define RTTD	(1)	//水平码盘
//#define RTTI	(2)	//水平初始位
//#define INJ		(3)	//泵

////自动清洗
//#define VRT_INIT	(0)//垂直初始光耦
//#define VRT_DOWN	(1)//垂直下降光耦

////盘
//#define DISK_ZERO	(0)//盘零位光耦
//#define DISK_RTT	(1)//盘马齿光耦
////按键
//#define KEY         (0)
//#define LED         (0)
//typedef enum
//{
//    MChoose_RTT = 0,  //旋转电机
//    MChoose_VRT = 1,  //垂直电机

//    MChoose_butt,     //无效值
//}mcu_motor_irt_enum;
//typedef uint8_t mcu_motor_irt_enum_u8;

////定义电机类型
//typedef enum 
//{
//	vrt_Motor = 1,
//	rtt_Motor = 2,
//	inj_Motor = 3,
//	dc_Motor  = 4,

//	Motor_butt
//}motor_t;

////注册电机
//#define NDL_VRT_MOTOR	vrt_Motor
//#define NDL_RTT_MOTOR	rtt_Motor	
//#define NDL_INJ_MOTOR	inj_Motor
//#define STIR_VRT_MOTOR	vrt_Motor
//#define STIR_RTT_MOTOR	rtt_Motor
//#define STIR_DC_MOTOR	dc_Motor
//#define WASH_VRT_MOTOR	vrt_Motor
//#define DISK_RTT_MOTOR	rtt_Motor
//typedef uint8_t motortype_enum_u8;

//#define SHELTER_YES  (1)    //遮挡 
//#define SHELTER_NO   (0)    //未遮挡

////注射泵遮挡
//#define SHELTER_INJ_YES  (0)    //遮挡 
//#define SHELTER_INJ_NO   (1)    //未遮挡


#endif
