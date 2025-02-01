/** yes, it's perfectly normal to create a whole diferent file for reading one line.  */


#include "../h/globals.h"
#include "../h/line.h"
#include <stdlib.h>

/**
 * Reads a line from standard input character by character.
 * Dynamically allocates memory for the line string, doubling the buffer size when needed.
 *
 * @return A pointer to the dynamically allocated string containing the line,
 *         or NULL if:
 *         - Memory allocation fails
 *         - EOF is encountered with no input
 *
 * @note The returned string must be freed by the caller when no longer needed
 * Code taken from maman22 by YonatanSchrift
 */
char *read_line(void)
{
    char cur;
    char *buffer, *line;
    size_t capacity, index;

    /* Initialize variables:
    * capacity: Initial size of the dynamic buffer.
    * index: Tracks the current position in the buffer.
    */
    capacity = 8; /* starting size for the line */
    index = 0;

    /* Allocating initial memory */
    line = (char *)malloc(capacity);
    if (!line)
    {
        fprintf(stderr, "Failed memory allocation\n");
        return NULL; /* Memory Allocation Error */
    }

    /* Read input character by character */
    while ((cur = getchar()) != EOF && cur != '\n')
    {
        if (index >= capacity - 1)
        {
            /* Buffer is too small, double the capacity */
            buffer = realloc(line, capacity * 2);
            if (!buffer)
            {
                free(line);
                return NULL; /* Memory Allocation Error */
            }
            line = buffer;
            capacity *= 2;
        }
        line[index++] = cur;
    }

    /* Handle EOF when no input is given */
    if (index == 0 && cur == EOF)
    {
        free(line);
        return NULL;
    }

    /* Null-terminate the string */
    line[index] = '\0';
    return line;
}

