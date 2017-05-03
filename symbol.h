#ifndef __SYMBOL_H__
#define __SYMBOL_H__

#include "hash.h"


struct symbol_t {
	char *symbol;
	int locctr;
};

extern int symbol_count;
extern char *symbol_list[0x1000];
extern hashtable_t *ht_symtab;

void init_symtab();
int insert_symtab(char *symbol, int locctr);
struct symbol_t *get_symtab_by_idx(int idx);
struct symbol_t *get_symtab_by_name(char *name);
void clear_symtab();

#endif
