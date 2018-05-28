#ifndef _COMMDEF_H
#define	_COMMDEF_H

	extern	uint8	bdata   g_u8ErrorType;
	
	//1S时钟标志(用于定时&预约)
	extern	bit	bSys1S_CookTime;
	extern	bit	bSys1S_BookTime;
	
	//系统标志
	extern	bit	bSysTick;	
	extern  bit bSys_1000MS; 
	extern  bit bKeyPress; 
    extern  bit bADOK;       
    extern  bit bFirstPress; 
	extern  bit bNewKey;     
    extern  bit bLEDBlank;   
    extern  bit bTempReach;  

	extern  bit bHeatArdReq;  
    extern  bit bHeatTopReq;  
	extern  bit bCookBySctEN;
	extern  bit bSysPwrOn;   
    extern  bit bTempAch1st;   
	extern  bit bHeatState;   
    extern  bit bCalBookTime_EN; 
    extern  bit bLEDComeback; 

    extern  bit bPowerAdjSt;     
    extern  bit bHeatPwrReq;      
    extern  bit bPwrDown;    
	extern  bit bTimeCountDown;  
	extern  bit bTimeDisp_EN;   
	extern  bit bBookTime_EN;  
    extern  bit bTempDisp_EN;
	extern  bit bLEDNumBlankSt; 

	extern  bit bStageChgReq; 
    extern  bit bStageChgOK;	
	extern  bit bWarm_EN;     
    extern  bit bWarmLedBlank;	
	extern  bit bBoilChk_EN;    
	extern  bit bBoilReach;    
	extern  bit bKeyMusic_EN;      

	//报警类型
    extern  bit bTopSensorErr;  
    extern  bit bBotSensorErr;   
    extern  bit bIGBTSensorErr; 
    extern  bit bNoPanErr;      
    extern  bit bPanOverTempErr; 
    extern  bit bVoltageErr;     
    extern  bit bHaltHeatErr;    
    extern  bit bIGBTOverTempErr;
	
	//报警阶段
	#define	SYS_ERROR_CHECK_START	0
	#define	SYS_ERROR_CHECK_END		0xA5

/* ------------------- BIT Register-------------------- */


//	sbit    IO_HEAT_BOT  	=   P1^3;   
//	sbit    IO_HEAT_TOP     =   P1^2;  
//	sbit    IO_BUZ		    =   P3^5;
//  sbit    IO_DIO	     	=   P1^5;   
//	sbit    IO_CLK     		=   P1^6;   
//	sbit    IO_STB			=	P1^7;	

    #define	NULL	0
	#define	FALSE	0
	#define	TRUE	1
	
//	#define	DEBUG0

#endif
