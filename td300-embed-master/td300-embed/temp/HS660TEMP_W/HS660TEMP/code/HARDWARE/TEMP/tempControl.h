/******************************************************************************

                  ��Ȩ���� (C), 2013-2023, �Ĵ��³�����Ƽ����޹�˾

 ******************************************************************************
  �� �� ��   : tempcontrol.h
  �� �� ��   : ����
  ��    ��   : Gao.J
  ��������   : 2014��5��26��
  ����޸�   :
  ��������   : tempcontrol.c ��ͷ�ļ�
  �����б�   :
******************************************************************************/
#ifndef  __TEMPCONTROL_H__
#define  __TEMPCONTROL_H__
#include <stdint.h>

//��׼�¶�ֵ
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

