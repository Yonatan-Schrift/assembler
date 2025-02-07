#ifndef FILE_FUNCS_H
#define FILE_FUNCS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER_SIZE 4096

char *copy_file(const char *src_path, char *extension);

char *change_extension(const char *filename, char *extension);

int delete_line(const char *filename, const char *search_text);

int find_line(const char *filename, const char *search_text);

int insert_text_at_line(const char *filename, const char *search_text, const char *text);

#endif /* FILE_FUNCS_H */