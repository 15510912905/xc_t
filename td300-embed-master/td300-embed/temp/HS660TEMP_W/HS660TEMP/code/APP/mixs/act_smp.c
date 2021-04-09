//#include "act_smp.h"
//#include "drv_motor.h"
//#include "eeprom_config.h"
//#include "app_eeprom.h"
//#include "ndl_operate.h"
//#include "cmd_funs.h"
//#include "led.h"
//#include "product.h"

//#define ActOutTime 5000
//#define ActOutTime_Reset (10000)   //һ�㳬ʱʱ��5s

//#define RTT_RST_ROTATE_STEP     (23)	//ˮƽ�����λ�ƶ�����
//#define VRT_RST_DOWN_STEP       (285)	//��ֱ�����λ�ƶ�����
//#define INJ_RST_IN_STEP         (20000)	//ע��õ����λ�������� 2000

//#define sdisk_clck Dir_aclckW
//#define sdisk_aclck Dir_clckw

//OS_EVENT *SDisk_Sem = NULL;	//ע��õ���ź���
//stMotor stMotorINJ;
//stMotor *g_pSdisk = &stMotorINJ;

////�����ڽ���ģ�add by zhaoli 
//stMotor *g_pSstir = &stMotorINJ;//


//uint8_t g_injflag = 0;	//ע��õ����־

//volatile uint8_t g_aupurp_smp = 1;       /*ˮƽλ��Ŀ��λ��*/
//volatile uint8_t g_aucurr_smp = 1;       /*ˮƽλ�õ�ǰλ��*/


///*����ע��ü������к���	*/
//void configSP_INJ(void * pstThis, uint32_t step)
//{
//    stMotor* stThis = pstThis;
//    MotorLineSt* g_mps = NULL;
//    fre_t *pFre = NULL;
//    uint32_t sp_ladder;

//    g_injflag = 1;
//    //����
//    g_mps = (step > LINE_SWITCH_INJ) ? MotorLine(injMotorHig) : MotorLine(injMotorLow);
//    pFre = (step > LINE_SWITCH_INJ) ? INJhigFre : INJlowFre;
//    config_ladstep(g_pSdisk, 0, g_mps->up_max_lader, g_mps->ech_lader_step);
//    stThis->pFre = pFre;
//    sp_ladder = (step * 2) / (g_mps->ech_lader_step * 5);
//    stThis->maxLadder_sp = sp_ladder > g_mps->up_max_lader ? g_mps->up_max_lader : sp_ladder;
//    stThis->maxLadder_sp = (stThis->maxLadder_sp > 1) ? (stThis->maxLadder_sp - 1) : 0;
//    //����
//    if (step > 2 * stThis->maxLadder_sp * g_mps->ech_lader_step)
//    {
//        stThis->constStep = step - 2 * stThis->maxLadder_sp * g_mps->ech_lader_step;
//    }
//    else
//    {
//        stThis->constStep = 1;
//    }
//    //����
//    stThis->maxLadder_sd = stThis->maxLadder_sp;
//    stThis->pFre_sd = pFre;
//    stThis->sd_total_step = stThis->maxLadder_sd * g_mps->ech_lader_step;
//    calc_sd_step(stThis, stThis->maxLadder_sd);
//    return;
//}

///*���ý���������к���*/
//void configSP_STIR(void * pstThis, uint32_t step)
//{
//    stMotor* stThis = pstThis;
//    MotorLineSt* g_mps = NULL;
//    fre_t *pFre = NULL;
//    uint32_t sp_ladder;

//    g_injflag = 1;
//    //����
//    g_mps = (step > LINE_SWITCH_INJ) ? MotorLine(stirMotorHig) : MotorLine(stirMotorLow);
//    pFre = (step > LINE_SWITCH_INJ) ? STRhigFre : STRlowFre;
//    config_ladstep(g_pSstir, 0, g_mps->up_max_lader, g_mps->ech_lader_step);
//    stThis->pFre = pFre;
//    sp_ladder = (step * 2) / (g_mps->ech_lader_step * 5);
//    stThis->maxLadder_sp = sp_ladder > g_mps->up_max_lader ? g_mps->up_max_lader : sp_ladder;
//    stThis->maxLadder_sp = (stThis->maxLadder_sp > 1) ? (stThis->maxLadder_sp - 1) : 0;
//    //����
//    if (step > 2 * stThis->maxLadder_sp * g_mps->ech_lader_step)
//    {
//        stThis->constStep = step - 2 * stThis->maxLadder_sp * g_mps->ech_lader_step;
//    }
//    else
//    {
//        stThis->constStep = 1;
//    }
//    //����
//    stThis->maxLadder_sd = stThis->maxLadder_sp;
//    stThis->pFre_sd = pFre;
//    stThis->sd_total_step = stThis->maxLadder_sd * g_mps->ech_lader_step;
//    calc_sd_step(stThis, stThis->maxLadder_sd);
//    return;
//}



///*�ͷ�ע��õ���ź�������	*/
//void postINJ(void * pstThis)
//{
//    stMotor* stThis = pstThis;

//    stThis->dir = DIR_IDLE;
//    if (g_injflag == 1)
//    {
//        g_injflag = 0;
//        OSSemPost(SDisk_Sem);
//    }
//}

///*�ͷŽ���õ���ź�������	*/
//void postSTR(void * pstThis)
//{
//    stMotor* stThis = pstThis;

//    stThis->dir = DIR_IDLE;
//    if (g_injflag == 1)
//    {
//        g_injflag = 0;
//        OSSemPost(SDisk_Sem);
//    }
//}


//void _creatSem_smp()
//{
//    SDisk_Sem = OSSemCreate(0);
//}

//void _init_motor_smp()
//{
//    /*����ע��õ����ʼ�����ô���*/
//    g_pSdisk->_mMType = NDL_INJ_MOTOR;
//    g_pSdisk->stLadStep_sp.ladstep = calloc_ladstep(g_pSstir, drv_motor_speedup, 1);//����ע��õ������̨��΢���ڴ�
//    g_pSdisk->stLadStep_sd.ladstep = calloc_ladstep(g_pSstir, drv_motor_speeddown, 2);//����ע��õ������̨��΢���ڴ�
//    g_pSdisk->MMoveSP = MotorMoveSP;
//    g_pSdisk->MMoveSD = MotorMoveSD;
//    g_pSdisk->MMoveRL = MotorMoveRL;

////    g_pSdisk->MDrv = MotorDrv;
////    g_pSdisk->MConfig = MConfig_INJ;
////    g_pSdisk->MEnableF = MEnableFun;
////    g_pSdisk->MDisableF = MDisableFun;

//    g_pSdisk->ConfigSD = NULL;
//    g_pSdisk->ConfigSP = configSP_INJ;
//    g_pSdisk->PostSem = postINJ;
//    g_pSdisk->dir = DIR_IDLE;
//}

////add by zhaoL start
//void _init_motor_stir()
//{
//    /*����ע��õ����ʼ�����ô���*/
//    g_pSstir->_mMType = NDL_INJ_MOTOR;
//    g_pSstir->stLadStep_sp.ladstep = calloc_ladstep(g_pSstir, drv_motor_speedup, 1);//����ע��õ������̨��΢���ڴ�
//    g_pSstir->stLadStep_sd.ladstep = calloc_ladstep(g_pSstir, drv_motor_speeddown, 2);//����ע��õ������̨��΢���ڴ�
//    g_pSstir->MMoveSP = MotorMoveSP;
//    g_pSstir->MMoveSD = MotorMoveSD;
//    g_pSstir->MMoveRL = MotorMoveRL;

////    g_pSstir->MDrv = MotorDrv;
////    g_pSstir->MConfig = MConfig_INJ;
////    g_pSstir->MEnableF = MEnableFun;
////    g_pSstir->MDisableF = MDisableFun;

//    g_pSstir->ConfigSD = NULL;
//    g_pSstir->ConfigSP = configSP_STIR;
//    g_pSstir->PostSem = postINJ;
//    g_pSstir->dir = DIR_IDLE;
//}
////ADD By zhaoL end



//bool _calcDirStep_smp(uint8_t *dir, uint32_t *journey)
//{
//    if (g_aupurp_smp > 10 || g_aupurp_smp<1 || g_aucurr_smp == g_aupurp_smp)
//    {
//        return false;
//    }
//    *dir = (g_aupurp_smp > g_aucurr_smp) ? Dir_clckw : Dir_aclckW;
//    *journey = abs(g_aupurp_smp - g_aucurr_smp) * 3500 / 3.14 / 20.7 * 8;
//    return true;
//}

//uint32_t act_reset_smp(void *cmd)
//{
//    uint8_t err = OS_ERR_NONE;
//    CleanSem_smp();
//    if (OP_INJ == SHELTER_INJ_YES)//�����������Ƿ��� δ���ڵ�
//    {
//        g_injflag = 1;
////			  g_pSdisk->MMoveSP(g_pSdisk, Dir_clckw, INJ_step_num[INJ_SOB_1], 0);//
//        g_pSdisk->MMoveRL((void*)g_pSdisk, Dir_clckw, 1000, ChansFre(1000));//Dir_clckw
//        OSSemPend(SDisk_Sem, ActOutTime_Reset, &err);
//        if (OS_ERR_NONE != err || OP_INJ == SHELTER_INJ_NO)//��������
//        {
////            CmdReturn(CR_Failed, 0, NULL, Unit_Smp);
//            return true;
//        }
//        OSTimeDly(200);
//    }
//		g_pSdisk->MMoveSP(g_pSdisk, Dir_aclckW, INJ_RST_IN_STEP * 5, 0);//Dir_aclckW
////    g_pSdisk->MMoveRL((void*)g_pSdisk, Dir_aclckW, INJ_RST_IN_STEP * 5, ChansFre(600));// Dir_clckw :ע���������  Dir_aclckW��ע���������
//    OSSemPend(SDisk_Sem, ActOutTime_Reset, &err);
//    g_aucurr_smp = 1;
////    ActComRes(cmd, err, Unit_Smp);//��������
//		 return (OS_ERR_NONE == err) ? TRUE : FALSE;
////    return true;
//}

//uint32_t act_move_smp(void *cmd)
//{
//    uint8_t err = OS_ERR_NONE;
//    uint8_t     dir = DIR_IDLE;
//    uint32_t    journey = 0;
//    cmd_stMove *pCmd = (cmd_stMove*)cmd;
//    CleanSem_smp();
//    if (pCmd->usMoveType == Move_PosDsk)
//    {
//        g_aupurp_smp = pCmd->usPara;
//    }
//    else if (pCmd->usMoveType == Move_PosNum)
//    {
//        g_aupurp_smp = (pCmd->usDir == Dir_clckw) ? (g_aucurr_smp + pCmd->usPara) : (g_aucurr_smp - pCmd->usPara);
//    }

//    if (_calcDirStep_smp(&dir, &journey))
//    {
//        g_pSdisk->MMoveSP((void*)g_pSdisk, dir, journey, 0);
//        OSSemPend(SDisk_Sem, ActOutTime, &err);
//        g_aucurr_smp = (err == OS_ERR_NONE) ? g_aupurp_smp : g_aucurr_smp;
//    }
//    ActComRes(cmd, err, Unit_Smp);
//	return 0;
//}
////�ù���  PB7
//void EXTI9_5_IRQHandler(void)        
//{
//    if (EXTI_GetITStatus(EXTI_Line7) != RESET)
//    {
//        EXTI_ClearITPendingBit(EXTI_Line7);
//			if (g_pSdisk->dir == NDL_OUT)
//        {
//            g_injflag = 1;
//            g_pSdisk->MDisableF((void *)g_pSdisk);
//            g_pSdisk->PostSem(g_pSdisk);
//        }
//        
//    }
//}
// 
//////�ù���  PD2
////void EXTI2_IRQHandler(void)
////{
////    LED2 = !LED2;
////    if (EXTI_GetITStatus(EXTI_Line2) != RESET)
////    {
////        EXTI_ClearITPendingBit(EXTI_Line2);
////        
////    }
////}

//bool _procStepTimer_smp(stMotor *pMotor, TIM_TypeDef * Timer)
//{
//    if (TIM_GetITStatus(Timer, TIM_IT_Update) != RESET)
//    {
//        TIM_ClearITPendingBit(Timer, TIM_IT_Update);
//        if (++(pMotor->curStep) >= pMotor->totalStep * 2 * 4)
//        {
//            pMotor->MDisableF((void*)pMotor);
//            pMotor->curStep = 0;
//            pMotor->MDrv((void*)pMotor);//������һ��̨��
//            return FALSE;
//        }
//        return TRUE;
//    }
//    return FALSE;
//}

//void TIM4_IRQHandler(void)
//{
//    if (TRUE == _procStepTimer_smp(g_pSdisk, Timer_INJ))
//    {
//        MINJ_Step = !MINJ_Step;//step���� �������  
//    }
//}
//void CleanSem_smp()
//{
//    uint8_t err = 0;
//    OSSemSet(SDisk_Sem, 0, &err);
//}

//void Act_init_smp()
//{
//    _creatSem_smp();

//    _init_motor_smp();
////    _init_motor_stir();//������ר��
//}

////ע�������1 
//bool act_sorb_inject(void *cmd)
//{
// 	 uint8_t err = OS_ERR_NONE;
//	 uint8_t     dir = DIR_IDLE;
//    CleanSem_smp();
//    g_injflag = 1;
//    g_pSdisk->MMoveSP(g_pSdisk, Dir_clckw, INJ_step_num[INJ_SOB_1], 0);// Dir_clckw :ע���������  Dir_aclckW��ע���������
//   OSSemPend(SDisk_Sem, ActOutTime, &err);
////    ActComRes(cmd, err, Unit_Mix);
//    return TRUE;
//}

////ע�������2 
//bool act_sorb1_inject(void *cmd)
//{
//    uint8_t i;
// 	 uint8_t err = OS_ERR_NONE;
//	 uint8_t     dir = DIR_IDLE;
//    CleanSem_smp();
//    g_injflag = 1;
//   
//    g_pSdisk->MMoveSP(g_pSdisk, Dir_clckw, INJ_step_num[INJ_SOB_2], 0);// Dir_clckw :ע���������  Dir_aclckW��ע���������
//   OSSemPend(SDisk_Sem, ActOutTime, &err);
////    ActComRes(cmd, err, Unit_Mix);
//    return TRUE;
//}

//bool act_stir_up(uint32_t step)
//{
//    uint8_t i;
// 	 uint8_t err = OS_ERR_NONE;
//	 uint8_t     dir = DIR_IDLE;
//    CleanSem_smp();
//    g_injflag = 1;
////    g_pSdisk->ConfigSP = configSP_STIR;//���ٽ���
//    g_pSdisk->MMoveSP(g_pSdisk, Dir_aclckW, step, 0);// Dir_clckw :ע���������  Dir_aclckW��ע���������
//    OSSemPend(SDisk_Sem, ActOutTime, &err);
////    g_pSdisk->ConfigSP = configSP_INJ;
//    return TRUE;
//}

//bool act_stir_down(uint32_t step)
//{
//    uint8_t i;
// 	 uint8_t err = OS_ERR_NONE;
//	 uint8_t     dir = DIR_IDLE;
//    CleanSem_smp();
//    g_injflag = 1;
////   g_pSdisk->ConfigSP = configSP_STIR;//���ٽ���
//   g_pSdisk->MMoveSP(g_pSdisk, Dir_clckw, step, 0);// Dir_clckw :ע���������  Dir_aclckW��ע���������
//   OSSemPend(SDisk_Sem, ActOutTime, &err);
////  g_pSdisk->ConfigSP = configSP_INJ;
//    return TRUE;
//}

////��������
//bool act_stir_up_height_speed(uint32_t step)
//{
//    uint8_t i;
// 	 uint8_t err = OS_ERR_NONE;
//	 uint8_t     dir = DIR_IDLE;
//    CleanSem_smp();
//    g_injflag = 1;
//    g_pSdisk->ConfigSP = configSP_STIR;//���ٽ���
//    g_pSdisk->MMoveSP(g_pSdisk, Dir_aclckW, step, 0);// Dir_clckw :ע���������  Dir_aclckW��ע���������
//    OSSemPend(SDisk_Sem, ActOutTime, &err);
//    g_pSdisk->ConfigSP = configSP_INJ;
//    return TRUE;
//}

//bool act_stir_smp1(void *cmd)
//{
//   uint8_t i;
//    char* str;
//    act_stir_up(INJ_step_num[INJ_SOB_1]);
//   for(i=0;i<5;i++)
//    {
//    act_stir_down(INJ_step_num[INJ_STR_1]); 
//    act_stir_up(INJ_step_num[INJ_STR_1]);
//    }
//}

//bool act_stir_smp2(void *cmd)
//{
//   uint8_t i;
//    char* str;
//    act_stir_up(INJ_step_num[INJ_SOB_2]);
//   for(i=0;i<5;i++)
//    {
//    act_stir_down(INJ_step_num[INJ_STR_1]); 
//    act_stir_up(INJ_step_num[INJ_STR_1]);
//    }
//}

////ע�������3
//bool act_sorb2_inject(void *cmd)
//{
// 	 uint8_t err = OS_ERR_NONE;
//	 uint8_t     dir = DIR_IDLE;
//    CleanSem_smp();
//    g_injflag = 1;
//    g_pSdisk->MMoveSP(g_pSdisk, Dir_clckw, INJ_step_num[INJ_SOB_3], 0);// Dir_clckw :ע���������  Dir_aclckW��ע���������
//   OSSemPend(SDisk_Sem, ActOutTime, &err);
////    ActComRes(cmd, err, Unit_Mix);
//    return TRUE;
//}

////����
//bool act_expel_inject(void *cmd)
//{
// 	 uint8_t err = OS_ERR_NONE;
//	 uint8_t     dir = DIR_IDLE;
//    CleanSem_smp();
//    g_injflag = 1;
//    g_pSdisk->MMoveSP(g_pSdisk, Dir_aclckW, INJ_step_num[INJ_EXPEL_1], 0);// Dir_clckw :ע���������  Dir_aclckW��ע���������
//   OSSemPend(SDisk_Sem, ActOutTime, &err);
////    ActComRes(cmd, err, Unit_Mix);
//    return TRUE;
//}


////ע��õ�����
//bool act_up_inject(void *cmd)
//{
//	 uint8_t err = OS_ERR_NONE;
//	 uint8_t     dir = DIR_IDLE;
//   CleanSem_smp();
//    g_injflag = 1;
//    g_pSdisk->MMoveSP(g_pSdisk, Dir_clckw, 2000, 0);// Dir_clckw :ע���������  Dir_aclckW��ע���������
//   OSSemPend(SDisk_Sem, ActOutTime, &err);
////    ActComRes(cmd, err, Unit_Mix);
//    return TRUE;

//}
