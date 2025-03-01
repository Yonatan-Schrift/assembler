/** yes, it's perfectly normal to create a whole diferent file for reading one line.  */

#include "../h/line.h"
#include "../h/argument_funcs.h"
#include "../h/globals.h"
#include "../h/string_funcs.h"
#include <string.h>

void init_line(Line *line) {
	int i;

    if (line == NULL) {
        return;
    }

    line->label = NULL;
    line->command = NULL;
    
    /* Allocate memory for the array on the heap */
    line->arguments = malloc(sizeof(char*) * (MAX_ARGS + 1));
    
    /* Initialize all elements to NULL */
    if (line->arguments) {    
        for (i = 0; i < MAX_ARGS + 1; i++) {
            line->arguments[i] = NULL;
        }
    }
}

int read_line(FILE *file, char *line) {
	char cur;
	int index = 0;

	/* Read input character by character */
	while ((cur = fgetc(file)) != EOF && cur != '\n') {

		if (index > MAX_LINE_LENGTH || (index == MAX_LINE_LENGTH && line[index - 1] != '\n')) {
			/* Line too long */
			free(line);
			line = copy_string(STOP_STRING);
			line[MAX_LINE_LENGTH] = '\0';
			
			return LINE_TOO_LONG;
		}
		line[index++] = cur;
	}

	/* Handle EOF or when when no input is given */
	if (index == 0 && cur == EOF) {
		return EXIT_FAILURE;
	}

	/* Null-terminate the string */
	line[index] = '\0';
	return EXIT_SUCCESS;
}

int split_line(char *line, Line *output) {
	char input_copy[MAX_LINE_LENGTH + 2], *token, **args;
	char *delims = " ,\t\n";
	int i;

	if (line == NULL) {
		return EXIT_FAILURE;
	}
	strcpy(input_copy, line);

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
		if (i >= MAX_ARGS) {
			return TOO_MANY_ARGS;
		}
		output->arguments[i] = copy_string(token);
	}

	return EXIT_SUCCESS;
}

void free_line(Line *line) {
	int i;

	if (!line)
		return;

	free(line->command);
	free(line->label);

	if (line->arguments) {
		for (i = 0; line->arguments[i] != NULL; i++) {
			free(line->arguments[i]);
		}
		free(line->arguments);
	}
}

int is_in_array(char *name, char **array) {
	int i, array_size;

	array_size = ARRAY_SIZE(array);

	for (i = 0; i < array_size; i++) {
		if (strcmp(name, array[i]) == STRCMP_SUCCESS) {
			return TRUE;
		}
	}
	return FALSE;
}

int isEmpty(char *line) {
	const char *p = line;

	/* Treat a NULL pointer as empty */
	if (line == NULL) {
		return TRUE;
	}

	while (*p != '\0') {
		if (!isspace((unsigned char)*p)) {
			return FALSE;
		}
		p++;
	}
	return TRUE; /* Line is empty */
}

int is_op_name(char *name) {
	char *array[] = OP_NAMES;

	if (name == NULL) {
		return FALSE;
	}

	return is_in_array(name, array);
}

int is_register(char *name) {
	char *array[] = REGISTERS;

	if (name == NULL) {
		return FALSE;
	}

	return is_in_array(name, array);
}

int is_instruction(char *name) {
	char *array[] = INSTRUCTIONS;

	if (name == NULL) {
		return FALSE;
	}

	return is_in_array(name, array);
}
