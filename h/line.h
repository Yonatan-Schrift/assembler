#ifndef LINE_H
#define LINE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/**
 * @brief Represents a line in an assembly file.
 *
 * This structure contains the components of a single line in an assembler program.
 * It includes:
 *   - A label, which acts as an identifier for the line if present.
 *   - A command, which specifies the instruction or directive.
 *   - An array of arguments related to the command.
 */
typedef struct Line {
	char *label;
	char *command;
	char **arguments;
	char *input_copy; 
} Line;

/**
 * @brief Initializes a Line structure by setting its fields to NULL.
 * @note If the provided pointer is NULL, the function does nothing.
 *
 * @param line Pointer to the Line structure to initialize.
 */
void init_line(Line *line);

/**
 * @brief Reads a line from a file.
 * @note Reads characters until EOF or newline is encountered. If a ';' is found, the rest
 *       of the line is ignored. Returns an error if the line exceeds MAX_LINE_LENGTH.
 *
 * @param file Pointer to the file to read from.
 * @param line Buffer to store the read line.
 *
 * @return EXIT_SUCCESS if the line is read successfully;
 *         LINE_TOO_LONG if the line exceeds MAX_LINE_LENGTH;
 *         EXIT_FAILURE if EOF is reached without reading any input.
 */
int read_line(FILE *file, char *line);

/**
 * @brief Splits an input line into its components: label, command, and arguments.
 * @note Allocates memory for a copy of the input line and the arguments array.
 *       The caller is responsible for freeing output->input_copy and output->arguments.
 *
 * @param line   The input line to be split.
 * @param output Pointer to a Line structure to store the parsed components.
 *
 * @return EXIT_SUCCESS if the line is split successfully;
 *         EXTRA_COMMA_AFTER_PARAM_PRE_ASS if an extra comma is found after the last argument;
 *         FAIL_CODE or EXIT_FAILURE on memory allocation or invalid input.
 */
int split_line(char *line, Line *output);

/**
 * @brief Frees memory allocated for a Line structure.
 * @note Only frees the dynamically allocated memory for the arguments array and the input copy.
 *       The label and command pointers are set to NULL because they point to parts of input_copy.
 *
 * @param line Pointer to the Line structure to free.
 */
void free_line(Line *line);

/**
 * @brief Checks if a string is present in a null-terminated array of strings.
 * @note Returns the index of the first occurrence, or FALSE if not found.
 *
 * @param name  The string to search for.
 * @param array The null-terminated array of strings.
 *
 * @return The index of the string if found; otherwise, FALSE.
 */
int is_in_array(char *name, char **array);

/**
 * @brief Checks if a string is empty or contains only whitespace.
 * @note Returns TRUE if the input is NULL or every character is a whitespace character.
 *
 * @param line The string to check.
 *
 * @return TRUE if the string is empty or only whitespace; FALSE otherwise.
 */
int isEmpty(char *line);

/**
 * @brief Checks if the given name is one of the operation names.
 * @note Compares the provided name against the OP_NAMES array.
 *
 * @param name The string to check.
 *
 * @return The index of the name in the OP_NAMES array if found; otherwise, FALSE.
 */
int is_op_name(char *name);


/**
 * @brief Checks if the given name is a register name.
 * @note Compares the provided name against the REGISTERS array.
 *
 * @param name The string to check.
 *
 * @return The index of the name in the REGISTERS array if found; otherwise, FALSE.
 */
int is_register(char *name);

/**
 * @brief Checks if the given name is an instruction name.
 * @note Compares the provided name against the INSTRUCTIONS array.
 *
 * @param name The string to check.
 *
 * @return The index of the name in the INSTRUCTIONS array if found; otherwise, FALSE.
 */
int is_instruction(char *name);

#endif /* LINE_H */