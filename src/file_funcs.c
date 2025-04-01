
#include "../h/file_funcs.h"
#include "../h/globals.h"

FILE *open_file(const char *filename, const char *extension, const char *mode) {
	char *file_name_with_ext = change_extension(filename, extension);

	FILE *file = fopen(file_name_with_ext, mode);
	free(file_name_with_ext);

	/* checks that the file opened */
	if (!file) {
		if (find_extension(filename)) {
			printerror(NO_LINE, FILE_INCLUDES_EXTENSION);
		} else {
			printerror(NO_LINE, COULD_NOT_OPEN_FILE);
		}
		return NULL;
	}

	return file;
}

/**
 * @brief Changes the extension of a filename.
 *
 * This function takes a filename and adds the given extension to the filename
 *
 * @param filename The original filename whose extension needs to be changed
 * @param extension The new extension
 * @return A pointer to a newly allocated string containing the filename with the new extension.
 *         The caller is responsible for freeing this memory.
 */
char *change_extension(const char *filename, const char *extension) {
	char *new_filename;
	size_t len;

	if (!filename || !extension) return NULL;

	/* Determine the length of the original filename + the extension*/
	len = strlen(filename) + strlen(extension);

	/* Allocate memory for the new filename + 1 for null terminator. */
	new_filename = (char *)malloc(len + 1);
	if (new_filename == NULL) return NULL;

	strcpy(new_filename, filename);

	/* Append the new extension */
	strcat(new_filename, extension);

	return new_filename;
}

char *find_extension(const char *filename) {
	char *ext;

	/* Find the last occurrence of '.' in the filename */
	ext = strrchr(filename, '.');

	if (ext != NULL && *(ext + 1) != '\0') {
		ext++;
	} else {
		/* No extension found */
		ext = NULL;
	}

	return ext;
}

void close_mult_files(FILE *file1, FILE *file2, FILE *file3, FILE *file4) {
	if (file1 != NULL) {
		fclose(file1);
	}
	if (file2 != NULL) {
		fclose(file2);
	}
	if (file3 != NULL) {
		fclose(file3);
	}
	if (file4 != NULL) {
		fclose(file4);
	}
}

void delete_mult_files(char *base_file_path, char *ext1, char *ext2, char *ext3) {
	char *file_name_with_ext;

	if (!base_file_path) {
		return;
	}

	if (ext1) {
		file_name_with_ext = change_extension(base_file_path, ext1);
		if (file_name_with_ext != NULL) {
			remove(file_name_with_ext);
			free(file_name_with_ext);
		}
	}

	if (ext2) {
		file_name_with_ext = change_extension(base_file_path, ext2);
		if (file_name_with_ext != NULL) {
			remove(file_name_with_ext);
			free(file_name_with_ext);
		}
	}

	if (ext3) {
		file_name_with_ext = change_extension(base_file_path, ext3);
		if (file_name_with_ext != NULL) {
			remove(file_name_with_ext);
			free(file_name_with_ext);
		}
	}
}

int is_file_empty(FILE *fp) {
    long size;

    if (fp == NULL) {
        return COULD_NOT_OPEN_FILE;  /* Could not open file */
    }

    if (fseek(fp, 0, SEEK_END) != 0) {
        fclose(fp);
        return COULD_NOT_OPEN_FILE;  /* Error while seeking */
    }

    size = ftell(fp);

    return (size == 0);
}

void delete_if_empty(FILE *fp, char *src_path, char *extension) {
    if (fp == NULL || src_path == NULL || extension == NULL) {
        return;
    }

    if (is_file_empty(fp)) {
        delete_mult_files(src_path, extension, NULL, NULL);
    }
}