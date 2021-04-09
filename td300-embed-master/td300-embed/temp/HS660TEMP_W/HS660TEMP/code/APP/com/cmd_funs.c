#include "cmd_funs.h"
#include "app_downmsg.h"
#include "sharememory.h"
#include "double_list.h"
#include "can.h"

//DList *dlist[UnitCount] = { NULL };
//st_CurCmd g_CurCmd[UnitCount] = { 0 };

//void CmdList_init()
//{
//    uint8_t loop = 0;
//    for (; loop < UnitCount; loop++)
//    {
//        dlist[loop] = dlist_create();
//    }
//}

//cmd_str *malloc_cmd(INT16U cmd, Func orderfun)
//{
//    cmd_str *order_str_p = (cmd_str *)calloc(1, sizeof(cmd_str));

//    if (NULL == order_str_p)
//    {
//        //WriteBBLog("malloc_cmd fail");
//        return NULL;
//    }

//    order_str_p->orderfun = orderfun;
//    order_str_p->cmd = cmd;

//    return order_str_p;
//}

//INT32U add_cmd_node(INT16U cmd, Func orderfun, uint8_t unit)
//{
//    cmd_str *cmd_str_q = malloc_cmd(cmd, orderfun);

//    if (NULL == cmd_str_q)
//    {
//        //WriteBBLog("add_cmd_nod fail.");
//        return 1;
//    }
//    dlist_add(dlist[unit], 0, cmd_str_q);
//    return 0;
//}

//void *cmd_dofun(void *ctx, void *data)
//{
//	cmd_str *p = data;
//    cmd_stComType *q = (cmd_stComType *)ctx;
//	CMDINT16U cmd =(q->cmd);
//	
//	if (cmd == p->cmd)
//	{
//		p->orderfun(ctx);//增加传入数据参数
//	}	
//    return NULL;
//}

//void DoCmd(uint8_t unit)
//{
//    INT8U buff_count = 0;
//    INT16U cmd_count = 0;

//    cmd_stComType *pCmd = NULL;
//    cmd_stSingleInfo *pSingCmd = NULL;
//    cmd_stComInfo *pCmdInfo = NULL;
//    st_Can2Downmach *msgtran_strup = NULL;

//    for (buff_count = 0; buff_count < SHARE_MEMORY_CNT; buff_count++)
//    {
//        if (EMPTY == getShareMemory(unit, buff_count).isused)
//        {
//            continue;
//        }
//        msgtran_strup = (st_Can2Downmach *)(getShareMemory(unit, buff_count).pShareMemory);

//        pCmdInfo = (cmd_stComInfo *)msgtran_strup->aucdata;
//        cmd_count = pCmdInfo->cmdCount;
//        pSingCmd = pCmdInfo->singleCmd;
//        g_CurCmd[unit].name = msgtran_strup->enDwnSubSys;
//        g_CurCmd[unit].currentframeid = msgtran_strup->ulFrameid;
//        while (cmd_count-- > 0)
//        {
//            pCmd = &(pSingCmd->cmdInfo);
//            g_CurCmd[unit].cmdType = pCmd->cmd;
//            dlist_foreach(dlist[unit], cmd_dofun, pCmd);
//            pSingCmd = (cmd_stSingleInfo *)((char*)pSingCmd + pSingCmd->cmdSize + sizeof(pSingCmd->cmdSize));
//        }
//        clrShareMemoryUsed(unit, buff_count);
//    }
//}

//void CmdReturn(uint8_t rslt, uint16_t dataLen, uint8_t *data, uint8_t id)
//{
//    msg_stDownmach2Can *pData;
//    st_CanFormat *pFormat;
//    uint32_t formatLen = sizeof(st_CanFormat) + sizeof(msg_stDownmach2Can) + dataLen;
//    pFormat = (st_CanFormat*)malloc(formatLen);

//    pFormat->ushead = 0x550f;
//    pFormat->usdatalen = sizeof(msg_stDownmach2Can) + dataLen;
//    pFormat->uscpuid = g_CurCmd[id].name;
//    pFormat->usCrc = 0;

//    pData = (msg_stDownmach2Can *)pFormat->aucdata;
//    pData->enDwnSubSys = g_CurCmd[id].name;
//    pData->enMsgType = EN_DType_res;
//    pData->ulFrameid = g_CurCmd[id].currentframeid;
//    pData->usCmdType = g_CurCmd[id].cmdType;
//    pData->enExeRslt = rslt;
//    pData->usCmdInfoLen = dataLen;
//    if (data != NULL && dataLen > 0)
//    {
//        memcpy(pData->aucinfo, data, dataLen);
//    }
////    CanSend((uint8_t*)pFormat, formatLen, id);
//    FREE_MEMORY(pFormat);
//}

//void ActComRes(void *cmd, uint8_t err, uint8_t unitId)
//{
//    uint8_t rslt = (OS_ERR_NONE == err) ? CR_Sucess : CR_Failed;
//    CmdReturn(rslt, 0, NULL, unitId);
//}
