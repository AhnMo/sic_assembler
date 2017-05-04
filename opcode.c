#include "opcode.h"
#include "hash.h"

struct opcode_t optbl[] = {
	{"ADD",		3, 0x18, 1, "m"},
	{"ADDF",	3, 0x58, 1, "m"},	// XF
	{"ADDR",	2, 0x90, 2, "rr"},	// X
	{"AND",		3, 0x40, 1, "m"},
	{"CLEAR",	2, 0xB4, 1, "r"},	// X
	{"COMP",	3, 0x28, 1, "m"},	//   C
	{"COMPF",	3, 0x88, 1, "m"},	// XFC
	{"COMPR",	2, 0xA0, 2, "rr"},	// X C
	{"DIV",		3, 0x24, 1, "m"},
	{"DIVF",	3, 0x64, 1, "m"},	// XF
	{"DIVR",	2, 0x9C, 2, "rr"},	// X
	{"FIX",		1, 0xC4, 0, NULL},	// XF
	{"FLOAT",	1, 0xC0, 0, NULL},	// XF
	{"HIO",		1, 0xF4, 0, NULL},	//PX
	{"J",		3, 0x3C, 1, "m"},
	{"JEQ",		3, 0x30, 1, "m"},
	{"JGT",		3, 0x34, 1, "m"},
	{"JLT",		3, 0x38, 1, "m"},
	{"JSUB",	3, 0x48, 1, "m"},
	{"LDA",		3, 0x00, 1, "m"},
	{"LDB",		3, 0x68, 1, "m"},	// X
	{"LDCH",	3, 0x50, 1, "m"},
	{"LDF",		3, 0x70, 1, "m"},	// XF
	{"LDL",		3, 0x08, 1, "m"},
	{"LDS",		3, 0x6C, 1, "m"},	// X
	{"LDT",		3, 0x74, 1, "m"},	// X
	{"LDX",		3, 0x04, 1, "m"},
	{"LPS",		3, 0xD0, 1, "m"},	//PX
	{"MUL",		3, 0x20, 1, "m"},
	{"MULF",	3, 0x60, 1, "m"},	// XF
	{"MULR",	2, 0x98, 2, "rr"},	// X
	{"NORM",	1, 0xC8, 0, NULL},	// XF
	{"OR",		3, 0x44, 1, "m"},
	{"RD",		3, 0xD8, 1, "m"},	//P
	{"RMO",		2, 0xAC, 2, "rr"},	// X
	{"RSUB", 	3, 0x4C, 0, NULL},
	{"SHIFTL",	2, 0xA4, 2, "rn"},	// X
	{"SHIFTR",	2, 0xA8, 2, "rn"},	// X
	{"SIO",		1, 0xF0, 0, NULL},	//PX
	{"SKK",		3, 0xEC, 1, "m"},	//PX
	{"STA",		3, 0x0C, 1, "m"},
	{"STB",		3, 0x78, 1, "m"},	// X
	{"STCH",	3, 0x54, 1, "m"},
	{"STF",		3, 0x80, 1, "m"},	// XF
	{"STI",		3, 0xD4, 1, "m"},	//PX
	{"STL",		3, 0x14, 1, "m"},
	{"STS",		3, 0x7C, 1, "m"},	// X
	{"STSW",	3, 0xE8, 1, "m"},	//P
	{"STT",		3, 0x84, 1, "m"},	// X
	{"STX",		3, 0x10, 1, "m"},
	{"SUB",		3, 0x1C, 1, "m"},
	{"SUBF",	3, 0x5C, 1, "m"},	// XF
	{"SUBR",	2, 0x94, 2, "rr"},	// X
	{"SVC",		2, 0xB0, 1, "n"},	// X
	{"TD",		3, 0xE0, 1, "m"},	//P  C
	{"TIO",		3, 0x2C, 1, "m"},	//PX C
	{"TIX",		3, 0x2C, 1, "m"},	//   C
	{"TIXR",	2, 0xB8, 1, "r"},	// X C
	{"WD",		3, 0xDC, 1, "m"},	//P
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

struct register_t regtbl[] = {
	{"A", 0b0000}, {"X", 0b0001}, {"L", 0b0010}, {"B", 0b0011},
	{"S", 0b0100}, {"T", 0b0101}, {"F", 0b0110}, {"PC", 0b1000}, {"SW", 0b1001}
};

int get_register_value(char *r) {
	int i, cnt = sizeof(regtbl) / sizeof(struct register_t);
	for (i = 0; i < cnt; ++i) {
		if (strcmp(regtbl[i].reg, r) == 0) {
			return regtbl[i].value;
		}
	}
	return -1;
}
