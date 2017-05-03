#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "util.h"

void hex_to_int(char *h, int *i) { sscanf (h, "%X", i); }
void int_to_hex(int  i, char *h) { sprintf(h, "%X", i); }
void str_to_int(char *h, int *i) { sscanf (h, "%d", i); }
void int_to_str(int  i, char *h) { sprintf(h, "%d", i); }

int is_numeric(char *s) {
	int i, len = strlen(s);
	for (i = 0; i < len; ++i) {
		if (!isdigit(*(s + i)))
			return 0;
	}
	return 1;
}

int is_float(char *operand) {               //if OPcodeis WORD or BYTE, execute is_float(operand).
	int i=0, len = strlen(operand);
	int flag=0;

	if(operand[0]=='#' || operand[0]=='+' || operand[0]=='@')
		i=1;
	
	if(operand[i]=='.')
		return 0;

	for(i ; i < len ; i++) { 
		if(operand[i]<'0' || operand[i]>'9') {
			if(operand[i] == '.') {
				flag++;
				continue;
			}
			else
				return 0;
		}
	}
	return (flag == 1) ? 1 : 0;            //float -> flag=1, else -> flag=0					 
}