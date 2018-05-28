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
void	SoundProc(void);
void	PlaySound(uint8	code *pMusic);

/*-----------------------------*
 *  static functions prototypes*
 *----------------------------*/


/*-----------------------------*
 *  public variable declaration*
 *----------------------------*/
uint8 xdata m_u8OnTime = 0;
uint8 xdata m_u8OffTime = 0;
uint8 xdata g_u8BuzMode = 0;            //0:over 1:100ms 2:300ms  3:800ms
uint8 code *m_u8pMusic = NULL;
uint8 code *m_u8pMusicBak = NULL;		//备份指针，用于重复播放

uint16	code MetroTable[] =
{
//	0ms		100ms	300ms   800ms  
	0,		13,		38,		100
};

uint8 code MusicPoweron[] =
{
    M_800|S_300,0x00
};

uint8 code MusicKeyDown[] =
{
    M_300|S_300,0x00
};

uint8 code MusicSysError[] =
{
    M_300|S_800,M_300|S_800,M_300|S_800,M_300|S_800,M_300|S_800,
	M_300|S_800,M_300|S_800,M_300|S_800,M_300|S_800,M_300|S_800,0x00
};

uint8 code MusicSysErrorNoPan[] = 
{
    M_300|S_800,M_300|S_800,M_300|S_800,M_300|S_800,M_300|S_800,0xFF
};

uint8 code MusicCookEnd[] =
{
    M_800|S_800,M_800|S_800,M_800|S_800,0x00
};

uint8 code MusicAchievedTemp[] =
{
	M_800|S_300,M_800|S_300,0x00
};

uint8 code MusicOFF[] =
{
	0x00
};

uint8 code MusicInputFood[] = 
{
    M_300|S_300,M_300|S_300,0x00    
};

uint8 code MusicCancelFunc[] =
{
    M_100|S_100,M_100|S_100,0x00
};

/*-----------------------------------------------------------------------------*
 *  Function Description:                                                      *
 *      声音播放函数                                                           *
 *  Parameters:                                                                *
 *      声音数据首地址                                                         *
 *  Return                                                                     *
 *      None                                                                   *
 *----------------------------------------------------------------------------*/
void	PlaySound(uint8	code	*pMusic)
{
	m_u8pMusicBak	=  	m_u8pMusic  =	pMusic;
    m_u8pMusic = pMusic;
	m_u8OnTime	= 0;
	m_u8OffTime	= 1;
    g_u8BuzMode = 0;
}

/*-----------------------------------------------------------------------------*
 *  Function Description:                                                      *
 *      声音处理进程                                                           *
 *  Parameters:                                                                *
 *      None                                                                   *
 *  Return                                                                     *
 *      None                                                                   *
 *----------------------------------------------------------------------------*/
void	SoundProc(void)
{
	uint8	u8Tmp;

	if(m_u8OnTime>0)
	{
		m_u8OnTime--;
	}
	else	
	{
		if(m_u8OffTime>0)
		{		
            g_u8BuzMode = 0;			
			m_u8OffTime--;
			if(m_u8OffTime==0)
			{    
				//取音符频率,和节拍
				u8Tmp =	*m_u8pMusic;
				if(u8Tmp==0x00)
				{ 	
					//音乐结束
			   		return;			
				}
				else	
				{
					if(u8Tmp==0xff)
					{     
						//音乐循环
			    		m_u8pMusic  =   m_u8pMusicBak;
               		    m_u8OnTime	=	0;
	            		m_u8OffTime	=	1;
                        g_u8BuzMode = 0;
						return;
             		}
	
					u8Tmp		&=	0x0f;				  //前4位为打开时间
					g_u8BuzMode =   u8Tmp;
					m_u8OnTime	=	MetroTable[u8Tmp];

            		u8Tmp		=	*m_u8pMusic++;
					u8Tmp		=	(u8Tmp>>4)&0x0f;      //后4位为关闭时间
					m_u8OffTime	=	MetroTable[u8Tmp];
					
//					P3OD  = 0x00;       // open drain
//					P3FSR |= 0x20;		// 设为PWM0O
//					T0CR = 0x94;    	// stop & PWM setting
//					T0DR = 0x7F;    	// period count
//					T0CR |= 0x01;   	// 启动蜂鸣器
				}
			}
		}
	}
}
