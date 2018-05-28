#ifndef _LED_H_
#define	_LED_H_

/*-----------------------------*
 *  include head files         *
 *----------------------------*/

/*-----------------------------*
 *  Macros for constants       *
 *----------------------------*/

/*-----------------------------*
 *  Macros for others          *
 *----------------------------*/
// #define		IO_COM1			P17
// #define		IO_COM2			P20	
// #define		IO_COM3			P25
// #define		IO_COM4         P21
// #define		IO_COM5         P22
// #define		IO_COM6         P23
// #define		IO_COM7         P24
// #define		IO_SEG1			P16
// #define		IO_SEG2			P30
// #define		IO_SEG3			P31
// #define		IO_SEG4			P33
// #define		IO_SEG5			P34
// #define		IO_SEG6			P13
// #define		IO_SEG7			P14
// #define		IO_SEG8			P15

/*
	 a
   - - -
  f| g |b
   - - -
  e|   |c
   - - -
     d	 
*/
#define		Sega	0x40	
#define		Segb	0x01	
#define		Segc	0x02	
#define		Segd	0x08	
#define		Sege	0x10	
#define		Segf	0x80	
#define		Segg	0x20

#define		Num0	(Sega|Segb|Segc|Segd|Sege|Segf)
#define		Num1	(Segb|Segc)
#define		Num2	(Sega|Segb|Segd|Sege|Segg)
#define		Num3	(Sega|Segb|Segc|Segd|Segg)
#define		Num4	(Segb|Segc|Segf|Segg)
#define		Num5	(Sega|Segc|Segd|Segf|Segg)
#define		Num6	(Sega|Segc|Segd|Sege|Segf|Segg)
#define		Num7	(Sega|Segb|Segc)
#define		Num8	(Sega|Segb|Segc|Segd|Sege|Segf|Segg)
#define		Num9	(Sega|Segb|Segc|Segd|Segf|Segg)
#define		Numb    (Segc|Segd|Sege|Segf|Segg)
#define		Nume	(Sega|Segd|Sege|Segf|Segg)
#define		Numf	(Sega|Sege|Segf|Segg)
#define		Numd	(Segb|Segc|Segd|Sege|Segg)
#define		Numu	(Segb|Segc|Segd|Sege|Segf)
#define		Nump	(Sega|Segb|Sege|Segf|Segg)
#define		Numa 	Segg
#define     Numh    (Segb|Segc|Sege|Segf|Segg)
#define		Numblk	0 

/*-----------------------------*
 *  type definitions           *
 *----------------------------*/

/*-----------------------------*
 *  structure,union and enum   *
 *----------------------------*/

/*-----------------------------*
 *  public functions prototypes*
 *----------------------------*/
extern  void    LEDDispAll(uint8);
extern  void  	LEDDispNum_HI(uint8);
extern  void 	LEDDispNum_LO(uint8);	
extern	void 	LEDDispTime(uint8 *pTime);
extern  void	LEDDispTemp(uint8);
extern  void  	LEDDispCookMode(uint8,uint8);
extern  void  	LEDProc(uint8);
extern  void  	LEDClearFun(void);
extern	void	LEDDispFoodType(uint8);
extern  void  	LEDDispMovie(uint8);
extern  void  	LEDClearNum(void);
extern  void    LEDDispError(void);	
extern  void	LEDDispReady(void);
extern  void    LEDDispPowerSaving(uint8);
//extern  void    LEDDispDIYNum(uint8,uint8);
#ifdef	DEBUG0
extern	void	LEDDispMessage(uint8 msg,uint8 u8CH);
#endif
/*-----------------------------*
 *  public variable declaration*
 *----------------------------*/	
extern	uint8 u8BlankCnt;
extern	uint8 u8Com_Mode;
extern  uint8 xdata m_u8SelfChkStep;
extern  uint8 xdata LEDBuf[];

extern  uint8 bdata	VBuf[];   	//LEDœ‘¥Ê”≥…‰
extern  bit bLED_Dot1;          //":"up
extern  bit bLED_Dot2;          //":"down

extern  bit bLED_Veg;           //Vegetables
extern  bit bLED_Jam;           //Jam
extern  bit bLED_OmPorridge;    //Oatmeal Porridge
extern  bit bLED_Pilaf;         //Pilaf/Risotto
extern  bit bLED_Pasta;         //Pasta/Pelmeni
extern  bit bLED_Meat;          //Meat
extern  bit bLED_Poultry;       //Poultry
extern  bit bLED_Fish;          //Fish

extern  bit bLED_Steam;         //Steam
extern  bit bLED_Reheat;        //Reheat
extern  bit bLED_Dough;         //Dough
extern  bit bLED_Cheese;        //Cheese
extern  bit bLED_Yogurt;        //Yogurt
extern  bit bLED_Soup;          //Soup/Boiling
extern  bit bLED_Bake;          //Bake
extern  bit bLED_Stew;          //Stew/Slowcook

extern  bit bLED_Fry;           //Fry
extern  bit bLED_DIY;           //DIY
extern  bit bLED_Preset;        //Preset
extern  bit bLED_Start;         //Start
extern  bit bLED_Warm;          //Warm
extern  bit bLED_Temp;          //Temperature
extern  bit bLED_Timer;         //Timer
extern  bit bLED_Rice;          //Rice/Cereal

#endif
