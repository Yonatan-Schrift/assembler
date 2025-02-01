#ifndef LINE_H
#define LINE_H

#include <stdio.h>

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
 * 
 *          Code taken from maman22 by Yonatan Schrift
 */
char *read_line(void);

#endif /* LINE_H */