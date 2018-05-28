/*-----------------------------*
 *  include head files         *
 *----------------------------*/
#include ".\Head\HD_cfg.h"
#include ".\Head\System.h"

/*-----------------------------*
 *  Macros for constants       *
 *----------------------------*/
//********通信端口定义***********
#define SDA_I   P0IO&= 0xfb
#define SDA_O	P0IO|= 0x04
#define SDA 	P02
#define SCL 	P03

/*-----------------------------*
 *  structure,union and enum   *
 *----------------------------*/

/*-----------------------------*
 *  public functions prototypes*
 *----------------------------*/
bit IICStart(void);
bit IICStop(void);
bit Data8bitWrite(uint8 datatemp);
bit Data8bitRead(bit acktempflag);
void CHK_IH_Connector(void);
void CHK_IH_Com_Set(void);

/*-----------------------------*
 *  public variable declaration*
 *----------------------------*/
uint8 idata ClockCount; 	    //时钟计数器
uint8 idata WriteDataSum;	    //操作数据总数
uint8 idata ReadDataSum;	    //操作数据总数
uint8 idata DelayTime;  		//延时时间
uint8 idata BitCount;
uint8 idata IICData;
uint8 idata ComStep;
uint8 idata DataCount;
uint8 idata BZcon;
uint8 idata power_value;

uint8 idata ReadDataBuf[12];

uint8 idata WriteInitDataBuf[8] = 
{
#define		C_INIT_LOAD_CUR		4		    //电流
#define     C_AB			    0
#define		C_COM_LOAD_TEST 	0x1B		//检锅强度
#define		C_COM_MAX_PPG		0xd0		//最大限制PPG		
#define		C_COM_LOAD_LEAVE	500/25		//移锅功率上限 负载移出检测功率明码/25
#define		C_COM_LOAD_PAN		0           //1600/25			//恶劣锅具保护 恶劣负载保护功率明码/25 0不保护
#define		C_COM_LOAD_CUR 		((C_INIT_LOAD_CUR*210)/25)	//确定有锅电流(最低允许工作电压下最大PPG时，负载有效电流明码功率设置值X，X=I*210/25
#define		C_COM_MINPOWERM		600/25	    //最小工作功率明码
#define		C_COM_MAXPOWERM		1600/25     //最大工作功率明码
	C_COM_LOAD_TEST,	//负载检测参数:检锅强度
	C_COM_MAX_PPG,		//最大限制PPG 
	C_COM_LOAD_LEAVE,	//负载移出检测功率明码设置值
	C_COM_LOAD_PAN,		//恶劣负载保护功率明码设置值
	C_COM_LOAD_CUR,		//负载有效电流
	C_AB,		        //电流修正系数
	C_COM_MINPOWERM,	//最小工作功率明码设置值
	C_COM_MAXPOWERM,	//最大工作功率明码设置值
};
uint8 idata WriteRunDataBuf[4];

bit CLK_Flag;
bit ACKFlag;
bit ReadFlag;
bit WriteFlag;
bit TempFlag;
bit InitFlag;
bit TopHeatOUT;
bit FANcon;


/*-----------------------------------------------------------------------------*
 *  Function Description:                                                      *
 *      IIC通信程序--启动信号                                                  * 
 *  Parameters:                                                                *
 *      None                                                                   *
 *  Return                                                                     *
 *      Flag                                                                   *
 *----------------------------------------------------------------------------*/
bit IICStart(void)
{
    bit Flag;
	Flag = 0;
	SDA_O;
	BitCount++;
	if(BitCount == 1) 
	{
		SDA =1;
		SCL =1;
	}
	else if(BitCount == 2) 
	{
		SDA = 0;
	}
	else if(BitCount == 3) 
	{
		SCL = 0;
		Flag = 1;
	}

	return Flag;
}

/*-----------------------------------------------------------------------------*
 *  Function Description:                                                      *
 *      IIC通信程序--停止信号                                                  * 
 *  Parameters:                                                                *
 *      None                                                                   *
 *  Return                                                                     *
 *      Flag                                                                   *
 *----------------------------------------------------------------------------*/
bit IICStop(void)
{
    bit flag;
	flag = 0;
	SDA_O;
	BitCount++;
	if(BitCount == 1)
	{
		SCL = 0;
		SDA = 0;
	}
	else if(BitCount == 2) 
	{
		SCL = 1;
	}
	else if(BitCount == 3) 
	{
		SDA = 1;
		flag = 1;
	}
	return flag;
} 

/*-----------------------------------------------------------------------------*
 *  Function Description:                                                      *
 *      IIC通信程序--写8位数据                                                 * 
 *  Parameters:                                                                *
 *      None                                                                   *
 *  Return                                                                     *
 *      Flag                                                                   *
 *----------------------------------------------------------------------------*/
bit Data8bitWrite(uint8 datatemp)
{
	bit  flag;
	flag = 0;
	BitCount++;
	if(CLK_Flag)
	{
		IICData = datatemp;
		CLK_Flag = 0;
	}
	if(TempFlag)
	{
		if(BitCount & 0x01)
		{
			SCL = 0;
			SDA_O;
			if(IICData & 0x80)
			{
			   SDA = 1;
			}
			else
			{
			   SDA = 0;
			}
		 }
		 else
		 {
		 	SCL =1;
			if(BitCount >=16)
			{
				TempFlag =0;
				BitCount = 0;		
			}
			else
			{
				IICData	= IICData<<1;		
			}
		 }
	}
	else
	{	
		SDA_I;
		if(BitCount==1)
		{
			SCL =0;
		}
		else
		{
			SCL =1;
			if(SDA)
			{
				SDA_O;
				SDA = 0;
				ACKFlag = 1;
				flag = 1;
			}
			else
			{
				flag = 1;
			}
		}
	}
	return flag;
}

/*-----------------------------------------------------------------------------*
 *  Function Description:                                                      *
 *      IIC通信程序--读8位数据                                                 * 
 *  Parameters:                                                                *
 *      None                                                                   *
 *  Return                                                                     *
 *      Flag                                                                   *
 *----------------------------------------------------------------------------*/
bit Data8bitRead(bit acktempflag)
{
	bit flag;
	flag = 0;
	BitCount++;
	if(TempFlag)
	{	
		SDA_I;
		if(BitCount & 0x01)
		{
		   SCL = 0;
		}
		else
		{
			if(SDA)
			{
				IICData |= 0x01; 
			}
			else
			{
				IICData	&= ~0x01;
			}
			if(BitCount >= 16)
			{
				 BitCount = 0;
				 TempFlag = 0;
			}
			else
			{	 
				 IICData <<=1;
			}
			SCL = 1;
		}
	}
	else
	{
		SDA_O;
	  if(BitCount == 1)
		{
			SCL = 0;
			if(acktempflag)
			{
				SDA = 0;
			}
			else
			{
				SDA = 1;
			}
		}
		else if(BitCount == 2)
		{
			SCL = 1;
			flag = 1;
		}
	} 
	return flag;
}

/*-----------------------------------------------------------------------------*
 *  Function Description:                                                      *
 *      数据通讯主程序                                                         * 
 *  Parameters:                                                                *
 *      None                                                                   *
 *  Return                                                                     *
 *      None                                                                   *
 *----------------------------------------------------------------------------*/
void CHK_IH_Connector(void)
{
	uint8 temp;
	ClockCount++;
	if(ClockCount>DelayTime)
	{
		DelayTime = 0;
		if(WriteFlag)
		{
			if(ComStep == 0)  				//启动
			{
				if(IICStart())
				{
					ComStep++;
					BitCount = 0;
					TempFlag = 1;
					CLK_Flag = 1;
					DelayTime = 3;
				}
			}
			else if(ComStep == 1)  		//引导字
			{
				if(Data8bitWrite(0x50))
				{
					ComStep++;
					BitCount = 0;
					TempFlag = 1;
					CLK_Flag = 1;
				}
			}
			else if(ComStep == 2)  		//命令字
			{
				if(InitFlag)
				{
					if(Data8bitWrite(0x00))	 //初始化命令
					{
						ComStep++;
						BitCount = 0;
						TempFlag = 1;
						DataCount = 0;
						CLK_Flag = 1;
					}
				}
				else
				{
					if(Data8bitWrite(0x10))  //发送功率控制信息
					{
						ComStep++;
						BitCount = 0;
						TempFlag = 1;
						DataCount = 0;
						CLK_Flag = 1;
					}
				}
			}
			else if(ComStep == 3)  			//数据区
			{
				if(InitFlag)
				{
					if(Data8bitWrite(WriteInitDataBuf[DataCount]))
					{
						DataCount++;
						if(DataCount> 7)
						{
							 ComStep++;
						}
   					CLK_Flag = 1;
						BitCount = 0;
						TempFlag = 1;
					}

				}
				else
				{
					if(Data8bitWrite(WriteRunDataBuf[DataCount]))
					{
						DataCount++;
						if(DataCount> 3)
						{
							 ComStep++;
						}
						CLK_Flag = 1;
						BitCount = 0;
						TempFlag = 1;
					}
				}
			}
			else if(ComStep == 4)  			//结束
			{
				if(IICStop())
				{
					ComStep = 0;
					BitCount = 0;
					TempFlag = 1;
					DelayTime = 70;
					WriteFlag = 0;
				}
			}
		}
		else if(ReadFlag)
		{
			if(ComStep == 0)
			{
				if(IICStart())						//开始
				{
					ComStep++;
					BitCount = 0;
					TempFlag = 1;
					CLK_Flag = 1;
					DelayTime = 10;
				}
			}
			else if(ComStep == 1)  			//引导字
			{
				if(Data8bitWrite(0x50))
				{
					ComStep++;
					BitCount = 0;
					CLK_Flag = 1;
					TempFlag = 1;
				}
			}
			else if(ComStep == 2)				//命令字
			{
				if(Data8bitWrite(0x10))
				{
					ComStep++;
					BitCount = 0;
					CLK_Flag = 1;
					TempFlag = 1;
				}
			} 
			else if(ComStep == 3)  			//结束
			{
				if(IICStop())
				{
					ComStep++;
					BitCount = 0;
					TempFlag = 1;
					CLK_Flag = 1;
					DelayTime = 100;	 			//等待50个中断周期
				}
			}
			else if(ComStep == 4)  			//重启
			{
				if(IICStart())
				{
					ComStep++;
					BitCount = 0;
					TempFlag = 1;
					CLK_Flag = 1;
					DelayTime = 10;
				}
			}
			else if(ComStep == 5)  			//引导字
			{
				if(Data8bitWrite(0x51))		//读取
				{
					ComStep++;
					BitCount = 0;
					TempFlag = 1;
					CLK_Flag = 1;
					DataCount = 0;
				}
			}
			else if(ComStep == 6)  			//读取数据
			{
				if(DataCount<10)
				{
					temp = 1;
				}
				else
				{
					temp = 0;
				}
				if(Data8bitRead(temp))
				{
					ReadDataBuf[DataCount] = IICData;
					DataCount++;
					if(DataCount> 10)
					{
						 ComStep++;
					}
					BitCount = 0;
					TempFlag = 1;
				}
			}
			else if(ComStep == 7)  			//结束
			{
				if(IICStop())
				{
					ComStep = 0;
					BitCount = 0;
					TempFlag = 1;
					DelayTime = 100;	  		//等待50个中断周期
					ReadFlag = 0;
				}
			}
		}
		
		ClockCount = 0;
		if(ACKFlag)
		{
			WriteFlag = 0;
			ReadFlag = 0;
			ACKFlag = 0;
			SDA = 1;
			SCL = 1;

		}
	}
}

/*-----------------------------------------------------------------------------*
 *  Function Description:                                                      *
 *      IH数据设置程序                                                         * 
 *  Parameters:                                                                *
 *      None                                                                   *
 *  Return                                                                     *
 *      None                                                                   *
 *----------------------------------------------------------------------------*/
void CHK_IH_Com_Set(void)
{
	uint8 temp;
	if((bCookBySctEN==1)||(m_u8SysID==SYS_ID_WARM))
	{
		FANcon = 1;
        if(bHeatState==0)                   //IH非加热状态
        {
            if(m_u8SysID==SYS_ID_WARM)      //保温，关风扇
            {
                FANcon = 0;
            }

            if((m_u8CookMode==FUNC_ID_BAKE)||(m_u8CookMode==FUNC_ID_STEW)||(m_u8CookMode==FUNC_ID_FRY)
    	    ||(m_u8CookMode==FUNC_ID_SOUP)||(m_u8CookMode==FUNC_ID_CHEESE)||((m_u8DIYMode>0)&&(m_u8CookMode==0)))
    	    {
    	        if(u8CookTemp<160)  //160度以下关风扇，160度一直开风扇
                {
                    FANcon = 0;
                }
            }
           
            if((m_u8CookMode==FUNC_ID_YOGHURT)||(m_u8CookMode==FUNC_ID_DOUGH)) //yoghurt/dough功能,关风扇
            {
                FANcon = 0;
            }
        }
	}
	else
	{
		FANcon = 0;
    }
    
	if(IHStatus & 0x80)
	{
	  	InitFlag = 0;
	}
	else
	{
	    InitFlag = 1;
	}

	if(FANcon&TopHeatOUT)	    //风扇控制与顶部加热
	{
		POwerIO = Bin(10010110);//0xa6;		
	}
	if(FANcon&(!TopHeatOUT))
	{
		POwerIO = Bin(10110100);//0xb4;    		
	}
	if((!FANcon)&(!TopHeatOUT))
	{
		POwerIO = Bin(11110000);//0xf0;    		
	}	
	if((!FANcon)&TopHeatOUT)
	{
		POwerIO = Bin(11010010);//0xd2;    		
	}	

	if(g_u8BuzMode>0)
    {
        BZcon = BZcon+g_u8BuzMode;
        g_u8BuzMode = 0;
    }  
	
	ControlSet = (ControlSet&0xfc)+(BZcon&0x03); //蜂鸣器
	temp = 	(~ControlSet)<<4;
	ControlSet = (ControlSet&0x0f)+ (temp&0xf0);

	
	PowerSetL = 0x13;	//功率输出开关，0停止加热，非0，加热

    if((bCookBySctEN==1)||(m_u8SysID==SYS_ID_WARM)||(m_u8SysID==SYS_ID_CHECK))
    {
        if(bHeatState)  
        {
            if((power_value>=58)||(power_value<=38))
	        {
		        power_value=46;
	        }

            POwerSetH = power_value;     //功率调整，实际功率/25
        }
        else
        {
            POwerSetH = 0;
        }
    }
    else
    {
        POwerSetH = 0;
    }
        
    if((g_u8ErrorType>0)||(bHeatState==0))      //有报警，不加热
	{
	    POwerSetH = 0;
    }
	
	if((bNoPanErr==1)&&(bPanOverTempErr==0))   //无锅(且无高温报警)时强制加热
    {
        POwerSetH = power_value;
    }

 	TempFlag = 1;
    ComStep	= 0;
	BitCount = 0;
	ReadFlag = 1;
	WriteFlag = 1;
}

