
#define TABLE_SIZE 101 /* Starting size for the hashmap */
#define LOAD_FACTOR_THRESHOLD 0.75 /* The percentage for resizing the hashmap  */

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "../h/pre_assembler.h"

typedef struct HashNode {
    char *key;
    Macro *value;
    struct HashNode *next;
} HashNode;

typedef struct hashmap {
    HashNode **table;
    int size;
    int count;
} hashmap_t;

/* Hash Function */
unsigned int hash(char *key, int table_size) {
    unsigned int hash_value = 0;
    while (*key) {
        hash_value = (hash_value << 5) + *key;
        key++;
    }
    return hash_value % table_size;
}

/* Initialize the hashmap */
void init_hashmap(hashmap_t *map, int initial_size) {
    int i;

    map->size = initial_size;
    map->count = 0;
    map->table = (HashNode **)malloc(sizeof(HashNode *) * map->size);

    for (i = 0; i < map->size; i++) {
        map->table[i] = NULL;
    }
}

void resize(hashmap_t *map) {
    int i;

    int new_size = map->size * 2;  /* Double the size of the table */
    HashNode **new_table = (HashNode **)malloc(sizeof(HashNode *) * new_size);
    
    if(new_table == NULL) {
        return;
    }
    for (i = 0; i < new_size; i++) {
        new_table[i] = NULL;
    }

    /* Rehash all existing nodes into the new table */
    for (i = 0; i < map->size; i++) {
        HashNode *node = map->table[i];
        while (node) {
            unsigned int new_index = hash(node->key, new_size);
            HashNode *next_node = node->next;

            node->next = new_table[new_index];
            new_table[new_index] = node;

            node = next_node;
        }
    }

    free(map->table);
    map->table = new_table;
    map->size = new_size;  // Update the table size
}

void insert(hashmap_t *map, char *key, Macro *value) {
    /* Check if resizing is necessary */
    if ((float)map->count / map->size > LOAD_FACTOR_THRESHOLD) {
        resize(map);
    }

    unsigned int index = hash(key, map->size);
    HashNode *new_node = (HashNode *)malloc(sizeof(HashNode));
    if(new_node == NULL) {
        return;
    }
    new_node->key = (char*)strdup(key);       /* I need to create strdup */
    new_node->value = value;
    new_node->next = map->table[index];  /* Insert at the head of the list */
    map->table[index] = new_node;

    map->count++;  /* Increment the element count */
}

Macro *lookup(hashmap_t *map, char *key) {
    unsigned int index = hash(key, map->size);
    HashNode *node = map->table[index];

    while (node != NULL) {
        if (strcmp(node->key, key) == 0) {
            return node->value;
        }
        node = node->next;
    }
    return NULL; /* Return NULL if key is not found */
}

void free_hashmap(hashmap_t *map) {
    int i;
    HashNode *node, *temp;

    if (map == NULL) return;

    for (i = 0; i < map->size; i++) {
        node = map->table[i];

        while (node != NULL) {
            temp = node;
            node = node->next;
            free(temp->value);
            free(temp->key);
            free(temp);
        }
    }
    free(map->table);
    free(map);
}

