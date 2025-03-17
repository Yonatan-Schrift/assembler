#include "../h/first_pass.h"
#include "../h/file_funcs.h"
#include "../h/globals.h"
#include "../h/hashmap.h"
#include "../h/line.h"
#include "../h/pre_assembler.h"

#define IS_STORE_INST(a) (strcmp((a), ".string") == STRCMP_SUCCESS || strcmp((a), ".data") == STRCMP_SUCCESS)
#define IS_ENTRY_OR_EXTERN(a) (strcmp((a), ".extern") == STRCMP_SUCCESS || strcmp((a), ".entry") == STRCMP_SUCCESS)
#define COMPARE_STR(a, b) (strcmp(a, b) == STRCMP_SUCCESS)

op_code OPCODES[] = {
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

int first_pass(char *src_path, hashmap_t *mcro_tb) {
	char line[MAX_LINE_LENGTH + 1], *new_path;
	FILE *file_in, *file_ob;
	Line parsed_line;
	int error_flag = FALSE, current_error = FALSE, is_symbol = FALSE;
	int line_count = 0, i, value, opcode_index, L;
	hashmap_t sym_table;
	int data_size = INITIAL_ARRAY_SIZE, *data_image, machine_code_size = INITIAL_ARRAY_SIZE;
	FirstInstruction *machine_code;
	int ICF, DCF;

	data_image = malloc(INITIAL_ARRAY_SIZE * sizeof(int));
	if (!data_image)
		return EXIT_FAILURE;

	machine_code = malloc(INITIAL_ARRAY_SIZE * sizeof(FirstInstruction));
	if (!machine_code) {
		free(data_image);
		return EXIT_FAILURE;
	}

	/*
		new_path = change_extension(src_path, ".ob");  might be useless
	 */
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
							free(machine_code);
							close_mult_files(file_in, file_ob, NULL, NULL, NULL, NULL);
							free_line(&parsed_line);
							free_hashmap(&sym_table, (void (*)(void *))free_symbol);
							free_hashmap(mcro_tb, (void (*)(void *))free_macro);

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
						free(data_image);
						free(machine_code);
						close_mult_files(file_in, file_ob, NULL, NULL, NULL, NULL);
						free_line(&parsed_line);
						free_hashmap(&sym_table, (void (*)(void *))free_symbol);
						free_hashmap(mcro_tb, (void (*)(void *))free_macro);

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
			/* Is an instructive statement */
			if (is_symbol) {
				current_error = insert_symbol(parsed_line.label, CODE, IC, &sym_table, mcro_tb);
				if (current_error != SUCCESS_CODE)
					printerror("SYMBOL ERROR", line_count, current_error);
				error_flag = TRUE;
				continue;
			}

			/* Stage 12 */
			/* find the opcode */
			opcode_index = find_opcode(parsed_line.command);
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
			/* L */
			L = count_info_words_required(parsed_line.arguments, &sym_table);
			if (L < SUCCESS_CODE) {
				printerror("Info word error found: \n", line_count, L);
				error_flag = TRUE;
				continue;
			}

			/* Stage 15 */
			/* idk what to do here */
			current_error = add_instruction(&parsed_line, machine_code, machine_code_size, L);
			if (current_error != SUCCESS_CODE) {
				printerror("Error doing something cool\n", line_count, current_error);
				error_flag = TRUE;
				continue;
			}

			/* Stage 16 */
			IC += L;
		}
	}

	/* FOR SURE NEEDS TO BE REDONE - Stage 17 */
	if (error_flag == TRUE) {
		printf("ERRORS WERE FOUND DURING THE FIRST PASS!");
	}
	free(data_image);
	free(machine_code);
	return EXIT_SUCCESS;

	/* Stage 18 */
	ICF = IC;
	DCF = DC;

	/* Stage 19 */
	/* Update the symbol table with ICF for DATA symbols */
	set_data_to_icf(&sym_table, ICF);
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

/* WIP */
int add_instruction(Line *line, FirstInstruction **machine_code, int *machine_code_size, int L) {
	FirstInstruction inst;
	FirstInstruction *temp;
	int num;

	int index = find_in_opcode(line->command);
	if (index < SUCCESS_CODE) {
		return index;
	}

	if (IC - 100 >= *machine_code_size) {
		*machine_code_size *= 2;
		temp = realloc(*machine_code, *machine_code_size * sizeof(FirstInstruction));
		if (!temp) {
			printerror("Memory failue", NO_LINE, 0);
			return EXIT_FAILURE;
		}
		*machine_code = temp;
	}

	inst.L = L; /* no bit representation */

	inst.funct = OPCODES[index].funct;
	inst.are = 4; /* 4 = 1-0-0 */
	inst.opcode = OPCODES[index].opcode;

	/* both source and dest is WIP (work in progress) */
	if (OPCODES[index].is_source) {
		if ((num = is_register(line->arguments[0])) != FALSE) {
			inst.src_register = num;
			inst.src_addressing = 0;
		}
	} else {
		inst.src_register = 0;
		inst.src_addressing = 0;
	}

	if (OPCODES[index].is_dest) {
		if ((num = is_register(line->arguments[1]) != FALSE)) {
			inst.dest_register = num;
			inst.dest_addressing = 0;
		} else {
			inst.dest_addressing = 0;
			inst.dest_register = 0;
		}
	}
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
	if (i > OPCODES->args_num) return TOO_MANY_ARGS;

	/* Check if there are less operands than required */
	if (i < OPCODES->args_num) return MISSING_ARGS;

	/* The amount of operands is correct. */
	return SUCCESS_CODE;
}

int find_addressing_method(char *operand, hashmap_t *sym_tb) {
	char *operand_without_start;

	if (!operand || isEmpty(operand)) return FAIL_CODE;

	if (*operand == '#') return IMMEDIATE;

	if (is_register(operand)) return REGISTER_DIRECT;

	if (lookup(sym_tb, operand)) return DIRECT;

	if (*operand == '&') {
		operand_without_start = operand + 1;
		if (lookup(sym_tb, operand_without_start))
			return RELATIVE;
		else
			return NOT_A_LABEL;
	}

	return FAIL_CODE;
}

/* Not used in the first pass */
int build_instruction_word(int opcode, int source_addressing, int source_register, int des_addressing, int des_register, int funct, int are) {
	int instruction = 0;

	instruction |= (opcode << 18);
	instruction |= (source_addressing << 16);
	instruction |= (source_register << 13);
	instruction |= (des_addressing << 11);
	instruction |= (des_register << 8);
	instruction |= (funct << 3);
	instruction |= are;

	return instruction;
}

/* Not used in the first pass */
int build_info_word(int address, int addressing_method, char *type) {
	int info_word = 0, ARE;

	info_word = (address << 3);

	switch (addressing_method) {
	case IMMEDIATE:
		ARE = 4; /* 4 is 100 in binary (A=1, R=0, E=0) */
		break;
	case DIRECT:
		ARE = (COMPARE_STR(type, EXTERNAL)) ? 1 : 2; /* 1 is 001 for external addresses, 2 is 010 for internal addresses */
		break;
	case RELATIVE:
		ARE = 4; /* 4 is 100 in binary (A=1, R=0, E=0) */
		break;
	default:
		return FAIL_CODE; /* Generic error code as there shouldn't be any errors heres */
		break;
	}

	info_word = info_word | ARE;

	return info_word;
}

/* Not used in the first pass - ALSO WIP */
int build_info_words(Line *line, hashmap_t *sym_tb) {
	int i, L, add_method, instr_word;
	L = count_info_words_required(line->arguments, sym_tb);
	if (L < SUCCESS_CODE) {
		return L;
	}
	instr_word = build_instruction_word(find_opcode(line->command), 0, 0, 0, 0, 0, 0);
	for (i = 0; i < L; i++) {
		add_method = find_addressing_method(line->arguments[i], sym_tb);
		if (add_method == IMMEDIATE) {
			return instr_word; /* UNUSED RN */
		}
	}
	return L;
}

int count_info_words_required(char **args, hashmap_t *sym_tb) {
	int L = 1; /* starting with 1 for the instruction itself */
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
}
