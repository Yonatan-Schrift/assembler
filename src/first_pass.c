#include "../h/first_pass.h"
#include "../h/file_funcs.h"
#include "../h/globals.h"
#include "../h/hashmap.h"
#include "../h/line.h"

#define IS_STORE_INST(a) (strcmp((a), ".string") == STRCMP_SUCCESS || strcmp((a), ".data") == STRCMP_SUCCESS)
#define IS_ENTRY_OR_EXTERN(a) (strcmp((a), ".extern") == STRCMP_SUCCESS || strcmp((a), ".entry") == STRCMP_SUCCESS)
#define COMPARE_STR(a, b) (strcmp(a, b) == STRCMP_SUCCESS)

int first_pass(char *src_path, hashmap_t *mcro_tb) {
	char line[MAX_LINE_LENGTH + 1], *new_path;
	FILE *file_in, *file_ob;
	Line parsed_line;
	int error_flag = FALSE, current_error = FALSE, is_symbol = FALSE;
	int line_count = 0, i, value;
	hashmap_t sym_table;

	int data_size = INITIAL_DATA_SIZE;
	int *data_image = malloc(INITIAL_DATA_SIZE * sizeof(int));
	if (!data_image)
		return EXIT_FAILURE;
	/*
		new_path = change_extension(src_path, ".ob");  might be useless
	 */
	file_in = open_file(src_path, ".am", READ_MODE);
	file_ob = open_file(src_path, ".ob", WRITE_MODE);
	if (!file_in || !file_ob) {
		close_mult_files(file_in, file_ob, NULL, NULL, NULL, NULL);
		free(data_image);

		return FAIL_CODE;
	}

	init_line(&parsed_line);
	init_hashmap(&sym_table, TABLE_SIZE);

	IC = 100;
	DC = 0;

	while ((current_error = read_line(file_in, line)) != EXIT_FAILURE) {
		line_count++;
		is_symbol = FALSE;

		/* Read a line from the source */
		printf("Read line is: %s\n", line); /* debug line */

		/* Check if the line encountered an error */
		if (COMPARE_STR(line, ERROR_STRING)) {
			printerror("error_flag", line_count, current_error);
			continue;
		}

		/* Skips the line if it's empty */
		if (isEmpty(line) == TRUE) {
			continue;
		}

		if (split_line(line, &parsed_line) != EXIT_FAILURE) {

			/* check for a label */
			if (parsed_line.label != NULL) {
				is_symbol = TRUE;
			}
			/* Stages 5-7 */
			/* check if the instruction stores data in the memory */
			if (IS_STORE_INST(parsed_line.command)) {
				if (is_symbol) {
					current_error = insert_symbol(parsed_line.label, DATA, DC, &sym_table, mcro_tb);
					printerror("IF_ERROR", line_count, current_error);
					if (current_error != FALSE)
						error_flag = TRUE;
				}

				if (COMPARE_STR(parsed_line.command, ".data")) {
					/* is '.data' instruction */
					if (parsed_line.arguments[0] == NULL) {
						/* Checks that the data does exist */
						printerror("MISSING PARAMETERS", line_count, MISSING_ARGS);
						error_flag = TRUE;
						continue;
					}
					for (i = 0; parsed_line.arguments != NULL; i++) {
						/* saves each integer to the data-image */
						value = atoi(parsed_line.arguments[i]);

						if (add_data_word(value, &data_size, &data_image) != EXIT_SUCCESS) {
							/* Memory failure */
							free(data_image);
							close_mult_files(file_in, file_ob, NULL, NULL, NULL, NULL);
							free_line(&parsed_line);
							free_hashmap(&sym_table);
							free_hashmap(mcro_tb);

							return EXIT_FAILURE;
						}
					}
				}

				else {
					/* is '.string' instruction */
					if (string_array_len((const char **)parsed_line.arguments) != 1) {
						printerror("TOO_MANY_ARGUMENTS", line_count, TOO_MANY_ARGS);
						error_flag = TRUE;
					}
					if (add_string_word(parsed_line.arguments[0], &data_size, &data_image) != EXIT_SUCCESS) {
						/* Memory failure */
						free(data_image);
						close_mult_files(file_in, file_ob, NULL, NULL, NULL, NULL);
						free_line(&parsed_line);
						free_hashmap(&sym_table);
						free_hashmap(mcro_tb);

						return EXIT_FAILURE;
					}
				}
			}

			/* Stages 8-10 */
			/* check if the instruction is an entry or extern variable */
			if (IS_ENTRY_OR_EXTERN(parsed_line.command)) {
				if (COMPARE_STR(parsed_line.command, ".extern")) {
					/* Is '.extern' instruction */
					current_error = insert_symbol(parsed_line.label, EXTERNAL, 0, &sym_table, mcro_tb);
					printerror("IF_ERROR", line_count, current_error);
					if (current_error != FALSE)
						error_flag = TRUE;
				}

				else {
					/* is '.entry' instruction */
					/* do nothing */
					continue;
				}
			}

			/* Stage 11 */
			if (is_symbol) {
				current_error = insert_symbol(parsed_line.label, CODE, IC, &sym_table, mcro_tb);
				printerror("IF_ERROR", line_count, current_error);
				if (current_error != FALSE)
					error_flag = TRUE;
			}
			
			/* Stage 12 */
			if (!is_op_name(parsed_line.command)) {
				printerror("OPERATION NOT FOUND", line_count, FAIL_CODE);
				error_flag = TRUE;
				continue;
			}
			
		}
	}

	/* FOR SURE NEEDS TO BE REDONE */
	if (error_flag == TRUE) {
		printf("ERRORS WERE FOUND DURING THE FIRST PASS!");
	}
	free(data_image);
	return EXIT_SUCCESS;
}

int insert_symbol(char *name, char *attribute, int value, hashmap_t *sym_tb, hashmap_t *mcro_tb) {
	Symbol *sym;

	if (!value || !attribute) {
		/* Missing values for the symbol */
		return MISSING_SYMBOL_VALUES;
	}
	if (lookup(sym_tb, name)) {
		/* Checks if a symbol is defined twice */
		return SYMBOL_ALREADY_EXISTS;
	}
	if (lookup(mcro_tb, name)) {
		/* Checks if a symbol is defined with the same name as a macro */
		return SYMBOL_IS_MACRO;
	}

	sym = malloc(sizeof(Symbol));
	if (!sym) {
		return EXIT_FAILURE;
	}

	sym->name = copy_string(name);
	sym->attribute = copy_string(attribute);
	sym->value = value;

	insert(sym_tb, (void *)sym, name);
	return SUCCESS_CODE;
}

int add_data_word(int value, int *data_cap, int **data_image) {
	int *img_buffer;

	if (DC >= *data_cap) {
		*data_cap *= 2;
		img_buffer = realloc(*data_image, *data_cap * sizeof(int));
		if (!img_buffer) {
			printerror("Memory failue", NO_LINE, 0);
			return EXIT_FAILURE;
		}
		*data_image = img_buffer;
	}
	*data_image[DC++] = value;
	return EXIT_SUCCESS;
}

int add_string_word(char *string, int *data_cap, int **data_image) {
	int i, value;
	remove_quotes(string);

	for (i = 0; string[i] != '\0'; i++) {
		value = (int)string[i];
		if (add_data_word(value, data_cap, data_image) == EXIT_FAILURE) {
			return EXIT_FAILURE;
		}
	}
	return EXIT_SUCCESS;
}
