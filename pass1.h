#ifndef __PASS1_H__

#define __PASS1_H__

#include <stdio.h>
#include <string.h>

#include "hash.h"

struct opcode_t {
	char *op;
	int format;
	char *code;
	int n_o; // the number of operand
	char *n_f; // the format of operand
};

struct statement_t {
	int loc;
	char symbol[8];
	char opcode[8];
	char operand[16];
	char objcode[16];
};

void pass1(char *src_filename, char *intermediate_filename, char *symbol_filename);

#endif
