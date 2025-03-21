
#include "../h/file_funcs.h"
#include "../h/globals.h"

FILE *open_file(const char *filename, const char *extension, const char *mode) {
	char *file_name_with_ext = change_extension(filename, extension);

	FILE *file = fopen(file_name_with_ext, mode);
	free(file_name_with_ext);

	/* checks that the file opened */
	if (!file) {
		if (find_extension(filename)) {
			printerror("error", NO_LINE, FILE_INCLUDES_EXTENSION);
		} else {
			printerror("failed to open the file\n", NO_LINE, EXIT_FAILURE);
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

void close_mult_files(FILE *file1, FILE *file2, FILE *file3, FILE *file4, FILE *file5, FILE *file6) {
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
	if (file5 != NULL) {
		fclose(file5);
	}
	if (file6 != NULL) {
		fclose(file6);
	}
}
