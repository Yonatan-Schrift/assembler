#ifndef FIRST_PASS_H
#define FIRST_PASS_H

#include "globals.h"
#include "hashmap.h"
#include "line.h"

/* Constants */
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

/* Types of symbol, starting from 10 to differ from other values */
enum SymbolType {
	DATA = 10,
	CODE,
	EXTERNAL,
	ENTRY,
	NO_ATTR
};

typedef struct FirstInstruction {
	int opcode;
	int src_addressing;
	int src_register;
	int dest_addressing;
	int dest_register;
	int funct;
	int are;

	int index;			 /* The index inside the op-codes array */
	char *src_operand;	 /* if a symbol, the name of the symbol */
	char *dest_operand;	 /* if a symbol, the name of the symbol */
	int immediate_value; /* if immediate addressing is used */
} FirstInstruction;

/**
 * @struct op_code
 * @brief Represents an operation code in the assembly language
 *
 * This structure contains all the information needed to identify and validate
 * an operation code, including its name, numeric codes, argument requirements,
 * and valid addressing methods.
 *
 * @param op_code_name String representing the mnemonic of the operation
 * @param opcode Integer value of the operation code
 * @param funct Function code (sub-operation) within the opcode
 * @param args_num Number of arguments this operation requires
 * @param is_source Flag indicating if this operation can be a source operand
 * @param is_dest Flag indicating if this operation can be a destination operand
 * @param valid_source_method Array indicating which addressing methods are valid for source operands
 * @param valid_dest_method Array indicating which addressing methods are valid for destination operands
 */
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

/**
 * @brief Structure representing a symbol in the assembler.
 *
 * This structure contains all the information needed to define and track a symbol
 * during the assembly process.
 *
 * @param name Pointer to the symbol name string
 * @param entry_or_extern Flag indicating if symbol is an entry or external reference
 * @param attribute The attribute type of the symbol
 * @param value The address or value associated with the symbol
 */
typedef struct Symbol {
	char *name;
	int entry_or_extern;
	int attribute;
	int value;
} Symbol;

/* Declaring the OPCODES array - 16 for the number of operations*/
extern const op_code OPCODES[OPERATION_COUNT];

int first_pass(char *file_path, hashmap_t *mcro_tb);

/**
 * @brief Inserts a new symbol in the symbol table.
 * @note Checks for duplicate symbols, macro conflicts, and reserved names.
 *
 * @param name      The symbol's name.
 * @param attribute The symbol's attribute.
 * @param is_ext    Non-zero if symbol is external.
 * @param value     The symbol's value.
 * @param sym_tb    Pointer to the symbol table.
 * @param mcro_tb   Pointer to the macro table.
 *
 * @return SUCCESS_CODE on success; error code otherwise.
 */
int insert_symbol(char *name, int attribute, int is_ext, int value, hashmap_t *sym_tb, hashmap_t *mcro_tb);

/**
 * @brief Adds a data word to the data image.
 * @note If the current data capacity is exceeded, the function doubles the capacity
 *       of the data image using realloc.
 *
 * @param value      The integer value to add.
 * @param data_cap   Pointer to the current capacity of the data image.
 * @param data_image Pointer to the data image (array of integers).
 *
 * @return EXIT_SUCCESS on success; EXIT_FAILURE on memory allocation failure.
 */
int add_data_word(int value, int *data_cap, int **data_image);

/**
 * @brief Adds a string as data words to the data image.
 * @note Removes quotes from the string, converts each character to its integer value, and adds it to the data image.
 *       A null terminator is also added. The input string is freed before returning.
 *
 * @param string     The input string to process.
 * @param data_cap   Pointer to the current capacity of the data image.
 * @param data_image Pointer to the data image (array of integers).
 *
 * @return EXIT_SUCCESS on success; EXIT_FAILURE if adding any data word fails.
 */
int add_string_word(char *string, int *data_cap, int **data_image);

/**
 * @brief Adds an instruction to the machine code array.
 * @note This function checks for null inputs, ensures sufficient capacity for the machine code array,
 *       finds the opcode index, allocates and initializes a new instruction, validates its arguments,
 *       and processes the source and destination operands.
 *
 * @param line             Pointer to the Line struct containing the command and arguments.
 * @param machine_code     Pointer to the machine code array (array of FirstInstruction pointers).
 * @param sym_tb           Pointer to the symbol table.
 * @param L                Number of info-words (unused in the current implementation).
 * @param line_num         The current line number (used for error reporting).
 * @param machine_code_size Pointer to the current size of the machine code array.
 *
 * @return 0 on success; non-zero value if an error occurred.
 */
int add_instruction(Line *line, FirstInstruction ***machine_code, hashmap_t *sym_tb, int L, int line_num, int *machine_code_size);

/**
 * @brief Finds the index of an opcode in the OPCODES array.
 *
 * @param string The opcode string to search for.
 *
 * @return The opcode index if found; EXTRA_COMMA_AFTER_COMMAND if the string ends with a comma;
 *         OPCODE_NOT_FOUND if the opcode is not found or if the input is NULL.
 */
int find_in_opcode(char *string);

/**
 * @brief Checks if the provided arguments match the expected count.
 * @note The expected count is taken from OPCODES if a valid index is provided; otherwise, a required count is used.
 *
 * @param args               The arguments array (array of strings).
 * @param index              If not NO_INDEX, the expected count is from OPCODES[index].args_num.
 * @param required_arg_count The required number of arguments if index is NO_INDEX.
 *
 * @return SUCCESS_CODE if the count is as expected;
 *         TOO_MANY_ARGS if there are too many arguments;
 *         MISSING_ARGS if there are too few arguments;
 *         FAIL_CODE if args is NULL.
 */
int check_arg_count(char **args, int index, int required_arg_count);

/**
 * @brief Determines the addressing method for an operand.
 * @note Returns a code corresponding to immediate, register, relative, or direct addressing.
 *
 * @param operand The operand string to evaluate.
 *
 * @return Addressing method code: IMMEDIATE, REGISTER_DIRECT, RELATIVE, or DIRECT.
 *         EXTRA_COMMA_AFTER_PARAM if operand is NULL or empty,
 *         NOT_AN_INT_WHEN_USING_IMMEDIATE if immediate value is invalid.
 */
int find_addressing_method(char *operand);

/**
 * @brief Counts the required number of info words for a list of arguments.
 * @note Each argument using immediate, direct, or relative addressing adds one info word.
 *       Returns an error code if an argument is not a valid label or has an extra comma.
 *
 * @param args Array of argument strings.
 *
 * @return Total info words required, or an error code.
 */
int count_info_words_required(char **args);

/**
 * @brief Updates data symbols in the symbol table by adding ICF to their values.
 * @note Iterates through all buckets of the symbol table. For each symbol with the DATA attribute,
 *       its value is incremented by ICF. Does nothing if the symbol table pointer is NULL.
 *
 * @param sym_tb Pointer to the symbol table.
 * @param ICF    The value to add to each data symbol's value.
 */
void set_data_to_icf(hashmap_t *sym_tb, int ICF);

/**
 * @brief Frees all allocated memory associated with a Symbol structure.
 *
 * @param sym Pointer to the Symbol structure to be freed
 */
void free_symbol(Symbol *sym);

/**
 * @brief Processes an argument to determine its addressing method, register, operand, and value.
 * @note Checks for comma errors, determines if the argument is a register, and handles immediate,
 *       relative, or direct addressing accordingly. Reports errors via printerror().
 *
 * @param arg      The argument string to process.
 * @param line_num The current line number (used for error reporting).
 * @param reg      Pointer to store the register number (if applicable).
 * @param addr     Pointer to store the addressing method code.
 * @param operand  Pointer to store the operand string (if applicable).
 * @param value    Pointer to store the immediate value (if applicable).
 *
 * @return SUCCESS_CODE if processed successfully; FAIL_CODE otherwise.
 */
int process_argument(char *argument, int line_num, int *reg, int *addr, char **operand, int *value);

/**
 * @brief Frees all allocated resources.
 * @note Frees the data image, machine code instructions (and their associated operands),
 *       the symbol table, the macro table, and the line structure.
 *
 * @param data_image        Pointer to the data image array.
 * @param machine_code      Array of pointers to FirstInstruction.
 * @param machine_code_size Number of instructions in the machine code array.
 * @param sym_table         Pointer to the symbol table hashmap.
 * @param mcro_tb           Pointer to the macro table hashmap.
 * @param line              Pointer to the Line structure.
 */
void free_everything(int *data_image, FirstInstruction **machine_code, int machine_code_size, hashmap_t *sym_table, hashmap_t *mcro_tb, Line *line);

/**
 * @brief Compares two symbols by their value.
 * @note Intended for use with sorting functions (e.g., qsort) to order symbols based on their value.
 *
 * @param a Pointer to a pointer to the first Symbol.
 * @param b Pointer to a pointer to the second Symbol.
 *
 * @return A negative integer if the first symbol's value is less than the second's,
 *         zero if they are equal, or a positive integer if the first symbol's value is greater.
 */
int compare_symbols_by_value(const void *a, const void *b);

/**
 * @brief Validates the addressing methods of the given arguments.
 * @note Checks source and destination arguments against the valid methods specified in the OPCODES table.
 *
 * @param args  Array of argument strings.
 * @param index Opcode index used to retrieve valid addressing methods.
 *
 * @return SUCCESS_CODE if all arguments are valid; INCORRECT_PARAM_TYPE if an argument's addressing method is invalid.
 */
int validate_arguments(char **args, int index);

#endif /* FIRST_PASS_H */