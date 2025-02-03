#ifndef PRE_ASSEMBLER_H
#define PRE_ASSEMBLER_H

#define MACRO_START_STRING "mcro"
#define MACRO_END_STRING "mcro_end"

#include "hashmap.h"
#include <stdio.h>

#define INITIAL_MACRO_SIZE 256 
#define MACRO_GROWTH_FACTOR 2

typedef struct Macro {
    char *name;
    char *body;
} Macro;

int pre_comp(FILE *file);
Macro *parse_macro(char *input, FILE *file);
int save_macro(hashmap_t *map, Macro *macro);
int check_macro_name(char *name);





#endif /* PRE_ASSEMBLER_H */