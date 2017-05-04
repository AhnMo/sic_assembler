#ifndef __STATEMENT_H__
#define __STATEMENT_H__

struct statement_t {
	int loc;
	char symbol[8];
	char opcode[8];
	char operand[16];
	char objcode[16];
	int flag;
	int size;
};

/*
about flag
10000000 - + extended
01000000 - # immediate
00100000 - @ indirect
00001000 - float			operand type
00000100 - numeric			operand type
*/

/*
about real format
00000010 - n	indirect addressing
00000001 - i	immediate addressing

1000xxxx - x	index addressing
0100xxxx - b	base relative
0010xxxx - p	pc relatvie
0001xxxx - e	extended format

xxxxxxxx


// STL RETADR
   1    7    2    0    2    D
0001 0111 0010 0000 0010 1101
       11 0010
 INDIRECT
 IMMEDIATE
 PC RELATIVE

// LDB LENGTH
   6    9    2    0    2    D
0110 1001 0010 0000 0010 1101
       01 0010
 IMMEDIATE
 PC RELATIVE

// BASE LENGTH

// +JSUB RDREC
   4    B    1    0    1    0    3    6
0100 1011 0001 0000 0001 0000 0011 0110
       11 0001
SIMPLE
EXTENDED

// LDA LENGTH
   0    3    2    0    2    6
0000 0011 0010 0000 0010 0110
       11 0010
XE SIMPLE
BASE RELATVIE

COMP #0
   2    9    0    0    0    0
0010 1001 0000 0000 0000 0000
       01 0000
IMMEDIATE

JEQ EN
   3    3    2    0    0    7
0011 0011 0010 0000 0000 0111
       11 0010
INDIRECT
IMMEDIATE
BASE RELATVIE

*/

#define SET_INDIRECT(x)		(x | 0b100000)
#define SET_IMMEDIATE(x)	(x | 0b010000)
#define SET_SIMPLE_XE(x)	(x | 0b110000)
#define SET_INDEX(x)		(x | 0b001000)
#define SET_BASERELATVIE(x)	(x | 0b000100)
#define SET_PCRELATVIE(x)	(x | 0b000010)
#define SET_EXTENDED(x)		(x | 0b000001)
//assemble
#define ASSEMBLE(x,y)		(x << 12 | y)		// 3byte	int okey
#define ASSEMBLE_EXT(x,y)	(x << 20 | y)		// 4byte	int okey
//#define ASSEMBLE_FLOAT(x,y)	(x << 20 | y)		// 6byte	long long int?

#endif
