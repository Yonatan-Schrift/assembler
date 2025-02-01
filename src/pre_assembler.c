
#include "../h/pre_assembler.h"
#include "../h/hashmap.h"
#include "../h/globals.h"
#include "../h/line.h"


int pre_comp(FILE *file) {
    char line[MAX_LINE_LENGTH + 1];

    while (readLine(file, line) >= 0) {
        printf("Read line: %s\n", line);
    }
    



    return SUCCESS_CODE;
}

int save_macro(hashmap_t *map, Macro *macro) 
{
    insert(map, macro);
    return SUCCESS_CODE;
}






