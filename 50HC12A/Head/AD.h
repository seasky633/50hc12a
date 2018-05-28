#ifndef _AD_H_
#define	_AD_H_

/*-----------------------------*
 *  include head files         *
 *----------------------------*/

/*-----------------------------*
 *  Macros for constants       *
 *----------------------------*/
//PIN25   AN2/P02   AD-RT-BOT	底部温度
//PIN24   AN3/P03   AD-RT-TOP   顶部温度

#define	 AD_CH_BOT	0
#define  AD_CH_TOP	1

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
extern	uint8	ADCGetData(uint8 u8CH);
extern	uint8	ADCGetVal(uint8 u8CH);
//extern	void	Sort8(uint8	*pDat,uint8	u8Size);
extern	void	ADC_INIT(void);
extern	void	ADCProc(void);

/*-----------------------------*
 *  public variable declaration*
 *----------------------------*/
extern	uint8   code    TempTable3950_50K[];

#endif


