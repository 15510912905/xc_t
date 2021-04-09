#include "app_eeprom.h"
#include "cmd_downmachine.h"
#include "app_com.h"


//eeprom_global_para      cmpnst_global_value;
//eeprom_cmpnst_para_info CmpnstValue;

//eeprom_cmpnst_para_info* get_eepromcmpnstparaaddr(void)
//{
//    return &CmpnstValue;
//}

//void get_eeprom_globel(void)
//{
    //EEPROM_Read(EEPROM_GLOBAL_PARA_ADDR, EEPROM_GLOBAL_PARA_PAGE, &cmpnst_global_value, MODE_8_BIT, sizeof(cmpnst_global_value));
//    return;
//}
//void set_eeprom_globel(void)
//{
//    //EEPROM_Write(EEPROM_GLOBAL_PARA_ADDR, EEPROM_GLOBAL_PARA_PAGE, &cmpnst_global_value, MODE_8_BIT, sizeof(cmpnst_global_value));
//    return;
//}
//void get_eeprom_cmpnst(void)
//{
//    //EEPROM_Read(EEPROM_CMPNST_PARA_ADDR, EEPROM_CMPNST_PARA_PAGE, &CmpnstValue, MODE_8_BIT, sizeof(CmpnstValue));
//    return;
//}
//void set_eeprom_cmpnst(void)
//{
//    //EEPROM_Write(EEPROM_CMPNST_PARA_ADDR, EEPROM_CMPNST_PARA_PAGE, &CmpnstValue, MODE_8_BIT, sizeof(CmpnstValue));
//    return;
//}
// 获取补偿值
//INT16U get_cmpnst_val(uint16_t index)
//{
//	INT16U value = 0;
//	if (index < (RTT_PARA_NUM - PHYSICS_PLACE))
//	{
//		value = CmpnstValue.cmpRTT[index + PHYSICS_PLACE].step;
//	}
//	return value;
//}
////设置补偿值
//void set_cmpnst_val(uint16_t index, uint16_t value)
//{
//	if (index < (RTT_PARA_NUM - PHYSICS_PLACE))
//	{
//		CmpnstValue.cmpRTT[index + PHYSICS_PLACE].step = value;
//        set_eeprom_cmpnst();
//	}
//	return;
//}
///*第一次初始化补偿参数*/
//void eeprom_recover_default_value(void)
//{
//    INT16U count;

//    memset((void*)&CmpnstValue, 0, sizeof(CmpnstValue));
//    for (count = 0; count < RST_PARA_NUM_REAL; count++)
//    {
//        CmpnstValue.cmpRST[count].dir = compensation_dir_anticlockwise;//盘都是逆时针复位
//        CmpnstValue.cmpRST[count].step = RST_CMPT;
//    }
//    for (count = 0; count < RTT_PARA_NUM_REAL; count++)
//    {
//        CmpnstValue.cmpRTT[count].dir = CmpnstDir[count];
//        CmpnstValue.cmpRTT[count].step = CmpnstPara[count];
//    }
//    for (count = 0; count < MOTOR_PARA_NUM_REAL; count++)
//    {
//        memcpy(CmpnstValue.cmpMotor + count, MotorPara + count, sizeof(MotorLineSt));
//    }
//    set_eeprom_cmpnst();
//}
//加载eeprom 补偿参数
//void eeprom_cmpnst_value_load(void)
//{
//// 	get_eeprom_globel();
//// 	if (EEPROM_FLAG_WRITED == cmpnst_global_value.eeprom_flag)
//// 	{
//// 		get_eeprom_cmpnst();
//// 	}
//// 	else
//	{
//		//cmpnst_global_value.eeprom_flag = EEPROM_FLAG_WRITED;
//        //set_eeprom_globel();
//        eeprom_recover_default_value();
//	}
//}

// INT32U set_eeprom_value_cmd(void *pData)
// {
//     cmd_dwnmachine_compensation *cmddata = (cmd_dwnmachine_compensation *)pData;
// 	INT32U count = 0;
// 
// 	if (NULL == cmddata)
// 	{
// 		cmd_returnmesg(cmddata->cmd, 1, 0, NULL);
// 		return (1);
// 	}
//     switch(cmddata->type)
//     {
// 		case compensation_type_reset:
// 		{
// 			cmd_compensation_para	*p_para = (cmd_compensation_para *)(&(cmddata->ppara));
// 
// 			if (RST_PARA_NUM < cmddata->paranum || NULL == p_para)
// 			{
// 				cmd_returnmesg(cmddata->cmd, 1, 0, NULL);
// 				return (1);
// 			}
// 			else
// 			{
// 				for (count = 0; count < cmddata->paranum; count++)
// 				{
// 				    if (p_para[count].location >= RST_PARA_NUM)
// 				    {
// 				        cmd_returnmesg(cmddata->cmd, 1, 0, NULL);
//                         return (1);
// 				    }
//                     CmpnstValue.cmpRST[p_para[count].location].dir  = p_para[count].dir;
//                     CmpnstValue.cmpRST[p_para[count].location].step = p_para[count].step;
// 				}
// 				set_eeprom_cmpnst();
// 			}
// 			break;
// 		}
// 		case compensation_type_rtt:
// 		{
// 			cmd_compensation_para *p_para = (cmd_compensation_para *)(&(cmddata->ppara));
// 
// 			if (RTT_PARA_NUM < cmddata->paranum || NULL == p_para)
// 			{
// 				cmd_returnmesg(cmddata->cmd, 1, 0, NULL);
// 				return (1);
// 			}
// 			else
// 			{
// 				for (count = 0; count < cmddata->paranum; count++)
// 				{
// 				    if (p_para[count].location >= RTT_PARA_NUM)
// 				    {
// 				        cmd_returnmesg(cmddata->cmd, 1, 0, NULL);
//                         return (1);
// 				    }
// 					CmpnstValue.cmpRTT[p_para[count].location].dir  = p_para[count].dir;
//                     CmpnstValue.cmpRTT[p_para[count].location].step = p_para[count].step;
// 				}
//                 set_eeprom_cmpnst();
// 			}
// 			break;
// 		}
// 		case compensation_type_default:
// 		{
// 			//eeprom_recover_default_value();
// 			break;
// 		}
// 		default:
// 		{
// 			break;
// 		}
// 	}
// 	cmd_returnmesg(cmddata->cmd, 0, 0, NULL);
//     return (0);
// }


//MotorLineSt* MotorLine(motor_para_enum motor)
//{
//    return CmpnstValue.cmpMotor + motor;
//}



