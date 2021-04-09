#ifndef _app_down_msg_h_
#define _app_down_msg_h_


#include "cmd_downmachine.h"
#include <stdio.h>
#include "stdint.h"

#define WAVE_NUM (14) 

typedef enum
{
    EN_p01 = 0,
    EN_p02 = 1,
    EN_p03 = 2,
    EN_p04 = 3,
    EN_p05 = 4,
    EN_v01 = 5
}EN_PvNum;

typedef enum
{
    EN_DType_req = 0,  //请求消息传输
    EN_DType_ack = 1,  //确认消息传输
    EN_DType_nak = 2,  //否定消息传输
    EN_DType_res = 3,  //响应消息传输

    EN_DType_butt
}EN_DMsgType;
typedef struct
{
    uint16_t ushead;      //帧头固定为0x550f
    uint16_t usdatalen;   //后面数据长度
    uint16_t uscpuid;     //下位机CPU号
    uint16_t usCrc;    //整个数据帧校验和
    char  aucdata[];    //下发为 st_Can2Downmach，接收为 msg_stDownmach2Can
}st_CanFormat;

//下位机编号
typedef enum
{
    DName_NDL = 0,          //针
    DName_Mix,          //杆
    DName_AutoWash,     //清洗
    DName_SDisdk,       //样本
    DName_ReagDisk,     //试剂
    DName_ReacDisk,     //反应
    DName_Temp,
    DName_AD1,
    DName_AD2,
}EN_DownName;

//下位机CANID编号
typedef enum
{
    CanId_NDL = DName_NDL,          //针
    CanId_Mix = DName_Mix,          //杆
    CanId_AutoWash = DName_AutoWash,       //样本
    CanId_SDisdk = DName_SDisdk,     //清洗  
    CanId_ReagDisk = DName_ReagDisk,     //试剂
    CanId_ReacDisk = DName_ReacDisk,     //反应
    CanId_Temp = DName_Temp,
    CanId_AD1 = DName_AD1,
    CanId_AD2 = DName_AD2
}EN_CanId;

typedef enum
{
    CR_Sucess = 0,
    CR_Failed = 1
}EN_CmdResult;

//=====下位机命令结构定义=====//
//===========定义串口管理模块与中位机下位机命令交互中心的VOS消息结构 下位机-can==========
typedef struct
{
    char  enDwnSubSys;
    char  enMsgType;
    uint16_t usCmdInfoLen;
    uint32_t ulFrameid;

    uint16_t usCmdType;
    char  enExeRslt;      //命令执行结果
    char  ucRsv[1];
    char aucinfo[];
}msg_stDownmach2Can;

//下位机动作请求（can发往）
typedef struct
{
    char  enDwnSubSys;
    char  enMsgType;
    uint16_t usCmdInfoLen;
    uint32_t ulFrameid;

    char aucdata[];
}st_Can2Downmach;

//=====命令解析结构体=====
typedef struct 
{
    uint16_t cmd;
    uint8_t rsv[2];
}cmd_stComType; //下位机普通命令信息结构体

// #ifdef MID
// typedef struct
// {
//     uint32_t cmdSize;	 //命令长度 (cmdInfo的大小)
//     char cmdInfo[];
// }cmd_stSingleInfo;//单个命令解析结构体
// #else
typedef struct
{
    uint32_t cmdSize;	 //命令长度 (cmdInfo的大小)
    cmd_stComType cmdInfo;
}cmd_stSingleInfo;//单个命令解析结构体
/*#endif*/

typedef struct
{
    uint32_t cmdCount;//命令条数
    cmd_stSingleInfo singleCmd[];
}cmd_stComInfo;//公用命令解析结构体

//=====命令关键字=====
typedef enum
{
    DCmd_Rest = 0x1000,
    DCmd_Move = 0x1001,
    DCmd_Wash = 0x1002,
    DCmd_UP = 0x1003,
    DCmd_Down = 0x1004,
    DCmd_PVCtrl = 0x1005,
    DCmd_PumpCtrl = 0x1006,
    DCmd_Mix = 0x1007,
    DCmd_Query = 0x1008,
    Dcmd_Absorb = 0x1009,
    Dcmd_Discharge = 0x100a,
    Dcmd_readAD = 0x100b
}EN_DownCmdType;

typedef enum
{
    Dir_Up = 1,//Dir_Up = 0,
    Dir_Down = 0 ,//Dir_Down = 1,
    Dir_In = 1,
    Dir_Out = 0,
    Dir_clckw = 1,
    Dir_aclckW = 0,
    DIR_IDLE = 0xf
}EN_DirType;

typedef enum
{
    NPos_Reagent = 4,
    NPos_Sample = 0,
    NPos_Wash = 1,
    NPos_dscrgS = 3,
    NPos_dscrg1 = 3,
    NPos_dscrg2 = 2
}EN_NdlPosition;

typedef enum
{
    MPos_Wash = 0,
    MPos_Mix
}EN_MixPosition;

typedef enum
{
    Move_Common,    //简单通用
    Move_Slow,      //低速、微步移动
    Move_Samp,      //吸样转动
    Move_Reag1,     //吸试剂转到
    Move_Reag2,     
    Move_Air,       //吸空气转动
    Move_PosDsk,    //转动到位置
    Move_PosNum     //转动杯位数
}EN_MoveType;

typedef enum
{
    ADChose_S = 0,
    ADChose_T = 1
}EN_ADChose;

//===
typedef struct
{
    uint8_t needNum;
    uint8_t readNum;
    uint16_t ad[39];
}st_AdReadMng;

//=====下位机特殊命令信息结构体======
//转动
typedef struct  
{
    uint16_t cmd;
    uint16_t usPosition;
}cmd_stSimpleMove;


//转动
typedef struct
{
    uint16_t cmd;
    uint16_t usMoveType;
    uint16_t usDir;
    uint16_t usPara;
}cmd_stMove;

//自动清洗
typedef struct
{
    uint16_t cmd;
    uint16_t usWashBitMap;
}cmd_stAutoWash;

//下降
typedef struct
{
    uint16_t cmd;
    uint16_t usTotalvol;
    uint16_t usBottlePos;
    uint16_t usReactCup;
}cmd_stDown;

//泵阀控制
typedef struct
{
    uint16_t cmd;
    uint8_t ucPVNum;
    uint8_t ucCtrl;
}cmd_stPVCtrl;

//采光
typedef struct
{
    uint16_t cmd;
    uint8_t needNum; //采光个数
    uint8_t AdType; //透射、散射
}cmd_stReadAD;

//查询
typedef struct
{
    uint16_t cmd;           //查询命令关键字:0xff09
    uint16_t usQueryType;     //查询的状态类型
}cmd_stDwnQuery;




//=================================self================================



#if 0
//===========故障恢复命令结构==========
typedef struct {
	uint16_t cmd;
    uint16_t rsv;  
    uint32_t faultId;
    uint16_t boardId;
    uint16_t deviceId;
}cmd_stRsmFault;

//===========中位机状态下发命令结构==========
typedef struct {
	uint16_t cmd;
    uint16_t midState;
}cmd_stMidState;

//定义虚拟示波器开关cmd数据结构
typedef struct
{
    uint16_t cmd;
    char  channel;
	char  switch_state;
}cmd_stShowave;

//===========下位机补偿命令数据结构==========
//补偿参数
typedef struct {
    uint16_t location;   //补偿位置
    uint16_t dir;        //补偿方向 1: 顺时针 2:逆时针
    uint32_t step;       //补偿步数 
}cmd_stCmpPara;

//补偿命令
typedef struct {
    uint16_t cmd;      //补偿命令字
    uint16_t type;  //补偿类型  1: 复位补偿  2: 旋转补偿 3: 主备区切换 4: 清除 5:垂直补偿
    uint32_t paranum;  //补偿参数个数
    cmd_stCmpPara *ppara;  //补偿参数 变长
}cmd_stCompensation;


//===========电机微步调整动作命令数据结构==========
typedef struct
{
    uint16_t cmd;
    char    motorType;  //电机类型
    char    dir;        //方向
    uint32_t step;       //微步数
}cmd_stStepMove;

//===========反应盘水平转动动作命令数据结构==========
typedef struct
{
    uint16_t cmd;
    uint16_t rttCupNum;      //转动的杯位数
}msg_stReactDskHrotate;

//===========反应盘光源灯控制命令数据结构==========
typedef struct {
    uint16_t cmd;
    uint16_t lightIntensity;  //光强度 0~255亮度调整
}cmd_stLightControl;

//===========温控开关命令数据结构==========
typedef struct {
    uint16_t cmd;
    uint16_t switchState;  //开关控制 0-关闭 1-打开
}cmd_stTempSwitch;

//===========配置AD控制数据结构==========
typedef struct {
	uint16_t cmd;    //命令字
    uint16_t srv;    //保留字节
    uint32_t dpValue[WAVE_NUM];  //每个通道对应的AD增益系数
}cmd_stAdConfig;

//===========样本盘水平转动动作命令数据结构==========
typedef struct
{
    uint16_t cmd;
    char  uccircleid;  //样本盘圈号
    char  uccupid;     //指定样本杯号或者待转过的杯位数
}msg_cmdSampleDskRtt;

//===========注射泵排气命令数据结构==========
typedef struct
{
    uint16_t cmd;
    char  num;//注射泵排气整个过程的次数
    char  rsv;//预留
}msg_stNdlAirout;

//===========试剂盘水平转动动作命令数据结构==========
typedef struct
{
    uint16_t cmd;
    char  ucPos;  //转到的位置
    char  ucCupId;     //指定杯号或者待转过的杯位数
}cmd_stRDskMove;

//===========试剂盘正、逆时针转动动作命令数据结构==========
typedef struct
{
    uint16_t cmd;
    char  cupNum;  //转过的杯位数
    char  rsv[1];  //指定杯号或者待转过的杯位数
}cmd_stRDskHrotate;

//===========试剂盘转动到换试剂位动作命令数据结构==========
typedef struct
{
    uint16_t cmd;
    char  cupId;  //转到换试剂位的杯位号
    char  rsv[1];  
}cmd_stRDskSwappos;

typedef struct
{
    uint16_t cmd;
    uint16_t cupNum;  //转过的杯位数
}cmd_stReactDskHrotate;

//===========样本针/试剂针吸样本or蒸馏水动作命令数据结构==========
typedef struct{
    uint16_t cmd;
    uint16_t quantity;//吸样本量
}msg_stNdlAbsrb;

//===========针垂直转动命令数据结构==========
typedef struct{
    uint16_t cmd;
    char  enposition; //运动到的目标位置
    char  reportmarginflag;//上报余量标志，1:上报，0:不上报
    uint16_t ustotalvol; //杯总体积用于随量跟踪
    char  rsv[2];
    uint16_t diskid;   //盘号，用于余量上报
    uint16_t cupid;    //杯位号，用于余量上报
    uint32_t reactcup;   //任务号，用于误检、漏检处理
}cmd_stNdlVrt;

//===========样本针排样本动作命令数据结构==========
typedef struct{
    uint16_t cmd;
    uint16_t quantity;  //排试剂量
    char  diluteflag;
    char  rsv[3];
}msg_cmdSampleNdlDischarge;

//===========自动清洗命令数据结构==========
typedef struct
{
    uint16_t cmd;
    uint16_t washBitMap;
}msg_stAutoWash;

//===========泵\阀控制命令数据结构==========
typedef struct
{
    uint16_t cmd;
    char PVNum;
    char operateType;   //开关
}cmd_stPVCtrl;

//===========泵\阀控制命令数据结构==========
typedef struct
{
    uint16_t cmd;
    char operateType;   //开关
    char rsv[1];
}cmd_stPVCtrl;

//===========单一命令数据结构==========
typedef struct{
    uint16_t cmd;
    char rsv[2];
}cmd_stComType;

//===========试剂盘X杯位旋转到条码扫描位==========
typedef struct
{
    uint16_t cmd;
    char  uccupnum; //某杯位转到换试剂位
    char  rsv[1];
}msg_stReagdskRttToBcscan;

//===========样本盘X杯位旋转到条码扫描位==========
typedef struct
{
    uint16_t cmd;
    char  uccircleid;  //样本盘圈号 (0xFF: 3圈同时扫描)
    char  uccupid;     //样本盘杯号:(0:整圈扫描完再返回数据，1-最大杯号:返回指定杯号数据)
}msg_stSmpdskRttToBcscan;

//===========针水平转动命令数据结构==========
typedef struct{
	uint16_t cmd;
	char  enPosition;  //运动到的目标位置
	char  rsv[1];
}msg_stNdlHrotate;


//===========下位机补偿上报结构体==========
typedef struct
{
    char  endwnmachname;
    char  enmsgtrantype;
    uint16_t uscmdinfolen;
    uint32_t ulcurcmdframeid;
    
    uint16_t usCmdType;
    uint16_t enStatusType; //上报数据类型
    uint16_t ulDwnmachName;  //下位机名称
    uint16_t usParaNum;  //参数个数
    xc8001_report_compensation_para ppara[];  //补偿参数 变长
}msg_stDwnmachCompensationReprot;


/*R1试剂针排试剂命令结构*/
typedef struct{
    uint16_t cmd;
    uint16_t quantity;  //排试剂量
    char  diluteflag;
    char  rsv[3];
}msg_stR1Discharge;


//---------------下位机调试-正常态切换--------------------
typedef struct
{
    uint16_t cmd;        //命令关键字0xff0a
    uint16_t operation;  //切换0:调试态，1:正常态
}cmd_stMachineDebug;


//===========开关=========
typedef struct
{
    vos_u16 cmd;
    uint16_t operation; //1:开、0:关
}cmd_switch;


typedef enum
{
    samp_key_led_off = 0,
    samp_key_led_on = 1,
    samp_key_led_flicker = 2,
    samp_key_led_butt,
}samp_key_led_value;

//样本指示灯控制
typedef struct
{
    vos_u16 cmd;
    char    ctl;
    char    rsv;
}cmd_sample_lightCtl;

//下位机升级数据传输
typedef struct
{
    vos_u16 cmd;
    vos_u16 exeCmd;//执行动作
    uint32_t dataPage;//页数
    char data[DWNPAGE_SIZE];//数据
}cmd_updateDwn;

//升级FPGA
typedef struct
{
    vos_u16 cmd;
    vos_u16 flag;  //是否为最后一帧数据标志(下发给下位机)
    vos_u16 dataPage;//本帧数据执行状态(从下位机反馈)
    vos_u16 length;//data的长度(取值只能为0----256)
    char  data[DWNPAGE_SIZE]; //数据内容
}cmd_updateFpga;
#endif

#endif
