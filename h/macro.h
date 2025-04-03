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

/**
 * @brief Initializes a Macro structure.
 * @note Sets the macro's body and name pointers to NULL.
 *
 * @param mcro Pointer to the Macro structure to initialize.
 */
void init_macro(Macro *mcro);

/**
 * Checks if the given name is a valid macro name.
 *
 * @param name The macro name to validate.
 * @return TRUE if valid, error code otherwise.
 */
int is_reserved_name(char *name);

/**
 * @brief Determines if the input line begins a macro definition.
 * @note Checks that the input is non-empty and that the command in the parsed Line
 *       structure matches MACRO_START_STRING. If so, returns the macro name (first argument).
 *
 * @param input The original input string.
 * @param line  Pointer to the parsed Line structure.
 *
 * @return The macro name if the line defines a macro; otherwise, NULL.
 */
char *is_macro_start(char *input, Line *line);

/**
 * @brief Searches for a macro definition in the provided macro hashmap.
 * @note Parses the input line to extract the command and then looks it up in the hashmap.
 *       If a macro is found and the first argument is absent, returns a copy of the command as the macro name.
 *
 * @param input The input line potentially containing a macro definition.
 * @param map   Pointer to the hashmap storing macros.
 *
 * @return A dynamically allocated string containing the macro name if found; otherwise, NULL.
 */
char *find_macro(char *input, hashmap_t *map);

/**
 * @brief Frees the memory allocated for a Macro structure.
 * @note Frees the macro's name and body if they exist, then frees the Macro structure itself.
 *
 * @param mcro Pointer to the Macro structure to free.
 */
void free_macro(Macro *mcro);

#endif /* MACRO_H */