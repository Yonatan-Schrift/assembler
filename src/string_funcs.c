#include <stdlib.h>
#include <string.h>
#include "../h/string_funcs.h"

char *copy_string(const char *origin)
{
    /* Allocate memory for new string (length + null terminator) */
    char *copy = malloc(strlen(origin) + 1);
    if (!copy)
    {
        /* printf("Failed memory allocation\n"); */
        return NULL; /* Memory allocation failed */
    }
    /* Copy the string contents to new memory */
    strcpy(copy, origin);

    return copy;
}