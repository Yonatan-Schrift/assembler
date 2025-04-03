#ifndef SECOND_PASS_H
#define SECOND_PASS_H

#include "first_pass.h" /* used for FirstInstruction type */
#include "hashmap.h"	/* used for hashmap_t type */

#define REQUIRED_ARGS_FOR_DIRECTIVE 1

#define COMPARE_STR(a, b) (strcmp(a, b) == STRCMP_SUCCESS)

/**
 * @brief Performs the second pass of the assembly process.
 * @note Processes the intermediate file (.am) to handle .entry directives, build the binary instructions
 *       for the object file, and write the data image. Internal comments have been added inside the method.
 *
 * @param src_path          The source file path (without extension).
 * @param sym_tb            Pointer to the symbol table.
 * @param data_image        The data image array.
 * @param data_size         The size of the data image.
 * @param machine_code      Array of machine code instructions.
 * @param machine_code_size The number of machine code instructions.
 * @param ICF               The final instruction counter.
 * @param DCF               The final data counter.
 *
 * @return EXIT_SUCCESS on success; otherwise, an error code.
 */
int second_pass(char *src_path, hashmap_t *sym_tb, int *data_image, int data_size, FirstInstruction **machine_code, int machine_code_size, int ICF, int DCF);

/**
 * @brief Builds an instruction word by combining opcode, addressing modes, registers, function, and ARE.
 * @note Internal comments have been added inside the method.
 *
 * @param opcode             The opcode field.
 * @param source_addressing  The addressing mode for the source operand.
 * @param source_register    The register number for the source operand.
 * @param des_addressing     The addressing mode for the destination operand.
 * @param des_register       The register number for the destination operand.
 * @param funct              The function field.
 * @param are                The ARE bits.
 *
 * @return The constructed instruction word.
 */
int build_instruction_word(int opcode, int source_addressing, int source_register, int des_addressing, int des_register, int funct, int are);

/**
 * @brief Builds an information word by shifting a value and appending ARE bits based on the addressing method.
 * @note Internal comments have been added inside the method. An error code is returned for an unrecognized addressing method.
 *
 * @param value              The numeric value to include in the info word.
 * @param addressing_method  The addressing method (IMMEDIATE, DIRECT, or RELATIVE).
 * @param type               The type (used when addressing_method is DIRECT) to determine external or relocatable.
 *
 * @return The constructed info word, or an error code if addressing_method is invalid.
 */
int process_operand(char *operand, int addressing, int immediate_value, hashmap_t *sym_tb, FILE *file_ob, int *ic, FILE *file_ent);

/**
 * @brief Builds an information word by shifting a value and appending ARE bits based on the addressing method.
 * @note Internal comments have been added inside the method. An error code is returned for an unrecognized addressing method.
 *
 * @param value              The numeric value to include in the info word.
 * @param addressing_method  The addressing method (IMMEDIATE, DIRECT, or RELATIVE).
 * @param type               The type (used when addressing_method is DIRECT) to determine external or relocatable.
 *
 * @return The constructed info word, or an error code if addressing_method is invalid.
 */
int build_info_word(int address, int addressing_method, int type);

/**
 * @brief Processes an operand during the build phase.
 * @note Cleans the operand, looks up its symbol value if applicable, adjusts the value for relative addressing,
 *       writes extern entries, builds the info word, and writes it to the object file.
 *
 * @param operand        The operand string (could be a symbol or an immediate value).
 * @param addressing     The addressing mode (e.g., IMMEDIATE, DIRECT, RELATIVE).
 * @param immediate_value The immediate value to use if the operand is not a symbol.
 * @param sym_tb         Pointer to the symbol table hashmap.
 * @param file_ob        File pointer to the object file for writing the machine code.
 * @param ic             Pointer to the current instruction counter.
 * @param file_ext       File pointer to the extern file for recording external symbol references.
 *
 * @return SUCCESS_CODE on success; otherwise, an error code.
 */
int process_operand(char *operand, int addressing, int immediate_value, hashmap_t *sym_tb, FILE *file_ob, int *ic, FILE *file_ext);

/**
 * @brief Builds a binary instruction and writes it to the object file.
 * @note Constructs the instruction word using build_instruction_word and writes it.
 *       Processes the source and destination operands if needed (skipping register direct addressing)
 *       and writes them using process_operand. Internal comments have been added inside the method.
 *
 * @param code     Pointer to the FirstInstruction structure.
 * @param sym_tb   Pointer to the symbol table.
 * @param file_ob  File pointer to the object file.
 * @param file_ext File pointer to the extern file.
 * @param ic       Pointer to the instruction counter.
 *
 * @return SUCCESS_CODE on success; otherwise, an error code.
 */
int build_binary_instruction(FirstInstruction *code, hashmap_t *sym_tb, FILE *file_ob, FILE *file_ext, int *ic);

/**
 * @brief Writes entry symbols from the symbol table to the entry file.
 * @note Collects all Symbol pointers from the symbol table into an array, sorts them by their value,
 *       and writes symbols with the ENTRY attribute to the provided entry file.
 *       Internal comments have been added inside the method.
 *
 * @param sym_tb  Pointer to the symbol table.
 * @param file_ent File pointer for the entry file.
 */
void write_symbols_to_files(hashmap_t *sym_tb, FILE *file_ent);

#endif /* SECOND_PASS_H */