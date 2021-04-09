#ifndef _SHARE_MEMORY_H_
#define _SHARE_MEMORY_H_

#include "includes.h"

#define FULL 	(1)
#define EMPTY 	(0)

#define SHARE_MEMORY_CNT        (5)//CMD_BUFF_COUNT
//#define ONT_SHARE_MEMORY_SIZE   (128)

//typedef enum {
//    EN_Idll = 0,
//    EN_Head = 1,
//    EN_Confirm = 2,
//    EN_Data = 3,
//}EN_canDataIdentify;

//typedef struct
//{
//    uint16_t mlocation;
//    uint16_t isused;
//    uint8_t *pShareMemory;
//}Share_Memory_TypeDef;

//typedef struct
//{
//    uint8_t m_state;
//    uint8_t bufIndex;
//    int16_t m_dataLen;
//    uint16_t m_dataId;
//    uint16_t m_dataCrc;
//    Share_Memory_TypeDef *msgBuffer;
//}st_subUnit;


//INT8U ShareMemInit(void);
//void FreeShareMem(void);
//void SaveData(uint8_t unit, uint8_t bufIndex, uint16_t dataLen, uint8_t* data, uint16_t totalLen);

//void setShareMemoryLocation(uint8_t unit,INT8U num, INT16U location);
//INT16U isShareMemoryUsed(uint8_t unit, INT8U num);
//void setShareMemoryUsed(uint8_t unit, INT8U num);
//void clrShareMemoryUsed(uint8_t unit, INT8U num);
//int8_t isShareMemoryEmpty(uint8_t unit);
//INT8U setShareMemoryValue(uint8_t unit, INT8U num, INT16U location, INT8U value);
//Share_Memory_TypeDef getShareMemory(uint8_t unit, INT8U num);

//extern st_subUnit SubUnit[UnitCount];

#endif
