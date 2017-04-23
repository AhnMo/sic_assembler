#include "pass2.h"

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
			return optbl[i].n_o;
		}
	}
	return -1;
}

char *nonobj_optbl[6] = {
	"START", "BASE", "RESW", "RESB", "END",
};
int is_nonobj(char *str) {
	int i;
	int count = 5;

	for (i = 0; i < count; ++i) {
		if (strcmp(str, nonobj_optbl[i]) == 0) {
			return 1;
		}
	}
	return 0;
}

// return value
//  1: normal statement line
//  2: base
//  3: end
/// 4: commandline
int parse_line(char *str, struct statement_t *sta) {
	const char s[2] = " \t";
	char *token;
	int flag = 1;

	memset(sta, 0, sizeof(struct statement_t));

	// first part
	token = strtok(str, s);
	if (strcmp(token, ".") == 0) {
		return 4;
	} else {
		// if not both base, end it is loc
		if (strcmp(token, "BASE") && strcmp(token, "END")) {
			strcpy(sta->loc, token); // loc
			token = strtok(NULL, s);
			if ((flag = is_opcode(token)) == -1) {
				strcpy(sta->symbol, token); // symbol
				token = strtok(NULL, s);
			}
		}
		strcpy(sta->opcode, token); // opcode

		if (flag > 0) {
			flag = is_nonobj(token);
			token = strtok(NULL, s);
			strcpy(sta->operand, token); // operand
			if (flag == 0) {
				token = strtok(NULL, s);
				strcpy(sta->objcode, token); // objcode
			}
		}
	}

	return 0;
}

void pass2() {
	FILE *f = fopen("sample.lst", "rb");
	char buffer[BUFSIZ];
	int t;
	struct statement_t state;
	puts("WTF1");
	while ((t = fscanf(f, "%[^\n]%*c", buffer) )!= EOF) {
		parse_line(buffer, &state);
		printf("%s %s %s %s %s\n",
			state.loc, state.symbol, state.opcode,
			state.operand, state.objcode);
	}
	printf("%d\n", t);
	puts("WTF2");
	fclose(f);
}
