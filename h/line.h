#ifndef LINE_H
#define LINE_H

#include <stdio.h>

/**
 * Reads a single line from a file into a buffer
 * @param file The file pointer to read from
 * @param buffer The buffer to store the line in
 * @return Number of characters read, or FAIL_CODE if reading failed
 */
int readLine(FILE *file, char *buffer);

#endif /* LINE_H */