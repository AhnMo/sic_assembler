#ifndef __UTIL_H__

#define __UTIL_H__


void hex_to_int(char *h, int *i);
void int_to_hex(int  i, char *h);
void str_to_int(char *h, int *i);
void int_to_str(int  i, char *h);
int is_numeric(char *s);
int is_float(char *operand);

#define FILE_FAIL_CHECK(fp) \
if (fp == NULL) { \
	perror("fopen error");\
	exit(1);\
}

#endif
