#include "../h/error.h"
#include <stdio.h>

#define LINE_TOO_LONG -2
#define TOO_MANY_ARGS -3
#define MISSING_ARGS -4
#define MACRO_ALREADY_EXISTS -5

#define NOT_ALLOWED_MACRO_NAME -10
#define SYMBOL_ALREADY_EXISTS -11
#define SYMBOL_IS_MACRO -12
#define MISSING_SYMBOL_VALUES -13
#define OPCODE_NOT_FOUND -14
#define NOT_A_LABEL -15


#define NO_FILES -20


void printerror(const char *message, int line_num, int error_code) {
	const char *error_msg;

    if (error_code == EXIT_SUCCESS)
        return;
    
    switch (error_code) {
        case LINE_TOO_LONG:
            error_msg = "Line too long";
            break;
        case TOO_MANY_ARGS:
            error_msg = "Too many arguments";
            break;
        case MISSING_ARGS:
            error_msg = "Missing arguments";
            break;
        case MACRO_ALREADY_EXISTS:
            error_msg = "Macro already exists";
            break;
        case NOT_ALLOWED_MACRO_NAME:
            error_msg = "Not allowed macro name";
            break;
        case SYMBOL_ALREADY_EXISTS:
            error_msg = "Symbol already exists";
            break;
        case SYMBOL_IS_MACRO:
            error_msg = "Symbol is defined as a macro";
            break;
        case MISSING_SYMBOL_VALUES:
            error_msg = "Missing symbol values";
            break;
        case OPCODE_NOT_FOUND:
            error_msg = "Opcode not found";
            break;
        case NOT_A_LABEL:
            error_msg = "Not a valid label";
            break;
        case NO_FILES:
            error_msg = "No files provided";
            break;
        default:
            error_msg = "Unknown error";
    }

    if (line_num == NO_LINE) {
        printf("Error: %s\n", error_msg);
    } else {
        printf("Error in line %d: %s\n", line_num, error_msg);
    }
}
