#include <stdio.h>
#include <string.h>

#include "util.h"

void hex_to_int(char *h, int *i) { sscanf (h, "%x",  i); }
void int_to_hex(int *i, char *h) { sprintf(h, "%x", *i); }
void str_to_int(char *h, int *i) { sscanf (h, "%d",  i); }
void int_to_str(int *i, char *h) { sprintf(h, "%d", *i); }