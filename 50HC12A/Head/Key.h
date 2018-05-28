#ifndef _KEY_H_
#define	_KEY_H_

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
extern  void	KeyProc(void);
extern	uint8  	GetKey(void);
extern	uint8  	IsNewKey(void);

/*-----------------------------*
 *  public variable declaration*
 *----------------------------*/
extern	uint16	g_u16KeyRpyCnt;
extern  uint8	m_u8KeyVal;
extern  uint8   g_u8KeyUpAct;
extern  Bool    bNewKey_Menu;
 
#endif

