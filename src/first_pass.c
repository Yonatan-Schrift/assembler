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
	int is_symbol = FALSE, error_flag = FALSE, line_count = 0;
	hashmap_t sym_table;

	new_path = change_extension(src_path, ".ob"); /* might be useless */

	file_in = open_file(src_path, ".am", READ_MODE);
	file_ob = open_file(src_path, ".ob", WRITE_MODE);
	if (!file_in || !file_ob) {
		close_mult_files(file_in, file_ob, NULL, NULL, NULL, NULL);
		return FAIL_CODE;
	}

	init_line(&parsed_line);
	init_hashmap(&sym_table, TABLE_SIZE);

	IC = 100;
	DC = 0;

	while ((error_flag = read_line(file_in, line)) != EXIT_FAILURE) {
		line_count++;
		/* Read a line from the source */
		printf("Read line is: %s\n", line); /* debug line */

		/* Check if the line encountered an error */
		if (COMPARE_STR(line, ERROR_STRING)) {
			printerror("error_flag", line_count, error_flag);
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
					error_flag = insert_symbol(parsed_line.label, parsed_line.command, DATA, DC, &sym_table, mcro_tb);
					printerror("IF_ERROR", line_count, error_flag);
				}

				if (COMPARE_STR(parsed_line.command, ".data")) {
					/* is '.data' instruction */
					DC += sizeof(".data");
				}

				else {
					/* is '.string' instruction */
					DC += sizeof(".string");
				}
			}
			
			/* Stages 8-10 */
			/* check if the instruction is an entry or extern variable */
			if (IS_ENTRY_OR_EXTERN(parsed_line.command)) {
				if (COMPARE_STR(parsed_line.command, ".extern")) {
					/* Is '.extern' instruction */
					error_flag = insert_symbol(parsed_line.label, parsed_line.command, EXTERNAL, 0, &sym_table, mcro_tb);
					printerror("IF_ERROR", line_count, error_flag);
				}

				else {
					/* is '.entry' instruction */
					/* do nothing */
					continue;
				}
			}
			
			/* Stages 11-? */

		}
	}
	return EXIT_SUCCESS;
}

int insert_symbol(char *name, char *instruction, char *attribute, int value, hashmap_t *sym_tb, hashmap_t *mcro_tb) {
	Symbol *sym;

	if(!name || !value || !instruction || !attribute) {
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

	sym->instruction = copy_string(instruction);
	sym->name = copy_string(name);
	sym->attribute = copy_string(attribute);
	sym->value = value;

	insert(sym_tb, (void *)sym, name);
	return SUCCESS_CODE;
}
