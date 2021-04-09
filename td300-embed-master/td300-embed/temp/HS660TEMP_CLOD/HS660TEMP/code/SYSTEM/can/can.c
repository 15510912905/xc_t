#include "can.h"
#include "app_downmsg.h"
#include "stm32f10x_can.h"
#include "sharememory.h"
#include "delay.h"

#define FORMAT_LEN (256)
//OS_EVENT *g_CanMutex = NULL;
//extern OS_EVENT *CMD_Sem[UnitCount];

//uint16_t Byte2Short_H_L(uint8_t Hbyte, uint8_t Lbtye)
//{
//	return (uint16_t)(Hbyte << 8 + Lbtye);
//}

//uint16_t CanCalcCrc(void *pdata, uint16_t len)
//{
//	uint8_t *pData = pdata;
//	uint16_t CrcVal = 0;

//	while (len--)
//	{
//		CrcVal += *pData;
//	}
//	return CrcVal;
//}

//void _notifyCmdTask(uint32_t id)
//{
//    OSSemPost(CMD_Sem[id]);//通知命令任务   
//}

//void _initialize(uint32_t id)
//{
//    SubUnit[id].m_state = EN_Idll;
//    SubUnit[id].m_dataId = 0;
//    SubUnit[id].m_dataLen = 0;
//    SubUnit[id].m_dataCrc = 0;
//    SubUnit[id].bufIndex = 0;
//    SubUnit[id].msgBuffer = NULL;
//}


//void _headConfirm(CanRxMsg *pFrame, uint32_t id)
//{
//    st_CanFormat*pHead = (st_CanFormat*)pFrame->Data;

//    if (pHead->ushead == 0x550f && pFrame->DLC == 8)
//    {
//        SubUnit[id].m_dataLen = (int16_t)pHead->usdatalen;
//        SubUnit[id].m_dataId = pHead->uscpuid;
//        SubUnit[id].m_dataCrc = pHead->usCrc;
//        if (SubUnit[id].m_dataLen > FORMAT_LEN)
//        {
//            return;
//        }
//        SubUnit[id].m_state = EN_Data;
//        SubUnit[id].bufIndex = isShareMemoryEmpty(id);
//    }
//}

//void _saveData(CanRxMsg *pFrame, uint32_t id)
//{
//    uint8_t bufIndex = SubUnit[id].bufIndex;
//    SubUnit[id].m_dataLen -= pFrame->DLC;

//    if (SubUnit[id].m_dataLen >= 0)
//    {
//        SaveData(id, bufIndex, pFrame->DLC, pFrame->Data, SubUnit[id].m_dataLen);
//    }
//    if (SubUnit[id].m_dataLen == 0)
//    {
//        _initialize(id);
//        _notifyCmdTask(id);
//    }
//}

//void _analysisData(CanRxMsg* pFrame)
//{
//    uint8_t id = UNITID(pFrame->StdId);
//    //if (id == Unit_RctD || id == Unit_RegD)
//    //{
//    //    return;
//    //}
//    if (SubUnit[id].m_state == EN_Idll)
//    {
//        _headConfirm(pFrame, id);
//    }
//    else if (SubUnit[id].m_state == EN_Data)
//    {
//        _saveData(pFrame, id);
//    }
//}

//void USB_LP_CAN1_RX0_IRQHandler(void)
//{
//    CanRxMsg CanRxMsgBuffer;
//	if (CAN_MessagePending(CAN1, CAN_FIFO0) != 0)
//	{
//		CAN_Receive(CAN1, CAN_FIFO0, &CanRxMsgBuffer);

//        //_canSend(CanRxMsgBuffer.Data, CanRxMsgBuffer.DLC, CanRxMsgBuffer.StdId);
//        //CanSend(CanRxMsgBuffer.Data, CanRxMsgBuffer.DLC, UNITID(CanRxMsgBuffer.StdId));
//        _analysisData(&CanRxMsgBuffer);
//		//OSMboxPost(g_CAN_Mbox, &CanRxMsgBuffer);
//	}
//}

////can 发送
//uint8_t CanSend(uint8_t* pData, uint32_t len, uint32_t unitId)
//{
//    uint8_t mbox;
//    CanTxMsg TxMessage = { 0 };
//    uint8_t* pTemp = pData;
//    uint8_t err = 0;
//    uint8_t state = 0;
//    TxMessage.StdId = CANID(unitId);
//    TxMessage.IDE = CAN_Id_Standard;
//    TxMessage.RTR = CAN_RTR_Data;
//    TxMessage.DLC = len;

//    //MUTEX_PEND_WITH_RET("_canSend", g_CanMutex, err);
//    OSMutexPend(g_CanMutex, 5000, &err);
//    while (len > 0)
//    {
//        TxMessage.DLC = (len >= 8) ? 8 : len;
//        memcpy(TxMessage.Data, pTemp, TxMessage.DLC);
//        delay_us(100);
//        mbox = CAN_Transmit(CAN1, &TxMessage);

//        state = CAN_TransmitStatus(CAN1, mbox);
//        while (state != CAN_TxStatus_Ok)
//        {      
//            if (state == CAN_TxStatus_Failed)
//            {
//                mbox = CAN_Transmit(CAN1, &TxMessage);
//            }
//            state = CAN_TransmitStatus(CAN1, mbox);
//        }
//        pTemp = pTemp + TxMessage.DLC;
//        len = (len >= 8) ? (len - 8) : 0;
//    }
//    OSMutexPost(g_CanMutex);
//    return 0;
//}
