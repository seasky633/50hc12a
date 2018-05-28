#ifndef _SCRIPT_H
#define	_SCRIPT_H

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
extern	void 	ScriptProc(void);
extern	void	BoilCheckProc(void);
#ifdef	DEBUG0
extern  uint8	GetScriptStage(void);
#endif
extern	void	ResetCurScript(void);
extern	void	SetCurScript(uint8 u8ID,uint8 u8Diy,uint8 u8Temp);
extern  uint16	m_u16MashTime;

/*-----------------------------*
 *  public variable declaration*
 *----------------------------*/


#endif