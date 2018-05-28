#ifndef _BUZZER_H
#define	_BUZZER_H

/*-----------------------------*
 *  include head files         *
 *----------------------------*/

/*-----------------------------*
 *  Macros for constants       *
 *----------------------------*/
//打开时间
#define		M_0			0
#define		M_100		1
#define 	M_300       2
#define 	M_800       3

//关闭时间
#define 	S_0         (0<<4)
#define		S_100		(1<<4)
#define		S_300		(2<<4)
#define		S_800		(3<<4)

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
extern	void	SoundProc(void);
extern	void	PlaySound(uint8	code *pMusic);

/*-----------------------------*
 *  public variable declaration*
 *----------------------------*/
extern  uint8   code	MusicPoweron[];
extern  uint8   code	MusicKeyDown[];
extern  uint8   code	MusicSysError[];
extern  uint8   code    MusicSysErrorNoPan[];
extern  uint8   code	MusicCookEnd[];
extern	uint8   code    MusicAchievedTemp[];
extern	uint8   code	MusicOFF[];
extern  uint8   code    MusicInputFood[];
extern  uint8   code    MusicCancelFunc[];
extern  uint8   xdata   g_u8BuzMode;

#endif
