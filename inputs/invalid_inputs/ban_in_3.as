mov r1, r2
add #abc, r3 ; NOT_AN_INT_WHEN_USING_IMMEDIATE
    .data 5, 10, abc ; UNALLOWED_CHARS_IN_DATA
LABEL1: mov r1, r2
LABEL1: mov r1, r2 ; SYMBOL_ALREADY_EXISTS
MACRO1: .macro
MACRO1: .macro ; SYMBOL_IS_MACRO
: mov r1, r2 ; EMPTY_LABEL
mov r1 ; MISSING_ARGS
add r1, r2, r3 ; INCORRECT_PARAM_TYPE
MACRO: .macro ; MCRO_DEFINITION_WITH_NO_NAME
    .string "This is a very long line that exceeds the maximum allowed length for a single line in the assembler and should trigger an error for being too long." ; LINE_TOO_LONG
    .data , 5, 10 ; EXTRA_COMMA_AFTER_COMMAND
LABEL2: .macro ; MCRO_CANNOT_BE_A_LABEL