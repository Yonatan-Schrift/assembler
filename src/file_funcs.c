
#include "../h/file_funcs.h"
#include "../h/globals.h"


char *copy_file(const char *src_path, char *extension) {
	FILE *src, *dest;
	char *dest_path;
	char buffer[BUFFER_SIZE];
	size_t bytesRead;

	dest_path = change_extension(src_path, extension);

	/* Open the source file in binary read mode */
	src = fopen(src_path, "rb");
	if (src == NULL) {
		perror("Error opening source file");
		free(dest_path);
		return NULL;
	}

	/* Open the destination file in binary write mode */
	dest = fopen(dest_path, "wb");
	if (dest == NULL) {
		perror("Error opening destination file");
		free(dest_path);
		fclose(src);
		return NULL;
	}

	/* Read from the source file and write to the destination file */
	while (TRUE) {
		bytesRead = fread(buffer, 1, BUFFER_SIZE, src);

		/* If fewer bytes than requested are returned, check if it’s due to an error */
		if (bytesRead < BUFFER_SIZE && ferror(src)) {
			perror("Error reading from source file");
			fclose(src);
			fclose(dest);
			free(dest_path);
			return NULL;
		}

		if (bytesRead > 0) {
			if (fwrite(buffer, 1, bytesRead, dest) != bytesRead) {
				perror("Error writing to destination file");
				fclose(src);
				fclose(dest);
				free(dest_path);
				return NULL;
			}
		}

		/* If we reached end-of-file, break out of the loop */
		if (feof(src)) {
			break;
		}
	}

	fclose(src);
	fclose(dest);
	return dest_path;
}

char *change_extension(const char *filename, char *extension) {
	char *new_filename;
	char *dot;
	size_t len;
	size_t base_len;

	if(!filename || !extension)
	{
		return NULL;
	}

	/* Determine the length of the original filename */
	len = strlen(filename) + strlen(extension);
	
	/* Allocate memory for the new filename.
	 * In the worst-case, there is no extension, so we need enough space for the original
	 * filename plus the new extension ".am" (3 characters) and the null terminator.
	 */
	new_filename = (char *)malloc(len + 1); /* +1 for '\0' */
	if (new_filename == NULL) {
		return NULL; /* Allocation failed */
	}

	/* Find the last occurrence of '.' to locate the extension */
	dot = strrchr(filename, '.');
	if (dot != NULL) {
		/* Calculate the length of the base name (without extension) */
		base_len = dot - filename;
		/* Copy the base name into new_filename */
		strncpy(new_filename, filename, base_len);
		new_filename[base_len] = '\0'; /* Ensure null termination */
	} else {
		/* If no '.' is found, copy the entire filename */
		strcpy(new_filename, filename);
	}

	/* Append the new extension ".am" */
	strcat(new_filename, extension);

	return new_filename;
}
