#ifndef FIRST_PASS_H
#define FIRST_PASS_H

#include "hashmap.h"
#include "line.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum SymbolType {
	DATA,
	CODE,
	EXTERNAL,
	ENTRY
};

#define IMMEDIATE 0
#define DIRECT 1
#define RELATIVE 2
#define REGISTER_DIRECT 3


#define ARE_ABSOLUTE 4
#define ARE_EXTERNAL 1
#define ARE_RELOCATEABLE 2

#define INITIAL_ARRAY_SIZE 128
#define IC_START 100
#define MAX_METHODS 3
#define EMPTY_VALUE -1

typedef struct FirstInstruction {
	int opcode;
	int src_addressing;
	int src_register;
	int dest_addressing;
	int dest_register;
	int funct;
	int are;
	
	int L;
	char *src_sym;
} FirstInstruction;

typedef struct op_code {
	char *op_code_name;
	int opcode;
	int funct;
	int args_num;
	int is_source;
	int is_dest;
	int valid_source_method[MAX_METHODS];
	int valid_deest_method[MAX_METHODS];
} op_code;

typedef struct Symbol {
	char *name;
	int attribute;
	int value;
} Symbol;

int first_pass(char *file_path, hashmap_t *mcro_tb);

int insert_symbol(char *name, int attribute, int value, hashmap_t *sym_tb, hashmap_t *mcro_tb);

int add_data_word(int value, int *data_cap, int **data_image);

int add_string_word(char *string, int *data_cap, int **data_image);

/**
 * @brief Adds an instruction to the machine code buffer.
 *
 * This function processes the instruction line, determines the appropriate opcode,
 * addressing methods, and other instruction components. It then builds a FirstInstruction
 * structure and adds it to the machine code buffer.
 *
 * @param line The parsed line containing the command and arguments.
 * @param machine_code Pointer to the machine code buffer array.
 * @param sym_tb Hash map containing the symbol table.
 * @param L The number of info-words for this instruction.
 * @param line_num The current line number in the source code.
 * @param machine_code_size Pointer to the current size of the machine code buffer.
 *
 * @return TRUE if an error was found during processing, FALSE otherwise.
 *
 * @note The function dynamically resizes the machine code buffer if needed.
 * @note IC (Instruction Counter) is a global variable starting at 100.
 */
int add_instruction(Line *line, FirstInstruction ***machine_code, hashmap_t *sym_tb, int L, int line_num, int *machine_code_size);

int find_in_opcode(char *string);

int check_arg_count(char **args, int index, int required_arg_count);

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
void free_symbol(Symbol *sym);

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