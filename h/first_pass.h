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
#define NOT_IN_TABLE -1

typedef struct Symbol {
    char *name;
    char *instruction;
    char *attribute;
    int value;
} Symbol;

/* Setting IC and DC as globals */
extern int IC;
extern int DC;

int first_pass(char *file_path, hashmap_t *mcro_tb);
int insert_symbol(char *name, char *instruction, char *attribute, int value, hashmap_t *sym_tb, hashmap_t *mcro_tb);

#endif
