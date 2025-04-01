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
#include "../h/first_pass.h"
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
	hashmap_t mcro_tb;

	/* Checks if recieved at least 1 input file */
	if (argc < REQ_ARGUMENT_COUNT) {
		printerror(NO_LINE, NO_FILES);
		exit(EXIT_FAILURE);
	}

	for (i = 1; i < argc; i++) {
		printf("\nWorking on file: %s\n", argv[i]);

		if (pre_comp(argv[i], &mcro_tb) != SUCCESS_CODE) {
			printf("\nFile: %s failed pre-proccessing\n", argv[i]);
			error_flag = TRUE;

		} else {
			if (first_pass(argv[i], &mcro_tb) != SUCCESS_CODE) /* First pass goes into second pass */ {
				error_flag = TRUE;
			} else
				printf("\nSuccessfully compiled file %s\n\n", argv[i]);
		}
	}

	return error_flag;
}
