#ifndef _TIME_H
#define	_TIME_H

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
extern	void	IncTime(uint8	*pTime);
extern	void	IncTime_MIN(uint8	*pTime);
extern	void	DecTime(uint8	*pTime);
//extern	void	IncTime_Hour(uint8	*pTime);

/*-----------------------------*
 *  public variable declaration*
 *----------------------------*/		
extern	uint8	tmCookTime[3];		
extern	uint8   tmBookTime[3];  	

#endif
