
#include "../h/pre_assembler.h"
#include "../h/file_funcs.h"
#include "../h/globals.h"
#include "../h/hashmap.h"
#include "../h/line.h"

void printerror(char *error) {
	printf("%s\n", error);
}

int pre_comp(char *src_path) {
	char *line, *new_path, *name;
	FILE *file;
	hashmap_t *map;
	Macro *macro;

	map = (hashmap_t *)malloc(sizeof(hashmap_t));
	if (map == NULL) {
		/* Memory failure */
		return FAIL_CODE;
	}

	new_path = copy_file(src_path, ".am");
	if (new_path == NULL) {
		free_hashmap(map);
		return FAIL_CODE;
	}

	file = fopen(new_path, "r");
	if (file == NULL) {
		free_hashmap(map);
		free(new_path);
		return FAIL_CODE;
	}

	init_hashmap(map, TABLE_SIZE);

	while ((line = read_line(file)) != NULL) {
		if (isEmpty(line) == TRUE) {
			continue;
		}
		printf("Read line: %s\n", line);

		if ((macro = parse_macro(line, new_path, file)) != NULL) {

			/* Checking if the macro name is allowed, if not, delete the file and stop the pre_assembler. */
			if (strcmp(macro->name, STOP_STRING) == 0) {
				fclose(file);
				remove(new_path);
				free(macro);
				free_hashmap(map);

				return SUCCESS_CODE;
			}
			delete_line(new_path, line);
			/* A macro definition was found */
			insert(map, (void *)macro, macro->name);
		} else if ((name = is_macro(line, map)) != NULL) {
			paste_macro(name, line, new_path, map);
			delete_line(new_path, line);
		}
	}

	fclose(file);
	free_hashmap(map);
	free(new_path);

	printf("\nPRECOMPILE SUCCESS\n");
	return SUCCESS_CODE;
}

Macro *parse_macro(char *input, char *filename, FILE *file) {
	Macro *output;
	Line *line;
	char *macro_body, *macro_name, *new_macro_body;
	int IS_MACRO = FALSE;
	size_t line_length, buffer_size = INITIAL_MACRO_SIZE, length = 0;

	/* Check for NULL input */
	if (input == NULL) {
		return NULL;
	}

	output = malloc(sizeof(Macro));
	if (output == NULL) {
		return NULL;
	}
	macro_body = malloc(buffer_size * sizeof(char));
	if (macro_body == NULL) {
		free(output);
		return NULL;
	}

	if ((macro_name = is_macro_start(input)) == NULL) {
		free(macro_body);
		free(output);
		return NULL;
	}

	if (is_opcode(macro_name) == TRUE) {
		printf("\nNOT ALLOWED MACRO NAME \"%s\"\n", macro_name);
		free(macro_body);

		output->name = STOP_STRING;
		return output;
	}

	output->name = macro_name; /* Saving the first argument as the macro name  */
	IS_MACRO = TRUE;

	while (IS_MACRO) {
		input = read_line(file);
		if (input == NULL)
			break;

		line = split_line(input);

		if (strcmp(line->command, MACRO_END_STRING) == 0) {
			/* IS_MACRO = FALSE; */
			delete_line(filename, MACRO_END_STRING);
			free_line(line);
			break;
		}

		line_length = strlen(input);

		/*  Ensure enough space in buffer */
		while (length + line_length + 2 > buffer_size) { /* +2 for '\n' and '\0' */
			buffer_size *= MACRO_GROWTH_FACTOR;
			new_macro_body = (char *)realloc(macro_body, buffer_size);
			if (new_macro_body == NULL) {
				free(macro_body);
				free(output);
				free_line(line);
				return NULL;
			}
			macro_body = new_macro_body;
		}

		/* Append new line */
		strcpy(macro_body + length, input);
		length += line_length;
		macro_body[length++] = '\n'; /* Preserve line breaks */
		macro_body[length] = '\0';	 /* Null terminate */
		delete_line(filename, input);
	}

	output->body = macro_body;

	return output;
}

int is_valid_macro_name(char *name) {
	/* Check if the first character is alphabetic or underscore */
	if (isalpha(name[0]) == FALSE && name[0] != '_') {
		return FALSE;
	}
	if (strlen(name) > MAX_MCRO_NAME_LENGTH) {
		return FALSE;
	}

	/* Check if the name is a reserved name */
	return is_op_name(name) && is_register(name) && is_instruction(name);
}

char *is_macro_start(char *input) {
	Line *line;
	char *name;

	/* Check for NULL input */
	if (input == NULL) {
		return NULL;
	}

	line = split_line(input);

	if (strcmp(line->command, MACRO_START_STRING) != STRCMP_SUCCESS) {
		/* Didn't find a macro definition */
		free_line(line);
		return NULL;
	}

	name = line->arguments[0];

	free_line(line);
	return name;
}

char *is_macro(char *input, hashmap_t *map) {
	Line *line;
	char *name;

	/* Check for NULL input */
	if (input == NULL) {
		return NULL;
	}

	line = split_line(input);

	if ((Macro *)lookup(map, line->command) == NULL) {
		/* Didn't find a macro */
		free_line(line);
		return NULL;
	}
	name = copy_string(line->command);

	free_line(line);
	return name;
}

void paste_macro(char *name, char *search_text, char *filename, hashmap_t *map) {
	char *body;
	body = ((Macro *)lookup(map, name))->body;

	if (insert_text_at_line(filename, search_text, body) != SUCCESS_CODE) {
		return;
	}
}

