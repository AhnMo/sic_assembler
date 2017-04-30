#include "pass1.h"

#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "opcode.h"
#include "hash.h"
#include "util.h"
#include "statement.h"
#include "symbol.h"

#define WORD_SIZE	3
int pass1_parse_line(char *str, struct statement_t *sta) {
	char s[] = " \t";
	char *token;
	int flag, t;
	struct opcode_t *op;

	memset(sta, 0, sizeof(struct statement_t));

	token = strtok(str, s);
	if (*token == '.') {
		return 1;
	}

	if (is_instruction(token) == 0 && is_directive(token) == 0) {
		strcpy(sta->symbol, token);
		token = strtok(NULL, s);
	}

	strcpy(sta->opcode, token);

	if (*token == '+') sta->flag &= 1;

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




int pass1_unlink_files(char *intermediate_filename, char *symbol_filename) {
	return unlink(intermediate_filename) | unlink(symbol_filename);
}

void pass1(char *src_filename, char *intermediate_filename, char *symbol_filename) {
	FILE *src_fp, *intermediate_fp, *symbol_fp;
	char buff[BUFSIZ];
	int start_addr, LOCCTR, old_LOCCTR;

	struct statement_t statement;
	int is_comment;
	struct opcode_t *op;
	int size, t;

	init_symtab();

	src_fp 			= (FILE *) fopen(src_filename,			"rb");
	intermediate_fp	= (FILE *) fopen(intermediate_filename, "wb");
	symbol_fp		= (FILE *) fopen(symbol_filename,		"wb");

#define READ_NEXT_INPUT_LINE {\
	fscanf(src_fp, "%[^\n]%*c", buff);\
	is_comment = pass1_parse_line(buff, &statement);\
}

#define WRITE_LINE_TO_INTERMEDIATE_FILE(s) {\
	fprintf(intermediate_fp, "%04x\t%-10s\t%-10s\t%-10s\t%04x\t%04x\n",\
	s.loc, s.symbol, s.opcode, s.operand, s.flag, s.size);\
}

#define WRITE_LINE_TO_SYMBOL_TABLE_FILE(s) {\
	fprintf(symbol_fp, "%-10s\t%04x\n",\
	s.symbol, s.loc);\
}

	READ_NEXT_INPUT_LINE; // read first line input

	if (strcmp(statement.opcode, "START") == 0) {
		hex_to_int(statement.operand, &start_addr);	// save #[OPERAND] as starting address
		LOCCTR = start_addr;	// initialize LOCCTR to starting address
		statement.loc = LOCCTR;

		int_to_hex(start_addr, statement.operand);

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
					insert_symtab(statement.symbol, statement.loc);
					WRITE_LINE_TO_SYMBOL_TABLE_FILE(statement);
				}
			} else {
				strcpy(statement.symbol, "-");
			}

			if (is_instruction(statement.opcode)) {	// found
				size = get_operator_length(statement.opcode);
			} else if (strcmp(statement.opcode, "WORD") == 0) {
				size = WORD_SIZE;
			} else if (strcmp(statement.opcode, "RESW") == 0) {
				str_to_int(statement.operand, &t);
				size = WORD_SIZE * t;
			} else if (strcmp(statement.opcode, "RESB") == 0) {
				str_to_int(statement.operand, &t);
				size = t;
			} else if (strcmp(statement.opcode, "BYTE") == 0) {
				size = get_operand_length(statement.operand);
			} else {
				fprintf(stderr, "invalid operation code: %s\n", statement.opcode);
				pass1_unlink_files(intermediate_filename, symbol_filename);
				exit(1);
			}
			LOCCTR += size;
			statement.size = size;
			if (strcmp(statement.operand, "") == 0) {
				strcpy(statement.operand, "-");
			}
			WRITE_LINE_TO_INTERMEDIATE_FILE(statement);
		}
		READ_NEXT_INPUT_LINE;	// read next input line
	}
	statement.loc = LOCCTR;	// for END
	strcpy(statement.symbol, "-");
	WRITE_LINE_TO_INTERMEDIATE_FILE(statement);

	fprintf(intermediate_fp, "%d\n", LOCCTR - start_addr); // save (LOCCTR - starting address) as program length

	fclose(src_fp);
	fclose(intermediate_fp);
	fclose(symbol_fp);

	clear_symtab();
}
