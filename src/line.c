#include "../h/line.h"
#include "../h/globals.h"
#include "../h/string_funcs.h"

void init_line(Line *line) {
	if (line == NULL) {
		return;
	}

	line->input_copy = NULL;
	line->label = NULL;
	line->command = NULL;
}

int read_line(FILE *file, char *line) {
	char cur;
	int index = 0;
	int finished_line = FALSE;

	/* Read input character by character, stopping at EOF, '\n' */
	while ((cur = fgetc(file)) != EOF && cur != '\n') {
		if (cur == ';') {
			finished_line = TRUE;
		}
		if (index > MAX_LINE_LENGTH || (index == MAX_LINE_LENGTH && line[index - 1] != '\n')) {
			/* Line too long */
			line[index] = '\0';
			return LINE_TOO_LONG;
		}
		if (!finished_line) line[index++] = cur;
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
	char *input_copy, *token, **args, **args_buffer;
	int i, arg_count = MAX_ARGS;

	if (line == NULL) {
		return FAIL_CODE;
	}
	output->command = NULL;
	output->label = NULL;

	input_copy = malloc(sizeof(char) * (MAX_LINE_LENGTH + 2)); /* +2 for null terminator and newline */
	if (!input_copy) {
		return FAIL_CODE;
	}
	strcpy(input_copy, line);
	/* Allocate memory for the array on the heap */
	args = malloc(sizeof(char *) * (arg_count + 1)); /* +1 for NULL */
	if (!args) {
		free(input_copy);
		return FAIL_CODE;
	}

	/* Extract label if found */
	token = strtok(input_copy, " ");
	
	output->input_copy = input_copy;

	if (token && strchr(token, ':')) {

		output->label = token;

		/* removing the ':' from the label name. */
		remove_after_delim(output->label, ':');

		token = strtok(NULL, " ");
	}

	/* Extract command */
	if (token) {
		output->command = token;
		token = strtok(NULL, " ");
	}

	/* Puts the rest of the tokens as arguments */
	for (i = 0; token; token = strtok(NULL, ","), i++) {
		if (i >= arg_count) {
			arg_count *= 2;
			args_buffer = realloc(args, (arg_count + 1) * sizeof(*args)); /* +1 for null */
			if (!args_buffer) {
				return EXIT_FAILURE;
			}
			args = args_buffer;
		}
		/* args[i] = clean_arg(token); */
		args[i] = token;
	}

	/* NULL TERMINATE THE ARRAY */
	args[i] = NULL;

	output->arguments = args;

	return EXIT_SUCCESS;
}

void free_line(Line *line) {
	if (!line) return;

	if (line->label) line->label = NULL;
	if (line->command) line->command = NULL;

	if (line->arguments) {
		free(line->arguments);
		line->arguments = NULL;
	}

	if (line->input_copy) {
		free(line->input_copy);
		line->input_copy = NULL;
	}
}

int is_in_array(char *name, char **array) {
	int i, array_size;

	array_size = string_array_len(array);

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

char *clean_arg(char *arg) {
	char *output = NULL;
	int i, j;
	int len;

	if (arg == NULL || *arg == '\0') {
		return NULL; /* Return NULL if the input is null or an empty string */
	}

	len = strlen(arg);

	/* Allocating memory with the length of arg. */
	output = malloc(len + 1); /* +1 for the null terminator */
	if (!output) {
		printf("Failed memory allocation\n");
		return NULL; /* Memory allocation failed */
	}

	/* Iterate through the input string and copy only non-whitespace characters */
	for (i = 0, j = 0; i < len; i++) {
		if (!isspace(arg[i])) {
			output[j] = arg[i];
			j++;
		}
	}
	output[j] = '\0'; /* Null-terminate the output string */
	return output;
}

void remove_special_chars(char *str) {
	char *dst = str; /* Destination pointer */

	while (*str) {
		if (isalnum(*str) || isspace(*str) || *str == '_') {
			*dst++ = *str;
		}
		str++;
	}
	/* Null-terminate the modified string. */
	*dst = '\0';
}
