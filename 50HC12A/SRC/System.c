/*-----------------------------*
 *  include head files         *
 *----------------------------*/
#include ".\Head\HD_cfg.h"
#include ".\Head\System.h"
#include ".\Head\Time.h"
#include ".\Head\TempCtr.h"
#include ".\Head\Script.h"

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
void	UIProc(void);
void	FaultCheckProc(void);
void    AutotestActivate(void);
void	EnterReset(void);
void	EnterReady(void);
void	EnterWarm(void);
void    EnterPowerDown(void);

/*-----------------------------*
 *  static functions prototypes*
 *----------------------------*/
static	void	ResetProc(void);
static	void	ReadyProc(void);
static  void    SetupProc(void);
static	void	RunProc(void);
static	void	JudgeTimeValid(void);
static  void    RestoreDispMenu(void);
static	void	DispDefCookTime(uint8 CookMode,uint8 DIYMode,uint8 FoodType);
static  void    SetDefCookTemp(uint8 CookMode,uint8 DIYMode,uint8 FoodType); 
static  void    SetCookModeByMenu(void);
static  void    SetDIYModeByDIY(uint8 diymode,uint8 diyswitch);
static  void	WarmProc(void);
static  void    PowerDownProc(void);
static	void	ErrorProc(void);
static	void	SelfCheckProc(void);

/*-----------------------------*
 *  public variable declaration*
 *----------------------------*/
uint8	m_u8SysID;	  				//系统状态变量
uint8   m_u8PrevSysID;  			//掉电之前系统状态变量
uint8   m_u8CookMode;     			//烹饪模式，0 无效，1 Soup 2 Bake 3 Stew 4 Steam 5 Fry
									//			     6 Rice 7 Pilaf 8 OPorridge 9 Jam 10 Pasta
                                    //               11 Yogurt 12 Cheese 13 Dough 14 Reheat 
uint8   m_u8DIYMode;                //DIY模式:DIY1,DIY2,DIY3,...
uint8   m_u8DIYSwitch;              //DIY模式4调整切换:0 不调整  1 DIY1  2 DIY2
uint8   m_u8DIYCookStage;           //DIY模式4运行阶段:0 结束    1 DIY1  2 DIY2
uint8   m_u8FoodType;      			//食物模式，0 无效，1 Vegetables  2 Fish  3 Poultry  4 Meat
uint8	m_u8PreTmp;					//温度初始值
uint8	m_u8TimeDispCnt;			//时间闪烁显示计数器
uint8	m_u8MovCnt;
uint8   m_u8ChkCnt;				
uint8	m_u8CurTimeDispCnt;
uint8	m_DataTmp;					//需要显示的数据
uint8   u8CookTemp;				    //烹饪温度
uint8   MaxCookTemp;				//最高的烹饪温度
uint8   DefCookTemp;                //默认的烹饪温度
uint8   MinCookTemp;                //最低的烹饪温度
uint8	tmMaxCookTime[3];			//最长的烹饪时间
uint8	tmDefCookTime[3];			//默认的烹饪时间
uint8	tmMinCookTime[3];			//最短的烹饪时间
uint8	tmCalCookTime[3];			//预约结束,烹饪开始时间
uint8	tmIntCookTime[3];			//间隔时间==预约设定时间-当前时间
uint8   idata  tmMinBookTime[3];	//最短的预约时间
uint8	idata  u8CHDispCnt[2];      //通道显示计数器
uint8   idata  m_u8DispDelayCnt;    //显示延时计数器
uint8   idata  m_u8AutoRunCnt;		//自动运行计数器
uint8   idata  u8ErrorCnt;          //报警计数器
uint8   idata  u8NoPanCnt;          //无锅计数器
uint8   idata  u8VoltageCnt;        //电压计数器
uint8   xdata  u16IGBTCnt;          //IGBT热敏开路计数器
uint8   xdata  m_u8SelfChkStep;		//自检步骤
uint16  xdata  u16PoweronCnt;       //上电自检计数器
uint16  xdata  u16DispDIYCnt;       //display diy xx 2s
uint8	xdata  m_u8DIYCookMode[3];	//复制到DIY的烹饪模式
uint8   xdata  m_u8DIYFoodType[3];  //复制到DIY的食物模式
uint8   xdata  tmDIY1CookTime[3];   //复制到DIY1的烹饪时间
uint8   xdata  u8DIY1CookTemp;      //复制到DIY1的烹饪温度
//uint8   xdata  tmDIY2CookTime[3];   //复制到DIY2的烹饪时间
//uint8   xdata  u8DIY2CookTemp;      //复制到DIY2的烹饪温度
//uint8   xdata  tmDIY3CookTime[3];   //复制到DIY3的烹饪时间
//uint8   xdata  u8DIY3CookTemp;      //复制到DIY3的烹饪温度
uint16  xdata  m_u16Movcnt;
uint8   xdata  u8BolingWaterTest;     //煲水测试模式
//烹饪时间表					       Soup     Bake     Stew     Steam    Frying   OmPdge   Jam
uint16  code	MinCookTime_Table[]	= {0x000A,  0x0005,  0x0014,  0x0005,  0x0005,  0x0005,  0x0005,      
//                                     Pasta    Yoghurt  Cheese   Dough    Reheat   DIY
                                       0x0003,  0x0100,  0x001E,  0x000A,  0x0005,  0x0005}; 
//                                     Soup     Bake     Stew     Steam    Frying   OmPdge   Jam
uint16	code	DefCookTime_Table[]	= {0x0028,	0x0023,  0x0028,  0x0023,  0x0014,	0x0019,	 0x0100,
//                                     Pasta    Yoghurt  Cheese   Dough    Reheat   DIY
                                       0x000A,  0x0800,  0x0200,  0x0100,  0x0019,  0x001E};
//                                     Soup     Bake     Stew     Steam    Frying   OmPdge   Jam
uint16	code	MaxCookTime_Table[]	= {0x0400,	0x0400,  0x0900,  0x0300,  0x011E,  0x0200,  0x0300,
//									   Pasta    Yoghurt  Cheese   Dough    Reheat   DIY
                                       0x0300,  0x0C00,  0x0400,  0x0600,  0x0100,  0x0300};
//                                           Veg	  Fish     Poultry  Meat
uint16  code    DefFoodTime_Table[4][4] = { {0x0028,  0x002D,  0x0032,  0x0100},     //Soup
											{0x0023,  0x0014,  0x0028,  0x0032},	 //Bake
                                            {0x0028,  0x0028,  0x0100,  0x0200},     //Stew
                                            {0x0023,  0x0019,  0x002D,  0x0037} };   //Steam
                                          
//烹饪温度表						   Bake     Soup     Fry      Stew     Porridge  Reheat  DIY
uint8	code 	MinCookTemp_Table[]	= {0x8C,    0x5A,    0x78,    0x50,    0x5A,     0x50,   0x28};
uint8   code    DefCookTemp_Table[] = {0x8C,    0x64,    0xA0,    0x64,    0x64,     0x64,   0x64};
uint8   code    MaxCookTemp_Table[] = {0xA0,    0x6E,    0xA0,    0x6E,    0x64,     0x64,   0xA0};
//                                     Veg      Fish     Poultry  Meat
uint8   code    DefFoodTemp_Table[] = {0x8C,    0xA0,    0xA0,    0xA0};
/*-----------------------------*
 *  static variable declaration*
 *----------------------------*/



/*-----------------------------------------------------------------------------*
 *  Function Description:                                                      *
 *      人机界面处理                                    					   *
 *  Parameters:                                                                *
 *      None                                                                   *
 *  Return                                                                     *
 *      None                                                                   *
 *----------------------------------------------------------------------------*/
void	UIProc(void)
{
	switch(m_u8SysID)
	{
		case	SYS_ID_RESET:
			ResetProc();
			break;
		
		case    SYS_ID_READY:
			ReadyProc();
		    break;
			
        case    SYS_ID_SETUP:
		    SetupProc();
            break;

		case 	SYS_ID_RUN:
			RunProc();
			break;

        case    SYS_ID_WARM:
		    WarmProc();
			break;
            
        case    SYS_ID_POWRDOWN:
            PowerDownProc();
            break;
            
        case    SYS_ID_ERROR:
		    ErrorProc();
			break;
			
        case	SYS_ID_CHECK:
			SelfCheckProc();
			break;
			
		default:
			break;	
	}
}


/*-----------------------------------------------------------------------------*
 *  Function Description:                                                      *
 *      	复位状态处理                                     				   *
 *  Parameters:                                                                *
 *      None                                                                   *
 *  Return                                                                     *
 *      None                                                                   *
 *----------------------------------------------------------------------------*/
static void	ResetProc(void)
{
 	if(bSys_1000MS)
	{
        if(rEepromCache[EEP_MUSIC_EN]==0xA5)
	    {
		    bKeyMusic_EN = 0;       //关闭按键声音
	    }
	    else
	    {
		    bKeyMusic_EN = 1;		
	    }
        EnterReady();    
	}								
}

/*-----------------------------------------------------------------------------*
 *  Function Description:                                                      *
 *      	待机状态处理                                     				   *
 *  Parameters:                                                                *
 *      None                                                                   *
 *  Return                                                                     *
 *      None                                                                   *
 *----------------------------------------------------------------------------*/
static void ReadyProc(void)
{
	uint8 u8Key;
	u8Key = GetKey();
		
	if(TstBit(u8Key,BIT_KEY_WARM))
	{
		if(IsNewKey())
		{
			EnterWarm(); 
			return;
		}
	}

    if(TstBit(u8Key,BIT_KEY_START))
    {
        if(IsNewKey())
        {
        
        }
        
        if((u8Key==VAL_KEY_START)&&(g_u16KeyRpyCnt==20))	//长按3S进入OFF模式
        {
            EnterPowerDown();
            PlaySound(MusicPoweron);
            return;
        }
    }

	if(TstBit(u8Key,BIT_KEY_MENU))
	{
		if(IsNewKey())
		{
			m_u8SysID =	SYS_ID_SETUP;
			m_u8CookMode = FUNC_ID_SOUP;     
            m_u8DIYMode = 0;                    
			m_u8FoodType = 1;     				//Vegetables   
			LEDClearFun();
			LEDDispFoodType(m_u8FoodType); 
			DispDefCookTime(m_u8CookMode,m_u8DIYMode,m_u8FoodType);	
			SetDefCookTemp(m_u8CookMode,m_u8DIYMode,m_u8FoodType);
			tmCookTime[0] =	59;
            tmCookTime[1] =	tmDefCookTime[1];	//默认烹饪时间
			tmCookTime[2] =	tmDefCookTime[2];
			tmBookTime[0] =	59;					//清预约时间
			tmBookTime[1] =	0;
			tmBookTime[2] =	0;
			u8CookTemp = DefCookTemp;			    
			LEDDispCookMode(m_u8CookMode,m_u8DIYMode);	    //显示烹饪模式
			m_u8DispDelayCnt = 60;
			return;
		}
	}
	
	if(TstBit(u8Key,BIT_KEY_DIY))
	{
		if(IsNewKey())
		{
			m_u8SysID =	SYS_ID_SETUP;
            m_u8DIYMode = 1;            //DIY1
            m_u8DIYSwitch = 0;
            SetDIYModeByDIY(m_u8DIYMode,m_u8DIYSwitch); 
            m_u8DispDelayCnt = 60;
            return;
		}	
	}

    if(u8Key>0) 
    {
        m_u8DispDelayCnt = 60;
    }
	
	g_u8HiTemp = 0;			//温度范围无效,不加热
	bTimeDisp_EN = 0;		//时间不允许调整
	bTempDisp_EN = 0;		//温度不允许调整
	bBookTime_EN =	0;		//取消预约
	bCalBookTime_EN	= 0;	//取消预约计算

    if(m_u8DispDelayCnt>0)
    {
        if(bSys_1000MS) m_u8DispDelayCnt--;
        if(m_u8DispDelayCnt==0)
        {
            EnterPowerDown();
            LEDDispPowerSaving(0);
            m_u16Movcnt = 0;
            m_u8DispDelayCnt = 240; //延迟4MIN进入OFF模式
        }
        else
        {
	        LEDDispReady();		//待机显示"----"
	    }
	}
}

/*-----------------------------------------------------------------------------*
 *  Function Description:                                                      *
 *      设置状态处理                                     					   *
 *  Parameters:                                                                *
 *      None                                                                   *
 *  Return                                                                     *
 *      None                                                                   *
 *----------------------------------------------------------------------------*/
static	void	SetupProc(void)
{
	uint8	u8Key;	
    uint8 code *p_EepRom;
	
	p_EepRom = EEP_SECTOR_ADDR;
	u8Key =	GetKey();

    if(g_u8KeyUpAct==VAL_KEY_CANCEL)
    {
        if(bKeyPress==0)        //按键释放
        {
            g_u8KeyUpAct = 0;   //清除抬起有效键值      
            EnterReady();
            return;
        }
    }
    
	if(TstBit(u8Key,BIT_KEY_WARM))
	{
		if(g_u16KeyRpyCnt==36)						//长按5S
		{
		    g_u8KeyUpAct = 0;
			bWarm_EN ^= 1;
			PlaySound(MusicPoweron);	
		}
  	}

	if(TstBit(u8Key,BIT_KEY_MENU))
	{
		if((IsNewKey())&&(u16DispDIYCnt==0))
		{
	        if(m_u8CookMode>=14)
			{
		        m_u8CookMode = 1;
	        }
			else
			{
		        m_u8CookMode++;
	        }
            SetCookModeByMenu();
		}
		
		if((u8Key==VAL_KEY_MENU)&&(g_u16KeyRpyCnt==36))				//长按5S						
		{
			bKeyMusic_EN ^= 1;
			if(bKeyMusic_EN)
			{
				rEepromCache[EEP_MUSIC_EN] = 0;	
			}
			else
			{
				rEepromCache[EEP_MUSIC_EN] = 0xA5;
			}  
			EEPROM_Process();	
			PlaySound(MusicPoweron);
		}
	}			

	if((u8Key==VAL_KEY_PLUS)&&(u16DispDIYCnt==0))
	{
		if((bTimeDisp_EN==0)&&(bTempDisp_EN==0)&&(bBookTime_EN==0))	
		{
			if(IsNewKey())
			{
				if((m_u8CookMode==FUNC_ID_SOUP)||(m_u8CookMode==FUNC_ID_BAKE)||(m_u8CookMode==FUNC_ID_STEW)
				||(m_u8CookMode==FUNC_ID_STEAM))
				{
					if(m_u8FoodType>=4)  m_u8FoodType = 1;
					else m_u8FoodType++;
					LEDDispFoodType(m_u8FoodType);	
					DispDefCookTime(m_u8CookMode,m_u8DIYMode,m_u8FoodType);	
					SetDefCookTemp(m_u8CookMode,m_u8DIYMode,m_u8FoodType);	
					tmCookTime[1] =	tmDefCookTime[1];	//默认烹饪时间
					tmCookTime[2] =	tmDefCookTime[2];
					u8CookTemp = DefCookTemp;			//默认烹饪温度
				}          
			}
		}
		else
		{
			if(bTimeDisp_EN==1)							//烹饪时间是否允许调整
			{
				if((IsNewKey())||(g_u16KeyRpyCnt>15))
				{
					//判断烹饪时间是否溢出 
					if((tmCookTime[2]==tmMaxCookTime[2])&&(tmCookTime[1]==tmMaxCookTime[1]))
					{
						tmCookTime[2] =	tmMinCookTime[2];
						tmCookTime[1] =	tmMinCookTime[1];
					}
					else
					{
						if((m_u8CookMode==FUNC_ID_FRY)||(m_u8CookMode==FUNC_ID_PASTA))				
						{
							tmCookTime[1]++;
						}
						else 
						{
							if(m_u8CookMode==FUNC_ID_YOGHURT)
							{
								tmCookTime[1] += 15;
							}
							else
							{
							    tmCookTime[1] += 5; 
							}
						}	
					}
				    JudgeTimeValid();		//判断时间是否在合理范围内
				    m_u8TimeDispCnt = 62;					
				}
			}	
					
			if(bBookTime_EN)			//预约时间是否允许调整
			{
				if((IsNewKey())||(g_u16KeyRpyCnt>15))
				{
					//判断预约时间是否溢出
					if((tmBookTime[2]==24)&&(tmBookTime[1]==0))
					{
						tmBookTime[2] =	tmMinBookTime[2];
						tmBookTime[1] =	tmMinBookTime[1];
					}
					else
					{   
						tmBookTime[1] = (tmBookTime[1]/5)*5+5;
					}
				}
				JudgeTimeValid();		//判断时间是否在合理范围内
				m_u8TimeDispCnt = 62;
			}
			
			if(bTempDisp_EN)			//温度是否允许调整
			{
				if((IsNewKey())||((g_u16KeyRpyCnt>15)&&(g_u16KeyRpyCnt%3==0)))
				{
					//判断烹饪温度是否溢出
					if(u8CookTemp==MaxCookTemp)
					{
						u8CookTemp = MinCookTemp;
					}
					else
					{
                        if((m_u8CookMode==FUNC_ID_BAKE)||(m_u8CookMode==FUNC_ID_FRY)||(m_u8CookMode==FUNC_ID_OPORRIDGE)
                        ||(m_u8CookMode==FUNC_ID_SOUP)||(m_u8CookMode==FUNC_ID_STEW)||((m_u8DIYMode>0)&&(m_u8CookMode==0)))
						{
							u8CookTemp += 10;
						}
                        if(m_u8CookMode==FUNC_ID_REHEAT)	
						{
							u8CookTemp += 20;
						}
					}
					JudgeTimeValid();	//判断温度是否在合理范围内
					m_u8TimeDispCnt = 62;
				}			
			}
		}
	}

	if((u8Key==VAL_KEY_MINUS)&&(u16DispDIYCnt==0))
	{
		if((bTimeDisp_EN==0)&&(bTempDisp_EN==0)&&(bBookTime_EN==0))	
		{	
			if(IsNewKey())
			{
				if((m_u8CookMode==FUNC_ID_SOUP)||(m_u8CookMode==FUNC_ID_BAKE)||(m_u8CookMode==FUNC_ID_STEW)
				||(m_u8CookMode==FUNC_ID_STEAM))
				{
					if(m_u8FoodType<=1)  m_u8FoodType = 4;
					else m_u8FoodType--;
					LEDDispFoodType(m_u8FoodType);	
					DispDefCookTime(m_u8CookMode,m_u8DIYMode,m_u8FoodType);	
					SetDefCookTemp(m_u8CookMode,m_u8DIYMode,m_u8FoodType);	
					tmCookTime[1] =	tmDefCookTime[1];	//默认烹饪时间
					tmCookTime[2] =	tmDefCookTime[2];
					u8CookTemp = DefCookTemp;			//默认烹饪温度					
				}
			}
		}
		else
		{
			if(bTimeDisp_EN==1)							//烹饪时间是否允许调整
			{
				if((IsNewKey())||(g_u16KeyRpyCnt>15))
				{
					//判断烹饪时间是否溢出 
					if((tmCookTime[2]==tmMinCookTime[2])&&(tmCookTime[1]==tmMinCookTime[1]))
					{
						tmCookTime[2] =	tmMaxCookTime[2];
						tmCookTime[1] =	tmMaxCookTime[1];
					}
					else
					{
						if((m_u8CookMode==FUNC_ID_FRY)||(m_u8CookMode==FUNC_ID_PASTA))				
						{
							if(tmCookTime[1]==0)
							{
								tmCookTime[1] = 59;
								tmCookTime[2]--;
							}
							else
							{
								tmCookTime[1]--;
							}
						}
						else 
						{
							if(m_u8CookMode==FUNC_ID_YOGHURT)
							{
								if(tmCookTime[1]==0)
								{
									tmCookTime[1] = 45;
									tmCookTime[2]--;
								}
								else
								{
									tmCookTime[1] -= 15;
								}
							}
							else
							{
    						    if(tmCookTime[1]==0)
    						    {
    							    tmCookTime[1] = 55;
    							    tmCookTime[2]--;
    						    }
    						    else
    						    {
    							    tmCookTime[1] -= 5;
    						    }  
							}
						}	
					}
					JudgeTimeValid();		//判断时间是否在合理范围内
					m_u8TimeDispCnt = 62;	
				}
			}

			if(bBookTime_EN)				//预约时间是否允许调整
			{
				if((IsNewKey())||(g_u16KeyRpyCnt>15))
				{
					//判断预约时间是否溢出
					if((tmBookTime[2]==tmMinBookTime[2])&&(tmBookTime[1]==tmMinBookTime[1]))
					{
						tmBookTime[2] =	24;
						tmBookTime[1] =	0;
					}
					else
					{					
						if(tmBookTime[1]==0)
						{
							tmBookTime[1] = 55;
							tmBookTime[2]--;
						}
						else
						{
							tmBookTime[1] -= 5;
						}	
					}
				}
				JudgeTimeValid();		//判断时间是否在合理范围内
				m_u8TimeDispCnt = 62;
			}
		
			if(bTempDisp_EN)			//温度是否允许调整
			{
				if((IsNewKey())||((g_u16KeyRpyCnt>15)&&(g_u16KeyRpyCnt%3==0)))
				{
					//判断烹饪温度是否溢出
					if(u8CookTemp==MinCookTemp)
					{
						u8CookTemp = MaxCookTemp;
					}
					else
					{
                        if((m_u8CookMode==FUNC_ID_BAKE)||(m_u8CookMode==FUNC_ID_FRY)||(m_u8CookMode==FUNC_ID_OPORRIDGE)
                        ||(m_u8CookMode==FUNC_ID_SOUP)||(m_u8CookMode==FUNC_ID_STEW)||((m_u8DIYMode>0)&&(m_u8CookMode==0)))
						{
							u8CookTemp -= 10;
						}
						if(m_u8CookMode==FUNC_ID_REHEAT)
						{
							u8CookTemp -= 20;
						}
					}
					JudgeTimeValid();	//判断温度是否在合理范围内
					m_u8TimeDispCnt = 62;
				}			
			}
		}
	}				

	if(TstBit(u8Key,BIT_KEY_TIMER))
	{
		if(IsNewKey())
		{
//            if((m_u8DIYMode==4)&&(m_u8DIYSwitch==0))
//            {
//                m_u8DIYSwitch = 1;          //先切换到DIY1设置
//                u16DispDIYCnt = 250;        //显示DIY1 2S
//            }

            if(u16DispDIYCnt==0)
            {
    			if(bTimeDisp_EN)
    			{
    				if((m_u8CookMode==FUNC_ID_BAKE)||(m_u8CookMode==FUNC_ID_STEW)||(m_u8CookMode==FUNC_ID_FRY)
    				||(m_u8CookMode==FUNC_ID_OPORRIDGE)||(m_u8CookMode==FUNC_ID_REHEAT)||(m_u8CookMode==FUNC_ID_SOUP)
    				||((m_u8DIYMode>0)&&(m_u8CookMode==0)))
    				{
    					bTempDisp_EN = 1;		//允许调整温度
    					bTimeDisp_EN = 0;		//不允许调整烹饪时间
    					bBookTime_EN = 0;	    //不允许调整预约时间
    					bCalBookTime_EN = 0;
    					m_u8TimeDispCnt = 62;
    					u8BlankCnt = 62;
    				}
    			}
    			else
    			{
    				if((m_u8CookMode!=FUNC_ID_RICE)&&(m_u8CookMode!=FUNC_ID_PILAF))
    				{
    					bTimeDisp_EN = 1;		//允许调整烹饪时间
    					bTempDisp_EN = 0;		//不允许调整温度
    					bBookTime_EN = 0;		//不允许调整预约时间
    					bCalBookTime_EN = 0;	
    					m_u8TimeDispCnt = 62;
    					u8BlankCnt = 62;					
    				}
    			}
            }
		}
	}	
			
	if(TstBit(u8Key,BIT_KEY_PRESET))
	{
		if(IsNewKey())
		{
//		    if(m_u8DIYMode!=4)      //diy4不能预约
//		    {
			if((m_u8CookMode!=FUNC_ID_FRY)&&(m_u8CookMode!=FUNC_ID_PASTA)&&(m_u8CookMode!=FUNC_ID_YOGHURT)
             &&(m_u8CookMode!=FUNC_ID_CHEESE)&&(m_u8CookMode!=FUNC_ID_DOUGH))
			{
			    if(u16DispDIYCnt==0)
                {
    				if(bBookTime_EN==0)
    				{
    					if((m_u8CookMode==FUNC_ID_RICE)||(m_u8CookMode==FUNC_ID_PILAF))
    					{
    						tmBookTime[1] =	50;				            //默认预约时间
    						tmBookTime[2] =	0;
    						tmMinBookTime[1] = 50;
    						tmMinBookTime[2] = 0;			
    					}
    					else
    					{
    						tmBookTime[1] = tmCookTime[1];
    						tmBookTime[2] = tmCookTime[2];
    						tmMinBookTime[1] = tmCookTime[1];
    						tmMinBookTime[2] = tmCookTime[2];
    					}
    				}
    				bBookTime_EN = 1;
    				bCalBookTime_EN	= 1;
    				bTimeDisp_EN = 0;
    				bTempDisp_EN = 0;
    				m_u8TimeDispCnt = 62;
    				u8BlankCnt = 62;
                }
    		}	
//          }
		}	
	}
	
	if(TstBit(u8Key,BIT_KEY_DIY))
	{
		if(IsNewKey())
		{
//            if(m_u8DIYMode>=4) m_u8DIYMode = 1;
//            else m_u8DIYMode++;     
            m_u8DIYMode = 1;
            m_u8DIYSwitch = 0;
            m_u8DIYCookStage = 0;
            SetDIYModeByDIY(m_u8DIYMode,m_u8DIYSwitch);  
		}
       
		if((u8Key==VAL_KEY_DIY)&&(g_u16KeyRpyCnt==36))   //长按DIY键5S恢复默认设置
		{
		    m_u8CookMode = 0;                   //清除烹饪模式
		    m_u8FoodType = 0;                   
			DispDefCookTime(m_u8CookMode,m_u8DIYMode,m_u8FoodType);	
			tmCookTime[1] =	tmDefCookTime[1];	//默认烹饪时间
			tmCookTime[2] =	tmDefCookTime[2];	
			SetDefCookTemp(m_u8CookMode,m_u8DIYMode,m_u8FoodType);
			u8CookTemp = DefCookTemp;
            if((m_u8DIYMode==1)||(m_u8DIYMode==4))  //DIY1 or DIY4
            {
                m_u8DIYCookMode[0] = 0;
                m_u8DIYFoodType[0] = 0;
                tmDIY1CookTime[1] = tmCookTime[1];
                tmDIY1CookTime[2] = tmCookTime[2];
                u8DIY1CookTemp = u8CookTemp;
                rEepromCache[EEP_DIY1_S0] = 0xA5;
                rEepromCache[EEP_DIY1_TM1] = tmCookTime[1];
                rEepromCache[EEP_DIY1_TM2] = tmCookTime[2];
                rEepromCache[EEP_DIY1_TMP] = u8CookTemp;          
                rEepromCache[EEP_DIY1_COOKMODE] = 0;
                rEepromCache[EEP_DIY1_FOODTYPE] = 0;
            }
//            if((m_u8DIYMode==2)||(m_u8DIYMode==4))  //DIY2 or DIY4
//            {   
//                m_u8DIYCookMode[1] = 0;
//                m_u8DIYFoodType[1] = 0;
//                tmDIY2CookTime[1] = tmCookTime[1];
//                tmDIY2CookTime[2] = tmCookTime[2];
//                u8DIY2CookTemp = u8CookTemp;
//                rEepromCache[EEP_DIY2_S0] = 0xA5;
//                rEepromCache[EEP_DIY2_TM1] = tmCookTime[1];
//                rEepromCache[EEP_DIY2_TM2] = tmCookTime[2];
//                rEepromCache[EEP_DIY2_TMP] = u8CookTemp;          
//                rEepromCache[EEP_DIY2_COOKMODE] = 0;
//                rEepromCache[EEP_DIY2_FOODTYPE] = 0;  
//            }
//            if(m_u8DIYMode==3)                      //DIY3
//            {   
//                m_u8DIYCookMode[2] = 0;
//                m_u8DIYFoodType[2] = 0;
//                tmDIY3CookTime[1] = tmCookTime[1];
//                tmDIY3CookTime[2] = tmCookTime[2];
//                u8DIY3CookTemp = u8CookTemp;
//                rEepromCache[EEP_DIY3_S0] = 0xA5;
//                rEepromCache[EEP_DIY3_TM1] = tmCookTime[1];
//                rEepromCache[EEP_DIY3_TM2] = tmCookTime[2];
//                rEepromCache[EEP_DIY3_TMP] = u8CookTemp;          
//                rEepromCache[EEP_DIY3_COOKMODE] = 0;
//                rEepromCache[EEP_DIY3_FOODTYPE] = 0;  
//            }
            EEPROM_Process();
            LEDDispFoodType(m_u8FoodType);
			LEDDispCookMode(m_u8CookMode,m_u8DIYMode);
			PlaySound(MusicPoweron);
		}
	}
	
	if(TstBit(u8Key,BIT_KEY_START))
	{
		if(IsNewKey())
		{
//            if((m_u8DIYMode==4)&&(m_u8DIYSwitch==1))
//            {
//                m_u8DIYSwitch = 2;
//                m_u8DIYCookMode[0] = m_u8CookMode;          //Save DIY1 Setting temporarily
//                m_u8DIYFoodType[0] = m_u8FoodType;
//                tmDIY1CookTime[1] = tmCookTime[1];
//                tmDIY1CookTime[2] = tmCookTime[2];
//                u8DIY1CookTemp = u8CookTemp;
//                SetDIYModeByDIY(m_u8DIYMode,m_u8DIYSwitch); //Display diy2
//            }
//            else
//            {
//    			if(m_u8DIYMode==4)    
//                {
//                    if(m_u8DIYSwitch==2)
//                    {
//                        m_u8DIYCookMode[1] = m_u8CookMode;      //Save DIY2 Setting temporarily
//                        m_u8DIYFoodType[1] = m_u8FoodType;
//                        tmDIY2CookTime[1] = tmCookTime[1];
//                        tmDIY2CookTime[2] = tmCookTime[2];
//                        u8DIY2CookTemp = u8CookTemp;             
//                    } 
//                    m_u8DIYCookStage = 1;                       //先启动DIY1
//                    m_u8CookMode = m_u8DIYCookMode[0];          //设置当前默认模式
//                    m_u8FoodType = m_u8DIYFoodType[0];
//                    tmCookTime[1] = tmDIY1CookTime[1];
//                    tmCookTime[2] = tmDIY1CookTime[2];
//                    u8CookTemp = u8DIY1CookTemp;             
//                }
                m_u8SysID =	SYS_ID_RUN;				//系统进入烹饪状态
    			SetCurScript(m_u8CookMode,m_u8DIYMode,u8CookTemp);
    			bLEDComeback = 1;                   //恢复正常功能显示
    			u16DispDIYCnt = 0;                  //Not display DIY xx
    			if(m_u8DIYMode==1)                  //DIY1               
    			{
                    if((p_EepRom[EEP_DIY1_S0]!=0xA5)||(p_EepRom[EEP_DIY1_TM1]!=tmCookTime[1])||(p_EepRom[EEP_DIY1_TM2]!=tmCookTime[2])
                    ||(p_EepRom[EEP_DIY1_TMP]!=u8CookTemp)||(p_EepRom[EEP_DIY1_COOKMODE]!=m_u8CookMode)||(p_EepRom[EEP_DIY1_FOODTYPE]!=m_u8FoodType))
                    {
                        rEepromCache[EEP_DIY1_S0] = 0xA5;
                        rEepromCache[EEP_DIY1_TM1] = tmCookTime[1];
                        rEepromCache[EEP_DIY1_TM2] = tmCookTime[2];
                        rEepromCache[EEP_DIY1_TMP] = u8CookTemp;
                        rEepromCache[EEP_DIY1_COOKMODE] = m_u8CookMode;
                        rEepromCache[EEP_DIY1_FOODTYPE] = m_u8FoodType;
                        EEPROM_Process();
                    }
                }
//                if(m_u8DIYMode==2)                  //DIY2
//                {
//                    if((p_EepRom[EEP_DIY2_S0]!=0xA5)||(p_EepRom[EEP_DIY2_TM1]!=tmCookTime[1])||(p_EepRom[EEP_DIY2_TM2]!=tmCookTime[2])
//                    ||(p_EepRom[EEP_DIY2_TMP]!=u8CookTemp)||(p_EepRom[EEP_DIY2_COOKMODE]!=m_u8CookMode)||(p_EepRom[EEP_DIY2_FOODTYPE]!=m_u8FoodType))
//                    {
//                        rEepromCache[EEP_DIY2_S0] = 0xA5;
//                        rEepromCache[EEP_DIY2_TM1] = tmCookTime[1];
//                        rEepromCache[EEP_DIY2_TM2] = tmCookTime[2];
//                        rEepromCache[EEP_DIY2_TMP] = u8CookTemp;
//                        rEepromCache[EEP_DIY2_COOKMODE] = m_u8CookMode;
//                        rEepromCache[EEP_DIY2_FOODTYPE] = m_u8FoodType;
//                        EEPROM_Process();
//                    }
//                }
//                if(m_u8DIYMode==3)                  //DIY3
//                {
//                    if((p_EepRom[EEP_DIY3_S0]!=0xA5)||(p_EepRom[EEP_DIY3_TM1]!=tmCookTime[1])||(p_EepRom[EEP_DIY3_TM2]!=tmCookTime[2])
//                    ||(p_EepRom[EEP_DIY3_TMP]!=u8CookTemp)||(p_EepRom[EEP_DIY3_COOKMODE]!=m_u8CookMode)||(p_EepRom[EEP_DIY3_FOODTYPE]!=m_u8FoodType))
//                    {
//                        rEepromCache[EEP_DIY3_S0] = 0xA5;
//                        rEepromCache[EEP_DIY3_TM1] = tmCookTime[1];
//                        rEepromCache[EEP_DIY3_TM2] = tmCookTime[2];
//                        rEepromCache[EEP_DIY3_TMP] = u8CookTemp;
//                        rEepromCache[EEP_DIY3_COOKMODE] = m_u8CookMode;
//                        rEepromCache[EEP_DIY3_FOODTYPE] = m_u8FoodType;
//                        EEPROM_Process();
//                    }
//                }
//                if(m_u8DIYMode==4)                  //DIY4
//                {
//                    if((p_EepRom[EEP_DIY1_S0]!=0xA5)||(p_EepRom[EEP_DIY1_TM1]!=tmDIY1CookTime[1])||(p_EepRom[EEP_DIY1_TM2]!=tmDIY1CookTime[2])
//                    ||(p_EepRom[EEP_DIY1_TMP]!=u8DIY1CookTemp)||(p_EepRom[EEP_DIY1_COOKMODE]!=m_u8DIYCookMode[0])||(p_EepRom[EEP_DIY1_FOODTYPE]!=m_u8DIYFoodType[0])
//                    ||(p_EepRom[EEP_DIY2_S0]!=0xA5)||(p_EepRom[EEP_DIY2_TM1]!=tmDIY2CookTime[1])||(p_EepRom[EEP_DIY2_TM2]!=tmDIY2CookTime[2])
//                    ||(p_EepRom[EEP_DIY2_TMP]!=u8DIY2CookTemp)||(p_EepRom[EEP_DIY2_COOKMODE]!=m_u8DIYCookMode[1])||(p_EepRom[EEP_DIY2_FOODTYPE]!=m_u8DIYFoodType[1]))
//                    {
//                        rEepromCache[EEP_DIY1_S0] = 0xA5;
//                        rEepromCache[EEP_DIY1_TM1] = tmDIY1CookTime[1];
//                        rEepromCache[EEP_DIY1_TM2] = tmDIY1CookTime[2];
//                        rEepromCache[EEP_DIY1_TMP] = u8DIY1CookTemp;
//                        rEepromCache[EEP_DIY1_COOKMODE] = m_u8DIYCookMode[0];
//                        rEepromCache[EEP_DIY1_FOODTYPE] = m_u8DIYFoodType[0];
//                        rEepromCache[EEP_DIY2_S0] = 0xA5;
//                        rEepromCache[EEP_DIY2_TM1] = tmDIY2CookTime[1];
//                        rEepromCache[EEP_DIY2_TM2] = tmDIY2CookTime[2];
//                        rEepromCache[EEP_DIY2_TMP] = u8DIY2CookTemp;
//                        rEepromCache[EEP_DIY2_COOKMODE] = m_u8DIYCookMode[1];
//                        rEepromCache[EEP_DIY2_FOODTYPE] = m_u8DIYFoodType[1];
//                        EEPROM_Process();           
//                    }
//                }
    			bTimeDisp_EN = 0;		
    			bTempDisp_EN = 0;			
    			m_u8PreTmp = ADCGetData(AD_CH_BOT);	
    			m_u8AutoRunCnt = 0;
                if(bKeyMusic_EN==0)PlaySound(MusicKeyDown);
//            }
		}
	}

    if(u8Key>0) 
    {
        m_u8DispDelayCnt = 60;
    }
    
	//恢复工作状态显示
	if(bLEDComeback)
	{
		bLEDComeback = 0;
		bSysPwrOn = 1;
	}

	//恢复显示
	if(bSysPwrOn)
	{
        RestoreDispMenu();
	}
    
	//Display DIY xx 2S
//    if(u16DispDIYCnt>0)
//    {
//        if((m_u8DIYMode==4)&&(m_u8DIYSwitch==0))
//        {
//        }
//        else
//        {
//            u16DispDIYCnt--;
//        }
//        LEDClearFun();
//        LEDDispDIYNum(m_u8DIYMode,m_u8DIYSwitch);
//        if(u16DispDIYCnt==0)
//        {
//            bLEDComeback = 1;
//        }
//    }
    
	//时间&&温度Blinking处理
	if((bTimeDisp_EN==1)||(bBookTime_EN==1)||(bTempDisp_EN==1))			
	{		
		m_u8TimeDispCnt++;
		if(m_u8TimeDispCnt==63)
		{
			bLED_Timer = 0;	 			//CLR Timer
			bLED_Temp =	0;				//CLR Temperature
			bLED_Preset = 0;			//CLR Preset
			bLED_Dot1 =	1;				//SET Dot
			bLED_Dot2 =	1;	

			if(bBookTime_EN==1)
			{
				bLED_Preset = 1;		
				LEDDispTime(tmBookTime);//显示预约时间
		   	}
			else
			{
				if(bTimeDisp_EN==1)
				{
					bLED_Timer = 1;					
					LEDDispTime(tmCookTime);//显示烹饪时间
				}
				else
				{
					if(bTempDisp_EN==1)
					{
						LEDDispTemp(u8CookTemp);//显示烹饪温度
					}
				}
			}
		}

		if(m_u8TimeDispCnt==126)
		{
			m_u8TimeDispCnt	= u8BlankCnt; //功能和数字同步闪烁
		}

		if(m_u8TimeDispCnt==0)
		{
			LEDClearNum();
	  	}
	}

	//1分钟无操作,进入PowerDown模式
	if(bSys_1000MS)
	{
		if(m_u8DispDelayCnt>0)
		{
			m_u8DispDelayCnt--;
			if(m_u8DispDelayCnt==0)
			{
			    EnterReady();
			    EnterPowerDown();
                LEDDispPowerSaving(0);
                m_u16Movcnt = 0;
                m_u8DispDelayCnt = 240; //延迟4分钟进入OFF模式
			}
		}	
	}		
	
	//保温ON/OFF显示标志
	if((m_u8CookMode==FUNC_ID_PASTA)||(m_u8CookMode==FUNC_ID_YOGHURT)||(m_u8CookMode==FUNC_ID_CHEESE)
    ||(m_u8CookMode==FUNC_ID_JAM)||(m_u8CookMode==FUNC_ID_DOUGH)||((m_u8DIYMode>0)&&(m_u8CookMode==0))||(bWarm_EN==0))
	{
		bWarmLedBlank = 0;
	}
	else
	{
		bWarmLedBlank = 1;
	}
}

//判断时间是否溢出
//说明：对于烹饪时间，如果时间大于最长的烹饪时间，那么设置为最小的烹饪时间
//  	对于预约时间，如果时间大于24:00，那么设置为烹饪时间
static void JudgeTimeValid(void)
{
	if(bBookTime_EN)
	{
		if(tmBookTime[1]>=60)
		{
			tmBookTime[1] =	0;
			tmBookTime[2] += 1;
	  	}
		
		if(tmBookTime[2]>24)
		{
			tmBookTime[2] =	24;
			tmBookTime[1] =	0;
	 	}
		
		if((tmBookTime[2]==24)&&(tmBookTime[1]>0))
		{
			tmBookTime[2] =	24;
			tmBookTime[1] =	0;
		}
		
		if(tmBookTime[2]<tmMinBookTime[2])
		{
			tmBookTime[2] = tmMinBookTime[2];
            tmBookTime[1] =	tmMinBookTime[1];		
		}
		
		if(tmBookTime[2]==tmMinBookTime[2])
		{
			if(tmBookTime[1]<tmMinBookTime[1])
			{
				tmBookTime[2] = tmMinBookTime[2];
				tmBookTime[1] =	tmMinBookTime[1];	
			}
		}	
  	}
	else
	{
		if(bTimeDisp_EN)
		{
			if(tmCookTime[1]>=60)
			{
				tmCookTime[1] =	0;
				tmCookTime[2] += 1;
			}
		
			if(tmCookTime[2]>tmMaxCookTime[2])
			{
				tmCookTime[2] =	tmMaxCookTime[2];
				tmCookTime[1] =	tmMaxCookTime[1];
			}
		
			if(tmCookTime[2]==tmMaxCookTime[2])
			{
				if(tmCookTime[1]>tmMaxCookTime[1])
				{
					tmCookTime[2] =	tmMaxCookTime[2];
					tmCookTime[1] =	tmMaxCookTime[1];
				}
			}

			if(tmCookTime[2]<tmMinCookTime[2])
			{
				tmCookTime[2] =	tmMinCookTime[2];
				tmCookTime[1] =	tmMinCookTime[1];
			}

			if(tmCookTime[2]==tmMinCookTime[2])
			{
				if(tmCookTime[1]<tmMinCookTime[1])
				{
					tmCookTime[2] =	tmMinCookTime[2];
					tmCookTime[1] =	tmMinCookTime[1];
				}
			}
		}
		else
		{
			if(bTempDisp_EN)
			{
				if(u8CookTemp>MaxCookTemp)
				{
					u8CookTemp = MaxCookTemp;
				}
	
				if(u8CookTemp<MinCookTemp)
				{
					u8CookTemp = MinCookTemp;
				}
			}
		}
	}
}	



//恢复正常显示(菜单、时间、定时和预约)
static void RestoreDispMenu(void)
{   
    LEDClearFun();
	LEDDispCookMode(m_u8CookMode,m_u8DIYMode);		//显示烹饪模式
	m_u8TimeDispCnt	= u8BlankCnt;		            //使闪烁同步	

	if((m_u8CookMode==FUNC_ID_SOUP)||(m_u8CookMode==FUNC_ID_BAKE)||(m_u8CookMode==FUNC_ID_STEW)
	||(m_u8CookMode==FUNC_ID_STEAM))
	{
		LEDDispFoodType(m_u8FoodType);
	}
	
	if(bBookTime_EN)
	{
		LEDDispTime(tmBookTime);
		bLED_Preset	= 1;				//SET Preset
		bLED_Dot1 =	1;					//SET Dot
		bLED_Dot2 =	1;			
	}
	else
	{
		if(bTimeDisp_EN)
		{
			LEDDispTime(tmCookTime);
			bLED_Timer = 1;				//SET Timer
			bLED_Dot1 =	1;				//SET Dot
			bLED_Dot2 =	1;
		}
		else
		{
			if(bTempDisp_EN)
			{
				LEDDispTemp(u8CookTemp);
			}
			else
			{
				if((tmCookTime[0]==59)&&(tmCookTime[1]==0)&&(tmCookTime[2]==0))
				{
					LEDDispMovie(0);
				}
				else
				{
					LEDDispTime(tmCookTime);
					bLED_Timer = 1;				//SET Timer
					bLED_Dot1 =	1;				//SET Dot
					bLED_Dot2 =	1;		
				}
			}
		}
	}
}

//根据烹饪模式选择并显示烹饪时间
static void	DispDefCookTime(uint8 CookMode,uint8 DIYMode,uint8 FoodType)
{
	uint8 index=0;

    if(DIYMode>0)
    {
        index = 12;
    }
    
	switch(CookMode)
	{
		case	FUNC_ID_SOUP:	
			index =	0;
			break;
		case	FUNC_ID_BAKE:
			index = 1;
			break;
		case	FUNC_ID_STEW:
			index = 2;
			break;
		case	FUNC_ID_STEAM:	
			index = 3;
		    break;
		case	FUNC_ID_FRY:
			index = 4;
		    break;
		case	FUNC_ID_OPORRIDGE:	
			index = 5;
			break;
		case	FUNC_ID_JAM:
			index = 6;
			break;
		case    FUNC_ID_PASTA:
			index = 7;
			break;
		case    FUNC_ID_YOGHURT:
			index = 8;
			break;
		case	FUNC_ID_CHEESE:
			index = 9;
			break;
		case   	FUNC_ID_DOUGH:	
			index = 10;
			break;
		case	FUNC_ID_REHEAT:
			index = 11;
			break;
		default:	
			break;
	}
   
	if(index<4)
	{	
		if(FoodType>0)
		{
			FoodType -= 1;
			tmDefCookTime[2] = HIBYTE(DefFoodTime_Table[index][FoodType]);
			tmDefCookTime[1] = LOBYTE(DefFoodTime_Table[index][FoodType]);	
		}
	}
	else
	{
		tmDefCookTime[2] = HIBYTE(DefCookTime_Table[index]);
		tmDefCookTime[1] = LOBYTE(DefCookTime_Table[index]);
	}
	
	tmMaxCookTime[2] = HIBYTE(MaxCookTime_Table[index]);
	tmMaxCookTime[1] = LOBYTE(MaxCookTime_Table[index]);
	tmMinCookTime[2] = HIBYTE(MinCookTime_Table[index]);
	tmMinCookTime[1] = LOBYTE(MinCookTime_Table[index]);
	LEDDispTime(tmDefCookTime);	//显示默认时间
	bTimeDisp_EN =0;			//不允许调整时间
	bLED_Timer = 1;      	    //SET Timer
	bLED_Dot1 =	1;				//SET Dot
	bLED_Dot2 =	1;	
}

//根据烹饪模式和食物种类选择烹饪温度
static void SetDefCookTemp(uint8 CookMode,uint8 DIYMode,uint8 FoodType)
{
	uint8 index=0;

    if(CookMode==FUNC_ID_SOUP) index = 1;
	if(CookMode==FUNC_ID_FRY) index = 2;	
    if(CookMode==FUNC_ID_STEW) index = 3;
	if(CookMode==FUNC_ID_OPORRIDGE) index = 4;		
    if(CookMode==FUNC_ID_REHEAT) index = 5;
	if((DIYMode>0)&&(CookMode==0)) index = 6;		
	
	if(CookMode==FUNC_ID_BAKE)
	{
		if(FoodType>0)
		{
			FoodType -= 1;
			DefCookTemp = DefFoodTemp_Table[FoodType];
		}
	}
	else
	{
		DefCookTemp = DefCookTemp_Table[index];
	}
	
	MaxCookTemp = MaxCookTemp_Table[index];
	MinCookTemp = MinCookTemp_Table[index];	
}

//MENU键设置CookMode
static  void    SetCookModeByMenu(void)
{
    m_u8FoodType = 0;
	bTimeDisp_EN = 0;				//时间不允许调整
	bTempDisp_EN = 0;				//温度不允许调整   
	bBookTime_EN = 0;				//取消预约
	bCalBookTime_EN	= 0;			//取消预约计算
	tmBookTime[0] =	59;	 			//清预约时间
	tmBookTime[1] =	0;
	tmBookTime[2] =	0;
	tmCookTime[0] =	59;				//清烹饪时间
	tmCookTime[1] =	0;
	tmCookTime[2] =	0;	
	u8CookTemp = 0;					//清烹饪温度
    LEDClearFun();	

    switch(m_u8CookMode)
	{
	    case	FUNC_ID_SOUP:		
		case	FUNC_ID_BAKE:		
		case	FUNC_ID_STEW:			
		case	FUNC_ID_STEAM:		
		case	FUNC_ID_FRY:		
		case	FUNC_ID_OPORRIDGE:		
		case	FUNC_ID_JAM:		
		case    FUNC_ID_PASTA:		
		case    FUNC_ID_YOGHURT:		
		case	FUNC_ID_CHEESE:		
		case   	FUNC_ID_DOUGH:			
		case   	FUNC_ID_REHEAT:		
			if((m_u8CookMode==FUNC_ID_SOUP)||(m_u8CookMode==FUNC_ID_BAKE)||(m_u8CookMode==FUNC_ID_STEW)
			||(m_u8CookMode==FUNC_ID_STEAM))
			{
				m_u8FoodType = 1;     //Vegetables
				LEDDispFoodType(m_u8FoodType);
			}
		    DispDefCookTime(m_u8CookMode,m_u8DIYMode,m_u8FoodType);	
		    tmCookTime[1] =	tmDefCookTime[1];		//默认烹饪时间
		    tmCookTime[2] =	tmDefCookTime[2];
		    if((m_u8CookMode==FUNC_ID_BAKE)||(m_u8CookMode==FUNC_ID_FRY)||(m_u8CookMode==FUNC_ID_STEW)
            ||(m_u8CookMode==FUNC_ID_OPORRIDGE)||(m_u8CookMode==FUNC_ID_REHEAT)||(m_u8CookMode==FUNC_ID_SOUP))
			{
				SetDefCookTemp(m_u8CookMode,m_u8DIYMode,m_u8FoodType);
				u8CookTemp = DefCookTemp;
			}
			break;
		case	FUNC_ID_RICE:		
		case	FUNC_ID_PILAF:		
			LEDDispMovie(0);
			break;					
		default:
			break;
	}
	LEDDispCookMode(m_u8CookMode,m_u8DIYMode);			//显示烹饪模式
}

//DIY键设置DIYMode
static  void    SetDIYModeByDIY(uint8 diymode,uint8 diyswitch)
{
    uint8 i,j;
    
    if((diymode>0)&&(diymode<5))
    { 
        if((diymode==1)||((diymode==4)&&(diyswitch<=1)))
        {
            i = 0;
            j = 0;
        }
        if((diymode==2)||((diymode==4)&&(diyswitch==2)))
        {
            i = 1;
            j = 6;
        }
        if(diymode==3)
        {
            i = 2;
            j = 12;
        }
//    	u16DispDIYCnt = 250;        //Display DIY 2s
        u16DispDIYCnt = 0;
    	bTimeDisp_EN = 0;           //不允许调整时间  
		bTempDisp_EN = 0;			//不允许调整温度
		bBookTime_EN = 0;			//取消预约
		bCalBookTime_EN	= 0;		//取消预约计算
		tmBookTime[0] =	59;	 		//清预约时间
		tmBookTime[1] =	0;
		tmBookTime[2] =	0;
		tmCookTime[0] =	59;			//清烹饪时间
		tmCookTime[1] =	0;
		tmCookTime[2] =	0;	
        u8CookTemp = 0;				//清烹饪温度
        m_u8CookMode = 0;
        m_u8FoodType = 0;
    	if(rEepromCache[j+EEP_DIY1_S0]==0xA5)
    	{
    	    m_u8DIYCookMode[i] = rEepromCache[j+EEP_DIY1_COOKMODE];
            m_u8DIYFoodType[i] = rEepromCache[j+EEP_DIY1_FOODTYPE];
            if(m_u8DIYCookMode[i]<15)
            {
                m_u8CookMode = m_u8DIYCookMode[i];
            }
            if(m_u8DIYFoodType[i]<5)
            {
                m_u8FoodType = m_u8DIYFoodType[i];
            }
    		DispDefCookTime(m_u8CookMode,m_u8DIYMode,m_u8FoodType);	
            SetDefCookTemp(m_u8CookMode,m_u8DIYMode,m_u8FoodType);
    		tmCookTime[1] = rEepromCache[j+EEP_DIY1_TM1];
    		tmCookTime[2] = rEepromCache[j+EEP_DIY1_TM2];
            u8CookTemp = rEepromCache[j+EEP_DIY1_TMP];
            if(i==0)
            {
                tmDIY1CookTime[1] = tmCookTime[1];
                tmDIY1CookTime[2] = tmCookTime[2];
                u8DIY1CookTemp = u8CookTemp;
            }
//            if(i==1)
//            {
//                tmDIY2CookTime[1] = tmCookTime[1];
//                tmDIY2CookTime[2] = tmCookTime[2];
//                u8DIY2CookTemp = u8CookTemp;    
//            }
//            if(i==2)
//            {
//                tmDIY3CookTime[1] = tmCookTime[1];
//                tmDIY3CookTime[2] = tmCookTime[2];
//                u8DIY3CookTemp = u8CookTemp;   
//            }
    	}
    	else
    	{
    	    m_u8DIYCookMode[i] = 0;
            m_u8DIYFoodType[i] = 0;
    		DispDefCookTime(m_u8CookMode,m_u8DIYMode,m_u8FoodType);
            SetDefCookTemp(m_u8CookMode,m_u8DIYMode,m_u8FoodType);
    		tmCookTime[1] =	tmDefCookTime[1];	//默认烹饪时间
    		tmCookTime[2] =	tmDefCookTime[2];	
    		u8CookTemp = DefCookTemp;	
            if(i==0)
            {
                tmDIY1CookTime[1] = tmCookTime[1];
                tmDIY1CookTime[2] = tmCookTime[2];
                u8DIY1CookTemp = u8CookTemp;
            }
//            if(i==1)
//            {
//                tmDIY2CookTime[1] = tmCookTime[1];
//                tmDIY2CookTime[2] = tmCookTime[2];
//                u8DIY2CookTemp = u8CookTemp;    
//            }
//            if(i==2)
//            {
//                tmDIY3CookTime[1] = tmCookTime[1];
//                tmDIY3CookTime[2] = tmCookTime[2];
//                u8DIY3CookTemp = u8CookTemp;   
//            }
    	}	
//        LEDClearFun();	
//        LEDDispDIYNum(m_u8DIYMode,m_u8DIYSwitch);
        RestoreDispMenu();
    }
}
/*-----------------------------------------------------------------------------*
 *  Function Description:                                                      *
 *      工作状态处理                                     					   *
 *  Parameters:                                                                *
 *      None                                                                   *
 *  Return                                                                     *
 *      None                                                                   *
 *----------------------------------------------------------------------------*/
static	void	RunProc(void)
{
	uint8 u8Key;
	u8Key =	GetKey();

    //开始键按下处理
    if(TstBit(u8Key, BIT_KEY_START))
    {
        if(IsNewKey())
        {
            if((m_u8CookMode==FUNC_ID_PASTA)&&(bTempAch1st==1)&&(bTimeCountDown==0))    //pasta
            {
                bTimeCountDown = 1;   
                if(bKeyMusic_EN==0)PlaySound(MusicKeyDown);    
            }
            else
            {
                if((bPowerAdjSt==1)||(rEepromCache[EEP_POWER_S0]!=0xA5))    //保存Power_value
                {
                    rEepromCache[EEP_POWER_S0] = 0xA5;
                    rEepromCache[EEP_POWER_VAL] = power_value;
                    EEPROM_Process();	
                }
                EnterReady();
                PlaySound(MusicCancelFunc);
			    return;
            }
        }
    }
    
	//取消键按下处理
	if(TstBit(u8Key,BIT_KEY_CANCEL))
	{
		if(IsNewKey())
		{   
		    if((bPowerAdjSt==1)||(rEepromCache[EEP_POWER_S0]!=0xA5))    //保存Power_value
            {
                rEepromCache[EEP_POWER_S0] = 0xA5;
                rEepromCache[EEP_POWER_VAL] = power_value;
                EEPROM_Process();	
            }
			EnterReady();
            PlaySound(MusicCancelFunc);
			return;
		}
  	}
	
    //Timer键按下处理
	if(TstBit(u8Key,BIT_KEY_TIMER))
	{
		if(IsNewKey())
		{	
		    if((u16DispDIYCnt==0)&&(bPowerAdjSt==0))      //no diy display and no power adjust              
            {
        		if(bBookTime_EN==0)					//无预约
        		{
        		    if((m_u8CookMode!=FUNC_ID_RICE)&&(m_u8CookMode!=FUNC_ID_PILAF))
                    {
        		        m_u8AutoRunCnt = 5;
        			    m_u8TimeDispCnt = 62;
        			    if(bTimeDisp_EN)
        			    {
    				        if((m_u8CookMode==FUNC_ID_BAKE)||(m_u8CookMode==FUNC_ID_STEW)||(m_u8CookMode==FUNC_ID_FRY)
    				        ||(m_u8CookMode==FUNC_ID_OPORRIDGE)||(m_u8CookMode==FUNC_ID_REHEAT)||(m_u8CookMode==FUNC_ID_SOUP)
    				        ||((m_u8DIYMode>0)&&(m_u8CookMode==0)))
        				    {
        					    bTempDisp_EN = 1;		//允许调整温度
        					    bTimeDisp_EN = 0;		//不允许调整烹饪时间
        				    }
        			    }
        			    else
        			    {
        				    bTimeDisp_EN = 1;		//允许调整烹饪时间
        				    bTempDisp_EN = 0;		//不允许调整温度			
        			    }
                    }
        		}
                else                              //预约状态显示温度&时间
                {
                    if((m_u8CookMode!=FUNC_ID_RICE)&&(m_u8CookMode!=FUNC_ID_PILAF))
                    {
                        m_u8CurTimeDispCnt = 5;	
    				    if((m_u8CookMode==FUNC_ID_BAKE)||(m_u8CookMode==FUNC_ID_STEW)||(m_u8CookMode==FUNC_ID_FRY)
    				    ||(m_u8CookMode==FUNC_ID_OPORRIDGE)||(m_u8CookMode==FUNC_ID_REHEAT)||(m_u8CookMode==FUNC_ID_SOUP)
    				    ||((m_u8DIYMode>0)&&(m_u8CookMode==0)))
        				{
                            u8CHDispCnt[1]++;				  //通道显示计数器加一
                            if(u8CHDispCnt[1]>1)
                            {
                                u8CHDispCnt[1] = 0;
                            }
                        }
                        else
                        {
                            u8CHDispCnt[1] = 0;
                        }
                    }
                }
            }
		}
	}
	
	if(u8Key==VAL_KEY_PLUS)
	{
		if(bTimeDisp_EN==1)							//烹饪时间是否允许调整
		{
			if((IsNewKey())||(g_u16KeyRpyCnt>15))
			{
				//判断烹饪时间是否溢出 
				if((tmCookTime[2]==tmMaxCookTime[2])&&(tmCookTime[1]==tmMaxCookTime[1]))
				{
					tmCookTime[2] =	tmMinCookTime[2];
					tmCookTime[1] =	tmMinCookTime[1];
				}
				else
				{
					if((m_u8CookMode==FUNC_ID_FRY)||(m_u8CookMode==FUNC_ID_PASTA))			
					{
						tmCookTime[1]++;
					}
					else 
					{
						if(m_u8CookMode==FUNC_ID_YOGHURT)
						{
							tmCookTime[1] = (tmCookTime[1]/15)*15+15;
						}
						else
						{   
                            tmCookTime[1] = (tmCookTime[1]/5)*5+5;     
						}
					}	
				}
				JudgeTimeValid();		//判断时间是否在合理范围内
				tmCookTime[0] = 59;
				m_u8TimeDispCnt = 62;	
				m_u8AutoRunCnt = 5;
			}
		}	
					
		if(bTempDisp_EN)			//温度是否允许调整
		{
			if((IsNewKey())||((g_u16KeyRpyCnt>15)&&(g_u16KeyRpyCnt%3==0)))
			{
				//判断烹饪温度是否溢出
				if(u8CookTemp==MaxCookTemp)
				{
					u8CookTemp = MinCookTemp;
				}
				else
				{
                    if((m_u8CookMode==FUNC_ID_BAKE)||(m_u8CookMode==FUNC_ID_FRY)||(m_u8CookMode==FUNC_ID_OPORRIDGE)
                    ||(m_u8CookMode==FUNC_ID_SOUP)||(m_u8CookMode==FUNC_ID_STEW)||((m_u8DIYMode>0)&&(m_u8CookMode==0)))
					{
						u8CookTemp += 10;
					}
                    if(m_u8CookMode==FUNC_ID_REHEAT)	
					{
						u8CookTemp += 20;
					}
				}
				JudgeTimeValid();	//判断温度是否在合理范围内
				m_u8TimeDispCnt = 62;
				m_u8AutoRunCnt = 5;
			}
		}

        if(bPowerAdjSt)
        {
            if(IsNewKey())
            {
                power_value++;
                if((power_value>=58)||(power_value<=38))
	            {
		            power_value=46;
	            }
            }
        }
	}

    if(u8Key==VAL_KEY_MINUS)
	{
		if(bTimeDisp_EN==1)							//烹饪时间是否允许调整
		{
			if((IsNewKey())||(g_u16KeyRpyCnt>15))
			{
				//判断烹饪时间是否溢出 
				if((tmCookTime[2]==tmMinCookTime[2])&&(tmCookTime[1]==tmMinCookTime[1]))
				{
					tmCookTime[2] =	tmMaxCookTime[2];
					tmCookTime[1] =	tmMaxCookTime[1];
				}
				else
				{
					if((m_u8CookMode==FUNC_ID_FRY)||(m_u8CookMode==FUNC_ID_PASTA))			
					{
						if(tmCookTime[1]==0)
						{
							tmCookTime[1] = 59;
							tmCookTime[2]--;
						}
						else
						{
							tmCookTime[1]--;
						}
					}
					else 
					{
						if(m_u8CookMode==FUNC_ID_YOGHURT)
						{
							if(tmCookTime[1]%15)
							{
								tmCookTime[1] =  (tmCookTime[1]/15)*15;
							}
							else
							{
								if(tmCookTime[1]==0)
								{
									tmCookTime[1] = 45;
									tmCookTime[2]--;
								}
								else
								{
									tmCookTime[1] -= 15;
								}
							}
						}
						else
						{                           
    					    if(tmCookTime[1]%5)
    					    {
    						    tmCookTime[1] = (tmCookTime[1]/5)*5;
    					    }
    					    else
    					    {
    						    if(tmCookTime[1]==0)
    						    {
    							    tmCookTime[1] = 55;
    							    tmCookTime[2]--;
    						    }
    						    else
    						    {
    							    tmCookTime[1] -= 5;
    						    }
    					    }
						}
					}	
				}
				JudgeTimeValid();		//判断时间是否在合理范围内
				tmCookTime[0] = 59;
				m_u8TimeDispCnt = 62;
				m_u8AutoRunCnt = 5;
			}
		}

		if(bTempDisp_EN)			//温度是否允许调整
		{
			if((IsNewKey())||((g_u16KeyRpyCnt>15)&&(g_u16KeyRpyCnt%3==0)))
			{
				//判断烹饪温度是否溢出
				if(u8CookTemp==MinCookTemp)
				{
					u8CookTemp = MaxCookTemp;
				}
				else
				{
                    if((m_u8CookMode==FUNC_ID_BAKE)||(m_u8CookMode==FUNC_ID_FRY)||(m_u8CookMode==FUNC_ID_OPORRIDGE)
                    ||(m_u8CookMode==FUNC_ID_SOUP)||(m_u8CookMode==FUNC_ID_STEW)||((m_u8DIYMode>0)&&(m_u8CookMode==0)))
					{
						u8CookTemp -= 10;
					}
					if(m_u8CookMode==FUNC_ID_REHEAT)
					{
						u8CookTemp -= 20;
					}
				}
				JudgeTimeValid();	//判断温度是否在合理范围内
				m_u8TimeDispCnt = 62;
				m_u8AutoRunCnt = 5;
			}			
		}

        if(bPowerAdjSt)
        {
            if(IsNewKey())
            {
                power_value--;
                if((power_value>=58)||(power_value<=38))
	            {
		            power_value=46;
	            }
            }
        }
	}

    if(TstBit(u8Key,BIT_KEY_DIY))
    {
        if((u8Key==VAL_KEY_DIY)&&(g_u16KeyRpyCnt==40))				//长按6S	
        {
            if(bBookTime_EN==0)					                    //无预约
            {
                if(m_u8CookMode==FUNC_ID_STEAM)
                {
                    if(bPowerAdjSt==0)
                    {
                        bPowerAdjSt = 1;
                        bTimeDisp_EN = 0;		
				        bTempDisp_EN = 0;	
                        PlaySound(MusicPoweron);
                    }
                }
            }
        }
    }

    //The assembly line boil water test
    if((u8Key==VAL_KEY_TEST)&&(g_u16KeyRpyCnt==12))
    {
        if(bBookTime_EN==0)
        {
            if(m_u8CookMode==FUNC_ID_REHEAT)
            {
                u8BolingWaterTest = 1;      //start boling water test
    		    SetCurScript(m_u8CookMode,m_u8DIYMode,u8CookTemp);  	
    		    bTimeDisp_EN = 0;		
    		    bTempDisp_EN = 0;
                LEDDispMovie(0);	
                PlaySound(MusicPoweron);
            }
        }
    }
    
	#ifdef	DEBUG0
	//定时键按下处理
	if(TstBit(u8Key,BIT_KEY_PRESET))
	{
		m_u8CurTimeDispCnt = 6;		
		if(IsNewKey())
		{
			u8CHDispCnt[0]++;				//通道显示计数器加一
			if(u8CHDispCnt[0]>=4)
			{
				u8CHDispCnt[0] = 0;
			}
		}
  	}
	#endif
	
	//恢复工作状态显示
	if(bLEDComeback)
	{
		bLEDComeback = 0;
		bSysPwrOn =	1;
	}
	
	if(bSys_1000MS)
	{
		if(m_u8CurTimeDispCnt>0)
		{
			m_u8CurTimeDispCnt--;
			if(m_u8CurTimeDispCnt==0)
			{
				bSysPwrOn =	1;
				u8CHDispCnt[0] -= 1;
                u8CHDispCnt[1] = 0;
		   	}
	  	}
		
		if(m_u8AutoRunCnt>0)
		{
			m_u8AutoRunCnt--;
			if(m_u8AutoRunCnt==0)
			{
				PlaySound(MusicPoweron);
				SetCurScript(m_u8CookMode,m_u8DIYMode,u8CookTemp);  	
				bTimeDisp_EN = 0;		
				bTempDisp_EN = 0;			
			}
		}
 	}
		
	if(bSysPwrOn)
	{
		LEDClearFun();
		LEDDispCookMode(m_u8CookMode,m_u8DIYMode);		//显示功能栏
	    if((m_u8CookMode==FUNC_ID_SOUP)||(m_u8CookMode==FUNC_ID_BAKE)||(m_u8CookMode==FUNC_ID_STEW)
	    ||(m_u8CookMode==FUNC_ID_STEAM))
		{
			LEDDispFoodType(m_u8FoodType);
		}	
	}

	if(bBookTime_EN==0)
	{
		//无预约
		bCookBySctEN = 1;		//允许功能脚本运行		
		bHeatPwrReq = 1;        //无预约，打开加热总开关
		if((tmCookTime[0]==59)&&(tmCookTime[1]==0)&&(tmCookTime[2]==0))
		{	
			//无定时，无预约
			if(m_u8CurTimeDispCnt==0)
			{
				if(bSysPwrOn)
				{
					LEDClearNum();			
					LEDDispMovie(0);	
			  	}

				//显示旋转图框
				m_u8MovCnt++;
				if((m_u8MovCnt&0x0f)==15)
				{
					if(((m_u8MovCnt>>4)&0x0f)==13)
					{
						m_u8MovCnt	=	0x1f;
				   	}
					LEDDispMovie((m_u8MovCnt>>4)&0x0f);
			  	}
			}
			else
			{	
				#ifdef	DEBUG0
				switch(u8CHDispCnt[0])
				{
					case	0:	
					case	1:
						m_DataTmp =	ADCGetData(u8CHDispCnt[0]);	
					    break;
					case	2:
						m_DataTmp =	GetScriptStage();
						break;
					case	3:
						m_DataTmp =	g_u16RelayTime/60;
						break;
					default:
						break;
				}
				LEDDispMessage(m_DataTmp,u8CHDispCnt[0]);	
				#endif
			}
		}
		else	
		{	
			//有定时，无预约
			if((bTimeDisp_EN==1)||(bTempDisp_EN==1))			
			{		
				m_u8TimeDispCnt++;
				if(m_u8TimeDispCnt==63)
				{
					bLED_Timer = 0;	 			//CLR Timer
					bLED_Temp =	0;				//CLR Temperature
					bLED_Preset = 0;			//CLR Preset
					bLED_Dot1 =	1;				//SET Dot
					bLED_Dot2 =	1;
					
					if(bTimeDisp_EN==1)
					{
						bLED_Timer = 1;					
						LEDDispTime(tmCookTime);	//显示烹饪时间
					}
					else
					{
						if(bTempDisp_EN==1)
						{
							LEDDispTemp(u8CookTemp);//显示烹饪温度
						}
					}
				}

				if(m_u8TimeDispCnt==126)
				{
					m_u8TimeDispCnt	= 0; 
					LEDClearNum();
				}
			}
			else
			{
				if((m_u8CookMode==FUNC_ID_PASTA)&&(bTimeCountDown==0))
				{
				    if(bTempAch1st==0)
                    {
					    bLED_Dot1 = 1;
					    bLED_Dot2 = 1;
                        m_u8TimeDispCnt = 0;
                    }
                    else
                    {
                        if(bSys1S_CookTime) 
                        {
                            bSys1S_CookTime = 0;
                            m_u8TimeDispCnt++;
                            if(m_u8TimeDispCnt>=20)
                            {
                                m_u8TimeDispCnt = 0;
                                PlaySound(MusicInputFood);
                            }
                           	bLED_Dot1	^=	1;		//NOT Dot
						    bLED_Dot2	^=	1;
                            if(bLED_Dot1)
                            {
                                LEDDispTime(tmCookTime);//显示烹饪时间  
                                bLED_Timer = 1;			//SET Timer
					            bLED_Preset	= 0;		//CLR Preset
					            bLED_Temp = 0;			//CLR Temp
                            }
                            else
                            {
                                LEDClearNum();
                            }
                        }
                        return;
                    }
				}
			    else
				{
					if(bSys1S_CookTime)
					{
						bSys1S_CookTime	= 0;
						DecTime(tmCookTime);
						bLED_Dot1	^=	1;		//NOT Dot
						bLED_Dot2	^=	1;	
						if((tmCookTime[0]==59)&&(tmCookTime[1]==0)&&(tmCookTime[2]==0))
						{
//						    if((m_u8DIYMode==4)&&(m_u8DIYCookStage==1))
//                            {
//                                //载入DIY4 Stage2参数
//                                m_u8DIYCookStage = 2;                       //启动DIY2
//                                m_u8CookMode = m_u8DIYCookMode[1];          //载入DIY2参数
//                                m_u8FoodType = m_u8DIYFoodType[1];
//                                tmCookTime[1] = tmDIY2CookTime[1];
//                                tmCookTime[2] = tmDIY2CookTime[2];
//                                u8CookTemp = u8DIY2CookTemp;  
//                                SetCurScript(m_u8CookMode,m_u8DIYMode,u8CookTemp);
//                                bTimeDisp_EN = 0;		
//    			                bTempDisp_EN = 0;			
//    			                m_u8AutoRunCnt = 0;
//                                bLEDComeback = 1;                           //恢复DIY2显示
//                            }
//                            else
//                            {
                        	    if((m_u8CookMode==FUNC_ID_PASTA)||(m_u8CookMode==FUNC_ID_YOGHURT)||(m_u8CookMode==FUNC_ID_CHEESE)
                                ||(m_u8CookMode==FUNC_ID_JAM)||(m_u8CookMode==FUNC_ID_DOUGH)||((m_u8DIYMode>0)&&(m_u8CookMode==0))||(bWarm_EN==0))
							    {
								    EnterReady();
							    }
							    else
							    {
								    EnterWarm();
							    }
							    bWarm_EN = 1;				
//                            }
                            PlaySound(MusicCookEnd);
						}
					}
				}

				if(m_u8CurTimeDispCnt==0)
				{
                    if((m_u8CookMode==FUNC_ID_REHEAT)&&(u8BolingWaterTest==1))       
                    {
                        //boling water mode display a ring
                    	m_u8MovCnt++;
                    	if((m_u8MovCnt&0x0f)==15)
                    	{
                    		if(((m_u8MovCnt>>4)&0x0f)==13)
                    		{
                    			m_u8MovCnt	= 0x1f;
                    	   	}
                    		LEDDispMovie((m_u8MovCnt>>4)&0x0f);
                      	}	
                        bLED_Timer = 0;
                        bLED_Preset = 0;
                        bLED_Temp = 0;
                        bLED_Dot1 = 0;
                        bLED_Dot2 = 0;
                    }
                    else
                    {
    				    //other functions display cooking time
    					LEDDispTime(tmCookTime);
    					bLED_Timer = 1;			//SET Timer
    					bLED_Preset	= 0;		//CLR Preset
    					bLED_Temp = 0;			//CLR Temp
    		        }
				}
				else
				{	
					#ifdef	DEBUG0
					switch(u8CHDispCnt[0])
					{
						case	0:	
						case	1:
							m_DataTmp =	ADCGetData(u8CHDispCnt[0]);	
					        break;
						case	2:
							m_DataTmp =	GetScriptStage();
							break;
						case	3:
						    m_DataTmp =	g_u16RelayTime/60;
							break;
						default:
							break;
					}
					LEDDispMessage(m_DataTmp,u8CHDispCnt[0]);
					#endif
				}
			}
		}
	}
	else
	{
	    //有预约
	    bHeatPwrReq = 0;        //关所有加热

		if(bCalBookTime_EN)
		{
			bCalBookTime_EN	=	0;			//清预约计算

			tmIntCookTime[2]	=	tmBookTime[2];
			tmIntCookTime[1]	=	tmBookTime[1];

			//计算烹饪需要的时间
			tmCalCookTime[2]	=	tmCookTime[2];
			tmCalCookTime[1]	=	tmCookTime[1];
			if((tmCookTime[0]==59)&&(tmCookTime[1]==0)&&(tmCookTime[2]==0))
			{
				tmCalCookTime[2]	=	0;
				tmCalCookTime[1]	=	50;
			}

			//间隔时间是否满足烹饪所需时间
			if(tmIntCookTime[2]<tmCalCookTime[2])
			{
				bBookTime_EN	=	0;			//间隔时间小于烹饪时间，取消预约
				bLEDComeback	=	1;			//恢复正常烹饪显示
		  	}
			else	
			{
				if(tmIntCookTime[2]==tmCalCookTime[2])
				{
					if(tmIntCookTime[1]<=tmCalCookTime[1])
					{
						bBookTime_EN	=	0;			//间隔时间小于烹饪时间，取消预约
						bLEDComeback	=	1;			//恢复正常烹饪显示
					}
				}
		  	}
		}
		else
		{
			if((tmCalCookTime[2]==tmBookTime[2])&&(tmCalCookTime[1]==tmBookTime[1]))
			{
				//烹饪开始时间==当前时间
				bBookTime_EN	=	0;				//预约标志取消
				bLEDComeback	=	1;				//恢复到烹饪状态显示
		  	}
		}

		if(bSys1S_BookTime)
		{
			bSys1S_BookTime	= 0;
			DecTime(tmBookTime);
			bLED_Dot1 ^= 1;				//NOT Dot
			bLED_Dot2 ^= 1;	
		}

        if(m_u8CurTimeDispCnt==0)
        {
		    //显示预约时间
    		LEDDispTime(tmBookTime);
    		bLED_Timer = 0;	 			//CLR Timer
    		bLED_Temp = 0;
    		bLED_Preset	= 1;			//SET Preset	
        }
        else
        {
            if(u8CHDispCnt[1]==0)       //Display Cooktime
            {
                LEDDispTime(tmCookTime);
				bLED_Timer = 1;			//SET Timer
				bLED_Preset	= 0;		//CLR Preset
				bLED_Temp = 0;			//CLR Temp   
				bLED_Dot1 = 1;
                bLED_Dot2 = 1;
            }
            if(u8CHDispCnt[1]==1)       //Display Cooktemp
            {
                LEDDispTemp(u8CookTemp);//显示烹饪温度
            }
        }
	}
    
    //Display DIY xx 2s
//    if(u16DispDIYCnt>0)
//    {
//        u16DispDIYCnt--;
//        LEDClearFun();
//        LEDDispDIYNum(m_u8DIYMode,m_u8DIYCookStage);
//        if(u16DispDIYCnt==0)
//        {
//            bLEDComeback = 1;
//        }
//    }

    if(bPowerAdjSt)             //调功时数码管显示power_value
    {
        LEDDispTemp(power_value);
        bLED_Temp = 0;			//CLR Temp  
    }
}

/*-----------------------------------------------------------------------------*
 *  Function Description:                                                      *
 *      保温状态处理                                     					   *
 *  Parameters:                                                                *
 *      None                                                                   *
 *  Return                                                                     *
 *      None                                                                   *
 *----------------------------------------------------------------------------*/
static	void	WarmProc(void)
{
	uint8	u8Key;
	uint8	u8TopDegree = ADCGetData(AD_CH_TOP);

	u8Key =	GetKey();

    bHeatPwrReq =   1;      //保温状态下，打开加热总开关
    
	if(m_u8ChkCnt<6)
	{
//		if(m_u8ChkCnt>=5)
//		{
			g_u8HiTemp	=	75;
			g_u8LoTemp	=	70;
			g_u8OnTime_Set	=	0xf0;
			bHeatTopReq	=	0;
//		}
//		else
//		{
//			g_u8HiTemp	=	0;
//			bHeatTopReq	=	1;
//		}
		if(bSys_1000MS)	m_u8ChkCnt++;	
	}
    else
    {
    	g_u8HiTemp	=	75;
        g_u8LoTemp	=	70;
        g_u8OnTime_Set	=	10;
        g_u8OffTime_Set	=	20;
//        if(u8TopDegree<80)	bHeatTopReq	=	1;
//        if(u8TopDegree>85)	bHeatTopReq	=	0;
    }
		
	if(TstBit(u8Key,BIT_KEY_CANCEL))
	{
		if(IsNewKey())
		{
			EnterReady();
            PlaySound(MusicCancelFunc);
			return;
	   	}
 	}

    if(TstBit(u8Key, BIT_KEY_START))
    {
        if(IsNewKey())
        {
            EnterReady();
            PlaySound(MusicCancelFunc);
			return;  
        }
    }

	if(bSys1S_CookTime)
	{
		bSys1S_CookTime	= 0;
		IncTime(tmCookTime);
        LEDClearFun();
		LEDDispTime(tmCookTime);
		bLED_Dot1 ^= 1;			//NOT Dot
		bLED_Dot2 ^= 1;
				
//		if(m_u8CookMode==FUNC_ID_BABYFOOD)
//		{
//			if((tmCookTime[2]==1)&&(tmCookTime[1]==0)&&(tmCookTime[0]==0))
//			{
//				EnterReady();
//			}	
//		}
//		else
//		{
			if((tmCookTime[2]==23)&&(tmCookTime[1]==59)&&(tmCookTime[0]==59))
			{
				EnterReady();
			}
//		}
	}
}
			
/*-----------------------------------------------------------------------------*
 *  Function Description:                                                      *
 *      系统报警状态处理                                     				   *
 *  Parameters:                                                                *
 *      None                                                                   *
 *  Return                                                                     *
 *      None                                                                   *
 *----------------------------------------------------------------------------*/
static	void	ErrorProc(void)
{
	uint8	u8Key;
	u8Key =	GetKey();

	if(TstBit(u8Key,BIT_KEY_CANCEL))
	{
		if(IsNewKey())
		{
			EnterReady();
			if(bKeyMusic_EN==0)
			{
				PlaySound(MusicOFF);
			}
			return;
	   	}
 	}
	
	if(g_u8ErrorType==0)			
	{
		//报警解除
		m_u8SysID =	m_u8PrevSysID;
		bLEDComeback = 1;
		PlaySound(MusicKeyDown);
		return;
  	}
		
	LEDDispError();
}

/*-----------------------------------------------------------------------------*
 *  Function Description:                                                      *
 *      系统OFF模式,关闭所有加热和显示                                         *
 *  Parameters:                                                                *
 *      None                                                                   *
 *  Return                                                                     *
 *      None                                                                   *
 *----------------------------------------------------------------------------*/
static void PowerDownProc(void)
{
    uint8 u8Key;	
	u8Key = GetKey(); 

    if(u8Key>0)         //按下任何键唤醒进入待机
    {
        if(IsNewKey())
        {
            EnterReady();  
            if(bKeyMusic_EN==0)PlaySound(MusicKeyDown);
            return;
        }
    }

    if(m_u8DispDelayCnt>0)
    {
        if(bSys_1000MS) m_u8DispDelayCnt--;
        if(m_u8DispDelayCnt==0)
        {
            LEDDispAll(0);      //关闭所有显示
        }
        else
        {
            m_u16Movcnt++;
            if((m_u16Movcnt&0x00ff)==62)
            {
                m_u16Movcnt = (m_u16Movcnt&0xff00)+0x0100;
                if(((m_u16Movcnt>>8)&0x00ff)==6)
        	    {
        	        m_u16Movcnt = 0;
        	    }
        	    LEDDispPowerSaving((m_u16Movcnt>>8)&0x00ff);
            }  
	    } 
    }
    else
    {
        LEDDispAll(0);      //关闭所有显示
        bHeatPwrReq = 0;    //关所有加热
        g_u8HiTemp = 0;
        bHeatTopReq	=	0;  //关闭加热
    }
}

/*-----------------------------------------------------------------------------*
 *  Function Description:                                                      *
 *      系统自检状态处理                                     				   *
 *  Parameters:                                                                *
 *      None                                                                   *
 *  Return                                                                     *
 *      None                                                                   *
 *----------------------------------------------------------------------------*/
static	void	SelfCheckProc(void)
{
	uint8	u8Key;
	u8Key =	GetKey();

	if(TstBit(u8Key,BIT_KEY_CANCEL))
	{
		if(IsNewKey())
		{	
		    if(m_u8SelfChkStep==1)
            {
                m_u8SelfChkStep++;
            }
            else
            {
                EnterReady();
            }
		}
  	}

    if(TstBit(u8Key,BIT_KEY_PLUS))
    {
        if(IsNewKey())
        {
            if((m_u8SelfChkStep==0)||(m_u8SelfChkStep==6))
            {
                m_u8SelfChkStep++;
            }
        }
    }

    if(TstBit(u8Key,BIT_KEY_TIMER))
    {
        if(IsNewKey())
        {
            if(m_u8SelfChkStep==2)
            {
                m_u8SelfChkStep++;
            }
        }
    }

    if(TstBit(u8Key,BIT_KEY_PRESET))
    {
        if(IsNewKey())
        {
             if(m_u8SelfChkStep==3)
             {
                m_u8SelfChkStep++;
             }
        }
    }

    if(TstBit(u8Key,BIT_KEY_MINUS))
    {
        if(IsNewKey())
        {
            if(m_u8SelfChkStep==4)
            {
                 m_u8SelfChkStep++;
            }
        }
    }

    if(TstBit(u8Key,BIT_KEY_DIY))
    {
        if(IsNewKey())
        {
            if(m_u8SelfChkStep==5)
            {
                 m_u8SelfChkStep++;
            }
        }
    }

    if(TstBit(u8Key,BIT_KEY_MENU))
    {
        if(IsNewKey())
        {
            if((m_u8SelfChkStep==7)||(m_u8SelfChkStep==8)||(m_u8SelfChkStep==9)
            ||(m_u8SelfChkStep==10)||(m_u8SelfChkStep==11)||(m_u8SelfChkStep==12))
            {
                m_u8SelfChkStep++;  
                if(m_u8SelfChkStep==11)
                {
                    bHeatPwrReq =  1;
                    g_u8HiTemp = 102;
    		        g_u8LoTemp = 100;
    		        g_u8OnTime_Set = 0xf0;
                    bHeatTopReq	=	0;
                }
                if(m_u8SelfChkStep==12)
                {
                    bHeatPwrReq = 0;        //关所有加热
                    g_u8HiTemp = 0;	        //停止加热	
                    bHeatTopReq	=	0;
                    g_u8OnTime_Set = 0;
                    PlaySound(MusicPoweron);
                }
                if(m_u8SelfChkStep==13)
                {
                    EnterReady();
                }
            }
        }
    }
}

/*-------------------------------状态入口程序----------------------------------*
/*-----------------------------------------------------------------------------*
 *  Function Description:                                                      *
 *      进入复位模式                                     				       *
 *  Parameters:                                                                *
 *      None                                                                   *
 *  Return                                                                     *
 *      None                                                                   *
 *----------------------------------------------------------------------------*/
void	EnterReset(void)
{
	uint8 code *p_EepRom;
	uint8 i;
    
	p_EepRom = EEP_SECTOR_ADDR;
    for(i=0;i<EEPROMPAGESIZE;i++)
    {
        rEepromCache[i] = *p_EepRom;    //Read data form Eeprom to cache
        p_EepRom++;
    }	
	bKeyMusic_EN = 0;                  //复位中关闭按键声音
	m_u8SysID =	SYS_ID_RESET;
	bSys_1000MS	= 0;
    u16PoweronCnt = 320;         
	bWarm_EN = 1;	
    if(rEepromCache[EEP_POWER_S0]==0xA5)
    {
        power_value = rEepromCache[EEP_POWER_VAL];  //读取Power_value
    }
    else
    {
        power_value = 46;;
    }
	PlaySound(MusicPoweron);		
}

/*-----------------------------------------------------------------------------*
 *  Function Description:                                                      *
 *      进入待机模式                                     				       *
 *  Parameters:                                                                *
 *      None                                                                   *
 *  Return                                                                     *
 *      None                                                                   *
 *----------------------------------------------------------------------------*/
void	EnterReady(void)
{
	m_u8SysID =	SYS_ID_READY;					//系统进入待机状态
	ResetCurScript();							//禁止Script运行
	bHeatPwrReq =   0;                              //关所有加热
	g_u8HiTemp = 0;								//停止加热	
	bHeatTopReq	=	0;						    //关闭顶加热
		
	tmCookTime[0] =	59;							//清烹饪时间
	tmCookTime[1] =	0;
	tmCookTime[2] =	0;
	tmBookTime[0] =	59;	  						//清预约时间
	tmBookTime[1] =	0;
	tmBookTime[2] =	0;
	m_u8ChkCnt = 0;								//清自检计数器
	g_u8ErrorType =	0;							//清报警类型

	bBookTime_EN = 0;
	bTempDisp_EN = 0;
	bCalBookTime_EN	= 0;
	bTimeDisp_EN = 0;		
	m_u8CurTimeDispCnt = 0;	
	m_u8DispDelayCnt = 60; 
	m_u8AutoRunCnt = 0;
	u8ErrorCnt = 0; 
    u8NoPanCnt = 0;
    u8VoltageCnt = 0;
    u16IGBTCnt = 0;
    bPowerAdjSt = 0;
    m_u8SelfChkStep = 0;
    m_u8DIYMode = 0;
    m_u8DIYSwitch = 0;
    m_u8DIYCookStage = 0;
    u16DispDIYCnt = 0;
        
	LEDClearFun();
	m_u8CookMode = 0;
    m_u8FoodType = 0;   
    u8BolingWaterTest = 0;
}

/*-----------------------------------------------------------------------------*
 *  Function Description:                                                      *
 *      进入保温模式                                     				       *
 *  Parameters:                                                                *
 *      None                                                                   *
 *  Return                                                                     *
 *      None                                                                   *
 *----------------------------------------------------------------------------*/
void	EnterWarm(void)
{
	m_u8SysID =	SYS_ID_WARM;
	ResetCurScript();							//禁止Script运行
	bHeatPwrReq =   0;                          //关所有加热
	g_u8HiTemp = 0;		
    bHeatTopReq	=	0;						    //关闭顶加热
	tmCookTime[0] =	0;							//清烹饪时间
	tmCookTime[1] =	0;
	tmCookTime[2] =	0;
	bBookTime_EN = 0;
	bCalBookTime_EN	= 0;
	bTimeDisp_EN = 0;    						//时间不允许调整
	bTempDisp_EN = 0;
	m_u8ChkCnt = 0;								//清自检计数器
	LEDClearFun();
	LEDDispTime(tmCookTime);
}

/*-----------------------------------------------------------------------------*
 *  Function Description:                                                      *
 *      进入关机模式                                    				       *
 *  Parameters:                                                                *
 *      None                                                                   *
 *  Return                                                                     *
 *      None                                                                   *
 *----------------------------------------------------------------------------*/
 void   EnterPowerDown(void)
 {
    m_u8SysID = SYS_ID_POWRDOWN;
    ResetCurScript();               //禁止Script运行
    bHeatPwrReq =   0;              //关所有加热
    g_u8HiTemp = 0;
    bHeatTopReq	=	0;			    //关闭顶加热
	m_u8DispDelayCnt = 0;           //进入OFF模式
    LEDClearFun();
 }
/*-------------------------------故障检测程序----------------------------------*

/*-----------------------------------------------------------------------------*
 *  Function Description:                                                      *
 *      故障检测                                      		   				   *
 *  Parameters:                                                                *
 *      None                                                                   *
 *  Return                                                                     *
 *      None                                                                   *
 *----------------------------------------------------------------------------*/
void	FaultCheckProc(void)
{
	uint8	u8BotDegree	=	ADCGetData(AD_CH_BOT);
	uint8   u8TopDegree =   ADCGetData(AD_CH_TOP);
	uint16	adBotData	=	ADCGetVal(AD_CH_BOT);
	uint16	adTopData	=	ADCGetVal(AD_CH_TOP);

	if(bCookBySctEN==1)
	{
		if((m_u8CookMode==FUNC_ID_FRY)||(m_u8CookMode==FUNC_ID_BAKE))
		{
//			if(bTempAch1st==0)
//			{
//				if(u8CookTemp==160)
//				{
//					if(u8BotDegree>=148)
//					{
//						bTempAch1st = 1;
//					}
//				}

//				if(u8CookTemp==140)
//				{
//					if(u8BotDegree>=128)
//					{
//						bTempAch1st = 1;
//					}
//				}
//                
//				if(u8CookTemp==130)
//				{
//					if(u8BotDegree>=120)
//					{
//						bTempAch1st = 1;
//					}
//				}
//                
//				if(u8CookTemp==100)
//				{
//					if(u8BotDegree>=98)
//					{
//						bTempAch1st = 1;
//					}
//				}
//                bHeatTopReq = 1;
//			}
//			else
//			{
//				if(m_u8ChkCnt<60)					//到温度点后延迟提示
//				{
//					if(bSys_1000MS)	m_u8ChkCnt++;
//                    bHeatTopReq = 1;                
//				}
//                else
//                {   
//                    bHeatTopReq = 0;
//                }
//			}
		}
		else if(m_u8CookMode==FUNC_ID_PASTA)
		{
			if(bTempAch1st==0)
			{
				if(u8TopDegree>=84)
				{
					bTempAch1st = 1;
					PlaySound(MusicAchievedTemp);
				}
			}	
		}	
	}

    //干烧
    if(bCookBySctEN==1)
	{   
	    if(bSys_1000MS)
		{
            if(u8BotDegree>=160)
            {
                u8ErrorCnt++;
                if(u8ErrorCnt>=3)
                {
                    if((m_u8CookMode!=0)&&(m_u8CookMode!=FUNC_ID_BAKE)&&(m_u8CookMode!=FUNC_ID_FRY))
                    {
                        bPanOverTempErr	= 1;			    
                    }
                }         
			}
            else
            {
                u8ErrorCnt = 0;
            }
        }
	}

	if((m_u8SysID==SYS_ID_CHECK)||(m_u8SysID==SYS_ID_ERROR)||(bCookBySctEN==1)||(m_u8SysID==SYS_ID_WARM))
	{
	    //热敏电阻开短路
		if((adBotData<3)||(adBotData>252))	bBotSensorErr=1;
		else    bBotSensorErr=0;	
		if((adTopData<3)||(adTopData>252))	bTopSensorErr=1;
		else    bTopSensorErr=0;	
	

        //判锅
        if(TstBit(IHStatus,4))
        {
            u8NoPanCnt++;
            if(u8NoPanCnt>=20)
            {
                u8NoPanCnt = 0;
                bNoPanErr = 1;
            }
        }
        else  
        {
            u8NoPanCnt = 0;
            bNoPanErr = 0;
        }
    

        //IGBT过热
        if(IGBTValue>=90)
        {
            bIGBTOverTempErr = 1;
        }
        if(bIGBTOverTempErr==1)
        {
            if(IGBTValue<=80)
            {
                bIGBTOverTempErr = 0;
            }
        }

        //IGBT热敏电阻开路
        if(bSys_1000MS)
        {
            if(IGBTValue<3)
            {
                u16IGBTCnt++;
                if(u16IGBTCnt>=600)
                {
                    u16IGBTCnt = 0;
                    bIGBTSensorErr = 1;
                }
            }
            else
            {
                u16IGBTCnt = 0;
                bIGBTSensorErr = 0;
            }
        }
        
        //电压过高或过低
        if((VoltageValue>=205)||(VoltageValue<=115))
        {
            u8VoltageCnt++;
            if(u8VoltageCnt>=20)
            {
                u8VoltageCnt = 0;
                bVoltageErr = 1;
            }
        }
        else
        {
            u8VoltageCnt = 0;
            bVoltageErr = 0;
        }
    }
    
	if((m_u8SysID==SYS_ID_RUN)||(m_u8SysID==SYS_ID_WARM)||(m_u8SysID==SYS_ID_CHECK))
	{
		if(g_u8ErrorType)
		{
			m_u8PrevSysID = m_u8SysID;
			m_u8SysID = SYS_ID_ERROR;
            if(bNoPanErr==1)
            {
                PlaySound(MusicSysErrorNoPan);      //开无锅报警声音
            }
            else
            {
			    PlaySound(MusicSysError); 	        //开报警声音		
		    }
			bHeatPwrReq =   0;                      //关所有加热
			g_u8HiTemp	=	0;
            bHeatTopReq	=	0;					    //关闭顶加热
		}
 	}
}

/*------------------------Autotest activate Process----------------------------*

/*-----------------------------------------------------------------------------*
 *  Function Description:                                                      *
 *      Plug in and in the same time  press the start botton during 2s         *
 *  Parameters:                                                                *
 *      None                                                                   *
 *  Return                                                                     *
 *      None                                                                   *
 *----------------------------------------------------------------------------*/
void    AutotestActivate(void)
{
    if(u16PoweronCnt>0)
    {
        u16PoweronCnt--;
        if(m_u8KeyVal==VAL_KEY_START)
        {
            if(g_u16KeyRpyCnt>12)
            {
                if(m_u8SysID!=SYS_ID_CHECK)
                {
                    m_u8SysID = SYS_ID_CHECK;
                    ResetCurScript();							//禁止Script运行
	                bHeatPwrReq =   0;                          //关所有加热
	                g_u8HiTemp = 0;								//停止加热	
	                bHeatTopReq	=	0;						    //关闭顶加热
			        m_u8SelfChkStep = 0;
                    PlaySound(MusicKeyDown);
                }
            }
        }
    }
}
