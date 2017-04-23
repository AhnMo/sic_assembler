#include <stdio.h>
#include <string.h>

/*
struct opcode_t {
	char *op;
	int format;
	char *code;
	int n_o; // the number of operand
	char *n_f; // the format of operand
};
*/
struct opcode_t {
	char *op;
	int n_o; // the number of operand
	int format;
	char *code;
	char *n_f; // the format of operand
};

struct opcode_t optbl[] = {
	{"add",		1, 3, "18", "m"},
	{"addr",	2, 2, "90", "rr"},
	{"clear",	1, 2, "b4", "r"},
	{"comp",	1, 3, "28", "m"},
	{"compr",	2, 2, "a0", "rr"},
	{"div",		1, 3, "24", "m"},
	{"divr",	2, 2, "9c", "rr"},
	{"j",		1, 3, "3c", "m"},
	{"jeq",		1, 3, "30", "m"},
	{"jgt",		1, 3, "34", "m"},
	{"jlt",		1, 3, "38", "m"},
	{"jsub",	1, 3, "48", "m"},
	{"lda",		1, 3, "00", "m"},
	{"ldb",		1, 3, "68", "m"},
	{"ldch",	1, 3, "50", "m"},
	{"ldl",		1, 3, "08", "m"},
	{"lds",		1, 3, "6c", "m"},
	{"ldt",		1, 3, "74", "m"},
	{"ldx",		1, 3, "04", "m"},
	{"mul",		1, 3, "20", "m"},
	{"mulr",	2, 2, "98", "rr"},
	{"rd",		1, 3, "d8", "m"},
	{"rmo",		2, 2, "ac", "rr"},
	{"rsub", 	0, 3, "4c", NULL},
	{"shiftl",	2, 2, "a4", "rn"},
	{"shiftr",	2, 2, "a8", "rn"},
	{"sta",		1, 3, "0c", "m"},
	{"stb",		1, 3, "78", "m"},
	{"stch",	1, 3, "54", "m"},
	{"stl",		1, 3, "14", "m"},
	{"sts",		1, 3, "7c", "m"},
	{"stt",		1, 3, "84", "m"},
	{"stx",		1, 3, "10", "m"},
	{"sub",		1, 3, "1c", "m"},
	{"subr",	2, 2, "94", "rr"},
	{"td",		1, 3, "e0", "m"},
	{"tix",		1, 3, "2c", "m"},
	{"tixr",	1, 2, "b8", "r"},
	{"wd",		1, 3, "dc", "m"}
};

int is_opcode(char *target) {
	int i;
	int count_optbl = sizeof(optbl) / sizeof(struct opcode_t); // 39

	for (i = 0; i < count_optbl; ++i) {
		if (strcmp(target, optbl[i].op) == 0) {
			return 1;
		}
	}
	return 0;
}

int parse_line(char *str) {

	;
}

void pass2() {
	parse_line("0000  COPY    START   0");
	parse_line("0000  FIRST   ST      RETADR,LX         17202D");
}
