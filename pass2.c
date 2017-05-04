#include "pass2.h"

#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "opcode.h"
#include "statement.h"
#include "util.h"
#include "symbol.h"
//#include "hash.h"

//#define SICXE_MACHINE
#define WORD_SIZE	3

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

	token = strtok(NULL, s);
	strcpy(sta->operand, token);

	token = strtok(NULL, s);
	hex_to_int(token, &tmp);
	sta->flag = tmp;

	token = strtok(NULL, s);
	hex_to_int(token, &tmp);
	sta->size = tmp;

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
		strcpy(buf, a);
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
	int t, tmp, program_length, i, cur, current_text_record_length, operand_addr, start_addr, base_relative, base_offset;
	struct statement_t statement;
	struct opcode_t *op;
	struct symbol_t *sym;
	char *ptr, symbol[32];

	obj_idx = 0;

	intermediate_fp	= fopen(intermediate_filename,	"rb");
	FILE_FAIL_CHECK(intermediate_fp);
	symbol_fp		= fopen(symbol_filename,		"rb");
	FILE_FAIL_CHECK(symbol_fp);
	listing_fp		= fopen(listing_filename,		"wb");
	FILE_FAIL_CHECK(listing_fp);

	init_symtab();
	while (fscanf(symbol_fp, "%[^\n]%*c", buff) != EOF) {
		ptr = strtok(buff, " \t");
		strcpy(symbol, ptr);
		ptr = strtok(NULL, " \t");
		hex_to_int(ptr, &tmp);
		insert_symtab(symbol, tmp);
	}

#define NEW_OBJ_LINE	(obj_code[obj_idx++]=(char *) malloc(sizeof(char) * BUFSIZ))
#define CURRENT_OBJ_LINE (obj_code[obj_idx - 1])
#define READ_NEXT_INTERMEDIATE_LINE {\
	fscanf(intermediate_fp, "%[^\n]%*c", buff);\
	pass2_parse_line(buff, &statement);\
}

#define WRITE_LINE_TO_LISTING_FILE(s) {\
	fprintf(listing_fp, "%04x\t%-10s\t%-10s\t%-10s\t%-10s\n",\
	s.loc, s.symbol, s.opcode, s.operand, s.objcode);\
}

	READ_NEXT_INTERMEDIATE_LINE;

	if (strcmp(statement.opcode, "START") == 0) {
		WRITE_LINE_TO_LISTING_FILE(statement);
		NEW_OBJ_LINE;

		sprintf(obj_code[0], "H^%-6s^%06x^", statement.symbol, statement.loc);

		start_addr = cur = statement.loc;
		READ_NEXT_INTERMEDIATE_LINE;
	} else {
		cur = 0;
	}

	NEW_OBJ_LINE;

	current_text_record_length = 0;
	base_relative = 0;
	base_offset = 0;
	while (strcmp(statement.opcode, "END") != 0) {
		//printf("%04x\t%-10s\t%-10s\t%04x\t%04x\n", statement.loc, statement.opcode, statement.operand, statement.flag, statement.size);
		memset(buff, 0, BUFSIZ);
		if ((op = get_instruction_info(statement.opcode)) != NULL) {
			switch (op->format) {
			case 2:
				operand_addr = 0;
				if (strcmp(op->n_f, "rr") == 0) {
					ptr = strtok(statement.operand, ",");
					operand_addr += get_register_value(ptr) << 4;
					ptr = strtok(NULL, "\0");
					operand_addr += get_register_value(ptr);
				} else if (strcmp(op->n_f, "r") == 0){
					operand_addr += (get_register_value(statement.operand) & 0b1111) << 4;
				} else if (strcmp(op->n_f, "rn") == 0) {
					ptr = strtok(statement.operand, ",");
					operand_addr += get_register_value(ptr) << 4;
					ptr = strtok(NULL, "\0");
					str_to_int(ptr, &t);
					operand_addr += t;
				} else {
					// unknow format
					exit(1);
				}
				sprintf(buff, "%02X%02X", op->code, operand_addr);
				break;
			case 3:
#ifdef SICXE_MACHINE

				if (op->n_o > 0) {
					t = 0;

					if ((statement.flag & 0b10000000) != 0) {
						// extended
						t = SET_EXTENDED(t);
						t = SET_SIMPLE_XE(t);
					} else {
						if ((statement.flag & 0b01000000) != 0) {
							// immediate
							t = SET_IMMEDIATE(t);
						} else if ((statement.flag & 0b00100000) != 0) {
							// indirect
							t = SET_INDIRECT(t);
						} else {
							t = SET_SIMPLE_XE(t);
						}

						//if (base_relative) {
						//	t = SET_BASERELATVIE(t);
						//} else
						if ((statement.flag & 0b01000000) == 0)
						{
							t = SET_PCRELATVIE(t);
						}
					}


					ptr = strtok(statement.operand, ",");
					if (is_numeric(ptr)) {
						str_to_int(ptr, &operand_addr);
					} else if (is_float(ptr)) {
					} else {
						sym = get_symtab_by_name(ptr);
						if (sym == NULL) {
							fprintf(stderr, "undefined symbol: %s\n", ptr);
							pass2_unlink_files(listing_filename, object_filename);
							exit(1);
						}
						//if (base_relative) {
						//	operand_addr = sym->locctr - base_offset;
						//} else
						if ((statement.flag & 0b10000000) == 0) {
							operand_addr = sym->locctr - statement.loc - WORD_SIZE;
						} else {
							operand_addr = sym->locctr;
						}
					}
				}
				//printf("t: %d\n", t);
				t = (op->code << 4) + t;

				if ((statement.flag & 0b10000000) != 0) { // extended
					sprintf(buff, "%08X", ASSEMBLE_EXT(t, operand_addr));
				} else { // not extended
					sprintf(buff, "%06X", ASSEMBLE(t, operand_addr));
				}
#else
				if (op->n_o > 0) {
					ptr = strtok(statement.operand, ",");
					sym = get_symtab_by_name(ptr);
					if (sym == NULL) {
						fprintf(stderr, "undefined symbol: %s\n", ptr);
						pass2_unlink_files(listing_filename, object_filename);
						exit(1);
					}
					operand_addr = sym->locctr;
					if (strtok(NULL, "\0") != 0) {
						operand_addr |= 0x8000;
					}
				} else {
					operand_addr = 0;
				}
				sprintf(buff, "%02X%04X", op->code, operand_addr);
#endif
				break;
			case 1:
				sprintf(buff, "%02X", op->code);
				break;
			default:
				operand_addr = 0;
			}
		} else if (strcmp(statement.opcode, "BYTE") == 0){
			sprintf(buff, "%s", convert_constant(statement.operand));
		} else if (strcmp(statement.opcode, "WORD") == 0){
			str_to_int(convert_constant(statement.operand), &t);
			sprintf(buff, "%06x", t);
		} else if (strcmp(statement.opcode, "RESW") == 0 || strcmp(statement.opcode, "RESB") == 0) {
			sprintf(buff, "");
		} else if (strcmp(statement.opcode, "BASE") == 0) {
			base_relative = 1;

			sym = get_symtab_by_name(statement.operand);
			if (sym == NULL) {
				fprintf(stderr, "undefined symbol: %s\n", statement.symbol);
				pass2_unlink_files(listing_filename, object_filename);
				exit(1);
			}
			base_offset = sym->locctr;
		} else if (strcmp(statement.opcode, "NOBASE") == 0) {
			base_relative = 0;
			base_offset = 0;
		}

		strcpy(statement.objcode, buff);
		WRITE_LINE_TO_LISTING_FILE(statement);

		if (current_text_record_length + statement.size > 0x1E) {
			if (strcmp(record, "") != 0) {
				sprintf(CURRENT_OBJ_LINE, "T^%06X^%02X%s", cur, current_text_record_length, record);
				NEW_OBJ_LINE;
			}

			cur += current_text_record_length;
			current_text_record_length = 0;
			memset(record, 0, BUFSIZ);
		}
		if (strcmp(buff, "") != 0) {
			strcpy(record2, record);
			sprintf(record, "%s^%s", record2, buff);
		}

		current_text_record_length += statement.size;

		READ_NEXT_INTERMEDIATE_LINE;
	}
	sprintf(CURRENT_OBJ_LINE, "T^%06X^%02X%s", cur, current_text_record_length, record);
	NEW_OBJ_LINE;
	sprintf(CURRENT_OBJ_LINE, "E^%06X", start_addr);


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

	//printf("%d\n", obj_idx);
	object_fp = fopen(object_filename, "wb");
	FILE_FAIL_CHECK(object_fp);
	for (i = 0; i < obj_idx; ++i)
		fprintf(object_fp, "%s\n", obj_code[i]);
	fclose(object_fp);
}
