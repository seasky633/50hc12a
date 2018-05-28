#ifndef _DATATYPE_H
#define	_DATATYPE_H

typedef	bit				Bool;
typedef unsigned char 	uint8;
typedef unsigned int	uint16;

typedef struct { 
		char b0:1; 
		char b1:1; 
		char b2:1; 
		char b3:1;
		char b4:1; 
		char b5:1; 
		char b6:1; 
		char b7:1;
}Bit;

typedef	union	
{
	uint8		val;
	Bit	   	    B;
}Byte;

#define	SetBit(reg,b)	reg	|=	(1<<b)
#define	ClrBit(reg,b)	reg	&=	~(1<<b)
#define	TstBit(reg,b)   (reg & (1<<b))

#define	MAKEWORD(v1,v2)	(((uint16)(v1)<<8) + (uint16)(v2))
#define	HIBYTE(v1)		((uint8)((v1)>>8))
#define	LOBYTE(v1)		((uint8)((v1)&0xff))

#define LongToBin(n) \
(                    \
((n >> 21) & 0x80) | \
((n >> 18) & 0x40) | \
((n >> 15) & 0x20) | \
((n >> 12) & 0x10) | \
((n >>  9) & 0x08) | \
((n >>  6) & 0x04) | \
((n >>  3) & 0x02) | \
((n      ) & 0x01)   \
)

#define Bin(n) LongToBin(0x##n##l)	//write binary charactor set,exsample : Bin(11111111) = 0xff

#define	CLOCK_8MS   (65536-5333)	//16 bit counter.T=12/8us  t=8000us  N=t/T=5333

#endif
