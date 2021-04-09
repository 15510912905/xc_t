#include "spi.h"	
#include  "delay.h"
#define RES_CS_1 GPIO_SetBits(GPIOB,GPIO_Pin_12) //PB12
#define RES_CS_0 GPIO_ResetBits(GPIOB,GPIO_Pin_12)
#define RES_CLK_1 GPIO_SetBits(GPIOB,GPIO_Pin_13)//pb13
#define RES_CLK_0 GPIO_ResetBits(GPIOB,GPIO_Pin_13)
#define RES_OUT_1 GPIO_SetBits(GPIOB,GPIO_Pin_15)//	pb15
#define RES_OUT_0 GPIO_ResetBits(GPIOB,GPIO_Pin_15)

 
//������SPIģ��ĳ�ʼ�����룬���ó�����ģʽ������SD Card/W25X16/24L01/JF24C							  
//SPI�ڳ�ʼ��
//�������Ƕ�SPI1�ĳ�ʼ��

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
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;//GPIO_Mode_AF_PP;  //�����������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

 	GPIO_SetBits(GPIOB,GPIO_Pin_13|GPIO_Pin_15);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  //�����������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	

	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;  //����SPI�������˫�������ģʽ:SPI����Ϊ˫��˫��ȫ˫��
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;		//����SPI����ģʽ:����Ϊ��SPI
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;		//����SPI�����ݴ�С:SPI���ͽ���8λ֡�ṹ
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;		//ѡ���˴���ʱ�ӵ���̬:ʱ�����ո�
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;	//���ݲ����ڵڶ���ʱ����
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;		//NSS�ź���Ӳ����NSS�ܽţ����������ʹ��SSIλ������:�ڲ�NSS�ź���SSIλ����
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_256;		//���岨����Ԥ��Ƶ��ֵ:������Ԥ��ƵֵΪ256
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;	//ָ�����ݴ����MSBλ����LSBλ��ʼ:���ݴ����MSBλ��ʼ
	SPI_InitStructure.SPI_CRCPolynomial = 7;	//CRCֵ����Ķ���ʽ
	SPI_Init(SPI2, &SPI_InitStructure);  //����SPI_InitStruct��ָ���Ĳ�����ʼ������SPIx�Ĵ���
 
	SPI_Cmd(SPI2, ENABLE); //ʹ��SPI����
	
	SPIx_ReadWriteByte(0xff);//��������		 
}   
//SPI �ٶ����ú���
//SpeedSet:
//SPI_BaudRatePrescaler_2   2��Ƶ   (SPI 36M@sys 72M)
//SPI_BaudRatePrescaler_8   8��Ƶ   (SPI 9M@sys 72M)
//SPI_BaudRatePrescaler_16  16��Ƶ  (SPI 4.5M@sys 72M)
//SPI_BaudRatePrescaler_256 256��Ƶ (SPI 281.25K@sys 72M)

  
void SPIx_SetSpeed(u8 SpeedSet)
{
	SPI_InitStructure.SPI_BaudRatePrescaler = SpeedSet ;
  SPI_Init(SPI2, &SPI_InitStructure);
	SPI_Cmd(SPI2,ENABLE);
} 

//SPIx ��дһ���ֽ�
//TxData:Ҫд����ֽ�
//����ֵ:��ȡ�����ֽ�
u16 SPIx_ReadWriteByte(u16 TxData)
{		
	u8 retry=0;				 

	while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET) //���ָ����SPI��־λ�������:���ͻ���ձ�־λ
		{
		retry++;
		if(retry>200)return 0;
		}			  

	SPI_I2S_SendData(SPI2, TxData); //ͨ������SPIx����һ������
	retry=0;

	while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET) //���ָ����SPI��־λ�������:���ܻ���ǿձ�־λ
		{
		retry++;
		if(retry>200)return 0;
		}	  						    

	return SPI_I2S_ReceiveData(SPI2); //����ͨ��SPIx������յ�����					    
}
//��������ķŴ���
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