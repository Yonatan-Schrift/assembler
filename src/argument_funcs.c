
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

/**
 * Cleans a string argument by removing all whitespace characters.
 *
 * @param arg The input string to be cleaned.
 *
 * @return A newly allocated string with all whitespace characters removed,
 *         or NULL if:
 *         - Input is NULL
 *         - Input is empty string
 *         - Memory allocation fails
 *
 * @note The caller is responsible for freeing the returned string.
 */
char *clean_arg( char *arg)
{
    char *output = NULL;
    int i , j;
    int len;

    if (arg == NULL || *arg == '\0')
    {
        return NULL; /* Return NULL if the input is null or an empty string */
    }

    len = strlen(arg);
    
    /* Allocating memory with the length of arg. */
    output = malloc(len + 1); /* +1 for the null terminator */
    if (!output)
    {
        printf("Failed memory allocation\n");
        return NULL; /* Memory allocation failed */
    }

    /* Iterate through the input string and copy only non-whitespace characters */
    for (i = 0, j = 0; i < len; i++)
    {
        if (!isspace(arg[i]))
        {
            output[j] = arg[i];
            j++;
        }
    }
    output[j] = '\0'; /* Null-terminate the output string */
    return output;
}