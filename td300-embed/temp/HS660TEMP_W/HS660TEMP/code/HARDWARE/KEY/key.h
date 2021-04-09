#ifndef __KEY_H
#define __KEY_H	 
//////////////////////////////////////////////////////////////////////////////////	 

//V1.1�޸�˵��
//�޸İ���ɨ�躯����ʹ�����������֧��SWD���ء�
//////////////////////////////////////////////////////////////////////////////////	 
typedef enum 
{  
	Button_KEY1 = 1,
	Button_KEY2 = 2,
	Button_KEY3 = 3,
	Button_KEY4 = 4,
} Button_TypeDef;

typedef struct  
{   GPIO_TypeDef*       BUTTON_PORT ;
    uint16_t            BUTTON_PIN ;
    GPIOMode_TypeDef    BUTTON_MODE;
}Button_PinModeDef;	

#define KEY1 PDin(15)   //PD15
#define KEY2 PDin(14)	//PD14 
#define KEY3 PDin(13)	//PD13  
#define KEY4 PDin(12)	//PD12
	 
void KEY_Init(void);//IO��ʼ��
u8 KEY_Scan(void);  //����ɨ�躯��					    
#endif
