/********************************************************
   版权所有 (C), 2001-2100, 四川新健康成生物股份有限公司
  -------------------------------------------------------

				嵌入式开发平台——指令调度器

  -------------------------------------------------------
   文 件 名   : Prt_CmdScheduler.h
   版 本 号   : V1.0.0.0
   作    者   : ryc
   生成日期   : 2019年07月17日
   功能描述   : 实现指令与处理函数的对应调度
   依赖于     : 
 ********************************************************/
 
#include "Prt_CmdScheduler.h"

/* 接口函数
 ********************************************************/

/* 指令调度器
  ----------------------------------
  入口：收到的指令，指令注册表
  返回值：true，执行完成，false，未执行
*/
bool Prt_CmdScheduler(DRV_CAN_RX_BUFFER_TYPE* p_msg, CMD_REGEDIT_TYPE* p_cmd_regedit)
{
	if(true == dlist_foreach(p_cmd_regedit->dlist, _Prt_Cmd_DoFunc, p_msg))	
	{
		return true;	/*已经执行*/
	}
	else
	{
		return false;	/*未执行*/
	}
}

/* 内部函数
 ********************************************************/

/* 指令执行函数
  ----------------------------------
  入口：收到的指令，指令注册表节点
  返回值：true，执行完成，false，未执行
*/
static bool _Prt_Cmd_DoFunc(void *ctx, void *data)
{
	CMD_REGEDIT_NODE_TYPE*  p = (CMD_REGEDIT_NODE_TYPE *)data;
	DRV_CAN_RX_BUFFER_TYPE* q = (DRV_CAN_RX_BUFFER_TYPE *)ctx;
		
	if (q->buffer[0][1] == p->cmd)	/*收到的命令字与该指令注册表节点命令字相同*/
	{
		p->orderfun(ctx);	/*执行该指令注册表节点处理函数*/
		return true;	/*已经执行*/
	}
	
    return false;	/*未执行*/
}
