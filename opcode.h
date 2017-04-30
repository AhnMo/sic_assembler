#ifndef __OPCODE_H__
#define __OPCODE_H__

#include <stdio.h>
#include "hash.h"

struct opcode_t {
	char *op;
	int format;
	char *code;
	int n_o; // the number of operand
	char *n_f; // the format of operand
};

int is_directive(char *str);

extern struct opcode_t optbl[];
extern int optbl_size;

extern hashtable_t *ht_optbl;
void init_optbl();
struct opcode_t *get_instruction_info(char *str);
int is_instruction(char *str);

#endif
