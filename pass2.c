#include "pass2.h"

#include <stdio.h>
#include <string.h>
#include "opcode.h"
#include "statement.h"
#include "util.h"
#include "symbol.h"

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

int pass2_parse_line(char *str, struct statement_t *sta) {
	const char s[2] = " \t";
	char *token;
	int flag = 1, tmp;

	memset(sta, 0, sizeof(struct statement_t));

	token = strtok(str, s);
	hex_to_int(token, &tmp);
	sta->loc = tmp;

	token = strtok(NULL, s);
	strcpy(sta->symbol, token);

	token = strtok(NULL, s);
	strcpy(sta->opcode, token);

	if (strcmp(sta->opcode, "RSUB") == 0)
		return 0;
	token = strtok(NULL, s);
	strcpy(sta->operand, token);

	return 0;
}

char *convert_constant(char *a) {
	char buf[BUFSIZ], buf2[8];
	int i, t;
	memset(buf, 0, BUFSIZ);
	if (*a == 'X' || *a == 'x') {
		strncpy(buf, a + 2, 2);
	} else if(*a == 'C' || *a == 'c') {
		strcpy(buf, "");
		for (i = 2; i < strlen(a) - 1; ++i) {
			int_to_hex(((int)*(a + i)), buf2);
			strcat(buf, buf2);
		}
	} else {
		// illegal
	}
	return strdup(buf);
}

int pass2_unlink_files(char *listing_filename, char *object_filename) {
	return unlink(listing_filename) | unlink(object_filename);
}


int obj_idx;
char *obj_code[300];

void pass2(char *intermediate_filename, char *symbol_filename, char *listing_filename, char *object_filename) {
	FILE *intermediate_fp, *symbol_fp, *listing_fp, *object_fp;
	char buff[BUFSIZ], record[BUFSIZ], record2[BUFSIZ];
	int t, tmp, program_length, i, cur, this_len, operand_addr;
	struct statement_t statement;
	struct opcode_t *op;
	struct symbol_t *sym;
	char *ptr, symbol[32];

	obj_idx = 0;

	intermediate_fp	= fopen(intermediate_filename,	"rb");
	symbol_fp		= fopen(symbol_filename,		"rb");
	listing_fp		= fopen(listing_filename,		"wb");

	init_symtab();

	while (fscanf(symbol_fp, "%[^\n]%*c", buff) != EOF) {
		ptr = strtok(buff, " \t");
		strcpy(symbol, ptr);
		ptr = strtok(NULL, " \t");
		hex_to_int(ptr, &tmp);
		//printf("%s \t %d\t%s\t", symbol, tmp,ptr);

		insert_symtab(symbol, tmp);

		//printf("%p\n", get_symtab_by_name(symbol));
	}

#define NEW_OBJ_LINE	(obj_code[obj_idx++]=(char *) malloc(sizeof(char) * BUFSIZ))
#define CURRENT_OBJ_LINE (obj_code[obj_idx - 1])
#define READ_NEXT_INTERMEDIATE_LINE {\
	fscanf(intermediate_fp, "%[^\n]%*c", buff);\
	pass2_parse_line(buff, &statement);\
}

#define WRITE_LINE_TO_LISTING_FILE(s) {\
	fprintf(intermediate_fp, "%04x\t%-10s\t%-10s\t%-10s\t%-10s\n",\
	s.loc, s.symbol, s.opcode, s.operand, s.objcode);\
}

	READ_NEXT_INTERMEDIATE_LINE; // read first line input

	if (strcmp(statement.opcode, "START") == 0) {
		WRITE_LINE_TO_LISTING_FILE(statement);
		NEW_OBJ_LINE;
		sprintf(obj_code[0], "H^%-6s^%06X^", statement.symbol, statement.loc);
		READ_NEXT_INTERMEDIATE_LINE;
	}
	NEW_OBJ_LINE;
	cur = 0;
	this_len = 0;
	while (strcmp(statement.opcode, "END") != 0) {
		/*printf("%04x\t%-10s\t%-10s\t%-10s\n",
			statement.loc,
			statement.symbol,
			statement.opcode,
			statement.operand
		);*/


		op = get_instruction_info(statement.opcode);
		if (op) {
			if (op->n_o > 0) {
				ptr = strtok(statement.operand, ",");
				sym = get_symtab_by_name(ptr);
				//sym = get_symtab_by_name("THREE");
				//printf("/%s/%p\n", ptr,sym);
				if (sym == NULL) {
					fprintf(stderr, "undefined symbol: %s\n", statement.symbol);
					pass2_unlink_files(listing_filename, object_filename);
					exit(1);
				}
				operand_addr = sym->locctr;
				//printf("//////%d\n", sym->locctr);
			} else {
				operand_addr = 0;
			}

			sprintf(buff, "%02X%04X", op->code, operand_addr);
		} else if (strcmp(statement.opcode, "BYTE") == 0 || strcmp(statement.opcode, "WORD") == 0) {
			// CONVERT CONSTANT TO OBJECT CODE
			//printf("%s\n", );
			sprintf(buff, "%s", convert_constant(statement.operand));
		}
		printf("%s\n", buff);
		t = this_len + strlen(buff);

		if (strlen(buff) != 0) {
			if (t > 0x1D) {
				sprintf(CURRENT_OBJ_LINE, "T^%06X^%02X%s", cur, this_len, record);
				cur += this_len;
				this_len = 0;
				memset(record, 0, BUFSIZ);
				//printf ();
				NEW_OBJ_LINE;
			}
			strcpy(record2, record);
			sprintf(record, "%s^%s", record2, buff);
			//printf(record);
			this_len += strlen(buff) / 2;
			printf("%d ", this_len);
		}
		READ_NEXT_INTERMEDIATE_LINE;
	}

	printf("%04x\t%-10s\t%-10s\t%-10s\n",
		statement.loc,
		statement.symbol,
		statement.opcode,
		statement.operand
	);

	fscanf(intermediate_fp, "%[^\n]%*c", buff);
	str_to_int(buff, &tmp);
	program_length = tmp;

	if (*obj_code[0] == 'H') {
		sprintf(buff, "%06X", program_length);
		strcat(obj_code[0], buff);
	}

	clear_symtab();

	fclose(intermediate_fp);
	fclose(symbol_fp);
	fclose(listing_fp);

	printf("%d\n", obj_idx);
	object_fp		= fopen(object_filename,		"wb");
	for (i = 0; i < obj_idx; ++i) {
		fprintf(object_fp, "%s\n", obj_code[i]);
	}
	fclose(object_fp);
}
