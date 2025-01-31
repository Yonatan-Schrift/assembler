/** yes, it's perfectly normal to create a whole diferent file for reading one line.  */


#include "../h/globals.h"
#include "../h/line.h"

int readLine(FILE *file, char *buffer) {
    int count = 0;
    int ch;

    if (ferror(file) || feof(file))
    {
        return FAIL_CODE;
    }

    /* Read characters until newline, EOF, or max length */
    while ((ch = fgetc(file)) != EOF) {
        if (ch == '\n') {
            break;
        }
        if (count >= MAX_LINE_LENGTH) {
            break;
        }
        buffer[count++] = (char)ch;
    }
    
    /* Null terminate the string */
    buffer[count] = '\0';

    return count;
}