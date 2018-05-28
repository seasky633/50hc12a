#ifndef _HD_CFG_H_
#define _HD_CFG_H_

/*-----------------------------*
 *  include head files         *
 *----------------------------*/ 
#include ".\Head\MC96F8316S.h"
#include ".\Head\Datatype.h"
#include ".\Head\CommDef.h"
#include ".\Head\AD.h"
#include ".\Head\LED.h"
#include ".\Head\Key.h"
#include ".\Head\Buzzer.h"
#include ".\Head\EEPflash.h"
#include ".\Head\IH_Com.h"

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
extern	void	System_INIT(void);
extern 	void 	WDT_clear();

/*-----------------------------*
 *  public variable declaration*
 *----------------------------*/

#endif
