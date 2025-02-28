#include <stdio.h>
#include "../h/error.h"

void printerror(const char *message, int line_num) {
    fprintf(stderr, "Error in line %d: %s\n", line_num, message);
}