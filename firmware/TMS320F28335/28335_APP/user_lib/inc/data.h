#ifndef __DATA_H__
#define __DATA_H__
#define NULL 0
#define     __IO    volatile             /*!< Defines 'read / write' permissions              */
typedef unsigned short int u16;
typedef unsigned long int u32;
typedef unsigned char u8;
//-------------------------
typedef struct
{
	unsigned char bit0 :1;
	unsigned char bit1 :1;
	unsigned char bit2 :1;
	unsigned char bit3 :1;
	unsigned char bit4 :1;
	unsigned char bit5 :1;
	unsigned char bit6 :1;
	unsigned char bit7 :1;
} Bits_8s;
typedef struct
{
	unsigned char byte0 :4;
	unsigned char byte1 :4;
} Byte_8s;
typedef union
{
	unsigned char all;
	Bits_8s bit;
	Byte_8s byte;
} Char_8s;
//------------------------
typedef struct
{
	unsigned char bit0 :1;
	unsigned char bit1 :1;
	unsigned char bit2 :1;
	unsigned char bit3 :1;
	unsigned char bit4 :1;
	unsigned char bit5 :1;
	unsigned char bit6 :1;
	unsigned char bit7 :1;
	unsigned char bit8 :1;
	unsigned char bit9 :1;
	unsigned char bit10 :1;
	unsigned char bit11 :1;
	unsigned char bit12 :1;
	unsigned char bit13 :1;
	unsigned char bit14 :1;
	unsigned char bit15 :1;
} Bits_16s;
typedef struct
{
	unsigned char byte0 :4;
	unsigned char byte1 :4;
	unsigned char byte3 :4;
	unsigned char byte4 :4;
} Byte_16s;
typedef union
{
	unsigned short int all;
	Bits_16s bit;
	Byte_16s byte;
} Uint_16s;
//#define u8 unsigned char
//#define u16 unsigned short int
//#define u32 unsigned long int
#endif
