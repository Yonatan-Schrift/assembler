/**
 * Assembler
 *
 * @file main.c
 * @brief currently for testing I ain't sure what it's gonna do
 *
 * @author Yonatan Schrift
 * @date Created 31.01.2025
 */

#include "../h/main.h"
#include "../h/file_funcs.h"
#include "../h/globals.h"
#include "../h/pre_assembler.h"
#include <stdio.h>

#define REQ_ARGUMENT_COUNT 2

/**
 * This method opens a file and reads it line by line.asm
 * Do notice that each line has a max of 80 characters + 1 for null terminator.
 *
 */
int main(int argc, char *argv[]) {
	int i, error_flag = FALSE;

	/* Checks if recieved at least 1 input file */
	if (argc < REQ_ARGUMENT_COUNT) {
		printerror("MISSING ARGUMET\n");
		exit(EXIT_FAILURE);
	}

	if (strcmp(find_extension(argv[1]), AS_EXTENSION) != STRCMP_SUCCESS) {
		printf("Not a .as file.\n");
		return FAIL_CODE;
	}

	for (i = 1; i < argc; i++) {
		if (pre_comp(argv[i]))
			error_flag = TRUE;
	}

	return error_flag;
}
