#ifndef ARGUMENT_FUNCS_H
#define ARGUMENT_FUNCS_H

/**
 * Cleans a string argument by removing all whitespace characters.
 *
 * @param arg The input string to be cleaned.
 *
 * @return A newly allocated string with all whitespace characters removed,
 *         or NULL if:
 *         - Input is NULL
 *         - Input is empty string
 *         - Memory allocation fails
 *
 * @note The caller is responsible for freeing the returned string.
 */
char *clean_arg(char *arg);

#endif /* ARGUMENT_FUNCS_H */