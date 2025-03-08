#ifndef ERROR_H
#define ERROR_H

#include <stdio.h>
#include <stdlib.h>

#define ERROR_STRING "ERROR\n"  /* A string used for alerting the program when encountering errors. */
#define NO_LINE -1            /* A used when an error has no specific line */

/* Error and Success codes */
#define SUCCESS_CODE 0
#define FAIL_CODE -1

#define LINE_TOO_LONG -2
#define TOO_MANY_ARGS -3
#define MISSING_ARGS -4

#define NOT_ALLOWED_MACRO_NAME -10
#define SYMBOL_ALREADY_EXISTS -11
#define SYMBOL_IS_MACRO -12
#define MISSING_SYMBOL_VALUES -13
#define OPCODE_NOT_FOUND -14
#define NOT_A_LABEL -15


#define NO_FILES -20


/* Print an error message and continue execution. */
void printerror(const char *message, int line_num, int error_code);

#endif /* ERROR_H */