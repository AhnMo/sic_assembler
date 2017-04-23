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

void pass1(char *src_filename, char *intermediate_filename);

#endif
