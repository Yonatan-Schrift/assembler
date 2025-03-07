#ifndef FIRST_PASS_H
#define FIRST_PASS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hashmap.h"

#define DATA "data"
#define CODE "code"
#define EXTERNAL "external"
#define ENTRY "entry"

#define INITIAL_DATA_SIZE 128

typedef struct op_code {
	char *op_code_name;
	int opcode;
	int funct;
	int args_num;
} op_code;

typedef struct Symbol {
    char *name;
    char *attribute;
    int value;
} Symbol;

/* Setting IC and DC as globals */
extern int IC;
extern int DC;

int first_pass(char *file_path, hashmap_t *mcro_tb);

int insert_symbol(char *name, char *attribute, int value, hashmap_t *sym_tb, hashmap_t *mcro_tb);

int add_data_word(int value, int *data_cap, int **data_image);

int find_opcode(char *string); 

#endif /* FIRST_PASS_H */