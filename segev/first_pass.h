#ifndef FIRST_PASS_H
#define FIRST_PASS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DATA 1
#define CODE 2
#define EXTERNAL 3
#define ENTRY 4
#define NOT_IN_TABLE -1

typedef struct Symbol {
    char *name;
    int value;
    int instruction;
} Symbol;


extern Symbol *symbolTable;
extern int symbolCount;
extern int IC;
extern int DC;
extern int ICF;
extern int DCF;


int first_pass(char *file_path);
int findSymbol(char *name);
void insertSymbol(char *name, int value, int instruction);

#endif
