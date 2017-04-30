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


#endif
