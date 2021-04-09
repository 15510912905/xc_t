#include "ndl_operate.h"
#include "drv_motor.h"
#include "product.h"

uint32_t total_vrt_step = 0;

//void MConfig_INJ(uint32_t pulse, uint32_t fre, uint8_t dir)
//{
//    TIM_SetAutoreload(Timer_INJ, fre);
//    MINJ_Dir = dir;
//    return;
//}

//void MConfig_RTT(uint32_t pulse, uint32_t fre, uint8_t dir)
//{
//    TIM_SetAutoreload(Timer_RTT, fre);
//    MRTT_Dir = dir;
//    return;
//}

//void MConfig_VRT(uint32_t pulse, uint32_t fre, uint8_t dir)
//{
//    TIM_SetAutoreload(Timer_VRT, fre);
//    MVRT_Dir = dir;
//    return;
//}

//void MEnable(motortype_enum_u8 motortype)
//{
//    if (NDL_VRT_MOTOR == motortype)
//    {
//        MVRT_EN = p_MEnable;
//        MVRT_Step = !(MINJ_Step);
//        TIM_Cmd(Timer_VRT, ENABLE);
//    }
//    else if (NDL_RTT_MOTOR == motortype)
//    {
//        MRTT_EN = p_MEnable;
//        MRTT_Step = !(MRTT_Step);
//        TIM_Cmd(Timer_RTT, ENABLE);
//    }
//    else if(NDL_INJ_MOTOR == motortype)
//    {
//        MINJ_EN = p_MEnable;
//        MINJ_Step = !(MINJ_Step);
//        TIM_Cmd(Timer_INJ, ENABLE);
//    }
//}

//void MDisable(motortype_enum_u8 motortype)
//{
//    uint32_t motorstep = 0;
//    if (motortype == NDL_VRT_MOTOR)
//    {
//        //MVRT_EN = p_MDisable;
//        MVRT_CT = p_MHoled;
//        TIM_Cmd(Timer_VRT, DISABLE);
//        TIM_ClearITPendingBit(Timer_VRT, TIM_IT_Update);
//        total_vrt_step += motorstep;
//    }
//    else if (motortype == NDL_RTT_MOTOR)
//    {
//        //MRTT_EN = p_MDisable;
//        MRTT_CT = p_MHoled;
//        TIM_Cmd(Timer_RTT, DISABLE);
//        TIM_ClearITPendingBit(Timer_RTT, TIM_IT_Update);
//    }
//    else if (motortype == NDL_INJ_MOTOR)
//    {
//        //MINJ_EN = p_MDisable;
//        MINJ_CT = p_MHoled;
//        TIM_Cmd(Timer_INJ, DISABLE);
//        TIM_ClearITPendingBit(Timer_INJ, TIM_IT_Update);
//    }
//}

//void ReadOPTstate(uint8_t *optotype)
//{

//}
//void ReadMStep_IRQ(uint8_t * motorirt)
//{

//}
//void CleanMStep_IRQ(void)
//{

//}
//void ReadOPT_IRQ(uint8_t *optoirt)
//{

//}
//void CleanOPT_IRQ(void)
//{

//}

//void WASH_start(void)
//{

//}

//void WASH_outside(void)
//{

//}

//void WASH_stop(void)
//{
//    //writebyte_emc(CHIP0, MR_iWash, CLOSE);//关内壁清洗阀
//#ifdef SAMPLENDL
//	OSTimeDly(100);
//#else
//	OSTimeDly(200);
//#endif 
//    //writebyte_emc(CHIP0, MR_oWash, CLOSE);//关外壁清洗阀
//    return;
//}

//void MEnableFun(void * pstThis)
//{
//    stMotor* stThis = pstThis;
//    MEnable(stThis->_mMType);
//    return;
//}

//void MDisableFun(void *pstThis)
//{
//    stMotor* stThis = pstThis;
//    MDisable(stThis->_mMType);
//    return;
//}
