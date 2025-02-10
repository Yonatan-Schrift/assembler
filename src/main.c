/**
 * Assembler
 * 
 * @file main.c
 * @brief currently for testing I ain't sure what it's gonna do
 * 
 * @author Yonatan Schrift
 * @date Created 31.01.2025
 */

#include <stdio.h>
#include "../h/file_funcs.h"
#include "../h/globals.h"
#include "../h/main.h"
#include "../h/pre_assembler.h"

#define REQ_ARGUMENT_COUNT 2

/**
 * This method opens a file and reads it line by line.asm
 * Do notice that each line has a max of 80 characters + 1 for null terminator.
 * 
*/
int main(int argc, char *argv[]) {
    FILE *file;

    /* Checks if recieved the correct amount of arguments (only 1, the filename) */
    if (argc > REQ_ARGUMENT_COUNT) {
        printf("Too many arguments provided\n");
        return FAIL_CODE;
    }
    else if(argc < REQ_ARGUMENT_COUNT) {
        printf("No text file provided.\n");
        return FAIL_CODE;
    }
    if(strcmp(find_extension(argv[1]), AS_EXTENSION) != STRCMP_SUCCESS) {
        printf("Not a .as file.\n");
        return FAIL_CODE;
    }
    
    /* Try to open the file in read mode */
    file = fopen(argv[1], "r"); 
    if (file == NULL) {
        printf("File does not exist or cannot be opened.\n");
        return FAIL_CODE;
    }

    pre_comp(argv[1]);
    
    /* Close the file */
    fclose(file); 

    return SUCCESS_CODE;
}
