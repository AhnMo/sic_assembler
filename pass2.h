#ifndef __PASS2__

#define __PASS2__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>

struct opcode_t {
	char *op;
	int format;
	char *code;
	int n_o; // the number of operand
	char *n_f; // the format of operand
};

int is_opcode(char *target);


void pass2();

#endif
