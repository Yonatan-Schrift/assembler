#include "../h/line.h"
#include "../h/globals.h"
#include "../h/string_funcs.h"

void init_line(Line *line) {
	int i;

    if (line == NULL) {
        return;
    }

    line->label = NULL;
    line->command = NULL;
    
}

int read_line(FILE *file, char *line) {
	char cur;
	int index = 0;

	/* Read input character by character */
	while ((cur = fgetc(file)) != EOF && cur != '\n') {

		if (index > MAX_LINE_LENGTH || (index == MAX_LINE_LENGTH && line[index - 1] != '\n')) {
			/* Line too long */
			free(line);
			line = copy_string(ERROR_STRING);
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
	char input_copy[MAX_LINE_LENGTH + 2], *token, **args, **args_buffer;
	char *delims = " ,\t\n";
	int i, arg_count = MAX_ARGS;

	if (line == NULL) {
		return EXIT_FAILURE;
	}
	strcpy(input_copy, line);

	/* Allocate memory for the array on the heap */
    args = malloc(sizeof(char*) * (MAX_ARGS + 1));
    if (!args) {    
        return EXIT_FAILURE;
    }
	output->arguments = args;

	/* removing the comments from the line. */
	remove_after_delim(input_copy, ';');

	/* Extract label if found */
	token = strtok(input_copy, delims);

	if (token && strchr(token, ':')) {
		output->label = clean_arg(token);

		token = strtok(NULL, delims);
	}

	/* Extract command */
	if (token) {
		output->command = clean_arg(token);
		token = strtok(NULL, delims);
	}

	/* Puts the rest of the tokens as arguments */
	for (i = 0; token; token = strtok(NULL, delims), i++) {
		if (i >= arg_count) {
			arg_count *= 2;
			args_buffer = realloc(args, arg_count);
			if(!args_buffer) {
				return EXIT_FAILURE;
			}
			args = args_buffer;
		}
		args[i] = clean_arg(token);
	}
	output->arguments = args;

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
			return i;
		}
	}
	return FALSE;
}

int isEmpty(char *line) {
	char *p = line;

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

char *clean_arg( char *arg)
{
    char *output = NULL;
    int i , j;
    int len;

    if (arg == NULL || *arg == '\0')
    {
        return NULL; /* Return NULL if the input is null or an empty string */
    }

    len = strlen(arg);
    
    /* Allocating memory with the length of arg. */
    output = malloc(len + 1); /* +1 for the null terminator */
    if (!output)
    {
        printf("Failed memory allocation\n");
        return NULL; /* Memory allocation failed */
    }

    /* Iterate through the input string and copy only non-whitespace characters */
    for (i = 0, j = 0; i < len; i++)
    {
        if (!isspace(arg[i]))
        {
            output[j] = arg[i];
            j++;
        }
    }
    output[j] = '\0'; /* Null-terminate the output string */
    return output;
}
