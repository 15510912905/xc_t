#include "eeprom_config.h"
#include "app_eeprom.h"

#ifndef SAMPLENDL
#define SAMPLENDL           	//����������
#endif
//fre_t *VRTlowFre = NULL;//��ֱ��������ṹ��
//fre_t *RTTlowFre = NULL;//ˮƽ������ٲ����ṹ��
//fre_t *RTThigFre = NULL;	//ˮƽ������ٲ����ṹ��
//fre_t *INJlowFre = NULL;//ע��õ�����ٲ����ṹ��
//fre_t *INJhigFre = NULL;	//ע��õ�����ٲ����ṹ��

//fre_t *STRlowFre = NULL;	//����õ�����ٲ����ṹ��
//fre_t *STRhigFre = NULL;	//����õ�����ٲ����ṹ��


//int IsStopTest =0;//�Ƿ�ֹͣ����

//place_t * const Journey = &CmpnstValue.cmpRTT[0].step;//��ˮƽ����λ��

//const uint16_t CmpnstDir [RTT_PARA_NUM_REAL] =
//{
//     2, 2, 2, 2, 2, 2, 2, 2, 2,//����������ʱ��
//     2, 1, 1, 2, 2,
//     2, 2, 2, 1, 2, 1, 1, 1
//};
//const float angle[5] = { 20.0, 51.34};
//const uint16_t CmpnstPara[RTT_PARA_NUM_REAL] =
//{
//    SUBDIVIDE*(20.0) / 1.8, SUBDIVIDE*(51.34) / 1.8
//};

//const MotorLineSt MotorPara[MOTOR_PARA_NUM_REAL]=
//{
//		{ 800,  6400, 4, 30, 1000 },//{ 600,  6400, 4, 30, 6500 },//vrt_moter_para  //{ 600,  6400, 4, 30, 1000 },//vrt_moter_para  // 800 //ԭ���ڶ���������600,  6400, 4, 30, 65000{ 1200,  1600, 4, 30, 65000 }
//    { 1200, 1600, 4, 30, 6500 },//rrt_l_moter_para//ԭ����һ������Ϊ150 ,3100,4,50,1700;
//    { 150, 3100, 8, 65, 2000 },//rrt_h_moter_para
////  { 800,  5000, 16, 25, 1000 },//rrt_l_moter_para
//// 	{ 800, 16000, 8, 50,  1800 },//rrt_h_moter_para
//	{ 800, 5000, 4, 16, 65000 },//{ 800, 5000, 8, 4, 65000 },//inj_l_moter_para
//	{ 800, 5000, 8, 16, 65000 }, //inj_h_moter_para
//	{ 800, 6400, 8, 60, 65000 },  //stir_l_motor_para
//	{ 800, 18000, 8, 60, 65000 }, //stir_l_motor_para

//};

//ˮƽ����Ĳ��� 
//h :3520,0,0,394,437,822,822,1720,0,3520,972
//const uint32_t H_step_num [H_POS_NUM] ={3520,0,0,394,437,516,823,1720,0,3520,972};//{3520,0,0,372,490,566,873,1720,0,3520,972};//{3520,0,0,372,490,566,873,1459,0,3520,1000};
//const uint32_t V_step_num [V_POS_NUM] ={2200,700,870,870,220,2100,0,0,0,0,0};//{2200,700,870,870,220,1840,0,0,0,0,0};//1950//{1950,700,690,870,220,1840,0,0,0,0,0};//{1950,700,690,930,220,1840,0,0,0,0,0};//{1870,700,690,930,220,1840,0,0,0,0,0};//1850 ,1050,...
//const uint32_t INJ_step_num [INJ_POS_NUM] ={5405,13514,13514,14414,14414,17117};//{6486,10810,13514,17296,10810,16215};//{6486,10810,16215,17296,10810,16215};//{5000,8000,8000,8000,8000,8000};

clums_para all_clum_paras[8];//��Ŀ����
motor_compensation motor_compen;//�������
void init_motor_compen(){
//��ʼ������;
}

////���ص���Ӽ��ٲ������Ӽ�����������
//void init_motor_speed_line(void)
//{
//	callocMotorFre(MotorLine(vrtMotorLow)->up_max_lader, &VRTlowFre);
//	callocMotorFre(MotorLine(rrtMotorLow)->up_max_lader, &RTTlowFre);
//	callocMotorFre(MotorLine(rrtMotorHig)->up_max_lader, &RTThigFre);
//	callocMotorFre(MotorLine(injMotorLow)->up_max_lader, &INJlowFre);
//	callocMotorFre(MotorLine(injMotorHig)->up_max_lader, &INJhigFre);
//	//add by zhaoL start
//	callocMotorFre(MotorLine(stirMotorLow)->up_max_lader, &STRlowFre);
//	callocMotorFre(MotorLine(stirMotorHig)->up_max_lader, &STRhigFre);
//	//add by zhaoL end 
//	
//	
//	calcMotorLine(MotorLine(vrtMotorLow), VRTlowFre);
//	calcMotorLine(MotorLine(rrtMotorLow), RTTlowFre);
//	calcMotorLine(MotorLine(rrtMotorHig), RTThigFre);
//	calcMotorLine(MotorLine(injMotorLow), INJlowFre);
//	calcMotorLine(MotorLine(injMotorHig), INJhigFre);
//	
//	//add by zhaoL start
//	calcMotorLine(MotorLine(stirMotorLow), STRlowFre);
//	calcMotorLine(MotorLine(stirMotorHig), STRhigFre);
//	//add by zhaoL end
//}

//void ReportMotorLineType(xc8001_dwnmachine_motorline *pReport)
//{
//    pReport->ppara[0].drvID = cmd_motortype_vrt;
//    pReport->ppara[1].drvID = cmd_motortype_rtt;
//    pReport->ppara[2].drvID = cmd_motortype_rtt;
//    pReport->ppara[3].drvID = cmd_motortype_inj;
//    pReport->ppara[4].drvID = cmd_motortype_inj;
//    
//    pReport->ppara[0].lineID = mLine_low;
//    pReport->ppara[1].lineID = mLine_low;
//    pReport->ppara[2].lineID = mLine_high;
//    pReport->ppara[3].lineID = mLine_low;
//    pReport->ppara[4].lineID = mLine_high;
//}

