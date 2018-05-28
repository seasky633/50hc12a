#ifndef _SYSTEM_H
#define	_SYSTEM_H

/*-----------------------------*
 *  include head files         *
 *----------------------------*/

/*-----------------------------*
 *  Macros for constants       *
 *----------------------------*/
#define     BIT_KEY_START		0	
#define		BIT_KEY_PRESET		1	
#define		BIT_KEY_MINUS		2	
#define     BIT_KEY_MENU	 	3
#define		BIT_KEY_DIY 		4
#define		BIT_KEY_PLUS		5
#define		BIT_KEY_TIMER 		6
#define		BIT_KEY_WARM		7	
#define		BIT_KEY_CANCEL		7	

#define		VAL_KEY_START		0x01
#define		VAL_KEY_PRESET		0x02
#define     VAL_KEY_MINUS		0x04
#define		VAL_KEY_MENU		0x08
#define		VAL_KEY_DIY 		0x10
#define		VAL_KEY_PLUS		0x20
#define		VAL_KEY_TIMER 		0x40
#define		VAL_KEY_WARM		0x80
#define		VAL_KEY_CANCEL		0x80
#define     VAL_KEY_TEST        0x24

//系统状态
#define		SYS_ID_RESET	0  		//系统复位的ID
#define		SYS_ID_READY	1		//系统待机的ID
#define     SYS_ID_SETUP    2       //系统功能设置的ID
#define	    SYS_ID_RUN		3		//系统开机运行的ID
#define	    SYS_ID_WARM		4		//系统保温的ID
#define     SYS_ID_POWRDOWN 5       //系统掉电的ID
#define     SYS_ID_ERROR    6       //系统报警的ID
#define		SYS_ID_CHECK	7		//系统自检的ID

//功能ID
#define     FUNC_ID_SOUP        1
#define     FUNC_ID_BAKE        2
#define     FUNC_ID_STEW        3
#define     FUNC_ID_STEAM       4
#define     FUNC_ID_FRY         5
#define     FUNC_ID_RICE        6
#define     FUNC_ID_PILAF       7
#define     FUNC_ID_OPORRIDGE   8
#define     FUNC_ID_JAM         9
#define     FUNC_ID_PASTA       10
#define     FUNC_ID_YOGHURT     11
#define     FUNC_ID_CHEESE      12
#define     FUNC_ID_DOUGH       13
#define     FUNC_ID_REHEAT      14
#define     FUNC_ID_DIY         15

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
extern	void	UIProc(void);
extern	void	FaultCheckProc(void);
extern  void    AutotestActivate(void);
extern	void	EnterReset(void);
extern	void	EnterReady(void);
extern  void	EnterWarm(void);

/*-----------------------------*
 *  public variable declaration*
 *----------------------------*/
extern	uint8	m_u8SysID;
extern  uint8   m_u8ChkCnt;
extern	uint8	g_u8SecTick;
extern  uint8   m_u8PreTmp;
extern  uint8   u8CookTemp;
extern  uint8   m_u8DIYMode;
extern  uint8   m_u8CookMode;
extern  uint8 xdata u8BolingWaterTest;
   
#endif
