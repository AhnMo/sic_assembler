#include "pass2.h"

#include <stdio.h>
#include <string.h>
#include "opcode.h"
#include "statement.h"
#include "util.h"

int is_opcode(char *str) {
	int i;
	int count = optbl_size;

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
int pass2_parse_line(char *str, struct statement_t *sta) {
	const char s[2] = " \t";
	char *token;
	int flag = 1, tmp;

	memset(sta, 0, sizeof(struct statement_t));

	// first part
	token = strtok(str, s);
	if (strcmp(token, ".") == 0) {
		return 4;
	} else {
		// if not both base, end it is loc
		if (strcmp(token, "BASE") && strcmp(token, "END")) {
			//strcpy(, token); // loc
			hex_to_int(token, &tmp);
			sta->loc = tmp;
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
		}
	}

	return 0;
}

void pass2(char *intermediate_filename, char *symbol_filename, char *listing_filename, char *object_filename) {
	FILE *intermediate_fp, *symbol_fp, *listing_fp, *object_fp;
	char buffer[BUFSIZ];
	int t;
	struct statement_t statement;

	intermediate_fp	= fopen(intermediate_filename,	"rb");
	symbol_fp		= fopen(symbol_filename,		"rb");
	listing_fp		= fopen(listing_filename,		"wb");
	object_fp		= fopen(object_filename,		"wb");

	#define READ_NEXT_INTERMEDIATE_LINE {\
		fscanf(intermediate_fp, "%[^\n]%*c", buff);\
		pass2_parse_line(buff, &statement);\
	}

	#define WRITE_LINE_TO_INTERMEDIATE_FILE(s) {\
		fprintf(intermediate_fp, "%04x\t%-10s\t%-10s\t%-10s\n",\
		s.loc, s.symbol, s.opcode, s.operand);\
	}

	#define WRITE_LINE_TO_SYMBOL_TABLE_FILE(s) {\
		fprintf(symbol_fp, "%-10s\t%04x\n",\
		s.symbol, s.loc);\
	}

	READ_NEXT_INTERMEDIATE_LINE; // read first line input

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
			if (*statement.symbol != (int) NULL) {
				if (get_symtab_by_name(statement.symbol) != NULL) {
					fprintf(stderr, "duplicate symbol: %s\n", statement.symbol);
					pass1_unlink_files(intermediate_filename, symbol_filename);
					exit(1);
				} else {
					insert_symtab(&statement);
					WRITE_LINE_TO_SYMBOL_TABLE_FILE(statement);
				}
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
			} else if (is_directive(statement.opcode)) {

			} else {
				fprintf(stderr, "invalid operation code: %s\n", statement.opcode);
				pass1_unlink_files(intermediate_filename, symbol_filename);
				exit(1);
			}
			WRITE_LINE_TO_INTERMEDIATE_FILE(statement);
		}
		READ_NEXT_INPUT_LINE;	// read next input line
	}
	statement.loc = LOCCTR;	// for END
	WRITE_LINE_TO_INTERMEDIATE_FILE(statement);
	printf("%d\n", t);
	puts("WTF2");
	fclose(intermediate_fp);
}
