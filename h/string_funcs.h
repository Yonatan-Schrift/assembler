#ifndef STRING_FUNCS_H
#define STRING_FUNCS_H

#define ARRAY_SIZE(x) (sizeof(x) / sizeof((x)[0]))

/**
 * @brief Creates a new copy of a string in dynamically allocated memory
 * 
 * @param origin The source string to copy
 * @return char* Pointer to the newly allocated copy of the string, or NULL if allocation fails
 * 
 * This function allocates new memory and copies the contents of the origin string.
 * The caller is responsible for freeing the returned memory when no longer needed.
 * 
 * (This method is copied from Maman22 by Yonatan Schrift)
 */
char *copy_string(const char *origin);

void remove_after_delim(char *origin, char delim);

/**
 * @brief Calculates the length of a NULL-terminated array of strings
 * 
 * @param args Pointer to array of string pointers (char*), must be NULL-terminated
 * @return Number of strings in array before NULL terminator, or FAILED_CODE if args is NULL
 */
int string_array_len(const char **args);

#endif /* STRING_FUNCS_H */