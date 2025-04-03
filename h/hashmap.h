#ifndef HASHMAP_H
#define HASHMAP_H

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "string_funcs.h"

#define TABLE_SIZE 51 /* Starting size for the hashmap */
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

/**
 * @brief Computes a hash value for a given string key.
 * @note Uses a left-shift and additive method. The final hash value is taken modulo the table size.
 *
 * @param key        The string key to hash.
 * @param table_size The size of the hash table.
 *
 * @return The computed hash value as an unsigned int.
 */
unsigned int hash(char *key, int table_size);

/**
 * @brief Initializes a hashmap.
 * @note Allocates memory for the hash table and initializes its buckets to NULL.
 *
 * @param map          Pointer to the hashmap to initialize.
 * @param initial_size The initial size of the hashmap.
 */
void init_hashmap(hashmap_t *map, int initial_size);

/**
 * @brief Inserts a new key-value pair into the hashmap.
 * @note Checks if resizing is needed before inserting the new node.
 *
 * @param map   Pointer to the hashmap.
 * @param value The value to be stored.
 * @param key   The key associated with the value.
 */
void insert(hashmap_t *map, void *value, char *key);

/**
 * @brief Resizes the hashmap by doubling its capacity.
 * @note Rehashes all existing nodes into a new table of double the current size.
 *       If memory allocation for the new table fails, the original table remains unchanged.
 *
 * @param map Pointer to the hashmap to resize.
 */
void resize(hashmap_t *map);

/**
 * @brief Looks up a value in the hashmap by key.
 * @note Returns the value associated with the key if found; otherwise, returns NULL.
 *
 * @param map Pointer to the hashmap.
 * @param key The key to search for.
 *
 * @return Pointer to the value associated with the key, or NULL if the key is not found.
 */
void *lookup(hashmap_t *map, char *key);

/**
 * @brief Frees the hashmap and all its nodes.
 * @note For each node in the hashmap, the provided free_value function is used to free the node's value.
 *
 * @param map         Pointer to the hashmap to free.
 * @param free_value  Function pointer to free the value stored in each node.
 */
void free_hashmap(hashmap_t *map, void (*free_value)(void *));

#endif /* HASHMAP_H */