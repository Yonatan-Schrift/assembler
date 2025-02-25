#include <stdio.h>
#include "../h/error.h"

void printerror(const char *message) {
    fprintf(stderr, "Error: %s\n", message);
}