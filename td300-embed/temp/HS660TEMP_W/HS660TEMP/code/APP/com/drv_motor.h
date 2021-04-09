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
//    uint32_t ladder;	//̨��
//    uint32_t step;		//΢��
//}LadderStep;
//typedef struct
//{
//    uint8_t count;//����̨��΢���ڴ��ĸ���
//    LadderStep* ladstep;//ָ�����̨��΢����ָ��
//    
//}ladstep;

//typedef struct
//{
//    uint8_t  status;   //���״̬
//    uint8_t  dir;      //����
//    uint8_t _mMType;//�������

//    uint32_t maxLadder_sp;     //����̨����
//    uint32_t maxLadder_sd;  //����̨����
//    uint32_t constStep;     //���ٲ���
//    uint32_t slowStep;      //���ٲ���
//    uint32_t slowFre;		//��������Ƶ��
//    int32_t  curLadder;     //��ǰ̨����
//    uint32_t *pFre;			//���Ƶ�ʱ�
//    uint32_t *pFre_sd;		//�������Ƶ�ʱ�

//    ladstep  stLadStep_sd;//����̨��-΢���ṹ��
//    ladstep  stLadStep_sp;//̨��-΢�����ýṹ��
//    uint32_t sd_total_step;	//���ٵ��ܲ���
//    uint32_t totalStep;
//    uint32_t curStep;

//    MoveFunc MMoveSP;		//����-����-����
//    MoveFunc MMoveSD;		//����
//    MoveFunc MMoveRL;		//����

//    DrvFunc  MDrv;
//    ConfigFunc MConfig;		//������ú���
//    MotorCtlFunc MEnableF;	//���ʹ��
//    MotorCtlFunc MDisableF;	//�������

//    configStep ConfigSP;	//����-��������
//    configStep ConfigSD;	//��������

//    actFunc PostSem;		//�ͷ��ź���

//}stMotor;

///*����Ӽ������ýṹ��*/
//typedef struct
//{
//	uint16_t    start_fre;			//����Ƶ��
//    uint16_t    end_fre;			//���Ƶ��
//	uint16_t    ech_lader_step;		//ÿ̨��΢��
//	uint16_t    up_max_lader;		//������̨��
//	uint16_t	s_par;				//S�����ߵ���״����
//    
//}MotorLineSt;

////�������״̬
//typedef enum {
//    drv_motor_idle = 0,		//�������̬
//    drv_motor_speedup = 1,	//�������̬
//    drv_motor_runing = 2,	//�������̬
//    drv_motor_speeddown = 3,//�������̬
//    drv_motor_slow = 4,		//�������̬

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


