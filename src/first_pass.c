#include "../h/first_pass.h"
#include "../h/file_funcs.h"
#include "../h/hashmap.h"
#include "../h/line.h"
#include "../h/macro.h"
#include "../h/second_pass.h"
#include <string.h>

#define IS_STORE_INST(a) (strcmp((a), ".string") == STRCMP_SUCCESS || strcmp((a), ".data") == STRCMP_SUCCESS)
#define IS_ENTRY_OR_EXTERN(a) (strcmp((a), ".extern") == STRCMP_SUCCESS || strcmp((a), ".entry") == STRCMP_SUCCESS)
#define COMPARE_STR(a, b) (strcmp(a, b) == STRCMP_SUCCESS)
#define METHOD_COUNT_TO_CHECK 3


const op_code OPCODES[] = {
	/* {"command", "opcode", "funct", "req_args", "is_source", "is_dest", allowed_source_methods, allowed_dest_methods} */
	{"mov", 0, 0, 2, TRUE, TRUE, {IMMEDIATE, DIRECT, REGISTER_DIRECT}, {DIRECT, REGISTER_DIRECT, EMPTY_VALUE}},
	{"cmp", 1, 0, 2, TRUE, TRUE, {IMMEDIATE, DIRECT, REGISTER_DIRECT}, {IMMEDIATE, DIRECT, REGISTER_DIRECT}},
	{"add", 2, 1, 2, TRUE, TRUE, {IMMEDIATE, DIRECT, REGISTER_DIRECT}, {DIRECT, REGISTER_DIRECT, EMPTY_VALUE}},
	{"sub", 2, 2, 2, TRUE, TRUE, {IMMEDIATE, DIRECT, REGISTER_DIRECT}, {DIRECT, REGISTER_DIRECT, EMPTY_VALUE}},
	{"lea", 4, 0, 2, TRUE, TRUE, {DIRECT, EMPTY_VALUE, EMPTY_VALUE}, {DIRECT, REGISTER_DIRECT, EMPTY_VALUE}},

	{"clr", 5, 1, 1, FALSE, TRUE, {EMPTY_VALUE, EMPTY_VALUE, EMPTY_VALUE}, {DIRECT, REGISTER_DIRECT, EMPTY_VALUE}},
	{"not", 5, 2, 1, FALSE, TRUE, {EMPTY_VALUE, EMPTY_VALUE, EMPTY_VALUE}, {DIRECT, REGISTER_DIRECT, EMPTY_VALUE}},
	{"inc", 5, 3, 1, FALSE, TRUE, {EMPTY_VALUE, EMPTY_VALUE, EMPTY_VALUE}, {DIRECT, REGISTER_DIRECT, EMPTY_VALUE}},
	{"dec", 5, 4, 1, FALSE, TRUE, {EMPTY_VALUE, EMPTY_VALUE, EMPTY_VALUE}, {DIRECT, REGISTER_DIRECT, EMPTY_VALUE}},
	{"jmp", 9, 1, 1, FALSE, TRUE, {EMPTY_VALUE, EMPTY_VALUE, EMPTY_VALUE}, {DIRECT, RELATIVE, EMPTY_VALUE}},
	{"bne", 9, 2, 1, FALSE, TRUE, {EMPTY_VALUE, EMPTY_VALUE, EMPTY_VALUE}, {DIRECT, RELATIVE, EMPTY_VALUE}},
	{"jsr", 9, 3, 1, FALSE, TRUE, {EMPTY_VALUE, EMPTY_VALUE, EMPTY_VALUE}, {DIRECT, RELATIVE, EMPTY_VALUE}},
	{"red", 12, 0, 1, FALSE, TRUE, {EMPTY_VALUE, EMPTY_VALUE, EMPTY_VALUE}, {DIRECT, REGISTER_DIRECT, EMPTY_VALUE}},
	{"prn", 13, 0, 1, FALSE, TRUE, {EMPTY_VALUE, EMPTY_VALUE, EMPTY_VALUE}, {IMMEDIATE, DIRECT, REGISTER_DIRECT}},

	{"rts", 14, 0, 0, FALSE, FALSE, {EMPTY_VALUE, EMPTY_VALUE, EMPTY_VALUE}, {EMPTY_VALUE, EMPTY_VALUE, EMPTY_VALUE}},
	{"stop", 15, 0, 0, FALSE, FALSE, {EMPTY_VALUE, EMPTY_VALUE, EMPTY_VALUE}, {EMPTY_VALUE, EMPTY_VALUE, EMPTY_VALUE}}};

int IC;
int DC;

int first_pass(char *src_path, hashmap_t *mcro_tb) {
	char line[MAX_LINE_LENGTH + 1], *string_arg;
	FILE *file_in;
	Line parsed_line;
	int error_flag = FALSE, current_error = FALSE, is_symbol = FALSE;
	int line_count = 0, i, value, opcode_index, L, ret;
	hashmap_t sym_table;
	int data_size, *data_image, machine_code_size;
	FirstInstruction **machine_code;
	int ICF;
	int DCF;

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
	if (!file_in) {
		close_mult_files(file_in, NULL, NULL, NULL);
		free(data_image);
		free(machine_code);

		return FAIL_CODE;
	}
	init_line(&parsed_line);
	init_hashmap(&sym_table, TABLE_SIZE);

	IC = 100;
	DC = 0;

	while ((current_error = read_line(file_in, line)) != EXIT_FAILURE) {
		/*  For iterations after the first, free the memory allocated for the previous line.
		 * (On the first iteration, no memory has been allocated yet.) */
		if (line_count > 0) {
			free_line(&parsed_line);
		}

		/* Reinitialize parsed_line to prepare for processing the next line. */
		init_line(&parsed_line);

		line_count++;
		is_symbol = FALSE;

		/* Check if the line encountered an error */
		if (current_error < SUCCESS_CODE) {
			printerror(line_count, current_error);
			continue;
		}

		/* Read a line from the source */

		/* Skips the line if it's empty */
		if (isEmpty(line) == TRUE) {
			continue;
		}

		if (split_line(line, &parsed_line) != EXIT_FAILURE) {

			/* check for a label */
			if (parsed_line.label != NULL) {
				is_symbol = TRUE;
			}
			/* check for an empty label */
			if (!parsed_line.command) {
				printerror(line_count, EMPTY_LABEL);
				error_flag = TRUE;
				continue;
			}
			/* Stages 5-7 */
			/* check if the instruction stores data in the memory */
			if (IS_STORE_INST(parsed_line.command)) {
				if (is_symbol) {
					current_error = insert_symbol(parsed_line.label, DATA, FALSE, DC, &sym_table, mcro_tb);

					if (current_error != SUCCESS_CODE) {
						printerror(line_count, current_error);
						error_flag = TRUE;
					}
				}

				if (COMPARE_STR(parsed_line.command, ".data")) {
					/* is '.data' instruction */
					if (parsed_line.arguments[0] == NULL) {
						/* Checks that the data does exist */
						printerror(line_count, MISSING_ARGS);
						error_flag = TRUE;
						continue;
					}
					for (i = 0; parsed_line.arguments[i] != NULL; i++) {
						/* saves each integer to the data-image */
						value = atoi(parsed_line.arguments[i]);
						/* Here error check the data */
						if ((current_error = check_valid_number(parsed_line.arguments[i])) < SUCCESS_CODE) {
							printerror(line_count, current_error);
							error_flag = TRUE;
							break;
						}
						if (add_data_word(value, &data_size, &data_image) != SUCCESS_CODE) {
							/* Memory failure */
							free_everything(data_image, machine_code, machine_code_size, &sym_table, mcro_tb, &parsed_line);
							close_mult_files(file_in, NULL, NULL, NULL);

							return EXIT_FAILURE;
						}
					}
					/* Check for an extra comma on the last param */
					i = (i != 0) ? i - 1 : i; /* Lower the index by 1 if there are more than one number */
					if ((current_error = check_for_commas(parsed_line.arguments[i])) < SUCCESS_CODE) {
						printerror(line_count, current_error);
						error_flag = TRUE;
						continue;
					}
				}

				else if (COMPARE_STR(parsed_line.command, ".string")) {
					/* is '.string' instruction */

					ret = find_quotes(line);
					/* Checking if the quote has an error */
					if (ret < SUCCESS_CODE) {
						error_flag = TRUE;
						printerror(line_count, ret);
						continue;
					}
					string_arg = copy_string(line + ret);

					if (add_string_word(string_arg, &data_size, &data_image) != EXIT_SUCCESS) {

						/* Memory failure */
						close_mult_files(file_in, NULL, NULL, NULL);
						free_everything(data_image, machine_code, machine_code_size, &sym_table, mcro_tb, &parsed_line);

						return EXIT_FAILURE;
					}
				}
			}

			/* Stages 8-10 */
			/* check if the instruction is an entry or extern variable */
			else if (IS_ENTRY_OR_EXTERN(parsed_line.command)) {
				current_error = check_arg_count(parsed_line.arguments, NO_INDEX, REQUIRED_ARGS_FOR_DIRECTIVE);
				if (current_error != SUCCESS_CODE) {
					printerror(line_count, current_error);
					error_flag = TRUE;
					continue;
				}
				if (COMPARE_STR(parsed_line.command, ".extern")) {
					/* Is '.extern' instruction */
					current_error = insert_symbol(parsed_line.arguments[0], NO_ATTR, EXTERNAL, 0, &sym_table, mcro_tb); /* probl */

					if (current_error != SUCCESS_CODE) {
						printerror(line_count, current_error);
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
					current_error = insert_symbol(parsed_line.label, CODE, FALSE, IC, &sym_table, mcro_tb);
					if (current_error != SUCCESS_CODE) {
						printerror(line_count, current_error);
						error_flag = TRUE;
					}
				}

				/* Stage 12 */
				/* find the opcode */
				opcode_index = find_in_opcode(parsed_line.command);
				/* Saving the opcode and checking if it failed. */
				if (opcode_index < SUCCESS_CODE) {
					printerror(line_count, opcode_index);
					error_flag = TRUE;
					continue;
				}

				/* checking the amount of arguments */
				current_error = check_arg_count(parsed_line.arguments, opcode_index, NO_ARG_COUNT);
				if (current_error != SUCCESS_CODE) {
					printerror(line_count, current_error);
					error_flag = TRUE;
					continue;
				}

				/* Stage 13 */
				/* find L - the number of info-words required */
				L = count_info_words_required(parsed_line.arguments);
				if (L < SUCCESS_CODE) {
					printerror(line_count, L);
					error_flag = TRUE;
					continue;
				}

				/* Stage 15 */

				current_error = add_instruction(&parsed_line, &machine_code, &sym_table, L, line_count, &machine_code_size);
				if (current_error != SUCCESS_CODE) {
					/* error is printed in the function */
					error_flag = TRUE;
					continue;
				}

				/* Stage 16 */
				IC += L;
			}
		}
	}

	/* Stage 17 */
	close_mult_files(file_in, NULL, NULL, NULL);

	/* Check if the program used too much memory */
	if ((IC - 100) + DC >= MAX_MEMORY) {
		printerror(NO_LINE, OUT_OF_MEMORY);
		error_flag = TRUE;
	}

	if (error_flag == TRUE) {
		printf("\n\n>>> ERRORS WERE FOUND DURING THE FIRST PASS!\n\n");

		free_everything(data_image, machine_code, machine_code_size, &sym_table, mcro_tb, &parsed_line);

		return FAIL_CODE;
	}

	/* Stage 18 */
	ICF = IC;
	DCF = DC;

	/* Stage 19 */
	/* Update the symbol table with ICF for DATA symbols */
	set_data_to_icf(&sym_table, ICF);

	free_hashmap(mcro_tb, (void (*)(void *))free_macro);

	printf(">>> Finished first pass successfully \n");

	/* starts the second pass, saves the return code*/
	current_error = second_pass(src_path, &sym_table, data_image, data_size, machine_code, machine_code_size, ICF, DCF);

	/* free everything */
	free_everything(data_image, machine_code, machine_code_size, &sym_table, NULL, &parsed_line);

	return current_error;
}

int insert_symbol(char *name, int attribute, int is_ext, int value, hashmap_t *sym_tb, hashmap_t *mcro_tb) {
	Symbol *sym, *lookup_ret;
	int ret;

	if ((lookup_ret = (Symbol *)lookup(sym_tb, name))) {
		/* Checks if a symbol is defined twice */
		if (lookup_ret->entry_or_extern == EXTERNAL) return INITIALIZING_EXTERN;
		return SYMBOL_ALREADY_EXISTS;
	}
	if (lookup(mcro_tb, name)) {
		/* Checks if a symbol is defined with the same name as a macro */
		return SYMBOL_IS_MACRO;
	}

	/* Check for unallowed names */
	if ((ret = is_reserved_name(name)) != FALSE) {
		return ret;
	}

	sym = malloc(sizeof(Symbol));
	if (!sym) {
		return EXIT_FAILURE;
	}

	sym->entry_or_extern = is_ext;
	sym->name = copy_string(name);
	sym->attribute = attribute;
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
			printerror(NO_LINE, 0);
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
			free(string);
			return EXIT_FAILURE;
		}
	}
	/* adding the null terminator */
	value = (int)string[i];
	if (add_data_word(value, data_cap, data_image) == EXIT_FAILURE) {
		free(string);
		return EXIT_FAILURE;
	}
	free(string);
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
		printerror(line_num, index);
		return TRUE;
	}

	/* allocating memory for the instruction */
	inst = malloc(sizeof(FirstInstruction));
	if (!inst) return TRUE;

	/* L is the number of info-words */
	inst->L = L;
	inst->index = index;

	/* update the instruction based on the operations table. */
	inst->funct = OPCODES[index].funct;
	inst->are = ARE_ABSOLUTE;
	inst->opcode = OPCODES[index].opcode;

	/* initiallizing the operands */
	inst->dest_operand = NULL;
	inst->src_operand = NULL;

	/* Validate the argument types (addressing method) */
	return_code = validate_arguments(line->arguments, index);
	if (return_code != SUCCESS_CODE) {
		printerror(line_num, return_code);
		free(inst);
		return TRUE;
	}

	/* checking if the operation has a source argument */
	if (OPCODES[index].is_source) {

		return_code = process_argument(line->arguments[arg_index], line_num, &inst->src_register, &inst->src_addressing, &inst->src_operand, &inst->immediate_value);
		if (return_code != SUCCESS_CODE) {
			free(inst);
			return TRUE;
		}
		arg_index++;
	} else {
		inst->src_register = 0;
		inst->src_addressing = 0;
	}
	/* checking if the operation has a destination argument */
	if (OPCODES[index].is_dest) {
		return_code = process_argument(line->arguments[arg_index], line_num, &inst->dest_register, &inst->dest_addressing, &inst->dest_operand, &inst->immediate_value);
		if (return_code != SUCCESS_CODE) {
			free(inst);
			return TRUE;
		}
	} else {
		inst->dest_register = 0;
		inst->dest_addressing = 0;
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

	if (string[strlen(string) - 1] == ',') return EXTRA_COMMA_AFTER_COMMAND;

	for (i = 0; i < num_opcodes; i++) {
		if (COMPARE_STR(string, OPCODES[i].op_code_name)) {
			return i;
		}
	}

	return OPCODE_NOT_FOUND;
}

int check_arg_count(char **args, int index, int required_arg_count) {
	int actual_arg_count = 0;
	int expected_arg_count;

	if (!args)
		return FAIL_CODE;

	while (args[actual_arg_count]) actual_arg_count++;

	/* Checking if an index is given, otherwise uses the given arg_count */
	expected_arg_count = (index != NO_INDEX) ? OPCODES[index].args_num : required_arg_count;

	if (actual_arg_count > expected_arg_count) return TOO_MANY_ARGS;
	if (actual_arg_count < expected_arg_count) return MISSING_ARGS;

	return SUCCESS_CODE;
}

int find_addressing_method(char *operand) {

	if (!operand || isEmpty(operand)) return FAIL_CODE;

	if (*operand == '#') {
		if (check_valid_number((operand + 1))) return NOT_AN_INT_WHEN_USING_IMMEDIATE;
		return IMMEDIATE;
	}

	if (is_register(operand)) return REGISTER_DIRECT;

	if (*operand == '&') return RELATIVE;

	return DIRECT;
}

int count_info_words_required(char **args) {
	int L = 1; /* At least 1 is required */
	int i, arg_count, addressing_method;
	char *cur_arg;

	arg_count = string_array_len(args);

	for (i = 0; i < arg_count; i++) {
		cur_arg = clean_arg(args[i]);
		addressing_method = find_addressing_method(cur_arg);
		free(cur_arg);
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

			if (sym->attribute == DATA) {
				sym->value += ICF;
			}
		}
	}
}

void free_symbol(Symbol *sym) {
	if (sym->name != NULL) {
		free(sym->name);
	}
	free(sym);
}

int process_argument(char *arg, int line_num, int *reg, int *addr, char **operand, int *value) {
	int num;
	char *cur_arg = clean_arg(arg);

	/* Check for errors with commas */
	if ((num = check_for_commas(arg)) < SUCCESS_CODE) {
		free(cur_arg);
		printerror(line_num, num);
		*addr = num;
		return FAIL_CODE;
	}

	/* Check if the argument is a register */
	if ((num = is_register(cur_arg)) != FALSE) {
		free(cur_arg);
		*reg = num;
		*addr = REGISTER_DIRECT;
		return SUCCESS_CODE;
	} else {
		*reg = 0;
		num = find_addressing_method(cur_arg);
		if (num < SUCCESS_CODE) {
			free(cur_arg);
			printerror(line_num, num);
			*addr = num; /* Store the error code in the addressing field */
			return FAIL_CODE;
		}
		*addr = num;

		/* Check which addressing method is used for the operand */
		switch (num) {
		case IMMEDIATE:
			*value = atoi(cur_arg + 1); /* +1 to skip '#' */
			free(cur_arg);
			operand = NULL;
			break;
		case RELATIVE:
			*operand = (cur_arg + 1); /* + 1 to skip '&' */
			value = 0;
			break;
		case DIRECT:
			*operand = cur_arg;
			value = 0;
			break;
		}

		return SUCCESS_CODE;
	}
}

void free_everything(int *data_image, FirstInstruction **machine_code, int machine_code_size, hashmap_t *sym_table, hashmap_t *mcro_tb, Line *line) {
	int i;
	FirstInstruction *cur_inst;
	if (data_image)
		free(data_image);

	if (machine_code) {
		for (i = 0; i < machine_code_size; i++) {
			if (machine_code[i]) {
				cur_inst = machine_code[i];

				if (cur_inst->src_operand) {
					if (cur_inst->src_addressing == RELATIVE) cur_inst->src_operand--; /* if relative addressing, offset the operand back by 1 */
					free(cur_inst->src_operand);
				}
				if (machine_code[i]->dest_operand) {
					if (cur_inst->dest_addressing == RELATIVE) cur_inst->dest_operand--; /* if relative addressing, offset the operand back by 1 */
					free(cur_inst->dest_operand);
				}

				free(cur_inst);
			}
		}
		free(machine_code);
	}

	if (sym_table) {
		free_hashmap(sym_table, (void (*)(void *))free_symbol);
	}

	if (mcro_tb)
		free_hashmap(mcro_tb, (void (*)(void *))free_macro);

	if (line)
		free_line(line);
}

int compare_symbols_by_value(const void *a, const void *b) {
	Symbol *sym_a = *(Symbol **)a;
	Symbol *sym_b = *(Symbol **)b;
	return sym_a->value - sym_b->value;
}

int validate_arguments(char **args, int index) {
	int i, arg_index = 0, addr, found;
	char *cur_arg;

	if (OPCODES[index].is_source) {
		cur_arg = clean_arg(args[arg_index++]);

		addr = find_addressing_method(cur_arg);

		free(cur_arg);

		found = FALSE;
		for (i = 0; i < METHOD_COUNT_TO_CHECK; i++) {
			if (addr == OPCODES[index].valid_source_method[i]) {
				found = TRUE;
				break;
			}
		}
		if (!found) return INCORRECT_PARAM_TYPE;
	}
	if (OPCODES[index].is_dest) {
		cur_arg = clean_arg(args[arg_index]);

		addr = find_addressing_method(cur_arg);

		free(cur_arg);

		found = FALSE;
		for (i = 0; i < METHOD_COUNT_TO_CHECK; i++) {
			if (addr == OPCODES[index].valid_dest_method[i]) {
				found = TRUE;
				break;
			}
		}
		if (!found) return INCORRECT_PARAM_TYPE;
	}

	return SUCCESS_CODE;
}