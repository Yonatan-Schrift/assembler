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
#define MACRO_ALREADY_EXISTS -5
#define FILE_INCLUDES_EXTENSION -6
#define COULD_NOT_OPEN_FILE -7
#define NAME_TOO_LONG -8
#define NAME_DOESNT_START_WITH_LETTER -9

#define RESERVED_NAME -10
#define SYMBOL_ALREADY_EXISTS -11
#define SYMBOL_IS_MACRO -12
#define MISSING_SYMBOL_VALUES -13
#define OPCODE_NOT_FOUND -14
#define NOT_A_LABEL -15
#define OUT_OF_MEMORY -16
#define EMPTY_LABEL -17
#define ENTRY_SYMBOL_NOT_FOUND -18
#define SYMBOL_NOT_FOUND_DURING_BUILD -19

#define NO_FILES -20
#define NAME_USING_UNALLOWED_LETTERS -21
#define MISSING_COMMA -22

#define MISSING_CLOSING_QUOTE -23
#define EXTRA_QUOTATION_MARK -24
#define MISSING_OPENING_QUOTE -25
#define EXTRA_TEXT_AFTER_STRING -26
#define INITIALIZING_EXTERN -27
#define EXTRA_COMMA_AFTER_PARAM -28
#define EXTRA_COMMA_BEFORE_PARAM -29


/* Print an error message and continue execution. */
void printerror(const char *message, int line_num, int error_code);

#endif /* ERROR_H */