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
10000000 - # immediate
01000000 - @ indirect
00100000 - float			operand type
00010000 - numeric			operand type


*/

#endif
