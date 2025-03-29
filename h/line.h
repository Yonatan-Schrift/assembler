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
} Line;

void init_line(Line *line);

/**
 * Reads a line from standard input character by character.
 * Dynamically allocates memory for the line string, doubling the buffer size when needed.
 *
 * @return A pointer to the dynamically allocated string containing the line,
 *         or NULL if:
 *         - Memory allocation fails
 *         - EOF is encountered with no input
 *
 * @note The returned string must be freed by the caller when no longer needed
 * 
 *          Code taken from maman22 by Yonatan Schrift
 */
int read_line(FILE *file, char *line);

/**
 * split_line - Parses an input string to extract a label, command, and arguments.
 *
 * This function processes a line from the assembler input by following these steps:
 *   1. Removes any comments (text following a semicolon ';').
 *   2. Checks if the first token contains a colon ':'. if so, it considers that token as a label.
 *   3. The next token (after an optional label) is treated as the command.
 *   4. Any subsequent tokens are collected as arguments.
 *
 * The resulting tokens are stored in a dynamically allocated Line structure, where:
 *   - 'label' holds the label (if it exists),
 *   - 'command' holds the command,
 *   - 'arguments' is an array of strings holding the arguments.
 *
 * The caller is responsible for freeing the allocated Line structure and any associated memory.
 *
 * @param line A null-terminated string containing the line to be parsed.
 * @return A pointer to a Line structure with the parsed components, or NULL if the line is NULL or a memory allocation error occurs.
 */
int split_line(char *line, Line *output);

/** 
 * A cool ass method for freeing a line.
 */
void free_line(Line *line);

int is_in_array(char *name, char **array);

int isEmpty(char *line);

int is_op_name(char *name);

int is_register(char *name);

int is_instruction(char *name);

/**
 * Cleans a string argument by removing all whitespace characters.
 *
 * @param arg The input string to be cleaned.
 *
 * @return A newly allocated string with all whitespace characters removed,
 *         or NULL if:
 *         - Input is NULL
 *         - Input is empty string
 *         - Memory allocation fails
 *
 * @note The caller is responsible for freeing the returned string.
 * code taken from Maman 22 by Yonatan Schrift
 */
char *clean_arg( char *arg);


#endif /* LINE_H */