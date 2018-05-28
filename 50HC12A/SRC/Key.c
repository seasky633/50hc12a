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
void   KeyProc(void);
uint8  GetKeyVal(void);
uint8  IsNewKey(void);

/*-----------------------------*
 *  static functions prototypes*
 *----------------------------*/

/*-----------------------------*
 *  public variable declaration*
 *----------------------------*/
uint8	m_u8KeyVal;

/*-----------------------------*
 *  static variable declaration*
 *----------------------------*/


 /*-----------------------------------------------------------------------------*
  *  Function Description:                                                      *
  *      从IO口上读取键值                                                       *
  *  Parameters:                                                                *
  *      None                                                                   *
  *  Return                                                                     *
  *      键值                                                                   *
  *----------------------------------------------------------------------------*/
uint8  GetKeyVal(void)
{
    if(bPwrDown==1) return 0;
    
	m_u8KeyVal	=  0x00;	
	cli();
	//关闭显示
    P1 &= 0x87;                         //all segment off
    P3 &= 0xC0;							
	P1 |= 0x80;							//all com off
	P2 |= 0x3F;	

	//设置扫键IO上拉输入并置一
    P1IO = 0x89;        //direction
    P1PU = 0x76;        //pullup
    P1 |= 0x76;        
    P3IO = 0x25;        //direction
    P3PU = 0x1A;        //pullup
    P3 |= 0x1A;
	NOP;
	NOP;
	NOP;
	NOP;
	NOP;
	NOP;
	NOP;
	NOP;
	NOP;
	NOP;
	NOP;
	NOP;
	NOP;
	NOP;
	NOP;
	NOP;
	NOP;
	NOP;
	NOP;
	NOP;
	if((P1&(1<<1))==0) m_u8KeyVal |= (1<<0);		//"Start"
	if((P1&(1<<5))==0) m_u8KeyVal |= (1<<1);		//"Delayed Start"            
	if((P3&(1<<4))==0) m_u8KeyVal |= (1<<2);		//"Minus" 
	if((P3&(1<<3))==0) m_u8KeyVal |= (1<<3);		//"Menu"     
	if((P1&(1<<6))==0) m_u8KeyVal |= (1<<4);		//"DIY"     	
	if((P3&(1<<1))==0) m_u8KeyVal |= (1<<5);		//"PLUS"    
	if((P1&(1<<4))==0) m_u8KeyVal |= (1<<6);		//"Temperature/Time"   
	if((P1&(1<<2))==0) m_u8KeyVal |= (1<<7);		//"Keep Warm/Cancel"
	
	//恢复显示
	P1IO = 0xF9;    	// direction
	P1PU = 0x06;    	// pullup
	P1 &= 0x87;
	P3IO = 0x3F;    	// direction
	P3PU = 0x00;    	// pullup
	P3 &= 0xC0;	
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
	sei();
	
	return	m_u8KeyVal;
}
  

/*                     下面程序可移植                    */

/*-----------------------------------------------------------------------------*
 *  Function Description:                                                      *
 *      扫键进程,连键时间=(100-95)*8+(100-0)*8+(100-85)*8*(n-2)                *
 *  Parameters:                                                                *
 *      None                                                                   *
 *  Return                                                                     *
 *      None                                                                   *
 *----------------------------------------------------------------------------*/
Bool bNewKey_Menu,bKeyPress_Menu;
uint16	g_u16KeyRpyCnt	=	0;		//连键次数
static	uint8	m_u8KeyAct	=	0;	//按键有效动作值
uint8 g_u8KeyUpAct = 0;             //按键抬起有效动作值
#define	KEY_START_TIME	95
#define	KEY_RPY_TIME	85
#define	KEY_END_TIME	100

void	KeyProc(void)	//8ms执行一次本程序
{
	static uint8	u8KeyPressCnt	=	0;
	static uint8	u8PrevKey	=	0;
	uint8	u8Key	=	GetKeyVal();	//获取键值

	if(u8Key==u8PrevKey)
	{
		u8KeyPressCnt++; //防抖计数变量（连按键间隔时间变量）

		if(u8KeyPressCnt>KEY_END_TIME)
		{
			m_u8KeyAct	=	u8PrevKey;	//最终有效键值m_u8KeyAct
			
			if(m_u8KeyAct==0)
			{
				bKeyPress	=	0;		//按键释放
			}
			else
			{
				bKeyPress	=	1;		//有键按下
			}

            if(m_u8KeyAct&0x08)
            {
                bKeyPress_Menu = 1;     //menu键按下
            }
            else
            {
                bKeyPress_Menu = 0;    //menu键释放
            }
			
			g_u16KeyRpyCnt++;		//连键次数加一
			if(bFirstPress)
			{
				u8KeyPressCnt	=	0;
				bFirstPress		=	0;
				if((u8Key)&&(bNewKey))
				{
				    g_u8KeyUpAct = m_u8KeyAct;
                    
				    if(bKeyMusic_EN)
                    {
					    PlaySound(MusicKeyDown);
                    }
				}
			} 
			else
			{
				u8KeyPressCnt = KEY_RPY_TIME;
			}
		}
	}
	else
	{ 
		if(!bKeyPress)	
		{
			bNewKey		=	1;			//按键释放才置新键标志
		}
		else
		{
			bNewKey		=	0;		
		}
        if((!bKeyPress_Menu)&&(u8Key&0x08))
        {
            bNewKey_Menu = 1;           //menu新键          
        }
        else
        {
            bNewKey_Menu = 0;
        }
		m_u8KeyAct		=	0;      	//此处不清掉，会出现松开键才有反应
		g_u16KeyRpyCnt	=	0;			//连键次数清零
		bFirstPress		=	1; 			//第一次按键标志，用于连按处理
		u8PrevKey		=	u8Key; 		//prev_key为前一次键值
		u8KeyPressCnt	=	KEY_START_TIME;  
	}
}    

/*-----------------------------------------------------------------------------*
 *  Function Description:                                                      *
 *      读取键值                                                      		   *
 *  Parameters:                                                                *
 *      None                                                                   *
 *  Return                                                                     *
 *      经过处理后的键值                                                       *
 *----------------------------------------------------------------------------*/
uint8	GetKey(void)
{
	uint8	u8Ret	=	m_u8KeyAct;
	m_u8KeyAct	=	0;
	return	u8Ret;
}

/*-----------------------------------------------------------------------------*
 *  Function Description:                                                      *
 *      判断是否按下新键                                                       *
 *  Parameters:                                                                *
 *      None                                                                   *
 *  Return                                                                     *
 *      判断结果                                                        	   *
 *----------------------------------------------------------------------------*/
uint8	IsNewKey(void)
{
	if(bNewKey)
	{
		bNewKey	=	0;
		return	TRUE;
	}
	return	FALSE;
}
