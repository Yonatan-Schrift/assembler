#include "../h/first_pass.h"
#include "../h/hashmap.h"
#include "../h/line.h"
#include "../h/globals.h"

int first_pass (char *src_path){
    char *line, *new_path;
    FILE *file;
    int is_symbol = FALSE;

	file = fopen(new_path, "r");
	if (file == NULL)
    {
		free_hashmap(symbolTable);
		free(new_path);
		return FAIL_CODE;
	}

    init_hashmap(symbolTable, TABLE_SIZE);

    IC = 100;
    DC = 0;

    while (line = read_line(file) != NULL)
    {
        Line *parsed_line = split_line(line);
        if (parsed_line != NULL)
        {
            if (parsed_line -> label != NULL)
            {
                is_symbol = TRUE;
            }

            if (strcmp(parsed_line -> command, ".data") == STRCMP_SUCCESS || strcmp(parsed_line -> command, ".string") == STRCMP_SUCCESS)
            {
                if (is_symbol){
                    if(findSymbol(parsed_line -> label) == NOT_IN_TABLE)
                    {
                        insertSymbol(parsed_line -> label, DC, DATA);
                    }
                    else
                    {
                        printerror("Error: Symbol already exists");
                        return FAIL_CODE;
                    }
                }
                
            }
              
              
         }
    }
    

}
