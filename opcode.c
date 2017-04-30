#include "opcode.h"
#include "hash.h"

struct opcode_t optbl[] = {
	{"ADD",		3, "18", 1, "m"},
	{"ADDF",	3, "58", 1, "m"},	// XF
	{"ADDR",	2, "90", 2, "rr"},	// X
	{"AND",		3, "40", 1, "m"},
	{"CLEAR",	2, "B4", 1, "r"},	// X
	{"COMP",	3, "28", 1, "m"},	//   C
	{"COMPF",	3, "88", 1, "m"},	// XFC
	{"COMPR",	2, "A0", 2, "rr"},	// X C
	{"DIV",		3, "24", 1, "m"},
	{"DIVF",	3, "64", 1, "m"},	// XF
	{"DIVR",	2, "9C", 2, "rr"},	// X
	{"FIX",		1, "C4", 0, NULL},	// XF
	{"FLOAT",	1, "C0", 0, NULL},	// XF
	{"HIO",		1, "F4", 0, NULL},	//PX
	{"J",		3, "3C", 1, "m"},
	{"JEQ",		3, "30", 1, "m"},
	{"JGT",		3, "34", 1, "m"},
	{"JLT",		3, "38", 1, "m"},
	{"JSUB",	3, "48", 1, "m"},
	{"LDA",		3, "00", 1, "m"},
	{"LDB",		3, "68", 1, "m"},	// X
	{"LDCH",	3, "50", 1, "m"},
	{"LDF",		3, "70", 1, "m"},	// XF
	{"LDL",		3, "08", 1, "m"},
	{"LDS",		3, "6C", 1, "m"},	// X
	{"LDT",		3, "74", 1, "m"},	// X
	{"LDX",		3, "04", 1, "m"},
	{"LPS",		3, "D0", 1, "m"},	//PX
	{"MUL",		3, "20", 1, "m"},
	{"MULF",	3, "60", 1, "m"},	// XF
	{"MULR",	2, "98", 2, "rr"},	// X
	{"NORM",	1, "C8", 0, NULL},	// XF
	{"OR",		3, "44", 1, "m"},
	{"RD",		3, "D8", 1, "m"},	//P
	{"RMO",		2, "AC", 2, "rr"},	// X
	{"RSUB", 	3, "4C", 0, NULL},
	{"SHIFTL",	2, "A4", 2, "rn"},	// X
	{"SHIFTR",	2, "A8", 2, "rn"},	// X
	{"SIO",		1, "F0", 0, NULL},	//PX
	{"SKK",		3, "EC", 1, "m"},	//PX
	{"STA",		3, "0C", 1, "m"},
	{"STB",		3, "78", 1, "m"},	// X
	{"STCH",	3, "54", 1, "m"},
	{"STF",		3, "80", 1, "m"},	// XF
	{"STI",		3, "D4", 1, "m"},	//PX
	{"STL",		3, "14", 1, "m"},
	{"STS",		3, "7C", 1, "m"},	// X
	{"STSW",	3, "E8", 1, "m"},	//P
	{"STT",		3, "84", 1, "m"},	// X
	{"STX",		3, "10", 1, "m"},
	{"SUB",		3, "1C", 1, "m"},
	{"SUBF",	3, "5C", 1, "m"},	// XF
	{"SUBR",	2, "94", 2, "rr"},	// X
	{"SVC",		2, "B0", 1, "n"},	// X
	{"TD",		3, "E0", 1, "m"},	//P  C
	{"TIO",		3, "2C", 1, "m"},	//PX C
	{"TIX",		3, "2C", 1, "m"},	//   C
	{"TIXR",	2, "B8", 1, "r"},	// X C
	{"WD",		3, "DC", 1, "m"},	//P
};

int optbl_size = sizeof(optbl) / sizeof(struct opcode_t);

hashtable_t *ht_optbl = NULL;
void init_optbl() {
	int i, size = optbl_size;
	ht_optbl = ht_create(0x1000);
	for (i = 0; i < size; ++i) {
		ht_set(ht_optbl, optbl[i].op, (void *) (optbl + i));
	}
}
struct opcode_t *get_instruction_info(char *str) {
	if (*str == '+') return get_instruction_info(str + 1);
	return (struct opcode_t *)ht_get(ht_optbl, str);
}

int is_instruction(char *str) {
	return get_instruction_info(str) != NULL;
}

char *directive[] = {"START", "END", "BYTE", "WORD", "RESB", "RESW", "BASE"};
int is_directive(char *str) {
	int i, size = sizeof(directive) / sizeof(char *);

	for (i = 0; i < size; ++i) {
		if (strcmp(str, directive[i]) == 0) {
			return 1;
		}
	}
	return 0;
}
