#include "../h/first_pass.h"
#include "../h/file_funcs.h"
#include "../h/globals.h"
#include "../h/hashmap.h"
#include "../h/line.h"

#define IS_STORE_INST(a) (strcmp((a), ".string") == STRCMP_SUCCESS || strcmp((a), ".data") == STRCMP_SUCCESS)

int first_pass(char *src_path) {
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
		printf("Read line is: %s\n", line);

		if (strcmp(line, STOP_STRING) == STRCMP_SUCCESS) {

			printf("%d", error_flag); /* needs to be removed once printerror is done */
			
			printerror("error_flag", line_count);
			continue;
		}

		/* Skips the line if it's empty */
		if (isEmpty(line) == TRUE) {
			continue;
		}

		if (split_line(line, &parsed_line) != EXIT_FAILURE) {

			if (parsed_line.label != NULL) {
				/* check for a label */
				is_symbol = TRUE;
			}

			if (IS_STORE_INST(parsed_line.command)) {
				if (is_symbol){
                    insert_symbol(parsed_line.label, parsed_line.command, DC, &sym_table);
                }
			}
		}
	}
}

int insert_symbol(char *name, char *instruction, int value, hashmap_t *map) {
	Symbol *sym;
	
	if(lookup(map, name)) {
		return REDEFINE_SYMBOL;
	}

	sym = malloc(sizeof(Symbol));

	if (!sym || !name || !value || !instruction) {
		return EXIT_FAILURE;
	}
	
	sym->instruction = copy_string(instruction);
	sym->name = copy_string(name);
	sym->value = value;

	
	insert(map, (void *)sym, name);
	return SUCCESS_CODE;
}
