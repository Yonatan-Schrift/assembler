#ifndef STRING_FUNCS_H
#define STRING_FUNCS_H

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

#endif /* STRING_FUNCS_H */