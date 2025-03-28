#ifndef SECOND_PASS_H
#define SECOND_PASS_H

#include "hashmap.h" /* used for hashmap_t type */
#include "first_pass.h" /* used for FirstInstruction type */

#define REQUIRED_ARGS_FOR_DIRECTIVE 1

int second_pass(char *src_path, hashmap_t *sym_tb, int *data_image, FirstInstruction **machine_code, int machine_code_size, int ICF, int DCF);

int build_instruction_word(int opcode, int source_addressing, int source_register, int des_addressing, int des_register, int funct, int are);

int build_info_word(int address, int addressing_method, int type);

#endif /* SECOND_PASS_H */