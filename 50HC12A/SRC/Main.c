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
uint8	g_u8SecTick	= 0;				   	//时钟节拍计数器
uint8	bdata	 g_u8SysStatus[4];		   	//系统标志
sbit bSysTick	 =	g_u8SysStatus[0]^0;	   	//系统Tick标志
sbit bSys_1000MS =  g_u8SysStatus[0]^1;    	//系统1000ms标志
sbit bKeyPress   =  g_u8SysStatus[0]^2;    	//有无按键标志
sbit bADOK       =	g_u8SysStatus[0]^3;    	//AD转换完成标志
sbit bFirstPress = 	g_u8SysStatus[0]^4;    	//第一次按键标志
sbit bNewKey     =	g_u8SysStatus[0]^5;    	//新按键按下标志
sbit bLEDBlank   =	g_u8SysStatus[0]^6;	   	//闪灯标志
sbit bTempReach  =  g_u8SysStatus[0]^7;    	//达到温度值标志

sbit bHeatArdReq  = g_u8SysStatus[1]^0;     //侧部加热请求标志
sbit bHeatTopReq  = g_u8SysStatus[1]^1;     //顶部加热请求标志
sbit bCookBySctEN = g_u8SysStatus[1]^2;     //允许按温度曲线烹饪标志
sbit bSysPwrOn    = g_u8SysStatus[1]^3;     //系统上电标志
sbit bTempAch1st  = g_u8SysStatus[1]^4;     //第一次达到设定温度点
sbit bHeatState   = g_u8SysStatus[1]^5;     //加热状态标志
sbit bCalBookTime_EN = g_u8SysStatus[1]^6;	//允许预约计算标志
sbit bLEDComeback =	g_u8SysStatus[1]^7;	    //LED显示恢复标志

sbit bPowerAdjSt     = g_u8SysStatus[2]^0;  //功率调整判断标志
sbit bHeatPwrReq    = g_u8SysStatus[2]^1;   //加热总开关
sbit bPwrDown     = g_u8SysStatus[2]^2;     //系统掉电标志
sbit bTimeCountDown = g_u8SysStatus[2]^3;
sbit bTimeDisp_EN   = g_u8SysStatus[2]^4;
sbit bBookTime_EN   = g_u8SysStatus[2]^5;   //预约时间有效
sbit bTempDisp_EN   = g_u8SysStatus[2]^6;   
sbit bLEDNumBlankSt = g_u8SysStatus[2]^7;   //LED数字闪烁标志

sbit bStageChgReq   = g_u8SysStatus[3]^0;	//控制跳转请求
sbit bStageChgOK    = g_u8SysStatus[3]^1;	//允许跳转
sbit bWarm_EN       = g_u8SysStatus[3]^2;	//保温标志
sbit bWarmLedBlank  = g_u8SysStatus[3]^3;	//保温灯闪烁
sbit bBoilChk_EN    = g_u8SysStatus[3]^4;	//沸腾判断有效
sbit bBoilReach     = g_u8SysStatus[3]^5;   //已沸腾标志
sbit bKeyMusic_EN   = g_u8SysStatus[3]^6;	//蜂鸣器标志

uint8	bdata   g_u8ErrorType;				//报警类型
sbit bTopSensorErr   = g_u8ErrorType^0;     //上传感器错误标志
sbit bBotSensorErr   = g_u8ErrorType^1;     //下传感器错误标志
sbit bIGBTSensorErr  = g_u8ErrorType^2;     //IGBT传感器错误标志
sbit bNoPanErr       = g_u8ErrorType^3;    	//无锅标志
sbit bPanOverTempErr = g_u8ErrorType^4;    	//锅底温度过高
sbit bVoltageErr     = g_u8ErrorType^5;    	//电压异常标志
sbit bHaltHeatErr    = g_u8ErrorType^6;    	//不加热标志
sbit bIGBTOverTempErr= g_u8ErrorType^7;     //IGBT温度过高标志


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
			//关闭显示
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

        //自检启动
        AutotestActivate();
        
 		//故障检测
 	  	FaultCheckProc();
 		
 		//沸腾判断
 //		BoilCheckProc();

 		//温度控制
 		KeepTempProc();
 		HeatProc();

        bSysPwrOn   =   0;
		bSys_1000MS	=	0;
	}
}
