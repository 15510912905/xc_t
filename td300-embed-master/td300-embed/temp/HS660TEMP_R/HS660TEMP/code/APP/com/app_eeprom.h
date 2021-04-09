#ifndef _APP_EEPROM_H_
#define _APP_EEPROM_H_

#include "includes.h"
//#include "eeprom_config.h"
#include "drv_motor.h"


/* begin of 以下宏带REAL 后缀的需要实际用户定义*/
#define RST_PARA_NUM     RST_PARA_NUM_REAL
#define RTT_PARA_NUM     RTT_PARA_NUM_REAL
#define MOTOR_PARA_NUM	 MOTOR_PARA_NUM_REAL
#define PHYSICS_PLACE	 PHYSICS_PLACE_REAL
#define RST_CMPT		 RST_CMPT_REAL
/* end of 以上宏带REAL 后缀的需要实际用户定义*/

#define EEPROM_PAGEBYTE_SIZE      (64)
/*					eeprom 页						*/
/*0	 1	| 2	 3	 4	 5  6	 7	 8  9	10|	...25|	*/
/*标识区               补偿参数区      	        	*/

//全局标识区: 起始偏移地址为0, 大小为128Bytes
#define EEPROM_GLOBAL_PARA_PAGE   (0)
#define EEPROM_GLOBAL_PARA_ADDR   (0)
#define EEPROM_GLOBAL_PARA_SIZE   (128)

//补偿标识区: 起始偏移地址为0, 大小为256Bytes
#define EEPROM_CMPNST_PARA_PAGE	 (2)
#define EEPROM_CMPNST_PARA_ADDR	 (0)
#define EEPROM_CMPNST_PARA_SIZE	 (512)

/*补偿全局标识类型*/
typedef enum
{
	EEPROM_FLAG_EMPTY  = (INT16U)0,
	EEPROM_FLAG_WRITED = (INT16U)88,

} EEPROM_FLAG_ENUM;

/*eeprom全局标识结构体*/
typedef struct
{
	INT16U eeprom_flag;
	
} eeprom_global_para;

/*补偿参数结构体*/
typedef struct
{
    INT16U dir;
    INT16U step;
} cmpnst_para_st;

/*补偿类型结构体*/
//typedef struct
//{
//    cmpnst_para_st	cmpRST[RST_PARA_NUM];
//    cmpnst_para_st	cmpRTT[RTT_PARA_NUM];  
//	MotorLineSt		cmpMotor[MOTOR_PARA_NUM];
//    
//} eeprom_cmpnst_para_info;

/*声明部分*/
//extern eeprom_global_para       cmpnst_global_value;
//extern eeprom_cmpnst_para_info  CmpnstValue;
///*函数声明部分*/
//INT16U get_cmpnst_val(uint16_t index);
//void set_cmpnst_val(uint16_t index, uint16_t value);
//void set_eeprom_cmpnst(void);
//void eeprom_cmpnst_value_load(void);
//INT32U set_eeprom_value_cmd(void *pData);
//eeprom_cmpnst_para_info* get_eepromcmpnstparaaddr(void);
//MotorLineSt* MotorLine(motor_para_enum motor);
#endif

