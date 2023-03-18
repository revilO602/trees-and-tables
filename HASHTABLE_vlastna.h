//my own hashtable with chaining
// DSA 2020
// Oliver Leontiev
//#define _CRT_SECURE_NO_DEPRECATE
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#define MUL_CONST 4294967296 //2^32
#define DIV_CONST 281474976710656 //2^48
#define RN_ONE 12985 //random pregenerated number
#define RN_TWO 26204 //random pregenerated number

typedef struct cell {
	int key;
	struct cell* next;
}CELL;

typedef struct bucket {
	CELL* first_cell;
}BUCKET;

typedef struct table {
	int size;
	int cells;
	BUCKET* buckets;
}TABLE;

unsigned long long xorshift(unsigned long long key) {
	key = key ^ (key >> 17);
	key = key ^ (key << 15);
	return key % DIV_CONST;
}

int hashOwn(int key,int tableSize) {
	unsigned long long hashkey = (unsigned long long)key * MUL_CONST;
	hashkey = xorshift(hashkey);
	hashkey = hashkey*RN_ONE + RN_TWO;
	return hashkey%tableSize;
}

CELL* addCellOwn(int key, BUCKET* bucket) {
	CELL* new_cell;
	if ((new_cell = (CELL*)malloc(sizeof(CELL)))==NULL)return NULL;
	new_cell->key = key;
	new_cell->next = bucket->first_cell;
	bucket->first_cell = new_cell;
	return new_cell;
}

TABLE* createTableOwn(int size) {
	TABLE* table;
	int i;
	if ((table = (TABLE*)malloc(sizeof(TABLE))) == NULL)return NULL;
	if ((table->buckets = (BUCKET*)malloc(size * sizeof(BUCKET)))==NULL)return NULL;
	for (i = 0; i < size; i++) {
		table->buckets[i].first_cell = NULL;
	}
	table->size = size;
	table->cells = 0;
	return table;
}
//checks if value is anywhere in the bucket
int checkBucket(BUCKET* bucket, int value) {
	CELL* act = bucket->first_cell;
	while (act != NULL) {
		if (act->key == value)
			return 1;
		act = act->next;
	}
	return 0;
}
void freeHTableOwn(TABLE* table) {
	int i;
	BUCKET* act_bucket;
	CELL* act_cell, * del;
	if (table == NULL)return;
	for (i = 0; i < table->size; i++) {
		act_bucket = &(table)->buckets[i];
		act_cell = act_bucket->first_cell;
		while (act_cell != NULL) {
			del = act_cell;
			act_cell = act_cell->next;
			free(del);
		}
	}
	free(table->buckets);
	free(table);
}

void insertHTableOwn(TABLE** table, int key);
//calculates next size: nearest prime to double the old size
nextSize(int old_size) {
	int new_size = (2 * old_size)+1;
	int i,prime=0;
	while (1) {
		int limit = floor(sqrt(new_size));
		for (i = 3; i <= limit; i += 2) {
			if (new_size % i == 0)break;
			prime = 1;
		}
		if (prime == 1)return new_size;
		new_size += 2;
	}
}
//resizes the hashtable
TABLE* rehashOwn(TABLE* old_table) {
	TABLE* new_table = createTableOwn(nextSize(old_table->size));
	int i;
	BUCKET* act_bucket;
	CELL* act_cell;

	for (i = 0; i < old_table->size; i++) {
		act_bucket = &(old_table)->buckets[i];
		act_cell = act_bucket->first_cell;
		while (act_cell != NULL) {
			insertHTableOwn(&new_table, act_cell->key);
			act_cell = act_cell->next;
		}
	}
	freeHTableOwn(old_table);
	return new_table;
}
void insertHTableOwn(TABLE** table,int key) {
	int hashkey = hashOwn(key, (*table)->size);
	if (!checkBucket(&((*table)->buckets[hashkey]), key)) { //we add the key only if its not already there
		addCellOwn(key, &(*table)->buckets[hashkey]);
		(*table)->cells++;
		if ((*table)->cells / (float)(*table)->size > 0.5) // keeping load factor under 0.5
			* table = rehashOwn(*table);
	}
}


int searchHTOwn(TABLE* table, int key) {
	int hashkey = hashOwn(key, table->size);
	return checkBucket(&(table->buckets[hashkey]), key);
}
