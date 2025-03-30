#ifndef FILE_FUNCS_H
#define FILE_FUNCS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER_SIZE 256
#define WRITE_MODE "w"
#define READ_MODE "r"


FILE *open_file(const char *filename, const char *extension, const char *mode);

char *change_extension(const char *filename, const char *extension);

char *find_extension(const char *filename);

void close_mult_files(FILE *file1, FILE *file2, FILE *file3, FILE *file4);


void delete_mult_files(char *base_file_path, char *ext1, char *ext2, char *ext3);

int is_file_empty(FILE *fp);

void delete_if_empty(FILE *fp, char *src_path, char *extension);
#endif /* FILE_FUNCS_H */
