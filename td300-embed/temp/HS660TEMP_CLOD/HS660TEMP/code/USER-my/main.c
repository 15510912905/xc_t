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

//	ShareMemInit();//������Դ��ʼ��   
//	RegistCmd(); //ע������ؼ���

	//IIC_Init();
	//AT24CXX_Init();
	ProductInit(); //��Ʒ�ӿڳ�ʼ��
	delay_init(72);//ʹ��ϵͳ�Դ�����ʱ����
	//��ʼ��eeprom
//	motor_compen_init();
	//	   IIC_Init();

	OSTaskCreate(TaskStart,	//task pointer
		(void *)0,	//parameter
		(OS_STK *)&TASK_START_STK[START_STK_SIZE - 1],	//task stack top pointer
		START_TASK_Prio);	//task priority
	OSStart();
	return 0;

	//				 SystemInit();
	//	       delay_init(72);//ʹ��ϵͳ�Դ�����ʱ����
	//	       ProductInit(); //��Ʒ�ӿڳ�ʼ��
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


















