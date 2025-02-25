#ifndef PRE_ASSEMBLER_H
#define PRE_ASSEMBLER_H


#define STOP_STRING "STOP"

#include "hashmap.h"
#include "macro.h"

#define MACRO_FOUND 1
#define MACRO_NOT_FOUND 0

#define INITIAL_MACRO_SIZE 256 
#define MACRO_GROWTH_FACTOR 2


int pre_comp(char *src_path);

Macro *parse_macro(char *input, char *filename, FILE *file);

#endif /* PRE_ASSEMBLER_H */