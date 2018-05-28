/*-----------------------------*
 *  include head files         *
 *----------------------------*/
#include".\Head\HD_cfg.h"

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
void	System_INIT(void);

/*-----------------------------*
 *  static functions prototypes*
 *----------------------------*/
static 	void 	SetSysClock(void);
static	void 	Port_INIT(void);
// static  void	Timer0_INIT(void);
static 	void 	Timer1_INIT(void);
static  void    WDT_init(void);

/*-----------------------------*
 *  public variable declaration*
 *----------------------------*/
uint8	bdata	SysTime;					
sbit	bSys1S_CookTime	=	SysTime^0;      
sbit	bSys1S_BookTime	=	SysTime^1; 

/*-----------------------------*
 *  static variable declaration*
 *----------------------------*/
static	uint8	xdata m_u8SysTickCnt = 0;
static	uint16	xdata m_u16SysTimeCnt =	0;
static  uint8   xdata m_u8U125Cnt = 0;
static  uint8   xdata m_u8M100Cnt = 0;     

/*-----------------------------------------------------------------------------*
 *  Function Description:                                                      *
 *      Initialize the System.                                                 *
 *  Parameters:                                                                *
 *      None                                                                   *
 *  Return                                                                     *
 *      None                                                                   *
 *----------------------------------------------------------------------------*/
void System_INIT(void)
{
    cli();          	// disable INT. during peripheral setting
	Port_INIT();        // initialize ports
	SetSysClock();      // initialize operation clock
	ADC_INIT();         // initialize A/D convertor
//	Timer0_INIT();      // Timer0 to PWM
	Timer1_INIT();      // Timer1 to Count
	WDT_init();     	// initialize Watch-dog timer
    sei();           	// enable INT.
}

/*-----------------------------------------------------------------------------*
 *  Function Description:                                                      *
 *      Configures the System clock frequency                                  *
 *  Parameters:                                                                *
 *      None                                                                   *
 *  Return                                                                     *
 *      None                                                                   *
 *----------------------------------------------------------------------------*/
static void SetSysClock(void)
{
	// external clock
	OSCCR = 0x0A;   	// Enable int. 1MHz and Ext. OSC
	BITCR = 0x09;   	// Set waiting time : 16ms@1MHz
	while((BITCR & 0x80) == 0);	// Ext. OSC stabilizing time
	SCCR = 0x01;    	// Change to Ext. OSC
	OSCCR |= 0x04;  	// Disable Int. OSC
}

/*-----------------------------------------------------------------------------*
 *  Function Description:                                                      *
 *      Initialize the GPIO.                                                   *
 *  Parameters:                                                                *
 *      None                                                                   *
 *  Return                                                                     *
 *      None                                                                   *
 *----------------------------------------------------------------------------*/
static	void Port_INIT(void)
{
	// initialize ports
	//   2 : XOUT     i 8MHz
	//   3 : XIN      i 8MHz
	//   4 : P35      o NC
	//   5 : P34      o SEG5
	//   6 : P33      o SEG4
	//   7 : P32      o NC
	//   8 : P31      o SEG3
	//   9 : P30      o SEG2
	//  10 : P25      o COM3
	//  11 : P24      o COM7
	//  12 : P23      o COM6
	//  13 : P22      o COM5
	//  14 : P21      o COM4
	//  15 : P20      o COM2
	//  16 : P17      o COM1
	//  17 : P16      o SEG1
	//  18 : P15      o SEG8       
	//  19 : P14      o SEG7       
	//  20 : P13      o SEG6        
	//  21 : P12      i IO_KEY_WARM
	//  22 : P11      i IO_KEY_START
	//  23 : P10      o NC
 	//  24 : P03      o SCL
 	//  25 : P02      o SDA
 	//  26 : P01      o NC
 	//  27 : P00      o NC
	P0IO = 0xFF;    	// direction
	P0PU = 0x00;    	// pullup
	P0OD = 0x00;    	// open drain
	P03DB = 0x00;   	// bit7~6(debounce clock), bit5~0=P35,P06~02 debounce
	P0   = 0x00;    	// port initial value
	
	P1IO = 0xF9;    	// direction
	P1PU = 0x06;    	// pullup
	P1OD = 0x80;    	// open drain
	P12DB = 0x06;   	// debounce : P23~20, P13~10
	P1   = 0x80;    	// port initial value
	
	P2IO = 0xFF;    	// direction
	P2PU = 0x00;    	// pullup
	P2OD = 0x3F;    	// open drain
	P2   = 0x3F;    	// port initial value
	
	P3IO = 0x3F;    	// direction
	P3PU = 0x00;    	// pullup
	P3OD = 0x00;    	// open drain
	P3   = 0x00;    	// port initial value
	
	// Set port functions
	P0FSR = 0x00;   	// P0 selection
	P1FSRH = 0x00;  	// P1 selection High
	P1FSRL = 0x00;  	// P1 selection Low
	P2FSR = 0x00;   	// P2 selection
	P3FSR = 0xC0;   	// P3 selection
}

/*-----------------------------------------------------------------------------*
 *  Function Description:                                                      *
 *      Initialize the Timer0.                                                 *
 *  Parameters:                                                                *
 *      None                                                                   *
 *  Return                                                                     *
 *      None                                                                   *
 *----------------------------------------------------------------------------*/
// void Timer0_init()
// {
// 	// initialize Timer0
// 	// 8bit PWM, period = 0.256000mS ( 3906.250000Hz )
// 	//     PWM duty = 50.000000%
// 	T0CR = 0x94;    	// stop & PWM setting
// 	T0DR = 0x7F;    	// period count
// 	T0CR |= 0x01;   	// start
// }

/*-----------------------------------------------------------------------------*
 *  Function Description:                                                      *
 *      Initialize the Timer1.                                                 *
 *  Parameters:                                                                *
 *      None                                                                   *
 *  Return                                                                     *
 *      None                                                                   *
 *----------------------------------------------------------------------------*/
static void Timer1_INIT(void)
{
	// initialize Timer1
	// 16bit timer, period = 0.125000mS
	T1CRH = 0x00;   	// timer setting High
	T1CRL = 0xC0;   	// timer setting Low
	T1ADRH = 0x03;  	// period count High
	T1ADRL = 0xE7;  	// period count Low
	IE2 |= 0x04;    	// Enable Timer1 interrupt
	T1CRH |= 0x80;  	// start
}

/*-----------------------------------------------------------------------------*
 *  Function Description:                                                      *
 *      Initialize the Watch-dog timer                                         *
 *  Parameters:                                                                *
 *      None                                                                   *
 *  Return                                                                     *
 *      None                                                                   *
 *----------------------------------------------------------------------------*/
void WDT_clear(void)
{
	WDTCR |= 0x20;  	// Clear Watch-dog timer
}

void WDT_init(void)
{
	// initialize Watch-dog timer
	WDTDR = 0x14;   	// period
	WDTCR = 0xC2;   	// setting
	WDT_clear();
}

/*-----------------------------------------------------------------------------*
 *  Function Description:                                                      *
 *      定时器1中断处理(中断入口地址 = 0x0073 =115=N * 8 + 3=>>N = 14)     	   *
 *  Parameters:                                                                *
 *      None                                                                   *
 *  Return                                                                     *
 *      None                                                                   *
 *----------------------------------------------------------------------------*/
void	Timer1_ISP(void) interrupt 14       //125us 
{
    CHK_IH_Connector();
    
    m_u8U125Cnt++;
    if(m_u8U125Cnt>=8)
    {
        m_u8U125Cnt = 0;    	
        m_u8SysTickCnt++;
    	m_u16SysTimeCnt++;
        m_u8M100Cnt++;
    	
    	if(m_u8SysTickCnt>=8) 
    	{
    		m_u8SysTickCnt = 0;
    		bSysTick = 1;					// 系统8MS时钟节拍标志
    	}
        
    	if(m_u16SysTimeCnt>=1000)
    	{
    		m_u16SysTimeCnt	= 0;
    		SysTime	= 0xff;
    	}

        if(m_u8M100Cnt>103)
        {
            m_u8M100Cnt = 0;
        }
        else
        {
            if(m_u8M100Cnt==103)
            {
                CHK_IH_Com_Set();           //刷新IH设置数据
            }
        }

        P1 &= 0x87;                         //all segment off
        P3 &= 0xC0;							
	    P1 |= 0x80;							//all com off
	    P2 |= 0x3F;	
        if(bPwrDown==0)
        {
    	    u8Com_Mode++;
    	    if(u8Com_Mode>7) u8Com_Mode = 0;	//1ms x 7 = 7 ms  Display Frame Frequency = 142Hz 
    	    if(u8Com_Mode<7)
            {
    	        if(LEDBuf[u8Com_Mode]&0x01) P1 |= (1<<6);  //segment data out
    	        if(LEDBuf[u8Com_Mode]&0x02) P3 |= (1<<0);
    	        if(LEDBuf[u8Com_Mode]&0x04) P3 |= (1<<1);
    	        if(LEDBuf[u8Com_Mode]&0x08) P3 |= (1<<3);
    	        if(LEDBuf[u8Com_Mode]&0x10) P3 |= (1<<4);
    	        if(LEDBuf[u8Com_Mode]&0x20) P1 |= (1<<3);
    	        if(LEDBuf[u8Com_Mode]&0x40) P1 |= (1<<4);
    	        if(LEDBuf[u8Com_Mode]&0x80) P1 |= (1<<5);
        	    switch(u8Com_Mode)						//grid switch on
        	    {
        		    case	0:
        			    P1 &= ~(1<<7);					//com1
        		        break;
        		    case	1:
        			    P2 &= ~(1<<0);					//com2
        			    break;
        		    case	2:
        			    P2 &= ~(1<<5);					//com3
        			    break;
        		    case	3:
        			    P2 &= ~(1<<1);					//com4
        			    break;
        		    case	4:
        			    P2 &= ~(1<<2);					//com5
        			    break;
        		    case	5:
        			    P2 &= ~(1<<3);					//com6
        		        break;
        		    case	6:
        			    P2 &= ~(1<<4);					//com7
        			    break;
        	    }        
            }
        }
    }
}
