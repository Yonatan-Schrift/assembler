#include "../h/error.h"
#include <stdio.h>

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
        case FILE_INCLUDES_EXTENSION:
            error_msg = "File includes an extension, the input should not have an extension";
            break;
        case COULD_NOT_OPEN_FILE:
            error_msg = "Could not open the file, check that it exists";
            break;
        case MACRO_NAME_TOO_LONG:
            error_msg = "Macro name too long ";
            break;
        case NAME_DOESNT_START_WITH_LETTER:
            error_msg = "Macro name doesn't start with a letter or underscore";
            break;
        case RESERVED_MACRO_NAME:
            error_msg = "Macro using reserved name";
            break;
        case SYMBOL_ALREADY_EXISTS:
            error_msg = "Symbol already exists";
            break;
        case SYMBOL_IS_MACRO:
            error_msg = "Symbol is already defined as a macro";
            break;
        case MISSING_SYMBOL_VALUES:
            error_msg = "Missing symbol values";
            break;
        case OPCODE_NOT_FOUND:
            error_msg = "Command not found";
            break;
        case NOT_A_LABEL:
            error_msg = "Not a valid label";
            break;
        case OUT_OF_MEMORY:
            error_msg = "Out of memory";
            break;
        case EMPTY_LABEL:
            error_msg = "Empty label is not allowed";
            break;
        case ENTRY_SYMBOL_NOT_FOUND:
            error_msg = "Entry symbol not found";
            break;
        case SYMBOL_NOT_FOUND_DURING_BUILD:
            error_msg = "Symbol not found during build";
            break;
        case NO_FILES:
            error_msg = "No files provided";
            break;
        case MACRO_USING_UNALLOWED_LETTERS:
            error_msg = "Macro uses unallowed characters";
            break;
        case MISSING_COMMA:
            error_msg = "Missing closing quote";
            break;
        default:
            error_msg = "Unknown error";
    }

    if (line_num == NO_LINE) {
        printf(">>> Error: %s\n", error_msg);
    } else {
        printf(">>> Error in line %d: %s\n", line_num, error_msg);
    }
}
