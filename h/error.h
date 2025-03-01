#ifndef ERROR_H
#define ERROR_H

#include <stdio.h>
#include <stdlib.h>

#define STOP_STRING "STOP\n"  /* A string used for stopping the program when encountering errors. */
#define NO_LINE -1            /* A used when an error has no specific line */

/* Error and Success codes */
#define SUCCESS_CODE 0
#define FAIL_CODE -1
#define LINE_TOO_LONG -2
#define TOO_MANY_ARGS -3
#define NOT_ALLOWED_MACRO_NAME -4
#define REDEFINE_SYMBOL -5

/* Print an error message and continue execution. */
void printerror(const char *message, int line_num, int error_code);

#endif /* ERROR_H */