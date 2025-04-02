

#include "../h/second_pass.h" /*  */
#include "../h/file_funcs.h"  /* used for open_file, close_mult_files and WRITE_MODE */
#include "../h/first_pass.h"  /* Used for symbol and instructions */
#include "../h/globals.h"	  /* globals */
#include "../h/hashmap.h"	  /* used as the data structure for the symbol table */
#include "../h/line.h"		  /* Used for processing lines in the source */

#define COMPARE_STR(a, b) (strcmp(a, b) == STRCMP_SUCCESS)
/* temp macro */

int second_pass(char *src_path, hashmap_t *sym_tb, int *data_image, int data_size, FirstInstruction **machine_code, int machine_code_size, int ICF, int DCF) {
	char line[MAX_LINE_LENGTH + 1];
	FILE *file_ob, *file_ent, *file_ext, *file_am;
	Line parsed_line;
	Symbol *sym;
	int error_flag = FALSE, current_error = FALSE;
	int line_count = 0, i, ic = 0;

	file_ob = open_file(src_path, ".ob", WRITE_MODE);
	file_ent = open_file(src_path, ".ent", WRITE_MODE);
	file_ext = open_file(src_path, ".ext", WRITE_MODE);
	file_am = open_file(src_path, ".am", READ_MODE);

	if (!file_ext || !file_ob || !file_ent || !file_am) {
		close_mult_files(file_ext, file_ob, file_ent, file_am);
		return FAIL_CODE;
	}
	init_line(&parsed_line);

	/* Write the first line in the object file */
	fprintf(file_ob, "     %d %d\n", ICF - 100, DCF);

	while ((current_error = read_line(file_am, line)) != EXIT_FAILURE) {
		/* For iterations after the first, free the memory allocated for the previous line.
		 * (On the first iteration, no memory has been allocated yet.) */
		if (line_count > 0) free_line(&parsed_line);
		/* Reinitialize parsed_line to prepare for processing the next line. */
		init_line(&parsed_line);

		line_count++;

		if (current_error < SUCCESS_CODE) {
			printerror(line_count, current_error);
			continue;
		}

		if (isEmpty(line) == TRUE) {
			continue;
		}

		if (split_line(line, &parsed_line) != EXIT_FAILURE) {

			/* ignore extern / data / string */
			if (COMPARE_STR(parsed_line.command, ".data") ||
				COMPARE_STR(parsed_line.command, ".string") ||
				COMPARE_STR(parsed_line.command, ".extern")) {
				continue;
			}

			/* is .entry, insert it to the sym table */
			if (COMPARE_STR(parsed_line.command, ".entry")) {
				/* A check that there is only the argument */
				current_error = check_arg_count(parsed_line.arguments, NO_INDEX, REQUIRED_ARGS_FOR_DIRECTIVE);
				if (current_error) {
					printerror(line_count, current_error);
					error_flag = TRUE;
					continue;
				}

				if ((sym = (Symbol *)lookup(sym_tb, parsed_line.arguments[0]))) {
					sym->entry_or_extern = ENTRY; /* setting the attribute to ENTRY */
				} else {
					printerror(line_count, ENTRY_SYMBOL_NOT_FOUND);
					error_flag = TRUE;
					continue;
				}
			}
		}
	}
	/* check for errors before building the files */
	if (error_flag == TRUE) {
		printf("\n\n>>> ERRORS WERE FOUND DURING THE SECOND PASS!\n\n");
		delete_mult_files(src_path, ".ob", ".ent", ".ext");

		return FAIL_CODE;
	}

	/* writing the machine code into the object file */
	for (i = 0; i < machine_code_size; i++) {
		ic = i + 100;
		current_error = build_binary_instruction(machine_code[i], sym_tb, file_ob, file_ext, &ic);
		if (current_error) {
			printerror(ic, current_error);
			error_flag = TRUE;
			continue;
		}
	}
	/* writing the data image into the object file */
	for (i = 0, ic = ICF; i < DCF; i++) {
		ic = ICF + i;

		fprintf(file_ob, "%07d %06x\n", ic, data_image[i] & 0xFFFFFF);
	}

	write_symbols_to_files(sym_tb, file_ent);

	/* Check for any errors that might happen during file building */
	if (error_flag == TRUE) {
		printf("\n\n>>> ERRORS WERE FOUND DURING THE SECOND PASS!\n\n");
		delete_mult_files(src_path, ".ob", ".ent", ".ext");

		return FAIL_CODE;
	}

	/* Checking if the output files are empty and removing them if so */
	delete_if_empty(file_ent, src_path, ".ent");
	delete_if_empty(file_ext, src_path, ".ext");
	delete_if_empty(file_ob, src_path, ".ob");	

	free_line(&parsed_line);

	printf(">>> Finished second pass successfully\n");

	return EXIT_SUCCESS;
}

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

int build_info_word(int value, int addressing_method, int type) {
	int info_word = 0, ARE;

	info_word = (value << 3);

	switch (addressing_method) {
	case IMMEDIATE:
		ARE = ARE_ABSOLUTE; /*  (A=1, R=0, E=0) */

		break;
	case DIRECT:
		ARE = (type == EXTERNAL) ? ARE_EXTERNAL : ARE_RELOCATEABLE; /* 001 for external addresses, 010 for internal addresses */
		break;
	case RELATIVE:
		ARE = ARE_ABSOLUTE; /* (A=1, R=0, E=0) */
		break;
	default:
		return FAIL_CODE; /* Generic error code as there shouldn't be any errors heres */
		break;
	}

	info_word = info_word | ARE;

	return info_word;
}

int process_operand(char *operand, int addressing, int immediate_value, hashmap_t *sym_tb, FILE *file_ob, int *ic, FILE *file_ext) {
	int value, type = NO_ATTR, word;
	Symbol *sym;
	char *cur_operand = clean_arg(operand);
	if (cur_operand) {
		/* Lookup the symbol */
		sym = (Symbol *)lookup(sym_tb, cur_operand);
		if (!sym) {
			free(cur_operand);
			return SYMBOL_NOT_FOUND_DURING_BUILD;
		}
		value = sym->value;
		type = sym->entry_or_extern;
	} else {
		/* Use immediate value if no symbol is provided */
		value = immediate_value;
		addressing = IMMEDIATE;
	}
	/* Specific check for relative adressing */
	if (addressing == RELATIVE) {
		value = (value - (*ic - 1)); /* removing 1 from ic to return to the instruction's adress */
	}

	/* writing into the externs file */
	if (type == EXTERNAL) {
		fprintf(file_ext, "%s %07d\n", cur_operand, *ic);
		fflush(file_ext);
	}

	word = build_info_word(value, addressing, type);
	fprintf(file_ob, "%07d %06x\n", *ic, word & 0xFFFFFF);
	fflush(file_ob);

	free(cur_operand);
	return SUCCESS_CODE;
}

int build_binary_instruction(FirstInstruction *code, hashmap_t *sym_tb, FILE *file_ob, FILE *file_ext, int *ic) {
	int word, ret;

	if (!sym_tb || !file_ob) return FAIL_CODE;
	if (!code) return SUCCESS_CODE;

	/* Building the first word */
	word = build_instruction_word(code->opcode, code->src_addressing,
								  code->src_register, code->dest_addressing,
								  code->dest_register, code->funct, code->are);

	fprintf(file_ob, "%07d %06x\n", *ic, word & 0xFFFFFF);
	fflush(file_ob);

	/* Process source operand if required */
	if (OPCODES[code->index].is_source && code->src_addressing != REGISTER_DIRECT) {
		(*ic)++;
		ret = process_operand(code->src_operand, code->src_addressing, code->immediate_value, sym_tb, file_ob, ic, file_ext);
		if (ret != SUCCESS_CODE)
			return ret;
	}

	/* Process destination operand if required */
	if (OPCODES[code->index].is_dest && code->dest_addressing != REGISTER_DIRECT) {
		(*ic)++;
		ret = process_operand(code->dest_operand, code->dest_addressing, code->immediate_value, sym_tb, file_ob, ic, file_ext);
		if (ret != SUCCESS_CODE)
			return ret;
	}

	return SUCCESS_CODE;
}

void write_symbols_to_files(hashmap_t *sym_tb, FILE *file_ent) {
	int i;
	int capacity = 128;
	int count = 0;
	Symbol **symbols, *sym;
	HashNode *node;

	if (!sym_tb) return;

	symbols = malloc(capacity * sizeof(Symbol *));
	if (!symbols) {
		printf(" !!! Memory error when writing the entry file !!! \n");
		return;
	}

	/* Collect all Symbol* pointers into array */
	for (i = 0; i < sym_tb->size; i++) {
		node = sym_tb->table[i];
		while (node) {
			if (count == capacity) {
				capacity *= 2;
				symbols = realloc(symbols, capacity * sizeof(Symbol *));
				if (!symbols) {
					printf(" !!! Memory error when writing the entry file !!! \n");
					return;
				}
			}
			symbols[count++] = (Symbol *)node->value;
			node = node->next;
		}
	}

	/* Sort by symbol value */
	qsort(symbols, count, sizeof(Symbol *), compare_symbols_by_value);

	/* Write sorted symbols */
	for (i = 0; i < count; i++) {
		sym = symbols[i];
		if (sym->entry_or_extern == ENTRY) {
			fprintf(file_ent, "%s %07d\n", sym->name, sym->value);
		}
	}
	free(symbols);
}
