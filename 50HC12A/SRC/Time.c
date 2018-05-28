/*-----------------------------*
 *  include head files         *
 *----------------------------*/
#include ".\Head\HD_cfg.h"
#include ".\Head\Time.h"

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
void	IncTime(uint8	*pTime);
void	DecTime(uint8	*pTime);
void	IncTime_MIN(uint8	*pTime);
//void	IncTime_Hour(uint8	*pTime);

/*-----------------------------*
 *  static functions prototypes*
 *----------------------------*/


/*-----------------------------*
 *  public variable declaration*
 *----------------------------*/
uint8	tmCookTime[3];	//烹饪倒计时时间
uint8   tmBookTime[3];  //预约时间

/*-----------------------------*
 *  static variable declaration*
 *----------------------------*/


/*-----------------------------------------------------------------------------*
 *  Function Description:                                                      *
 *      时间增加1秒        													   *
 *  Parameters:                                                                *
 *      需要增加的时间地址指针                                                 *
 *  Return                                                                     *
 *      None                                                                   *
 *----------------------------------------------------------------------------*/
void	IncTime(uint8	*pTime)
{
	pTime[0]++;
	if(pTime[0]<60)	
	{
		return;
	}
    else
	{
		pTime[0]	=	0;
		IncTime_MIN(pTime);
	}
}

/*-----------------------------------------------------------------------------*
 *  Function Description:                                                      *
 *      时间减少1秒        													   *
 *  Parameters:                                                                *
 *      需要减少的时间地址指针                                                 *
 *  Return                                                                     *
 *      None                                                                   *
 *----------------------------------------------------------------------------*/
void	DecTime(uint8	*pTime)
{	
	pTime[0]--;
	if(pTime[0]==0xff)
	{
		pTime[0]	=	59;
		pTime[1]--;
		if(pTime[1]==0xff)
		{
			pTime[1]	=	59;
			pTime[2]--;
            if(pTime[2]==0xff)
			{
				pTime[2]	=	23;
			}
	  	}
  	}
}

/*-----------------------------------------------------------------------------*
 *  Function Description:                                                      *
 *      时间增加1分钟       												   *
 *  Parameters:                                                                *
 *      需要增加的时间地址指针                                                 *
 *  Return                                                                     *
 *      None                                                                   *
 *----------------------------------------------------------------------------*/
void	IncTime_MIN(uint8	*pTime)
{
	pTime[1]++;
	if(pTime[1]<60)
	{
		return;
	}
	else
	{
		pTime[1]	=	0;
		pTime[2]++;
		if(pTime[2]==24)
		{
			pTime[2]	=	0;
		}
  	}
}

/*-----------------------------------------------------------------------------*
 *  Function Description:                                                      *
 *      时间增加1小时      												   	   *
 *  Parameters:                                                                *
 *      需要增加的时间地址指针                                                 *
 *  Return                                                                     *
 *      None                                                                   *
 *----------------------------------------------------------------------------*/
// void	IncTime_Hour(uint8 	*pTime)
// {
// 	pTime[2]++;
// 	if(pTime[2]>=24)  pTime[2]	=	0;
// }
