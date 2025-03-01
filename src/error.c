#include "../h/error.h"
#include <stdio.h>

void printerror(const char *message, int line_num, int error_code) {
	if (error_code == EXIT_SUCCESS)
		return;
	
	if (line_num == NO_LINE) {
		printf("Error: %s\n", message);
	} else {
		printf("Error in line %d: %s\n", line_num, message);
	}
}