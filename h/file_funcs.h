#ifndef FILE_FUNCS_H
#define FILE_FUNCS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER_SIZE 256
#define WRITE_MODE "wb"
#define READ_MODE "rb"


FILE *open_file(const char *filename, const char *extension, const char *mode);

char *change_extension(const char *filename, const char *extension);

char *find_extension(const char *filename);

void close_mult_files(FILE *file1, FILE *file2, FILE *file3, FILE *file4, FILE *file5, FILE *file6);

#endif /* FILE_FUNCS_H */
