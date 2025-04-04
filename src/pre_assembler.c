
#include "../h/pre_assembler.h"
#include "../h/file_funcs.h"
#include "../h/globals.h"
#include "../h/line.h"
#include "../h/macro.h"

int pre_comp(char *src_path, hashmap_t *mcro_table) {
	char line[MAX_LINE_LENGTH + 2], *name;
	int error_flag = FALSE, current_error = FALSE, line_count = 0;
	FILE *file_in, *file_out;
	Macro *mcro, *lookup_result;

	init_hashmap(mcro_table, TABLE_SIZE);

	/* Open the input and output files */
	file_in = open_file(src_path, ".as", READ_MODE);
	if(file_in) file_out = open_file(src_path, ".am", WRITE_MODE);
	
	if (!file_in || !file_out) {
		delete_mult_files(src_path, ".as", ".am", NULL);
		free_hashmap(mcro_table, (void (*)(void *))free_macro);
		return EXIT_FAILURE;
	}

	while ((current_error = read_line(file_in, line)) != EXIT_FAILURE) {
		line_count++;

		if (current_error < EXIT_SUCCESS) {
			printerror(line_count, current_error);
			error_flag = TRUE;
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
		if ((current_error = parse_macro(line, &line_count, file_in, mcro)) == EXIT_SUCCESS) {
			if (lookup(mcro_table, mcro->name) != NULL) {
				error_flag = TRUE;
				printerror(line_count, MACRO_ALREADY_EXISTS);
			}
			insert(mcro_table, (void *)mcro, mcro->name);

		}
		/* Check if an error was found during parsing */
		else if (current_error < EXIT_SUCCESS) {
			free_macro(mcro);
			error_flag = TRUE;
			printerror(line_count, current_error);
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
		/* Deleting the .am file */
		delete_mult_files(src_path, ".am", NULL, NULL);

		free_hashmap(mcro_table, (void (*)(void *))free_macro);

		return EXIT_FAILURE;
	}

	printf("\n>>> Finished pre-compiling successfully\n");
	return SUCCESS_CODE;
}

int parse_macro(char *input, int *line_count, FILE *file, Macro *mcro) {
	Line line;
	char *macro_body, *macro_name, *new_macro_body;
	int IS_MACRO = FALSE, buffer_size = INITIAL_MACRO_SIZE, line_length, total_length = 0, current_error;

	/* Initialize the line */
	init_line(&line);

	macro_body = malloc(buffer_size);
	if (macro_body == NULL) {
		return FAIL_CODE;
	}

	if ((current_error = split_line(input, &line)) < SUCCESS_CODE) {
		free(macro_body);
		return current_error;
	}

	if (!(macro_name = is_macro_start(input, &line))) {
		/* Not a macro start */
		free(macro_body);
		free_line(&line);
		return NOT_A_MACRO;
	}

	if ((current_error = is_reserved_name(macro_name)) != FALSE) {
		/* Invalid macro name */
		free(macro_body);
		free_line(&line);

		mcro->name = NULL;
		return current_error;
	}

	/* Checks that the macro doesn't have a label */
	if (line.label != NULL) {
		free(macro_body);
		free_line(&line);
		mcro->name = NULL;
		return MCRO_CANNOT_BE_A_LABEL;
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

		if (!line.command) {
			/* Empty line */
			continue;
		}

		if (strcmp(line.command, MACRO_END_STRING) == STRCMP_SUCCESS) {
			/* Check if mcroend has any text afterwards */
			if (line.arguments[0] != NULL) {
				free(macro_body);
				free_line(&line);
				mcro->body = NULL;
				return EXTRA_TEXT_AFTER_MCRO;
			}
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
				return FAIL_CODE;
			}
			macro_body = new_macro_body;
		}

		/* Append the current input line to the macro body */
		strcpy(macro_body + total_length, input);
		total_length += line_length;
		macro_body[total_length++] = '\n'; /* Preserve line breaks */
		macro_body[total_length] = '\0';   /* Null terminate */
	}

	/* If no lines were added, ensure macro_body is an empty string */
	if (total_length == 0) *macro_body = '\0';

	free_line(&line);

	mcro->body = macro_body;
	return EXIT_SUCCESS; /* No error */
}
