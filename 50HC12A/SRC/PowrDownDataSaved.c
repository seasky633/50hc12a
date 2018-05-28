//================================
#define _EEPROM_H
#define Mydef extern
#include "MC96F8316.h"
#include <intrins.h>
//================================
#define ON 1
#define OFF 0
//================================


#define MCUTYPE F8316	// MCU TYPE
//#define MCUTYPE F6432

//#if(MCUTYPE==F8316)
#define EEPROMPAGESIZE 32
//#endif
//#if(MCUTYPE==F6432)
//#define EEPROMPAGESIZE 64
//#endif

//================================
typedef unsigned char Byte;
typedef unsigned int Word;

//================================
#define EEP_STEP_STANBY 0
#define EEP_STEP_ERASE 1
#define EEP_STEP_WRITE 2

#define EEP_ADDRESS_START 0x3FA0	//EEPROM 起始地址
#define EEP_ADDRESS_END 0x3FFF		//EEPROM 结束地址
#define	EEP_SECTOR_ADDR	0x3FC0	

#define EPROM_DEFAULT_ADR EEP_ADDRESS_START	// 此处指定默认不使用Eprom擦除/写时设置的地址指向用户指定的EPROM地址

#define EEPROM_W_MAX 5 //允许写5次，不成功退出
#define	EEPERASE_CMD	0x02
#define	EEPWRITE_CMD	0x03
//================================
unsigned char xdata *ImageDat=0x8000;	//镜像数据头地址，32byte

unsigned char rEepromCache[32];	// 数据缓存
unsigned char  rEEP_FlagA,rEEP_FlagB;
unsigned char rDataSavedFlag,rPowerRecoverCnt,r1msTick;

//================================
 unsigned char EEPROM_Process(void);
 void Program_Reset(void);
 void EpromInit(void);
 void FlashSector_Operate(unsigned int p_EEPADDR, unsigned char  p_CMD );
void main(void)
{  
	rDataSavedFlag=0xAA;//上电将标志置位,防止上电电压稳定前定EEPROM操作
	LVICR=0x1B;//setting LVI detet <4.0V
	EpromInit();
	while(1)
	{
		WDTCR =0xE2;	// clear WDT
		if(LVICR&0x20)//power down to <4.0V
			{
			rEEP_FlagA=0x5A;//置A标志,防止异常调用
			rPowerRecoverCnt=0;			
			if(rDataSavedFlag==0)
				{
				EEPROM_Process();//此处参考之前提供EEPROM操作注意事项
				rDataSavedFlag=0xAA;
				}
			LVICR=0x1B;//clear flag and detect again
			}
		else//
			{
			if(r1msTick==0xFF)
				{
				r1msTick=0;
				if(rPowerRecoverCnt>20)//20 ms debounce ,用户可自行修改合适的时间
					rDataSavedFlag=0;//power recovered morethan 20ms,clear "saved "flag
				else rPowerRecoverCnt++;
				}
			}

	}

}


//================================
//主函数调用
// 示范应用，仅供参考
//================================
unsigned char EEPROM_Process(void)
{
unsigned char code *p_EepRom;
unsigned char xdata *p_Buffer=0x8000;
unsigned char i;
ClosedDisplay();//关闭显示,最大可能省电,保证足够时间完成EEPROM操作
rEEP_FlagB=0xA5;//置B标志,防止异常调用
DI();//关闭中断
WDTCR=0;//关闭看门狗;
LVRCR=0x01;//关闭低电压复位,防止中断EEPROM操作
for(i=0;i<EEPROMPAGESIZE;i++)
	{
	*p_Buffer=rEepromCache[i];
	p_Buffer++;
	}
FlashSector_Operate(EEP_SECTOR_ADDR,EEPERASE_CMD);	
FlashSector_Operate(EEP_SECTOR_ADDR,EEPWRITE_CMD);
p_EepRom=EEP_SECTOR_ADDR;
for(i=0;i<EEPROMPAGESIZE;i++)
{
	if(*p_EepRom!=rEepromCache[i])
		return 0;
	else p_EepRom++;
}
return 1;
}


//---------------------------------------------------
//程序复位指令
// 当检测到程序异常可以使用复位
//---------------------------------------------------
void Program_Reset(void)
 {
	((void (code *) (void)) 0x0000) ();

 }


//---------------------------------------------------
//正常不调用EEPROM 擦除/ 写 程序时初始化寄存器
//---------------------------------------------------
void EpromInit(void)
{
	//------------------------------------------------------------------------------------------------------------
	FIDR=0;		// 只有当FIDR =0xa5 并且FMCR= 02 或者FMCR=03 时条件同时成立才可以擦除或者写EEPROM
				//所以正常不擦除写的情况下FIDR设置为0避免程序跑飞情况下出异常
	//------------------------------------------------------------------------------------------------------------
	//正常不擦除/写 EEPROM 时寄存器地址写入一特定地址，
	// 地址为EEPROM地址或者其它非程序代码占用地址(如果程序空间够，可以使用_at_ 指令额外指定一个地址)
	// 这样做保证异常情况下进入擦/写命令时不会擦除/修改ROM 的程序指令段，以确保程序正常工作
	FSADRH = 0;
	FSADRM =(unsigned char)(EPROM_DEFAULT_ADR>>8);	
	FSADRL = (unsigned char)(EPROM_DEFAULT_ADR);	
	rEEP_FlagA=0;
	rEEP_FlagB=0;
	//------------------------------------------------------------------------------------------------------------
}


void FlashSector_Operate(unsigned int p_EEPADDR, unsigned char  p_CMD )
{
	FSADRH=0;	
	FSADRM=(unsigned char)(p_EEPADDR>>8);
	FSADRL=(unsigned char)(p_EEPADDR);	
	FIDR=0xA5;	
	if(rEEP_FlagA!=0x5A) Program_Reset();
	if(rEEP_FlagB!=0xA5) Program_Reset();
	if(FSADRH!=0) Program_Reset();
	if(FSADRM<0x3F) Program_Reset();
	if(FSADRL<0xA0) Program_Reset();
	IE&=0x7F;
	FMCR=p_CMD;
	_nop_();
	_nop_();
	_nop_();
	_nop_();
	_nop_();
	_nop_();
	_nop_();
	_nop_();
	IE|=0x80;	
	rEEP_FlagA=0;
	rEEP_FlagB=0;
}

