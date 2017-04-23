#include "hash.h"

hashtable_t *ht_create(int size) {
	hashtable_t *hashtable = NULL;
	int i;

	if( size < 1 ) { return NULL; }

	if( ( hashtable = malloc( sizeof( hashtable_t ) ) ) == NULL ) {
		return NULL;
	}

	if( ( hashtable->table = malloc( sizeof( entry_t * ) * size ) ) == NULL ) {
		return NULL;
	}
	for( i = 0; i < size; i++ ) { hashtable->table[i] = NULL; }

	hashtable->size = size;

	return hashtable;
}

/*
int ht_hash(hashtable_t *hashtable, char *key) {
	unsigned long int hashval;
	int i;

	for (i = 0; hashval < ULONG_MAX && i < strlen(key); ++i) {
		hashval = hashval << 8;
		hashval += key[i];
	}

	return hashval % hashtable->size;
}
*/
// https://rosettacode.org/wiki/Hash_from_two_arrays#C
int ht_hash(hashtable_t *hashtable, char *key) {
    unsigned h = 0;
    unsigned hl, hr;

    while(*key) {
        h += *key;
        hl = 0x5C5 ^ (h & 0xfff00000) >> 18;
        hr = (h & 0x000fffff);
        h = hl ^ hr ^ *key++;
    }
    return h % hashtable->size;
}

entry_t *ht_newpair(char *key, void *value) {
	entry_t *newpair;

	if ((newpair = malloc(sizeof(entry_t))) == NULL) {
		return NULL;
	}

	if ((newpair->key = strdup(key)) == NULL) {
		return NULL;
	}

	if((newpair->value = value) == NULL) {
		return NULL;
	}

	newpair->next = NULL;

	return newpair;
}

void ht_set(hashtable_t *hashtable, char *key, void *value) {
	int bin = 0;
	entry_t *newpair = NULL;
	entry_t *next = NULL;
	entry_t *last = NULL;

	bin = ht_hash(hashtable, key);

	next = hashtable->table[bin];

	while (next != NULL && next->key != NULL && strcmp(key, next->key) > 0) {
		last = next;
		next = next->next;
	}

	if (next != NULL && next->key != NULL && strcmp(key, next->key) == 0) {
		next->value = value;
	} else {
		newpair = ht_newpair(key, value);
		if (next == hashtable->table[bin]) {
			newpair->next = next;
			hashtable->table[bin] = newpair;
		} else if (next == NULL) {
			last->next = newpair;
		} else {
			newpair->next = next;
			last->next = newpair;
		}
	}
}

void *ht_get(hashtable_t *hashtable, char *key) {
	int bin = ht_hash(hashtable, key);
	entry_t *pair;

	pair = hashtable->table[bin];
	while (pair != NULL && pair->key != NULL && strcmp(key, pair->key) > 0) {
		pair = pair->next;
	}

	if (pair == NULL || pair->key == NULL || strcmp(key, pair->key) != 0) {
		return NULL;
	} else {
		return pair->value;
	}
}
