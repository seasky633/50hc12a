#ifndef _IH_COM_H_
#define	_IH_COM_H_

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
extern  void    CHK_IH_Connector(void);
extern  void    CHK_IH_Com_Set(void);   

/*-----------------------------*
 *  public variable declaration*
 *----------------------------*/
extern	uint8 idata ReadDataBuf[12];
extern  uint8 idata WriteRunDataBuf[4];
extern  uint8 idata power_value;

#define IHStatus 		ReadDataBuf[0]
#define VoltageValue 	ReadDataBuf[1]
#define CurrentValue 	ReadDataBuf[2]
#define IGBTValue 		ReadDataBuf[3]
#define AD_BottomTemperature  	ReadDataBuf[4]
#define AD_TopTemperature 	ReadDataBuf[5]
#define ActualPower 	ReadDataBuf[6]
#define TargetPower 	ReadDataBuf[7]
#define ActualPPG   	ReadDataBuf[8]
#define PowerStatus 	ReadDataBuf[9]
#define RequirePower 	ReadDataBuf[10]


#define ControlSet 		WriteRunDataBuf[0]
#define PowerSetL 		WriteRunDataBuf[1]
#define POwerSetH 		WriteRunDataBuf[2]
#define POwerIO 		WriteRunDataBuf[3]

#endif
