#ifndef __CMD_FUNS_H__
#define __CMD_FUNS_H__

#include "double_list.h"
#include "includes.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */



typedef INT32U(*Func)(void* data);

typedef struct cmd_str {
	INT16U cmd;
	Func orderfun;
}cmd_str;

typedef struct
{
    INT8U name;
    INT16U cmdType;
    INT32U currentframeid;
}st_CurCmd;

extern INT32U add_cmd_node(INT16U cmd, Func orderfun, uint8_t unit);
extern void CmdList_init(void);

extern void DoCmd(uint8_t unit);
extern void CmdReturn(uint8_t rslt, uint16_t dataLen, uint8_t *data, uint8_t id);
extern void ActComRes(void *cmd, uint8_t err, uint8_t unitId);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */


#endif /* __CMD_FUNS_H__ */
