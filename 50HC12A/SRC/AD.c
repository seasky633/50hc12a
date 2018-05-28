/*-----------------------------*
 *  include head files         *
 *----------------------------*/
#include ".\Head\HD_cfg.h"

/*-----------------------------*
 *  Macros for constants       *
 *----------------------------*/
//#define		AD_TEMP_SIZE		3		//临时数据缓冲的大小
#define     AD_CH_NUM			2		//通道个数
#define     AD_STAT_SIZE		8		//每个通道堆栈的大小

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
void	ADC_INIT(void);
uint8	ADCGetData(uint8 u8CH);
uint8	ADCGetVal(uint8 u8CH);
//void	Sort8(uint8	*pDat,uint8	u8Size);
//void	Sort16(uint16 *pDat,uint8 u8Size); 
void	ADCProc(void);

/*-----------------------------*
 *  static functions prototypes*
 *----------------------------*/
//static	void	ADC_Start(uint8  u8CH);							
static	uint8	GetDegree(uint8 adData,uint8 u8CH);
static	uint8	Dichotomy_Search(uint8 u8Array[],uint8 n,uint8 u8Key);  

/*-----------------------------*
 *  public variable declaration*
 *----------------------------*/
uint8	idata 	u8ADBuf[AD_CH_NUM][AD_STAT_SIZE];	//ADC缓冲器
//uint16	idata	u16AdTempBuf[AD_TEMP_SIZE];		    //临时数据缓冲
//uint8	u8CurCH		=	0;							//当前通道
//uint8	u8CurCHID;									//当前通道对应的ID
//uint8	u8ADCnt		=	0;							//连续转换的次数
uint8	idata   u8ChgCnt =	0;						//转换次数
uint8	idata	g_u8ADResult[AD_CH_NUM];			//AD转换最终结果(温度)
uint8	idata	g_u8ADValue[AD_CH_NUM];			//AD转换最终结果(AD)
//uint8   code CHNO2ID[] = {2,3};  

/*-----------------------------*
 *  static variable declaration*
 *----------------------------*/


/*-----------------------------------------------------------------------------*
 *  Function Description:                                                      *
 *      AD convert complete interrupt handler.                                 *
 *  Parameters:                                                                *
 *      None                                                                   *
 *  Return                                                                     *
 *      None                                                                   *
 *----------------------------------------------------------------------------*/
//void	ADC_ISP(void) interrupt 18
//{
//    uint16	u16AdTemp;
//	
//	u16AdTemp =	(ADCDRH << 8) | ADCDRL;		// read ADC
//    u16AdTempBuf[u8ADCnt++] = u16AdTemp;	//将转换结果保存到临时缓冲区中
//	
//    if(u8ADCnt>=AD_TEMP_SIZE)           //是否连续采集了(AD_TEMP_SIZE)次
//	{					
//		u8ADCnt  =	0;
//		bADOK	 =	1;		            //当临时缓冲区已存满数据时，置ADOK位为1 
//		ADCCRL   =  0;    				//停止转换
//	}
//	else
//	{
//		ADCCRL |= 0x40; 	// start ADC	
//	}
//}

/*-----------------------------------------------------------------------------*
 *  Function Description:                                                      *
 *      Initialize the ADC.                                                    *
 *  Parameters:                                                                *
 *      None                                                                   *
 *  Return                                                                     *
 *      None                                                                   *
 *----------------------------------------------------------------------------*/
void	ADC_INIT(void)
{
	// initialize A/D convertor
	ADCCRL = 0x00;  	// setting
	ADCCRH = 0x00;  	// trigger source, alignment, frequency
	ADWCRL = 0x00;  	// Wake-up enable Low
	ADWCRH = 0x00;  	// Wake-up enable High
	ADWRCR0 = 0x00; 	// Wake-up R selection
	ADWRCR1 = 0x00; 	// Wake-up R selection
	ADWRCR2 = 0x00; 	// Wake-up R selection
	ADWRCR3 = 0x00; 	// Wake-up R selection
	IE3 &= 0xFE;    	// disable ADC interrupt
//	u8CurCH	= 0;        //u8CurCH : 当前通道序号
//	u8CurCHID =	CHNO2ID[u8CurCH];	//u8CurCHID: 当前通道ID
//	ADC_Start(u8CurCHID);			//选择通道&开始转换
}

/*-----------------------------------------------------------------------------*
 *  Function Description:                                                      *
 *      Select ADC Channel.                                                    *
 *  Parameters:                                                                *
 *      u8Ch                                                                   *
 *  Return                                                                     *
 *      None                                                                   *
 *----------------------------------------------------------------------------*/
//static	void	ADC_Start(uint8  u8CH)
//{
//	// start A/D convertor
//	ADCCRL = 0x80;  	// setting
//	ADCCRL = (ADCCRL & 0xf0) | (u8CH & 0x0f);	// select channel
//	ADCCRL |= 0x40; 	// start ADC
//}

/*-----------------------------------------------------------------------------*
 *  Function Description:                                                      *
 *      查询AD转换最终结果.                                                    *
 *  Parameters:                                                                *
 *      转换通道                                                               *
 *  Return                                                                     *
 *      AD转换最终结果(温度)                                                   *
 *----------------------------------------------------------------------------*/
uint8	ADCGetData(uint8 u8CH)
{
	return	g_u8ADResult[u8CH];	
}

/*-----------------------------------------------------------------------------*
 *  Function Description:                                                      *
 *      查询AD转换最终结果.                                                    *
 *  Parameters:                                                                *
 *      转换通道                                                               *
 *  Return                                                                     *
 *      AD转换最终结果(AD)                                               	   *
 *----------------------------------------------------------------------------*/
uint8   ADCGetVal(uint8	u8CH)
{
	return	g_u8ADValue[u8CH];
}

/*-----------------------------------------------------------------------------*
 *  Function Description:                                                      *
 *      AD值转换成温度值并处理.                                                *
 *  Parameters:                                                                *
 *      AD值,转换通道                                                          *
 *  Return                                                                     *
 *      温度值                                                                 *
 *----------------------------------------------------------------------------*/
static	uint8	GetDegree(uint8 adData,uint8 u8CH)
{
	uint8	i=0; 
    
//	adData	= adData^0xff;
	
    switch(u8CH)
	{
	    case    AD_CH_BOT:
        case	AD_CH_TOP:
			//判断AD是否在查找范围内(0度到190度)
		    if((adData>=7)&&(adData<241))
			{
				//对分法查找相应温度
				i =	Dichotomy_Search((uint8*)TempTable3950_50K,190,adData);
			}
			else
			{
				if(adData<7)	 i = 0;
				if(adData>=241)  i = 190;
			}
		    break;
            
		default:
		    break;
	}

	return	i;
}

/*                     下面程序可移植                    */

/*-----------------------------------------------------------------------------*
 *  Function Description:                                                      *
 *      冒泡排序.                                                              *
 *  Parameters:                                                                *
 *      pDat需要排序的数据指针，u8Size需要排序的个数.                          *
 *  Return                                                                     *
 *      None                                                                   *
 *----------------------------------------------------------------------------*/
//void	Sort8(uint8	*pDat,uint8	u8Size)
//{
//	uint8	i,j,temp,tag=1;

//	for(i = 0; tag==1&&i<u8Size-1; i++)
//	{
//		tag = 0;
//		for(j=0; j<u8Size-i-1; j++)
//		{
//			if(pDat[j]>pDat[j+1])
//			{
//				temp	=	pDat[j];
//				pDat[j]  =  pDat[j+1];
//				pDat[j+1] = temp;	
//				tag = 1;				
//			}
//		}
//	}
//}

//void	Sort16(uint16 *pDat,uint8 u8Size)
//{
//	uint8	i,j,tag=1;
//	uint16	temp;

//	for(i = 0; tag==1&&i<u8Size-1; i++)
//	{
//		tag = 0;
//		for(j=0; j<u8Size-i-1; j++)
//		{
//			if(pDat[j]>pDat[j+1])
//			{
//				temp	=	pDat[j];
//				pDat[j]  =  pDat[j+1];
//				pDat[j+1] = temp;	
//				tag = 1;				
//			}
//		}
//	}
//}

/*-----------------------------------------------------------------------------* 				                         
 *  Function Description:                                                                    
 *  	有序数组二分查找算法
 *	Parameters: 
 * 		u8Array[]:被查找数组,  	n:被查找数组元素个数,	u8Key:被查找的关键值
 * 	Return:   
 *		对应的温度值
 *----------------------------------------------------------------------------*/
static	uint8	Dichotomy_Search(uint8 u8Array[],uint8 n,uint8 u8Key)    
{    
    uint8 		u8Low,u8High,u8Mid;    
    u8Low = 0;    
    u8High = n - 1;    
	
    while(1) {    
		u8Mid = (u8High+u8Low)/2;    
		if(u8Key>=u8Array[u8Mid]){    
		   if(u8Mid==u8High||u8Key < u8Array[u8Mid+1])
		    return(u8Mid);    
		}
		/*key>array[mid] 表明要求查找的值在[mid+1,high]*/   
		/*否则,在[low,mid-1]*/   
		if(u8Low==u8High)break;
		if(u8Key > u8Array[u8Mid])    
		    u8Low = u8Mid + 1;    
		else   
		    u8High = u8Mid - 1;    
	}
	return(0);    
}

/*-----------------------------------------------------------------------------*
 *  Function Description:                                                      *
 *      ADC采集处理	                                                           *
 *  Parameters:                                                                *
 *      None                                                                   *
 *  Return                                                                     *
 *      None                                                                   *
 *----------------------------------------------------------------------------*/
void	ADCProc(void)
{
	uint8	i,j;
	uint16	u16Tmp;

	u8ADBuf[AD_CH_BOT][u8ChgCnt] = AD_BottomTemperature;
    u8ADBuf[AD_CH_TOP][u8ChgCnt] = AD_TopTemperature;

    //滑动平均值滤波
	u8ChgCnt++;
	if(u8ChgCnt==AD_STAT_SIZE)
	{
		u8ChgCnt = u8ChgCnt-1; 
		//得到AD的最终值
		for(j=0;j<AD_CH_NUM;j++)
		{
			u16Tmp  =	0;
			for(i=0;i<AD_STAT_SIZE;i++)
			{
				u16Tmp	+=	(uint16)u8ADBuf[j][i];
			}
			u16Tmp = u16Tmp/AD_STAT_SIZE;
			g_u8ADValue[j] = (uint8)u16Tmp;		    //(AD值)
			g_u8ADResult[j]	= GetDegree((uint8)u16Tmp,j);	//(温度值)
		}
						
		for(j=0;j<AD_CH_NUM;j++)
		{
			for(i=0;i<AD_STAT_SIZE-1;i++)
			{
				u8ADBuf[j][i]	=	u8ADBuf[j][i+1];
			}
		} 
	}
}
