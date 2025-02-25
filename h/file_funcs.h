#ifndef FILE_FUNCS_H
#define FILE_FUNCS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER_SIZE 256
#define WRITE_MODE "wb"
#define READ_MODE "rb"


FILE *open_file(const char *filename, const char *extension, const char *mode);

char *copy_file(const char *src_path, char *extension);

char *change_extension(const char *filename, const char *extension);

char *find_extension(const char *filename);

int delete_line(const char *filename, const char *search_text);

int find_line(const char *filename, const char *search_text);

int insert_text_at_line(const char *filename, const char *search_text, const char *text);

#endif /* FILE_FUNCS_H */
