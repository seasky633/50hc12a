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
void	LEDDispAll(uint8 mode);
void	LEDDispNum_HI(uint8 hdigit);
void    LEDDispNum_LO(uint8 ldigit);
void	LEDDispTime(uint8 *pTime);
void	LEDDispTemp(uint8 temp);
void	LEDDispCookMode(uint8 cookmode,uint8 diymode);
void    LEDProc(uint8 sysid);
void    LEDClearFun(void);
void    LEDDispFoodType(uint8 type);
void  	LEDDispMovie(uint8 sequence);
void  	LEDClearNum(void);
void    LEDDispError(void);
void    LEDDispReady(void);
void    LEDDispSelfChk(uint8 step);
void	LEDDispPowerSaving(uint8 sequence);
//void    LEDDispDIYNum(uint8 num,uint8 sw);
#ifdef	DEBUG0
void	LEDDispMessage(uint8 msg,uint8 u8CH);
#endif

/*-----------------------------*
 *  static functions prototypes*
 *----------------------------*/
static	unsigned int HEX8toBCD(unsigned char hex_data);

/*-----------------------------*
 *  public variable declaration*
 *----------------------------*/
//          	SEG1	SEG2	SEG3	SEG4	SEG5	SEG6	SEG7	SEG8
//				bit0    bit1    bit2    bit3    bit4    bit5    bit6    bit7
//VBuf[0]		Veg     Jam     OmPdge  Pilaf   Pasta   Meat 	Poultry Fish
//VBuf[1]		Steam   Reheat  Dough   Cheese  Yogurt  Soup    Bake    Stew
//VBuf[2]	    Fry     DIY     Preset  Start   Warm    Temp    Timer   Rice
//VBuf[3]       1B		1C		/		1D		1E		1G		1A		1F
//VBuf[4]       2B		2C		2DP		2D		2E		2G		2A		2F
//VBuf[5]       3B		3C		3DP		3D		3E		3G		3A		3F
//VBuf[6]       4B		4C		/		4D		4E		4G		4A		4F

uint8 bdata	VBuf[7];   	//LED显存映射
sbit bLED_Dot1      = VBuf[4]^2;//":"up
sbit bLED_Dot2      = VBuf[5]^2;//":"down

sbit bLED_Veg       = VBuf[0]^0;//Vegetables
sbit bLED_Jam       = VBuf[0]^1;//Jam
sbit bLED_OmPorridge= VBuf[0]^2;//Oatmeal Porridge
sbit bLED_Pilaf     = VBuf[0]^3;//Pilaf/Risotto
sbit bLED_Pasta     = VBuf[0]^4;//Pasta/Pelmeni
sbit bLED_Meat      = VBuf[0]^5;//Meat
sbit bLED_Poultry   = VBuf[0]^6;//Poultry
sbit bLED_Fish      = VBuf[0]^7;//Fish

sbit bLED_Steam 	= VBuf[1]^0;//Steam
sbit bLED_Reheat    = VBuf[1]^1;//Reheat
sbit bLED_Dough     = VBuf[1]^2;//Dough
sbit bLED_Cheese    = VBuf[1]^3;//Cheese
sbit bLED_Yogurt    = VBuf[1]^4;//Yogurt
sbit bLED_Soup      = VBuf[1]^5;//Soup/Boiling
sbit bLED_Bake  	= VBuf[1]^6;//Bake
sbit bLED_Stew      = VBuf[1]^7;//Stew/Slowcook

sbit bLED_Fry	    = VBuf[2]^0;//Fry
sbit bLED_DIY   	= VBuf[2]^1;//DIY
sbit bLED_Preset	= VBuf[2]^2;//Preset
sbit bLED_Start     = VBuf[2]^3;//Start
sbit bLED_Warm      = VBuf[2]^4;//Warm
sbit bLED_Temp      = VBuf[2]^5;//Temperature
sbit bLED_Timer     = VBuf[2]^6;//Timer
sbit bLED_Rice      = VBuf[2]^7;//Rice/Cereal

uint8 xdata LEDBuf[7];
uint8 u8Com_Mode;
uint8 u8BlankCnt;
uint8 code LED_TAB[] = 
{
	Num0,Num1,Num2,Num3,Num4,Num5,Num6,Num7,
	Num8,Num9,Numa,Numb,Numh,Nump,Nume,Numd
};



/*-----------------------------------------------------------------------------*
 *  Function Description:                                                      *
 *      HEX转BCD                                                               * 
 *  Parameters:                                                                *
 *      hex_data(<0xff,>0)                                                     *
 *  Return                                                                     *
 *      bcd_data(<0x255,>0)                                                    *
 *----------------------------------------------------------------------------*/
static	unsigned int HEX8toBCD(unsigned char hex_data)
{
	unsigned int bcd_data;
	unsigned char temp;
	temp = hex_data%100;
	bcd_data = ((unsigned int)hex_data)/100<<8;
	bcd_data = bcd_data|temp/10<<4;
	bcd_data = bcd_data|temp%10;
	return bcd_data;
}

/*-----------------------------------------------------------------------------*
 *  Function Description:                                                      *
 *      ALL LED显示模式                                                        *
 *  Parameters:                                                                *
 *      None                                                                   *
 *  Return                                                                     *
 *      None                                                                   *
 *----------------------------------------------------------------------------*/
void	LEDDispAll(uint8 mode)
{	
	uint8	i;
			
	for(i=0;i<7;i++)
	{
		if(mode>0)
		{
			VBuf[i]	= 0xff;
		}
		else
		{
			VBuf[i]	= 0x00;
		}
	}
}	

/*-----------------------------------------------------------------------------*
 *  Function Description:                                                      *
 *      显示数字高两位D4-D3                                                    *
 *  Parameters:                                                                *
 *      hdigit                                                                 *
 *  Return                                                                     *
 *      None                                                                   *
 *----------------------------------------------------------------------------*/
void	LEDDispNum_HI(uint8	hdigit)
{
	uint8	i;

	VBuf[6]	 =	0x00;						//清LED缓冲区	
	VBuf[5]	&=	0x04;

	i	=	hdigit & 0x0f;	  				//显示低位D3
	VBuf[5]	|=	LED_TAB[i];  				
   	
	i   =	(hdigit>>4) & 0x0f;             //显示高位D4
	if(i)	VBuf[6]	=	LED_TAB[i];  		
}

/*-----------------------------------------------------------------------------*
 *  Function Description:                                                      *
 *      显示数字低两位D2-D1                                                    *
 *  Parameters:                                                                *
 *      ldigit                                                                 *
 *  Return                                                                     *
 *      None                                                                   *
 *----------------------------------------------------------------------------*/
void	LEDDispNum_LO(uint8	ldigit)
{
	uint8	i;

    if(ldigit==0xff)
    {
        VBuf[4] = Numf;                         //显示"FF"
        VBuf[3] = Numf;
    }
    else
    {
    	VBuf[4]	&=	0x04;						//清LED缓冲区	
    	VBuf[3]	=	0x00;

    	i	=	ldigit & 0x0f;	  				//显示低位D1
    	VBuf[3]	=	LED_TAB[i];  				

    	i   =	(ldigit>>4) & 0x0f;             //显示高位D2
    	VBuf[4]	|=	LED_TAB[i];  				
    }
}

/*-----------------------------------------------------------------------------*
 *  Function Description:                                                      *
 *      清除功能显示                                                           *
 *  Parameters:                                                                *
 *      None                                                                   *
 *  Return                                                                     *
 *      None                                                                   *
 *----------------------------------------------------------------------------*/
void  LEDClearFun(void)
{
	VBuf[0]	=	0x00;						//Veg/Jam/OmPdge/Pilaf/Pasta/Meat/Poultry/Fish
	VBuf[1]	=	0x00;						//Steam/Reheat/Dough/Cheese/Yogurt/Soup/Bake/Stew
	VBuf[2]	&=	0x18;						//Fry/DIY/Preset/Start/Warm/Temp/Timer/Rice
}

/*-----------------------------------------------------------------------------*
 *  Function Description:                                                      *
 *      清除数字显示                                                           *
 *  Parameters:                                                                *
 *      None                                                                   *
 *  Return                                                                     *
 *      None                                                                   *
 *----------------------------------------------------------------------------*/
void  	LEDClearNum(void)
{
	VBuf[6]	 =	0x00;						
	VBuf[5]	&=	0x04;
	VBuf[4]	&=	0x04;						
	VBuf[3]	=	0x00;
}

#ifdef	DEBUG0
/*-----------------------------------------------------------------------------*
 *  Function Description:                                                      *
 *      调试信息显示(温度或AD值)                                          	   *
 *  Parameters:                                                                *
 *      msg(需要显示的信息),u8CH(通道号)                                       *
 *  Return                                                                     *
 *      None                                                                   *
 *----------------------------------------------------------------------------*/
void	LEDDispMessage(uint8 msg,uint8 u8CH)
{
	uint8	i;

	u8CH = u8CH+1;

	LEDClearNum();								//清除数字区显示

	bLED_Dot1	=	0;							//clr Dot1
	bLED_Dot2	=	0;		  					//clr Dot2
	
	i	=	LOBYTE(HEX8toBCD(msg));		  		//显示信息低二位
	LEDDispNum_LO(i);

	i	=	HIBYTE(HEX8toBCD(msg)|(u8CH<<12));	//显示信息高二位和通道号
	LEDDispNum_HI(i);
}
#endif

/*-----------------------------------------------------------------------------*
 *  Function Description:                                                      *
 *      报警显示                                          					   *
 *  Parameters:                                                                *
 *      None                                                                   *
 *  Return                                                                     *
 *      None                                                                   *
 *----------------------------------------------------------------------------*/
void	LEDDispError(void)
{
	uint8	i;

	for(i=0;i<7;i++)			// LED Buf全部清零
	{
		VBuf[i]	= 0;
	}

	if(bTopSensorErr)			//显示E0-顶部传感器开路或短路
	{
		LEDDispNum_LO(0xe0);
		return;
	}

	if(bBotSensorErr) 			//显示E1-底部传感器开路或短路
	{
		LEDDispNum_LO(0xe1);
		return;
	}

 	if(bPanOverTempErr)	 		//显示E3-锅底温度过高
 	{
		LEDDispNum_LO(0xe3);
 		return;
    }	

    if(bNoPanErr)               //显示E5-无锅
    {
        LEDDispNum_LO(0xe5);
        return;
    }
}

/*-----------------------------------------------------------------------------*
 *  Function Description:                                                      *
 *      食物类型显示                                          				   *
 *  Parameters:                                                                *
 *      type                                                                   *
 *  Return                                                                     *
 *      None                                                                   *
 *----------------------------------------------------------------------------*/
void    LEDDispFoodType(uint8 type) 
{
	bLED_Veg = 0;
	bLED_Fish = 0;
	bLED_Poultry = 0;
	bLED_Meat =	0;

	switch(type)
	{
		case	1:
			bLED_Veg = 1;
			break;
		case	2:
			bLED_Fish = 1;
			break;
		case	3:
			bLED_Poultry = 1;
			break;
		case	4:
			bLED_Meat = 1;
			break;
		default:
			break;
	}	
}
  
/*-----------------------------------------------------------------------------*
 *  Function Description:                                                      *
 *      烹饪模式显示                                         				   *
 *  Parameters:                                                                *
 *      cookmode                                                               *
 *  Return                                                                     *
 *      None                                                                   *
 *----------------------------------------------------------------------------*/
void	LEDDispCookMode(uint8 cookmode,uint8 diymode)
{	
	bLED_Soup = 0;
    bLED_Bake = 0;
    bLED_Stew = 0;
    bLED_Steam = 0;
    bLED_Fry = 0;
    bLED_Rice = 0;
    bLED_Pilaf = 0;
    bLED_OmPorridge = 0;
    bLED_Jam = 0;
    bLED_Pasta = 0;
    bLED_Yogurt = 0;
    bLED_Cheese = 0;
    bLED_Dough = 0;
    bLED_Reheat = 0;
    bLED_DIY = 0;

    if(diymode>0)    //diy
    {
        bLED_DIY = 1;
    }
    
	switch(cookmode)
	{
        case	1:	 		
	        bLED_Soup = 1;
            break;	
        case	2:			
            bLED_Bake = 1;
            break;			
        case	3: 			
            bLED_Stew = 1;
            break;
     	case	4:		    
			bLED_Steam = 1;
			break;
        case	5:	        
            bLED_Fry = 1;
			break;
		case	6:	        
			bLED_Rice = 1;
			break;
		case	7:         
			bLED_Pilaf = 1;	
			break;	
		case	8:			
			bLED_OmPorridge = 1;	
			break;
		case	9:			
			bLED_Jam = 1;	
			break;	
		case	10:			
			bLED_Pasta= 1;	
			break;
		case    11:			
			bLED_Yogurt = 1;
		    break;
		case    12:			
			bLED_Cheese = 1;
			break;
		case    13:			
			bLED_Dough= 1;
			break;
		case 	14:			
			bLED_Reheat = 1;
			break;
		default:
			break;
    }
}

/*-----------------------------------------------------------------------------*
 *  Function Description:                                                      *
 *      待机显示                                        					   *
 *  Parameters:                                                                *
 *      None                                                                   *
 *  Return                                                                     *
 *      None                                                                   *
 *----------------------------------------------------------------------------*/
void	LEDDispReady(void)
{
	uint8	i;

	for(i=0;i<7;i++)			// LED Buf全部清零
	{
		VBuf[i]	=	0;
	}
	
	LEDDispNum_HI(0xAA);		//显示"----"
	LEDDispNum_LO(0xAA);
}

/*-----------------------------------------------------------------------------*
 *  Function Description:                                                      *
 *      OFF模式省电显示                                        				   *
 *  Parameters:                                                                *
 *      None                                                                   *
 *  Return                                                                     *
 *      None                                                                   *
 *----------------------------------------------------------------------------*/
void	LEDDispPowerSaving(uint8 sequence)
{
	VBuf[0]	= 0;
    VBuf[1] = 0;
    VBuf[2] = 0;
    
    switch(sequence)
    {
        case	0: 
            VBuf[6] = Numa;
            VBuf[5] = Numblk;
            VBuf[4] = Numblk;
            VBuf[3] = Numblk;
            break;
        case    1:
            VBuf[6] = Numblk;
            VBuf[5] = Numa;
            break;
        case    2:
            VBuf[5] = Numblk;
            VBuf[4] = Numa;
            break;
        case    3:
            VBuf[4] = Numblk;
            VBuf[3] = Numa;
            break;
        case    4:
            VBuf[4] = Numa;
            VBuf[3] = Numblk; 
            break;
        case    5:
            VBuf[5] = Numa;
            VBuf[4] = Numblk;
            break;
        case    6:
            VBuf[6] = Numa;
            VBuf[5] = Numblk;
            break;
        default:
            break;
    }
}

/*-----------------------------------------------------------------------------*
 *  Function Description:                                                      *
 *      LED旋转方框                                         				   *
 *  Parameters:                                                                *
 *      sequence                                                               *
 *  Return                                                                     *
 *      None                                                                   *
 *----------------------------------------------------------------------------*/
void  	LEDDispMovie(uint8 sequence)
{
	switch(sequence)
	{
		case	0:         
			//C4		 (FAGED/CB)
			VBuf[6]	= Bin(11011000);
			//C3
			VBuf[5]	= Bin(01001000);	
			//C2
			VBuf[4]	= Bin(01001000);
			//C1
			VBuf[3]	= Bin(01001011);
			break;
		case	1:		
			VBuf[6]	= Bin(10011000);
			break;
		case	2:
			VBuf[6]	= Bin(11011000);
			VBuf[5]	= Bin(00001000);	
			break;
		case	3:
			VBuf[5]	= Bin(01001000);	
			VBuf[4]	= Bin(00001000);	
			break;
		case	4:
			VBuf[4]	= Bin(01001000);	
			VBuf[3]	= Bin(00001011);
			break;
		case	5:
			VBuf[3]	= Bin(01001010);
			break;
		case	6:
			VBuf[3]	= Bin(01001001);
			break;
		case	7:
			VBuf[3]	= Bin(01000011);
			break;
		case	8:
			VBuf[3]	= Bin(01001011);
			VBuf[4]	= Bin(01000000);
			break;
		case	9:
			VBuf[4]	= Bin(01001000);
			VBuf[5]	= Bin(01000000);
			break;
		case	10:
			VBuf[5]	= Bin(01001000);
			VBuf[6]	= Bin(11010000);
			break;
		case	11:
			VBuf[6]	= Bin(11001000);
			break;
		case	12:
			VBuf[6]	= Bin(01011000);
			break;
		default:
			break;
	}
}

/*-----------------------------------------------------------------------------*
 *  Function Description:                                                      *
 *      LED时间显示                                        				       *
 *  Parameters:                                                                *
 *      需要显示的时间地址指针                                                 *
 *  Return                                                                     *
 *      None                                                                   *
 *----------------------------------------------------------------------------*/
void	LEDDispTime(uint8	*pTime)
{
	uint8	i;

	i	=	LOBYTE(HEX8toBCD(pTime[1]));
	LEDDispNum_LO(i);				//显示分钟

	i	=	LOBYTE(HEX8toBCD(pTime[2]));
	LEDDispNum_HI(i);				//显示小时
}

/*-----------------------------------------------------------------------------*
 *  Function Description:                                                      *
 *      LED温度显示                                        				       *
 *  Parameters:                                                                *
 *      需要显示的温度值                                                       *
 *  Return                                                                     *
 *      None                                                                   *
 *----------------------------------------------------------------------------*/
void	LEDDispTemp(uint8 temp)
{
	uint8 i;
	
	LEDClearNum();							//清除数字区显示
	bLED_Timer = 0;	 						//CLR Timer
	bLED_Preset = 0;						//CLR Preset
	bLED_Temp = 1;							//set Temperature
	bLED_Dot1 =	0;							//clr Dot1
	bLED_Dot2 =	0;		  					//clr Dot2
	
	i =	LOBYTE(HEX8toBCD(temp));		  	//显示温度低二位
	LEDDispNum_LO(i);
	i = HIBYTE(HEX8toBCD(temp));			//显示温度高二位
	LEDDispNum_HI(i);
	if(temp<100)
	{
		VBuf[6]	 =	0x00;					//温度小于100度只显示低两位	
		VBuf[5]	 =	0x00;
	}
}

/*-----------------------------------------------------------------------------*
 *  Function Description:                                                      *
 *      DIY切换显示(d1,d2,d3)                                       		   *
 *  Parameters:                                                                *
 *      DIY Number                                                             *
 *  Return                                                                     *
 *      None                                                                   *
 *----------------------------------------------------------------------------*/
// void   LEDDispDIYNum(uint8 num,uint8 sw)
// {
//    if((num>0)&&(num<5))
//    {
//        LEDClearNum();						    //清除数字区显示  
//        bLED_Timer = 0;	 						//CLR Timer
//        bLED_Preset = 0;                        //CLR Preset
//        bLED_Temp = 0;                          //CLR Temperature
//        bLED_Dot1 =	0;							//clr Dot1
//	    bLED_Dot2 =	0;		  					//clr Dot2
//	    if(num==1)
//        {
//            LEDDispNum_LO(0xf1);
//        }
//        if(num==2)
//        {
//            LEDDispNum_LO(0xf2);
//        }
//        if(num==3)
//        {
//            LEDDispNum_LO(0xf3);
//        }
//        if(num==4)
//        {       
//            LEDDispNum_HI(0xf1);
//            LEDDispNum_LO(0xf2);
//            if(sw==1)
//            {
//                if(u8BlankCnt>=63)
//                {
//                    LEDDispNum_HI(0xf1);
//                }
//                else
//                {
//                   	VBuf[6]	 =	0x00;						
//	                VBuf[5]	 =	0x00;
//                }
//            }
//            if(sw==2)
//            {
//                if(u8BlankCnt>=63)
//                {
//                    LEDDispNum_LO(0xf2);
//                }
//                else
//                {
//                    VBuf[4]	=	0x00;						
//	                VBuf[3]	=	0x00;
//                }
//            }
//        }
//    }
// }

/*-----------------------------------------------------------------------------*
 *  Function Description:                                                      *
 *      按步骤显示自检内容                                        			   *
 *  Parameters:                                                                *
 *      自检步骤                                                               *
 *  Return                                                                     *
 *      None                                                                   *
 *----------------------------------------------------------------------------*/
 void    LEDDispSelfChk(uint8 step)
{
    uint8	u8BotDegree	=	ADCGetData(AD_CH_BOT);
	uint8   u8TopDegree =   ADCGetData(AD_CH_TOP);
    
    LEDDispAll(0);                      //清除所有显示
    switch(step)
    {
        case    0:
            LEDDispNum_HI(0x12);	 	//显示序列号--Plug in & Long press"start"
			LEDDispNum_LO(0x04);    
            break;
        case    1:
            LEDDispAll(1);              //LED全显--Press">"
            break;
        case    2: 
            LEDDispNum_LO(0xB1);        //显示"b1"--Press"cancel"
            break;                      
        case    3:                                           
            LEDDispNum_LO(0xB2);        //显示"b2"--Press"Timer"
            break; 
        case    4:
            LEDDispNum_LO(0xB3);        //显示"b3"--Press"Preset"
            break;
        case    5:
            LEDDispNum_LO(0xB4);        //显示"b4"--Press"<"
            break;
        case    6:
            LEDDispNum_LO(0xB5);        //显示"b5"--Press"DIY"
            break;
        case    7:
            LEDDispNum_LO(0xB6);        //显示"b6"--Press">"
            break;
        case    8:
            LEDDispNum_LO(0xB7);        //显示"b7"--Press"Menu"
            break;
        case    9:
            LEDDispTemp(u8BotDegree);   //显示"temperature of bottom NTC"--Press"Menu"
            break;
        case    10:
            LEDDispTemp(u8TopDegree);   //显示"temperature of top NTC"--Press"Menu"
            break;
        case    11:
            LEDDispNum_LO(0xC1);        //显示"H1"--Press"Menu"
            break;
        case    12:
            LEDDispNum_HI(0x00);        //显示"OFF"--Press"Menu"
            LEDDispNum_LO(0xFF);
            break;
    }
}

/*-----------------------------------------------------------------------------*
 *  Function Description:                                                      *
 *      LED处理(功能菜单闪烁控制&&状态指示灯控制)                              *
 *  Parameters:                                                                *
 *      None                                                                   *
 *  Return                                                                     *
 *      None                                                                   *
 *----------------------------------------------------------------------------*/
void    LEDProc(uint8 sysid)
{
	uint8 i;
	
	u8BlankCnt++;

	if(u8BlankCnt>=63)
	{
		if(u8BlankCnt>=126)
		{
			u8BlankCnt = 0;
		}    
	}

	//状态指示灯控制
	switch(sysid)
	{
		case	0:					//复位
			LEDDispAll(1);
			break;
		
		case    1:					//待机
		case    5:                  //OFF模式
        case	6:					//报警
			bLED_Start = 0;
			bLED_Warm = 0;
			break;    
		
		case	2:					//设置
			bLED_Start = 0;
			bLED_Warm =	0;
			if(u8BlankCnt >= 63)
			{
				bLED_Start = 1;
				if(bWarmLedBlank) bLED_Warm =1;
			}
			break;

	   	case	3:	 				//运行
			bLED_Start = 1;	
			bLED_Warm =	0;
			break;

		case	4:					//保温
			bLED_Start = 0;
			bLED_Warm =	1; 
			break;	
			
	    case    7:					//自检
            LEDDispSelfChk(m_u8SelfChkStep);
			break;
			
		default:	
			break;	
	}
	
	for(i=0;i<7;i++)
	{
		LEDBuf[i] = VBuf[i];
	}
}

