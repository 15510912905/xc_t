#pragma once

#include "property.h"
#include "includes.h"

#define MOTOR_MAX_STEP		  (0xffffu)  //1000
#define FPAG_CLK             (60000000)

//void MConfig_INJ(uint32_t pulse, uint32_t fre, uint8_t dir);
//void MConfig_RTT(uint32_t pulse, uint32_t fre, uint8_t dir);
//void MConfig_VRT(uint32_t pulse, uint32_t fre, uint8_t dir);

//void WASH_stop(void);
//void WASH_outside(void);
//void WASH_start(void);

//void CleanMStep_IRQ(void);
//void CleanOPT_IRQ(void);
//void ReadOPT_IRQ(uint8_t *optoirt);
//void ReadMStep_IRQ(uint8_t * motorirt);
//void ReadOPTstate(uint8_t *optotype);

//void MDisable(uint8_t motortype);
//void MEnable(uint8_t motortype);

//void MEnableFun(void * pstThis);
//void MDisableFun(void *pstThis);


//enum MR_enum  //�Ĵ�������
//{
//    MR_mStep1 = 0x01FC,	//��ֱ��ˮƽ������
//    MR_mStep2 = 0x01FD,
//    MR_mStep3 = 0x01FE,
//    MR_mStep4 = 0x01FF,

//    MR_injStep1 = 0x0130,	//��������
//    MR_injStep2 = 0x0131,
//    MR_injStep3 = 0x0132,
//    MR_injStep4 = 0x0133,

//    MR_mFre1 = 0x00BC,	//��ֱ��ˮƽ���Ƶ��
//    MR_mFre2 = 0x00BD,
//    MR_mFre3 = 0x00BE,
//    MR_mFre4 = 0x00BF,

//    MR_injFre1 = 0x00DC,	//�õ��Ƶ��
//    MR_injFre2 = 0x00DD,
//    MR_injFre3 = 0x00DE,
//    MR_injFre4 = 0x00DF,

//    MR_mChoose = 0x01F8,		//��ֱˮƽѡ��:1����ֱ��0��ˮƽ	

//    MR_vrtEnable = 0x00B8,	//��ֱʹ��λ 1ʹ�ܣ�0��ֹ
//    MR_rttEnable = 0x00B8,	//ˮƽʹ��
//    MR_injEnable = 0x00D8,	//��ʹ��

//    MR_vrtDir = 0x00C4,	//��ֱ���� 0�����ϣ�  1������
//    MR_rttDir = 0x00B4,	//��ת���� 0��˳ʱ�룬1����ʱ��
//    MR_injDir = 0x00D4,	//ע�������� 0�����ϣ�1������

//    MR_vrtOPTstate = 0x0158,	//��ֱ��ʼλ����״̬
//    MR_rttOPTDstate = 0x0154,	//ˮƽλ�����̹���״̬
//    MR_rttOPTIstate = 0x0150,	//ˮƽ��ʼλ����״̬
//    MR_injOPTstate = 0x015C,	//�ù���״̬	

//    MR_mStep_IRQ = 0x00AC,		//�ⲿ�ж�2(����ж�)

//    MR_vrtOPT_IRQ = 0x020c,	//��ֱ�����ж�
//    MR_rttOPTD_IRQ = 0x020a,	//ˮƽ���̹����ж�
//    MR_rttOPTI_IRQ = 0x0208,	//ˮƽ��ʼ�����ж�
//    MR_injOPTD_IRQ = 0x020e,	//�ù����ж�

//    MR_vrtStepCount_ll = 0x0240,//��ֱ������������Ĵ���
//    MR_vrtStepCount_lh = 0x0241,
//    MR_vrtStepCount_hl = 0x0242,
//    MR_vrtStepCount_hh = 0x0243,
//    
//    MR_rttIrqReg = 0x022E,  //ˮƽ��ת�������ʽ�Ĵ���

//    MR_iWash = 0x01C4,	//�ڱ���ϴ��		1�򿪣�0�ر�
//    MR_bWash = 0x01F4,	//�ڱ���ϴ������
//    MR_oWash = 0x0168,	//�����ϴ��		1�򿪣�0�ر�

//};

//extern uint32_t total_vrt_step;

