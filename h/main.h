#ifndef MAIN_H
#define MAIN_H

#define AS_EXTENSION "as"

/**
 * @brief Entry point for the assembler.
 *
 * This function processes command line arguments, opens the file,
 * and initiates the pre-assembly phase.
 *
 * @param argc The number of command line arguments.
 * @param argv Array of command line arguments.
 * @return int Returns SUCCESS_CODE on success or FAIL_CODE on failure.
 */
int main(int argc, char *argv[]);

#endif // MAIN_H