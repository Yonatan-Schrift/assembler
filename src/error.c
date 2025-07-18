#include "../h/error.h"
#include <stdio.h> /* used for printf */

void printerror(int line_num, int error_code) {
	const char *error_msg;

    if (error_code == 0)
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
        case NAME_TOO_LONG:
            error_msg = "Macro name too long ";
            break;
        case NAME_DOESNT_START_WITH_LETTER:
            error_msg = "Macro / Symbol name doesn't start with a letter or underscore";
            break;
        case RESERVED_NAME:
            error_msg = "Macro / Symbol using reserved name";
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
        case NAME_USING_UNALLOWED_LETTERS:
            error_msg = "Macro / Label uses unallowed characters";
            break;
        case MISSING_COMMA:
            error_msg = "Missing comma between arguments";
            break;
        case MISSING_CLOSING_QUOTE:
            error_msg = "Missing closing quote";
            break;
        case EXTRA_QUOTATION_MARK:
            error_msg = "Extra quotation mark";
            break;
        case MISSING_OPENING_QUOTE:
            error_msg = "Missing opening quote";
            break;
        case EXTRA_TEXT_AFTER_STRING:
            error_msg = "Extra text after quote end";
            break;
        case INITIALIZING_EXTERN:
            error_msg = "Cannot give value to external symbol";
            break;
        case EXTRA_COMMA_AFTER_PARAM:
            error_msg = "Extra comma after parameter";
            break;
        case EXTRA_COMMA_BEFORE_PARAM:
            error_msg = "Extra comma before parameter";
            break;
        case EXTRA_COMMA_AFTER_COMMAND:
            error_msg = "Extra comma after command";
            break;
        case UNALLOWED_CHARS_IN_DATA:
            error_msg = "Unallowed characters in .data directive, expected numbers";
            break;
        case EXTRA_COMMA_AFTER_PARAM_PRE_ASS:
            error_msg = "Extra comma after parameter - found during the pre-assembler";
            break;
        case MCRO_CANNOT_BE_A_LABEL:
            error_msg = "A macro definition cannot be a label";
            break;
        case EXTRA_TEXT_AFTER_MCRO:
            error_msg = "Extra text after macro definition / end";
            break;
        case MCRO_DEFINITION_WITH_NO_NAME:
            error_msg = "Macro definition with no name";
            break;
        case NOT_AN_INT_WHEN_USING_IMMEDIATE:
            error_msg = "Not a digit when using #";
            break;
        case INCORRECT_PARAM_TYPE:
            error_msg = "Incorrect parameter type";
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
