#include "../h/macro.h"

void init_macro(Macro *mcro) {
	mcro->body = NULL;
	mcro->name = NULL;
}

int is_reserved_name(char *name) {
	if (!name || !(*name)) /* Check if name is not NULL or empty */
		return MCRO_DEFINITION_WITH_NO_NAME;

	/* Check if the name is a reserved name */
	if (is_op_name(name) || is_register(name) || is_instruction(name) || strcmp(name, "mcro") == STRCMP_SUCCESS || strcmp(name, "mcroend") == STRCMP_SUCCESS)
		return RESERVED_NAME;
	
	if (isalpha(*name) == FALSE && *name != '_') /* Check if the first character is alphabetic or underscore */
		return NAME_DOESNT_START_WITH_LETTER;

	if (strlen(name) > MAX_MCRO_NAME_LENGTH) /* Check if the name is in the allowed size */
		return NAME_TOO_LONG;

	while (*name) {
		if (!isalnum(*name) && *name != '_')
			return NAME_USING_UNALLOWED_LETTERS; /* Check if the name contains only the allowed characters */
		name++;
	}

	
	return FALSE;
}

char *is_macro_start(char *input, Line *line) {
	char *name;

	/* Check for NULL input */
	if (input == NULL || isEmpty(input))
		return NULL;

	if (!line->command) return NULL;

	if (strcmp(line->command, MACRO_START_STRING) != STRCMP_SUCCESS) {
		/* Didn't find a macro definition */
		return NULL;
	}
	/* sets the name as the first argument */
	name = line->arguments[0];

	return name;
}

char *find_macro(char *input, hashmap_t *map) {
	/* This method searchs for the macro inside the hashmap */
	Line line;
	char *name = NULL;

	/* Check for NULL input */
	if (input == NULL) {
		return NULL;
	}

	init_line(&line);
	split_line(input, &line);

	if (lookup(map, line.command) == NULL) {
		/* Didn't find a macro */
		free_line(&line);
		return NULL;
	}
	if (line.arguments[0] == NULL)
		name = copy_string(line.command);

	free_line(&line);
	return name;
}

void free_macro(Macro *mcro) {
    if (!mcro) return;  

    if (mcro->name) {
        free(mcro->name);
        mcro->name = NULL;
    }	

    if (mcro->body) {
        free(mcro->body);
        mcro->body = NULL;
    }

    free(mcro);
}

