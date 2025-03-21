

#include "../h/second_pass.h"
#include "../h/first_pass.h"
#include "../h/globals.h"
#include "../h/hashmap.h"
#include "../h/line.h"

#define COMPARE_STR(a, b) (strcmp(a, b) == STRCMP_SUCCESS)
/* temp macro */

int second_pass(char *src_path) {
	char line[MAX_LINE_LENGTH + 1];
	FILE *file_in, *file_ob;
	Line parsed_line;
	int error_flag = FALSE, current_error = FALSE, is_symbol = FALSE;
	int line_count = 0, i, value, opcode_index, L;
	hashmap_t sym_table;
	int is_entry = FALSE;

	file_in = open_file(src_path, ".am", READ_MODE);
	file_ob = open_file(src_path, ".ob", WRITE_MODE);
	if (!file_in || !file_ob) {
		close_mult_files(file_in, file_ob, NULL, NULL, NULL, NULL);
		return FAIL_CODE;
	}
	init_line(&parsed_line);

	while ((current_error = read_line(file_in, line)) != EXIT_FAILURE) {
		if (line_count > 0) free_line(&parsed_line);
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

			/* Stage 2 */
			if (parsed_line.label != NULL) {
				continue;
			}

			/* Stage 3 */
			if (!COMPARE_STR(parsed_line.command, ".data") &
				!COMPARE_STR(parsed_line.command, ".string") &
				!COMPARE_STR(parsed_line.command, ".extern")) {

				/* Stage 4 */
				if (COMPARE_STR(parsed_line.command, ".entry")) {
					is_entry = TRUE;

					/*need to do stage 5 in this if */
				}
			}
		}

		return EXIT_SUCCESS;
	}
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
	instr_word = build_instruction_word(find_in_opcode(line->command), 0, 0, 0, 0, 0, 0);
	for (i = 0; i < L; i++) {
		add_method = find_addressing_method(line->arguments[i], sym_tb);
		if (add_method == IMMEDIATE) {
			return instr_word; /* UNUSED RN */
		}
	}
	return L;
}
