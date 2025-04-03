
#include "../h/assembler.h"
#include "../h/first_pass.h"
#include "../h/globals.h"
#include "../h/pre_assembler.h"
#include <stdio.h>

int main(int argc, char *argv[]) {
	int i, error_flag = FALSE;
	hashmap_t mcro_tb;
	
	/* Checks if recieved at least 1 input file */
	if (argc < REQ_ARGUMENT_COUNT) {
		printerror(NO_LINE, NO_FILES);
		exit(EXIT_FAILURE);
	}

	/* for each file run pre-assember, first and second pass */
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
