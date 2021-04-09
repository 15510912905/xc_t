#include "sys.h"
#include "delay.h"	
#include "includes.h"	 
//#include "cmd.h"
#include "task.h"
//#include "product.h"
//#include "sharememory.h"
//#include "stmflash.h"
//#include "cardCheck.h"

int main(void)
{
	OSInit();

//	ShareMemInit();//共享资源初始化   
//	RegistCmd(); //注册命令关键字

	//IIC_Init();
	//AT24CXX_Init();
	ProductInit(); //产品接口初始化
	delay_init(72);//使用系统自带的延时机制
	//初始化eeprom
//	motor_compen_init();
	//	   IIC_Init();

	OSTaskCreate(TaskStart,	//task pointer
		(void *)0,	//parameter
		(OS_STK *)&TASK_START_STK[START_STK_SIZE - 1],	//task stack top pointer
		START_TASK_Prio);	//task priority
	OSStart();
	return 0;

	//				 SystemInit();
	//	       delay_init(72);//使用系统自带的延时机制
	//	       ProductInit(); //产品接口初始化
	//	       
	//	  while(1){
	//			//Ledbleek();
	//			vertical_motor_move();
	//			horizontal_motor_move();
	//			inject_motor_move();
	//			
	//			
	//		}

}


















