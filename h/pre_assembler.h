#ifndef PRE_ASSEMBLER_H
#define PRE_ASSEMBLER_H

#define MACRO_START_STRING "mcro"
#define MACRO_END_STRING "mcro_end"

#include "hashmap.h"
#include <stdio.h>

#define MACRO_FOUND 1
#define MACRO_NOT_FOUND 0

#define INITIAL_MACRO_SIZE 256 
#define MACRO_GROWTH_FACTOR 2

typedef struct Macro {
    char *name;
    char *body;
} Macro;

int pre_comp(char *src_path);

Macro *parse_macro(char *input, char *filename, FILE *file);

int check_macro_name(char *name);

void paste_macro(char *name, char *search_text, char *filename, hashmap_t *map);

char *is_macro_start(char *input);

char *is_macro(char *input, hashmap_t *map);

#endif /* PRE_ASSEMBLER_H */