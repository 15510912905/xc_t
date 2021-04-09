#include "sharememory.h"

//static Share_Memory_TypeDef mShareMemory[UnitCount][SHARE_MEMORY_CNT];
// 
//st_subUnit SubUnit[UnitCount];

//INT8U ShareMemInit()
//{
//    uint8_t cnt = 0;
//    uint8_t unitIndex = 0;
//    for (unitIndex = 0; unitIndex < UnitCount; unitIndex++)
//    {
//        for (cnt = 0; cnt < SHARE_MEMORY_CNT; cnt++)
//        {
//            mShareMemory[unitIndex][cnt].mlocation = 0;
//            mShareMemory[unitIndex][cnt].isused = EMPTY;
//            mShareMemory[unitIndex][cnt].pShareMemory = (uint8_t *)malloc(ONT_SHARE_MEMORY_SIZE);
//            if (mShareMemory[unitIndex][cnt].pShareMemory == NULL){ return 1; }
//            memset(mShareMemory[unitIndex][cnt].pShareMemory, 0, ONT_SHARE_MEMORY_SIZE);
//        }
//        SubUnit[unitIndex].m_state = EN_Idll;
//        SubUnit[unitIndex].bufIndex = 0;
//        SubUnit[unitIndex].msgBuffer = &mShareMemory[unitIndex][0];
//    }   
//    return 0;
//}

//void FreeShareMem()
//{
//    INT8U cnt = 0;
//    uint8_t unitIndex = 0;
//    for (unitIndex = 0; unitIndex < UnitCount; unitIndex++)
//    {
//        for (cnt = 0; cnt < SHARE_MEMORY_CNT; cnt++)
//        {
//            FREE_MEMORY(mShareMemory[unitIndex][cnt].pShareMemory);
//        }
//    }
//}

//void SaveData(uint8_t unit, uint8_t bufIndex, uint16_t dataLen, uint8_t* data, uint16_t totalLen)
//{
//    if ((SHARE_MEMORY_CNT <= bufIndex) || (ONT_SHARE_MEMORY_SIZE < mShareMemory[unit][bufIndex].mlocation + dataLen))
//    {
//        return;
//    }
//    memcpy(mShareMemory[unit][bufIndex].pShareMemory + mShareMemory[unit][bufIndex].mlocation, data, dataLen);
//    mShareMemory[unit][bufIndex].mlocation += dataLen;
//    if (totalLen == 0)
//    {
//        mShareMemory[unit][bufIndex].isused = FULL;
//    }
//}


//void setShareMemoryLocation(uint8_t unit, INT8U num, INT16U location)
//{
//    if (num < SHARE_MEMORY_CNT && location < ONT_SHARE_MEMORY_SIZE && unit < UnitCount)
//    {
//        mShareMemory[unit][num].mlocation = location;
//    }
//}

//INT16U isShareMemoryUsed(uint8_t unit, INT8U num)
//{
//    if (SHARE_MEMORY_CNT <= num || UnitCount <= unit)
//    {
//        return EMPTY;
//    }
//    return mShareMemory[unit][num].isused;
//}

//void setShareMemoryUsed(uint8_t unit, INT8U num)
//{
//    if (SHARE_MEMORY_CNT <= num || UnitCount <= unit)
//    {
//        return;
//    }
//    mShareMemory[unit][num].isused = FULL;
//}

//void clrShareMemoryUsed(uint8_t unit,INT8U num)
//{
//    if (SHARE_MEMORY_CNT <= num || UnitCount <= unit)
//    {
//        return;
//    }
//    mShareMemory[unit][num].isused = EMPTY;
//}

//int8_t isShareMemoryEmpty(uint8_t unit)
//{
//    INT8U cnt = 0;
//    for (cnt = 0; cnt < SHARE_MEMORY_CNT; cnt++)
//    {
//        if (mShareMemory[unit][cnt].isused == EMPTY)
//        {
//            mShareMemory[unit][cnt].mlocation = 0;
//            memset(mShareMemory[unit][cnt].pShareMemory, 0, ONT_SHARE_MEMORY_SIZE);
//            return cnt;
//        }
//    }
//    return -1;
//}

//INT8U setShareMemoryValue(uint8_t unit, INT8U num, INT16U location, INT8U value)
//{
//    if ((SHARE_MEMORY_CNT <= num) || (ONT_SHARE_MEMORY_SIZE <= location))
//    {
//        return 0;
//    }
//    mShareMemory[unit][num].pShareMemory[location] = value;
//    return 1;
//}

//Share_Memory_TypeDef getShareMemory(uint8_t unit, INT8U num)
//{
//    if (SHARE_MEMORY_CNT <= num)
//    {
//        num = SHARE_MEMORY_CNT - 1;
//    }
//    return mShareMemory[unit][num];
//}
