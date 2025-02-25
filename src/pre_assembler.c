
#include "../h/pre_assembler.h"
#include "../h/error.h"
#include "../h/file_funcs.h"
#include "../h/globals.h"
#include "../h/line.h"
#include "../h/macro.h"
#include <ctype.h>
#include <stdio.h>

int pre_comp(char *src_path) {
	char *line, *name;
	int line_counter, error_flag = FALSE;
	FILE *file_in, *file_out;
	Macro *mcro;
	hashmap_t mcro_table;

	init_hashmap(&mcro_table, TABLE_SIZE);

	file_in = open_file(src_path, ".am" READ_MODE);
	file_out = open_file(src_path, ".as", WRITE_MODE);

	printf(" >>> Starting to work on file %s.as\n", src_path);

	while ((line = read_line(file_in)) != NULL) {
		line_counter++;

		if (isEmpty(line) == TRUE) {
			continue;
		}

		printf("Read line: %s\n", line);

		if ((mcro = parse_macro(line, new_path, file)) != NULL) {

			/* Checking if the macro name is allowed, if not, delete the file and stop the pre_assembler. */
			if (strcmp(mcro->name, STOP_STRING) == STRCMP_SUCCESS) {
				fclose(file);
				remove(new_path);
				free(mcro);
				free_hashmap(&mcro_table);

				return SUCCESS_CODE;
			}
			delete_line(new_path, line);
			/* A macro definition was found */
			insert(&mcro_table, (void *)mcro, mcro->name);
		} else if ((name = find_macro(line, &mcro_table)) != NULL) {
			paste_macro(name, line, new_path, &mcro_table);
			delete_line(new_path, line);
		}
	}

	free_hashmap(&mcro_table);

	printerror("\nPRECOMPILE SUCCESS\n");
	return error_flag;
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

	if (is_valid_macro_name(macro_name) == FALSE) {
		free(macro_body);

		output->name = STOP_STRING;
		return output;
	}

	while (IS_MACRO) {
		input = read_line(file);
		if (input == NULL)
			break;

		line = split_line(input);

		if (strcmp(line->command, MACRO_END_STRING) == STRCMP_SUCCESS) {
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
