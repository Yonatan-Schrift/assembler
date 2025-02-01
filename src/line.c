/** yes, it's perfectly normal to create a whole diferent file for reading one line.  */

#include "../h/line.h"
#include "../h/argument_funcs.h"
#include "../h/globals.h"
#include "../h/string_funcs.h"
#include <stdlib.h>
#include <string.h>

typedef struct Line {
	char *label;
	char *command;
	char **arguments;
	int arg_count;
} Line;

char *read_line(void) {
	char cur;
	char *buffer, *line;
	size_t capacity, index;

	/* Initialize variables:
	 * capacity: Initial size of the dynamic buffer.
	 * index: Tracks the current position in the buffer.
	 */
	capacity = 8; /* starting size for the line */
	index = 0;

	/* Allocating initial memory */
	line = (char *)malloc(capacity);
	if (!line) {
		fprintf(stderr, "Failed memory allocation\n");
		return NULL; /* Memory Allocation Error */
	}

	/* Read input character by character */
	while ((cur = getchar()) != EOF && cur != '\n') {
		/* Line too long */
		if (index >= MAX_LINE_LENGTH) {
			free(line);
			return NULL;
		}

		/* Buffer is too small, double the capacity */
		if (index >= capacity - 1) {
			buffer = realloc(line, capacity * 2);
			if (!buffer) {
				free(line);
				return NULL; /* Memory Allocation Error */
			}
			line = buffer;
			capacity *= 2;
		}
		line[index++] = cur;
	}

	/* Handle EOF when no input is given */
	if (index == 0 && cur == EOF) {
		free(line);
		return NULL;
	}

	/* Null-terminate the string */
	line[index] = '\0';
	return line;
}

/**
 * Steps for splitting:
 *
 * 1. remove comments
 * 2. split into:
 *               label     - if ':' is found
 *               command   - first word after label (or first word if label not found)
 *               arguments - everything after the command
 */
Line *split_line(char *line) {
	Line *output;
	char *input_copy, *token, *cln_arg, **args, **new_args;
	char *delims = " ,\t\n";
	size_t i, max_args = MAX_ARGS;

	if (line == NULL) {
		return NULL;
	}
	output = malloc(sizeof(Line));
	if (output == NULL) {
		/* error failed mem allocation */
		return NULL;
	}
	input_copy = copy_string(line);
	if (input_copy == NULL) {
		/* Something failed gotta check what */
		return output;
	}
	args = malloc(sizeof(char *) * max_args);

	/* Initialize all fields */
	output->arg_count = 0;
	output->command = NULL;
	output->label = NULL;
	output->arguments = args;

	/* removing the comments from the line. */
	remove_after_delim(input_copy, ';');

	/* Extract label if found */
	token = strtok(input_copy, delims);

	if (token && strchr(token, ':')) {
		output->label = copy_string(token);

		token = strtok(NULL, delims);
	}

	/* Extract command if exists */
	if (token) {
		output->command = copy_string(token);

		token = strtok(NULL, delims);
	}

	for (i = 0; token; token = strtok(NULL, delims), i++) {
		if (i >= max_args) {
			new_args = realloc(args, sizeof(char *) * max_args * 2);
			if (!new_args) {
				/* Handle memory allocation failure */
				free(args);
                free(input_copy);
				return output;
			}
			args = new_args;
			output->arguments = args;
			max_args *= 2;
		}
		args[i] = token;
	}

	free(input_copy);
	return output;
}
