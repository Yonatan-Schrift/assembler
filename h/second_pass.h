#ifndef SECOND_PASS_H
#define SECOND_PASS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hashmap.h"
#include "line.h"
#include "file_funcs.h"

int build_instruction_word(int opcode, int source_addressing, int source_register, int des_addressing, int des_register, int funct, int are);

int build_info_word(int address, int addressing_method, char *type);

#endif /* SECOND_PASS_H */