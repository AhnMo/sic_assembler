#include "pass1.h"

#define OPTAB_HASH 1		// remove or comment this line make disable ht

struct opcode_t optab[] = {
	{"ADD",		3, "18", 1, "m"},
	{"ADD",		3, "58", 1, "m"},	// XF
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

int find_flag(char* str) { // Determine whether string's first character is flag or not
	int flag;

	if(str[0]=='+')
		flag=1;    // 1 if + 
	if(str[0]=='#')
		flag=2;    // 2 if #
	if(str[0]=='@')
		flag=3;    // 3 if @
	else
		flag=0;

	return flag;
}

int is_float(char* str) {   // if '.' appears in str, returns 1. Else 0.
	int i, ctr=0, len;

	len=strlen(str);

	if(find_flag(str)!=0)
		ctr=1;

	for(i=0 ; i<len ; i++) {
		if(str[ctr]=='.')
			return 1;
		ctr++;
	}
	return 0;   
}

int BYTE_length(char* str) {  //find length of BYTE operand
	int inlen, outlen;
	char buffer[10];
	
	inlen=strlen(str);

	strcpy(buffer, str);
	if(buffer[0]=='C' || buffer[0]=='c' && buffer[1]=='\'') {
		for(outlen=2 ; outlen<inlen ; outlen++) {
			if(buffer[outlen]=='\'') {
				outlen = outlen-2;
				break;
			}
		}
	}

	if(buffer[0]=='X' || buffer[0]=='x' && buffer[1] =='\'')
		outlen=1;

	return outlen;
}

char *directive[] = {"START", "END", "BYTE", "WORD", "RESB", "RESW", "BASE"} ;

int is_directive(char *str) {
	int i, size = sizeof(directive) / sizeof(char *) ;

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

	if (is_directive(token) || (op = get_instruction_info(token)) != NULL && op->n_o > 0) {
		token = strtok(NULL, s);
		strcpy(sta->operand, token);
	}

	return 0;
}

void hex_to_int(char *h, int *i) { sscanf (h, "%x",  i); }
void int_to_hex(int *i, char *h) { sprintf(h, "%x", *i); }

void pass1(char *src_filename, char *intermediate_filename, char *symbol_filename) {
	FILE *src_fp, *intermediate_fp;
	char buff[BUFSIZ];
	int start_addr, LOCCTR;

	struct statement_t statement;
	int is_comment;

#ifdef OPTAB_HASH
	init_optab();
#endif

	src_fp 			= (FILE *) fopen(src_filename,			"rb");
	intermediate_fp	= (FILE *) fopen(intermediate_filename, "wb");

#define READ_NEXT_INPUT_LINE {\
	fscanf(src_fp, "%[^\n]%*c", buff);\
	parse_line(buff, &statement);\
}

	READ_NEXT_INPUT_LINE; // read first line input

	if (strcmp(statement.opcode, "START") == 0) {
		hex_to_int(statement.operand, &start_addr);	// save #[OPERAND] as starting address
		LOCCTR = start_addr;	// initialize LOCCTR to starting address
		fprintf(intermediate_fp, "%d\n", start_addr); // write line to intermediate file
		READ_NEXT_INPUT_LINE;	// read next input line
	} else {
		LOCCTR = 0;
	}
	//"BYTE", "WORD", "RESB", "RESW", "BASE"
	do {
		if (!is_comment) {
			printf("%s/%s/%s\n", statement.symbol, statement.opcode, statement.operand);
			if(statement.opcode=="BYTE") {
				if(is_float(statement.operand)==1)
					LOCCTR = LOCCTR+6;
				else
					LOCCTR = LOCCTR+BYTE_length(statement.operand);
				fprintf(intermediate_fp, "%s\t%s\t%d\t", statement.opcode, statement.symbol, LOCCTR);   // directive label LOCCTR
			}
			if(statement.opcode=="WORD")
			if(statement.opcode=="RESW")
			if(statement.opcode=="BASE")
		}
		READ_NEXT_INPUT_LINE;	// read next input line
	} while (strcmp(statement.opcode, "END") != 0);

	fprintf(intermediate_fp, "???"); // write last line to intermediate file
	fprintf(intermediate_fp, "%d\n", LOCCTR - start_addr); // save (LOCCTR - starting address) as program length

	fclose(src_fp);
	fclose(intermediate_fp);
}