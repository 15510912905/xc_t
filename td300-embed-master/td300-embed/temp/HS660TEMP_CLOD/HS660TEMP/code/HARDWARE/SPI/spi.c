#include "spi.h"	
#include  "delay.h"
#define RES_CS_1 GPIO_SetBits(GPIOB,GPIO_Pin_12) //PB12
#define RES_CS_0 GPIO_ResetBits(GPIOB,GPIO_Pin_12)
#define RES_CLK_1 GPIO_SetBits(GPIOB,GPIO_Pin_13)//pb13
#define RES_CLK_0 GPIO_ResetBits(GPIOB,GPIO_Pin_13)
#define RES_OUT_1 GPIO_SetBits(GPIOB,GPIO_Pin_15)//	pb15
#define RES_OUT_0 GPIO_ResetBits(GPIOB,GPIO_Pin_15)

 
//以下是SPI模块的初始化代码，配置成主机模式，访问SD Card/W25X16/24L01/JF24C							  
//SPI口初始化
//这里针是对SPI1的初始化

SPI_InitTypeDef  SPI_InitStructure;

void  MCP410_SendData(unsigned char x)
{
	unsigned char i,command=0x11;//command=0x11;
	RES_CS_0; //????,??SPI
	 delay_us(1);
	for(i=0;i<8;i++)//??1???8???&1?8????
	{ 
		RES_CLK_0;
		delay_us(1);
		if(command&0x80)//?dat?????????MOSI??
		{ 
			RES_OUT_1; 
		} 
		else
		{ 
			RES_OUT_0; 
		} 			
		RES_CLK_1;
		delay_us(1);
		command<<=1; //dat????1?
	}
	for(i=0;i<8;i++)//??1???8???&1?8????
	{ 
		RES_CLK_0;
		if(x&0x80)//?dat?????????MOSI??
		{ 
			RES_OUT_1; 
		} 
		else
		{ 
			RES_OUT_0;
		} 			
		RES_CLK_1;
		delay_us(1);
		x<<=1; //dat????1?
	}
	delay_us(1); 
	RES_CS_1;
}

void SPIx_Init(void)
{

	GPIO_InitTypeDef GPIO_InitStructure;
  
	RCC_APB2PeriphClockCmd(	RCC_APB2Periph_GPIOB|RCC_APB1Periph_SPI2, ENABLE );	
   
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 |GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;//GPIO_Mode_AF_PP;  //复用推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

 	GPIO_SetBits(GPIOB,GPIO_Pin_13|GPIO_Pin_15);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  //复用推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	

	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;  //设置SPI单向或者双向的数据模式:SPI设置为双线双向全双工
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;		//设置SPI工作模式:设置为主SPI
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;		//设置SPI的数据大小:SPI发送接收8位帧结构
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;		//选择了串行时钟的稳态:时钟悬空高
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;	//数据捕获于第二个时钟沿
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;		//NSS信号由硬件（NSS管脚）还是软件（使用SSI位）管理:内部NSS信号有SSI位控制
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_256;		//定义波特率预分频的值:波特率预分频值为256
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;	//指定数据传输从MSB位还是LSB位开始:数据传输从MSB位开始
	SPI_InitStructure.SPI_CRCPolynomial = 7;	//CRC值计算的多项式
	SPI_Init(SPI2, &SPI_InitStructure);  //根据SPI_InitStruct中指定的参数初始化外设SPIx寄存器
 
	SPI_Cmd(SPI2, ENABLE); //使能SPI外设
	
	SPIx_ReadWriteByte(0xff);//启动传输		 
}   
//SPI 速度设置函数
//SpeedSet:
//SPI_BaudRatePrescaler_2   2分频   (SPI 36M@sys 72M)
//SPI_BaudRatePrescaler_8   8分频   (SPI 9M@sys 72M)
//SPI_BaudRatePrescaler_16  16分频  (SPI 4.5M@sys 72M)
//SPI_BaudRatePrescaler_256 256分频 (SPI 281.25K@sys 72M)

  
void SPIx_SetSpeed(u8 SpeedSet)
{
	SPI_InitStructure.SPI_BaudRatePrescaler = SpeedSet ;
  SPI_Init(SPI2, &SPI_InitStructure);
	SPI_Cmd(SPI2,ENABLE);
} 

//SPIx 读写一个字节
//TxData:要写入的字节
//返回值:读取到的字节
u16 SPIx_ReadWriteByte(u16 TxData)
{		
	u8 retry=0;				 

	while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET) //检查指定的SPI标志位设置与否:发送缓存空标志位
		{
		retry++;
		if(retry>200)return 0;
		}			  

	SPI_I2S_SendData(SPI2, TxData); //通过外设SPIx发送一个数据
	retry=0;

	while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET) //检查指定的SPI标志位设置与否:接受缓存非空标志位
		{
		retry++;
		if(retry>200)return 0;
		}	  						    

	return SPI_I2S_ReceiveData(SPI2); //返回通过SPIx最近接收的数据					    
}
//设置增溢的放大倍数
u16 setAMP(u16 amp_num)
{
//	MCP410_SendData(amp_num);
	u8 flag;
	SPI_CS =0;
	SPIx_ReadWriteByte(0X11);
	flag=SPIx_ReadWriteByte(amp_num);
	SPI_CS =1;
	
	return 0;
}