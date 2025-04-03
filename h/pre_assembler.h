#ifndef PRE_ASSEMBLER_H
#define PRE_ASSEMBLER_H



#include "hashmap.h"
#include "macro.h"

#define NOT_A_MACRO 999


#define INITIAL_MACRO_SIZE 256 
#define MACRO_GROWTH_FACTOR 2

/**
 * @brief Pre-compiles the source file by processing macros.
 * @note Opens the source file (with ".as" extension) for reading and creates an output file (with ".am" extension).
 *       The function processes each line to handle macro definitions and macro usages. If a macro is defined,
 *       it is added to the macro table; if a macro is used, its body is substituted into the output.
 *       Internal comments have been added inside the method for clarity.
 *
 * @param src_path   The path to the source file (without extension).
 * @param mcro_table Pointer to a hashmap for storing macros.
 *
 * @return SUCCESS_CODE on success; otherwise, EXIT_FAILURE.
 */
int pre_comp(char *src_path, hashmap_t *mcro_table);

/**
 * @brief Parses a macro definition from the input and file stream.
 * @note Processes the macro start line, validates the macro name and ensures no label is present,
 *       then accumulates subsequent lines until the macro end directive is encountered.
 *       Error codes are returned on failure.
 *
 * @param input      The initial input line with the macro start.
 * @param line_count Pointer to the current line count, which is updated as lines are read.
 * @param file       File stream pointer for reading additional lines.
 * @param mcro       Pointer to the Macro structure to store the macro name and body.
 *
 * @return EXIT_SUCCESS on success; otherwise, an error code.
 */
int parse_macro(char *input, int *line_count, FILE *file, Macro *mcro);

#endif /* PRE_ASSEMBLER_H */