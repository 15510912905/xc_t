#include "ad7685.h"
#include "delay.h"
//#include "led.h"

void AD7685_GPIO_Config(void)//AD的控制
{
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB ,ENABLE);

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; //NSS-PA15 ->PB6->pa15->PA4
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;//MOSI-PB5 ->NO->pA7
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; //MISO-PB4 -> PB4->PA6
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;//SCLK-PB3-> PB3->PA5
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
    
    //GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);
    //
    //GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; //NSS-PA15 ->PB6->pa15
    //GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;
    //GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    //GPIO_Init(GPIOA,&GPIO_InitStructure);
    //
    //GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;//MOSI-PB5 ->NO
    //GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
    //GPIO_Init(GPIOB,&GPIO_InitStructure);
    //
    //GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; //MISO-PB4 -> PB4
    //GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
    //GPIO_Init(GPIOB,&GPIO_InitStructure);
    //
    //GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;//SCLK-PB3-> PB3
    //GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
    //GPIO_Init(GPIOB,&GPIO_InitStructure);
}

void LIGHT_LED_Config(void)//荧光激发灯的端口初始化
{
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);
    
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; //PC9 ->PC1->pc14-》pc6
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOC,&GPIO_InitStructure);

    GPIO_ResetBits(GPIOC,GPIO_Pin_6);//高有效
}

void AD7685_SPI_Config(void)
{
    SPI_InitTypeDef SPI_InitStructure;
    
//    SPI_I2S_DeInit(SPI1); 
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1,ENABLE);
    
    SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_64;//SPI_BaudRatePrescaler_64;
    SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
    SPI_InitStructure.SPI_CPHA =SPI_CPHA_2Edge;
    SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
    SPI_InitStructure.SPI_DataSize = SPI_DataSize_16b;
//    SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
    SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
    SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
    SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
    SPI_InitStructure.SPI_CRCPolynomial = 7;
    
    SPI_Init(SPI1,&SPI_InitStructure);
    SPI_Cmd(SPI1,ENABLE);
}


static void AD7685_ACQ_time(void)        //posedge采样
{
    CNV_LOW;
    delay_us(1);//OSTimeDlyHMSM(0, 0, 0, 1);//Delay_us(1);
    CNV_HIGH;
    
}

static void AD7685_CNV_Time(void)
{
      delay_us(1);//OSTimeDlyHMSM(0, 0, 0, 1);//Delay_us(5);
} 

static uint16_t AD7685_ReadData_Time(uint16_t tData)
{
//    CNV_LOW;
//    while(SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_TXE) == RESET)
//        ;
//	while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_BSY) == SET);
//    SPI_I2S_SendData(SPI1, tData); 
//    while(SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_RXNE) == RESET)
//        ;
//    return (SPI_I2S_ReceiveData(SPI1));
		
		
		
		u8 retry=0;				 
		CNV_LOW;//while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_BSY) == SET)//
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET) //检查指定的SPI标志位设置与否:发送缓存空标志位
		{
		retry++;
		if(retry>200)
			return 0;
		}			  

	SPI_I2S_SendData(SPI1, tData); //通过外设SPIx发送一个数据
	retry=0;//while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_BSY) == SET)//
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET) //检查指定的SPI标志位设置与否:接受缓存非空标志位
		{
		retry++;
		if(retry>200)
			return 0;
		}	  						    

	return SPI_I2S_ReceiveData(SPI1); //返回通过SPIx最近接收的数据					
    
}

uint16_t AD7685_oneAcq(uint16_t tData)
{
    int16_t Data = 0;
    
    CNV_HIGH;//zl add 
    AD7685_ACQ_time();
    AD7685_CNV_Time();
    Data = AD7685_ReadData_Time(DUMMY) ;
    CNV_HIGH;
    return Data;
}

uint16_t read_color(void)
{
				uint16_t ADData = 0;
        ADData = AD7685_oneAcq(DUMMY);
        return ADData;
}

//AD的数据处理 

