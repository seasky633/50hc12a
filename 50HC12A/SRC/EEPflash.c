/*-----------------------------*
 *  include head files         *
 *----------------------------*/
#include ".\Head\HD_cfg.h"

/*-----------------------------*
 *  Macros for constants       *
 *----------------------------*/

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
uint8 	EEPROM_Process(void);
void	Program_Reset(void);
void 	EpromInit(void);
void 	FlashSector_Operate(uint16 p_EEPADDR, uint8 p_CMD);

/*-----------------------------*
 *  public variable declaration*
 *----------------------------*/
uint8 xdata rEepromCache[32];	// 数据缓存
uint8 idata rEEP_FlagA,rEEP_FlagB;
//uint8 idata rDataSavedFlag;




/*-----------------------------------------------------------------------------*
 *  Function Description:                                                      *
 *      缓存区内容写入EEPROM                                                   * 
 *  Parameters:                                                                *
 *      None                                                                   *
 *  Return                                                                     *
 *      写是否成功                                                             *
 *----------------------------------------------------------------------------*/
uint8	EEPROM_Process(void)
{
	uint8 code *p_EepRom;
	uint8 xdata *p_Buffer=0x8000;
	uint8 i;
	
	rEEP_FlagA=0x5A;
	rEEP_FlagB=0xA5;	//防止异常调用
	cli();				//关闭中断
	WDTCR=0;			//关闭看门狗;
	LVRCR=0x01;			//关闭低电压复位,防止中断EEPROM操作
	for(i=0;i<EEPROMPAGESIZE;i++)
	{
		*p_Buffer=rEepromCache[i];
		p_Buffer++;
	}
	FlashSector_Operate(EEP_SECTOR_ADDR,EEPERASE_CMD);	
	FlashSector_Operate(EEP_SECTOR_ADDR,EEPWRITE_CMD);
	LVRCR = 0x00;	
    WDTDR = 0x14;   	// period
	WDTCR = 0xC2;   	// setting
	WDTCR |= 0x20; 
	sei(); 
	rEEP_FlagA=0;
	rEEP_FlagB=0;
	p_EepRom=EEP_SECTOR_ADDR;
	for(i=0;i<EEPROMPAGESIZE;i++)
	{
		if(*p_EepRom!=rEepromCache[i])
			return 0;
		else p_EepRom++;
	}
	return 1;
}

/*-----------------------------------------------------------------------------*
 *  Function Description:                                                      *
 *      程序复位指令,当检测到程序异常可以使用复位                              * 
 *  Parameters:                                                                *
 *      None                                                                   *
 *  Return                                                                     *
 *      None                                                                   *
 *----------------------------------------------------------------------------*/
void Program_Reset(void)
{
	((void (code *) (void)) 0x0000) ();
}

/*-----------------------------------------------------------------------------*
 *  Function Description:                                                      *
 *      正常不调用,EEPROM擦除/写程序时初始化寄存器                             * 
 *  Parameters:                                                                *
 *      None                                                                   *
 *  Return                                                                     *
 *      None                                                                   *
 *----------------------------------------------------------------------------*/
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
	FSADRM =(uint8)(EPROM_DEFAULT_ADR>>8);	
	FSADRL = (uint8)(EPROM_DEFAULT_ADR);	
	rEEP_FlagA=0;
	rEEP_FlagB=0;
	//------------------------------------------------------------------------------------------------------------
}

/*-----------------------------------------------------------------------------*
 *  Function Description:                                                      *
 *      烧写/擦除扇区操作                                                      * 
 *  Parameters:                                                                *
 *      p_EEPADDR，p_CMD                                                       *
 *  Return                                                                     *
 *      None                                                                   *
 *----------------------------------------------------------------------------*/
void FlashSector_Operate(uint16 p_EEPADDR, uint8 p_CMD)
{
	FSADRH=0;	
	FSADRM=(uint8)(p_EEPADDR>>8);
	FSADRL=(uint8)(p_EEPADDR);	
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
}

