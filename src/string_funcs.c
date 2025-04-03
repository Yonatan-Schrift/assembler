#include "../h/string_funcs.h"
#include "../h/error.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *copy_string(const char *origin) {
	char *copy;
	if (!origin) return NULL;

	/* Allocate memory for new string (length + null terminator) */
	copy = malloc(strlen(origin) + 1);
	if (!copy) {
		/* Memory allocation failed */
		return NULL;
	}
	/* Copy the string contents to new memory */
	strcpy(copy, origin);
	return copy;
}

void remove_quotes(char *string) {
	int len;
	/* Check if string is valid */
	if (!string) return;

	len = strlen(string);
	/* Check if string has at least opening and closing quotes */
	if (len >= 2 && string[0] == '"' && string[len - 1] == '"') {
		/* Shift all characters left by one position */
		memmove(string, string + 1, len - 2);
		/* Null terminate the string after removing quotes */
		string[len - 2] = '\0';
	}
}

void remove_after_delim(char *origin, char delim) {
	char *pos;
	pos = strchr(origin, delim);
	if (pos != NULL) {
		*pos = '\0';
	}
}

int string_array_len(char **args) {
	int count;
	if (args == NULL) return EXIT_FAILURE;

	/* Count strings until NULL terminator is found */
	for (count = 0; args[count] != NULL; count++);

	return count;
}

int check_for_commas(char *string) {
	if (!string)
		return FAIL_CODE;

	/* Check if the string ends with a comma */
	if (*(string + 1)) {
		if (string[strlen(string) - 1] == ',') {
			return EXTRA_COMMA_AFTER_PARAM;
		}
	}

	/* Check if the string starts with a comma */
	if (*(string - 1)) {
		if (*string == ',') {
			return EXTRA_COMMA_BEFORE_PARAM;
		}
	}

	return EXIT_SUCCESS;
}

int check_valid_number(char *string) {
	int i;
	char *copy;

	/* Check if string is valid */
	if (!string) return FAIL_CODE;

	/* Check if string is empty */
	if (string[0] == '\0') return FAIL_CODE;

	copy = clean_arg(string);

	if (*copy == '\0') {
		free(copy);
		return EXTRA_COMMA_AFTER_PARAM;
	}

	/* Check if first character is minus sign for negative numbers */
	i = (copy[0] == '-') ? 1 : 0;

	/* Iterate through string and check if each character is a digit */
	for (; copy[i] != '\0'; i++) {
		if ((copy[i] < '0' || copy[i] > '9')) {
			free(copy);
			return UNALLOWED_CHARS_IN_DATA;
		}
	}

	free(copy);

	return EXIT_SUCCESS;
}

int find_quotes(char *string) {
	int i, ret = -1, count = 0;

	/* Check if the input string is valid */
	if (!string) return FAIL_CODE;

	/* Search for quotation marks and track their count and position */
	for (i = 0; string[i] != '\0'; i++) {
		if (string[i] == '"') {
			count++;
			if (ret == -1)
				ret = i; /* Save the index of the first quote */
		}
	}

	/* Validate quote usage */
	if (count == 2) {
		/* Exactly two quotes */
		if (string[i - 1] == '"')
			return ret;
		return EXTRA_TEXT_AFTER_STRING;
	} else if (count > 2) {
		/* More than two quotes indicates extra marks */
		return EXTRA_QUOTATION_MARK;
	} else if (count == 1) {
		/* One quote: check if it's opening or closing */
		return (string[i - 1] == '"') ? MISSING_OPENING_QUOTE : MISSING_CLOSING_QUOTE;
	}

	/* No quotes found */
	return MISSING_ARGS;
}

char *find_last_occurrence(const char *haystack, const char *needle) {
	char *result = NULL;
	char *temp = (char *)haystack;

	/* Edge case: if needle is an empty string, return haystack */
	if (*needle == '\0') {
		return (char *)haystack;
	}

	while ((temp = strstr(temp, needle)) != NULL) {
		result = temp;
		temp++; /* Advance pointer to search for further occurrences */
	}
	return result;
}

char *clean_arg(char *arg) {
	char *output = NULL;
	int i, j;
	int len;

	if (arg == NULL || *arg == '\0') {
		return NULL; /* Return NULL if the input is null or an empty string */
	}

	len = strlen(arg);

	/* Allocating memory with the length of arg. */
	output = malloc(len + 1); /* +1 for the null terminator */
	if (!output) {
		printf("Failed memory allocation\n");
		return NULL; /* Memory allocation failed */
	}

	/* Iterate through the input string and copy only non-whitespace characters */
	for (i = 0, j = 0; i < len; i++) {
		if (!isspace(arg[i])) {
			output[j] = arg[i];
			j++;
		}
	}
	output[j] = '\0'; /* Null-terminate the output string */
	return output;
}