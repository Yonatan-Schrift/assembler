#ifndef HASHMAP_H
#define HASHMAP_H

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "string_funcs.h"

#define TABLE_SIZE 101 /* Starting size for the hashmap */
#define LOAD_FACTOR_THRESHOLD 0.75 /* The percentage for resizing the hashmap  */

/* Hashmap node structure */
typedef struct HashNode {
    char *key;
    void *value;
    struct HashNode *next;
} HashNode;

/* Hashmap structure */
typedef struct hashmap {
    HashNode **table;
    int size;
    int count;
} hashmap_t;

/* Function declarations */
unsigned int hash(char *key, int table_size);
void init_hashmap(hashmap_t *map, int initial_size);
void insert(hashmap_t *map, void *value, char *key);
void *lookup(hashmap_t *map, char *key);
void free_hashmap(hashmap_t *map);

#endif /* HASHMAP_H */