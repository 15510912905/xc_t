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
    EN_DType_req = 0,  //������Ϣ����
    EN_DType_ack = 1,  //ȷ����Ϣ����
    EN_DType_nak = 2,  //����Ϣ����
    EN_DType_res = 3,  //��Ӧ��Ϣ����

    EN_DType_butt
}EN_DMsgType;
typedef struct
{
    uint16_t ushead;      //֡ͷ�̶�Ϊ0x550f
    uint16_t usdatalen;   //�������ݳ���
    uint16_t uscpuid;     //��λ��CPU��
    uint16_t usCrc;    //��������֡У���
    char  aucdata[];    //�·�Ϊ st_Can2Downmach������Ϊ msg_stDownmach2Can
}st_CanFormat;

//��λ�����
typedef enum
{
    DName_NDL = 0,          //��
    DName_Mix,          //��
    DName_AutoWash,     //��ϴ
    DName_SDisdk,       //����
    DName_ReagDisk,     //�Լ�
    DName_ReacDisk,     //��Ӧ
    DName_Temp,
    DName_AD1,
    DName_AD2,
}EN_DownName;

//��λ��CANID���
typedef enum
{
    CanId_NDL = DName_NDL,          //��
    CanId_Mix = DName_Mix,          //��
    CanId_AutoWash = DName_AutoWash,       //����
    CanId_SDisdk = DName_SDisdk,     //��ϴ  
    CanId_ReagDisk = DName_ReagDisk,     //�Լ�
    CanId_ReacDisk = DName_ReacDisk,     //��Ӧ
    CanId_Temp = DName_Temp,
    CanId_AD1 = DName_AD1,
    CanId_AD2 = DName_AD2
}EN_CanId;

typedef enum
{
    CR_Sucess = 0,
    CR_Failed = 1
}EN_CmdResult;

//=====��λ������ṹ����=====//
//===========���崮�ڹ���ģ������λ����λ����������ĵ�VOS��Ϣ�ṹ ��λ��-can==========
typedef struct
{
    char  enDwnSubSys;
    char  enMsgType;
    uint16_t usCmdInfoLen;
    uint32_t ulFrameid;

    uint16_t usCmdType;
    char  enExeRslt;      //����ִ�н��
    char  ucRsv[1];
    char aucinfo[];
}msg_stDownmach2Can;

//��λ����������can������
typedef struct
{
    char  enDwnSubSys;
    char  enMsgType;
    uint16_t usCmdInfoLen;
    uint32_t ulFrameid;

    char aucdata[];
}st_Can2Downmach;

//=====��������ṹ��=====
typedef struct 
{
    uint16_t cmd;
    uint8_t rsv[2];
}cmd_stComType; //��λ����ͨ������Ϣ�ṹ��

// #ifdef MID
// typedef struct
// {
//     uint32_t cmdSize;	 //����� (cmdInfo�Ĵ�С)
//     char cmdInfo[];
// }cmd_stSingleInfo;//������������ṹ��
// #else
typedef struct
{
    uint32_t cmdSize;	 //����� (cmdInfo�Ĵ�С)
    cmd_stComType cmdInfo;
}cmd_stSingleInfo;//������������ṹ��
/*#endif*/

typedef struct
{
    uint32_t cmdCount;//��������
    cmd_stSingleInfo singleCmd[];
}cmd_stComInfo;//������������ṹ��

//=====����ؼ���=====
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
    Move_Common,    //��ͨ��
    Move_Slow,      //���١�΢���ƶ�
    Move_Samp,      //����ת��
    Move_Reag1,     //���Լ�ת��
    Move_Reag2,     
    Move_Air,       //������ת��
    Move_PosDsk,    //ת����λ��
    Move_PosNum     //ת����λ��
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

//=====��λ������������Ϣ�ṹ��======
//ת��
typedef struct  
{
    uint16_t cmd;
    uint16_t usPosition;
}cmd_stSimpleMove;


//ת��
typedef struct
{
    uint16_t cmd;
    uint16_t usMoveType;
    uint16_t usDir;
    uint16_t usPara;
}cmd_stMove;

//�Զ���ϴ
typedef struct
{
    uint16_t cmd;
    uint16_t usWashBitMap;
}cmd_stAutoWash;

//�½�
typedef struct
{
    uint16_t cmd;
    uint16_t usTotalvol;
    uint16_t usBottlePos;
    uint16_t usReactCup;
}cmd_stDown;

//�÷�����
typedef struct
{
    uint16_t cmd;
    uint8_t ucPVNum;
    uint8_t ucCtrl;
}cmd_stPVCtrl;

//�ɹ�
typedef struct
{
    uint16_t cmd;
    uint8_t needNum; //�ɹ����
    uint8_t AdType; //͸�䡢ɢ��
}cmd_stReadAD;

//��ѯ
typedef struct
{
    uint16_t cmd;           //��ѯ����ؼ���:0xff09
    uint16_t usQueryType;     //��ѯ��״̬����
}cmd_stDwnQuery;




//=================================self================================



#if 0
//===========���ϻָ�����ṹ==========
typedef struct {
	uint16_t cmd;
    uint16_t rsv;  
    uint32_t faultId;
    uint16_t boardId;
    uint16_t deviceId;
}cmd_stRsmFault;

//===========��λ��״̬�·�����ṹ==========
typedef struct {
	uint16_t cmd;
    uint16_t midState;
}cmd_stMidState;

//��������ʾ��������cmd���ݽṹ
typedef struct
{
    uint16_t cmd;
    char  channel;
	char  switch_state;
}cmd_stShowave;

//===========��λ�������������ݽṹ==========
//��������
typedef struct {
    uint16_t location;   //����λ��
    uint16_t dir;        //�������� 1: ˳ʱ�� 2:��ʱ��
    uint32_t step;       //�������� 
}cmd_stCmpPara;

//��������
typedef struct {
    uint16_t cmd;      //����������
    uint16_t type;  //��������  1: ��λ����  2: ��ת���� 3: �������л� 4: ��� 5:��ֱ����
    uint32_t paranum;  //������������
    cmd_stCmpPara *ppara;  //�������� �䳤
}cmd_stCompensation;


//===========���΢�����������������ݽṹ==========
typedef struct
{
    uint16_t cmd;
    char    motorType;  //�������
    char    dir;        //����
    uint32_t step;       //΢����
}cmd_stStepMove;

//===========��Ӧ��ˮƽת�������������ݽṹ==========
typedef struct
{
    uint16_t cmd;
    uint16_t rttCupNum;      //ת���ı�λ��
}msg_stReactDskHrotate;

//===========��Ӧ�̹�Դ�ƿ����������ݽṹ==========
typedef struct {
    uint16_t cmd;
    uint16_t lightIntensity;  //��ǿ�� 0~255���ȵ���
}cmd_stLightControl;

//===========�¿ؿ����������ݽṹ==========
typedef struct {
    uint16_t cmd;
    uint16_t switchState;  //���ؿ��� 0-�ر� 1-��
}cmd_stTempSwitch;

//===========����AD�������ݽṹ==========
typedef struct {
	uint16_t cmd;    //������
    uint16_t srv;    //�����ֽ�
    uint32_t dpValue[WAVE_NUM];  //ÿ��ͨ����Ӧ��AD����ϵ��
}cmd_stAdConfig;

//===========������ˮƽת�������������ݽṹ==========
typedef struct
{
    uint16_t cmd;
    char  uccircleid;  //������Ȧ��
    char  uccupid;     //ָ���������Ż��ߴ�ת���ı�λ��
}msg_cmdSampleDskRtt;

//===========ע��������������ݽṹ==========
typedef struct
{
    uint16_t cmd;
    char  num;//ע��������������̵Ĵ���
    char  rsv;//Ԥ��
}msg_stNdlAirout;

//===========�Լ���ˮƽת�������������ݽṹ==========
typedef struct
{
    uint16_t cmd;
    char  ucPos;  //ת����λ��
    char  ucCupId;     //ָ�����Ż��ߴ�ת���ı�λ��
}cmd_stRDskMove;

//===========�Լ���������ʱ��ת�������������ݽṹ==========
typedef struct
{
    uint16_t cmd;
    char  cupNum;  //ת���ı�λ��
    char  rsv[1];  //ָ�����Ż��ߴ�ת���ı�λ��
}cmd_stRDskHrotate;

//===========�Լ���ת�������Լ�λ�����������ݽṹ==========
typedef struct
{
    uint16_t cmd;
    char  cupId;  //ת�����Լ�λ�ı�λ��
    char  rsv[1];  
}cmd_stRDskSwappos;

typedef struct
{
    uint16_t cmd;
    uint16_t cupNum;  //ת���ı�λ��
}cmd_stReactDskHrotate;

//===========������/�Լ���������or����ˮ�����������ݽṹ==========
typedef struct{
    uint16_t cmd;
    uint16_t quantity;//��������
}msg_stNdlAbsrb;

//===========�봹ֱת���������ݽṹ==========
typedef struct{
    uint16_t cmd;
    char  enposition; //�˶�����Ŀ��λ��
    char  reportmarginflag;//�ϱ�������־��1:�ϱ���0:���ϱ�
    uint16_t ustotalvol; //�������������������
    char  rsv[2];
    uint16_t diskid;   //�̺ţ����������ϱ�
    uint16_t cupid;    //��λ�ţ����������ϱ�
    uint32_t reactcup;   //����ţ�������졢©�촦��
}cmd_stNdlVrt;

//===========�����������������������ݽṹ==========
typedef struct{
    uint16_t cmd;
    uint16_t quantity;  //���Լ���
    char  diluteflag;
    char  rsv[3];
}msg_cmdSampleNdlDischarge;

//===========�Զ���ϴ�������ݽṹ==========
typedef struct
{
    uint16_t cmd;
    uint16_t washBitMap;
}msg_stAutoWash;

//===========��\�������������ݽṹ==========
typedef struct
{
    uint16_t cmd;
    char PVNum;
    char operateType;   //����
}cmd_stPVCtrl;

//===========��\�������������ݽṹ==========
typedef struct
{
    uint16_t cmd;
    char operateType;   //����
    char rsv[1];
}cmd_stPVCtrl;

//===========��һ�������ݽṹ==========
typedef struct{
    uint16_t cmd;
    char rsv[2];
}cmd_stComType;

//===========�Լ���X��λ��ת������ɨ��λ==========
typedef struct
{
    uint16_t cmd;
    char  uccupnum; //ĳ��λת�����Լ�λ
    char  rsv[1];
}msg_stReagdskRttToBcscan;

//===========������X��λ��ת������ɨ��λ==========
typedef struct
{
    uint16_t cmd;
    char  uccircleid;  //������Ȧ�� (0xFF: 3Ȧͬʱɨ��)
    char  uccupid;     //�����̱���:(0:��Ȧɨ�����ٷ������ݣ�1-��󱭺�:����ָ����������)
}msg_stSmpdskRttToBcscan;

//===========��ˮƽת���������ݽṹ==========
typedef struct{
	uint16_t cmd;
	char  enPosition;  //�˶�����Ŀ��λ��
	char  rsv[1];
}msg_stNdlHrotate;


//===========��λ�������ϱ��ṹ��==========
typedef struct
{
    char  endwnmachname;
    char  enmsgtrantype;
    uint16_t uscmdinfolen;
    uint32_t ulcurcmdframeid;
    
    uint16_t usCmdType;
    uint16_t enStatusType; //�ϱ���������
    uint16_t ulDwnmachName;  //��λ������
    uint16_t usParaNum;  //��������
    xc8001_report_compensation_para ppara[];  //�������� �䳤
}msg_stDwnmachCompensationReprot;


/*R1�Լ������Լ�����ṹ*/
typedef struct{
    uint16_t cmd;
    uint16_t quantity;  //���Լ���
    char  diluteflag;
    char  rsv[3];
}msg_stR1Discharge;


//---------------��λ������-����̬�л�--------------------
typedef struct
{
    uint16_t cmd;        //����ؼ���0xff0a
    uint16_t operation;  //�л�0:����̬��1:����̬
}cmd_stMachineDebug;


//===========����=========
typedef struct
{
    vos_u16 cmd;
    uint16_t operation; //1:����0:��
}cmd_switch;


typedef enum
{
    samp_key_led_off = 0,
    samp_key_led_on = 1,
    samp_key_led_flicker = 2,
    samp_key_led_butt,
}samp_key_led_value;

//����ָʾ�ƿ���
typedef struct
{
    vos_u16 cmd;
    char    ctl;
    char    rsv;
}cmd_sample_lightCtl;

//��λ���������ݴ���
typedef struct
{
    vos_u16 cmd;
    vos_u16 exeCmd;//ִ�ж���
    uint32_t dataPage;//ҳ��
    char data[DWNPAGE_SIZE];//����
}cmd_updateDwn;

//����FPGA
typedef struct
{
    vos_u16 cmd;
    vos_u16 flag;  //�Ƿ�Ϊ���һ֡���ݱ�־(�·�����λ��)
    vos_u16 dataPage;//��֡����ִ��״̬(����λ������)
    vos_u16 length;//data�ĳ���(ȡֵֻ��Ϊ0----256)
    char  data[DWNPAGE_SIZE]; //��������
}cmd_updateFpga;
#endif

#endif
