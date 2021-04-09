
#ifndef _DRV_EM1399_H_
#define _DRV_EM1399_H_


#include "stdint.h"
#include "stdlib.h"
#include "stdbool.h"
#include "BSP_Uart.h"
#include "ucos_ii.h"


typedef enum{
	EM1399_1 = 0,
	EM1399_BUTT
} DRV_EM1399_ENUM;

typedef enum{
	EM1399_UNDEFINED = 0,
	EM1399_READY = 1
} DRV_EM1399_STATUS_ENUM;

typedef struct{
	DRV_EM1399_STATUS_ENUM status;
	uint8_t read_buffer;
	uint8_t write_buffer;
	DRV_EM1399_ENUM id;
	BSP_USART_ENUM Uart_port;
} DRV_EM1399_TYPE;

typedef struct{
	uint8_t prefix_MSB;
	uint8_t prefix_LSB;
	uint8_t type;
	uint8_t lens;
	uint8_t addr_MSB;
	uint8_t addr_LSB;
	uint8_t data;     /*cmd*/
	uint8_t FCS_MSB;
	uint8_t FCS_LSB;
} CMD_EM1399_TYPE;
typedef struct{
	uint8_t RX_Ptr[100]; 
    uint8_t RX_LEN;
} RX_BUF;
extern DRV_EM1399_TYPE* EM1399Init(DRV_EM1399_ENUM id, uint32_t baud_rate, BSP_USART_ENUM usart_number,
									BSP_PORT_ENUM tx_port_number, BSP_PIN_ENUM tx_pin_number,
									BSP_PORT_ENUM rx_port_number, BSP_PIN_ENUM rx_pin_number
);
extern RX_BUF EM1399_RX;
extern OS_EVENT* _gp_ScanSemIn;
extern bool EM1399_Write(DRV_EM1399_TYPE* p_unit,CMD_EM1399_TYPE* para_unit); /*Ð´¼Ä´æÆ÷*/

extern bool EM1399_Receive(DRV_EM1399_TYPE* p_unit,RX_BUF* EM1399_RX);  /*¶Á¼Ä´æÆ÷*/

extern void EM1399_Enable(DRV_EM1399_TYPE* p_unit); /*Æô¶¯É¨Ãè */

extern void EM1399_DISABLE(DRV_EM1399_TYPE* p_unit);

extern void BSP_Usart2_IRQHandler(void);

extern void BSP_Usart1_IRQHandler(void);

extern uint16_t crc_cal_by_bit(uint8_t* ptr, uint8_t len); /*¼ÆËãCRC*/

#endif
