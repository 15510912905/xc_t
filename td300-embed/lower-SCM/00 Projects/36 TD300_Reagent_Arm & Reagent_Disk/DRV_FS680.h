
#ifndef _DRV_FS680_H_
#define _DRV_FS680_H_


#include "stdint.h"
#include "stdlib.h"
#include "stdbool.h"
#include "BSP_Uart.h"
#include "ucos_ii.h"


typedef enum{
	FS680_1 = 0,
	FS680_BUTT
} DRV_FS680_ENUM;

typedef enum{
	FS680_UNDEFINED = 0,
	FS680_READY = 1
} DRV_FS680_STATUS_ENUM;

typedef struct{
	DRV_FS680_STATUS_ENUM status;
	DRV_FS680_ENUM id;
	BSP_USART_ENUM Uart_port;
} DRV_FS680_TYPE;

typedef struct{
	uint8_t len;
	uint8_t cmd;
	uint8_t status;
	uint8_t checksum_MSB;
	uint8_t checksum_LSB;
} CMD_FS680_TYPE;

typedef struct{
	uint8_t RX_Ptr[100]; 
    uint8_t RX_LEN;
} RX_BUF;
extern DRV_FS680_TYPE* FS680Init(DRV_FS680_ENUM id, uint32_t baud_rate, BSP_USART_ENUM usart_number,
									BSP_PORT_ENUM tx_port_number, BSP_PIN_ENUM tx_pin_number,
									BSP_PORT_ENUM rx_port_number, BSP_PIN_ENUM rx_pin_number
								);
extern RX_BUF FS680_RX;
extern OS_EVENT* _gp_ScanSemIn;
extern bool FS680_Write(DRV_FS680_TYPE* p_unit,CMD_FS680_TYPE* para_unit); /*Ð´¼Ä´æÆ÷*/

extern bool FS680_Receive(DRV_FS680_TYPE* p_unit,RX_BUF* FS680_RX);  /*¶Á¼Ä´æÆ÷*/

extern void FS680_Enable(DRV_FS680_TYPE* p_unit); /*Æô¶¯É¨Ãè */

extern void FS680_DISABLE(DRV_FS680_TYPE* p_unit);

extern void BSP_Usart1_IRQHandler(void);


#endif
