
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

int delete_line(const char *filename, const char *search_text) {
    FILE *originalFile, *tempFile;
    char buffer[MAX_LINE_LENGTH];
	int line_to_delete = find_line(filename, search_text);
    int current_line = 1;
    const char *temp_filename = "temp.txt";

    /* Open the original file for reading */
    originalFile = fopen(filename, "r");
    if (originalFile == NULL) {
        perror("Error opening file for reading");
        return FAIL_CODE;
    }

    /* Open a temporary file for writing */
    tempFile = fopen(temp_filename, "w");
    if (tempFile == NULL) {
        perror("Error opening temporary file for writing");
        fclose(originalFile);
        return FAIL_CODE;
    }

    /* Read each line from the original file and write it to the temporary file,
       except for the line that should be deleted */
    while (fgets(buffer, MAX_LINE_LENGTH, originalFile) != NULL) {
        if (current_line != line_to_delete) {
            if (fputs(buffer, tempFile) == EOF) {
                perror("Error writing to temporary file");
                fclose(originalFile);
                fclose(tempFile);
                return FAIL_CODE;
            }
        }
        current_line++;
    }

    /* Close both files */
    fclose(originalFile);
    fclose(tempFile);

    /* Delete the original file */
    if (remove(filename) != 0) {
        perror("Error deleting original file");
        return FAIL_CODE;
    }

    /* Rename the temporary file to the original file name */
    if (rename(temp_filename, filename) != 0) {
        perror("Error renaming temporary file");
        return FAIL_CODE;
    }

    return SUCCESS_CODE;
}

int find_line(const char *filename, const char *search_text) {
    FILE *file;
    char buffer[MAX_LINE_LENGTH];
    int line_number = 0;

    file = fopen(filename, "r");
    if (file == NULL) {
        perror("Error opening file");
        return FAIL_CODE;
    }

    /* Read the file line by line */
    while (fgets(buffer, MAX_LINE_LENGTH, file) != NULL) {
        line_number++;

        if (strstr(buffer, search_text) != NULL) {
            fclose(file);
            return line_number;
        }
    }

    fclose(file);
    return FAIL_CODE;  /* search_text not found in the file */
}

int insert_text_at_line(const char *filename, const char *search_text, const char *text) {
    FILE *origFile, *tempFile;
    char buffer[MAX_LINE_LENGTH];
	int pos = find_line(filename, search_text);
    int current_line = 1;
    int inserted = 0;
    const char *temp_filename = "temp.txt";

    /* Open the original file for reading */
    origFile = fopen(filename, "r");
    if (origFile == NULL) {
        perror("Error opening original file");
        return FAIL_CODE;
    }

    /* Open a temporary file for writing */
    tempFile = fopen(temp_filename, "w");
    if (tempFile == NULL) {
        perror("Error opening temporary file");
        fclose(origFile);
        return FAIL_CODE;
    }

    /* Read and write each line from the original file */
    while (fgets(buffer, MAX_LINE_LENGTH, origFile) != NULL) {
        /* If we've reached the specified line and haven't inserted the text yet,
           insert the new text before writing the current line */
        if (current_line == pos && !inserted) {
            if (fputs(text, tempFile) == EOF) {
                perror("Error writing inserted text to temporary file");
                fclose(origFile);
                fclose(tempFile);
                return FAIL_CODE;
            }
            inserted = 1;
        }
        /* Write the current line to the temporary file */
        if (fputs(buffer, tempFile) == EOF) {
            perror("Error writing line to temporary file");
            fclose(origFile);
            fclose(tempFile);
            return FAIL_CODE;
        }
        current_line++;
    }

    /* If pos is beyond the end of the file, append the text now */
    if (!inserted) {
        if (fputs(text, tempFile) == EOF) {
            perror("Error appending inserted text to temporary file");
            fclose(origFile);
            fclose(tempFile);
            return FAIL_CODE;
        }
    }

    /* Close both files */
    fclose(origFile);
    fclose(tempFile);

    /* Remove the original file */
    if (remove(filename) != 0) {
        perror("Error removing original file");
        return FAIL_CODE;
    }

    /* Rename the temporary file to the original file name */
    if (rename(temp_filename, filename) != 0) {
        perror("Error renaming temporary file");
        return FAIL_CODE;
    }

    return SUCCESS_CODE;
}
