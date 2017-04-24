#include "pass1.h"

#define OPTAB_HASH	1		// remove or comment this line make disable ht

#define WORD_SIZE	3

#ifdef OPTAB_HASH
hashtable_t *ht_optab;
void init_optab() {
	int i, size = sizeof(optab) / sizeof(struct opcode_t);
	ht_optab = ht_create(0x1000);
	for (i = 0; i < size; ++i) {
		ht_set(ht_optab, optab[i].op, (void *) (optab + i));
	}
}
#endif

struct opcode_t *get_instruction_info(char *str) {
	if (*str == '+') return get_instruction_info(str + 1);
#ifdef OPTAB_HASH
	return (struct opcode_t *)ht_get(ht_optab, str);
#else
	int i, size = sizeof(optab) / sizeof(struct opcode_t);
	for (i = 0; i < size; ++i) {
		if (strcmp(str, optab[i].op) == 0) {
			return optab + i;
		}
	}
	return NULL;
#endif
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

int parse_line(char *str, struct statement_t *sta) {
	char s[] = " \t";
	char *token;
	int flag;
	struct opcode_t *op;

	memset(sta, 0, sizeof(struct statement_t));

	token = strtok(str, s);
	if (*token == '.') {
		return 1; // is comment
	}

	if (is_instruction(token) == 0 && is_directive(token) == 0) {
		strcpy(sta->symbol, token);
		token = strtok(NULL, s);
	}

	strcpy(sta->opcode, token);

	if (*token == '+') sta->extended = 1;

	if (is_directive(token) || ((op = get_instruction_info(token)) != NULL && op->n_o > 0)) {
		token = strtok(NULL, s);
		strcpy(sta->operand, token);
	}
	return 0;
}

int get_operator_length(char *str) {
	struct opcode_t *op = get_instruction_info(str);
	int ret = 0;
	switch(op->format) {
		case 1:	ret += 1; break;
		case 2:	ret += 2; break;
		case 3:
			ret += 3;
			if (*str == '+')
				ret += 1;
		break;
	}

	return ret;
}

int get_operand_length(char *str) {
	int len = strlen(str);
	if (str[1] == '\'' && str[len - 1] == '\'') {
		if (str[0] == 'C' || str[0] == 'c') {
			return len - 3;
		} else if (str[0] == 'X' || str[0] == 'x') {
			return (len - 3) / 2;
		} else {
			// error... not C or X
			return -1;
		}
	} else {
		// it is errror, illegal operand length
		return -1;
	}
}

void hex_to_int(char *h, int *i) { sscanf (h, "%x",  i); }
void int_to_hex(int *i, char *h) { sprintf(h, "%x", *i); }
void str_to_int(char *h, int *i) { sscanf (h, "%d",  i); }
void int_to_str(int *i, char *h) { sprintf(h, "%d", *i); }

struct symbol_t {
	char *symbol;
	int locctr;
};

char *symbol_count;
char *symbol_list[0x1000];
hashtable_t *ht_symtab;
void init_symtab() {
	ht_symtab = ht_create(0x1000);
	symbol_count = 0;
}

insert_symtab(char *symbol, int locctr) {

}
struct symbol_t *get_symtab_by_idx(int idx) {

}

void pass1(char *src_filename, char *intermediate_filename, char *symbol_filename) {
	FILE *src_fp, *intermediate_fp;
	char buff[BUFSIZ];
	int start_addr, LOCCTR, old_LOCCTR;

	struct statement_t statement;
	int is_comment;
	struct opcode_t *op;
	int size;

#ifdef OPTAB_HASH
	init_optab();
#endif

	src_fp 			= (FILE *) fopen(src_filename,			"rb");
	intermediate_fp	= (FILE *) fopen(intermediate_filename, "wb");

#define READ_NEXT_INPUT_LINE {\
	fscanf(src_fp, "%[^\n]%*c", buff);\
	is_comment = parse_line(buff, &statement);\
}

#define WRITE_LINE_TO_INTERMEDIATE_FILE(s) {\
	fprintf(intermediate_fp, "%04x\t%s\t%s\t%s\n",\
	s.loc, s.symbol, s.opcode, s.operand);\
}

	READ_NEXT_INPUT_LINE; // read first line input

	if (strcmp(statement.opcode, "START") == 0) {
		hex_to_int(statement.operand, &start_addr);	// save #[OPERAND] as starting address
		LOCCTR = start_addr;	// initialize LOCCTR to starting address
		statement.loc = LOCCTR;
		WRITE_LINE_TO_INTERMEDIATE_FILE(statement);
		READ_NEXT_INPUT_LINE;	// read next input line
	} else {
		LOCCTR = 0;
	}

	while (strcmp(statement.opcode, "END") != 0) {
		if (is_comment) {
			//printf("%s/%s/%s\n", statement.symbol, statement.opcode, statement.operand);
		} else {
			statement.loc = LOCCTR;
			if (statement.symbol != NULL) {

			}

			if (is_instruction(statement.opcode)) {	// found
				LOCCTR += get_operator_length(statement.opcode);
			} else if (strcmp(statement.opcode, "WORD") == 0) {
				LOCCTR += WORD_SIZE;
			} else if (strcmp(statement.opcode, "RESW") == 0) {
				str_to_int(statement.operand, &size);
				LOCCTR += WORD_SIZE * size;
			} else if (strcmp(statement.opcode, "RESB") == 0) {
				str_to_int(statement.operand, &size);
				LOCCTR += size;
			} else if (strcmp(statement.opcode, "BYTE") == 0) {
				LOCCTR += get_operand_length(statement.operand);
			} else {
				fprintf(stderr, "invalid operation code: %s\n", statement.opcode);
				exit(1);
			}
			WRITE_LINE_TO_INTERMEDIATE_FILE(statement);
		}
		READ_NEXT_INPUT_LINE;	// read next input line
	}
	statement.loc = LOCCTR;	// for END
	WRITE_LINE_TO_INTERMEDIATE_FILE(statement);

	fprintf(intermediate_fp, "%d\n", LOCCTR - start_addr); // save (LOCCTR - starting address) as program length

	fclose(src_fp);
	fclose(intermediate_fp);
}
