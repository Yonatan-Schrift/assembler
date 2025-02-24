#include "first_pass.h"
#include "hashmap.h"
#include "line.h"
#include "globals.h"

void printerror(char *error) {
	printf("%s\n", error);
}

int first_pass (char *src_path){
    char *line, *new_path;
    FILE *file;

	file = fopen(new_path, "r");
	if (file == NULL) {
		free_hashmap(symbolTable);
		free(new_path);
		return FAIL_CODE;
	}

    init_hashmap(symbolTable, TABLE_SIZE);

    IC = 100;
    DC = 0;

    while (line = read_line(file) != NULL) {
       Line *parsed_line = split_line(line);
        
    }
    

}
