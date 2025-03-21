#ifndef FIRST_PASS_H
#define FIRST_PASS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hashmap.h"
#include "line.h"

#define DATA "data"
#define CODE "code"
#define EXTERNAL "external"
#define ENTRY "entry"

#define IMMEDIATE 0
#define DIRECT 1
#define RELATIVE 2
#define REGISTER_DIRECT 3

#define ARE_ABSOLUTE 4

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
    int is_source;
    int is_dest;
} op_code;

typedef struct Symbol {
    char *name;
    char *attribute;
    int value;
} Symbol;

int first_pass(char *file_path, hashmap_t *mcro_tb);

int insert_symbol(char *name, char *attribute, int value, hashmap_t *sym_tb, hashmap_t *mcro_tb);

int add_data_word(int value, int *data_cap, int **data_image);

int add_string_word(char *string, int *data_cap, int **data_image);

int add_instruction(Line *line, FirstInstruction ***machine_code, hashmap_t *sym_tb, int L, int line_num, int *machine_code_size);

int find_in_opcode(char *string); 

int check_arg_count(char **args, int index);

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

void set_data_to_icf(hashmap_t *sym_tb, int ICF);

/**
 * @brief Frees all allocated memory associated with a Symbol structure.
 * 
 * @param sym Pointer to the Symbol structure to be freed
 */
void free_symbol (Symbol *sym);

/**
 * @brief Process an argument to determine its type (register or immediate/direct/index) 
 * and addressing method.
 *
 * @param argument The argument string to process
 * @param sym_tb Pointer to the symbol table
 * @param line_num Current line number in the assembly source file
 * @param reg Pointer to store register number (if applicable, otherwise 0)
 * @param addr Pointer to store addressing method code or error code
 * 
 * @return SUCCESS_CODE if processing was successful, FAIL_CODE if an error occurred
 */
int process_argument(char *argument, hashmap_t *sym_tb, int line_num, int *reg, int *addr);

void free_everything(int *data_image, FirstInstruction **machine_code, int machine_code_size, hashmap_t *sym_table, hashmap_t *mcro_tb, Line *line);

void print_symbol(Symbol *sym);

#endif /* FIRST_PASS_H */