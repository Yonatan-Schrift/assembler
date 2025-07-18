
#ifndef GLOBALS_H
#define GLOBALS_H

#include "error.h"

#define MAX_LINE_LENGTH 81
#define MAX_ARGS 2

/* Memory Limit - 2^21 */
#define MAX_MEMORY 2097152
#define OPERATION_COUNT 16

#define TRUE 1
#define FALSE 0

#define STRCMP_SUCCESS 0

#define NO_ARG_COUNT 999
#define NO_INDEX 998

#define OP_NAMES {"mov", "cmp", "add", "sub", "lea", "clr", "not", "inc", "dec", "jmp", "bne", "jsr", "red", "prn", "rts", "stop", NULL}
#define INSTRUCTIONS {"data", "string", "extern", "entry", NULL}
#define REGISTERS {"r0", "r1", "r2", "r3", "r4", "r5", "r6", "r7", NULL}


#endif /* GLOBALS_H */