/** yes, it's perfectly normal to create a whole diferent file for reading one line.  */

#include "../h/line.h"
#include "../h/argument_funcs.h"
#include "../h/globals.h"
#include "../h/string_funcs.h"

char *read_line(FILE *file) {
	char cur;
	char *buffer, *line;
	int index = 0;

	/* Allocating initial memory */
	line = (char *)malloc(MAX_LINE_LENGTH + 2);
	if (!line) {
		fprintf(stderr, "Failed memory allocation\n");
		return NULL; /* Memory Allocation Error */
	}

	/* Read input character by character */
	while ((cur = fgetc(file)) != EOF && cur != '\n') {
		
		if (index > MAX_LINE_LENGTH || (index == MAX_LINE_LENGTH && line[index - 1] != '\n')) {
			/* Line too long */
			printerror("LINE_TOO_LONG\n");
			free(line);
			return NULL;
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

Line *split_line(char *line) {
	Line *output;
	char *input_copy, *token, **args, **new_args;
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
			new_args = realloc(args, sizeof(char *) * (max_args * 2));
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

void free_line(Line *line) {
	/* int i; */
	
	if (!line) return;

	free(line->command);
	free(line->label);

	if (line->arguments) {
        /* for (i = 0; line->arguments[i] != NULL; i++) {
            free(line->arguments[i]);
        } */
        free(line->arguments);
    }

	free(line);
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

    /* Treat a NULL pointer as empty if desired */
    if (line == NULL) {
        return TRUE;
    }

    while (*p != '\0') {
        /* Cast to unsigned char to avoid undefined behavior with isspace */
        if (!isspace((unsigned char)*p)) {
            return FALSE;  
        }
        p++;
    }
    return TRUE;  /* Line is empty */
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

int is_instuction(char *name) {
	char *array[] = INSTRUCTIONS;

	if (name == NULL) {
		return FALSE;
	}

	return is_in_array(name, array);
}
