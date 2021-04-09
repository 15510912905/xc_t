/******************************************************************************

                  版权所有 (C), 2013-2023, 四川新成生物科技有限公司

 ******************************************************************************
  文 件 名   : pid.c
  版 本 号   : 初稿
  作    者   : PengChuan
  生成日期   : 2014年4月11日
  最近修改   :
  功能描述   : pid控制c文件
  函数列表   :
  修改历史   :
  1.日    期   : 2016年9月11日
    作    者   : PengChuan
    修改内容   : 修改文件

******************************************************************************/

/*----------------------------------------------*
 * 包含头文件                                   *
 *----------------------------------------------*/
#include "pid.h"
#include "float.h"
#include "switch.h"
//#include <math.h>
//#include "USART.h"
//#include "user_conf.h"
//#include <string.h>
/*----------------------------------------------*
 * 内部函数原型说明                             *
 *----------------------------------------------*/

/*----------------------------------------------*
 * 模块级变量                                   *
 *----------------------------------------------*/
// pidtuningpara reac_tuning, wash_tuning, wash_tuning1, wash_tuning2, wash_tuning3,wash_tuning4;
uint8_t heat_ch1_duty;
pidtuningpara reac_tunning;

/*----------------------------------------------*
 * 常量定义                                     *
 *----------------------------------------------*/
 

/*----------------------------------------------*
 * 宏定义                                       *
 *----------------------------------------------*/ 
 
/*****************************************************************************
 函 数 名  : pid_init
 功能描述  : PID结构初始化
 输入参数  : pid* Control
 				float dsttemp
              	float p
             	float i
             	float d
 输出参数  : 无
 返 回 值  : void
 
 修改历史      :
  1.日    期   : 2014年5月27日
    作    者   : Gao.J

    修改内容   : 新生成函数

*****************************************************************************/
void pid_init(pid *Control, float dsttemp, float p, float i, float d)
{
    memset(Control, 0, sizeof(pid));
    //设置目标温度
    Control->setpoint = dsttemp;

    //设定PID参数
    Control->kp = p;
    Control->ki = i;
    Control->kd = d;
	
}
/*****************************************************************************
 函 数 名  : pid_inittuningstru
 功能描述  : 初始化PID自整定参数结构体
 输入参数  : pidtuningpara (*tuning)
 输出参数  : 无
 返 回 值  : void
 
 修改历史      :
  1.日    期   : 2014年5月27日
    作    者   : Gao.J

    修改内容   : 新生成函数

*****************************************************************************/
void pid_inittuningstru(pidtuningpara *tuning)
{
    tuning->period = DEFAULTPERIOD;
    tuning->status = PRE_PROCESS;
    tuning->tuning_max = 0;   //不能修改，用于第一次比较判断
    tuning->tuning_min = 100;   //不能修改，用于第一次比较判断
	  tuning->jitter = 0;
	
	 tuning->delay = heat_delay1s;
   tuning->pulsehigh = heat_Chx_on;
   tuning->pulselow  = heat_Chx_off;
}

float ArrayDuty[4] = {100, 50, 50, 50};  //积分限幅百分比
float ArrayErr[4] = {3, 1, 2, 2};  //积分舍弃偏差
/*****************************************************************************
 函 数 名  : pid_calculate
 功能描述  : 根据测试值和PID参数计算输出值
 输入参数  : pidtuningpara *tuning 
               float currentTemp
 输出参数  : 无
 返 回 值  : float
 
 修改历史      :
  1.日    期   : 2014年09月15日
    作    者   : PengChuan

    修改内容   : 新生成函数

*****************************************************************************/
float pid_calculate(pidtuningpara *tuning, float currentTemp)
{
    float kp = 0.0;
    float ki = 0.0;
    float kd = 0.0;
    float sum = 0.0;
    float sumerr = 0.0;
    int index = 0;
    
    tuning->pid.preverr = tuning->pid.lasterr;
    tuning->pid.lasterr = tuning->pid.setpoint - currentTemp;
    tuning->pid.sumerr += tuning->pid.lasterr;
    
//    if(&reac_tuning == tuning)
//    {
//        index = 0;
//    }
//    else if(&wash_tuning1 == tuning)
//    {
//        index = 1;
//    } 
//    else if(&wash_tuning2 == tuning)
//    {
//        index = 2;
//    }
//    else if(&wash_tuning3 == tuning)
//    {
//        index = 3;
//    }
    
    if(fabs(tuning->pid.lasterr) > ArrayErr[index]) //积分清零
    {
        tuning->pid.sumerr = 0;
    }
    
    if(fabs(tuning->pid.ki) > FLT_EPSILON)
    {
        sumerr = (2 * ArrayDuty[index]/tuning->pid.ki);
    }
    else
    {
        sumerr = 0.0;
    }

    if(tuning->pid.sumerr > sumerr) 
    {
        tuning->pid.sumerr = sumerr;
    }
    else if(tuning->pid.sumerr < -sumerr)
    {
        tuning->pid.sumerr = -sumerr;
    }
    
    kp = tuning->pid.kp * tuning->pid.lasterr;
    ki = tuning->pid.ki * tuning->pid.sumerr;
    kd = tuning->pid.kd * (tuning->pid.lasterr - tuning->pid.preverr);

    sum = kp + ki + kd;
    if (sum <= 0)
    {
        return 0;
    }
    else if (sum >= FULLDUTY)
    {
        return FULLDUTY;
    }
    else
    {
        return sum;
    }
}

/*****************************************************************************
 函 数 名  : pid_para
 功能描述  : 根据ku、tu计算PID调节参数，选较为简单的临界比例度法，即Tp=0.6*ku，
 			Ti=0.5*tu,Td=0.125*tu，其中ku=(4*d)/(3.141592*a),d为继电器特性幅值，
 			a为继电器型非线性环节输入的一次谐波振幅。
 输入参数  : pid *pid
             uint16_t tu
             float ku
 输出参数  : 无
 返 回 值  : void
 
 修改历史      :
  1.日    期   : 2014年4月15日
    作    者   : Gao.J

    修改内容   : 新生成函数

*****************************************************************************/
static void pid_para(pid *pid, unsigned int tu, float k)
{
    float Ti = 0.0;
    float Td = 0.0;
    float ku = 0.0;
    
//    if(fabs(k) > FLT_EPSILON)
//    {
        ku = 4 * 100 / (PI * k);
//    }
    (*pid).kp = 0.6 * ku;

    
    Ti = tu * 0.5;
    
//    if(fabs(Ti) > FLT_EPSILON)
//    {
        (*pid).ki = (*pid).kp / Ti;
//    }
//    else
//    {
//        (*pid).ki = 0;
//    }
    
    Td = tu * 0.125;
    (*pid).kd=(*pid).kp * Td;
}

/*****************************************************************************
 函 数 名  : pid_output
 功能描述  : PID输出控制,指定加热通道
 输入参数  : pidtuningpara *tuning
 			 heat_channel chx
 输出参数  : 无
 返 回 值  : void
 
 修改历史      :
  1.日    期   : 2014年5月27日
    作    者   : Gao.J

    修改内容   : 新生成函数

*****************************************************************************/
void pid_output(pidtuningpara *tuning)
{	
	
 heat_ch1_duty = (uint8_t) tuning->duty;
//	switch(chx)
//	{
//	  case 1 : heat_ch1_duty = (uint8_t) tuning->duty;
//	        break;
//	  case 2 : heat_ch2_duty = (uint8_t) tuning->duty;
//	        break;
//	  case 3 : heat_ch3_duty = (uint8_t) tuning->duty;
//	        break;
//	  case 4 : heat_ch4_duty = (uint8_t) tuning->duty;
//	        break;
//	}

}







/*****************************************************************************
 函 数 名  : pid_tuning
 功能描述  : pid参数自整定,指定加热通道
 输入参数  : pidtuningpara *tuning
             float curtemp
             heat_channel chx
 输出参数  : 无
 返 回 值  : TUNING_STAT
 
 修改历史      :
  1.日    期   : 2014年5月27日
    作    者   : Gao.J

    修改内容   : 新生成函数
  2.日    期   : 2016年6月28日
    作    者   : PengChuan

    修改内容   : 优化
  3.日    期   : 2019年11月11日
    作    者   : ryc

    修改内容   : 改为制冷

*****************************************************************************/
TUNING_STAT pid_tuning(pidtuningpara *tuning, float curtemp)
{
	float setpoint = tuning->pid.setpoint;
	
    if ((tuning->status) != END)
    {
        switch(tuning->status)
        {
            case PRE_PROCESS:   //预制冷
            {
                if (avoidjitter(tuning) && (setpoint < curtemp))
                {
                    heat_Chx_on();
                }
                
//                tuning->delay(tuning->period);
                if (avoidjitter(tuning) && (setpoint >= curtemp))	//防止温度抖动
                {
                    tuning->status = HIGH1;
                     heat_Chx_off(); 
                }
                break;
            }
            case HIGH1: //第一个振荡周期舍弃
            {
                if (avoidjitter(tuning) && (setpoint >= curtemp))
                {
                    heat_Chx_off();
                }
                
//                tuning->delay(tuning->period);
                if (avoidjitter(tuning) && (setpoint < curtemp))	//防止温度抖动
                {
                    tuning->status = LOW1;
                   heat_Chx_on();
                }
                break;
            }
            case LOW1:
            {
                if (avoidjitter(tuning) && (setpoint < curtemp))
                {
                    heat_Chx_on();
                }
                
//                tuning->delay(tuning->period); 
                if (avoidjitter(tuning) && (setpoint >= curtemp))	//防止温度抖动
                {
                    tuning->status = HIGH2;
                    heat_Chx_off();
                }
                break;
            }
            case HIGH2:
            {
                if (avoidjitter(tuning) && (setpoint >= curtemp))
                {
                    heat_Chx_off();
                }
                
//                tuning->delay(tuning->period);
                (tuning->tuning_period)++;  //记录周期个数
                if ((tuning->tuning_max) > curtemp)
                {
                    (tuning->tuning_max) = curtemp; //取该阶段最高点
                }	
                if (avoidjitter(tuning) && (setpoint < curtemp))    //防止温度抖动
                {
                    tuning->status = LOW2;
                    heat_Chx_on();
                }
                break;
            }
            case LOW2:
            {
                if (avoidjitter(tuning) && (setpoint < curtemp))
                {
                    heat_Chx_on();
                }
                
//                tuning->delay(tuning->period);
                (tuning->tuning_period)++;  //记录周期个数
                if ((tuning->tuning_min) < curtemp)
                {
                    (tuning->tuning_min) = curtemp; //取该阶段最低点
                }
                if (avoidjitter(tuning) && (setpoint >= curtemp))   //防止温度抖动
                {//单
                    tuning->tuning_period = (tuning->period)*(tuning->tuning_period);   //个周期时间x周期个数
                    pid_para(&(tuning->pid), tuning->tuning_period, ((tuning->tuning_max)-(tuning->tuning_min))/2);  //根据振荡周期特征量整定pid参数
                    tuning->status = END;
//									printf("自整定算出来的PID的数据为%f \n %f \n %f \n ",tuning->pid.kp,tuning->pid.ki,tuning->pid.kd);
                }
                break;
            }
            default:
            {
                break;
            }
        }
    }
    else
    {
        tuning->duty = pid_calculate(tuning, curtemp);
        pid_output(tuning);   //总延时为一个pid控制周期
    }
    return tuning->status;
}

/*****************************************************************************
 函 数 名  : pid_solo
 功能描述  : 不需要pid参数自整定的控制过程,指定加热通道
 输入参数  : pidtuningpara *tuning
               float curtemp
			    heat_channel chx
 输出参数  : 无
 返 回 值  : void
 
 修改历史      :
  1.日    期   : 2016年8月26日
    作    者   : PengChuan

    修改内容   : 新生成函数

*****************************************************************************/
void pid_solo(pidtuningpara *tuning, float curtemp)
{
    //接近目标值开始pid控制过程
    tuning->duty  = pid_calculate(tuning, curtemp);
    pid_output(tuning);
}

/*****************************************************************************
 函 数 名  : avoidjitter
 功能描述  : 消除整定过程状态迁移的抖动
 输入参数  : pidtuningpara *tuning

 输出参数  : 无
 返 回 值  : bool
 
 修改历史      :
  1.日    期   : 2014年6月30日
    作    者   : Gao.J

    修改内容   : 新生成函数

*****************************************************************************/
bool avoidjitter(pidtuningpara *tuning)
{
	(tuning->jitter)++;
	if ((tuning->jitter) == 3)
	{
		(tuning->jitter) = 0;
		return true;
	}
	return false;
}
