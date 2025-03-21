
#include "../h/pre_assembler.h"
#include "../h/file_funcs.h"
#include "../h/globals.h"
#include "../h/line.h"
#include "../h/macro.h"

int pre_comp(char *src_path, hashmap_t *mcro_table) {
	char line[MAX_LINE_LENGTH + 2], *name;
	int error_flag = FALSE, read_line_err_flag = FALSE, is_error_string, line_count = 0;
	FILE *file_in, *file_out;
	Macro *mcro;
	Macro *lookup_result;
	char *new_file_name; 

	init_hashmap(mcro_table, TABLE_SIZE);

	/* Open the input and output files */
	file_in = open_file(src_path, ".as", READ_MODE);
	file_out = open_file(src_path, ".am", WRITE_MODE);

	if (!file_in || !file_out) {
		free_hashmap(mcro_table, (void (*)(void *))free_macro);
		return FAIL_CODE;
	}

	while ((read_line_err_flag = read_line(file_in, line)) != EXIT_FAILURE) {
		line_count++;
		/* printf("Read line: %s\n", line); */

		if (read_line_err_flag < EXIT_SUCCESS) {
			printerror("ERROR", line_count, read_line_err_flag);
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
		if (parse_macro(line, &line_count, file_in, mcro) == EXIT_SUCCESS) {
			is_error_string = (strcmp(mcro->name, ERROR_STRING) == STRCMP_SUCCESS);

			if (is_error_string) {
				error_flag = TRUE;
				free_macro(mcro);
			} else {
				if (lookup(mcro_table, mcro->name) != NULL) {
					error_flag = TRUE;
					printerror("ERROR\n", line_count, MACRO_ALREADY_EXISTS);
				}
				insert(mcro_table, (void *)mcro, mcro->name);
			}
		}
		/* Check for a macro usage*/
		else if ((name = find_macro(line, mcro_table))) {
			free_macro(mcro);

			lookup_result = (Macro *)lookup(mcro_table, name);
			fprintf(file_out, "%s", lookup_result->body);
			free(name);
		}
		/* Didn't find a macro, save the line in the output file */
		else {
			free_macro(mcro);
			fprintf(file_out, "%s\n", line);
			fflush(file_out);
		}
	}

	fclose(file_in);
	fclose(file_out);

	if (error_flag != FALSE) {
		printerror("Error Flag\n", line_count, error_flag);
		new_file_name = change_extension(src_path, ".am");

		remove(new_file_name); /* Removing the .am file */

		free(new_file_name);
		free_hashmap(mcro_table, (void (*)(void *))free_macro);

		printf("\n\npre-compilation failed\n\n");
		return EXIT_FAILURE;
	}

	printf("\n\nPRECOMPILE SUCCESS\n\n");
	return SUCCESS_CODE;
}

int parse_macro(char *input, int *line_count, FILE *file, Macro *mcro) {
	Line line;
	char *macro_body, *macro_name, *new_macro_body;
	int IS_MACRO = FALSE, buffer_size = INITIAL_MACRO_SIZE, line_length, total_length = 0;

	/* Initialize the line */
	init_line(&line);

	macro_body = malloc(buffer_size);
	if (macro_body == NULL) {
		return EXIT_FAILURE;
	}

	split_line(input, &line);
	
	if (!(macro_name = is_macro_start(input, &line))) {
		/* Not a macro start */
		free(macro_body);
		free_line(&line);
		return EXIT_FAILURE;
	}

	if (is_valid_macro_name(macro_name) == FALSE) {
		/* Invalid macro name */
		free(macro_body);
		free_line(&line);

		mcro->name = ERROR_STRING;
		return NOT_ALLOWED_MACRO_NAME;
	}

	mcro->name = copy_string(macro_name);
	IS_MACRO = TRUE;

	while (IS_MACRO) {
		/* For iterations after the first, free the memory allocated for the previous line.
		 * (On the first iteration, no memory has been allocated yet.) */
		if (*line_count > 0) free_line(&line);

		/* Reinitialize parsed_line to prepare for processing the next line. */
		init_line(&line);

		read_line(file, input);
		(*line_count)++;

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
				return EXIT_FAILURE;
			}
			macro_body = new_macro_body;
		}

		/* Append new line */
		strcpy(macro_body + total_length, input);
		total_length += line_length;
		macro_body[total_length++] = '\n'; /* Preserve line breaks */
		macro_body[total_length] = '\0';   /* Null terminate */
	}

	/* A check for an empty macro */
	if(total_length == 0) *macro_body = '\0';
	
	free_line(&line);

	mcro->body = macro_body;
	return EXIT_SUCCESS;
}
