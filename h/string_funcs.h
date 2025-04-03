#ifndef STRING_FUNCS_H
#define STRING_FUNCS_H

#define ARRAY_SIZE(x) (sizeof(x) / sizeof((x)[0]))

/**
 * @brief Creates a duplicate of the given string.
 * @note Allocates new memory for the copy; returns NULL if the input is NULL or if memory allocation fails.
 *
 * @param origin The original string to copy.
 * @return A pointer to the newly allocated string, or NULL on error.
 */
char *copy_string(const char *origin);

/**
 * @brief Removes the first occurrence of a specified delimiter and truncates the string at that point.
 * @note Searches for the delimiter in the given string and, if found, replaces it with a null terminator,
 *       effectively removing the delimiter and any characters that follow.
 *
 * @param origin The string to process.
 * @param delim  The delimiter character.
 */
void remove_after_delim(char *origin, char delim);

/**
 * @brief Calculates the length of a null-terminated array of strings.
 * @note Returns the number of strings in the array, or EXIT_FAILURE if the array is NULL.
 *
 * @param args The null-terminated array of strings.
 * @return The count of strings, or EXIT_FAILURE on error.
 */
int string_array_len(char **args);

/**
 * @brief Removes the leading and trailing double quotes from a string.
 *
 * @param string The string to process.
 */
void remove_quotes(char *string);

/**
 * @brief Finds and validates quotation marks in a string.
 * @note Returns the index of the first quote if exactly two quotes are present and valid.
 *       Otherwise, returns an error code indicating the type of quotation issue.
 *
 * @param string The string to search.
 * @return Index of the first quote if valid; otherwise, an error code.
 */
int find_quotes(char *string);

/**
 * @brief Checks for misplaced commas in a parameter string.
 * @note Returns an error if a comma appears at the start or end of the string.
 *
 * @param string The string to check.
 * @return EXIT_SUCCESS if commas are correctly placed; error code otherwise.
 */
int check_for_commas(char *string);

/**
 * @brief Validates whether a string represents a valid integer number.
 * @note Accepts optional leading minus for negatives and ensures all other characters are digits.
 *
 * @param string The input string to validate.
 * @return EXIT_SUCCESS if valid; error code otherwise.
 */
int check_valid_number(char *string);

/**
 * @brief Removes all whitespace characters from an argument string.
 * @note Allocates and returns a new string containing only non-space characters.
 *       Returns NULL if the input is NULL, empty, or memory allocation fails.
 *
 * @param arg The input string to clean.
 * @return A newly allocated cleaned string, or NULL on error.
 */
char *clean_arg(char *arg);

/**
 * @brief Finds the last occurrence of a substring within a string.
 * @note Returns a pointer to the last occurrence of `needle` in `haystack`, or NULL if not found.
 *       If `needle` is an empty string, returns the beginning of `haystack`.
 *
 * @param haystack The string to search in.
 * @param needle   The substring to find.
 * @return Pointer to the last occurrence of `needle` in `haystack`, or NULL if not found.
 */
char *find_last_occurrence(const char *haystack, const char *needle);

#endif /* STRING_FUNCS_H */