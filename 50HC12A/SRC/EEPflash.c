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
uint8 xdata rEepromCache[32];	// ���ݻ���
uint8 idata rEEP_FlagA,rEEP_FlagB;
//uint8 idata rDataSavedFlag;




/*-----------------------------------------------------------------------------*
 *  Function Description:                                                      *
 *      ����������д��EEPROM                                                   * 
 *  Parameters:                                                                *
 *      None                                                                   *
 *  Return                                                                     *
 *      д�Ƿ�ɹ�                                                             *
 *----------------------------------------------------------------------------*/
uint8	EEPROM_Process(void)
{
	uint8 code *p_EepRom;
	uint8 xdata *p_Buffer=0x8000;
	uint8 i;
	
	rEEP_FlagA=0x5A;
	rEEP_FlagB=0xA5;	//��ֹ�쳣����
	cli();				//�ر��ж�
	WDTCR=0;			//�رտ��Ź�;
	LVRCR=0x01;			//�رյ͵�ѹ��λ,��ֹ�ж�EEPROM����
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
 *      ����λָ��,����⵽�����쳣����ʹ�ø�λ                              * 
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
 *      ����������,EEPROM����/д����ʱ��ʼ���Ĵ���                             * 
 *  Parameters:                                                                *
 *      None                                                                   *
 *  Return                                                                     *
 *      None                                                                   *
 *----------------------------------------------------------------------------*/
void EpromInit(void)
{
	//------------------------------------------------------------------------------------------------------------
	FIDR=0;		// ֻ�е�FIDR =0xa5 ����FMCR= 02 ����FMCR=03 ʱ����ͬʱ�����ſ��Բ�������дEEPROM
				//��������������д�������FIDR����Ϊ0��������ܷ�����³��쳣
	//------------------------------------------------------------------------------------------------------------
	//����������/д EEPROM ʱ�Ĵ�����ַд��һ�ض���ַ��
	// ��ַΪEEPROM��ַ���������ǳ������ռ�õ�ַ(�������ռ乻������ʹ��_at_ ָ�����ָ��һ����ַ)
	// ��������֤�쳣����½����/д����ʱ�������/�޸�ROM �ĳ���ָ��Σ���ȷ��������������
	FSADRH = 0;
	FSADRM =(uint8)(EPROM_DEFAULT_ADR>>8);	
	FSADRL = (uint8)(EPROM_DEFAULT_ADR);	
	rEEP_FlagA=0;
	rEEP_FlagB=0;
	//------------------------------------------------------------------------------------------------------------
}

/*-----------------------------------------------------------------------------*
 *  Function Description:                                                      *
 *      ��д/������������                                                      * 
 *  Parameters:                                                                *
 *      p_EEPADDR��p_CMD                                                       *
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

