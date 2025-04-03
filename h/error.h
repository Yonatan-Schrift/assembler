#ifndef ERROR_H
#define ERROR_H

/* Constants */
#define NO_LINE -1              /* Used when an error has no specific line */

/* Error codes */
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
#define EXTRA_COMMA_AFTER_COMMAND -30
#define UNALLOWED_CHARS_IN_DATA -31
#define EXTRA_COMMA_AFTER_PARAM_PRE_ASS -32
#define INCORRECT_PARAM_TYPE -33
#define MCRO_CANNOT_BE_A_LABEL -34
#define EXTRA_TEXT_AFTER_MCRO -35
#define MCRO_DEFINITION_WITH_NO_NAME -36
#define NOT_AN_INT_WHEN_USING_IMMEDIATE -37

/**
 * @brief Prints an error message based on the error code
 * 
 * This function takes an error code and prints a corresponding human-readable error message.
 * If the line number is provided, it includes it in the error message.
 * If the error code is 0, the function returns without printing anything.
 * 
 * @param line_num The line number where the error occurred. Use NO_LINE if not applicable.
 * @param error_code The error code identifying the type of error.
 * 
 * @return void
 * @note the method calling printerror continues execution afterwards.
 */
void printerror(int line_num, int error_code);

#endif /* ERROR_H */