/*-----------------------------*
 *  include head files         *
 *----------------------------*/
#include ".\Head\HD_cfg.h"
#include ".\Head\Time.h"
#include ".\Head\Script.h"
#include ".\Head\TempCtr.h"
#include ".\Head\System.h"

/*-----------------------------*
 *  Macros for constants       *
 *----------------------------*/
//#define		BOIL_TEMP_SIZE		12		//临时数据缓冲的大小
//#define		BOIL_OFFSET_VAL		1		//判断沸点的偏移值
//#define		BOIL_DATA_PER_MIN	12	    //每分钟更新数据数

/*-----------------------------*
 *  Macros for others          *
 *----------------------------*/


/*-----------------------------*
 *  type definitions           *
 *----------------------------*/


/*-----------------------------*
 *  structure,union and enum   *
 *----------------------------*/


/*-----------------------------*
 *  public functions prototypes*
 *----------------------------*/
void 	ScriptProc(void);
//void	BoilCheckProc(void);
#ifdef	DEBUG0
uint8	GetScriptStage(void);
#endif
void	ResetCurScript(void);
void	SetCurScript(uint8 u8ID,uint8 u8Diy,uint8 u8Temp);
void	SetBotTemp(uint8 code *pScript);
//void	SetPowerLevel(uint8	code *pScript);
void	Heat_Ctr(uint8 code *pScript);
uint8	OtherCmds(uint8	code *pScript);
uint8	ByTemp_ChgStage(uint8 code *pScript);
uint8	ByTime_ChgStage(uint8 code *pScript);
void	ChangeScriptStage(uint8	u8Stage);
void	GetPowerByDT(void);

/*-----------------------------*
 *  static functions prototypes*
 *----------------------------*/


/*-----------------------------*
 *  public variable declaration*
 *----------------------------*/
uint8	code *m_u8pBaseScript =	NULL;
uint8	code *m_u8pScript	=	NULL;
uint8	m_u8ScriptStage		=	0;				//script状态
uint16	m_u16CookTime		=	0;				//烹饪时间,以秒为单位
uint8	m_u8AddOnParam		=	0;				//临时参数
uint8	m_u8AddOffParam		=	0;
uint16	m_u16IntCookTime	=	0;			    //间隔烹饪时间,以秒为单位
uint16  m_u16MashTime		=	0;              //米饭糊化时间,以秒为单位 

//火力控制
uint8	idata m_u8OffsetTemp = 0;				//偏移温度
uint8	idata m_u8TargetTemp = 0;				//目标温度
uint8	idata m_u8EstimateTemp = 0;             //预估温度
uint8	idata m_u8PwrCycle		=	0;			//调功周期
uint8	idata m_u8OnTimeInc		=	0;			//加热时间增量
uint8	idata m_u8OnTimeDec		=	0;			//加热时间减量
uint8	idata m_u8MinOnTime_Set	=	0;			//最小加热时间
uint8	idata m_u8MaxOnTime_set	=	0;			//最大加热时间	
                            
//沸腾判断
//uint8	idata   u8BoilTempBuf[BOIL_TEMP_SIZE];	//临时数据缓冲
//uint8	idata	u8BoilTempBufBAK[BOIL_TEMP_SIZE]; //数据备份
//uint8	idata	u8BoilChkCnt	=	0;			
//uint8	idata	m_u8BoilChkTime	=	0;
//uint8	idata	m_u8BoilDegree	=	0;	

uint8	code	m_u8aTemp_40[] =
{
	0x02,0x0C,
	//------------====== 状态 0 ======-------------
	0x00,0x28,0x28,0x14,0x05,0x00,          //底部温度范围
	0x01,0x20,0x01,          //底部温度大于指定温度跳转
	0xFF,          //状态结束
	//------------====== 状态 1 ======-------------
	0x00,0x27,0x27,0x05,0x19,0x00,          //底部温度范围
	0x02,0x1E,0x00,          //底部温度小于指定温度跳转
	0xFF,          //状态结束
};

uint8	code	m_u8aTemp_50[] =
{
	0x02,0x0C,
	//------------====== 状态 0 ======-------------
	0x00,0x32,0x32,0x14,0x05,0x00,          //底部温度范围
	0x01,0x2A,0x01,          //底部温度大于指定温度跳转
	0xFF,          //状态结束
	//------------====== 状态 1 ======-------------
	0x00,0x30,0x30,0x05,0x19,0x00,          //底部温度范围
	0x02,0x28,0x00,          //底部温度小于指定温度跳转
	0xFF,          //状态结束

};

uint8	code	m_u8aTemp_60[] =
{
	0x02,0x0C,
	//------------====== 状态 0 ======-------------
	0x00,0x3C,0x3C,0x14,0x05,0x00,          //底部温度范围
	0x01,0x34,0x01,          //底部温度大于指定温度跳转
	0xFF,          //状态结束
	//------------====== 状态 1 ======-------------
	0x00,0x3A,0x3A,0x05,0x19,0x00,          //底部温度范围
	0x02,0x32,0x00,          //底部温度小于指定温度跳转
	0xFF,          //状态结束

};

uint8	code	m_u8aTemp_70[] =
{
	0x02,0x0C,
	//------------====== 状态 0 ======-------------
	0x00,0x46,0x46,0x14,0x05,0x00,          //底部温度范围
	0x01,0x3E,0x01,          //底部温度大于指定温度跳转
	0xFF,          //状态结束
	//------------====== 状态 1 ======-------------
	0x00,0x44,0x44,0x05,0x19,0x00,          //底部温度范围
	0x02,0x3C,0x00,          //底部温度小于指定温度跳转
	0xFF,          //状态结束
};

uint8	code	m_u8aTemp_80[] =
{
    0x02,0x0C,
	//------------====== 状态 0 ======-------------
	0x00,0x50,0x50,0x14,0x05,0x00,          //底部温度范围
	0x01,0x48,0x01,          //底部温度大于指定温度跳转
	0xFF,          //状态结束
	//------------====== 状态 1 ======-------------
	0x00,0x4C,0x4C,0x05,0x19,0x00,          //底部温度范围
	0x02,0x46,0x00,          //底部温度小于指定温度跳转
	0xFF,          //状态结束

};

uint8	code	m_u8aTemp_90[] =
{
	0x02,0x0C,
	//------------====== 状态 0 ======-------------
	0x00,0x5A,0x5A,0x14,0x05,0x00,          //底部温度范围
	0x01,0x50,0x01,          //底部温度大于指定温度跳转
	0xFF,          //状态结束
	//------------====== 状态 1 ======-------------
	0x00,0x56,0x56,0x05,0x19,0x00,          //底部温度范围
	0x02,0x4F,0x00,          //底部温度小于指定温度跳转
	0xFF,          //状态结束
};

uint8	code	m_u8aTemp_100[] =
{
    0x05,0x0F,0x1A,0x24,0x2E,
	//------------====== 状态 0 ======-------------
	0x00,0x5C,0x5D,0x14,0x05,0x00,          //底部温度范围
	0x01,0x46,0x81,          //底部温度大于指定温度跳转
	0xFF,          //状态结束
	//------------====== 状态 1 ======-------------
	0x00,0x2E,0x30,0x02,0x1C,0x00,          //底部温度范围
	0x07,0x1E,0x00,0x02,          //烹饪时间大于指定秒钟跳转
	0xFF,          //状态结束
	//------------====== 状态 2 ======-------------
	0x00,0x5C,0x5D,0x0A,0x0A,0x00,          //底部温度范围
	0x01,0x58,0x04,          //底部温度大于指定温度跳转
	0xFF,          //状态结束
	//------------====== 状态 3 ======-------------
	0x00,0x5C,0x5D,0x14,0x05,0x00,          //底部温度范围
	0x01,0x58,0x04,          //底部温度大于指定温度跳转
	0xFF,          //状态结束
	//------------====== 状态 4 ======-------------
	0x00,0x5C,0x5C,0xF3,0xF4,0x00,          //底部温度范围
	0x02,0x58,0x03,          //底部温度小于指定温度跳转
	0xFF,          //状态结束
};

uint8	code	m_u8aTemp_110[] =
{
    0x05,0x0F,0x1A,0x24,0x2E,
	//------------====== 状态 0 ======-------------
	0x00,0x64,0x64,0x14,0x05,0x00,          //底部温度范围
	0x01,0x50,0x81,          //底部温度大于指定温度跳转
	0xFF,          //状态结束
	//------------====== 状态 1 ======-------------
	0x00,0x2E,0x30,0x02,0x1C,0x00,          //底部温度范围
	0x07,0x1E,0x00,0x02,          //烹饪时间大于指定秒钟跳转
	0xFF,          //状态结束
	//------------====== 状态 2 ======-------------
	0x00,0x64,0x64,0x0A,0x0A,0x00,          //底部温度范围
	0x01,0x5F,0x04,          //底部温度大于指定温度跳转
	0xFF,          //状态结束
	//------------====== 状态 3 ======-------------
	0x00,0x64,0x64,0x14,0x05,0x00,          //底部温度范围
	0x01,0x5F,0x04,          //底部温度大于指定温度跳转
	0xFF,          //状态结束
	//------------====== 状态 4 ======-------------
	0x00,0x64,0x64,0xF3,0xF4,0x00,          //底部温度范围
	0x02,0x5F,0x03,          //底部温度小于指定温度跳转
	0xFF,          //状态结束
};

uint8	code	m_u8aTemp_120[] =
{
    0x05,0x0F,0x1A,0x24,0x2E,
	//------------====== 状态 0 ======-------------
	0x00,0x6C,0x6C,0x14,0x05,0x00,          //底部温度范围
	0x01,0x55,0x81,          //底部温度大于指定温度跳转
	0xFF,          //状态结束
	//------------====== 状态 1 ======-------------
	0x00,0x2E,0x30,0x02,0x1C,0x00,          //底部温度范围
	0x07,0x1E,0x00,0x02,          //烹饪时间大于指定秒钟跳转
	0xFF,          //状态结束
	//------------====== 状态 2 ======-------------
	0x00,0x6C,0x6C,0x0A,0x0A,0x00,          //底部温度范围
	0x01,0x67,0x04,          //底部温度大于指定温度跳转
	0xFF,          //状态结束
	//------------====== 状态 3 ======-------------
	0x00,0x6C,0x6C,0x14,0x05,0x00,          //底部温度范围
	0x01,0x67,0x04,          //底部温度大于指定温度跳转
	0xFF,          //状态结束
	//------------====== 状态 4 ======-------------
	0x00,0x6C,0x6C,0xF3,0xF4,0x00,          //底部温度范围
	0x02,0x67,0x03,          //底部温度小于指定温度跳转
	0xFF,          //状态结束
};

uint8	code	m_u8aTemp_130[] =
{
    0x05,0x0F,0x1A,0x24,0x2E,
	//------------====== 状态 0 ======-------------
	0x00,0x74,0x74,0x14,0x05,0x00,          //底部温度范围
	0x01,0x5A,0x81,          //底部温度大于指定温度跳转
	0xFF,          //状态结束
	//------------====== 状态 1 ======-------------
	0x00,0x2E,0x30,0x02,0x1C,0x00,          //底部温度范围
	0x07,0x1E,0x00,0x02,          //烹饪时间大于指定秒钟跳转
	0xFF,          //状态结束
	//------------====== 状态 2 ======-------------
	0x00,0x74,0x74,0x0A,0x0A,0x00,          //底部温度范围
	0x01,0x6F,0x04,          //底部温度大于指定温度跳转
	0xFF,          //状态结束
	//------------====== 状态 3 ======-------------
	0x00,0x74,0x74,0x14,0x05,0x00,          //底部温度范围
	0x01,0x6F,0x04,          //底部温度大于指定温度跳转
	0xFF,          //状态结束
	//------------====== 状态 4 ======-------------
	0x00,0x74,0x74,0xF3,0xF4,0x00,          //底部温度范围
	0x02,0x6F,0x03,          //底部温度小于指定温度跳转
	0xFF,          //状态结束

};

uint8 code m_u8aTemp_140[] =
{
    0x05,0x0F,0x1A,0x24,0x2E,
	//------------====== 状态 0 ======-------------
	0x00,0x7C,0x7C,0x14,0x05,0x00,          //底部温度范围
	0x01,0x64,0x81,          //底部温度大于指定温度跳转
	0xFF,          //状态结束
	//------------====== 状态 1 ======-------------
	0x00,0x2E,0x30,0x02,0x1C,0x00,          //底部温度范围
	0x07,0x32,0x00,0x02,          //烹饪时间大于指定秒钟跳转
	0xFF,          //状态结束
	//------------====== 状态 2 ======-------------
	0x00,0x7C,0x7C,0x0A,0x0A,0x00,          //底部温度范围
	0x01,0x77,0x04,          //底部温度大于指定温度跳转
	0xFF,          //状态结束
	//------------====== 状态 3 ======-------------
	0x00,0x7C,0x7C,0x14,0x05,0x00,          //底部温度范围
	0x01,0x77,0x04,          //底部温度大于指定温度跳转
	0xFF,          //状态结束
	//------------====== 状态 4 ======-------------
	0x00,0x7C,0x7C,0xF3,0xF4,0x00,          //底部温度范围
	0x02,0x77,0x03,          //底部温度小于指定温度跳转
	0xFF,          //状态结束

};

uint8	code	m_u8aTemp_150[] =
{
    0x05,0x0F,0x1A,0x24,0x2E,
	//------------====== 状态 0 ======-------------
	0x00,0x82,0x82,0x14,0x05,0x00,          //底部温度范围
	0x01,0x64,0x81,          //底部温度大于指定温度跳转
	0xFF,          //状态结束
	//------------====== 状态 1 ======-------------
	0x00,0x2E,0x30,0x02,0x1C,0x00,          //底部温度范围
	0x07,0x32,0x00,0x02,          //烹饪时间大于指定秒钟跳转
	0xFF,          //状态结束
	//------------====== 状态 2 ======-------------
	0x00,0x82,0x82,0x0A,0x0A,0x00,          //底部温度范围
	0x01,0x7D,0x04,          //底部温度大于指定温度跳转
	0xFF,          //状态结束
	//------------====== 状态 3 ======-------------
	0x00,0x82,0x82,0x14,0x05,0x00,          //底部温度范围
	0x01,0x7D,0x04,          //底部温度大于指定温度跳转
	0xFF,          //状态结束
	//------------====== 状态 4 ======-------------
	0x00,0x82,0x82,0xF3,0xF4,0x00,          //底部温度范围
	0x02,0x7D,0x03,          //底部温度小于指定温度跳转
	0xFF,          //状态结束

};

uint8	code	m_u8aTemp_160[] =
{
	0x05,0x0F,0x1A,0x24,0x2E,
	//------------====== 状态 0 ======-------------
	0x00,0x7B,0x7B,0x14,0x05,0x00,          //底部温度范围
	0x01,0x64,0x81,          //底部温度大于指定温度跳转
	0xFF,          //状态结束
	//------------====== 状态 1 ======-------------
	0x00,0x2E,0x30,0x02,0x1C,0x00,          //底部温度范围
	0x07,0x32,0x00,0x02,          //烹饪时间大于指定秒钟跳转
	0xFF,          //状态结束
	//------------====== 状态 2 ======-------------
	0x00,0x7B,0x7B,0x0A,0x0A,0x00,          //底部温度范围
	0x01,0x76,0x04,          //底部温度大于指定温度跳转
	0xFF,          //状态结束
	//------------====== 状态 3 ======-------------
	0x00,0x7B,0x7B,0x14,0x05,0x00,          //底部温度范围
	0x01,0x76,0x04,          //底部温度大于指定温度跳转
	0xFF,          //状态结束
	//------------====== 状态 4 ======-------------
	0x00,0x7B,0x7B,0xF3,0xF4,0x00,          //底部温度范围
	0x02,0x76,0x03,          //底部温度小于指定温度跳转
	0xFF,          //状态结束
};

uint8	code	m_u8aRice[]	=
{
    0x0B,0x19,0x24,0x37,0x4A,0x5D,0x70,0x83,0x8E,0x99,0xA4,
	//------------====== 状态 0 ======-------------
	0x00,0x55,0x5A,0xF0,0x05,0x00,          //底部温度范围
	0x01,0x41,0x81,          //底部温度大于指定温度跳转
	0x05,0x08,0x00,0x81,          //烹饪时间大于指定分钟跳转
	0xFF,          //状态结束
	//------------====== 状态 1 ======-------------
	0x00,0x3C,0x41,0x07,0x14,0x00,          //底部温度范围
	0x05,0x08,0x00,0x82,          //烹饪时间大于指定分钟跳转
	0xFF,          //状态结束
	//------------====== 状态 2 ======-------------
	0x00,0x78,0x82,0xF0,0x05,0x00,          //底部温度范围
	0x03,0x41,0x03,          //顶部温度大于指定温度跳转
	0x0E,0x03,0x02,0x41,0x0A,          //特别指令
	0x05,0x23,0x00,0x03,          //烹饪时间大于指定分钟跳转
	0xFF,          //状态结束
	//------------====== 状态 3 ======-------------
	0x00,0x78,0x82,0xF1,0xF1,0x00,          //底部温度范围
	0x03,0x50,0x84,          //顶部温度大于指定温度跳转
	0x0E,0x03,0x03,0x55,0x01,          //特别指令
	0x05,0x23,0x00,0x84,          //烹饪时间大于指定分钟跳转
	0xFF,          //状态结束
	//------------====== 状态 4 ======-------------
	0x00,0x78,0x82,0xF1,0xF1,0x00,          //底部温度范围
	0x03,0x58,0x85,          //顶部温度大于指定温度跳转
	0x0E,0x03,0x03,0x62,0x01,          //特别指令
	0x05,0x0A,0x00,0x85,          //烹饪时间大于指定分钟跳转
	0xFF,          //状态结束
	//------------====== 状态 5 ======-------------
	0x00,0x82,0x87,0xF1,0xF1,0x00,          //底部温度范围
	0x01,0x76,0x87,          //底部温度大于指定温度跳转
	0x05,0x08,0x00,0x86,          //烹饪时间大于指定分钟跳转
	0x0E,0x03,0x00,0x01,0x00,          //特别指令
	0xFF,          //状态结束
	//------------====== 状态 6 ======-------------
	0x00,0x82,0x87,0xF1,0xF1,0x00,          //底部温度范围
	0x01,0x76,0x87,          //底部温度大于指定温度跳转
	0x05,0x08,0x00,0x87,          //烹饪时间大于指定分钟跳转
	0x0E,0x03,0x00,0x02,0x00,          //特别指令
	0xFF,          //状态结束
	//------------====== 状态 7 ======-------------
	0x00,0x67,0x69,0x06,0x0F,0x00,          //底部温度范围
	0x05,0xF2,0x00,0x88,          //烹饪时间大于指定分钟跳转
	0xFF,          //状态结束
	//------------====== 状态 8 ======-------------
	0x00,0x5A,0x5C,0x03,0x19,0x00,          //底部温度范围
	0x05,0x04,0x00,0x89,          //烹饪时间大于指定分钟跳转
	0xFF,          //状态结束
	//------------====== 状态 9 ======-------------
	0x00,0x1E,0x28,0x02,0x1C,0x00,          //底部温度范围
	0x05,0x04,0x00,0x8A,          //烹饪时间大于指定分钟跳转
	0xFF,          //状态结束
	//------------====== 状态 10 ======-------------
	0x0E,0x01,0x00,0x00,0x00,          //特别指令
	0xFF,          //状态结束
};

uint8	code	m_u8aPilaf[] = 
{
    0x0B,0x19,0x24,0x37,0x4A,0x5D,0x70,0x83,0x8E,0x99,0xA4,
	//------------====== 状态 0 ======-------------
	0x00,0x55,0x5A,0xF0,0x05,0x00,          //底部温度范围
	0x01,0x41,0x81,          //底部温度大于指定温度跳转
	0x05,0x08,0x00,0x81,          //烹饪时间大于指定分钟跳转
	0xFF,          //状态结束
	//------------====== 状态 1 ======-------------
	0x00,0x3C,0x41,0x07,0x14,0x00,          //底部温度范围
	0x05,0x01,0x00,0x82,          //烹饪时间大于指定分钟跳转
	0xFF,          //状态结束
	//------------====== 状态 2 ======-------------
	0x00,0x78,0x82,0xF0,0x05,0x00,          //底部温度范围
	0x03,0x41,0x03,          //顶部温度大于指定温度跳转
	0x0E,0x03,0x02,0x41,0x0A,          //特别指令
	0x05,0x23,0x00,0x03,          //烹饪时间大于指定分钟跳转
	0xFF,          //状态结束
	//------------====== 状态 3 ======-------------
	0x00,0x78,0x82,0xF1,0xF1,0x00,          //底部温度范围
	0x03,0x50,0x84,          //顶部温度大于指定温度跳转
	0x0E,0x03,0x03,0x55,0x01,          //特别指令
	0x05,0x23,0x00,0x84,          //烹饪时间大于指定分钟跳转
	0xFF,          //状态结束
	//------------====== 状态 4 ======-------------
	0x00,0x78,0x82,0xF1,0xF1,0x00,          //底部温度范围
	0x03,0x58,0x85,          //顶部温度大于指定温度跳转
	0x0E,0x03,0x03,0x62,0x01,          //特别指令
	0x05,0x0A,0x00,0x85,          //烹饪时间大于指定分钟跳转
	0xFF,          //状态结束
	//------------====== 状态 5 ======-------------
	0x00,0x82,0x87,0xF1,0xF1,0x00,          //底部温度范围
	0x01,0x74,0x87,          //底部温度大于指定温度跳转
	0x05,0x08,0x00,0x86,          //烹饪时间大于指定分钟跳转
	0x0E,0x03,0x00,0x01,0x00,          //特别指令
	0xFF,          //状态结束
	//------------====== 状态 6 ======-------------
	0x00,0x82,0x87,0xF1,0xF1,0x00,          //底部温度范围
	0x01,0x74,0x87,          //底部温度大于指定温度跳转
	0x05,0x08,0x00,0x87,          //烹饪时间大于指定分钟跳转
	0x0E,0x03,0x00,0x02,0x00,          //特别指令
	0xFF,          //状态结束
	//------------====== 状态 7 ======-------------
	0x00,0x67,0x69,0x06,0x0F,0x00,          //底部温度范围
	0x05,0xF2,0x00,0x88,          //烹饪时间大于指定分钟跳转
	0xFF,          //状态结束
	//------------====== 状态 8 ======-------------
	0x00,0x5A,0x5C,0x03,0x19,0x00,          //底部温度范围
	0x05,0x01,0x00,0x89,          //烹饪时间大于指定分钟跳转
	0xFF,          //状态结束
	//------------====== 状态 9 ======-------------
	0x00,0x1E,0x28,0x02,0x1C,0x00,          //底部温度范围
	0x05,0x01,0x00,0x8A,          //烹饪时间大于指定分钟跳转
	0xFF,          //状态结束
	//------------====== 状态 10 ======-------------
	0x0E,0x01,0x00,0x00,0x00,          //特别指令
	0xFF,          //状态结束
};

uint8	code	m_u8aPorridge_90[] =
{
    0x05,0x18,0x2B,0x36,0x40,
	//------------====== 状态 0 ======-------------
	0x00,0x78,0x82,0xF0,0x05,0x00,          //底部温度范围
	0x03,0x32,0x01,          //顶部温度大于指定温度跳转
	0x0E,0x03,0x02,0x32,0x14,          //特别指令
	0x05,0x1E,0x00,0x01,          //烹饪时间大于指定分钟跳转
	0xFF,          //状态结束
	//------------====== 状态 1 ======-------------
	0x00,0x78,0x82,0xF1,0xF1,0x00,          //底部温度范围
	0x03,0x50,0x82,          //顶部温度大于指定温度跳转
	0x0E,0x03,0x03,0x50,0x02,          //特别指令
	0x05,0x28,0x00,0x82,          //烹饪时间大于指定分钟跳转
	0xFF,          //状态结束
	//------------====== 状态 2 ======-------------
	0x00,0x28,0x2D,0x05,0x37,0x00,          //底部温度范围
	0x05,0x01,0x00,0x83,          //烹饪时间大于指定分钟跳转
	0xFF,          //状态结束
	//------------====== 状态 3 ======-------------
	0x00,0x54,0x54,0x04,0x1E,0x00,          //底部温度范围
	0x03,0x58,0x84,          //顶部温度大于指定温度跳转
	0xFF,          //状态结束
	//------------====== 状态 4 ======-------------
	0x00,0x50,0x50,0x04,0x32,0x00,          //底部温度范围
	0x04,0x52,0x03,          //顶部温度小于指定温度跳转
	0xFF,          //状态结束
};

uint8	code	m_u8aPorridge_100[] =
{	
    0x06,0x19,0x2C,0x3C,0x4A,0x54,
	//------------====== 状态 0 ======-------------
	0x00,0x78,0x82,0xF0,0x05,0x00,          //底部温度范围
	0x03,0x32,0x01,          //顶部温度大于指定温度跳转
	0x0E,0x03,0x02,0x32,0x14,          //特别指令
	0x05,0x1E,0x00,0x01,          //烹饪时间大于指定分钟跳转
	0xFF,          //状态结束
	//------------====== 状态 1 ======-------------
	0x00,0x78,0x82,0xF1,0xF1,0x00,          //底部温度范围
	0x03,0x50,0x82,          //顶部温度大于指定温度跳转
	0x0E,0x03,0x03,0x50,0x02,          //特别指令
	0x05,0x28,0x00,0x82,          //烹饪时间大于指定分钟跳转
	0xFF,          //状态结束
	//------------====== 状态 2 ======-------------
	0x00,0x28,0x2D,0x05,0x37,0x00,          //底部温度范围
	0x05,0x01,0x00,0x83,          //烹饪时间大于指定分钟跳转
	0x0E,0x03,0x00,0x00,0x00,          //特别指令
	0xFF,          //状态结束
	//------------====== 状态 3 ======-------------
	0x00,0x6E,0x78,0xF2,0xF2,0x00,          //底部温度范围
	0x03,0x5A,0x84,          //顶部温度大于指定温度跳转
	0x05,0x0F,0x00,0x84,          //烹饪时间大于指定分钟跳转
	0xFF,          //状态结束
	//------------====== 状态 4 ======-------------
	0x00,0x6E,0x78,0xF2,0xF2,0x00,          //底部温度范围
	0x03,0x5A,0x05,          //顶部温度大于指定温度跳转
	0xFF,          //状态结束
	//------------====== 状态 5 ======-------------
	0x00,0x6E,0x78,0xF2,0xF3,0x00,          //底部温度范围
	0x04,0x50,0x04,          //顶部温度小于指定温度跳转
	0x0E,0x03,0x01,0x00,0x00,          //特别指令
	0xFF,          //状态结束
};

uint8	code	m_u8aSteam[] =	
{
    0x04,0x11,0x1E,0x2B,
	//------------====== 状态 0 ======-------------
	0x00,0x82,0x8C,0xF0,0x05,0x00,          //底部温度范围
	0x03,0x3C,0x01,          //顶部温度大于指定温度跳转
	0x01,0x8C,0x03,          //底部温度大于指定温度跳转
	0xFF,          //状态结束
	//------------====== 状态 1 ======-------------
	0x00,0x82,0x8C,0x0C,0x05,0x00,          //底部温度范围
	0x03,0x50,0x02,          //顶部温度大于指定温度跳转
	0x01,0x8C,0x03,          //底部温度大于指定温度跳转
	0xFF,          //状态结束
	//------------====== 状态 2 ======-------------
	0x00,0x82,0x8C,0x0A,0x0A,0x00,          //底部温度范围
	0x04,0x4E,0x01,          //顶部温度小于指定温度跳转
	0x01,0x8C,0x03,          //底部温度大于指定温度跳转
	0xFF,          //状态结束
	//------------====== 状态 3 ======-------------
	0x0E,0x01,0x00,0x00,0x00,          //特别指令
	0xFF,          //状态结束
};

uint8	code	m_u8aSoup_90[] =
{
    0x03,0x0D,0x17,
	//------------====== 状态 0 ======-------------
	0x00,0x78,0x82,0xF0,0x05,0x00,          //底部温度范围
	0x03,0x37,0x01,          //顶部温度大于指定温度跳转
	0xFF,          //状态结束
	//------------====== 状态 1 ======-------------
	0x00,0x64,0x69,0x0A,0x0A,0x00,          //底部温度范围
	0x03,0x46,0x02,          //顶部温度大于指定温度跳转
	0xFF,          //状态结束
	//------------====== 状态 2 ======-------------
	0x00,0x58,0x58,0x06,0x18,0x00,          //底部温度范围
	0x04,0x44,0x01,          //顶部温度小于指定温度跳转
	0xFF,          //状态结束
};

uint8	code	m_u8aSoup_100[] =	
{
    0x03,0x0D,0x17,
	//------------====== 状态 0 ======-------------
	0x00,0x78,0x82,0xF0,0x05,0x00,          //底部温度范围
	0x03,0x37,0x01,          //顶部温度大于指定温度跳转
	0xFF,          //状态结束
	//------------====== 状态 1 ======-------------
	0x00,0x78,0x82,0x0A,0x0A,0x00,          //底部温度范围
	0x03,0x50,0x02,          //顶部温度大于指定温度跳转
	0xFF,          //状态结束
	//------------====== 状态 2 ======-------------
	0x00,0x62,0x64,0x07,0x18,0x00,          //底部温度范围
	0x04,0x4E,0x01,          //顶部温度小于指定温度跳转
	0xFF,          //状态结束
};

uint8   code    m_u8aJam[] = 
{
    0x03,0x10,0x1A,
	//------------====== 状态 0 ======-------------
	0x00,0x82,0x8C,0x14,0x05,0x00,          //底部温度范围
	0x03,0x32,0x01,          //顶部温度大于指定温度跳转
	0x01,0x50,0x01,          //底部温度大于指定温度跳转
	0xFF,          //状态结束
	//------------====== 状态 1 ======-------------
	0x00,0x6E,0x73,0x08,0x0A,0x00,          //底部温度范围
	0x03,0x46,0x02,          //顶部温度大于指定温度跳转
	0xFF,          //状态结束
	//------------====== 状态 2 ======-------------
	0x00,0x64,0x69,0x05,0x19,0x00,          //底部温度范围
	0x04,0x3C,0x01,          //顶部温度小于指定温度跳转
	0xFF,          //状态结束
};
   
uint8	code	m_u8aYoghurt[] = 
{
	0x02,0x0C,
	//------------====== 状态 0 ======-------------
	0x00,0x28,0x28,0xF0,0x05,0x00,          //底部温度范围
	0x01,0x20,0x01,          //底部温度大于指定温度跳转
	0xFF,          //状态结束
	//------------====== 状态 1 ======-------------
	0x00,0x28,0x28,0x05,0x19,0x00,          //底部温度范围
	0x02,0x1E,0x00,          //底部温度小于指定温度跳转
	0xFF,          //状态结束
};

uint8   code    m_u8aDough[] =
{
    0x03,0x0D,0x18,
	//------------====== 状态 0 ======-------------
	0x00,0x26,0x26,0x14,0x05,0x00,          //底部温度范围
	0x01,0x20,0x81,          //底部温度大于指定温度跳转
	0xFF,          //状态结束
	//------------====== 状态 1 ======-------------
	0x00,0x26,0x26,0x05,0x19,0x00,          //底部温度范围
	0x05,0x05,0x00,0x82,          //烹饪时间大于指定分钟跳转
	0xFF,          //状态结束
	//------------====== 状态 2 ======-------------
	0x00,0x26,0x26,0x05,0x19,0x00,          //底部温度范围
	0x02,0x1E,0x00,          //底部温度小于指定温度跳转
	0xFF,          //状态结束
};

uint8	code	m_u8aPasta[] = 
{
    0x04,0x0E,0x1A,0x24,
	//------------====== 状态 0 ======-------------
	0x00,0x82,0x8C,0xF0,0x05,0x00,          //底部温度范围
	0x03,0x50,0x01,          //顶部温度大于指定温度跳转
	0xFF,          //状态结束
	//------------====== 状态 1 ======-------------
	0x00,0x6E,0x73,0x0A,0x0A,0x00,          //底部温度范围
	0x0E,0x04,0x00,0x00,0x01,          //特别指令
	0xFF,          //状态结束
	//------------====== 状态 2 ======-------------
	0x00,0x6E,0x73,0x0A,0x0A,0x00,          //底部温度范围
	0x03,0x50,0x03,          //顶部温度大于指定温度跳转
	0xFF,          //状态结束
	//------------====== 状态 3 ======-------------
	0x00,0x5F,0x60,0x06,0x14,0x00,          //底部温度范围
	0xFF,          //状态结束
};

uint8   code    m_u8aCheese[] =
{
    0x03,0x10,0x1A,
	//------------====== 状态 0 ======-------------
	0x00,0x64,0x69,0xF0,0x05,0x00,          //底部温度范围
	0x01,0x46,0x01,          //底部温度大于指定温度跳转
	0x03,0x32,0x02,          //顶部温度大于指定温度跳转
	0xFF,          //状态结束
	//------------====== 状态 1 ======-------------
	0x00,0x64,0x69,0x06,0x14,0x00,          //底部温度范围
	0x03,0x32,0x02,          //顶部温度大于指定温度跳转
	0xFF,          //状态结束
	//------------====== 状态 2 ======-------------
	0x00,0x4E,0x4E,0x05,0x19,0x00,          //底部温度范围
	0xFF,          //状态结束
};

uint8   code    m_u8aReheat_80[] =
{
    0x03,0x10,0x1A,
	//------------====== 状态 0 ======-------------
	0x00,0x78,0x7D,0xF0,0x05,0x00,          //底部温度范围
	0x01,0x50,0x01,          //底部温度大于指定温度跳转
	0x03,0x32,0x02,          //顶部温度大于指定温度跳转
	0xFF,          //状态结束
	//------------====== 状态 1 ======-------------
	0x00,0x69,0x6E,0x07,0x0F,0x00,          //底部温度范围
	0x03,0x32,0x02,          //顶部温度大于指定温度跳转
	0xFF,          //状态结束
	//------------====== 状态 2 ======-------------
	0x00,0x4C,0x4C,0x05,0x14,0x00,          //底部温度范围
	0xFF,          //状态结束
};

uint8	code	m_u8aReheat_100[] = 
{
    0x03,0x10,0x1A,
	//------------====== 状态 0 ======-------------
	0x00,0x78,0x7D,0x14,0x05,0x00,          //底部温度范围
	0x01,0x5A,0x01,          //底部温度大于指定温度跳转
	0x03,0x46,0x02,          //顶部温度大于指定温度跳转
	0xFF,          //状态结束
	//------------====== 状态 1 ======-------------
	0x00,0x6E,0x70,0x06,0x0F,0x00,          //底部温度范围
	0x03,0x50,0x02,          //顶部温度大于指定温度跳转
	0xFF,          //状态结束
	//------------====== 状态 2 ======-------------
	0x00,0x62,0x64,0x06,0x14,0x00,          //底部温度范围
	0xFF,          //状态结束
};

uint8   code    m_u8aBoilingWater[] =
{
    0x03,0x0D,0x1B,
	//------------====== 状态 0 ======-------------
	0x00,0x70,0x76,0xF0,0x05,0x00,          //底部温度范围
	0x01,0x32,0x81,          //底部温度大于指定温度跳转
	0xFF,          //状态结束
	//------------====== 状态 1 ======-------------
	0x00,0x70,0x76,0x05,0x02,0x00,          //底部温度范围
	0x03,0x41,0x02,          //顶部温度大于指定温度跳转
	0x05,0x03,0x00,0x02,          //烹饪时间大于指定分钟跳转
	0xFF,          //状态结束
	//------------====== 状态 2 ======-------------
	0x0E,0x01,0x00,0x00,0x00,          //特别指令
	0xFF,          //状态结束
};

/*-----------------------------*
 *  static variable declaration*
 *----------------------------*/



#ifdef	DEBUG0
/*-----------------------------------------------------------------------------*
 *  Function Description:                                                      *
 *      查询当前状态位置                                   				       *
 *  Parameters:                                                                *
 *      None                                                                   *
 *  Return                                                                     *
 *      m_u8ScriptStage                                                        *
 *----------------------------------------------------------------------------*/
uint8	GetScriptStage(void)
{
	return	m_u8ScriptStage;
}
#endif

/*-----------------------------------------------------------------------------*
 *  Function Description:                                                      *
 *      复位当前Script参数                                   				   *
 *  Parameters:                                                                *
 *      None                                                                   *
 *  Return                                                                     *
 *      None                                                                   *
 *----------------------------------------------------------------------------*/
void	ResetCurScript(void)
{  
     m_u8ScriptStage	=	0;
     m_u8pScript		=	NULL;
     bCookBySctEN		=	0;			//禁止Script运行
	 bBoilChk_EN		=	0;	   		//取消沸腾判断
	 bBoilReach			=	0;
     bPowerAdjSt        =   0;          //禁止功率调节
//	 u8BoilChkCnt		=	0;
//	 m_u8BoilChkTime	=	0;
//	 m_u8BoilDegree		=	0;
     m_u16CookTime		=	0;
	 m_u16IntCookTime	=	0;
	 m_u16MashTime		=	0;
	 g_u16RelayTime		=	0;
	 m_u8AddOnParam		=	0;			//临时参数清零
	 m_u8AddOffParam	=	0;
	 bTempAch1st        =   0;
     bTimeCountDown     =   0;
	 m_u8ChkCnt         =   0;
}

/*-----------------------------------------------------------------------------*
 *  Function Description:                                                      *
 *      设置功能地址                                   				           *
 *  Parameters:                                                                *
 *      None                                                                   *
 *  Return                                                                     *
 *      None                                                                   *
 *----------------------------------------------------------------------------*/
void	SetCurScript(uint8 u8ID,uint8 u8Diy,uint8 u8Temp)
{
    if((u8Diy>0)&&(u8ID==0))
    {
        u8ID = FUNC_ID_DIY;    
    }
    
	switch(u8ID)
	{
		case	FUNC_ID_BAKE:
		case	FUNC_ID_FRY:
        case	FUNC_ID_STEW:
        case    FUNC_ID_DIY:
			if(u8Temp==40)  m_u8pBaseScript = (uint8 code *)m_u8aTemp_40;
			if(u8Temp==50)  m_u8pBaseScript = (uint8 code *)m_u8aTemp_50;
			if(u8Temp==60)  m_u8pBaseScript = (uint8 code *)m_u8aTemp_60;
			if(u8Temp==70)  m_u8pBaseScript = (uint8 code *)m_u8aTemp_70;
			if(u8Temp==80)  m_u8pBaseScript = (uint8 code *)m_u8aTemp_80;
			if(u8Temp==90)  m_u8pBaseScript = (uint8 code *)m_u8aTemp_90;
			if(u8Temp==100) m_u8pBaseScript = (uint8 code *)m_u8aTemp_100;
			if(u8Temp==110) m_u8pBaseScript = (uint8 code *)m_u8aTemp_110;	
			if(u8Temp==120) m_u8pBaseScript = (uint8 code *)m_u8aTemp_120;	
			if(u8Temp==130) m_u8pBaseScript = (uint8 code *)m_u8aTemp_130;	
		    if(u8Temp==140) m_u8pBaseScript = (uint8 code *)m_u8aTemp_140;	
			if(u8Temp==150) m_u8pBaseScript = (uint8 code *)m_u8aTemp_150;
		    if(u8Temp==160) m_u8pBaseScript = (uint8 code *)m_u8aTemp_160;
		    break;
        case	FUNC_ID_DOUGH:
            m_u8pBaseScript = (uint8 code *)m_u8aDough;    
            break;
		case	FUNC_ID_SOUP:
            if(u8Temp==90)  m_u8pBaseScript = (uint8 code *)m_u8aSoup_90;
			if(u8Temp==100) m_u8pBaseScript = (uint8 code *)m_u8aSoup_100;
            if(u8Temp==110) m_u8pBaseScript = (uint8 code *)m_u8aTemp_110;	
			break;
		case	FUNC_ID_STEAM:
			m_u8pBaseScript = (uint8 code *)m_u8aSteam;
			break;
		case	FUNC_ID_RICE:
			m_u8pBaseScript = (uint8 code *)m_u8aRice;
			break;
		case	FUNC_ID_PILAF:
			m_u8pBaseScript = (uint8 code *)m_u8aPilaf;
			break;
        case    FUNC_ID_OPORRIDGE:
            if(u8Temp==90)  m_u8pBaseScript = (uint8 code *)m_u8aPorridge_90;
			if(u8Temp==100) m_u8pBaseScript = (uint8 code *)m_u8aPorridge_100;
			break;
        case    FUNC_ID_JAM:
            m_u8pBaseScript = (uint8 code *)m_u8aJam;
            break;
		case	FUNC_ID_YOGHURT:
		    m_u8pBaseScript = (uint8 code *)m_u8aYoghurt;
		    break;
        case    FUNC_ID_CHEESE:
            m_u8pBaseScript = (uint8 code *)m_u8aCheese;
            break;
		case	FUNC_ID_PASTA:
		    m_u8pBaseScript = (uint8 code *)m_u8aPasta;
			break;
		case	FUNC_ID_REHEAT:
            if(u8BolingWaterTest==0)
            {
                if(u8Temp==80)  m_u8pBaseScript = (uint8 code *)m_u8aReheat_80;  
                if(u8Temp==100) m_u8pBaseScript = (uint8 code *)m_u8aReheat_100;
            }
            else
            {
                m_u8pBaseScript = (uint8 code *)m_u8aBoilingWater;
            }
			break;
		default:
			break;
	}	
	ResetCurScript();	
}

/*-----------------------------------------------------------------------------*
 *  Function Description:                                                      *
 *      烹饪处理进程                                  				           *
 *  Parameters:                                                                *
 *      None                                                                   *
 *  Return                                                                     *
 *      None                                                                   *
 *----------------------------------------------------------------------------*/
void 	ScriptProc(void)
{
	uint8	u8Tmp;

	if(g_u8ErrorType)	return;				//如果系统处于报警状态，不执行Script

	if(bCookBySctEN)						//如果Script有效，执行Script
	{
		if(m_u8pScript==NULL)
		{
			m_u8pScript	=	m_u8pBaseScript+m_u8pBaseScript[m_u8ScriptStage];
		}

		if(bSys_1000MS)	m_u16CookTime++;

		while(m_u8pScript!=NULL)
		{
			switch(m_u8pScript[0])
			{
				//=========================================
				//底部温度指令,格式:00,LoTmp,HiTmp,OnTime,OffTime,bBoilChk_EN
				case 	0x00:
						SetBotTemp(m_u8pScript);	//设置底部温度
						m_u8pScript	+=	6;
						break;

			 	//=========================================
				//温度跳转指令,格式:1/2/3/4,Temp,JmpStage
				case	0x01:
				case	0x02:
				case	0x03:
				case	0x04:
						u8Tmp	=	ByTemp_ChgStage(m_u8pScript);
						m_u8pScript	+=	3;
						ChangeScriptStage(u8Tmp);
						break;

		   		//=========================================
				//时间跳转指令,格式：5/6/7/8/9,TimeLo,TimeHi,JmpStage
				case	0x05:
				case	0x06:
				case	0x07:
				case	0x08:
				case	0x09:
						u8Tmp	=	ByTime_ChgStage(m_u8pScript);
						m_u8pScript	+=	4;
						ChangeScriptStage(u8Tmp);
						break;

				//=========================================
				//加热功率参数,格式：0x0a/0x0b,PowerLevel
				//case	0x0a:					//最大加热功率
				//case	0x0b:					//最小加热功率
				//		SetPowerLevel(m_u8pScript);
				//		m_u8pScript	+=	2;
				//		break;

				//=========================================
				//顶部和侧面加热指令,格式：0x0c/0x0d,u8JmpMode,Temp
				case    0x0c:                   //顶部加热控制
				case    0x0d:                   //侧部加热控制			
						Heat_Ctr(m_u8pScript);
						m_u8pScript	+=	3;
						break;

				//=========================================
				//特别指令，目前包括进入保温
				case	0x0e:
						u8Tmp = OtherCmds(m_u8pScript);
						if(m_u8pScript==NULL)	break;	  //保温直接跳出
						else	m_u8pScript	+= 5;
						ChangeScriptStage(u8Tmp);
						break;

				case	0xff:
						m_u8pScript	= NULL;
						break;

			 	default:
						EnterReady();	   		//脚本出错进待机
						break;
			}
		}
	}
}

/*-----------------------------------------------------------------------------*
 *  Function Description:                                                      *
 *      当前状态转换                                   				           *
 *  Parameters:                                                                *
 *      u8Stage需要跳转的状态                                                  *
 *  Return                                                                     *
 *      None                                                                   *
 *----------------------------------------------------------------------------*/
void	ChangeScriptStage(uint8	u8Stage)
{
	if(u8Stage==0xff)	return;
	m_u8ScriptStage	=	u8Stage&0x7f;
	if(u8Stage&0x80)	m_u16CookTime	=	0;
}

/*-----------------------------------------------------------------------------*
 *  Function Description:                                                      *
 *      设置底部温度范围                                   				       *
 *  Parameters:                                                                *
 *      pScript，脚本数据                                                      *
 *  Return                                                                     *
 *      None                                                                   *
 *----------------------------------------------------------------------------*/
void	SetBotTemp(uint8 code *pScript)
{
	uint8 u8BotDegree = ADCGetData(AD_CH_BOT);

	g_u8LoTemp	=	pScript[1];
	g_u8HiTemp	=	pScript[2];
	g_u8OnTime_Set	=	pScript[3];
	g_u8OffTime_Set	=	pScript[4];
	
	if(pScript[5]==0x0A)	bBoilChk_EN	=	1;	//使能沸腾检测
	else	bBoilChk_EN	=	0;					//禁止沸腾检测
			
	/*-----------------可调加热功率设置--------------------*/
	if(g_u8OnTime_Set>0xf0)
	{
		if(g_u8OnTime_Set==0xf1)		//Rice&&Porridge
		{
			g_u8OnTime_Set = m_u8AddOnParam;  	
		}	
		
		if(g_u8OnTime_Set==0xf2)		//Porridge
		{
			g_u8OnTime_Set = m_u8AddOnParam/2;	
			if(g_u8OnTime_Set<4) g_u8OnTime_Set = 4;   //煮粥维持阶段最小功率限制
			if(g_u8OnTime_Set>6) g_u8OnTime_Set = 6;   //煮粥维持阶段最大功率限制
		}	
		
		if(g_u8OnTime_Set==0xf3)		//Fry
		{
			if(u8CookTemp==160)
			{
				if(u8BotDegree<123)
				{
					g_u8OnTime_Set = (133-u8BotDegree)/2;
					if(g_u8OnTime_Set>10) g_u8OnTime_Set = 10; 
				}
				else
				{
					g_u8OnTime_Set = 0;
				}
			}
			
			if(u8CookTemp==150)
			{
				if(u8BotDegree<130)
				{
					g_u8OnTime_Set = (140-u8BotDegree)/2;
					if(g_u8OnTime_Set>10) g_u8OnTime_Set = 10; 
				}
				else
				{
					g_u8OnTime_Set = 0;
				}
			}
			
			if(u8CookTemp==140)
			{
				if(u8BotDegree<124)
				{
					g_u8OnTime_Set = (134-u8BotDegree)/2;
					if(g_u8OnTime_Set>10) g_u8OnTime_Set = 10; 
				}
				else
				{
					g_u8OnTime_Set = 0;
				}
			}
			
			if(u8CookTemp==130)
			{
				if(u8BotDegree<116)
				{
					g_u8OnTime_Set = (126-u8BotDegree)/2;
					if(g_u8OnTime_Set>10) g_u8OnTime_Set = 10; 
				}
				else
				{
					g_u8OnTime_Set = 0;
				}
			}
			
			if(u8CookTemp==120)
			{
				if(u8BotDegree<108)
				{
					g_u8OnTime_Set = (118-u8BotDegree)/2;
					if(g_u8OnTime_Set>10) g_u8OnTime_Set = 10; 
				}
				else
				{
					g_u8OnTime_Set = 0;
				}
			}
			
			if(u8CookTemp==110)
			{
				if(u8BotDegree<100)
				{
					g_u8OnTime_Set = (110-u8BotDegree)/2;
					if(g_u8OnTime_Set>10) g_u8OnTime_Set = 10; 
				}
				else
				{
					g_u8OnTime_Set = 0;
				}
			}
			
			if(u8CookTemp==100)
			{
				if(u8BotDegree<92)
				{
					g_u8OnTime_Set = (102-u8BotDegree)/2;
					if(g_u8OnTime_Set>10) g_u8OnTime_Set = 10; 
				}
				else
				{
					g_u8OnTime_Set = 0;
				}
			}		
		}
	}

	if(g_u8OffTime_Set>0xf0)
	{
		if(g_u8OffTime_Set==0xf1)		//Rice&&Porridge
		{
			g_u8OffTime_Set	=	m_u8AddOffParam;
			if(g_u8OffTime_Set==0)
			{
				g_u8OnTime_Set = 0xf0;	//连续加热
			}
		}
		
		if(g_u8OffTime_Set==0xf2)		//Porridge(80->90)
		{			
			g_u8OffTime_Set	= 20;	
		}
		
		if(g_u8OffTime_Set==0xf3)		//Porridge(90->100)
		{	
			if(m_u8AddOnParam>=14)
			{
				g_u8OffTime_Set	= 35+m_u8AddOffParam;
			}
			else
			{
				g_u8OffTime_Set = 28+m_u8AddOffParam;
			}
		}
		
		if(g_u8OffTime_Set==0xf4)		//Fry
		{
			g_u8OffTime_Set = 30 - g_u8OnTime_Set;
		}
	}
}

/*-----------------------------------------------------------------------------*
 *  Function Description:                                                      *
 *      温度跳转处理                                   				           *
 *  Parameters:                                                                *
 *      pScript，脚本数据                                                      *
 *  Return                                                                     *
 *      跳转的状态,0xff不跳转,其他值跳转                                       *
 *----------------------------------------------------------------------------*/
uint8	ByTemp_ChgStage(uint8 code *pScript)
{
	uint8   i	=	pScript[1];					//跳转温度值
	uint8	u8ChReq	=	0;		                //跳转标志位
	uint8	u8BotDegree	=	ADCGetData(AD_CH_BOT);
	uint8	u8TopDegree	=	ADCGetData(AD_CH_TOP);

	switch(pScript[0])
	{
		case	1:	
				if(u8BotDegree > i)
				{
					u8ChReq	=	1;
			  	}
				break;
	 	case	2:
				if(u8BotDegree < i)
				{
					u8ChReq	=	1;
				}
				break;
  		case	3:
				if(u8TopDegree > i)
				{
					u8ChReq	=	1;
				}
//				if(bBoilChk_EN&&bBoilReach)	//沸腾判断&&已沸腾
//				{
//					u8ChReq	=	1;
//				}
				break;
   		case	4:
				if(u8TopDegree < i)
				{
					u8ChReq	=	1;
				}
				break;
		default:
				break;
	}

	if(u8ChReq)
	{
		return	pScript[2];
	}
	return	0xff;
}

/*-----------------------------------------------------------------------------*
 *  Function Description:                                                      *
 *      时间跳转处理                                   				           *
 *  Parameters:                                                                *
 *      pScript，脚本数据                                                      *
 *  Return                                                                     *
 *      跳转的状态,0xff不跳转,其他值跳转                                       *
 *----------------------------------------------------------------------------*/
uint8	ByTime_ChgStage(uint8 code	*pScript)
{
	uint16	u16Time	=	pScript[1]+pScript[2]*256;		//指定的时间条件(分钟)

	if(pScript[0]==9)									//烹饪时间清零
	{
		m_u16CookTime	=	0;
		return	0xff;
 	}

	//========================================================
	//5和6为分钟跳转,5为大于指定分钟跳转，6为小于指定分钟跳转
	//7和8为秒的跳转,7为大于指定秒跳转，8为小于指定秒跳转

	if(pScript[0]==5||pScript[0]==6)
	{
		if(u16Time > 0xf0)
		{					
			if(u16Time==0xf2)							//糊化时间
			{					
				u16Time = 1080;
				if(u16Time>m_u16MashTime)
				{
					u16Time -= m_u16MashTime;
				}
				else
				{
					u16Time = 0;
				}
			}
		}
		else
		{
		    u16Time	*=	60;
		}
	}

	if(pScript[0]==5||pScript[0]==7)
	{
		if(m_u16CookTime > u16Time)
		{
			return	pScript[3];
		}
	}
	else
	{
		if(m_u16CookTime < u16Time)
		{
			return	pScript[3];
		}
	}
	return	0xff;
}

/*-----------------------------------------------------------------------------*
 *  Function Description:                                                      *
 *      顶侧加热处理                                   				           *
 *  Parameters:                                                                *
 *      pScript,脚本数据                                                       *
 *  Return                                                                     *
 *      None                                                                   *
 *----------------------------------------------------------------------------*/
void	Heat_Ctr(uint8 code	*pScript)
{
	uint8	u8ChReq	=	0;							//开断标志位
	uint8	u8BotDegree	=	ADCGetData(AD_CH_BOT);
	uint8	u8TopDegree	=	ADCGetData(AD_CH_TOP);

	if(pScript[2]!=0)								//Temp
	{
		switch(pScript[1])
		{
			case	1:
					if(u8TopDegree < pScript[2])
					{
						u8ChReq	=	1;				//开
					}
					break;
			case	2:
					if(u8TopDegree > pScript[2])
					{
						u8ChReq	=	2;        		//断
				 	}
					break;
			case	5:
					if(u8BotDegree < pScript[2])
					{
						u8ChReq	=	1;				//开
				  	}
					break;
	   		case	6:
					if(u8BotDegree > pScript[2])
					{
						u8ChReq	=	2;        		//断
				 	}
					break;
			default:
					u8ChReq	=	0;
					break;
	   	}
	}
	else
	{
		//直接开或断
		if(pScript[1]==1)
		{
			u8ChReq	=	1;							//开
		}
		else
		{
			u8ChReq	=	2;        					//断
		}
	}

	if(u8ChReq==1)
	{
		if(pScript[0]==0x0c)
		{
			bHeatTopReq	=	1;						//打开顶部加热
	  	}
		else	//pScript[0]==0x0d
		{
//			bHeatArdReq	=	1;						//打开侧部加热
	 	}
	}
	
	if(u8ChReq==2)
	{
		if(pScript[0]==0x0c)
		{
			bHeatTopReq	=	0;						//关闭顶部加热
	 	}
		else	//pScript[0]==0x0d
		{
//			bHeatArdReq	=	0;						//关闭侧部加热
	 	}
	}
}
/*-----------------------------------------------------------------------------*
 *  Function Description:                                                      *
 *      特殊跳转处理                                   				           *
 *  Parameters:                                                                *
 *      pScript，脚本数据                                                      *
 *  Return                                                                     *
 *      None                                                                   *
 *----------------------------------------------------------------------------*/
uint8	OtherCmds(uint8	code *pScript)
{
    uint8 u8TopDegree = ADCGetData(AD_CH_TOP);
	uint8 u8ChReq = 0;    //跳转标志位
	uint8 u8ExtStage = 0xff;
	
	switch(pScript[1])
	{
		case	1:									//结束当前状态，进入保温模式
			    if(bWarm_EN)
				{
					EnterWarm();
				}
				else
				{
					EnterReady();
				}
				bWarm_EN = 1;
				PlaySound(MusicCookEnd);		
				break;
	  	case	2:									//烹饪时间倒计时
				break;
		case	3:		
		        //--------------------------------------------------
				//加热时间计算
				if(pScript[2]==0)	
				{
					//---------------煮粥维持沸腾火力---------------
					if(pScript[3]==0)
					{
						if(g_u16RelayTime)
						{
							m_u8AddOnParam	=	g_u16RelayTime/60;
						}
						else
						{
							m_u8AddOnParam	=	8;		//热锅维持功率
						}
						m_u8AddOffParam		=	0;	
					}
					
					if(bSys_1000MS)	
					{
						m_u16MashTime++;
						m_u16IntCookTime++;	
					}
					
					//----------------煮饭糊化小火力------------------
					if(pScript[3]==1)
					{
						m_u8AddOnParam = g_u16RelayTime/60;
						if(m_u8AddOnParam<8)	m_u8AddOnParam = 8;
						if(m_u8AddOnParam>16)	m_u8AddOnParam = 16;
                        m_u8AddOnParam = m_u8AddOnParam/2;
						m_u8AddOffParam = 20 - m_u8AddOnParam;	
					}
					
					//----------------煮饭糊化大火力------------------
					if(pScript[3]==2)
					{
						m_u8AddOnParam	=	g_u16RelayTime/60;
						if(m_u8AddOnParam<14)	m_u8AddOnParam = 14;
						if(m_u8AddOnParam>20)	m_u8AddOnParam = 20;
                        m_u8AddOnParam = m_u8AddOnParam/2;
						m_u8AddOffParam = 20 - m_u8AddOnParam;	
					}
				}
				
                //--------------------------------------------------
				//功率递减(Porridge)
				if(pScript[2]==1)					
				{			
					if(bSys_1000MS)	m_u16IntCookTime++;
				
				    if(m_u16IntCookTime>=600)		//10min
					{
						m_u16IntCookTime = 0;
						m_u8AddOffParam	+= 2;
					}
//					if(m_u8AddOnParam<18)	m_u8AddOnParam	= 18;
//					if(m_u8AddOnParam>30)	m_u8AddOnParam	= 30;
					if(m_u8AddOffParam>10)	m_u8AddOffParam	= 10;    			
				}
					
				//--------------------------------------------------
				//设定火力控制初始值
				if(pScript[2]==2) 					
				{
					m_u8EstimateTemp	=	pScript[3];				//预估温度初始值
					m_u8AddOnParam		=	g_u16RelayTime/60+10;	//功率初始值
					if(m_u8AddOnParam>24)	m_u8AddOnParam	=	24;
					if(m_u8AddOnParam<12)	m_u8AddOnParam	=	12;
                    m_u8AddOnParam = m_u8AddOnParam/2;
					m_u8AddOffParam	= 20-m_u8AddOnParam;		
				}

                //--------------------------------------------------
				//根据温差调整火力
				if(pScript[2]==3)
				{
					m_u8TargetTemp		=	pScript[3];	
					
					//-----------------Rice-------------------------
			 		if(pScript[4]==1)						
					{
						if(m_u8TargetTemp==85)				//目标温度85度
						{
							if(u8TopDegree>72)	
							{
								m_u8OffsetTemp	=	2;		//上盖温度大于72度,偏移温度2
							}
							else
							{
								m_u8OffsetTemp	=	4;		//上盖温度小于72度,偏移温度4
							}
							m_u8OnTimeDec	=	2;			//加热时间减少量2S
							m_u8OnTimeInc	=	1;			//加热时间增加量1S
							m_u8MinOnTime_Set	=	5;		//最小加热时间5S
							m_u8MaxOnTime_set	=	15;		//最大加热时间15S
							m_u8PwrCycle	=	20;			//调功周期20S							
						}
					
						if(m_u8TargetTemp==98)				//目标温度98度
						{
							m_u8OffsetTemp	=	1;			//偏移温度1
							m_u8OnTimeDec	=	1;          //加热时间减少量1S
							m_u8OnTimeInc	=	1;          //加热时间增加量1S
							m_u8MinOnTime_Set	=	4;      //最小加热时间4S
							m_u8MaxOnTime_set	=	g_u16RelayTime/120+2;     //最大加热时间
							if(m_u8MaxOnTime_set>10)	
							{
								m_u8MaxOnTime_set = 10;
							}		
							m_u8PwrCycle	=	20;			//调功周期20S
						}
					}
					
					//-------------------Porridge-------------------------
					if(pScript[4]==2)						
					{
						if(m_u8TargetTemp==80)				//目标温度80度
						{
							if(u8TopDegree>65)	
							{
								m_u8OffsetTemp = 1;			//上盖温度大于65度,偏移温度1
								m_u8MaxOnTime_set =	5;		//最大加热时间5S
							}
							else
							{
								m_u8OffsetTemp	=	2;		//上盖温度小于65度,偏移温度2
								m_u8MaxOnTime_set	=	8;	//最大加热时间8S
							}
							m_u8OnTimeDec	=	1;			//加热时间减少量1S
							m_u8OnTimeInc	=	1;			//加热时间增加量1S
							m_u8MinOnTime_Set	=	4;		//最小加热时间4S
							m_u8PwrCycle	=	20;			//调功周期20S
						}
					}
					GetPowerByDT();							//根据温差调功
				}
				break;
			case    4:			//外部控制跳转
				if(bTimeCountDown)
				{
					u8ChReq = 1;
					u8ExtStage = 2;	//pasta 按下开始键
				}
				break;	
		default:
				break;
	}
	
	if(u8ChReq)
	{
		return	  u8ExtStage;
	}
	return	0xff;
}

/*-----------------------------------------------------------------------------*
 *  Function Description:                                                      *
 *      根据温差调整功率                                				       *
 *  Parameters:                                                                *
 *      None                                                                   *
 *  Return                                                                     *
 *      None                                                                   *
 *----------------------------------------------------------------------------*/
void	GetPowerByDT(void)
{
	uint8	u8TopDegree	=	ADCGetData(AD_CH_TOP);
	
	if(bSys_1000MS)	m_u16IntCookTime++;
	if(m_u16IntCookTime>=30)		//调功周期30S
	{
		m_u16IntCookTime	=	0;
		m_u8EstimateTemp	+=	m_u8OffsetTemp;

        if(m_u8EstimateTemp>m_u8TargetTemp)
		{
			m_u8EstimateTemp =	m_u8TargetTemp;
		}
		
		//顶部温度大于预期值,加热时间缩短
		if(u8TopDegree>m_u8EstimateTemp)
		{
			m_u8AddOnParam	-=	m_u8OnTimeDec;		
		}
		
		//顶部温度小于预期值,加热时间增加
		if(u8TopDegree<m_u8EstimateTemp)
		{
			m_u8AddOnParam	+=	m_u8OnTimeInc;
		}
		
		//判断加热时间是否正确
		if(m_u8AddOnParam>m_u8MaxOnTime_set)
		{
			m_u8AddOnParam	=	m_u8MaxOnTime_set;
		}
		
		if(m_u8AddOnParam<m_u8MinOnTime_Set)
		{
			m_u8AddOnParam	=	m_u8MinOnTime_Set;
		}
		
		if(m_u8PwrCycle>m_u8AddOnParam)
		{
			m_u8AddOffParam	=	m_u8PwrCycle-m_u8AddOnParam;	
		}
		else
		{
			m_u8AddOffParam	=	0;
		}
		
		//更新预期值	
		m_u8EstimateTemp	=	u8TopDegree;	
	}
}
 
/*-----------------------------------------------------------------------------*
 *  Function Description:                                                      *
 *      沸腾检测                                                               *
 *  Parameters:                                                                *
 *      None                                                                   *
 *  Return                                                                     *
 *      None                                                                   *
 *----------------------------------------------------------------------------*/
//void	BoilCheckProc(void)
//{
//	uint8	u8TopDegree	=	ADCGetData(AD_CH_TOP);
//	uint8	i;

//	if(bBoilChk_EN)
//	{
//		if(bSys_1000MS)	m_u8BoilChkTime++;

//		if(m_u8BoilChkTime>=60/BOIL_DATA_PER_MIN)	 								
//		{
//			m_u8BoilChkTime	=	0;
//	
//			for(i=0;i<BOIL_TEMP_SIZE-1;i++)			//填充缓冲区		
//			{
//				u8BoilTempBuf[i]	=	u8BoilTempBuf[i+1];		
//			}
//			u8BoilTempBuf[BOIL_TEMP_SIZE-1]	=	u8TopDegree;

//			for(i=0;i<BOIL_TEMP_SIZE;i++)  			//备份缓冲区
//			{
//				u8BoilTempBufBAK[i]	=	u8BoilTempBuf[i];
//			}				

//			u8BoilChkCnt++;							

//			if(u8BoilChkCnt== BOIL_TEMP_SIZE)	   	//判断是否填充完成
//			{
//				u8BoilChkCnt = u8BoilChkCnt - 1;

//				Sort8(u8BoilTempBufBAK,BOIL_TEMP_SIZE);	//排序

//				if((u8BoilTempBufBAK[BOIL_TEMP_SIZE-1]-u8BoilTempBufBAK[0])<BOIL_OFFSET_VAL)
//				{			
//					bBoilReach	=	1;
//					m_u8BoilDegree	=	u8TopDegree;	
//				}				
//			}
//		}
//	}	

//	if(bBoilReach)		
//	{
//		if(u8TopDegree>m_u8BoilDegree)
//		{
//			m_u8BoilDegree	=	u8TopDegree;      //更新沸点
//		}
//	}
//}
