#include <stdio.h>
#include "symbol.h"
#include "hash.h"

int symbol_count = 0;
char *symbol_list[0x1000] = {};
hashtable_t *ht_symtab =  NULL;
void init_symtab() {
	ht_symtab = ht_create(0x1000);
	symbol_count = 0;
}

int insert_symtab(char *symbol, int locctr) {
	struct symbol_t *s = (struct symbol_t *) malloc(sizeof(struct symbol_t));
	s->symbol = symbol_list[symbol_count++] = strdup(symbol);
	s->locctr = locctr;

	ht_set(ht_symtab, symbol, (void *)s);

	return 1;
}
struct symbol_t *get_symtab_by_idx(int idx) {
	return (struct symbol_t *) ht_get(ht_symtab, symbol_list[idx]);
}
struct symbol_t *get_symtab_by_name(char *name) {
	return (struct symbol_t *) ht_get(ht_symtab, name);
}

void clear_symtab() {
	int i;
	struct symbol_t *s;

	for (i = symbol_count - 1; i >= 0; --i) {
		s = (struct symbol_t *) ht_get(ht_symtab, symbol_list[i]);
		//printf("%s %d\n", s->symbol, s->locctr);
		free(s);
		free(symbol_list[i]);
	}
	symbol_count = 0;
	ht_symtab = NULL;
}
