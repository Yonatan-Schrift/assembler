
#include "../h/pre_assembler.h"
#include "../h/globals.h"
#include "../h/hashmap.h"
#include "../h/line.h"

int is_macro(char *);
void do_stuff();

/**
 * Missing some stuff:
 * @Segev2101 you need to add do_stuff (change the name) and is_macro inside pre_assembler.
 * one checks for if a macro is found (it's ez trust) and do_stuff copies the code from the macro into the text.
 * 
 * also missing editing the text file itself removing anything that resambles the macro.
 *  
*/

int pre_comp(FILE *file) {
	char *line;
	hashmap_t *map;
    Macro *macro;

	map = (hashmap_t *)malloc(sizeof(hashmap_t));

	if (map == NULL) {
		/* Memory failure */
		return FAIL_CODE;
	}

	init_hashmap(map, TABLE_SIZE);

	while ((line = read_line(file)) >= 0) {
        
		printf("Read line: %s\n", line);
        if((macro = parse_macro(line, file))) {
            insert(map, macro);
        }
        if(is_macro(line)) {
            do_stuff(); 
        }

	}

	return SUCCESS_CODE;
}

Macro *parse_macro(char *input, FILE *file) {
	Macro *output;
	Line *line;
	char *macro_body;
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

	line = split_line(input);

	if (strcmp(line->command, MACRO_START_STRING) != 0) {
		/* Didn't find a macro */
        free(macro_body);
		free(output);
		return NULL;
	}
    if(check_macro_name(line->arguments[0]) == FALSE) {
        free(macro_body);
        free(output);
        return NULL;
    }
	output->name = line->arguments[0]; /* Saving the first argument as the macro name  */
	IS_MACRO = TRUE;

	while (IS_MACRO) {
		input = read_line(file);
		if (input == NULL)
			break;

		line = split_line(input);

		if (strcmp(line->command, MACRO_END_STRING) == 0) {
			/* IS_MACRO = FALSE; */
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
				return NULL;
			}
			macro_body = new_macro_body;
		}

		/* Append new line */
		strcpy(macro_body + length, input);
		length += line_length;
		macro_body[length++] = '\n'; /* Preserve line breaks */
		macro_body[length] = '\0';	 /* Null terminate */
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