
#include "../h/pre_assembler.h"
#include "../h/globals.h"
#include "../h/hashmap.h"
#include "../h/line.h"
#include "../h/file_funcs.h"

char *is_macro_start(char *line);
char *is_macro(char *input, hashmap_t *map);

/**
 * Missing some stuff:
 * @Segev2101 you need to add do_stuff (change the name) and is_macro inside pre_assembler.
 * one checks for if a macro is found (it's ez trust) and do_stuff copies the code from the macro into the text.
 * 
 * also missing editing the text file itself removing anything that resambles the macro.
 *  
*/

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

	while ((line = read_line(file)) >= 0) {    
		printf("Read line: %s\n", line);

        if((macro = parse_macro(line, new_path, file)) != NULL) {
			printf("Deleting line\n");
			delete_line(new_path, line);
			/* A macro definition was found */
            insert(map, macro);
        }
        else if((name = is_macro(line, map)) != NULL) {
			delete_line(new_path, line);
            paste_macro(name, line, new_path, map); 
        }
	}

	return SUCCESS_CODE;
}

Macro *parse_macro(char *input, char *filename, FILE *file) {
	Macro *output;
	Line *line;
	char *macro_body, *macro_name;
	int IS_MACRO = FALSE;
	size_t buffer_size = INITIAL_MACRO_SIZE;
	size_t length = 0;

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

	if((macro_name = is_macro_start(input)) == NULL) {
		free(macro_body);
		free(output);
		return NULL;
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
			free_line(line);
			break;
		}

		size_t line_length = strlen(input);

		/*  Ensure enough space in buffer */
		while (length + line_length + 2 > buffer_size) { /* +2 for '\n' and '\0' */
			buffer_size *= MACRO_GROWTH_FACTOR;
			char *new_macro_body = (char *)realloc(macro_body, buffer_size);
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

/* need to add the list tbh */
int check_macro_name(char *name) {
    if(strcmp(name, "LIST OF NOT ALLOWED NAMES")) {
        return FALSE;
    }
    return TRUE;
}

char *is_macro_start(char *input) {
	Line *line;
	char *name;

	/* Check for NULL input */
	if (input == NULL) {
		return NULL;
	}

	line = split_line(input);

	if (strcmp(line->command, MACRO_START_STRING) != 0) {
		/* Didn't find a macro definition */
		free_line(line);
		return NULL;
	}
	
	name = line->arguments[0];
	if(check_macro_name(name) == FALSE) {
		free_line(line);

		printerror("NOT ALLOWED MACRO NAME"); 	/* need to make this */

		return NULL;
	}

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

	if (lookup(map ,line->command) == NULL) {
		/* Didn't find a macro */
		free_line(line);
		return NULL;
	}
	name = line->command;

	free_line(line);
	return name;
}

void paste_macro(char *name, char *search_text, char *filename, hashmap_t *map) {
	char *body;

	body = lookup(map, name)->body;

	if(insert_text_at_line(filename, search_text, body) != SUCCESS_CODE) {
		return;
	}
}