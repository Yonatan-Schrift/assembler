#ifndef MACRO_H
#define MACRO_H

#include "globals.h"
#include "line.h"
#include "hashmap.h"

#define MACRO_START_STRING "mcro"
#define MACRO_END_STRING "mcroend"
#define MAX_MCRO_NAME_LENGTH 31

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

void init_macro(Macro *mcro);

/**
 * Checks if the given name is a valid macro name.
 *
 * @param name The macro name to validate.
 * @return TRUE if valid, FALSE otherwise.
 */
int is_valid_macro_name(char *name);

/**
 * Determines if the input line starts a macro definition.
 *
 * @param input The input line.
 * @return The macro name if the line defines a macro, otherwise NULL.
 */
char *is_macro_start(char *input, Line *line);

/**
 * Finds a macro within the hashmap based on the input line.
 *
 * @param input The input line.
 * @param map The hashmap storing macros.
 * @return The macro name if found, otherwise NULL.
 */
char *find_macro(char *input, hashmap_t *map);

/**
 * Pastes the macro body into a file at the specified search location.
 *
 * @param name The name of the macro.
 * @param search_text The text to search for the insertion point.
 * @param filename The file where the macro body will be inserted.
 * @param map The hashmap storing macros.
 */
void paste_macro(char *name, char *search_text, char *filename, hashmap_t *map);

#endif /* MACRO_H */