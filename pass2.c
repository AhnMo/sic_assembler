#include "pass2.h"

struct opcode_t optbl[] = {
	{"ADD",		3, "18", 1, "m"},
	{"ADDR",	2, "90", 2, "rr"},
	{"BASE",	9, NULL, 1, "m"},
	{"BYTE", 	0, NULL, 1, "m"},
	{"CLEAR",	2, "B4", 1, "r"},
	{"COMP",	3, "28", 1, "m"},
	{"COMPR",	2, "A0", 2, "rr"},
	{"DIV",		3, "24", 1, "m"},
	{"DIVR",	2, "9C", 2, "rr"},
	{"END",		9, NULL, 1, "m"},
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
	{"RESB", 	9, NULL, 1, "n"},
	{"RESW",	9, NULL, 1, "n"},
	{"RMO",		2, "AC", 2, "rr"},
	{"RSUB", 	3, "4C", 0, NULL},
	{"SHIFTL",	2, "A4", 2, "rn"},
	{"SHIFTR",	2, "A8", 2, "rn"},
	{"STA",		3, "0C", 1, "m"},
	{"START",	9, NULL, 1, "n"},
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
	{"WD",		3, "DC", 1, "m"},
	{"WORD", 	0, NULL, 1, "n"},
};

int regex_match(char *pat, char *sub) {
	int flag;
	regex_t regex;

	if ((flag = regcomp(&regex, pat, REG_EXTENDED))) {
		fprintf(stderr, "Could not compile regex\n");
		exit(1);
	}

	return regexec(&regex, sub, 0, NULL, 0) != REG_NOMATCH;

}

struct statement_t {
	char loc[8];
	char symbol[8];
	char opcode[8];
	char operand[16];
	char objcode[16];
};

int is_opcode(char *str) {
	int i;
	int count = sizeof(optbl) / sizeof(struct opcode_t);

	if (*str == '+')
		return is_opcode(str + 1);

	for (i = 0; i < count; ++i) {
		if (strcmp(str, optbl[i].op) == 0) {
			return ((optbl[i].n_o & 0xFF) | ((optbl[i].format & 0xFF) << 8));
		}
	}
	return -1;
}

// return value
//  1: normal statement line
/// 0: comment
int parse_line(char *str, struct statement_t *sta) {
	const char s[2] = " \t";
	char *token;
	int flag = 1;

	memset(sta, 0, sizeof(struct statement_t));

	token = strtok(str, s);
	// check loc
	/*
	if (regex_match("^([[:xdigit:]]{4})$", token) > 0) {
		strcpy(sta->loc, token);
		token = strtok(NULL, s);
	} else */if (*token == '.') {
		return 0;
	}
	if (((is_opcode(token) >> 8) & 0xFF) == -1) {
		strcpy(sta->symbol, token);
		token = strtok(NULL, s);
	}
	strcpy(sta->opcode, token);
	flag = is_opcode(token);

	if ((flag & 0xFF) > 0) {
		token = strtok(NULL, s);
		strcpy(sta->operand, token);
	}

	if (((flag >> 8) & 0xFF) != 9) {
		token = strtok(NULL, s);
		strcpy(sta->objcode, token);
	}

	return 1;
}

void pass2() {
	FILE *immediate = fopen("sample.asm", "rb");
	char buffer[BUFSIZ];
	int t;
	struct statement_t state;

	while ((t = fscanf(f, "%[^\n]%*c", buffer))!= EOF) {
		parse_line(buffer, &state);
		printf("%s/%s/%s/%s/%s\n",
			state.loc, state.symbol, state.opcode,
			state.operand, state.objcode);

		if (strcmp(state.opcode, "START") == 0) {

		}
	}
	fclose(f);
}
