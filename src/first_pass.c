#include "../h/first_pass.h"
#include "../h/file_funcs.h"
#include "../h/globals.h"
#include "../h/hashmap.h"
#include "../h/line.h"
#include "../h/pre_assembler.h"

#define IS_STORE_INST(a) (strcmp((a), ".string") == STRCMP_SUCCESS || strcmp((a), ".data") == STRCMP_SUCCESS)
#define IS_ENTRY_OR_EXTERN(a) (strcmp((a), ".extern") == STRCMP_SUCCESS || strcmp((a), ".entry") == STRCMP_SUCCESS)
#define COMPARE_STR(a, b) (strcmp(a, b) == STRCMP_SUCCESS)

const op_code OPCODES[] = {
	/* {"command", "opcode", "funct", "req_args", "is_source", "is_dest"} */
	{"mov", 0, 0, 2, TRUE, TRUE},
	{"cmp", 1, 0, 2, TRUE, TRUE},
	{"add", 2, 1, 2, TRUE, TRUE},
	{"sub", 2, 2, 2, TRUE, TRUE},
	{"lea", 4, 0, 2, TRUE, TRUE},

	{"clr", 5, 1, 1, FALSE, TRUE},
	{"not", 5, 2, 1, FALSE, TRUE},
	{"inc", 5, 3, 1, FALSE, TRUE},
	{"dec", 5, 4, 1, FALSE, TRUE},
	{"jmp", 9, 1, 1, FALSE, TRUE},
	{"bne", 9, 2, 1, FALSE, TRUE},
	{"jsr", 9, 3, 1, FALSE, TRUE},
	{"red", 12, 0, 1, FALSE, TRUE},
	{"prn", 13, 0, 1, FALSE, TRUE},

	{"rts", 14, 0, 0, FALSE, FALSE},
	{"stop", 15, 0, 0, FALSE, FALSE}};

int IC;
int DC;

int first_pass(char *src_path, hashmap_t *mcro_tb) {
	char line[MAX_LINE_LENGTH + 1];
	FILE *file_in, *file_ob;
	Line parsed_line;
	int error_flag = FALSE, current_error = FALSE, is_symbol = FALSE;
	int line_count = 0, i, value, opcode_index, L;
	hashmap_t sym_table;
	int data_size, *data_image, machine_code_size;
	FirstInstruction **machine_code;
	int ICF;
	/* int DCF */

	machine_code_size = INITIAL_ARRAY_SIZE;
	data_size = INITIAL_ARRAY_SIZE;

	machine_code = calloc(machine_code_size, sizeof(FirstInstruction *));
	if (!machine_code) return EXIT_FAILURE;

	data_image = malloc(data_size * sizeof(int));
	if (!data_image) {
		free(machine_code);
		return EXIT_FAILURE;
	}

	file_in = open_file(src_path, ".am", READ_MODE);
	file_ob = open_file(src_path, ".ob", WRITE_MODE);
	if (!file_in || !file_ob) {
		close_mult_files(file_in, file_ob, NULL, NULL, NULL, NULL);
		free(data_image);
		free(machine_code);

		return FAIL_CODE;
	}
	init_line(&parsed_line);
	init_hashmap(&sym_table, TABLE_SIZE);

	IC = 100;
	DC = 0;

	while ((current_error = read_line(file_in, line)) != EXIT_FAILURE) {
		/* For iterations after the first, free the memory allocated for the previous line.
		 * (On the first iteration, no memory has been allocated yet.) */
		if (line_count > 0) free_line(&parsed_line);
		/* Reinitialize parsed_line to prepare for processing the next line. */
		init_line(&parsed_line);

		line_count++;
		is_symbol = FALSE;

		/* Check if the line encountered an error */
		if (current_error < SUCCESS_CODE) {
			printerror("error_flag", line_count, current_error);
			continue;
		}

		/* Read a line from the source */
		printf("Read line is: %d:%s\n", IC + DC, line); /* debug line */

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

					if (current_error != SUCCESS_CODE) {
						printerror("IF_ERROR", line_count, current_error);
						error_flag = TRUE;
					}
				}

				if (COMPARE_STR(parsed_line.command, ".data")) {
					/* is '.data' instruction */
					if (parsed_line.arguments[0] == NULL) {
						/* Checks that the data does exist */
						printerror("MISSING PARAMETERS", line_count, MISSING_ARGS);
						error_flag = TRUE;
						continue;
					}
					for (i = 0; parsed_line.arguments[i] != NULL; i++) {
						/* saves each integer to the data-image */
						value = atoi(parsed_line.arguments[i]);

						if (add_data_word(value, &data_size, &data_image) != EXIT_SUCCESS) {
							/* Memory failure */
							free_everything(data_image, machine_code, machine_code_size, &sym_table, mcro_tb, &parsed_line);
							close_mult_files(file_in, file_ob, NULL, NULL, NULL, NULL);

							return EXIT_FAILURE;
						}
					}
				}

				else {
					/* is '.string' instruction */
					if (string_array_len(parsed_line.arguments) != 1) {
						printerror("TOO_MANY_ARGUMENTS", line_count, TOO_MANY_ARGS);
						error_flag = TRUE;
					}
					if (add_string_word(parsed_line.arguments[0], &data_size, &data_image) != EXIT_SUCCESS) {

						/* Memory failure */
						close_mult_files(file_in, file_ob, NULL, NULL, NULL, NULL);
						free_everything(data_image, machine_code, machine_code_size, &sym_table, mcro_tb, &parsed_line);

						return EXIT_FAILURE;
					}
				}
			}

			/* Stages 8-10 */
			/* check if the instruction is an entry or extern variable */
			else if (IS_ENTRY_OR_EXTERN(parsed_line.command)) {
				if (COMPARE_STR(parsed_line.command, ".extern")) {
					/* Is '.extern' instruction */
					current_error = insert_symbol(parsed_line.label, EXTERNAL, 0, &sym_table, mcro_tb);

					if (current_error != SUCCESS_CODE) {
						printerror("IF_ERROR", line_count, current_error);
						error_flag = TRUE;
					}
				}

				else {
					/* is '.entry' instruction */
					/* do nothing */
					continue;
				}
			}

			/* Stage 11 */
			/* Is an instructive statement */
			else {
				if (is_symbol) {
					current_error = insert_symbol(parsed_line.label, CODE, IC, &sym_table, mcro_tb);
					if (current_error != SUCCESS_CODE) {
						printerror("SYMBOL ERROR\n", line_count, current_error);
						error_flag = TRUE;
					}
				}

				/* Stage 12 */
				/* find the opcode */
				opcode_index = find_in_opcode(parsed_line.command);
				/* Saving the opcode and checking if it failed. */
				if (opcode_index < SUCCESS_CODE) {
					printerror("OPERATION NOT FOUND", line_count, opcode_index);
					error_flag = TRUE;
					continue;
				}

				/* checking the amount of arguments */
				current_error = check_arg_count(parsed_line.arguments, opcode_index);
				if (current_error != SUCCESS_CODE) {
					printerror("Argument Error found\n", line_count, current_error);
					error_flag = TRUE;
					continue;
				}

				/* Stage 13 */
				/* find L - the number of info-words required */
				L = count_info_words_required(parsed_line.arguments, &sym_table);
				if (L < SUCCESS_CODE) {
					printerror("Info word error found: \n", line_count, L);
					error_flag = TRUE;
					continue;
				}

				/* Stage 15 */

				current_error = add_instruction(&parsed_line, &machine_code, &sym_table, L, line_count, &machine_code_size);
				if (current_error != SUCCESS_CODE) {
					printerror("Error doing something cool\n", line_count, current_error);
					error_flag = TRUE;
					continue;
				}

				/* Stage 16 */
				IC += L;
			}
		}
	}

	/* Stage 17 */
	close_mult_files(file_in, file_ob, NULL, NULL, NULL, NULL);

	if (error_flag == TRUE) {
		printf("ERRORS WERE FOUND DURING THE FIRST PASS!\n\n");

		free_everything(data_image, machine_code, machine_code_size, &sym_table, mcro_tb, &parsed_line);

		return FAIL_CODE;
	}

	/* Stage 18 */
	ICF = IC;
	/* DCF = DC;  DCF unused rn*/

	/* Stage 19 */
	/* Update the symbol table with ICF for DATA symbols */
	set_data_to_icf(&sym_table, ICF);

	/* start second pass */

	/* free everything */
	free_everything(data_image, machine_code, machine_code_size, &sym_table, mcro_tb, &parsed_line);

	return SUCCESS_CODE;
}

int insert_symbol(char *name, char *attribute, int value, hashmap_t *sym_tb, hashmap_t *mcro_tb) {
	Symbol *sym;

	if (!attribute) {
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
	(*data_image)[DC++] = value;
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
	/* adding the null terminator */
	value = (int)string[i];
	if (add_data_word(value, data_cap, data_image) == EXIT_FAILURE) {
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}

int add_instruction(Line *line, FirstInstruction ***machine_code, hashmap_t *sym_tb, int L, int line_num, int *machine_code_size) {
	FirstInstruction *inst, **machine_code_buffer;
	int return_code, index, arg_index = 0;
	int found_error = FALSE;

	/* checking for null input */
	if (!machine_code || !sym_tb || !line || !machine_code_size) return TRUE;

	/* Using IC - 100 because IC starts at 100. */
	if ((IC - IC_START) >= *machine_code_size) {
		*machine_code_size *= 2;
		machine_code_buffer = realloc(*machine_code, *machine_code_size * sizeof(FirstInstruction *));

		if (!machine_code_buffer) return TRUE; /* Memory Error - Abort method */

		*machine_code = machine_code_buffer;
	}

	/* finding the index inside the OPCODES array */
	index = find_in_opcode(line->command);
	if (index < SUCCESS_CODE) {
		/* OPCODE not found */
		printerror("Error,\n", line_num, index);
		found_error = TRUE;
		return found_error;
	}

	/* allocating memory for the instruction */
	inst = malloc(sizeof(FirstInstruction));
	if (!inst) return TRUE;

	/* L is the number of info-words */
	inst->L = L;

	/* update the instruction based on the operations table. */
	inst->funct = OPCODES[index].funct;
	inst->are = ARE_ABSOLUTE;
	inst->opcode = OPCODES[index].opcode;

	/* checking if the operation has a source argument */
	if (OPCODES[index].is_source) {

		return_code = process_argument(line->arguments[arg_index], sym_tb, line_num, &inst->src_register, &inst->src_addressing);
		if (return_code != SUCCESS_CODE) {
			free(inst);
			return TRUE;
		}
		arg_index++;
	}
	/* checking if the operation has a destionation argument */
	if (OPCODES[index].is_dest) {
		return_code = process_argument(line->arguments[arg_index], sym_tb, line_num, &inst->dest_register, &inst->dest_addressing);
		if (return_code != SUCCESS_CODE) {
			free(inst);
			return TRUE;
		}
	}

	(*machine_code)[IC - IC_START] = inst;

	return found_error;
}

/*
 * Find the index of an opcode in the OPCODES array
 * Returns the index if found, OEPRATION_NOT_FOUND otherwise
 */
int find_in_opcode(char *string) {
	int i;
	const int num_opcodes = ARRAY_SIZE(OPCODES);

	if (!string) {
		return OPCODE_NOT_FOUND;
	}
	for (i = 0; i < num_opcodes; i++) {
		if (COMPARE_STR(string, OPCODES[i].op_code_name)) {
			return i;
		}
	}

	return OPCODE_NOT_FOUND;
}

int check_arg_count(char **args, int index) {
	int i;

	if (!args)
		return 0;
	/* Counting the amount of arguments given */
	for (i = 0; args[i] != NULL; i++);

	/* Check if there are more operands than required */
	if (i > OPCODES[index].args_num) return TOO_MANY_ARGS;

	/* Check if there are less operands than required */
	if (i < OPCODES[index].args_num) return MISSING_ARGS;

	/* The amount of operands is correct. */
	return SUCCESS_CODE;
}

int find_addressing_method(char *operand, hashmap_t *sym_tb) {

	if (!operand || isEmpty(operand)) return FAIL_CODE;

	if (*operand == '#') return IMMEDIATE;

	if (is_register(operand)) return REGISTER_DIRECT;

	if (*operand == '&') return RELATIVE;

	return DIRECT;
}

int count_info_words_required(char **args, hashmap_t *sym_tb) {
	int L = 1; /* At least 1 is required */
	int i, arg_count, addressing_method;

	arg_count = string_array_len(args);

	for (i = 0; i < arg_count; i++) {
		addressing_method = find_addressing_method(args[i], sym_tb);
		switch (addressing_method) {
		case IMMEDIATE:
			L += 1;
			break;
		case DIRECT:
			L += 1;
			break;
		case REGISTER_DIRECT:
			break;
		case RELATIVE:
			L += 1;
			break;
		case NOT_A_LABEL:
			return NOT_A_LABEL;
		case FAIL_CODE:
			return FAIL_CODE;
		};
	}
	return L;
}

void set_data_to_icf(hashmap_t *sym_tb, int ICF) {
	int i;
	HashNode *node, *temp;
	Symbol *sym;

	if (sym_tb == NULL) return;

	for (i = 0; i < sym_tb->size; i++) {
		node = sym_tb->table[i];

		while (node != NULL) {
			temp = node;
			node = node->next;

			sym = (Symbol *)temp->value;

			if (COMPARE_STR(sym->attribute, DATA)) {
				sym->value += ICF;
			}
		}
	}
}

void free_symbol(Symbol *sym) {
	if (sym->attribute != NULL) {
		free(sym->attribute);
	}
	if (sym->name != NULL) {
		free(sym->name);
	}
	free(sym);
}

int process_argument(char *argument, hashmap_t *sym_tb, int line_num, int *reg, int *addr) {
	int num;
	/* Check if the argument is a register */
	if ((num = is_register(argument)) != FALSE) {
		*reg = num;
		*addr = 0;
		return SUCCESS_CODE;
	} else {
		*reg = 0;
		num = find_addressing_method(argument, sym_tb);
		if (num < SUCCESS_CODE) {
			printerror("Error,\n", line_num, num);
			*addr = num; /* Store the error code in the addressing field */
			return FAIL_CODE;
		}
		*addr = num;
		return SUCCESS_CODE;
	}
}

void free_everything(int *data_image, FirstInstruction **machine_code, int machine_code_size, hashmap_t *sym_table, hashmap_t *mcro_tb, Line *line) {
	int i;

	if (data_image)
		free(data_image);

	if (machine_code) {
		for (i = 0; i < machine_code_size; i++) {
			if (machine_code[i])
				free(machine_code[i]);
		}
		free(machine_code);
	}

	if (sym_table) {
		print_hashmap(sym_table, (void (*)(void *))print_symbol); /* debug line */
		free_hashmap(sym_table, (void (*)(void *))free_symbol);
	}

	if (mcro_tb)
		free_hashmap(mcro_tb, (void (*)(void *))free_macro);

	if (line)
		free_line(line);
}

/* debug method */
void print_symbol(Symbol *sym) {
	printf("Symbol Name: %s, Attribute: %s, Value: %d",
		   sym->name ? sym->name : "NULL",
		   sym->attribute ? sym->attribute : "NULL",
		   sym->value);
}