//==============================================================================
//	Project     : CFXB50HC12A-V4(1204)	
//	Date	    : 2018.5.14
//	Author      : wuyong

//	Device      : MC96F8316S(28SOP)
//	Voltage     : 5V
//	Frequency	: external 8 MHz OSC
//  ADC range   : 5V (Vdd)
//==============================================================================
//	History :
/*
    2017-12-20	1. Display Program Start
    2018-01-09  1. Updated the program list(the number of programs)
    2018-01-10  1. Add 3 temperature settings for soup
                2. Add 1 temperature setting for stew
                3. Change time settings for Buckwheat
    2018-01-12  1. Change buckwheat porridge to jam
    2018-05-14  1. Add boling water mode 
    
//==============================================================================

                              MC86F8316S (28SOP)
                  ____________________  ____________________
         0V --> 1|VSS                                   VDD|28 <-- 5V
       8MHz <-- 2|P37/XOUT                     DSDA/AN0/P00|27 --> 
       8MHz --> 3|P36/XIN                      DSCL/AN1/P01|26 -->
            <-- 4|P35/EINT10/PWM0/T0O   AN2/AVREF/EINT0/P02|25 <-> SDA
	   SEG5 <-- 5|P34/SXIN                    AN3/EINT1/P03|24 --> SCL
       SEG4 <-- 6|P33/SXOUT                   AN7/EINT5/P10|23 --> 
            <-- 7|P32/RESETB          AN8/EINT6/EC1/BUZ/P11|22 <-- IO_KEY_START
       SEG3 <-- 8|P31/RXD/(SCL)      AN9/EINT11/T1/PWM1/P12|21 <-- IO_KEY_WARM
       SEG2 <-- 9|P30/TXD/(SDA)     AN10/EINT12/T2/PWM2/P13|20 --> SEG6         
       COM3 <--10|P25/SCL                     AN11/MISO/P14|19 --> SEG7         
       COM7 <--11|P24/SDA                     AN12/MOSI/P15|18 --> SEG8         
       COM6 <--12|P23/EINTA                    AN13/SCK/P16|17 --> SEG1  
       COM5 <--13|P22/EINT9                 AN14/EC2/SS/P17|16 --> COM1
       COM4 <--14|P21/EINT8                       EINT7/P20|15 --> COM2
                 -------------------------------------------
*/


/*-----------------------------*
 *  include head files         *
 *----------------------------*/
#include ".\Head\HD_cfg.h"
#include ".\Head\System.h"
#include ".\Head\TempCtr.h"
#include ".\Head\Script.h"

/*-----------------------------*
 *  public variable declaration*
 *----------------------------*/
uint8	g_u8SecTick	= 0;				   	//ʱ�ӽ��ļ�����
uint8	bdata	 g_u8SysStatus[4];		   	//ϵͳ��־
sbit bSysTick	 =	g_u8SysStatus[0]^0;	   	//ϵͳTick��־
sbit bSys_1000MS =  g_u8SysStatus[0]^1;    	//ϵͳ1000ms��־
sbit bKeyPress   =  g_u8SysStatus[0]^2;    	//���ް�����־
sbit bADOK       =	g_u8SysStatus[0]^3;    	//ADת����ɱ�־
sbit bFirstPress = 	g_u8SysStatus[0]^4;    	//��һ�ΰ�����־
sbit bNewKey     =	g_u8SysStatus[0]^5;    	//�°������±�־
sbit bLEDBlank   =	g_u8SysStatus[0]^6;	   	//���Ʊ�־
sbit bTempReach  =  g_u8SysStatus[0]^7;    	//�ﵽ�¶�ֵ��־

sbit bHeatArdReq  = g_u8SysStatus[1]^0;     //�ಿ���������־
sbit bHeatTopReq  = g_u8SysStatus[1]^1;     //�������������־
sbit bCookBySctEN = g_u8SysStatus[1]^2;     //�����¶�������⿱�־
sbit bSysPwrOn    = g_u8SysStatus[1]^3;     //ϵͳ�ϵ��־
sbit bTempAch1st  = g_u8SysStatus[1]^4;     //��һ�δﵽ�趨�¶ȵ�
sbit bHeatState   = g_u8SysStatus[1]^5;     //����״̬��־
sbit bCalBookTime_EN = g_u8SysStatus[1]^6;	//����ԤԼ�����־
sbit bLEDComeback =	g_u8SysStatus[1]^7;	    //LED��ʾ�ָ���־

sbit bPowerAdjSt     = g_u8SysStatus[2]^0;  //���ʵ����жϱ�־
sbit bHeatPwrReq    = g_u8SysStatus[2]^1;   //�����ܿ���
sbit bPwrDown     = g_u8SysStatus[2]^2;     //ϵͳ�����־
sbit bTimeCountDown = g_u8SysStatus[2]^3;
sbit bTimeDisp_EN   = g_u8SysStatus[2]^4;
sbit bBookTime_EN   = g_u8SysStatus[2]^5;   //ԤԼʱ����Ч
sbit bTempDisp_EN   = g_u8SysStatus[2]^6;   
sbit bLEDNumBlankSt = g_u8SysStatus[2]^7;   //LED������˸��־

sbit bStageChgReq   = g_u8SysStatus[3]^0;	//������ת����
sbit bStageChgOK    = g_u8SysStatus[3]^1;	//������ת
sbit bWarm_EN       = g_u8SysStatus[3]^2;	//���±�־
sbit bWarmLedBlank  = g_u8SysStatus[3]^3;	//���µ���˸
sbit bBoilChk_EN    = g_u8SysStatus[3]^4;	//�����ж���Ч
sbit bBoilReach     = g_u8SysStatus[3]^5;   //�ѷ��ڱ�־
sbit bKeyMusic_EN   = g_u8SysStatus[3]^6;	//��������־

uint8	bdata   g_u8ErrorType;				//��������
sbit bTopSensorErr   = g_u8ErrorType^0;     //�ϴ����������־
sbit bBotSensorErr   = g_u8ErrorType^1;     //�´����������־
sbit bIGBTSensorErr  = g_u8ErrorType^2;     //IGBT�����������־
sbit bNoPanErr       = g_u8ErrorType^3;    	//�޹���־
sbit bPanOverTempErr = g_u8ErrorType^4;    	//�����¶ȹ���
sbit bVoltageErr     = g_u8ErrorType^5;    	//��ѹ�쳣��־
sbit bHaltHeatErr    = g_u8ErrorType^6;    	//�����ȱ�־
sbit bIGBTOverTempErr= g_u8ErrorType^7;     //IGBT�¶ȹ��߱�־


/*-----------------------------------------------------------------------------*
 *  Function Description:                                                      *
 *      Main Function.                                                         *
 *  Parameters:                                                                *
 *      None                                                                   *
 *  Return                                                                     *
 *      None                                                                   *
 *----------------------------------------------------------------------------*/
void main(void)
{   	
	System_INIT();
	LVICR=0x1B;//setting LVI detet <4.0V
	EpromInit();
	EnterReset();

	while(1)
	{
		WDT_clear();

		if(LVICR&0x20)//power down to <4.0V
		{
		    bPwrDown = 1;                       //system Powerdown
			//�ر���ʾ
            P1 &= 0x87;                         //all segment off
            P3 &= 0xC0;							
	        P1 |= 0x80;							//all com off
	        P2 |= 0x3F;	
			LVICR=0x1B;//clear flag and detect again
			continue;
		}
        else
        {
            bPwrDown = 0;                       
        }
		
		if(!bSysTick)continue;
		bSysTick	=	0;

		g_u8SecTick++;
		if(g_u8SecTick>=125)
		{
			g_u8SecTick	=	0;
			bSys_1000MS	=	1;
		}
		
		ADCProc();	
		LEDProc(m_u8SysID);	   	
		KeyProc();
	    SoundProc();
		
		UIProc();
 		ScriptProc();

        //�Լ�����
        AutotestActivate();
        
 		//���ϼ��
 	  	FaultCheckProc();
 		
 		//�����ж�
 //		BoilCheckProc();

 		//�¶ȿ���
 		KeepTempProc();
 		HeatProc();

        bSysPwrOn   =   0;
		bSys_1000MS	=	0;
	}
}
