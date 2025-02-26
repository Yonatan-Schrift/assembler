
#include "../h/pre_assembler.h"
#include "../h/error.h"
#include "../h/file_funcs.h"
#include "../h/globals.h"
#include "../h/line.h"
#include "../h/macro.h"

int pre_comp(char *src_path) {
	char line[MAX_LINE_LENGTH + 2], *name;
	int error_flag = FALSE, is_stop_string;
	FILE *file_in, *file_out;
	Macro *mcro;
	Macro *lookup_result;
	hashmap_t mcro_table;

	char *filename = change_extension(src_path, ".am");

	init_hashmap(&mcro_table, TABLE_SIZE);

	/* Open the input and output files */
	file_in = open_file(src_path, ".as", READ_MODE);
	file_out = open_file(src_path, ".am", WRITE_MODE);

	printf(" >>> Starting to work on file %s\n\n", filename);

	while (read_line(file_in, line) != EXIT_FAILURE) {
		printf("Read line: %s\n", line);

		if (strcmp(line, STOP_STRING) == STRCMP_SUCCESS) {
			error_flag = EXIT_FAILURE;
			continue;
		}

		/* Skips the line if it's empty */
		if (isEmpty(line) == TRUE) {
			continue;
		}

		mcro = malloc(sizeof(Macro));
		if (!mcro) {
			return EXIT_FAILURE;
		}
		init_macro(mcro);

		/* Check for a macro definition */
		if (parse_macro(line, file_in, mcro) == EXIT_SUCCESS) {
			printf("\n >>> Macro is %s\n", mcro->name);
			is_stop_string = (strcmp(mcro->name, STOP_STRING) == STRCMP_SUCCESS);

			if (is_stop_string) {
				error_flag = EXIT_FAILURE;
				free_macro(mcro);
			} else {
				insert(&mcro_table, (void *)mcro, mcro->name);
			}
		}
		/* Check for a macro usage*/
		else if ((name = find_macro(line, &mcro_table))) {
			free_macro(mcro);

			lookup_result = (Macro *)lookup(&mcro_table, name);
			fprintf(file_out, "%s", lookup_result->body);
		}
		/* Didn't find a macro, save the line in the output file */
		else {
			free_macro(mcro);
			fprintf(file_out, "%s\n", line);
		}
	}

	free_hashmap(&mcro_table);
	if (error_flag != FALSE) {
		remove(filename);
	}

	free(filename);

	fclose(file_in);
	fclose(file_out);

	printf("\nPRECOMPILE SUCCESS\n");
	return error_flag;
}

int parse_macro(char *input, FILE *file, Macro *mcro) {
	Line line;
	char *macro_body, *macro_name, *new_macro_body;
	int IS_MACRO = FALSE, buffer_size = INITIAL_MACRO_SIZE, line_length, total_length = 0;

	/* Initialize the line */
	init_line(&line);

	macro_body = malloc(buffer_size);
	if (macro_body == NULL) {
		return EXIT_FAILURE;
	}

	if (!(macro_name = is_macro_start(input, &line))) {
		/* Not a macro start */
		free(macro_body);
		free_line(&line);
		return EXIT_FAILURE;
	}

	if (is_valid_macro_name(macro_name) == FALSE) {
		/* Invalid macro name */
		printerror("\nNOT ALLOWED MACRO NAME\n");
		free(macro_body);
		free_line(&line);

		mcro->name = STOP_STRING;
		return EXIT_SUCCESS;
	}

	mcro->name = copy_string(macro_name);
	IS_MACRO = TRUE;

	while (IS_MACRO) {
		read_line(file, input);
		if (input == NULL) {
			free(macro_body);
			free_line(&line);
			break;
		}
		split_line(input, &line);

		if (strcmp(line.command, MACRO_END_STRING) == STRCMP_SUCCESS) {
			IS_MACRO = FALSE;
			continue;
		}

		line_length = strlen(input);

		/* Ensure enough space in buffer */
		while ((total_length + line_length + 2) > buffer_size) { /* +2 for '\n' and '\0' */
			buffer_size *= MACRO_GROWTH_FACTOR;
			new_macro_body = (char *)realloc(macro_body, buffer_size);
			if (new_macro_body == NULL) {
				free(macro_body);
				free_line(&line);
				exit(EXIT_FAILURE);
			}
			macro_body = new_macro_body;
		}

		/* Append new line */
		strcpy(macro_body + total_length, input);
		total_length += line_length;
		macro_body[total_length++] = '\n'; /* Preserve line breaks */
		macro_body[total_length] = '\0';   /* Null terminate */
	}

	free_line(&line);

	mcro->body = macro_body;
	return EXIT_SUCCESS;
}
