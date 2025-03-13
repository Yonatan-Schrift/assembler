#ifndef FIRST_PASS_H
#define FIRST_PASS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hashmap.h"

#define DATA "data"
#define CODE "code"
#define EXTERNAL "external"
#define ENTRY "entry"

#define IMMEDIATE 0
#define DIRECT 1
#define RELATIVE 2
#define REGISTER_DIRECT 3


#define INITIAL_ARRAY_SIZE 128

typedef struct FirstInstruction {
    int opcode;
    int src_addressing;
    int src_register;
    int dest_addressing;
    int dest_register;
    int funct;
    int are;
    int L;
} FirstInstruction;

typedef struct op_code {
	char *op_code_name;
	int opcode;
	int funct;
	int args_num;
} op_code;

typedef struct Symbol {
    char *name;
    char *attribute;
    int value;
} Symbol;

/* Setting IC and DC as globals */
extern int IC;
extern int DC;

int first_pass(char *file_path, hashmap_t *mcro_tb);

int insert_symbol(char *name, char *attribute, int value, hashmap_t *sym_tb, hashmap_t *mcro_tb);

int add_data_word(int value, int *data_cap, int **data_image);

int find_in_opcode(char *string); 

/**
 * @brief Determines the addressing method of an operand.
 * 
 * This function analyzes the given operand string and determines its addressing method
 * based on the following criteria:
 * - 0: Immediate addressing (operand starts with '#')
 * - 1: Direct addressing (operand exists in the symbol table)
 * - 2: Relative addressing (operand starts with '&' and the rest exists in the symbol table)
 * - 3: Direct register addressing (operand is a valid register)
 * 
 * @param operand The operand string to analyze
 * @param sym_tb A hash map containing the symbol table
 * @return The addressing method code (0-3) or FAIL_CODE if the addressing method is invalid
 */
int find_addressing_method(char *operand, hashmap_t *sym_tb);

int count_info_words_required(char **args, hashmap_t *sym_tb);

int build_instruction_word(int opcode, int source_addressing, int source_register, int des_addressing, int des_register, int funct, int are);

#endif /* FIRST_PASS_H */