/**
 * Assembler
 *
 * @file assembler.c
 * @brief currently for testing I ain't sure what it's gonna do
 *
 * @author Yonatan Schrift
 * @date Created 31.01.2025
 */

#include "../h/file_funcs.h"
#include "../h/globals.h"
#include "../h/pre_assembler.h"
#include <stdio.h>

#define REQ_ARGUMENT_COUNT 2

/**
 * @brief Entry point for the assembler.
 *
 * This function processes command line arguments, opens the file,
 * and initiates the pre-assembly phase.
 *
 * @param argc The number of command line arguments.
 * @param argv Array of command line arguments.
 * @return int Returns SUCCESS_CODE on success or FAIL_CODE on failure.
 */
int main(int argc, char *argv[]) {
	int i, error_flag = FALSE;

	/* Checks if recieved at least 1 input file */
	if (argc < REQ_ARGUMENT_COUNT) {
		printerror("MISSING FILE TO PROCESS\n", NO_LINE);
		exit(EXIT_FAILURE);
	}

	if (strcmp(find_extension(argv[1]), "as") != STRCMP_SUCCESS) {
		printf("Not a .as file.\n");
		return FAIL_CODE;
	}

	for (i = 1; i < argc; i++) {
		if (pre_comp(argv[i]))
			error_flag = TRUE;
	}

	return error_flag;
}
