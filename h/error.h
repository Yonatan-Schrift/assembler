#ifndef ERROR_H
#define ERROR_H

#include <stdio.h>
#include <stdlib.h>

#define STOP_STRING "STOP"

/* Error and Success codes */
#define SUCCESS_CODE 0
#define FAIL_CODE -1
#define LINE_TOO_LONG -2
#define TOO_MANY_ARGS -3


/* Print an error message and continue execution. */
void printerror(const char *message);

#endif /* ERROR_H */