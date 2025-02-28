#include "../h/error.h"
#include <stdio.h>

void printerror(const char *message, int line_num) {
	if (line_num == NO_LINE) {
		printf("Error: %s\n", message);
	} else {
		printf("Error in line %d: %s\n", line_num, message);
	}
}