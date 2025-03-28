

#include "../h/second_pass.h" /*  */
#include "../h/file_funcs.h"  /* used for open_file, close_mult_files and WRITE_MODE */
#include "../h/first_pass.h"  /* Used for symbol and instructions */
#include "../h/globals.h"	  /* globals */
#include "../h/hashmap.h"	  /* used as the data structure for the symbol table */
#include "../h/line.h"		  /* Used for processing lines in the source */

#define COMPARE_STR(a, b) (strcmp(a, b) == STRCMP_SUCCESS)
/* temp macro */

int second_pass(char *src_path, hashmap_t *sym_tb, int *data_image, FirstInstruction **machine_code, int machine_code_size, int ICF, int DCF) {
	char line[MAX_LINE_LENGTH + 1];
	FILE *file_ob, *file_ent, *file_ext, *file_am;
	Line parsed_line;
	Symbol *sym;
	int error_flag = FALSE, current_error = FALSE;
	int line_count = 0, arg_count, binary_code, i;

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
	fprintf(file_ob, "%d %d", ICF - 100, DCF);

	while ((current_error = read_line(file_am, line)) != EXIT_FAILURE) {
		/* For iterations after the first, free the memory allocated for the previous line.
		 * (On the first iteration, no memory has been allocated yet.) */
		if (line_count > 0) free_line(&parsed_line);
		/* Reinitialize parsed_line to prepare for processing the next line. */
		init_line(&parsed_line);

		line_count++;

		if (current_error < SUCCESS_CODE) {
			printerror("error_flag", line_count, current_error);
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
					printerror("current error", line_count, current_error);
					error_flag = TRUE;
					continue;
				}

				if ((sym = (Symbol *)lookup(sym_tb, parsed_line.arguments[0]))) {
					sym->attribute = ENTRY; /* setting the attribute to ENTRY */
				} else {
					printerror("Symbol not found", line_count, ENTRY_SYMBOL_NOT_FOUND);
					error_flag = TRUE;
					continue;
				}
			}

			
		}
		if (error_flag) {
			free_everything(data_image, machine_code, machine_code_size, sym_tb, NULL, &parsed_line);
		}

		/* finish the binary coding */
		for (i = 0; i < machine_code_size; i++) {
			binary_code = build_binary_instruction(NULL, NULL, NULL);
			fprintf(file_ob, "%06X\n", binary_code);
		}
	}

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

int build_info_word(int address, int addressing_method, int type) {
	int info_word = 0, ARE;

	info_word = (address << 3);

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

int build_binary_instruction(FirstInstruction *machine_code_line, hashmap_t *sym_tb, FILE *file_ob) {
	int word, i;
	/* 	int opcode;
	int src_addressing;
	int src_register;
	int dest_addressing;
	int dest_register;
	int funct;
	int are;
	int L; */

	/* Building the first word */
	word = build_instruction_word(machine_code_line->opcode, machine_code_line->src_addressing,
								  machine_code_line->src_register, machine_code_line->dest_addressing,
								  machine_code_line->dest_register, machine_code_line->funct, machine_code_line->are);

	fprintf(file_ob, "%06X\n", word);

	for(i = 0; i <= machine_code_line->L; i++) {
		build_info_word();
	}

	return SUCCESS_CODE;
}
