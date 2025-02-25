/**
 * File: macro.c
 * Description: Module for macro processing in the Assembler project.
 */
#include "../h/macro.h"

int is_valid_macro_name(char *name) {
	if (!name || !(*name)) /* Check if name is not NULL or empty */
		return FALSE;

	if (isalpha(*name) == FALSE && *name != '_') /* Check if the first character is alphabetic or underscore */
		return FALSE;

	if (strlen(name) > MAX_MCRO_NAME_LENGTH) /* Check if the name is in the allowed size */
		return FALSE;

	while (*name) {
		if (!isalnum(*name) && *name != '_')
			return FALSE; /* Check if the name contains only the allowed characters */
		name++;
	}

	/* Check if the name is a reserved name */
	return is_op_name(name) && is_register(name) && is_instruction(name);
}

char *is_macro_start(char *input) {
	Line *line;
	char *name;

	/* Check for NULL input */
	if (input == NULL)
		return NULL;

	line = split_line(input);

	if (strcmp(line->command, MACRO_START_STRING) != STRCMP_SUCCESS) {
		/* Didn't find a macro definition */
		free_line(line);
		return NULL;
	}
	/* sets the name as the first argument */
	name = line->arguments[0];

	free_line(line);
	return name;
}

char *find_macro(char *input, hashmap_t *map) {
	/* This method searchs for the macro inside the hashmap */
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
