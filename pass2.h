#ifndef __PASS2__

#define __PASS2__

#include <stdio.h>
#include <string.h>

struct opcode_t {
	char *op;
	int format;
	char *code;
	int n_o; // the number of operand
	char *n_f; // the format of operand
};

struct opcode_t optbl[] = {
	{"ADD",		3, "18", 1, "m"},
	{"ADDR",	2, "90", 2, "rr"},
	{"CLEAR",	2, "B4", 1, "r"},
	{"COMP",	3, "28", 1, "m"},
	{"COMPR",	2, "A0", 2, "rr"},
	{"DIV",		3, "24", 1, "m"},
	{"DIVR",	2, "9C", 2, "rr"},
	{"J",		3, "3C", 1, "m"},
	{"JEQ",		3, "30", 1, "m"},
	{"JGT",		3, "34", 1, "m"},
	{"JLT",		3, "38", 1, "m"},
	{"JSUB",	3, "48", 1, "m"},
	{"LDA",		3, "00", 1, "m"},
	{"LDB",		3, "68", 1, "m"},
	{"LDCH",	3, "50", 1, "m"},
	{"LDL",		3, "08", 1, "m"},
	{"LDS",		3, "6C", 1, "m"},
	{"LDT",		3, "74", 1, "m"},
	{"LDX",		3, "04", 1, "m"},
	{"MUL",		3, "20", 1, "m"},
	{"MULR",	2, "98", 2, "rr"},
	{"RD",		3, "D8", 1, "m"},
	{"RMO",		2, "AC", 2, "rr"},
	{"RSUB", 	3, "4C", 0, NULL},
	{"SHIFTL",	2, "A4", 2, "rn"},
	{"SHIFTR",	2, "A8", 2, "rn"},
	{"STA",		3, "0C", 1, "m"},
	{"STB",		3, "78", 1, "m"},
	{"STCH",	3, "54", 1, "m"},
	{"STL",		3, "14", 1, "m"},
	{"STS",		3, "7C", 1, "m"},
	{"STT",		3, "84", 1, "m"},
	{"STX",		3, "10", 1, "m"},
	{"SUB",		3, "1C", 1, "m"},
	{"SUBR",	2, "94", 2, "rr"},
	{"TD",		3, "E0", 1, "m"},
	{"TIX",		3, "2C", 1, "m"},
	{"TIXR",	2, "B8", 1, "r"},
	{"WD",		3, "DC", 1, "m"}
};

int is_opcode(char *target);


void pass2();

#endif
