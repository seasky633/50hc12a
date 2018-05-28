#ifndef _EEPROM_H
#define _EEPROM_H

/*-----------------------------*
 *  Macros for constants       *
 *----------------------------*/
#define EEP_STEP_STANBY 0
#define EEP_STEP_ERASE 1
#define EEP_STEP_WRITE 2

#define EEP_ADDRESS_START 0x3FA0	//EEPROM 起始地址
#define EEP_ADDRESS_END 0x3FFF		//EEPROM 结束地址
#define	EEP_SECTOR_ADDR	0x3FC0	

#define EPROM_DEFAULT_ADR EEP_ADDRESS_START	// 此处指定默认不使用Eprom擦除/写时设置的地址指向用户指定的EPROM地址

#define EEPROM_W_MAX 5 //允许写5次，不成功退出
#define	EEPERASE_CMD	0x02
#define	EEPWRITE_CMD	0x03

#define EEPROMPAGESIZE 32

#define EEP_DIY1_S0 0
#define EEP_DIY1_TM1 1
#define EEP_DIY1_TM2 2
#define EEP_DIY1_TMP 3
#define EEP_DIY1_COOKMODE 4
#define EEP_DIY1_FOODTYPE 5

#define EEP_DIY2_S0  6
#define EEP_DIY2_TM1 7
#define EEP_DIY2_TM2 8
#define EEP_DIY2_TMP 9
#define EEP_DIY2_COOKMODE 10
#define EEP_DIY2_FOODTYPE 11

#define EEP_DIY3_S0  12
#define EEP_DIY3_TM1 13
#define EEP_DIY3_TM2 14
#define EEP_DIY3_TMP 15
#define EEP_DIY3_COOKMODE 16
#define EEP_DIY3_FOODTYPE 17

#define EEP_MUSIC_EN 18

#define EEP_POWER_S0 19
#define EEP_POWER_VAL 20

/*-----------------------------*
 *  public functions prototypes*
 *----------------------------*/
extern	uint8 	EEPROM_Process(void);
extern  void 	EpromInit(void);
extern  uint8 xdata rEepromCache[32];


#endif