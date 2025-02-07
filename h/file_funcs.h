#ifndef FILE_FUNCS_H
#define FILE_FUNCS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER_SIZE 4096

char *copy_file(const char *src_path, char *extension);

char *change_extension(const char *filename, char *extension);


#endif /* FILE_FUNCS_H */