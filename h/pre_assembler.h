#ifndef PRE_ASSEMBLER_H
#define PRE_ASSEMBLER_H



#include "hashmap.h"
#include "macro.h"

#define NOT_A_MACRO 999


#define INITIAL_MACRO_SIZE 256 
#define MACRO_GROWTH_FACTOR 2

int pre_comp(char *src_path, hashmap_t *mcro_table);

int parse_macro(char *input, int *line_count, FILE *file, Macro *mcro);

#endif /* PRE_ASSEMBLER_H */