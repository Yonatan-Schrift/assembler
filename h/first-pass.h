#ifndef FIRST_PASS_H
#define FIRST_PASS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE_LENGTH 80

#define DATA 1
#define CODE 2
#define EXTERNAL 3
#define ENTRY 4

typedef struct {
    char name[IDK]; /*i dont know what to put inside insted of a number here*/
    int value;
    int instruction;
} Symbol;


extern Symbol *symbolTable;   
extern int symbolCount;       
extern int IC;                
extern int DC;                
extern int ICF;               
extern int DCF;               

int findSymbol(char *name);
void insertSymbol(char *name, int value, int instruction);
void processLine(char *line);
void processSourceFile(FILE *sourceFile);

#endif
