#ifndef __HASH_H__
#define __HASH_H__
// https://gist.github.com/tonious/1377667

#define _XOPEN_SOURCE 500

#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <string.h>

struct entry_s {
	char *key;
	void *value;
	struct entry_s *next;
};

typedef struct entry_s entry_t;

struct hashtable_s {
	int size;
	struct entry_s **table;
};

typedef struct hashtable_s hashtable_t;

hashtable_t *ht_create(int size);
int ht_hash(hashtable_t *hashtable, char *key);
entry_t *ht_newpair(char *key, void *value);
void ht_set(hashtable_t *hashtable, char *key, void *value);
void *ht_get(hashtable_t *hashtable, char *key);

#endif
