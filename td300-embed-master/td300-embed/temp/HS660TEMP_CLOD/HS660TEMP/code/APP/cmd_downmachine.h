#ifndef _CMD_DOWNMACHINE_H_
#define _CMD_DOWNMACHINE_H_
/******************************************************************************

                  ��Ȩ���� (C), 2013-2023, �Ĵ��³�����Ƽ����޹�˾

 ******************************************************************************
  �� �� ��   : cmd_downmachine.h
  �� �� ��   : ����
  ��    ��   : deng
  ��������   : 2014��5��19��
  ����޸�   :
  ��������   : �������������������ͷ�ļ�
  �����б�   :
  �޸���ʷ   :
  1.��    ��   : 2014��5��19��
    ��    ��   : deng
    �޸�����   : �����ļ�

******************************************************************************/


#include "cmd_basetype.h"

#include "stdbool.h"
#include "stdint.h"

//������λ���汾�ϱ��ṹ��(��fpga�汾��)
//ֻ����CPU���fpga�İ汾��
typedef struct
{
    CMDINT16U cmd;
    CMDINT16U enstatustype; //�ϱ���������
    CMDINT32U dwnmachname;  //��λ������
    CMDINT8U  date_time[32];
    CMDINT8U  dwnmachin_ver[24];
    CMDINT32U fpga_ver;
}cmd_fpga_dwnmach_version_stru;

/*****begin of fault check module*****/
//------------------------Begin: ��˫ϲ3.23���--------------------------------------

/*����IDλ���ʽ*/
//typedef CMDINT32U xc8001_faultid_enum_u32;
//bit31-30:ID_type
//#define XC8001_FAULTID_IDTYPE_BITMAP  (30)

#pragma pack(1)
//typedef struct{

//}motor_compensation  ;

//--------------------------------------------------------

//#pragma warning(default:)
#endif
