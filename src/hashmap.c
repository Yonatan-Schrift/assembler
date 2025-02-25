
#include "../h/hashmap.h"
#include "../h/pre_assembler.h"


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
    return;
}

/* Resize the hashmap when reaching the threshold */
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
    map->size = new_size; /* Update the table size */
}

/* Insert a macro into the hashmap */
void insert(hashmap_t *map, void *value, char *key) {
    /* Check if resizing is necessary */
    HashNode *new_node;
    unsigned int index;

    if ((float)map->count / map->size > LOAD_FACTOR_THRESHOLD) {
        resize(map);
    }

    index = hash(key, map->size);
    new_node = (HashNode *)malloc(sizeof(HashNode));
    if(new_node == NULL) {
        return;
    }
    new_node->key = copy_string(key);
    new_node->value = value;
    new_node->next = map->table[index];  /* Insert at the head of the list */
    map->table[index] = new_node;

    map->count++;  /* Increment the element count */
}

/* Lookup a macro via key */
void *lookup(hashmap_t *map, char *key) {
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

/* Free the hashmap */
/* Might need to also free the macro here, not sure */
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

