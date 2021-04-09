#include "drv_motor.h"
//#include "eeprom_config.h"
//#include "bb_logservice.h"
//extern uint8_t fault_downID;
////计算加速的台阶微步值
//uint32_t calc_cur_step(stMotor* stThis)
//{
//    uint8_t loop = 0;
//	uint32_t step = 0;
//	ladstep* p = NULL;

//	p = (stThis->status == drv_motor_speedup) ? &stThis->stLadStep_sp : &stThis->stLadStep_sd;
//	for (loop = 0; loop < p->count; loop++)
//    {
//		if (stThis->curLadder < p->ladstep[loop].ladder)//只要满足就直接退出，不用继续查找
//        {
//			step = p->ladstep[loop].step;
//            break;
//        }
//    }
//    stThis->totalStep = step;
//	return step;
//}
///*计算减速台阶步数*/
//void calc_sd_step(stMotor* stThis, int32_t sd_ladder)
//{
//	if (sd_ladder == 0)//除法保护
//	{
//		stThis->stLadStep_sd.ladstep[1].ladder = sd_ladder;
//		stThis->stLadStep_sd.ladstep[1].step = 0;
//		stThis->stLadStep_sd.ladstep[0].ladder = 1;//保证进入该减速块
//		stThis->stLadStep_sd.ladstep[0].step = stThis->sd_total_step;
//	}
//	else
//	{
//		stThis->stLadStep_sd.ladstep[1].ladder = sd_ladder;
//		stThis->stLadStep_sd.ladstep[1].step = stThis->sd_total_step / sd_ladder;
//		stThis->stLadStep_sd.ladstep[0].ladder = stThis->sd_total_step - stThis->stLadStep_sd.ladstep[1].step * sd_ladder;
//		stThis->stLadStep_sd.ladstep[0].step = stThis->stLadStep_sd.ladstep[1].step + 1;
//	}
//}
//void MotorDrv(void *pstThis)
//{
//    stMotor* stThis = pstThis;
//    uint32_t step = 0;

//    switch (stThis->status)
//    {
//    case drv_motor_speedup:
//        stThis->curLadder = (stThis->curLadder > stThis->maxLadder_sp) ? stThis->maxLadder_sp : stThis->curLadder;
//    
//		stThis->MConfig(calc_cur_step(stThis), stThis->pFre[stThis->curLadder], stThis->dir);
//        stThis->status = (++stThis->curLadder >= stThis->maxLadder_sp) ? drv_motor_runing : drv_motor_speedup;
//        break;
//    case drv_motor_runing:
//        stThis->MConfig(stThis->constStep, stThis->pFre[stThis->curLadder], stThis->dir);
//        stThis->status = drv_motor_speeddown;
//        stThis->totalStep = stThis->constStep;
//        break;

//    case drv_motor_speeddown:
//        stThis->curLadder = (stThis->curLadder > stThis->maxLadder_sd) ? stThis->maxLadder_sd : stThis->curLadder;
//    
//        stThis->status = (--stThis->curLadder > 0) ? drv_motor_speeddown : drv_motor_idle;
//        stThis->curLadder = (stThis->curLadder < 0) ? 0 : stThis->curLadder;

//		step = calc_cur_step(stThis);

//		step = (stThis->status == drv_motor_idle && stThis->slowStep != 0) ? stThis->slowStep : step;
//        stThis->MConfig(step, stThis->pFre_sd[stThis->curLadder], stThis->dir);
//        stThis->totalStep = step;
//        break;

//    case drv_motor_slow:
//		stThis->MConfig(stThis->constStep, stThis->slowFre, stThis->dir);
//        stThis->status = drv_motor_idle;
//        stThis->totalStep = stThis->constStep;
//        break;

//    case drv_motor_idle:
//        stThis->PostSem(stThis);
//        return;
//    default:
//        return;
//    }

//    stThis->MEnableF((void*)stThis);
//    return;
//}

//void MotorMoveSP(void * pstThis, uint8_t dir, uint32_t step, uint32_t slowStep)
//{
//    stMotor* stThis = pstThis;

//	stThis->dir = dir;
//	stThis->curLadder = 0;
//	stThis->slowStep = slowStep;
//	stThis->status = drv_motor_speedup;
//    stThis->curStep = 0;

//    if (stThis->ConfigSP != NULL)
//    {
//		stThis->ConfigSP(stThis, step);
//    }
//    
//    stThis->MDrv(stThis);
//	return;
//}

//void MotorMoveSD(void * pstThis, uint8_t dir, uint32_t step, uint32_t slowStep)
//{
//    stMotor* stThis = pstThis;

//	stThis->dir = dir;
//	stThis->slowStep = slowStep;
//	stThis->status = drv_motor_speeddown;
//    stThis->curStep = 0;
//    if (stThis->ConfigSD != NULL)
//    {
//		stThis->ConfigSD(stThis, step);
//    }
//    
//	stThis->MDrv(stThis);
//    return;
//}

//void MotorMoveRL(void * pstThis, uint8_t dir, uint32_t step, uint32_t fre)
//{
//    stMotor* stThis = pstThis;

//	stThis->dir = dir;
//	stThis->curLadder = 0;
//	stThis->slowStep = 0;
//	stThis->constStep = step;
//	stThis->slowFre = fre;
//	stThis->status = drv_motor_slow;
//   stThis->curStep = 0;

//	stThis->MDrv(stThis);
//    return;
//}
////申请电机频率内存
//uint32_t callocMotorFre(uint16_t num, fre_t **fre)
//{
//    if(num > 200)
//    {
//        //WriteBBLog("callocMotorFre__MFreOut200");
//        return 1;
//    }
//    if(*fre != NULL)
//    {
//        free(*fre);
//        *fre = NULL;
//    }
//    *fre = (fre_t *)calloc(1, num * sizeof(fre_t));
//    if(*fre == NULL)
//    {
//        //WriteBBLog("callocMotorFre__CallocMFreFil");
//        return 1;
//    }
//    return 0;
//}
///*计算加减速曲线函数*/
//void calcMotorLine(void *pstThis, fre_t *fre_arry)
//{
//	MotorLineSt* stThis = (MotorLineSt*)pstThis;
//	float exp_start = 0.0f, exp_end = 0.0f, para_a = 0.0f, para_b = 0.0f, s_par = 0.0;
//	int32_t loop = 0;
//	
//    if(fre_arry == NULL || stThis->up_max_lader <= 1 || stThis->s_par == 0)
//    {
//        //WriteBBLog("calcMotorLine__MotorParaErr");
//        return;
//    }
//    
//    s_par = stThis->s_par / 100.0f;
//	exp_start = exp(-1.0 / s_par);
//	exp_end = exp(-stThis->up_max_lader / s_par);
//	para_b = (float)(stThis->end_fre - stThis->start_fre)/(exp_start - exp_end);
//	para_a = stThis->start_fre + para_b*exp_start;

//	for (loop = 0; loop < stThis->up_max_lader; loop++)
//	{
//        *(fre_arry + loop) = para_a - para_b*exp(-(loop + 1) / s_par);
//        *(fre_arry + loop) = ChansFre(*(fre_arry + loop));//转换为细分
//	}

//	return ;
//}

//LadderStep *calloc_ladstep(stMotor* stThis, drv_motor_state_enum_u8 status, uint8_t count)
//{
//	LadderStep *pLadderStep = (LadderStep *)calloc(count, sizeof(LadderStep));
//    
//	if (NULL == pLadderStep)
//	{
//		//WriteBBLog("pLS is NULL.");
//        return NULL;
//	}
//	if (status == drv_motor_speedup)
//	{
//		stThis->stLadStep_sp.count = count;
//	}
//	if (status == drv_motor_speeddown)
//	{
//		stThis->stLadStep_sd.count = count;
//	}
//	return pLadderStep;
//}

//bool config_ladstep(stMotor* stThis, uint8_t index, uint32_t ladder, uint32_t step)
//{
//	if (index >= stThis->stLadStep_sp.count)
//	{
//		//WriteBBLog("index more than %d", stThis->stLadStep_sp.count);
//		return false;
//	}
//	stThis->stLadStep_sp.ladstep[index].ladder = ladder;
//	stThis->stLadStep_sp.ladstep[index].step = step;

//	return true;
//}

