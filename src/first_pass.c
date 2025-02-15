#include "first_pass.h"
#include "hashmap.h"
#include "line.h"

int first_pass(char *file_path) {
    FILE *file = fopen(file_path, "r");
    if (!file) {
        printf(stderr, "Error: Cannot open file %s\n", file_path);
        return -1;
    }

    IC = 100;
    DC = 0;
    char *line = NULL; 
    int is_symbol = 0; 

    while ((line = read_line(file)) != NULL) {
        Line *parsed_line = split_line(line);
        if (!parsed_line) {
            free(line);
            continue;
        }

        if (parsed_line->label) {
            is_symbol = 1;
        }

        if (strcmp(parsed_line->command, ".data") == 0 || strcmp(parsed_line->command, ".string") == 0) {
            if (is_symbol) {
                if (findSymbol(parsed_line->label) != -1) {
                    fprintf(stderr, "Error: Symbol %s already exists\n", parsed_line->label);
                } else {
                    insertSymbol(parsed_line->label, DC, DATA);
                }
            }

            if (strcmp(parsed_line->command, ".data") == 0) {
                char *token = strtok(parsed_line->arguments[0], ",");
                while (token != NULL) {
                    // Convert token to integer and store it in memory
                    DC++;
                    token = strtok(NULL, ",");
                }
            } else if (strcmp(parsed_line->command, ".string") == 0) {
                char *str = parsed_line->arguments[0];
                for (int i = 0; str[i] != '\0'; i++) {
                    // Store each character in memory
                    DC++;
                }
                DC++;
            }

            free_line(parsed_line);
            free(line);
            continue;
        }

// I need to continue...