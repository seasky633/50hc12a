/*-----------------------------*
 *  include head files         *
 *----------------------------*/
#include ".\Head\HD_cfg.h"
#include ".\Head\TempCtr.h"

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
void	KeepTempProc(void);
void	HeatProc(void);

/*-----------------------------*
 *  static functions prototypes*
 *----------------------------*/


/*-----------------------------*
 *  public variable declaration*
 *----------------------------*/
uint8	g_u8HiTemp;				//温度上限
uint8	g_u8LoTemp;				//温度下限
uint8	g_u8OnTime_Set;
uint8	g_u8OffTime_Set;
uint8	g_u8PwrCnt;
uint16	g_u16RelayTime;	        //继电器动作时间

/*-----------------------------*
 *  static variable declaration*
 *----------------------------*/
static	uint8	u8OnTime;
static	uint8	u8OffTime;

/*-----------------------------------------------------------------------------*
 *  Function Description:                                                      *
 *      温度维持程序                                   					       *
 *  Parameters:                                                                *
 *      None                                                                   *
 *  Return                                                                     *
 *      None                                                                   *
 *----------------------------------------------------------------------------*/
void	KeepTempProc(void)
{
	uint8	u8BotDegree	=	ADCGetData(AD_CH_BOT);

	if(g_u8HiTemp==0)
	{
		u8OnTime	=	0;
		return;
  	}

	if(bSys_1000MS)
	{ 
		if(bTempReach)
		{	
			//温度已经到达,则检查Low温度
			if(u8BotDegree>=g_u8LoTemp)
			{
				//温度还在目标范围内
				u8OnTime	=	0;
				return;
	  		}
			else
			{
				//已经低于LowTemp
				bTempReach	=	0;
			}	
		}

		//温度还没达到
		if(u8BotDegree>=g_u8HiTemp)
		{
			//温度大于上限温度
			bTempReach	=	1;			//置温度已经达到上限状态
			u8OnTime	=	0;
 		}
		else
		{
			//温度仍然低于上限温度
			//设置间隔加热参数
			u8OnTime	=	g_u8OnTime_Set;
			u8OffTime	=	g_u8OffTime_Set;
   		}
	}
}

/*-----------------------------------------------------------------------------*
 *  Function Description:                                                      *
 *      加热控制程序                                  				           *
 *  Parameters:                                                                *
 *      None                                                                   *
 *  Return                                                                     *
 *      None                                                               	   *
 *-----------------------------------------------------------------------------*/
void	HeatProc(void)
{
	if(bPwrDown==1)                 //掉电关闭加热
	{
        bHeatState = 0;
	}
	else
	{
		if(u8OnTime==0) 			//无功率输出
		{
	//		IO_HEAT_BOT	=	0;
	//		P1 &= ~(1<<0);
			bHeatState	=	0;
			g_u8PwrCnt	=	0;			
		}
		else
		{
			if(u8OnTime==0xf0) 		//连续加热
			{
	//			IO_HEAT_BOT	=	1;
	//			P1	|= (1<<0);
				bHeatState	=	1;
				g_u8PwrCnt	=	0;	
				if(bSys_1000MS)	g_u16RelayTime++;	
			}
			else
			{
				if(bSys_1000MS)
				{
					g_u8PwrCnt++;

					if(bHeatState)
					{
						//HeatProc_OnState
						if(g_u8PwrCnt>=u8OnTime)
						{
							bHeatState	=	0;		//设置间隔加热状态为断开状态
							g_u8PwrCnt	=	0;		//间隔加热计时器清零
	//						IO_HEAT_BOT	=	0;	
	//						P1 &= ~(1<<0);
						}
						else
						{
	//						IO_HEAT_BOT	=	1;	
	//						P1 |= (1<<0);
	                        bHeatState	=	1;
							g_u16RelayTime++;		
						}
					}
					else
					{
						//HeatProc_OffState
						if(g_u8PwrCnt>=u8OffTime)
						{
							bHeatState	=	1;		//设置间隔加热状态为打开状态
							g_u8PwrCnt	=	0;		//间隔加热计时器清零
	//						IO_HEAT_BOT	=	1;
	//						P1	|= (1<<0);
							g_u16RelayTime++;
						}
						else
						{
	//			 			IO_HEAT_BOT	=	0;
	//						P1 &= ~(1<<0);
	                        bHeatState	=	0;
						}
					}	
				}
			}
		}

//		if(bHeatTopReq)
//		{
//	//    	IO_HEAT_TOP	=	1;	
//			P0 |= (1<<0);                     
//		}
//		else
//		{
//	//    	IO_HEAT_TOP	=	0;	
//			P0 &= ~(1<<0);                    
//		}		
	}
}