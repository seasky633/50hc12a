/*-----------------------------*
 *  include head files         *
 *----------------------------*/
#include ".\Head\HD_cfg.h"

/*-----------------------------*
 *  Macros for constants       *
 *----------------------------*/
//#define		AD_TEMP_SIZE		3		//��ʱ���ݻ���Ĵ�С
#define     AD_CH_NUM			2		//ͨ������
#define     AD_STAT_SIZE		8		//ÿ��ͨ����ջ�Ĵ�С

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
uint8	idata 	u8ADBuf[AD_CH_NUM][AD_STAT_SIZE];	//ADC������
//uint16	idata	u16AdTempBuf[AD_TEMP_SIZE];		    //��ʱ���ݻ���
//uint8	u8CurCH		=	0;							//��ǰͨ��
//uint8	u8CurCHID;									//��ǰͨ����Ӧ��ID
//uint8	u8ADCnt		=	0;							//����ת���Ĵ���
uint8	idata   u8ChgCnt =	0;						//ת������
uint8	idata	g_u8ADResult[AD_CH_NUM];			//ADת�����ս��(�¶�)
uint8	idata	g_u8ADValue[AD_CH_NUM];			//ADת�����ս��(AD)
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
//    u16AdTempBuf[u8ADCnt++] = u16AdTemp;	//��ת��������浽��ʱ��������
//	
//    if(u8ADCnt>=AD_TEMP_SIZE)           //�Ƿ������ɼ���(AD_TEMP_SIZE)��
//	{					
//		u8ADCnt  =	0;
//		bADOK	 =	1;		            //����ʱ�������Ѵ�������ʱ����ADOKλΪ1 
//		ADCCRL   =  0;    				//ֹͣת��
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
//	u8CurCH	= 0;        //u8CurCH : ��ǰͨ�����
//	u8CurCHID =	CHNO2ID[u8CurCH];	//u8CurCHID: ��ǰͨ��ID
//	ADC_Start(u8CurCHID);			//ѡ��ͨ��&��ʼת��
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
 *      ��ѯADת�����ս��.                                                    *
 *  Parameters:                                                                *
 *      ת��ͨ��                                                               *
 *  Return                                                                     *
 *      ADת�����ս��(�¶�)                                                   *
 *----------------------------------------------------------------------------*/
uint8	ADCGetData(uint8 u8CH)
{
	return	g_u8ADResult[u8CH];	
}

/*-----------------------------------------------------------------------------*
 *  Function Description:                                                      *
 *      ��ѯADת�����ս��.                                                    *
 *  Parameters:                                                                *
 *      ת��ͨ��                                                               *
 *  Return                                                                     *
 *      ADת�����ս��(AD)                                               	   *
 *----------------------------------------------------------------------------*/
uint8   ADCGetVal(uint8	u8CH)
{
	return	g_u8ADValue[u8CH];
}

/*-----------------------------------------------------------------------------*
 *  Function Description:                                                      *
 *      ADֵת�����¶�ֵ������.                                                *
 *  Parameters:                                                                *
 *      ADֵ,ת��ͨ��                                                          *
 *  Return                                                                     *
 *      �¶�ֵ                                                                 *
 *----------------------------------------------------------------------------*/
static	uint8	GetDegree(uint8 adData,uint8 u8CH)
{
	uint8	i=0; 
    
//	adData	= adData^0xff;
	
    switch(u8CH)
	{
	    case    AD_CH_BOT:
        case	AD_CH_TOP:
			//�ж�AD�Ƿ��ڲ��ҷ�Χ��(0�ȵ�190��)
		    if((adData>=7)&&(adData<241))
			{
				//�Էַ�������Ӧ�¶�
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

/*                     ����������ֲ                    */

/*-----------------------------------------------------------------------------*
 *  Function Description:                                                      *
 *      ð������.                                                              *
 *  Parameters:                                                                *
 *      pDat��Ҫ���������ָ�룬u8Size��Ҫ����ĸ���.                          *
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
 *  	����������ֲ����㷨
 *	Parameters: 
 * 		u8Array[]:����������,  	n:����������Ԫ�ظ���,	u8Key:�����ҵĹؼ�ֵ
 * 	Return:   
 *		��Ӧ���¶�ֵ
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
		/*key>array[mid] ����Ҫ����ҵ�ֵ��[mid+1,high]*/   
		/*����,��[low,mid-1]*/   
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
 *      ADC�ɼ�����	                                                           *
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

    //����ƽ��ֵ�˲�
	u8ChgCnt++;
	if(u8ChgCnt==AD_STAT_SIZE)
	{
		u8ChgCnt = u8ChgCnt-1; 
		//�õ�AD������ֵ
		for(j=0;j<AD_CH_NUM;j++)
		{
			u16Tmp  =	0;
			for(i=0;i<AD_STAT_SIZE;i++)
			{
				u16Tmp	+=	(uint16)u8ADBuf[j][i];
			}
			u16Tmp = u16Tmp/AD_STAT_SIZE;
			g_u8ADValue[j] = (uint8)u16Tmp;		    //(ADֵ)
			g_u8ADResult[j]	= GetDegree((uint8)u16Tmp,j);	//(�¶�ֵ)
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
