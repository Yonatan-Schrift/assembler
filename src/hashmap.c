
#include "../h/hashmap.h"

unsigned int hash(char *key, int table_size) {
	unsigned int hash_value = 0;
	while (*key) {
		hash_value = (hash_value << 5) + *key;
		key++;
	}
	return hash_value % table_size;
}

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

void resize(hashmap_t *map) {
	int i, new_size;
	unsigned int new_index;
	HashNode *next_node, *node, **new_table;

	new_size = map->size * 2; /* Double the size of the table */
	new_table = (HashNode **)malloc(sizeof(HashNode *) * new_size);

	if (new_table == NULL) {
		return;
	}
	for (i = 0; i < new_size; i++) {
		new_table[i] = NULL;
	}

	/* Rehash all existing nodes into the new table */
	for (i = 0; i < map->size; i++) {
		node = map->table[i];
		while (node) {
			new_index = hash(node->key, new_size);
			next_node = node->next;

			node->next = new_table[new_index];
			new_table[new_index] = node;

			node = next_node;
		}
	}

	free(map->table);
	map->table = new_table;
	map->size = new_size; /* Update the table size */
}

void insert(hashmap_t *map, void *value, char *key) {
	HashNode *new_node;
	unsigned int index;

	/* Resize the hashmap if the load factor threshold is exceeded */
	if ((float)map->count / map->size > LOAD_FACTOR_THRESHOLD) {
		resize(map);
	}

	index = hash(key, map->size);
	new_node = (HashNode *)malloc(sizeof(HashNode));
	if (new_node == NULL) {
		return;
	}
	new_node->key = copy_string(key);
	new_node->value = value;
	new_node->next = map->table[index]; /* Insert at the head of the list */
	map->table[index] = new_node;

	map->count++; /* Increment the element count */
}

void *lookup(hashmap_t *map, char *key) {
	unsigned int index;
	HashNode *node;

	if (!key || !map) return NULL;

	index = hash(key, map->size);
	node = map->table[index];

	while (node != NULL) {
		if (strcmp(node->key, key) == 0) {
			return node->value;
		}
		node = node->next;
	}
	return NULL; /* Return NULL if key is not found */
}

void free_hashmap(hashmap_t *map, void (*free_value)(void *)) {
	int i;
	HashNode *node, *temp;

	if (map == NULL) return;

	for (i = 0; i < map->size; i++) {
		node = map->table[i];

		while (node != NULL) {
			temp = node;
			node = node->next;

			/* Free the value using the provided function */
			free_value(temp->value);

			free(temp->key);
			free(temp);
		}
	}

	free(map->table);
}
