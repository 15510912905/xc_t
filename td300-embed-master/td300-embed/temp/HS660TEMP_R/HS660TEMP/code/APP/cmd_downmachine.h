#ifndef _CMD_DOWNMACHINE_H_
#define _CMD_DOWNMACHINE_H_
/******************************************************************************

                  版权所有 (C), 2013-2023, 四川新成生物科技有限公司

 ******************************************************************************
  文 件 名   : cmd_downmachine.h
  版 本 号   : 初稿
  作    者   : deng
  生成日期   : 2014年5月19日
  最近修改   :
  功能描述   : 交互命令基本数据类型头文件
  函数列表   :
  修改历史   :
  1.日    期   : 2014年5月19日
    作    者   : deng
    修改内容   : 创建文件

******************************************************************************/


#include "cmd_basetype.h"

#include "stdbool.h"
#include "stdint.h"

//定义下位机版本上报结构体(带fpga版本号)
//只有主CPU会带fpga的版本号
typedef struct
{
    CMDINT16U cmd;
    CMDINT16U enstatustype; //上报数据类型
    CMDINT32U dwnmachname;  //下位机名称
    CMDINT8U  date_time[32];
    CMDINT8U  dwnmachin_ver[24];
    CMDINT32U fpga_ver;
}cmd_fpga_dwnmach_version_stru;

/*****begin of fault check module*****/
//------------------------Begin: 吴双喜3.23添加--------------------------------------

/*故障ID位域格式*/
//typedef CMDINT32U xc8001_faultid_enum_u32;
//bit31-30:ID_type
//#define XC8001_FAULTID_IDTYPE_BITMAP  (30)

#pragma pack(1)
//typedef struct{

//}motor_compensation  ;

//--------------------------------------------------------

//#pragma warning(default:)
#endif
