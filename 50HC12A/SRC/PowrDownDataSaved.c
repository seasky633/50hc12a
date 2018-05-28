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

#define EEP_ADDRESS_START 0x3FA0	//EEPROM ��ʼ��ַ
#define EEP_ADDRESS_END 0x3FFF		//EEPROM ������ַ
#define	EEP_SECTOR_ADDR	0x3FC0	

#define EPROM_DEFAULT_ADR EEP_ADDRESS_START	// �˴�ָ��Ĭ�ϲ�ʹ��Eprom����/дʱ���õĵ�ַָ���û�ָ����EPROM��ַ

#define EEPROM_W_MAX 5 //����д5�Σ����ɹ��˳�
#define	EEPERASE_CMD	0x02
#define	EEPWRITE_CMD	0x03
//================================
unsigned char xdata *ImageDat=0x8000;	//��������ͷ��ַ��32byte

unsigned char rEepromCache[32];	// ���ݻ���
unsigned char  rEEP_FlagA,rEEP_FlagB;
unsigned char rDataSavedFlag,rPowerRecoverCnt,r1msTick;

//================================
 unsigned char EEPROM_Process(void);
 void Program_Reset(void);
 void EpromInit(void);
 void FlashSector_Operate(unsigned int p_EEPADDR, unsigned char  p_CMD );
void main(void)
{  
	rDataSavedFlag=0xAA;//�ϵ罫��־��λ,��ֹ�ϵ��ѹ�ȶ�ǰ��EEPROM����
	LVICR=0x1B;//setting LVI detet <4.0V
	EpromInit();
	while(1)
	{
		WDTCR =0xE2;	// clear WDT
		if(LVICR&0x20)//power down to <4.0V
			{
			rEEP_FlagA=0x5A;//��A��־,��ֹ�쳣����
			rPowerRecoverCnt=0;			
			if(rDataSavedFlag==0)
				{
				EEPROM_Process();//�˴��ο�֮ǰ�ṩEEPROM����ע������
				rDataSavedFlag=0xAA;
				}
			LVICR=0x1B;//clear flag and detect again
			}
		else//
			{
			if(r1msTick==0xFF)
				{
				r1msTick=0;
				if(rPowerRecoverCnt>20)//20 ms debounce ,�û��������޸ĺ��ʵ�ʱ��
					rDataSavedFlag=0;//power recovered morethan 20ms,clear "saved "flag
				else rPowerRecoverCnt++;
				}
			}

	}

}


//================================
//����������
// ʾ��Ӧ�ã������ο�
//================================
unsigned char EEPROM_Process(void)
{
unsigned char code *p_EepRom;
unsigned char xdata *p_Buffer=0x8000;
unsigned char i;
ClosedDisplay();//�ر���ʾ,������ʡ��,��֤�㹻ʱ�����EEPROM����
rEEP_FlagB=0xA5;//��B��־,��ֹ�쳣����
DI();//�ر��ж�
WDTCR=0;//�رտ��Ź�;
LVRCR=0x01;//�رյ͵�ѹ��λ,��ֹ�ж�EEPROM����
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
//����λָ��
// ����⵽�����쳣����ʹ�ø�λ
//---------------------------------------------------
void Program_Reset(void)
 {
	((void (code *) (void)) 0x0000) ();

 }


//---------------------------------------------------
//����������EEPROM ����/ д ����ʱ��ʼ���Ĵ���
//---------------------------------------------------
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

