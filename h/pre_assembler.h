#ifndef PRE_ASSEMBLER_H
#define PRE_ASSEMBLER_H

#define MACRO_START_STRING "mcro"
#define MACRO_END_STRING "mcroend"
#define STOP_STRING "STOP"
#define MAX_MCRO_NAME_LENGTH 31

#include "hashmap.h"
#include <stdio.h>
#include <ctype.h>

#define MACRO_FOUND 1
#define MACRO_NOT_FOUND 0

#define INITIAL_MACRO_SIZE 256 
#define MACRO_GROWTH_FACTOR 2

/**
 * struct Macro - Represents a macro definition used by the pre-assembler.
 *
 * This data structure stores information about a macro including its name and
 * the corresponding replacement text (body).
 *
 * Members:
 *   name - A pointer to a string containing the macro's name.
 *   body - A pointer to a string containing the macro's body.
 */
typedef struct Macro {
    char *name;
    char *body;
} Macro;

int pre_comp(char *src_path);

Macro *parse_macro(char *input, char *filename, FILE *file);

void paste_macro(char *name, char *search_text, char *filename, hashmap_t *map);

int is_valid_macro_name(char *name);

char *is_macro_start(char *input);

char *is_macro(char *input, hashmap_t *map);

#endif /* PRE_ASSEMBLER_H */