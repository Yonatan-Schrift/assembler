#ifndef FILE_FUNCS_H
#define FILE_FUNCS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER_SIZE 256
#define WRITE_MODE "w"
#define READ_MODE "r"

/**
 * @brief Opens a file with the specified extension and mode.
 *
 * This function takes a filename, appends the specified extension to it if not already present,
 * and attempts to open the file with the given mode. If the file cannot be opened, appropriate
 * error messages are displayed.
 *
 * @param filename The name of the file to open (without extension)
 * @param extension The extension to append to the filename
 * @param mode The mode for opening the file (e.g., "r", "w", "a", etc.)
 *
 * @return A pointer to the opened FILE, or NULL if the file could not be opened
 * 
 * @note The function will print error messages if:
 *       - The file cannot be opened
 *       - The filename already includes an extension
 */
FILE *open_file(const char *filename, const char *extension, const char *mode);

/**
 * @brief Appends an extension to a filename.
 * 
 * This function takes a filename and an extension string and creates a new string
 * by appending the extension to the filename. The original strings are not modified.
 * 
 * @param filename The original filename to which the extension will be appended.
 * @param extension The extension to append to the filename.
 * 
 * @return A pointer to a newly allocated string containing the filename with the
 *         extension appended. The caller is responsible for freeing this memory.
 *         Returns NULL if either input parameter is NULL or if memory allocation fails.
 * 
 * @note This function does not replace an existing extension; it simply appends
 *       the provided extension to the original filename.
 */
char *change_extension(const char *filename, const char *extension);

/**
 * @brief Extracts the file extension from a filename
 *
 * This function finds the extension part of a filename by locating
 * the last occurrence of a period (.) character and returning the
 * text that follows it.
 *
 * @param filename The file name to extract the extension from
 * @return A pointer to the extension (without the dot) if found,
 *         or NULL if no valid extension exists (no dot or nothing after dot)
 */
char *find_extension(const char *filename);

/**
 * Closes multiple file pointers safely.
 * 
 * This function takes up to four FILE pointers and closes each one
 * if it is not NULL. This helps prevent errors from attempting to
 * close NULL file pointers.
 *
 * @param file1 First FILE pointer to close
 * @param file2 Second FILE pointer to close
 * @param file3 Third FILE pointer to close
 * @param file4 Fourth FILE pointer to close
 */
void close_mult_files(FILE *file1, FILE *file2, FILE *file3, FILE *file4);

/**
 * @brief Deletes multiple files with the same base name but different extensions.
 *
 * @param base_file_path The path to the file without extension
 * @param ext1 First extension to try (can be NULL to skip)
 * @param ext2 Second extension to try (can be NULL to skip)
 * @param ext3 Third extension to try (can be NULL to skip)
 * 
 * @note If base_file_path is NULL, the function returns without any action.
 */
void delete_mult_files(char *base_file_path, char *ext1, char *ext2, char *ext3);

/**
 * Checks if a file is empty by seeking to the end and checking the position.
 * 
 * @param fp The file pointer to check. It must be open in a mode that allows seeking.
 * 
 * @return 1 (true) if the file is empty, 0 (false) if it is not empty,
 *         or COULD_NOT_OPEN_FILE if an error occurred (e.g., NULL file pointer or seek error).
 * 
 * @note If a seek error occurs, this function will close the file before returning.
 */
int is_file_empty(FILE *fp);

/**
 * @brief Deletes a file if it is empty
 * 
 * This function checks if the given file is empty. If it is empty, it deletes the file.
 * 
 * @param fp File pointer to check if empty
 * @param src_path Source path where the file is located
 * @param extension File extension of the file to delete
 * 
 * @note The function does nothing if any of the parameters is NULL
 */
void delete_if_empty(FILE *fp, char *src_path, char *extension);

#endif /* FILE_FUNCS_H */
