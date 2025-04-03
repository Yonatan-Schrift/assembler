
#ifndef ASSEMBLER_H
#define ASSEMBLER_H

/**
 * @brief Main entry point for the assembler program
 * 
 * The assembler processes each input file through preprocessing and compilation phases.
 * For each file:
 * 1. Performs pre-compilation (macro expansion) via pre_comp()
 * 2. If successful, performs first pass of assembly via first_pass()
 * 3. The first pass function internally calls second pass
 * 
 * @param argc Number of command-line arguments
 * @param argv Array of command-line argument strings (input filenames)
 * @return int Returns TRUE if any errors occurred during compilation, FALSE otherwise
 */
int main(int argc, char *argv[]);

/* Constants */
#define REQ_ARGUMENT_COUNT 2 /* 2, one for "./assembler" another for 1 file */

#endif /* ASSEMBLER_H */