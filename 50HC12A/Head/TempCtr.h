#ifndef _TEMPCTR_H
#define	_TEMPCTR_H

/*-----------------------------*
 *  include head files         *
 *----------------------------*/

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
extern	void	KeepTempProc(void);
extern	void	HeatProc(void);

/*-----------------------------*
 *  public variable declaration*
 *----------------------------*/
extern	uint8	g_u8HiTemp;
extern	uint8	g_u8LoTemp;
extern	uint8	g_u8OnTime_Set;
extern	uint8	g_u8OffTime_Set;
extern	uint16	g_u16RelayTime;	

#endif
