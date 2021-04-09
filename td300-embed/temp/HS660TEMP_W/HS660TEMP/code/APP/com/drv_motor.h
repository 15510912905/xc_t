#ifndef _DRV_MOTOR_H__
#define _DRV_MOTOR_H__

#include "stdint.h"
//#include "math.h"
#include "includes.h"
#include "property.h"

//typedef void(*MoveFunc)(void * stThis, uint8_t dir, uint32_t step, uint32_t slowstep);
//typedef void(*DrvFunc)(void * stThis);
//typedef void(*ConfigFunc)(uint32_t pulse, uint32_t fre, uint8_t dir);
//typedef void(*MotorCtlFunc)(void * stThis);
//typedef void(*actFunc)(void *);
//typedef void(*configStep)(void *, uint32_t);
//typedef uint32_t fre_t;
//typedef uint16_t place_t;
//typedef struct
//{ 
//    uint32_t ladder;	//台阶
//    uint32_t step;		//微步
//}LadderStep;
//typedef struct
//{
//    uint8_t count;//加速台阶微步内存块的个数
//    LadderStep* ladstep;//指向加速台阶微步的指针
//    
//}ladstep;

//typedef struct
//{
//    uint8_t  status;   //电机状态
//    uint8_t  dir;      //方向
//    uint8_t _mMType;//电机类型

//    uint32_t maxLadder_sp;     //最大的台阶数
//    uint32_t maxLadder_sd;  //最大的台阶数
//    uint32_t constStep;     //匀速步数
//    uint32_t slowStep;      //低速步数
//    uint32_t slowFre;		//低速运行频率
//    int32_t  curLadder;     //当前台阶数
//    uint32_t *pFre;			//电机频率表
//    uint32_t *pFre_sd;		//电机减速频率表

//    ladstep  stLadStep_sd;//减速台阶-微步结构体
//    ladstep  stLadStep_sp;//台阶-微步配置结构体
//    uint32_t sd_total_step;	//减速的总步数
//    uint32_t totalStep;
//    uint32_t curStep;

//    MoveFunc MMoveSP;		//加速-匀速-减速
//    MoveFunc MMoveSD;		//减速
//    MoveFunc MMoveRL;		//匀速

//    DrvFunc  MDrv;
//    ConfigFunc MConfig;		//电机配置函数
//    MotorCtlFunc MEnableF;	//电机使能
//    MotorCtlFunc MDisableF;	//电机禁能

//    configStep ConfigSP;	//减速-减速配置
//    configStep ConfigSD;	//减速配置

//    actFunc PostSem;		//释放信号量

//}stMotor;

///*电机加减速配置结构体*/
//typedef struct
//{
//	uint16_t    start_fre;			//启动频率
//    uint16_t    end_fre;			//最高频率
//	uint16_t    ech_lader_step;		//每台阶微步
//	uint16_t    up_max_lader;		//最大加速台阶
//	uint16_t	s_par;				//S型曲线的形状参数
//    
//}MotorLineSt;

////电机运行状态
//typedef enum {
//    drv_motor_idle = 0,		//电机空闲态
//    drv_motor_speedup = 1,	//电机加速态
//    drv_motor_runing = 2,	//电机运行态
//    drv_motor_speeddown = 3,//电机减速态
//    drv_motor_slow = 4,		//电机慢速态

//    drv_motor_butt
//}drv_motor_state_enum;

//typedef uint8_t drv_motor_state_enum_u8;

//void MotorDrv(void * pstThis);
//uint32_t callocMotorFre(uint16_t num, fre_t **fre);
//void calc_sd_step(stMotor* stThis, int32_t sd_ladder);
//LadderStep *calloc_ladstep(stMotor* stThis, drv_motor_state_enum_u8 status, uint8_t count);
//bool config_ladstep(stMotor* stThis, uint8_t index, uint32_t ladder, uint32_t step);
//void MotorMoveSP(void * pstThis, uint8_t dir, uint32_t step, uint32_t slowStep);
//void MotorMoveSD(void * pstThis, uint8_t dir, uint32_t step, uint32_t slowStep);
//void MotorMoveRL(void * pstThis, uint8_t dir, uint32_t step, uint32_t slowStep);
//void calcMotorLine(void *pstThis, fre_t *fre_arry);
#endif // !_DRV_NDLMOTOR_H__


