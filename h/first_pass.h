#ifndef FIRST_PASS_H
#define FIRST_PASS_H

#include "globals.h"
#include "hashmap.h"
#include "line.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Types of symbol, starting from 10 to differ from other values */
enum SymbolType {
	DATA = 10,
	CODE,
	EXTERNAL,
	ENTRY,
	NO_ATTR
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

	int L;				 /* useless */
	int index;			 /* The index inside the op-codes array */
	char *src_operand;	 /* if a symbol, the name of the symbol */
	char *dest_operand;	 /* if a symbol, the name of the symbol */
	int immediate_value; /* if immediate addressing is used */
} FirstInstruction;

typedef struct op_code {
	char *op_code_name;
	int opcode;
	int funct;
	int args_num;
	int is_source;
	int is_dest;
	int valid_source_method[MAX_METHODS];
	int valid_dest_method[MAX_METHODS];
} op_code;

typedef struct Symbol {
	char *name;
	int entry_or_extern;
	int attribute;
	int value;
} Symbol;

/* Declaring the OPCODES array - 16 for the number of operations*/
extern const op_code OPCODES[OPERATION_COUNT];

int first_pass(char *file_path, hashmap_t *mcro_tb);

int insert_symbol(char *name, int attribute, int is_ext, int value, hashmap_t *sym_tb, hashmap_t *mcro_tb);

int add_data_word(int value, int *data_cap, int **data_image);

int add_string_word(char *string, int *data_cap, int **data_image);

int add_instruction(Line *line, FirstInstruction ***machine_code, hashmap_t *sym_tb, int L, int line_num, int *machine_code_size);

int find_in_opcode(char *string);

int check_arg_count(char **args, int index, int required_arg_count);

int find_addressing_method(char *operand);

int count_info_words_required(char **args);

void set_data_to_icf(hashmap_t *sym_tb, int ICF);

/**
 * @brief Frees all allocated memory associated with a Symbol structure.
 *
 * @param sym Pointer to the Symbol structure to be freed
 */
void free_symbol(Symbol *sym);

int process_argument(char *argument, int line_num, int *reg, int *addr, char **operand, int *value);

void free_everything(int *data_image, FirstInstruction **machine_code, int machine_code_size, hashmap_t *sym_table, hashmap_t *mcro_tb, Line *line);

void print_symbol(Symbol *sym);

int compare_symbols_by_value(const void *a, const void *b);

int validate_arguments(char **args, int index);

#endif /* FIRST_PASS_H */