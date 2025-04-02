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

#endif /* LINE_H */