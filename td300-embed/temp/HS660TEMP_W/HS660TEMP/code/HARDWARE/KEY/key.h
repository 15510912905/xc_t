#ifndef __KEY_H
#define __KEY_H	 
//////////////////////////////////////////////////////////////////////////////////	 

//V1.1修改说明
//修改按键扫描函数，使整个代码可以支持SWD下载。
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
	 
void KEY_Init(void);//IO初始化
u8 KEY_Scan(void);  //按键扫描函数					    
#endif
