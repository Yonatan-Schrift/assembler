#ifndef FIRST_PASS_H
#define FIRST_PASS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hashmap.h"

#define DATA 1
#define CODE 2
#define EXTERNAL 3
#define ENTRY 4
#define NOT_IN_TABLE -1

typedef struct Symbol {
    char *name;
    char *instruction;
    int value;
} Symbol;

extern int IC;
extern int DC;
extern int ICF;
extern int DCF;


int first_pass(char *file_path);
int insert_symbol(char *name, char *instruction, int value, hashmap_t *map);

#endif
