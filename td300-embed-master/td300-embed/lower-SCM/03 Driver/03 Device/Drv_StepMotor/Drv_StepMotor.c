/********************************************************
   版权所有 (C), 2001-2100, 四川新健康成生物股份有限公司
  -------------------------------------------------------

          嵌入式开发平台——电机模块（控制STEP、DIR和EN）

  -------------------------------------------------------
   文 件 名   : Drv_StepMotor.c
   版 本 号   : V1.0.0.0
   作    者   : ryc
   生成日期   : 2019年06月24日
   功能描述   : 定义一种控制步进电机加减速运动的模块，使用
				外部的PWM模块作为脉冲输出。通过配置加减速参
				数，实现电机的任意S型曲线控制、匀速控制，以
				及减速控制。
   使用方法   ：上层模块通过调用Motor_Init，获得一个电机模
				块的指针，通过调用Motor_MoveSP_Begin函数，
				启动电机的加减速控制，通过Motor_Move_Stop函
				数实现运动的中止，__Motor_Done电机运行完成
				函数由上层模块实现。
   依赖于     : BSP V1.0.0.0
				Drv_PWM_TimerDriven V1.0.0.0
   注         ：该模块不依赖于硬件电路。 
 ********************************************************/

#include "Drv_StepMotor.h"

static DRV_STEPMOTOR_TYPE* _gp_MotorUnits[MOTOR_BUTT] = 			/*电机模块的注册表*/
{
	0
};

static DRV_STEPMOTOR_LINE_TYPE _g_MotorLine[MOTOR_BUTT * 2] = 	/*电机加减速参数表*/
{	/* 初速度(pps)、末速度(pps)、每台阶步数、加速台阶数、S曲线参数*100 */
	{ 100, 10000, 3,  200,  5000},	//MOTOR_1低速参数
	{ 100, 10000, 3,  200,  5000},	//MOTOR_1高速参数
	{ 100, 10000, 3,  200,  5000},	//MOTOR_2低速参数
	{ 100, 10000, 3,  200,  5000},	//MOTOR_2高速参数
	{ 100, 10000, 3,  200,  5000},	//MOTOR_3低速参数
    { 100, 10000, 3,  200,  5000},	//MOTOR_3高速参数
	{ 100, 10000, 3,  200,  5000},	//MOTOR_4低速参数
	{ 100, 10000, 3,  200,  5000},	//MOTOR_4高速参数
};

/* 接口函数
 ********************************************************/

/* 电机模块初始化
  --------------------------------
  入口：模块编号，细分，使能引脚端口，方向引脚端口，脉冲引脚端口
  返回值：分配的电机模块结构体地址，失败返回NULL
*/
DRV_STEPMOTOR_TYPE* Motor_Init(DRV_STEPMOTOR_ENUM id, uint8_t subdivision, \
							   BSP_PORT_ENUM en_port_number,   BSP_PIN_ENUM en_pin_number, \
							   BSP_PORT_ENUM dir_port_number,  BSP_PIN_ENUM dir_pin_number, \
							   BSP_PORT_ENUM step_port_number, BSP_PIN_ENUM step_pin_number)
{
	DRV_STEPMOTOR_TYPE* p_unit = (DRV_STEPMOTOR_TYPE*)calloc(1, sizeof(DRV_STEPMOTOR_TYPE));
	
	/*分配失败返回NULL*/
	if(p_unit == NULL)	 					
	{
		return NULL;
	}
	
	/*id错误返回NULL*/
	if(id >= MOTOR_BUTT) 					
	{
		free(p_unit);
		return NULL;
	}
	
	if(_gp_MotorUnits[id] != NULL)			/*如果当前模块存在，取消分配新存储区*/
	{
		free(p_unit);
		p_unit = _gp_MotorUnits[id];
	}
	else
	{
		_gp_MotorUnits[id] = p_unit;
	}
	
	/*分模块初始化*/
	switch(id)
	{
		case MOTOR_1:
			/*------------------------------------------常量*/
			p_unit->step_pin = PWM_Init(PWM_1, _T2_, step_port_number, step_pin_number);	/*初始化PWM模块*/
			break;
			
		case MOTOR_2:
			/*------------------------------------------常量*/
			p_unit->step_pin = PWM_Init(PWM_2, _T3_, step_port_number, step_pin_number);
			break;
			
		case MOTOR_3:
			/*------------------------------------------常量*/
			p_unit->step_pin = PWM_Init(PWM_3, _T4_, step_port_number, step_pin_number);
			break;
			
		case MOTOR_4:
			/*------------------------------------------常量*/
			p_unit->step_pin = PWM_Init(PWM_4, _T5_, step_port_number, step_pin_number);
			break;
			
		default:
			free(p_unit);
			return NULL;
	}
	
	/*公共的初始化*/	
	
	/*------------------------------------------常量*/
	p_unit->id        = id;						/*模块编号*/
	p_unit->en_port_number = en_port_number;	/*使用的EN引脚端口号*/
	p_unit->en_pin_number = en_pin_number;		/*使用的EN引脚编号*/
	BSP_Init_Pin(en_port_number,  en_pin_number,  _OUT_PP_);
	p_unit->dir_port_number = dir_port_number;	/*使用的DIR引脚端口号*/
	p_unit->dir_pin_number = dir_pin_number;	/*使用的DIR引脚编号*/
	BSP_Init_Pin(dir_port_number, dir_pin_number, _OUT_PP_);		
	
	p_unit->line_high = &(_g_MotorLine[id * 2 + 1]);	/*高速运动参数表*/
	p_unit->line_low  = &(_g_MotorLine[id * 2]);		/*低速运动参数表*/
	
	p_unit->subdivision = subdivision;			/*电机细分数*/
	
	/*申请台阶-微步结构体组*/
	if(p_unit->stLadStep_sp.ladder_step == NULL)
	{
		if(_CallocLadstep(&(p_unit->stLadStep_sp), 2) == NULL)		
		{
			free(p_unit);
			return NULL;
		}
	}
	if(p_unit->stLadStep_sd.ladder_step == NULL)
	{
		if(_CallocLadstep(&(p_unit->stLadStep_sd), 2) == NULL)
		{
			free(p_unit);
			return NULL;
		}
	}
	
	/*申请电机频率表内存*/
	if(p_unit->line_hig_fre == NULL)
	{
		p_unit->line_hig_fre = _CallocMotorFre(p_unit->line_high->up_max_lader);
		if(p_unit->line_hig_fre == NULL)		
		{
			free(p_unit);
			return NULL;
		}
	}
	CalcMotorFre(p_unit->subdivision, p_unit->line_high, p_unit->line_hig_fre);	/*高速运动频率表计算*/
	if(p_unit->line_low_fre == NULL)
	{
		p_unit->line_low_fre = _CallocMotorFre(p_unit->line_low->up_max_lader);
		if(p_unit->line_low_fre == NULL)
		{
			free(p_unit);
			return NULL;
		}
	}
	CalcMotorFre(p_unit->subdivision, p_unit->line_low, p_unit->line_low_fre);	/*低速运动频率表计算*/
	
	/*------------------------------------------变常*/
	p_unit->en_buffer       = 0;			/*当前使能状态*/
    p_unit->dir_buffer      = 0;			/*当前运动方向*/
    p_unit->slowStep        = 0;      		/*慢速运动步数*/
    p_unit->slowFre         = 0;			/*慢速运行频率*/
    p_unit->p_fre_sp        = NULL;		/*电机加速频率表*/
    p_unit->p_fre_sd        = NULL;		/*电机减速频率表*/
	p_unit->break_away      = 0;			/*启动时是否先匀速脱离*/
	p_unit->break_away_step = 0;			/*匀速脱离步数*/
    p_unit->maxLadder_sp    = 0;			/*最大的加速台阶数*/
    p_unit->maxLadder_sd    = 0;  			/*最大的减速台阶数*/
    p_unit->constStep       = 0;     		/*匀速运动总步数*/
    p_unit->sd_total_step   = 0;			/*减速运动总步数*/
	
	/*------------------------------------------变量*/
    p_unit->status          = MOTOR_IDLE;	/*空闲态*/
    p_unit->curLadder       = 0;     		/*当前台阶*/
    p_unit->curStep         = 0;			/*当前步数*/
    p_unit->totalStep       = 0;   		/*当前台阶总步数*/
	p_unit->position        = 0;			/*当前坐标*/
	
	/*引脚初始化*/
	BSP_WritePin(p_unit->en_port_number, p_unit->en_pin_number, p_unit->en_buffer);
	BSP_WritePin(p_unit->dir_port_number, p_unit->dir_pin_number, p_unit->dir_buffer);
	
	return p_unit;
}

/* SP运动启动函数
  --------------------------------
  ->加速->匀速->减速->慢速->停止
  入口：p_unit 电机结构体，dir 方向，step 总步数，slowstep 慢速步数，line_switch 高低速曲线选择
  返回值：无
*/
void Motor_MoveSP_Begin(DRV_STEPMOTOR_TYPE* p_unit, uint8_t dir, uint32_t step, uint32_t slowstep, bool line_switch)
{
	p_unit->dir_buffer = dir;				/*控制并记录运动方向*/
	BSP_WritePin(p_unit->dir_port_number, p_unit->dir_pin_number, p_unit->dir_buffer);
	p_unit->slowStep  = slowstep;			/*记录慢速运动步数*/
	
	p_unit->status    = MOTOR_SPEEDUP;		/*当前运动从加速态开始*/
	p_unit->curLadder = 0;					/*当前台阶编号清零*/
	p_unit->curStep   = 0;					/*当前步数清零*/

	_ConfigSP(p_unit, step, line_switch);  /*配置SP运行参数*/
	
    _MotorDrv(p_unit);						/*进入第一个台阶运行*/
}

/* RL运动启动函数
  --------------------------------
  ->慢速->停止
  入口：p_unit 电机结构体，dir 方向，step 总步数，slowstep 慢速步数，fre 频率
  返回值：无
*/
void Motor_MoveRL_Begin(DRV_STEPMOTOR_TYPE* p_unit, uint8_t dir, uint32_t step, uint32_t slowstep, uint32_t fre)
{
	p_unit->dir_buffer = dir;				/*控制并记录运动方向*/
	BSP_WritePin(p_unit->dir_port_number, p_unit->dir_pin_number, p_unit->dir_buffer);
	p_unit->slowStep  = step + slowstep;				/*记录慢速运动步数*/
	
	p_unit->status    = MOTOR_SLOW;		/*当前运动从慢速态开始*/
	p_unit->curLadder = 0;					/*当前台阶编号清零*/
    p_unit->curStep   = 0;					/*当前步数清零*/
	
	p_unit->slowFre   = fre;				/*记录匀速运动频率*/

	_MotorDrv(p_unit);						/*进入第一个台阶运行*/
}

/* SD运动启动函数
  --------------------------------
  ->减速->慢速->停止
  入口：p_unit 电机结构体，dir 方向，step 总步数，slowstep 慢速步数
  返回值：无
*/
void Motor_MoveSD_Begin(DRV_STEPMOTOR_TYPE* p_unit, uint8_t dir, uint32_t step, uint32_t slowstep)
{
	p_unit->dir_buffer = dir;				/*控制并记录运动方向*/
	BSP_WritePin(p_unit->dir_port_number, p_unit->dir_pin_number, p_unit->dir_buffer);
	p_unit->slowStep = slowstep;			/*记录慢速运动步数*/
	
	p_unit->status   = MOTOR_SPEEDDOWN;	/*当前运动从减速态开始*/
    p_unit->curStep  = 0;					/*当前步数清零*/

	_ConfigSD(p_unit, step);        		/*配置SD运行参数*/
	
	_MotorDrv(p_unit);						/*进入第一个台阶运行*/
}

/* 运动停止函数
  --------------------------------
  入口：p_unit 电机结构体
  返回值：无
*/
void Motor_Move_Stop(DRV_STEPMOTOR_TYPE* p_unit)
{
	PWM_Disable(p_unit->step_pin);	/*停止PWM输出*/
	if(p_unit->dir_buffer == 0)	/*坐标更新*/
	{
		p_unit->position += (p_unit->step_pin->total_pulse - p_unit->step_pin->remaining_pulse / 2);
	}
	else
	{
		p_unit->position -= (p_unit->step_pin->total_pulse - p_unit->step_pin->remaining_pulse / 2);
	}
}

/* 设置是否缓慢退出
  --------------------------------
  入口：p_unit 电机结构体，break_away 是否需要，break_away_step 运动步数
  返回值：无
*/
void Motor_Set_BreakAway(DRV_STEPMOTOR_TYPE* p_unit, uint8_t break_away, uint32_t break_away_step)
{
	p_unit->break_away = break_away;
	p_unit->break_away_step = break_away_step;
}

/* 内部函数
 ********************************************************/

/* 运动过程更新函数
  --------------------------------
  在一个台阶的PWM输出完成时调用
  入口：p_unit 电机结构体
  返回值：更新后电机状态
*/
static DRV_STEPMOTOR_STATUS_ENUM _Motor_Move_Update(DRV_STEPMOTOR_TYPE* p_unit)
{
	PWM_Disable(p_unit->step_pin);	/*停止当前运动*/
	switch(p_unit->status)
	{
		case MOTOR_SPEEDUP:			/*在加速*/
			if(++(p_unit->curLadder) >= p_unit->maxLadder_sp)	/*台阶先加一，如果加速完成*/
			{
				p_unit->status = MOTOR_RUNING;			/*进入匀速运行态*/
			}
			break;
			
		case MOTOR_RUNING:			/*在匀速*/
			p_unit->status = MOTOR_SPEEDDOWN;			/*进入减速运行态*/
			break;
		
		case MOTOR_SPEEDDOWN:		/*在减速*/
			if(--(p_unit->curLadder) <= 0)
			{
				p_unit->status = MOTOR_SLOW;				/*进入慢速运行态*/
			}
			break;
			
		case MOTOR_SLOW:			/*在慢速*/
			p_unit->status = MOTOR_IDLE;					/*进入空闲态*/
			break;
		
		default:
			return (p_unit->status = MOTOR_IDLE);
	}
	_MotorDrv(p_unit);                        			/*进入下一个台阶*/
			
	return p_unit->status;
}

/* 配置SP运行函数
  --------------------------------
  入口：p_unit 电机结构体，step 总步数，line_switch 高低速曲线选择
  返回值：无
*/
static void _ConfigSP(DRV_STEPMOTOR_TYPE* p_unit, uint32_t step, bool line_switch)
{
    DRV_STEPMOTOR_LINE_TYPE* p_mps = NULL;				/*选择的电机运行参数表*/
    uint32_t 	 sp_ladder;

    /*加速*/
	if(line_switch == 1)			/*选取电机运行参数表和加速频率表*/
	{
		p_mps = p_unit->line_high;
		p_unit->p_fre_sp = p_unit->line_hig_fre;
	}
	else
	{
		p_mps = p_unit->line_low;
		p_unit->p_fre_sp = p_unit->line_low_fre;
	}
	
	if(p_unit->break_away)					/*是否需要在加速前慢速脱离*/
	{
        _LadstepSp_Config(p_unit, 0,                   1, p_unit->break_away_step);			/*先匀速脱离*/
        _LadstepSp_Config(p_unit, 1, p_mps->up_max_lader, p_mps->ech_lader_step);
	}
	else
	{
		_LadstepSp_Config(p_unit, 0, p_mps->up_max_lader, p_mps->ech_lader_step);			/*配置电机运动方式*/
	}
		
    sp_ladder = (step * 2) / (p_mps->ech_lader_step * 5);		/*计算最大加速台阶数限值，定义为总的可用台阶数乘以0.4*/
	
    p_unit->maxLadder_sp = (sp_ladder > p_mps->up_max_lader) ? p_mps->up_max_lader       : sp_ladder;	/*计算最大加速台阶数*/
    p_unit->maxLadder_sp = (p_unit->maxLadder_sp > 1)         ? (p_unit->maxLadder_sp - 1) : 0;
	
    /*匀速*/
    if (step > 2 * p_unit->maxLadder_sp * p_mps->ech_lader_step)	/*如果总步数大于加减速脉冲数，匀速步数为其差值*/
    {
        p_unit->constStep = step - 2 * p_unit->maxLadder_sp * p_mps->ech_lader_step;
    }
    else														/*否则为其1*/
    {
        p_unit->constStep = 1;
    }
	
    /*减速*/
    p_unit->maxLadder_sd  = p_unit->maxLadder_sp;							/*最大减速台阶数与加速台阶数相等*/
    p_unit->p_fre_sd      = p_unit->p_fre_sp;								/*减速频率表与加速频率表相同*/
    p_unit->sd_total_step = p_unit->maxLadder_sd * p_mps->ech_lader_step;	/*计算总减速步数*/
    _LadstepSd_Config(p_unit, 0, p_mps->up_max_lader, p_mps->ech_lader_step);
}

/* 配置SD运行函数
  --------------------------------
  入口：p_unit 电机结构体，step 总运动步数，
  返回值：无
*/
static void _ConfigSD(DRV_STEPMOTOR_TYPE* p_unit, uint32_t step)
{
    /*减速*/
    p_unit->maxLadder_sd  = p_unit->curLadder;
    p_unit->p_fre_sd      = p_unit->line_low_fre;
    p_unit->sd_total_step = step;
    _LadstepSd_Config(p_unit, 0, p_unit->maxLadder_sd, p_unit->line_low->ech_lader_step);
}

/* 电机控制状态机
  --------------------------------
  切换台阶时调用
  入口：p_unit 所需控制电机的结构体
  返回值：无
*/
static void _MotorDrv(DRV_STEPMOTOR_TYPE* p_unit)
{	
    switch (p_unit->status)
    {
		case MOTOR_SPEEDUP:		/*加速态*/
			PWM_Config(p_unit->step_pin, _Calc_Cur_Step(p_unit), p_unit->p_fre_sp[p_unit->curLadder]);		/*配置PWM输出，步数，频率，方向*/
			break;
			
		case MOTOR_RUNING:		/*运行态*/
			PWM_Config(p_unit->step_pin, p_unit->constStep, p_unit->p_fre_sp[p_unit->curLadder]);			/*配置PWM输出，步数，频率，方向*/
			break;

		case MOTOR_SPEEDDOWN:	/*减速态*/
			PWM_Config(p_unit->step_pin, _Calc_Cur_Step(p_unit), p_unit->p_fre_sd[p_unit->curLadder]);		/*配置PWM输出，步数，频率，方向*/
			break;

		case MOTOR_SLOW:		/*慢速态*/
			if(p_unit->slowStep != 0)															/*无需慢速运行*/
			{
				PWM_Config(p_unit->step_pin, p_unit->slowStep, p_unit->slowFre);							/*配置PWM输出，步数，频率，方向*/
				break;
			}
			else
			{
				p_unit->status = MOTOR_IDLE;													/*直接进入空闲态*/
				/*注意此处不break，直接进入MOTOR_IDLE*/
			}
			
		case MOTOR_IDLE:		/*空闲态，释放信号量*/
			__Motor_Done(p_unit);
			PWM_Disable(p_unit->step_pin);
			p_unit->dir_buffer = 0;
			BSP_WritePin(p_unit->dir_port_number, p_unit->dir_pin_number, p_unit->dir_buffer);
			return;
			
		default:
			return;
    }

    PWM_Enable(p_unit->step_pin);	
}

/* 配置SP Ladstep
  --------------------------------
  入口：p_unit 电机结构体，index 下标（用于分段运动使用），ladder 台阶数，step 每台阶步数
  返回值：false配置失败，true配置成功
*/
static bool _LadstepSp_Config(DRV_STEPMOTOR_TYPE* p_unit, uint8_t index, uint32_t ladder, uint32_t step)
{
	if (index >= p_unit->stLadStep_sp.count)	/*超出范围*/
	{
		//WriteBBLog("index more than %d", motor->stLadStep_sp.count);
		return false;
	}
	p_unit->stLadStep_sp.ladder_step[index].ladder = ladder;
	p_unit->stLadStep_sp.ladder_step[index].step = step;

	return true;
}

/* 配置SD Ladstep
  --------------------------------
  入口：p_unit 电机结构体，index 下标（用于分段运动使用），ladder 台阶数，step 每台阶步数
  返回值：false配置失败，true配置成功
*/
static bool _LadstepSd_Config(DRV_STEPMOTOR_TYPE* p_unit, uint8_t index, uint32_t ladder, uint32_t step)
{
	if (index >= p_unit->stLadStep_sd.count)	/*超出范围*/
	{
		//WriteBBLog("index more than %d", motor->stLadStep_sp.count);
		return false;
	}
	p_unit->stLadStep_sd.ladder_step[index].ladder = ladder;
	p_unit->stLadStep_sd.ladder_step[index].step   = step;
	
	return true;
}

/*
  获取加减速台阶的步数
  --------------------------------
  入口：p_unit 电机结构体
  返回值：当前台阶步数
*/
static uint32_t _Calc_Cur_Step(DRV_STEPMOTOR_TYPE* p_unit)
{
	uint8_t loop = 0;
	uint32_t step = 0;
	DRV_STEPMOTOR_LADDERSTEPGROUP_TYPE* p = NULL;

	p = (p_unit->status == MOTOR_SPEEDUP) ? &(p_unit->stLadStep_sp) : &(p_unit->stLadStep_sd);	/*加速调用加速ladstep*/
	for (loop = 0; loop < p->count; loop++)
    {
		if (p_unit->curLadder <= p->ladder_step[loop].ladder)	/*当前台阶号小于该阶段台阶数的，获取该阶段的步数*/
        {
			step = p->ladder_step[loop].step;
            break;
        }
    }
	
	return step;
}

/* 申请电机运行频率表存储空间
  --------------------------------
  入口：count 台阶个数
  返回值：分配的内存地址
*/
static uint32_t* _CallocMotorFre(uint16_t count)
{
	uint32_t* p_fre = NULL;
	
    if(count > 200)		/*大于200，报错*/
    {
        return NULL;
    }
	
    p_fre = calloc(count, sizeof(uint32_t));	/*申请连续内存空间*/
	
    return p_fre;
}

/*
  计算加减速曲线频率表
  --------------------------------
  入口：moto_line 加减速曲线参数表，fre_arry 加减速曲线频率表
  返回值：无
*/
void CalcMotorFre(uint8_t subdivision, DRV_STEPMOTOR_LINE_TYPE* moto_line, uint32_t* fre_arry)
{
	double exp_start = 0.0f, exp_end = 0.0f, para_a = 0.0f, para_b = 0.0f, s_par = 0.0;
	int32_t loop = 0;
	
    if(fre_arry == NULL || moto_line->up_max_lader <= 1 || moto_line->s_par == 0)
    {
        return;
    }
    
	s_par     = moto_line->s_par / 100.0f;
	exp_start = exp(-1.0 / (double)s_par);
	exp_end   = exp(- ((double)(moto_line->up_max_lader) / (double)s_par));
	para_b    = (double)(moto_line->end_fre - moto_line->start_fre) / (double)(exp_start - exp_end);
	para_a    = moto_line->start_fre + para_b * exp_start;

	for (loop = 0; loop < moto_line->up_max_lader; loop++)
	{
        *(fre_arry + loop) = para_a - para_b * exp(-(loop + 1) / s_par);	/*计算S型曲线元素值（单位pps 整步每秒即Hz）*/
		*(fre_arry + loop) = 8000000 / (2 * (*(fre_arry + loop)) * subdivision);		/*定时器中断时基8MHz，再除以细分数，得到定时器溢出周期*/
	}
}

/* 申请台阶-微步结构体组存储空间
  --------------------------------
  入口：p_ladder_step_group 台阶-微步结构体组指针，count 成员个数
  返回值：分配的内存地址
*/
static DRV_STEPMOTOR_LADDERSTEP_TYPE* _CallocLadstep(DRV_STEPMOTOR_LADDERSTEPGROUP_TYPE* p_ladder_step_group, uint8_t count)
{
	p_ladder_step_group->ladder_step = (DRV_STEPMOTOR_LADDERSTEP_TYPE *)calloc(count, sizeof(DRV_STEPMOTOR_LADDERSTEP_TYPE));
    
	if((p_ladder_step_group->ladder_step) == NULL)
	{
		p_ladder_step_group->count = 0;
        return NULL;
	}
	else
	{
		p_ladder_step_group->count = count;
		return p_ladder_step_group->ladder_step;
	}
}

/* 下层接口函数
 ********************************************************/
 
 /* PWM台阶输出完成
  --------------------------------
 */
void __PWM_Done(DRV_PWM_TIMERDRIVEN_TYPE* p_unit)
{
	/*每台阶输出完成，坐标更新*/
	if(_gp_MotorUnits[p_unit->id]->dir_buffer == 0)
	{
		_gp_MotorUnits[p_unit->id]->position += (_gp_MotorUnits[p_unit->id]->step_pin->total_pulse - _gp_MotorUnits[p_unit->id]->step_pin->remaining_pulse / 2);
	}
	else
	{
		_gp_MotorUnits[p_unit->id]->position -= (_gp_MotorUnits[p_unit->id]->step_pin->total_pulse - _gp_MotorUnits[p_unit->id]->step_pin->remaining_pulse / 2);
	}
	
	switch(p_unit->id)
	{
		case MOTOR_1: _Motor_Move_Update(_gp_MotorUnits[MOTOR_1]); break;
		case MOTOR_2: _Motor_Move_Update(_gp_MotorUnits[MOTOR_2]); break;
		case MOTOR_3: _Motor_Move_Update(_gp_MotorUnits[MOTOR_3]); break;
		case MOTOR_4: _Motor_Move_Update(_gp_MotorUnits[MOTOR_4]); break;
		default: break;
	}
}
