
#ifndef GLOBALS_H
#define GLOBALS_H

#define MAX_LINE_LENGTH 81
#define MAX_ARGS 2

#define TRUE 1
#define FALSE 0

#define STRCMP_SUCCESS 0

#define OP_NAMES {"mov", "cmp", "add", "sub", "lea", "clr", "not", "inc", "dec", "jmp", "bne", "jsr", "red", "prn", "rts", "stop"} 
#define INSTRUCTIONS {".data", ".string", ".extern", ".entry"}
#define REGISTERS {"r0", "r1", "r2", "r3", "r4", "r5", "r6", "r7"}

/* Error and Success codes */

#define SUCCESS_CODE 0
#define FAIL_CODE -1
#define LINE_TOO_LONG -2
#define TOO_MANY_ARGS -3

#include "error.h"





#endif /* GLOBALS_H */
